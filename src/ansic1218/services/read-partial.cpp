
#include <limits>
#include "read-partial.hpp"

using namespace std;
using namespace ansic1218::service;
using namespace ansic1218::table;

static const char *TAG = "ansic1218::services:read-partial";

struct ReadPartial::Request {
    uint8_t type;
    uint16_t tableId;
    uint24_t offset;
    uint16_t count;
} __attribute__((__packed__));

struct ReadPartial::Response {
    uint8_t nok;
    uint16_t count;
    uint8_t data[];
} __attribute__((__packed__));

ReadPartial::ReadPartial(Table &table) : Service(__PRETTY_FUNCTION__), table(table) {}

bool ReadPartial::request(std::vector<uint8_t> &buffer)
{
    static_assert(sizeof(Request) == 8, "");

    if (table.offset() > (numeric_limits<uint32_t>::max() >> 8))
        return 0;

    Request request{.type = PARTIAL_READ,
                    .tableId = htobe16(table.id()),
                    .offset =
                        {
                            .data = (htobe32(table.offset()) >> 8),
                        },
                    .count = htobe16(table.count())};

    auto *ptr = reinterpret_cast<uint8_t *>(&request);

    copy(&ptr[0], &ptr[sizeof(Request)], back_inserter(buffer));

    return true;
}

bool ReadPartial::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    if (!Service::validate(first, last))
        return false;

    static_assert(sizeof(Response) == 3, "");

    auto *resp = (Response *)&*first;

    uint8_t chk = 0;
    resp->count = be16toh(resp->count);
    auto expected_size = resp->count + sizeof(Response) + sizeof(chk);

    if (distance(first, last) != expected_size) {
        ESP_LOGW(TAG, "Wrong response size, expected: %d, received: %d", int(expected_size), distance(first, last));
        return false;
    }

    first += sizeof(Response);
    chk = checksum(first, last - 1);

    if (*prev(last) != chk) {
        ESP_LOGW(TAG, "Invalid checksum, expected: %d, received: %d", int(chk), int(*prev(last)));
        return false;
    }

    return table.response(first, last - 1);
}
