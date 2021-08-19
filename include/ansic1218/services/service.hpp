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
#include <string>
#include <vector>
#include <endian.h>
#include <machine/endian.h>
#include "esp_log.h"

namespace ansic1218 {

namespace service {

class Service {
    std::string _name;

protected:
    struct ResponseCode {
        const uint8_t value;
        const char *label;
    };

    static uint8_t checksum(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

    virtual bool validate(std::vector<uint8_t>::const_iterator &first, std::vector<uint8_t>::const_iterator &last);

public:
    explicit Service(const std::string &name);

    virtual bool request(std::vector<uint8_t> &buffer) = 0;

    virtual bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) = 0;

    const std::string &name() const;
};

}    // namespace service
}    // namespace ansic1218
