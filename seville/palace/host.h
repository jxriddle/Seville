#ifndef SEVILLE_PALACE_HOST_H_
#define SEVILLE_PALACE_HOST_H_

#include <QtCore>

namespace seville
{
   namespace palace
   {
      enum HostByteOrder {
         kUnknownEndian,
         kBigEndian,
         kLittleEndian
      };

      class Host
      {
      public:
         Host(void);
         virtual ~Host(void);

         static auto IsBigEndian(void) -> bool;

         static auto SwapI16(const qint16 unswapped) -> qint16;
         static auto SwapU16(const quint16 unswapped) -> quint16;
         static auto SwapI32(const qint32 unswapped) -> qint32;
         static auto SwapU32(const quint32 unswapped) -> quint32;

      private:
         static auto do_swap_word(const quint16 unswapped) -> quint16;
         static auto do_swap_dword(const quint32 unswapped) -> quint32;
      };
   }
}

#endif  // SEVILLE_PALACE_HOST_H_
