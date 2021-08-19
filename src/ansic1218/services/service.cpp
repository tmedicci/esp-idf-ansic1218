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
#include <sstream>
#include "service.hpp"

using namespace std;
using namespace ansic1218::service;

static const char *TAG = "ansic1218::services:service";

const string &Service::name() const
{
    return _name;
}

Service::Service(const string &name) : _name(name.substr(0, name.rfind(':', name.find('(')))) {}

bool Service::validate(vector<uint8_t>::const_iterator &first, vector<uint8_t>::const_iterator &last)
{

    static constexpr ResponseCode OK{0X00, "Ok"};

    static vector<ResponseCode> responses{
        OK,
        {0X01, "Error"},
        {0X02, "Service Not Supported"},
        {0X03, "Insufficient security Clearance"},
        {0X04, "Operation Not Possible"},
        {0X05, "Inappropriate Action Requested"},
        {0X06, "Device Busy"},
        {0X07, "Data Not Ready"},
        {0X08, "Data Locked"},
        {0X09, "Renegotiate Request"},
        {0X0A, "Invalid Service Sequence State"},
    };

    if (distance(first, last) <= 0)
    {

        ESP_LOGW(TAG, "Validation Fail: Data has no content");
        return false;
    }
    else if (*first == OK.value)
    {

        return true;
    }

    auto response = find_if(responses.begin(), responses.end(), [&first](ResponseCode r)
                            { return r.value == *first; });

    ostringstream out;

    if (response == responses.end())
    {
        out << "Received an unknown <NOK> code: " << int(*first);
    }
    else
    {
        out << response->label;
    }

    ESP_LOGW(TAG, "%s", out.str().c_str());
    return false;
}

uint8_t Service::checksum(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    uint8_t checksum = 0;
    for_each(first, last, [&checksum](uint8_t value)
             { checksum += value; });
    return ~checksum;
}
