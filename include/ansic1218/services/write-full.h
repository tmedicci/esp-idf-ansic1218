
#ifndef ANSIC1218_HOST_WRITEFULL_H
#define ANSIC1218_HOST_WRITEFULL_H

#include "../tables/table.h"
#include "service.h"

#include "esp_log.h"

namespace ansic1218
{

    namespace service
    {

        class WriteFull : public Service
        {

            struct Request;

            static constexpr uint8_t FULL_WRITE = 0x40;

            table::Table &table;

        public:
            explicit WriteFull(table::Table &table);

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
        };
    }
}

#endif //ANSIC1218_HOST_WRITEFULL_H
