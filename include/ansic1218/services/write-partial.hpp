
#ifndef ANSIC1218_HOST_WRITEPARTIAL_H
#define ANSIC1218_HOST_WRITEPARTIAL_H

#include "../tables/table.hpp"
#include "service.hpp"

#include "esp_log.h"

namespace ansic1218
{

    namespace service
    {

        class WritePartial : public Service
        {

            typedef struct
            {
                uint32_t data : 24;
            } __attribute__((__packed__)) uint24_t;

            struct Request;

            static constexpr uint8_t PARTIAL_WRITE = 0x4F;

            table::Table &table;

            uint24_t offset;

        public:
            explicit WritePartial(table::Table &table);

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
        };
    }
}

#endif //ANSIC1218_HOST_WRITEPARTIAL_H
