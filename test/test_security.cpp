
#include "unity.h"

#include <ansic1218/services/security.hpp>

using namespace std;
using namespace ansic1218::service;

TEST_CASE("Should generate password properly", "[ansic1218][services][Security]")
{

    vector<uint8_t> identity = {77, 78, 48, 48, 48, 68, 48, 48, 48, 48, 48, 51, 48, 56, 50, 54, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    vector<uint8_t> password = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Security security(identity, password);

    vector<uint8_t> expected = {0x51, 0x33, 0xad, 0x0a, 0x1c, 0x60, 0x7e, 0xc0, 0x3b, 0x09,
                                0xe6, 0xcd, 0x98, 0x93, 0x68, 0x0c, 0xe2, 0x10, 0xad, 0xf3, 0x00};

    vector<uint8_t> request;
    security.request(request);

    TEST_ASSERT_TRUE(request == expected);
}
