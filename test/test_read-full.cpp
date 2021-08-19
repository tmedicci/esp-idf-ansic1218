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


#include "unity.h"

#include <algorithm>
#include <ansic1218/tables/table.hpp>
#include <ansic1218/services/read-full.hpp>

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

class MockTable : public Table
{
public:
    explicit MockTable(uint16_t id) : Table(id) {}

    bool validate(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last) override { return true; }
};

TEST_CASE("Should request and validate tables properly", "[ansic1218][services][ReadFull]")
{

    MockTable table(0x1234);
    ReadFull read_full(table);

    vector<uint8_t> expected_request = {0x30, 0x12, 0x34};
    vector<uint8_t> table_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    vector<uint8_t> response = {0x00, 0x00, uint8_t(table_data.size())};

    uint8_t checksum = 0;
    for_each(table_data.begin(), table_data.end(), [&checksum](uint8_t value)
             { checksum += value; });
    checksum = ~checksum;

    copy(table_data.begin(), table_data.end(), back_inserter(response));
    response.push_back(checksum);

    vector<uint8_t> request;
    read_full.request(request);

    TEST_ASSERT_TRUE(request == expected_request);
    TEST_ASSERT_TRUE(read_full.response(response.cbegin(), response.cend()));
    TEST_ASSERT_TRUE(table.data() == table_data);
}
