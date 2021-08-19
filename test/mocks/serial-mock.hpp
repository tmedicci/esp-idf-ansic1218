
#pragma once

#include <ansic1218/serial.hpp>

namespace mock {

class Serial : public ansic1218::Serial {
    std::vector<uint8_t> request;
    std::vector<uint8_t> response;

public:
    Serial();

    void setResponse(const std::vector<uint8_t> &response);

    const std::vector<uint8_t> &getRequested() const;

    void write(const std::vector<uint8_t> &buffer) override;

    size_t read(std::vector<uint8_t> &buffer, size_t nBytesToRead, std::chrono::nanoseconds timeout) override;
};
}    // namespace mock
