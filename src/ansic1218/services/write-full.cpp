
#include "write-full.h"

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

struct WriteFull::Request
{
    uint8_t type;
    uint16_t tableId;
    uint16_t count;
} __attribute__((__packed__));

WriteFull::WriteFull(Table &table) : Service(__PRETTY_FUNCTION__), table(table) {}

bool WriteFull::request(std::vector<uint8_t> &buffer)
{

    Request header{
        .type = FULL_WRITE,
        .tableId = htobe16(table.id()),
        .count = htobe16(table.data().size())};

    uint8_t chksum = checksum(table.data().begin(), table.data().end());

    auto *p = reinterpret_cast<uint8_t *>(&header);

    copy(p, p + sizeof(header), back_inserter(buffer));
    copy(table.data().begin(), table.data().end(), back_inserter(buffer));
    buffer.push_back(chksum);

    return true;
}

bool WriteFull::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    return Service::validate(first, last);
}
