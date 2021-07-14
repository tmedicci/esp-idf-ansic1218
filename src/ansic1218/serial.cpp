// Copyright 2021 Time Energy Industria e Comercio Ltda
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstring>
#include "serial.h"

extern "C"
{
#include "esp_err.h"
}

using namespace std;
using namespace chrono;
using namespace ansic1218;

static const int RX_BUF_SIZE = 1024;

Serial::Serial() : uart_num(-1) {}

Serial::~Serial()
{
    uart_driver_delete(uart_num);
}

void Serial::open(uart_port_t uart_num, int uart_baud_rate, int tx_io_num, int rx_io_num)
{
    esp_err_t ret;

    Serial::uart_num = uart_num;

    const uart_config_t uart_config = {
        .baud_rate = uart_baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB};

    ret = uart_driver_install(uart_num, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    if (ret != ESP_OK)
    {
        throw SerialException("Failed to install uart driver: ", esp_err_to_name(ret));
    }

    ret = uart_param_config(uart_num, &uart_config);
    if (ret != ESP_OK)
    {
        throw SerialException("Failed to set uart parameters: ", esp_err_to_name(ret));
    }

    ret = uart_set_pin(uart_num, tx_io_num, rx_io_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (ret != ESP_OK)
    {
        throw SerialException("Failed to set uart pin configuration: ", esp_err_to_name(ret));
    }
}

void Serial::set_loop_back()
{
    esp_err_t ret;

    ret = uart_set_loop_back(uart_num, true);
    if (ret != ESP_OK)
    {
        throw SerialException("Failed to set uart loop back: ", esp_err_to_name(ret));
    }
}

void Serial::write(const vector<uint8_t> &buffer)
{
    if (uart_num < 0)
    {
        throw SerialException("Failed to write to uart: uninitialized");
    }

    esp_err_t ret;

    ret = uart_write_bytes(uart_num, buffer.data(), buffer.size());

    if (ret < 0)
    {
        throw SerialException("Failed to write to uart: ", esp_err_to_name(ret));
    }
}

size_t Serial::read(vector<uint8_t> &buffer, size_t nBytesToRead, nanoseconds timeout)
{
    return read(buffer, nBytesToRead, timeout, FlushInputBuffer::Disable);
}

size_t Serial::read(vector<uint8_t> &buffer, size_t nBytesToRead, nanoseconds timeout, FlushInputBuffer flushInputBuffer)
{
    std::vector<uint8_t> empty;
    return read(buffer, empty, nBytesToRead, timeout, flushInputBuffer);
}

size_t Serial::read(vector<uint8_t> &buffer, vector<uint8_t> &pattern, size_t nBytesToRead, nanoseconds timeout)
{
    return read(buffer, pattern, nBytesToRead, timeout, FlushInputBuffer::Disable);
}

size_t Serial::read(vector<uint8_t> &buffer, vector<uint8_t> &pattern, size_t nBytesToRead, nanoseconds timeout, FlushInputBuffer flushInputBuffer)
{
    if (uart_num < 0)
    {
        throw SerialException("Failed to write to uart: uninitialized");
    }

    size_t totalRead = 0;
    bool patternFound = false;

    // make space for the new data
    buffer.resize(buffer.size() + nBytesToRead);

    // point to the beginning of new data
    auto ptr = buffer.data() + buffer.size() - nBytesToRead;
    auto beginOfReadData = buffer.begin() + buffer.size() - nBytesToRead;

    auto wait_until = timeout + steady_clock::now();

    while (totalRead < nBytesToRead)
    {

        if (steady_clock::now() > wait_until)
            break;

        int counter = 0;

        counter = uart_read_bytes(uart_num, &ptr[totalRead], nBytesToRead - totalRead, 100 / portTICK_RATE_MS);

        if (counter < 0)
        {
            throw SerialException("Failed to read from uart: ", esp_err_to_name(counter));
        }

        totalRead += counter;

        //check for matching pattern vector into buffer
        if (!pattern.empty())
        {
            auto res = search(beginOfReadData, end(buffer), begin(pattern), end(pattern));
            if (res != end(buffer))
            {
                totalRead = distance(beginOfReadData, res + pattern.size());
                patternFound = true;
                break;
            }
        }
    }
    // flush input buffer
    if (flushInputBuffer == FlushInputBuffer::Enable)
    {
        esp_err_t ret;

        ret = uart_flush(uart_num);

        if (ret < 0)
        {
            throw SerialException("Failed to flush uart: ", esp_err_to_name(ret));
        }
    }

    // remove missing items
    buffer.resize(buffer.size() - (nBytesToRead - totalRead));

    return !pattern.empty() ? (patternFound ? totalRead : -totalRead) : totalRead;
}
