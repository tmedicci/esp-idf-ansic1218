
#include "test-utils.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string utils::bufToStr(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last)
{
    ostringstream ss;
    ss << "{ ";
    for_each(first, last, [&ss](uint8_t value)
             { ss << setfill('0') << setw(2) << hex << int(value) << " "; });
    ss << "}";
    return ss.str();
}
