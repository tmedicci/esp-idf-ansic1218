
#ifndef ANSIC1218_HOST_READ_H
#define ANSIC1218_HOST_READ_H

#include "../tables/table.hpp"
#include "service.hpp"

namespace ansic1218
{

    namespace service
    {

        class ReadFull : public Service
        {

            static constexpr uint8_t FULL_READ = 0x30;

            table::Table &table;

        protected:
            struct Request
            {

                uint8_t type;
                uint16_t table_id;

            } __attribute__((__packed__));

            struct Response
            {

                uint8_t nok;
                uint16_t count;
                uint8_t data[];

            } __attribute__((__packed__));

        public:
            explicit ReadFull(table::Table &);

            bool request(std::vector<uint8_t> &buffer) override;

            bool
            response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
        };
    }
}

#endif //ANSIC1218_HOST_READ_H
