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


#ifndef ANSIC1218_IDENTIFICATIONSERVICE_H
#define ANSIC1218_IDENTIFICATIONSERVICE_H

#include "service.h"

#include "esp_log.h"

namespace ansic1218
{

    namespace service
    {

        class Identification : public Service
        {

            std::vector<uint8_t> content;

        public:
            struct Response;
            struct DeviceIdentity;

            Identification();

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;

            std::vector<uint8_t> getDeviceIdentity();
        };

    }
}

#endif //ANSIC1218_IDENTIFICATIONSERVICE_H