
#include <sstream>
#include "table.hpp"

using namespace std;
using namespace ansic1218::table;

static const char *TAG = "ansic1218::tables:table";

Table::Table(uint16_t id) : _id(id), _offset(0) {}

Table::Table(uint16_t id, uint32_t offset) : _id(id), _offset(offset) {}

Table::Table(uint16_t id, uint32_t offset, uint16_t count) : _id(id), _offset(offset), _count(count)
{
    raw_data.reserve(_count);
    raw_data.clear();
}

uint16_t Table::id() const
{
    return _id;
}

uint32_t Table::offset() const
{
    return _offset;
}

uint32_t Table::count() const
{
    return _count;
}

vector<uint8_t> &Table::data()
{
    return raw_data;
}

string Table::name() const
{
    ostringstream out;
    out << "Table" << _id;
    return out.str();
}

bool Table::response(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    if (!validate(first, last))
        return false;

    raw_data.reserve(distance(first, last));
    raw_data.clear();

    copy(first, last, back_inserter(raw_data));

    return true;
}

bool Table::validate(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    return 0 < distance(first, last);
}

void Table::initializer(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    raw_data.reserve(distance(first, last));
    raw_data.clear();

    copy(first, last, back_inserter(raw_data));
}
