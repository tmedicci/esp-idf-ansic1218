
#ifndef CHRONOS_ANSI_READER_HOST_CRC_H
#define CHRONOS_ANSI_READER_HOST_CRC_H

#include <cstdint>
#include <vector>

namespace ansic1218
{

    class CRC
    {

        static uint32_t table[256];

    public:
        static std::vector<uint8_t> &calculate(std::vector<uint8_t>::const_iterator first,
                                               std::vector<uint8_t>::const_iterator last,
                                               std::vector<uint8_t> &&buffer);

        static std::vector<uint8_t> &calculate(std::vector<uint8_t>::const_iterator first,
                                               std::vector<uint8_t>::const_iterator last,
                                               std::vector<uint8_t> &buffer);
    };
}

#endif //CHRONOS_ANSI_READER_HOST_CRC_H
