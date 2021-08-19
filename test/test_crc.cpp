
#include "ansic1218/crc.hpp"
#include "unity.h"

using namespace std;
using namespace ansic1218;

TEST_CASE("Should calculate CRC properly", "[ansic1218][crc]")
{
    vector<uint8_t> data{0xEE, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20};
    vector<uint8_t> expected{0x13, 0x10};
    vector<uint8_t> calculated;

    CRC crc;
    crc.calculate(data.begin(), data.end(), calculated);

    TEST_ASSERT_TRUE(expected == calculated);
}
