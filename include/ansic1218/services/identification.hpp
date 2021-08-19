
#pragma once

#include "esp_log.h"
#include "service.hpp"

namespace ansic1218 {

namespace service {

class Identification : public Service {
    std::vector<uint8_t> content;

public:
    struct Response;
    struct DeviceIdentity;

    Identification();

    bool request(std::vector<uint8_t> &buffer) override;

    bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last) override;

    std::vector<uint8_t> getDeviceIdentity();
};

}    // namespace service
}    // namespace ansic1218
