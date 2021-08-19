
#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "esp_log.h"
#include "crc.hpp"
#include "serial.hpp"
#include "services/service.hpp"

namespace ansic1218 {
class Transport {
    struct Packet;

    std::shared_ptr<ansic1218::Serial> serial;

    int receive(std::vector<uint8_t> &buffer, size_t size);

    void send(const std::vector<uint8_t> &data);

    bool wait(std::vector<uint8_t> &buffer, const std::vector<uint8_t> &data);

    void flush();

    bool nack(const std::vector<uint8_t> &sent, const std::vector<uint8_t> &received);

    template <typename... Args>
    bool nack(const service::Service &service, const std::vector<uint8_t> &sent, const std::vector<uint8_t> &received,
              Args... args)
    {
        std::ostringstream ss;
        using List = int[];
        (void)List{0, ((void)(ss << args), 0)...};
        ESP_LOGI("ansic1218::transport", "%s %s", service.name().c_str(), ss.str().c_str());
        return nack(sent, received);
    };

    std::string bufToStr(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

public:
    static constexpr uint8_t ACK = 0x06;
    static constexpr uint8_t NACK = 0x15;

    explicit Transport(std::shared_ptr<ansic1218::Serial> serial);

    bool request(service::Service &&service);

    bool request(service::Service &service);

    bool validate(Packet *packet);
};
}    // namespace ansic1218
