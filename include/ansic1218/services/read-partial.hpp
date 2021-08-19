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


#ifndef ANSIC1218_HOST_READPARTIAL_H
#define ANSIC1218_HOST_READPARTIAL_H

#include "../tables/table.hpp"
#include "service.hpp"

namespace ansic1218
{

    namespace service
    {

        class ReadPartial : public Service
        {

            typedef struct
            {
                uint32_t data : 24;
            } __attribute__((__packed__)) uint24_t;

            struct Request;

            struct Response;

            static constexpr uint8_t PARTIAL_READ = 0x3F;

            table::Table &table;

            uint24_t offset;

        public:
            explicit ReadPartial(table::Table &table);

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
        };
    }
}

#endif //ANSIC1218_HOST_READPARTIAL_H
