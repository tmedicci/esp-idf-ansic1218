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


#include <cassert>
#include "identification.hpp"

using namespace std;
using namespace ansic1218::service;

static const char *TAG = "ansic1218::services:identification";

struct Identification::Response {
    enum
    {
        DEVICE_CLASS = 0x06,
        DEVICE_IDENTITY = 0x07,
        END_OF_LIST = 0x00
    };

    uint8_t code;
    uint8_t std;
    uint8_t ver;
    uint8_t rev;
    uint8_t feature;

} __attribute__((__packed__));

static_assert(sizeof(Identification::Response) == 5, "Response header size according to specification");

struct Identification::DeviceIdentity {
    uint8_t type;
    uint8_t length;
    uint8_t char_format;
    uint8_t identification[];
};

Identification::Identification() : Service(__PRETTY_FUNCTION__) {}

bool Identification::request(std::vector<uint8_t> &buffer)
{
    buffer.push_back(0x20);
    return true;
}

bool Identification::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    if (!Service::validate(first, last))
        return false;

    auto length = distance(first, last);

    if (length < sizeof(Response)) {
        ESP_LOGW(TAG, "Wrong package size, expected at least: %d bytes. received: %d.", sizeof(Response), length);
        return false;
    }

    if (*prev(last) != Response::END_OF_LIST) {
        ESP_LOGW(TAG, "End of list is expected at the end of the package but %d was found.", int(*prev(last)));
        return false;
    }

    content.reserve(length);
    content.clear();

    copy(first, last, back_inserter(content));

    return true;
}

vector<uint8_t> Identification::getDeviceIdentity()
{
    vector<uint8_t> result;

    if (content.empty()) {
        ESP_LOGI(TAG, "Response is empty - no device identity");
        return result;
    }

    auto *resp = reinterpret_cast<Response *>(content.data());

    for (auto *feature = &resp->feature; *feature != Response::END_OF_LIST;) {
        if (*feature == Response::DEVICE_IDENTITY) {
            auto *identity = reinterpret_cast<DeviceIdentity *>(feature);

            for (auto i = 0; i < identity->length; i++) {
                result.push_back(identity->identification[i]);
            }

            return result;
        } else {
            assert(false && "Not implemented.");
        }
    }

    ESP_LOGI(TAG, "Feature 'device identity' not found");
    return result;
}
