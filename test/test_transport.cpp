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


#include "ansic1218/transport.hpp"
#include "mocks/serial-mock.hpp"
#include "unity.h"

using namespace std;
using namespace ansic1218;
using namespace ansic1218::service;

class MockedService : public Service {
    vector<uint8_t> _request;
    vector<uint8_t> _data;

public:
    MockedService() : Service(__PRETTY_FUNCTION__), _request(), _data() {}

    void setRequestContent(const vector<uint8_t> &request) { _request = request; }

    bool request(std::vector<uint8_t> &buffer) override
    {
        copy(_request.cbegin(), _request.cend(), back_inserter(buffer));
        return true;
    }

    bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override
    {
        _data.reserve(distance(first, last));
        _data.clear();

        copy(first, last, back_inserter(_data));

        return true;
    }

    vector<uint8_t> &data() { return _data; }
};

TEST_CASE("Should request and respond properly", "[ansic1218][transport]")
{
    auto serial = make_shared<mock::Serial>();

    MockedService service;

    Transport transport(serial);

    vector<uint8_t> service_request{1, 2, 3, 4, 5, 6};
    service.setRequestContent(service_request);

    vector<uint8_t> expected_request{0x06, 0xee, 5, 0, 0, 0, uint8_t(service_request.size())};
    copy(service_request.begin(), service_request.end(), back_inserter(expected_request));
    CRC::calculate(expected_request.begin() + 1, expected_request.end(), expected_request);
    expected_request.push_back(0x06);

    vector<uint8_t> content{0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x01, 0x02};
    vector<uint8_t> response{0x06, 0xee, 0, 0, 0, 0, uint8_t(content.size())};

    copy(content.begin(), content.end(), back_inserter(response));
    CRC::calculate(response.begin() + 1, response.end(), response);

    serial->setResponse(response);

    TEST_ASSERT_TRUE(transport.request(service));
    TEST_ASSERT_TRUE(serial->getRequested() == expected_request);
    TEST_ASSERT_TRUE(service.data() == content);
}

TEST_CASE("Should fail on nack", "[ansic1218][transport]")
{
    auto serial = make_shared<mock::Serial>();

    MockedService service;

    vector<uint8_t> service_request{1, 2, 3, 4, 5, 6};
    service.setRequestContent(service_request);

    Transport transport(serial);

    vector<uint8_t> response{0x15, 1, 2, 3, 4};

    serial->setResponse(response);

    TEST_ASSERT_FALSE(transport.request(service));

    vector<uint8_t> single_request{0x06, 0xee, 5, 0, 0, 0, uint8_t(service_request.size())};
    copy(service_request.begin(), service_request.end(), back_inserter(single_request));
    CRC::calculate(single_request.begin() + 1, single_request.end(), single_request);

    vector<uint8_t> expected_request = single_request;
    for (uint8_t i = 0; i < 3; i++) {
        copy(single_request.begin() + 1, single_request.end(), back_inserter(expected_request));
    }
    expected_request.push_back(0x06);

    TEST_ASSERT_TRUE(serial->getRequested() == expected_request);
}

TEST_CASE("Should request and return multiple packets (2 frames) properly", "[ansic1218][transport]")
{
    auto serial = make_shared<mock::Serial>();

    MockedService service;

    Transport transport(serial);

    vector<uint8_t> service_request{1, 2, 3, 4, 5, 6};
    service.setRequestContent(service_request);

    vector<uint8_t> expected_request{0x06, 0xee, 5, 0, 0, 0, uint8_t(service_request.size())};
    copy(service_request.begin(), service_request.end(), back_inserter(expected_request));
    CRC::calculate(expected_request.begin() + 1, expected_request.end(), expected_request);
    expected_request.push_back(0x06);
    expected_request.push_back(0x06);

    vector<uint8_t> content{0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x01, 0x02};

    vector<uint8_t> response;

    vector<uint8_t> response_1{0x06, 0xee, 0, 0xc0, 0x01, 0, uint8_t(content.size())};
    copy(content.begin(), content.end(), back_inserter(response_1));
    CRC::calculate(response_1.begin() + 1, response_1.end(), response_1);
    copy(response_1.begin(), response_1.end(), back_inserter(response));

    vector<uint8_t> response_2{0xee, 0, 0x80, 0x00, 0, uint8_t(content.size())};
    copy(content.begin(), content.end(), back_inserter(response_2));
    CRC::calculate(response_2.begin(), response_2.end(), response_2);
    copy(response_2.begin(), response_2.end(), back_inserter(response));

    vector<uint8_t> content_2 = content;
    copy(content.begin(), content.end(), back_inserter(content_2));

    serial->setResponse(response);

    TEST_ASSERT_TRUE(transport.request(service));
    TEST_ASSERT_TRUE(serial->getRequested() == expected_request);
    TEST_ASSERT_TRUE(service.data() == content_2);
}

TEST_CASE("Should request and return multiple packets (3 frames) properly", "[ansic1218][transport]")
{
    auto serial = make_shared<mock::Serial>();

    MockedService service;

    Transport transport(serial);

    vector<uint8_t> service_request{1, 2, 3, 4, 5, 6};
    service.setRequestContent(service_request);

    vector<uint8_t> expected_request{0x06, 0xee, 5, 0, 0, 0, uint8_t(service_request.size())};
    copy(service_request.begin(), service_request.end(), back_inserter(expected_request));
    CRC::calculate(expected_request.begin() + 1, expected_request.end(), expected_request);
    expected_request.push_back(0x06);

    vector<uint8_t> content{0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x01, 0x02};

    vector<uint8_t> response;

    vector<uint8_t> response_1{0x06, 0xee, 0, 0xc0, 0x02, 0, uint8_t(content.size())};
    copy(content.begin(), content.end(), back_inserter(response_1));
    CRC::calculate(response_1.begin() + 1, response_1.end(), response_1);
    copy(response_1.begin(), response_1.end(), back_inserter(response));

    vector<uint8_t> response_2{0xee, 0, 0x80, 0x01, 0, uint8_t(content.size())};
    copy(content.begin(), content.end(), back_inserter(response_2));
    CRC::calculate(response_2.begin(), response_2.end(), response_2);
    copy(response_2.begin(), response_2.end(), back_inserter(response));
    expected_request.push_back(0x06);

    vector<uint8_t> content_3 = content;
    copy(content.begin(), content.end(), back_inserter(content_3));

    vector<uint8_t> response_3{0xee, 0, 0x80, 0x00, 0, uint8_t(content.size())};
    copy(content.begin(), content.end(), back_inserter(response_3));
    CRC::calculate(response_3.begin(), response_3.end(), response_3);
    copy(response_3.begin(), response_3.end(), back_inserter(response));
    expected_request.push_back(0x06);

    copy(content.begin(), content.end(), back_inserter(content_3));

    serial->setResponse(response);

    TEST_ASSERT_TRUE(transport.request(service));
    TEST_ASSERT_TRUE(serial->getRequested() == expected_request);
    TEST_ASSERT_TRUE(service.data() == content_3);
}
