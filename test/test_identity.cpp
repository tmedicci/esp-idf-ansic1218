
#include "unity.h"

#include <ansic1218/services/identification.hpp>

using namespace std;
using namespace ansic1218::service;

TEST_CASE("Should identify response errors", "[ansic1218][services][Identity]")
{
    Identification identification;

    vector<uint8_t> nok = { 0x01 };
    TEST_ASSERT_FALSE(identification.response(nok.cbegin(), nok.cend()));
}
