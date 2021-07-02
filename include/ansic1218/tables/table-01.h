#ifndef CHRONOS_ANSI_READER_TABLE01_H
#define CHRONOS_ANSI_READER_TABLE01_H

namespace ansic1218
{
    namespace table
    {
        class Table01 : public Table
        {

        public:
            struct Content
            {
                uint8_t manufacturer[4];
                uint8_t ed_model[8];
                uint8_t hw_version_number;
                uint8_t hw_revision_number;
                uint8_t fw_version_number;
                uint8_t fw_revision_number;
                char mfg_serial_number[16];
            } __attribute__((__packed__));

            Table01() : Table(1) {}

            Content *content()
            {
                return reinterpret_cast<Content *>(data().data());
            };
        };
    }
}

#endif //CHRONOS_ANSI_READER_TABLE01_H
