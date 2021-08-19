
#pragma once

#include "esp_log.h"
#include "../tables/table.hpp"
#include "service.hpp"

namespace ansic1218 {

namespace service {

class WriteFull : public Service {
    struct Request;

    static constexpr uint8_t FULL_WRITE = 0x40;

    table::Table &table;

public:
    explicit WriteFull(table::Table &table);

    bool request(std::vector<uint8_t> &buffer) override;

    bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;
};
}    // namespace service
}    // namespace ansic1218
