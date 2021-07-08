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

#include <ansic1218/services/identification.h>

using namespace std;
using namespace ansic1218::service;

TEST_CASE("Should identify response errors", "[service][identity]")
{
    Identification identification;

    vector<uint8_t> nok = { 0x01 };
    TEST_ASSERT_FALSE(identification.response(nok.cbegin(), nok.cend()));
}
