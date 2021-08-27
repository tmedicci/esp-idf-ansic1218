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

#include "utils/serial.hpp"

namespace mock {

class Serial : public utils::Serial {
    std::vector<uint8_t> request;
    std::vector<uint8_t> response;

public:
    Serial();

    void setResponse(const std::vector<uint8_t> &response);

    const std::vector<uint8_t> &getRequested() const;

    void write(const std::vector<uint8_t> &buffer) override;

    size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout) override;
};
}    // namespace mock
