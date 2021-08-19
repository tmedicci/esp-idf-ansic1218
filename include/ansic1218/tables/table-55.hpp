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


#pragma once

#include <cstdint>
#include "table.hpp"

namespace ansic1218 {

namespace table {

class Table55 : public Table {
public:
    struct Content {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint8_t fraq;
        uint8_t weekDay;
        uint16_t currentSegment : 3;
        uint16_t notUsed : 3;
        uint16_t constValue : 2;
        uint16_t constValue2 : 8;
    } __attribute__((__packed__));

    Table55() : Table(55) {}

    Content *content() { return reinterpret_cast<Content *>(data().data()); };
};
}    // namespace table
}    // namespace ansic1218
