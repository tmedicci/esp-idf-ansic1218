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

#include "ansic1218/crc.h"

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
