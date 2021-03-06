#include <string>

#include <QByteArray>
#include <QDataStream>

#include "seville/base/types.h"

#include "seville/palace/host.h"
#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      NetMsg::NetMsg(NetMsgOptions options)
         //: ByteArray()
      {
         do_init(options);
      }

//      NetMsg::NetMsg(
//            Host::ByteOrder clientByteOrder,
//            Host::ByteOrder serverByteOrder,
//            NetMsgOptions options)
//         //: ByteArray()
//      {
//         //do_init_(clientByteOrder, serverByteOrder, options);
//         auto cond = clientByteOrder == serverByteOrder;
//         auto options = NetMsgOptions::kEndianSwap * cond;
//         do_init_(options);
//      }

      NetMsg::NetMsg(
            const char* data, int len, NetMsgOptions options)
         //: ByteArray()
      {
         do_init(data, len, options);
      }

//      NetMsg::NetMsg(
//            const QByteArray& bytesOfNetMsg, NetMsgOptions options)
//         //: ByteArray()
//      {
//         do_init_(bytesOfNetMsg, options);
//      }

//      NetMsg::NetMsg(
//            const NetMsg& netmsg, NetMsgOptions options)
//         //: ByteArray()
//      {
//         do_init_(netmsg, options);
//      }

      NetMsg::~NetMsg(void)
      {

      }
   }
}
