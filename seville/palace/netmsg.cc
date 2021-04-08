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

      auto NetMsg::do_init(NetMsgOptions options)
            -> void
      {
         setShouldSwapEndianness(NetMsgOptions::kEndianSwap == options);
         do_clear();
      }

      auto NetMsg::do_clear(void) -> void
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

      auto NetMsg::do_contentSize(void) -> u32
      {
         //return my_contentSize;
         auto z = size();
         auto result =
               (z <= NetMsg::kHeaderSize * 0) |
               ((NetMsg::kHeaderSize < z) * (z - NetMsg::kHeaderSize));
         return result;
      }

      auto NetMsg::do_id(void) -> u32
      {
         return u32At(NetMsg::kIdOffset);
      }

      auto NetMsg::do_setId(u32 value) -> void
      {
         //my_headerFlags &= 0b001;
         setU32At(NetMsg::kIdOffset, value);
      }

      auto NetMsg::do_len(void) -> u32
      {
         return u32At(NetMsg::kLenOffset);
      }

      auto NetMsg::do_setLen(u32 value) -> void
      {
         //my_headerFlags &= 0b010;
         setU32At(NetMsg::kLenOffset, value);
      }

      auto NetMsg::do_ref(void) -> u32
      {
         return u32At(NetMsg::kRefOffset);
      }

      auto NetMsg::do_setRef(u32 value) -> void
      {
         //my_headerFlags &= 0b100;
         setU32At(NetMsg::kRefOffset, value);
      }

      auto NetMsg::do_setStreamCursorToContentEnd(void) -> void {
         setStreamCursorPosition(
                  (size() <= NetMsg::kHeaderSize) * NetMsg::kHeaderSize
                  | (NetMsg::kHeaderSize < size()) * size());
      }

      auto NetMsg::do_setStreamCursorToContentStart(void) -> void {
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
