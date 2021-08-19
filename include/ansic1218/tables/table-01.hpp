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

namespace ansic1218 {
namespace table {
class Table01 : public Table {
public:
    struct Content {
        uint8_t manufacturer[4];
        uint8_t ed_model[8];
        uint8_t hw_version_number;
        uint8_t hw_revision_number;
        uint8_t fw_version_number;
        uint8_t fw_revision_number;
        char mfg_serial_number[16];
    } __attribute__((__packed__));

    Table01() : Table(1) {}

    Content *content() { return reinterpret_cast<Content *>(data().data()); };
};
}    // namespace table
}    // namespace ansic1218
