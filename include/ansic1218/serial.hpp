/**
 * Copyright 2021 Time Energy Industria e Comercio Ltda
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <chrono>
#include <vector>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "exception.hpp"

namespace ansic1218 {

class SerialException : public Exception {
public:
    template <typename... Args> explicit SerialException(Args &&...args) : Exception(__PRETTY_FUNCTION__, args...) {}
};

class Serial {
    uart_port_t uart_num;
    int uart_baud_rate;
    int tx_io_num;
    int rx_io_num;

public:
    enum class FlushInputBuffer : uint8_t
    {
        Disable = 0,
        Enable = 1
    } flushInputBuffer;

    Serial();

    ~Serial();

    void open(uart_port_t uart_num, int uart_baud_rate, int tx_io_num, int rx_io_num);

    void set_loop_back();

    virtual void write(const std::vector<uint8_t> &buffer);

    virtual size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout);

    virtual size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout,
                        FlushInputBuffer flushInputBuffer);

    virtual size_t read(std::vector<uint8_t> &buffer, std::vector<uint8_t> &pattern, size_t nBytesToRead,
                        std::chrono::nanoseconds timeout);

    virtual size_t read(std::vector<uint8_t> &buffer, std::vector<uint8_t> &pattern, size_t nBytesToRead,
                        std::chrono::nanoseconds timeout, FlushInputBuffer flushInputBuffer);
};
}    // namespace ansic1218
