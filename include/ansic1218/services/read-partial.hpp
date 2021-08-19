
#pragma once

#include "../tables/table.hpp"
#include "service.hpp"

namespace ansic1218 {

namespace service {

class ReadPartial : public Service {
    typedef struct {
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

    bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
};
}    // namespace service
}    // namespace ansic1218
