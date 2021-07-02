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

/* C++ exception handling example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <iostream>

extern "C" {
#include "driver/uart.h"
#include "esp_log.h"
}

#include "ansic1218/transport.h"
#include "ansic1218/services/identification.h"
#include "ansic1218/services/security.h"
#include "ansic1218/services/read-full.h"
#include "ansic1218/services/read-partial.h"
#include "ansic1218/services/write-full.h"
#include "ansic1218/services/write-partial.h"
#include "ansic1218/tables/table-01.h"

using std::cout;
using std::endl;
using std::runtime_error;

using namespace std;
using namespace ansic1218;
using namespace ansic1218::service;
using namespace ansic1218::table;

static const char *TAG = "meter_identification";

vector<uint8_t> HexToBytes(const char *hex_string)
{
    string hex(hex_string);
    vector<uint8_t> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

extern "C" void app_main(void)
{
    vector<uint8_t> identity{};
    vector<uint8_t> raw_pass = HexToBytes(CONFIG_ANSI_EXAMPLE_METER_PASSWORD);

    try
    {

        ansic1218::Transport transport(CONFIG_ANSI_EXAMPLE_UART_PORT_NUM,
                                       CONFIG_ANSI_EXAMPLE_UART_BAUD_RATE,
                                       CONFIG_ANSI_EXAMPLE_UART_TXD,
                                       CONFIG_ANSI_EXAMPLE_UART_RXD);

        Identification identification;

        if (!transport.request(identification))
        {
            ESP_LOGE("main", "não conseguiu requisitar uma identidade");
            return;
        }

        identity = identification.getDeviceIdentity();

        if (!transport.request(Security(identity, raw_pass)))
        {
            ESP_LOGE("main", "não conseguiu logar-se");
            return;
        }

        Table01 table01;

        if (!transport.request(ReadFull(table01)))
            return;

        auto info_01 = table01.content();
        string serial(info_01->mfg_serial_number, sizeof(info_01->mfg_serial_number));
        ostringstream ofw;
        ostringstream ohw;

        ofw << (int)info_01->fw_version_number << "." << (int)info_01->fw_revision_number;
        ohw << (int)info_01->hw_version_number << "." << (int)info_01->hw_revision_number;

        ESP_LOGI(TAG, "Meter firmware: %s", ofw.str().c_str());
        ESP_LOGI(TAG, "Meter hardware: %s", ohw.str().c_str());
        ESP_LOGI(TAG, "Meter serial: %s", serial.c_str());
    }
    catch (const runtime_error &e)
    {
        cout << "Exception caught: " << e.what() << endl;
    }

    cout << "app_main done" << endl;
}
