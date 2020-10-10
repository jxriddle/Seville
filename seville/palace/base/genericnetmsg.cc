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
      GenericNetMsg::GenericNetMsg(NetMsgOptions options)
         : ByteArray()
      {
         do_init_(options);
      }

      GenericNetMsg::GenericNetMsg(
            Host::ByteOrder clientByteOrder,
            Host::ByteOrder serverByteOrder,
            NetMsgOptions options)
         : ByteArray()
      {
         do_init_(clientByteOrder, serverByteOrder, options);
      }

      GenericNetMsg::GenericNetMsg(const char* data, int len, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(data, len, options);
      }

      GenericNetMsg::GenericNetMsg(const QByteArray& bytesOfNetMsg, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(bytesOfNetMsg, options);
      }

      GenericNetMsg::GenericNetMsg(const GenericNetMsg& netMsg, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(netMsg, options);
      }
   }
}
