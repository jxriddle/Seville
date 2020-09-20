#include <QByteArray>
#include <QDataStream>

#include "ZOG_ByteArray.h"
#include "Palace_Host.h"
#include "generic.h"
#include <string>

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         enum

         Generic::~Generic(void)
         {
            do_deinit();
         }

         Generic::Generic(bool shouldSwapEndianness)
            : ByteArray()
         {
            do_init(shouldSwapEndianness);
         }

         Generic::Generic(
               Host::ByteOrder clientByteOrder,
               Host::ByteOrder serverByteOrder,
               bool shouldSwapEndianness)
            : ByteArray()
         {
            do_init(clientByteOrder, serverByteOrder, shouldSwapEndianness);
         }

         Generic::Generic(const char* data, int len, bool shouldSwapEndianness)
            : ByteArray()
         {
            do_init(data, len, shouldSwapEndianness);
         }

         Generic::Generic(const QByteArray& bytesOfNetMsg, bool shouldSwapEndianness)
            : ByteArray()
         {
            do_init(bytesOfNetMsg, shouldSwapEndianness);
         }

         Generic::Generic(const Generic& netMsg, bool shouldSwapEndianness)
            : ByteArray()
         {
            do_init(netMsg, shouldSwapEndianness);
         }
      }
   }
}
