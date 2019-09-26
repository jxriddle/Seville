#ifndef PALACE_COMMON_H
#define PALACE_COMMON_H

#include <QtCore>

namespace Seville
{
   namespace Palace
   {
      class Host
      {
         public:
            Host();

            enum ByteOrder {
               Unknown, BigEndian, LittleEndian };

            static bool isBigEndian(void);
            static quint16 doSwapWord(const quint16 unswapped);
            static quint32 doSwapDword(const quint32 unswapped);
            static qint16 swapInt16(const qint16 unswapped);
            static quint16 swapUInt16(const quint16 unswapped);
            static qint32 swapInt32(const qint32 unswapped);
            static quint32 swapU32(const quint32 unswapped);
      };
   }
}

#endif // PALACE_COMMON_H
