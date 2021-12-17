#ifndef SEVILLE_PALACE_HOST_H_
#define SEVILLE_PALACE_HOST_H_

#include <QtCore>

namespace seville
{
    namespace palace
    {
        class Host
        {
        public:
            enum ByteOrder {
                kUnknownEndian,
                kBigEndian,
                kLittleEndian
            };

            Host(void);
            virtual ~Host(void);

            static bool IsBigEndian(void);

            static qint16 SwapI16(const qint16 unswapped);
            static quint16 SwapU16(const quint16 unswapped);
            static qint32 SwapI32(const qint32 unswapped);
            static quint32 SwapU32(const quint32 unswapped);

        private:
            static quint16 do_swap_word(const quint16 unswapped);
            static quint32 do_swap_dword(const quint32 unswapped);
        };
    }
}

#endif  // SEVILLE_PALACE_HOST_H_
