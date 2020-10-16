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
      NetMsg::NetMsg(NetMsgOptions options)
         : ByteArray()
      {
         do_init_(options);
      }

      NetMsg::NetMsg(
            Host::ByteOrder clientByteOrder,
            Host::ByteOrder serverByteOrder,
            NetMsgOptions options)
         : ByteArray()
      {
         do_init_(clientByteOrder, serverByteOrder, options);
      }

      NetMsg::NetMsg( \
            const char* data, int len, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(data, len, options);
      }

      NetMsg::NetMsg( \
            const QByteArray& bytesOfNetMsg, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(bytesOfNetMsg, options);
      }

      NetMsg::NetMsg( \
            const NetMsg& netmsg, NetMsgOptions options)
         : ByteArray()
      {
         do_init_(netmsg, options);
      }
   }
}
