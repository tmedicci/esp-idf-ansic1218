
#include <limits>
#include "write-partial.hpp"

#include "esp_log.h"

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

struct WritePartial::Request
{
    uint8_t type;
    uint16_t tableId;
    uint24_t offset;
    uint16_t count;
} __attribute__((__packed__));

WritePartial::WritePartial(Table &table) : Service(__PRETTY_FUNCTION__), table(table) {}

bool WritePartial::request(std::vector<uint8_t> &buffer)
{
    if (table.offset() > (numeric_limits<uint32_t>::max() >> 8))
        return 0;

    Request header{
        .type = PARTIAL_WRITE,
        .tableId = htobe16(table.id()),
        .offset = {
            .data = (htobe32(table.offset()) >> 8),
        },
        .count = htobe16(table.data().size())};

    uint8_t chksum = checksum(table.data().begin(), table.data().end());

    auto *p = reinterpret_cast<uint8_t *>(&header);

    copy(p, p + sizeof(header), back_inserter(buffer));
    copy(table.data().begin(), table.data().end(), back_inserter(buffer));
    buffer.push_back(chksum);

    return true;
}

bool WritePartial::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    return Service::validate(first, last);
}
