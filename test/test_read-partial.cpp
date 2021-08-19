
#include <algorithm>
#include <numeric>
#include "ansic1218/services/read-partial.hpp"
#include "ansic1218/tables/table.hpp"
#include "unity.h"

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

class MockTable : public Table {
    vector<uint8_t> _data;

public:
    explicit MockTable(uint16_t id, uint32_t offset, uint16_t count) : Table(id, offset, count) {}

    void setData(vector<uint8_t> __data) { _data = __data; }

    vector<uint8_t> &data() override { return _data; }
};

TEST_CASE("Should fail to request partial read because offset is higher than allowed (24 bits)",
          "[ansic1218][services][ReadPartial]")
{
    MockTable table(0x1234, 0x0FFFFFFF, 10);
    ReadPartial readPartial(table);

    vector<uint8_t> request;

    TEST_ASSERT_FALSE(readPartial.request(request));
}

TEST_CASE("Should not fail to request partial read because offset is equal than max allowed value (24 bits)",
          "[ansic1218][services][ReadPartial]")
{
    MockTable table(0x1234, 0x00FFFFFF, 10);
    ReadPartial readPartial(table);

    vector<uint8_t> request;

    TEST_ASSERT_TRUE(readPartial.request(request));
}

TEST_CASE("Should request and validate \"full read\" of table properly", "[ansic1218][services][ReadPartial]")
{
    MockTable table(0x1234, 0, 10);
    ReadPartial readPartial(table);

    vector<uint8_t> expected_request{0x3F, 0x12, 0x34, 0, 0, 0, 0, 10};
    vector<uint8_t> table_data(10);
    std::iota(std::begin(table_data), std::end(table_data), 1);

    table.setData(table_data);

    vector<uint8_t> response = {0x00, 0x00, uint8_t(table_data.size())};

    uint8_t checksum = 0;
    for_each(table_data.begin(), table_data.end(), [&checksum](uint8_t value) { checksum += value; });
    checksum = ~checksum;

    copy(table_data.begin(), table_data.end(), back_inserter(response));
    response.push_back(checksum);

    vector<uint8_t> request;
    readPartial.request(request);

    TEST_ASSERT_TRUE(request == expected_request);
    TEST_ASSERT_TRUE(readPartial.response(response.cbegin(), response.cend()));
    TEST_ASSERT_TRUE(table.data() == table_data);
}

TEST_CASE("Should request and validate half of a big table properly", "[ansic1218][services][ReadPartial]")
{
    MockTable table(0x1234, 500, 1000);
    ReadPartial readPartial(table);

    vector<uint8_t> expected_request{0x3F, 0x12, 0x34, 0, 0x01, 0xF4, 0x03, 0xE8};
    vector<uint8_t> table_data(800);
    std::iota(std::begin(table_data), std::end(table_data), 1);

    table.setData(table_data);

    vector<uint8_t> response = {0x00, 0x01, 0x2C};

    uint8_t checksum = 0;
    for_each(table_data.begin() + 500, table_data.end(), [&checksum](uint8_t value) { checksum += value; });
    checksum = ~checksum;

    copy(table_data.begin() + 500, table_data.end(), back_inserter(response));
    response.push_back(checksum);

    vector<uint8_t> request;
    readPartial.request(request);

    TEST_ASSERT_TRUE(request == expected_request);
    TEST_ASSERT_TRUE(readPartial.response(response.cbegin(), response.cend()));
    TEST_ASSERT_TRUE(table.data() == table_data);
}
