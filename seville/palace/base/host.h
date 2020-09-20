#ifndef PALACE_HOST_H
#define PALACE_HOST_H

#include <QtCore>

namespace seville
{
   namespace palace
   {
      class Host
      {
      public:
         enum ByteOrder
         {
            Unknown,
            BigEndian,
            LittleEndian
         };

      private:
         static quint16 do_swapWord(const quint16 unswapped);
         static quint32 do_swapDword(const quint32 unswapped);

      public:
         Host(void);
         virtual ~Host(void);

         static bool isBigEndian(void);

         static qint16 swapI16(const qint16 unswapped);
         static quint16 swapU16(const quint16 unswapped);
         static qint32 swapI32(const qint32 unswapped);
         static quint32 swapU32(const quint32 unswapped);
      };
   }
}

#endif // PALACE_HOST_H
