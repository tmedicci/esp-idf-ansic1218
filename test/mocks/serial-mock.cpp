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


#include "serial-mock.hpp"

using namespace std;
using namespace chrono;
using namespace mock;

Serial::Serial() : utils::Serial(), request(), response() {}

void Serial::write(const vector<uint8_t> &buffer)
{
    request.insert(request.end(), buffer.begin(), buffer.end());
}

size_t Serial::read(vector<uint8_t> &buffer, size_t nBytesToRead, nanoseconds timeout)
{
    nBytesToRead = nBytesToRead < response.size() ? nBytesToRead : response.size();

    buffer.reserve(buffer.size() + nBytesToRead);

    copy(begin(response), begin(response) + nBytesToRead, back_inserter(buffer));

    response.erase(begin(response), begin(response) + nBytesToRead);

    return nBytesToRead;
}

void Serial::setResponse(const vector<uint8_t> &response)
{
    Serial::response = response;
}

const vector<uint8_t> &Serial::getRequested() const
{
    return request;
}
