
#ifndef ANSIC1218_UTILS_H
#define ANSIC1218_UTILS_H

#include <vector>
#include <string>

using namespace std;

namespace utils
{

    std::string bufToStr(vector<uint8_t>::const_iterator first, vector<uint8_t>::const_iterator last);
}

#endif //ANSIC1218_UTILS_H
