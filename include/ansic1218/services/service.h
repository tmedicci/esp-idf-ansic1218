
#ifndef ANSIC1218_SERVICE_H
#define ANSIC1218_SERVICE_H

#include <cstdint>
#include <string>
#include <vector>

#include <endian.h>
#include <machine/endian.h>

#include "esp_log.h"

namespace ansic1218
{

    namespace service
    {

        class Service
        {

            std::string _name;

        protected:
            struct ResponseCode
            {
                const uint8_t value;
                const char *label;
            };

            static uint8_t checksum(std::vector<uint8_t>::const_iterator first,
                                    std::vector<uint8_t>::const_iterator last);

            virtual bool
            validate(std::vector<uint8_t>::const_iterator &first, std::vector<uint8_t>::const_iterator &last);

        public:
            explicit Service(const std::string &name);

            virtual bool request(std::vector<uint8_t> &buffer) = 0;

            virtual bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) = 0;

            const std::string &name() const;
        };

    }
}

#endif //ANSIC1218_SERVICE_H
