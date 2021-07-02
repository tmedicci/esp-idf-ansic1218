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


#ifndef ANSIC1218_HOST_TABLE_H
#define ANSIC1218_HOST_TABLE_H

// #include <endian.h>
// #include <machine/endian.h>
#include <stdint.h>

#include <cstdint>
#include <string>
#include <vector>

namespace ansic1218
{

    namespace table
    {

        class Table
        {

            uint16_t _id;
            uint32_t _offset;
            uint16_t _count;
            std::vector<uint8_t> raw_data;

        protected:
            virtual bool
            validate(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

        public:
            explicit Table(uint16_t id);

            explicit Table(uint16_t id, uint32_t offset);

            explicit Table(uint16_t id, uint32_t offset, uint16_t count);

            virtual void normalize(){};

            virtual bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

            uint16_t id() const;

            virtual uint32_t offset() const;

            virtual uint32_t count() const;

            virtual std::vector<uint8_t> &data();

            std::string name() const;

            virtual void
            initializer(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);
        };
    }
}

#endif //ANSIC1218_HOST_TABLE_H
