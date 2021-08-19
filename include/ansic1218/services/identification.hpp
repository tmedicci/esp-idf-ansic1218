
#ifndef ANSIC1218_IDENTIFICATIONSERVICE_H
#define ANSIC1218_IDENTIFICATIONSERVICE_H

#include "service.hpp"

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
