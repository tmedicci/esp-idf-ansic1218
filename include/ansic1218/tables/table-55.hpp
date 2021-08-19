
#pragma once

#include <cstdint>
#include "table.hpp"

namespace ansic1218 {

namespace table {

class Table55 : public Table {
public:
    struct Content {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint8_t fraq;
        uint8_t weekDay;
        uint16_t currentSegment : 3;
        uint16_t notUsed : 3;
        uint16_t constValue : 2;
        uint16_t constValue2 : 8;
    } __attribute__((__packed__));

    Table55() : Table(55) {}

    Content *content() { return reinterpret_cast<Content *>(data().data()); };
};
}    // namespace table
}    // namespace ansic1218
