
#ifndef ANSIC1218_TRANSPORT_H
#define ANSIC1218_TRANSPORT_H

#include <cstdint>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "services/service.hpp"
#include "serial.hpp"
#include "crc.hpp"

extern "C" {
#include "esp_log.h"
}

namespace ansic1218
{
    class Transport
    {
        struct Packet;

        std::shared_ptr<ansic1218::Serial> serial;

        int receive(std::vector<uint8_t> &buffer, size_t size);

        void send(const std::vector<uint8_t> &data);

        bool wait(std::vector<uint8_t> &buffer, const std::vector<uint8_t> &data);

        void flush();

        bool nack(const std::vector<uint8_t> &sent, const std::vector<uint8_t> &received);

        template <typename... Args>
        bool nack(const service::Service &service,
                  const std::vector<uint8_t> &sent, const std::vector<uint8_t> &received, Args... args)
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
}

#endif //ANSIC1218_TRANSPORT_H
