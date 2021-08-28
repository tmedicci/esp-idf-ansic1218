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
#include <iostream>
#include <sstream>
#include <string>
#include "esp_log.h"
#include "ansic1218/services/write-full.hpp"
#include "ansic1218/tables/table.hpp"
#include "utils/test-utils.hpp"
#include "unity.h"

using namespace std;
using namespace utils;
using namespace ansic1218::service;
using namespace ansic1218::table;

class MockTable : public Table {
    vector<uint8_t> _data;

public:
    explicit MockTable(uint16_t id) : Table(id) {}

    void setData(vector<uint8_t> __data) { _data = __data; }

    vector<uint8_t> &data() override { return _data; }
};

TEST_CASE("Should request and validate tables properly", "[ansic1218][services][WriteFulll]")
{
    Table table(55);
    WriteFull writeFull(table);

    vector<uint8_t> tableContent{19, 3, 19, 19, 4, 3, 0, 2, 0, 0};

    table.initializer(tableContent.cbegin(), tableContent.cend());

    vector<uint8_t> expected{64, 0, 55, 0, 10, 19, 3, 19, 19, 4, 3, 0, 2, 0, 0, 186};
    vector<uint8_t> request;

    writeFull.request(request);

    string message = "expected: " + bufToStr(expected.cbegin(), expected.cend()) +
                     "requested: " + bufToStr(request.cbegin(), request.cend()).c_str();

    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected.data(), request.data(), expected.size(), message.c_str());
}

/* This test case currently fails, and developer has added a tag to indicate this.
 * For the test runner, "[fails]" string does not carry any special meaning.
 * However it can be used to filter out tests when running.
 * The problem with this test relates to the child class (MockTable) inheritance
 * regardng the overriden '&data()' method: this method is able to return the
 * content of the '_data' vector when called by the local object 'table'. However,
 * when used by the external class 'WriteFull' in the 'request' method, the '&data()'
 * method does not return the actual '_data' vector. It calls the base class method
 * as it has not been overloaded (returning the base class 'raw_data' vector).
 */
TEST_CASE("Should fail to request and validate mocked table (overriden functions) with null offset",
          "[ansic1218][services][WriteFulll][fails]")
{
    MockTable table(55);
    WriteFull writeFull(table);

    vector<uint8_t> tableContent{19, 3, 19, 19, 4, 3, 0, 2, 0, 0};
    table.setData(tableContent);

    ESP_LOGI("[ansic1218][WriteFulll] Mocked table content:", "%s",
             (bufToStr(table.data().cbegin(), table.data().cend()).c_str()));
    ESP_LOGI("[ansic1218][WriteFulll] Mocked table address:", "%d", (uint32_t)table.data().data());
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(tableContent.data(), table.data().data(), tableContent.size(),
                                          bufToStr(table.data().cbegin(), table.data().cend()).c_str());
    TEST_ASSERT_EQUAL(tableContent.size(), table.data().size());

    vector<uint8_t> expected{0x4F, 0, 19, 0, 0, 0, 0, 10, 19, 3, 19, 19, 4, 3, 0, 2, 0, 0, 186};
    vector<uint8_t> request;

    TEST_ASSERT_TRUE(writeFull.request(request));

    string message = "expected: " + bufToStr(expected.cbegin(), expected.cend()) +
                     "requested: " + bufToStr(request.cbegin(), request.cend()).c_str();

    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(expected.data(), request.data(), expected.size(), message.c_str());
}
