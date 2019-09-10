#ifndef PALCOMMON_H
#define PALCOMMON_H

#include <QtCore>

namespace Seville
{
   union bi32
   {
      char32_t i32;
      unsigned char ch[sizeof(qint32)];
   };

   union bi16
   {
      char16_t i16;
      unsigned char ch[sizeof(qint16)];
   };

   class PalCommon
   {
      public:
         PalCommon();

         enum ByteOrder {
            Unknown, BigEndian, LittleEndian };

         static bool isBigEndian(void);
         static quint16 doSwapWBo(const quint16 unswapped);
         static quint32 doSwapDwBo(const quint32 unswapped);
         static qint16 swapI16Bo(const qint16 unswapped);
         static quint16 swapU16Bo(const quint16 unswapped);
         static qint32 swapI32Bo(const qint32 unswapped);
         static quint32 swapU32Bo(const quint32 unswapped);
   };
}

#endif // PALCOMMON_H
