
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <stdint.h>

namespace ansic1218 {

namespace table {

class Table {
    uint16_t _id;
    uint32_t _offset;
    uint16_t _count;
    std::vector<uint8_t> raw_data;

protected:
    virtual bool validate(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

public:
    explicit Table(uint16_t id);

    explicit Table(uint16_t id, uint32_t offset);

    explicit Table(uint16_t id, uint32_t offset, uint16_t count);

    virtual void normalize(){};

    virtual bool response(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);

    uint16_t id() const;

    virtual uint32_t offset() const;

    virtual uint32_t count() const;

    virtual std::vector<uint8_t> &data();

    std::string name() const;

    virtual void initializer(std::vector<uint8_t>::const_iterator first, std::vector<uint8_t>::const_iterator last);
};
}    // namespace table
}    // namespace ansic1218
