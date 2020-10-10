#include <string>

#include <QByteArray>
#include <QDataStream>

#include "seville/base/types.h"

#include "seville/palace/base/host.h"
#include "seville/palace/base/genericnetmsg.h"

namespace seville
{
   namespace palace
   {
      GenericNetMsg::GenericNetMsg(bool shouldSwapEndianness)
         : ByteArray()
      {
         do_init(shouldSwapEndianness);
      }

      GenericNetMsg::GenericNetMsg(
            Host::ByteOrder clientByteOrder,
            Host::ByteOrder serverByteOrder,
            bool shouldSwapEndianness)
         : ByteArray()
      {
         do_init(clientByteOrder, serverByteOrder, shouldSwapEndianness);
      }

      GenericNetMsg::GenericNetMsg(const char* data, int len, bool shouldSwapEndianness)
         : ByteArray()
      {
         do_init(data, len, shouldSwapEndianness);
      }

      GenericNetMsg::GenericNetMsg(const QByteArray& bytesOfNetMsg, bool shouldSwapEndianness)
         : ByteArray()
      {
         do_init(bytesOfNetMsg, shouldSwapEndianness);
      }

      GenericNetMsg::GenericNetMsg(const GenericNetMsg& netMsg, bool shouldSwapEndianness)
         : ByteArray()
      {
         do_init(netMsg, shouldSwapEndianness);
      }
   }
}
