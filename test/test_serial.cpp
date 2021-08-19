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

#include <chrono>
#include "ansic1218/serial.hpp"

using namespace std;
using namespace chrono;
using namespace ansic1218;

TEST_CASE("Should send and receive data without flushing input buffer", "[ansic1218][serial]")
{
    Serial serial;

    serial.open(UART_NUM_2,
                9600,
                GPIO_NUM_25,
                GPIO_NUM_26);

    serial.set_loop_back();

    string s{"Hello, world!"};

    serial.write({s.begin(), s.end()});

    vector<uint8_t> resp_first;
    serial.read(resp_first, 5, seconds(1));
    vector<uint8_t> expected_first{'H', 'e', 'l', 'l', 'o'};
    vector<uint8_t> resp_second;
    serial.read(resp_second, 8, seconds(1));
    vector<uint8_t> expected_second{',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};

    TEST_ASSERT_TRUE(resp_first == expected_first);
    TEST_ASSERT_TRUE(resp_second == expected_second);
}

TEST_CASE("Should send and receive data with flushed input buffer", "[ansic1218][serial]")
{
    Serial serial;

    serial.open(UART_NUM_2,
                9600,
                GPIO_NUM_25,
                GPIO_NUM_26);

    serial.set_loop_back();

    string s{"Hello, world!"};

    serial.write({s.begin(), s.end()});

    vector<uint8_t> resp_first;
    serial.read(resp_first, 5, seconds(1), Serial::FlushInputBuffer::Enable);
    vector<uint8_t> expected_first{'H', 'e', 'l', 'l', 'o'};
    vector<uint8_t> resp_second;
    serial.read(resp_second, 8, seconds(1));
    vector<uint8_t> expected_second{',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};

    TEST_ASSERT_TRUE(resp_first == expected_first);
    TEST_ASSERT_TRUE(resp_second != expected_second);
}

TEST_CASE("Should send and receive less data than expected (by timeout)", "[ansic1218][serial]")
{
    Serial serial;

    serial.open(UART_NUM_2,
                9600,
                GPIO_NUM_25,
                GPIO_NUM_26);

    serial.set_loop_back();

    string s{"Hell"};

    serial.write({s.begin(), s.end()});

    vector<uint8_t> resp;
    serial.read(resp, 5, seconds(5));
    vector<uint8_t> expected{'H', 'e', 'l', 'l', 'o'};
    TEST_ASSERT_TRUE(resp != expected);
}

TEST_CASE("Should send and receive data until pattern is found", "[ansic1218][serial]")
{

    Serial serial;

    serial.open(UART_NUM_2,
                9600,
                GPIO_NUM_25,
                GPIO_NUM_26);

    serial.set_loop_back();

    string s{"Lets say \"Hello World\", my dear!"};

    serial.write({s.begin(), s.end()});

    string r{"Existing Hello World!"};
    vector<uint8_t> resp(r.begin(), r.end());
    string p{"Hello"};
    vector<uint8_t> pattern(p.begin(), p.end());
    serial.read(resp, pattern, 100, seconds(10));
    string e{"Existing Hello World!Lets say \"Hello"};
    vector<uint8_t> expected(e.begin(), e.end());
    TEST_ASSERT_TRUE(resp == expected);
}

TEST_CASE("Should fail to send and receive data because pattern is not found", "[ansic1218][serial]")
{

    Serial serial;

    serial.open(UART_NUM_2,
                9600,
                GPIO_NUM_25,
                GPIO_NUM_26);

    serial.set_loop_back();

    string s{"Lets say \"Hello World\", my dear!"};
    vector<uint8_t> send(s.begin(), s.end());

    serial.write(send);

    string r{"Existing Hello World!"};
    vector<uint8_t> resp(r.begin(), r.end());
    string p{"Healo"};
    vector<uint8_t> pattern(p.begin(), p.end());
    auto ret = serial.read(resp, pattern, 100, seconds(1));
    string e{"Existing Hello World!Lets say \"Hello World\", my dear!"};
    vector<uint8_t> expected(e.begin(), e.end());
    TEST_ASSERT_TRUE(resp == expected);
    TEST_ASSERT_TRUE(ret == (0 - send.size()));
}
