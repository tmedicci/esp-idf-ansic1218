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


#include "test-utils.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string utils::bufToStr(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    ostringstream ss;
    ss << "{ ";
    for_each(first, last, [&ss](uint8_t value) { ss << setfill('0') << setw(2) << hex << int(value) << " "; });
    ss << "}";
    return ss.str();
}
