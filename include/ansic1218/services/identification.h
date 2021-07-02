
#ifndef CHRONOS_ANSI_READER_IDENTIFICATIONSERVICE_H
#define CHRONOS_ANSI_READER_IDENTIFICATIONSERVICE_H

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

#endif //CHRONOS_ANSI_READER_IDENTIFICATIONSERVICE_H
