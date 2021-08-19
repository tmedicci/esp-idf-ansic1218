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
#include <iomanip>
#include <sstream>
#include "read-full.hpp"
#include "../tables/table.hpp"

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

static const char *TAG = "ansic1218::services:read-full";

ReadFull::ReadFull(Table &table) : Service(__PRETTY_FUNCTION__ + table.name()), table(table) {}

bool ReadFull::request(std::vector<uint8_t> &buffer)
{
    static_assert(sizeof(Request) == 3, "");

    Request request{.type = FULL_READ, .table_id = htobe16(table.id())};

    auto *ptr = reinterpret_cast<uint8_t *>(&request);

    copy(&ptr[0], &ptr[sizeof(Request)], back_inserter(buffer));

    return true;
}

bool ReadFull::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    if (!Service::validate(first, last))
        return false;

    static_assert(sizeof(Response) == 3, "");

    auto *resp = (Response *)&*first;

    uint8_t chk = 0;
    resp->count = be16toh(resp->count);
    auto expected_size = resp->count + sizeof(Response) + sizeof(chk);

    if (distance(first, last) != expected_size) {
        ESP_LOGW(TAG, "Wrong response size, expected: %d, received: %d", int(expected_size), distance(first, last));
        return false;
    }

    first += sizeof(Response);
    chk = checksum(first, last - 1);

    if (*prev(last) != chk) {
        ESP_LOGW(TAG, "Invalid checksum, expected: %d, received: %d", int(chk), int(*prev(last)));
        return false;
    }

    return table.response(first, last - 1);
}
