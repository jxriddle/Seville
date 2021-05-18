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
//      NetMsg::NetMsg(void)
//      {
//         do_init();
//      }

      NetMsg::NetMsg(NetMsgOptions options)
         //: ByteArray()
      {
         do_init(options);
      }

      void NetMsg::do_init(NetMsgOptions options)
      {
         setShouldSwapEndianness(NetMsgOptions::kEndianSwap == options);
         do_clear();
      }

      void NetMsg::do_clear(void)
      {
         my_isValidFlag = 0;

         truncate(0);
         reserve(NetMsgSize::kHeaderSize);
         //my_headerFlags = 0;
         //do_setId(0);
         //do_setLen(0);
         //do_setRef(0);
         //my_contentSize = 0;
         //my_headerFlags = 0;
         //my_headerSize = 0;
      }

//      auto NetMsg::do_headerSize(void) -> u32
//      {
//         auto count = 0;
//         auto tmpHeaderFlags = my_headerFlags;
//         while (0 < tmpHeaderFlags) {
//            count += tmpHeaderFlags & 1;
//            tmpHeaderFlags >>= 1;
//         }

//         return count * sizeof(u32);
//      }

      i32 NetMsg::do_contentSize(void)
      {
         //return my_contentSize;
         auto z = size();
         auto result =
               (z <= NetMsg::kHeaderSize * 0) |
               ((NetMsg::kHeaderSize < z) * (z - NetMsg::kHeaderSize));
         return result;
      }

      u32 NetMsg::do_id(void)
      {
         return u32At(NetMsg::kIdOffset);
      }

      void NetMsg::do_setId(u32 value)
      {
         //my_headerFlags &= 0b001;
         setU32At(NetMsg::kIdOffset, value);
      }

      i32 NetMsg::do_bodyLen(void)
      {
         return i32At(NetMsg::kLenOffset);
      }

      void NetMsg::do_setBodyLen(i32 value)
      {
         //my_headerFlags &= 0b010;
         setI32At(NetMsg::kLenOffset, value);
      }

      i32 NetMsg::do_ref(void)
      {
         return i32At(NetMsg::kRefOffset);
      }

      void NetMsg::do_setRef(i32 value)
      {
         //my_headerFlags &= 0b100;
         setI32At(NetMsg::kRefOffset, value);
      }

      void NetMsg::do_setStreamCursorToContentEnd(void)
      {
         setStreamCursorPosition(
                  (size() <= NetMsg::kHeaderSize) * NetMsg::kHeaderSize
                  | (NetMsg::kHeaderSize < size()) * size());
      }

      void NetMsg::do_setStreamCursorToContentStart(void)
      {
         setStreamCursorPosition(NetMsg::kHeaderSize);
      }

//      auto NetMsg::do_setStreamCursorToHeaderStart(void) -> void {
//         setStreamCursorPosition(0);
//      }

//      auto NetMsg::do_setStreamCursorToHeaderEnd(void) -> void {
//         setStreamCursorPosition(do_headerSize());
//      }

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

//      NetMsg::NetMsg(
//            const char* data, int len, NetMsgOptions options)
//         //: ByteArray()
//      {
//         do_init(data, len, options);
//      }

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

//      NetMsg::~NetMsg(void)
//      {

//      }
   }
}
