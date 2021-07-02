
#ifndef CHRONOS_ANSI_READER_HOST_SECURITY_H
#define CHRONOS_ANSI_READER_HOST_SECURITY_H

#include "service.h"

namespace ansic1218
{

    namespace service
    {

        class Security : public Service
        {

            std::vector<uint8_t> _request;
            std::vector<uint8_t> _response;

            static constexpr uint8_t SECURITY = 0x51;
            static constexpr unsigned int IDENTITY_LAST_N_BYTES = 0x20;
            static constexpr unsigned int SECURITY_PASSWORD_SIZE = 0x14;

        public:
            Security(const std::vector<uint8_t> &identity, const std::vector<uint8_t> &password);

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
        };
    }
}

#endif //CHRONOS_ANSI_READER_HOST_SECURITY_H
