#ifndef SEVILLE_PALACE_NETMSG_GENERIC_H_
#define SEVILLE_PALACE_NETMSG_GENERIC_H_

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>

#include "seville/base/types.h"
#include "seville/base/bytearray.h"

#include "seville/palace/host.h"

namespace seville
{
   namespace palace
   {
      enum class NetMsgOptions: bool {
         kNoEndianSwap = 0,
         kEndianSwap = 1,
      };

      enum class NetMsgKind: u32 {
         // == Server ====================================================
         kNoKind                 = 0x00000000, // 0x6E696C20 // "nil "
         kUnknownServerKind      = 0x70736572, // "pser"
         kLittleEndianServerKind = 0x72796974, // "ryit"
         kBigEndianServerKind    = 0x74697972, // "tiyr"
         kAltLogonKind           = 0x72657032, // "rep2"
         kServerVersionKind      = 0x76657273, // "vers"
         kServerInfoKind         = 0x73696E66, // "sinf"
         kUserStatusKind         = 0x75537461, // "uSta"
         kUserLoggedOnAndMaxKind = 0x6C6F6720, // "log "
         kHttpServerLocationKind = 0x48545450, // "HTTP"
         kRoomDescriptionKind    = 0x726F6F6D, // "room"
         kAltRoomDescriptionKind = 0x73526F6D, // "sRom"
         kRoomUserListKind       = 0x72707273, // "rprs"
         kServerRoomListKind     = 0x724C7374, // "rLst"
         kRoomDescendKind        = 0x656E6472, // "endr"
         kUserNewKind            = 0x6E707273, // "nprs"
         kPingKind               = 0x70696E67, // "ping"
         kPongKind               = 0x706F6E67, // "pong"
         kXTalkKind              = 0x78746C6B, // "xtlk" (encrypted)
         kXWhisperKind           = 0x78776973, // "xwis" (encrypted)
         kTalkKind               = 0x74616C6B, // "talk" (unencrypted)
         kWhisperKind            = 0x77686973, // "whis" (unencrypted)
         kMovementKind           = 0x754C6F63, // "uLoc"
         kUserColorKind          = 0x75737243, // "usrC"
         kUserDescriptionKind    = 0x75737244, // "usrD"
         kUserFaceKind           = 0x75737246, // "usrF"
         kUserPropKind           = 0x75737250, // "usrP"
         kUserRenameKind         = 0x7573724E, // "usrN"
         kUserLeavingKind        = 0x62796520, // "bye "
         kFileIncomingKind       = 0x7346696C, // "sFil"
         kAssetIncomingKind      = 0x73417374, // "sAst"
         kUserExitRoomKind       = 0x65707273, // "eprs"
         kServerUserListKind     = 0x754C7374, // "uLst"
         kDoorLockKind           = 0x6C6F636B, // "lock"
         kDoorUnlockKind         = 0x756E6C6F, // "unlo"
         kSpotStateKind          = 0x73537461, // "sSta"
         kSpotMoveKind           = 0x636F4C73, // "coLs"
         kPictMoveKind           = 0x704C6F63, // "pLoc"
         kDrawKind               = 0x64726177, // "draw"
         kPropMoveKind           = 0x6D507270, // "mPrp"
         kPropDeleteKind         = 0x64507270, // "dPrp"
         kPropNewKind            = 0x6E507270, // "nPrp"
         kAssetQueryKind         = 0x71417374, // "qAst"
         kNavErrorKind           = 0x73457272, // "sErr"
         kConnectionErrorKind    = 0x646F776E, // "down"
         kBlowThruKind           = 0x626C6F77, // "blow"
         kAuthenticateKind       = 0x61757468, // "auth"
         // == Room =======================================================
         kGotoRoomKind           = 0x6E617652, // "navR"
         kRoomKind               = 0x30C6015D, // "0..]"
         kSuperUserKind          = 0x73757372, // "susr"
         kLogonKind              = 0x72656769, // "regi"
         kAssetRegiKind          = 0x72417374, // "rAst"
         kGlobalMessageKind      = 0x676D7367, // "gmsg"
         kRoomMessageKind        = 0x726D7367, // "rmsg"
         kSuperUserMessageKind   = 0x736D7367, // "smsg"
         kAuthResponseKind       = 0x61757472, // "autr"
      };

      // in i32
      //      static const i32 kSizeOfNetMsgHeaderInI32 = 3;
      //      static const i32 kOffsetForNetMsgIdInI32 = 0;
      //      static const i32 kOffsetForNetMsgLenInI32 = 1;
      //      static const i32 kOffsetForNetMsgRefInI32 = 2;
      //      static const i32 kOffsetForNetMsgPayloadInI32 = 3;

      // in Bytes
      //static const i32 kOffsetForPayloadInBytes = 12;

      enum class NetMsgOffset: u32 {
         kIdOffset = 0,
         kLenOffset = 4,
         kRefOffset = 8,
         kPayloadOffset = 12,
      };

      // Record Sizes (excludes header size)
      //static const u32 kAuthResponseSize =
      enum class NetMsgSize: u32 {
         kMinimumSize = 12,
         kMaximumSize = 1024000,
         //kHeaderSize = kMinimumSize,
         kGenericSize = kMinimumSize,
         kLogonSize = 128,
         kAltLogonSize = 0,
         kConnectionErrorSize = kGenericSize,
         kServerVersionSize = kGenericSize,
         kServerInfoSize = kGenericSize,
         kUserStatusSize = kGenericSize,
         kUserLoggedOnAndMaxSize = kGenericSize,
         kHttpServerLocationSize = kGenericSize,
         kRoomUserListSize = kGenericSize,
         kServerUserListSize = kGenericSize,
         kServerRoomListSize = kGenericSize,
         kRoomDescendedSize = kGenericSize,
         kUserNewSize = kGenericSize,
         kPingSize = kGenericSize,
         kPongSize = kGenericSize,
         kXTalkSize = kGenericSize,
         kXWhisperSize = kGenericSize,
         kTalkSize = kGenericSize,
         kWhisperSize = kGenericSize,
         kAssetIncomingSize = kGenericSize,
         kAssetQuerySize = kGenericSize,
         kMovementSize = kGenericSize,
         kUserColorSize = kGenericSize,
         kUserFaceSize = kGenericSize,
         kUserPropSize = kGenericSize,
         kUserDescriptionSize = kGenericSize,
         kUserRenameSize = kGenericSize,
         kUserLeavingSize = kGenericSize,
         kUserExitRoomSize = kGenericSize,
         kPropMoveSize = kGenericSize,
         kPropDeleteSize = kGenericSize,
         kPropNewSize = kGenericSize,
         kDoorLockSize = kGenericSize,
         kDoorUnlockSize = kGenericSize,
         kPictMoveSize = kGenericSize,
         kSpotStateSize = kGenericSize,
         kSpotMoveSize = kGenericSize,
         kDrawSize = kGenericSize,
         kNavErrorSize = kGenericSize,
         kBlowThruSize = kGenericSize,
         kAuthenticateSize = kGenericSize,
         kAltRoomDescriptionSize = kGenericSize,
         kRoomDescriptionBodySize = 40,
         kRoomDescriptionNetMsgSize = kMinimumSize + kRoomDescriptionBodySize,
         kHttpServerLocationPayloadSize = 0,
      };

      // : public QObject, public QByteArray {};
      class NetMsg : public ByteArray
      {
      public:
         static inline auto SwapI64(i64 value) -> i64 {
            auto result =
                  (((value >> 56) & 0x00000000000000ff) |
                   ((value << 40) & 0x00ff000000000000) |
                   ((value >> 24) & 0x0000000000ff0000) |
                   ((value <<  8) & 0x000000ff00000000) |
                   ((value >>  8) & 0x00000000ff000000) |
                   ((value << 24) & 0x0000ff0000000000) |
                   ((value >> 40) & 0x000000000000ff00) |
                   (static_cast<u64>(value << 56) & 0xff00000000000000));

            return static_cast<i64>(result);
         }

         static inline auto SwapU64(u64 value) -> u64 {
            auto result =
                  (((value >> 56) & 0x00000000000000ff) |
                   ((value << 40) & 0x00ff000000000000) |
                   ((value >> 24) & 0x0000000000ff0000) |
                   ((value <<  8) & 0x000000ff00000000) |
                   ((value >>  8) & 0x00000000ff000000) |
                   ((value << 24) & 0x0000ff0000000000) |
                   ((value >> 40) & 0x000000000000ff00) |
                   (static_cast<u64>(value << 56) & 0xff00000000000000));

            return result;
         }

         static inline auto SwapI32(i32 value) -> i32 {
            auto result =
                  (((value >> 24) & 0x000000ff) |
                   ((value <<  8) & 0x00ff0000) |
                   ((value >>  8) & 0x0000ff00) |
                   (static_cast<u32>(value << 24) & 0xff000000));

            return static_cast<i32>(result);
         }

         static inline auto SwapU32(u32 unswapped) -> u32 {
            return (((unswapped >> 24) & 0x000000ff) |
                    ((unswapped <<  8) & 0x00ff0000) |
                    ((unswapped >>  8) & 0x0000ff00) |
                    ((unswapped << 24) & 0xff000000));
         }

         static inline auto SwapI16(i16 value) -> i16 {
            return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
         }

         static auto SwapU16(u16 value) -> u16 {
            return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
         }

         NetMsg(NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         NetMsg(
               const HostByteOrder client_byteorder,
               const HostByteOrder server_byteorder,
               NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         NetMsg(
               const char* data,
               int len,
               NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         NetMsg(
               const QByteArray& ba,
               NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         NetMsg(
               const NetMsg& netmsg,
               NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         virtual ~NetMsg(void);

//         inline auto to_byte_array(void) const -> const ByteArray& {
//            return my_rep_;
//         }

//         inline auto to_bytearray_mut(void) -> ByteArray& {
//            return my_rep_;
//         }

//         inline auto from_bytearray(ByteArray& value) -> void {
//            my_rep_ = value;
//         }

         inline auto ShouldSwapEndianness(void) const -> bool {
            return my_swap_endian_flag_;
         }

         inline auto SetShouldSwapEndianness(bool value) -> void {
            my_swap_endian_flag_ = value;
         }

         inline auto ClientByteOrder(void) const -> HostByteOrder {
            return my_client_byte_order_;
         }

         inline auto SetClientByteOrder(HostByteOrder value) -> void {
            my_client_byte_order_ = value;
         }

         inline auto ServerByteOrder(void) const -> HostByteOrder {
            return my_server_byte_order_;
         }

         inline auto SetServerByteOrder(HostByteOrder value) -> void {
            my_server_byte_order_ = value;
         }

         inline auto Id(void) const -> u32 {
            return U32At(kIdOffset);
         }

         inline auto SetId(u32 value) -> void {
            SetU32At(kIdOffset, value);
         }

         inline auto Len(void) const -> i32 {
            return I32At(kLenOffset);
         }

         inline auto SetLen(i32 value) -> void {
            SetI32At(kLenOffset, value);
         }

         inline auto Ref(void) const -> u32 {
            return U32At(kRefOffset);
         }

         inline auto SetRef(u32 value) -> void {
            SetU32At(kRefOffset, value);
         }

         inline auto Reset(void) -> void {
            reset();
         }

         inline auto ReadInNetMsgFromSocketPtr(
               QTcpSocket* socket_ptr) -> int {
            // read in header
            auto read_header_ok = 0;
            auto len = this->size();
            if (len < NetMsgOffset::kPayloadOffset) {
               read_header_ok =
                     do_read_in_netmsg_header_from_socket_ptr_(socket_ptr);
            }

            auto read_body_ok =
                  do_read_in_netmsg_body_from_socket_ptr_(socket_ptr);
            //int minNetMsgOk = NetMsg::kByteSizeOfHeader <= this->size();

            return read_header_ok && read_body_ok; //&& minNetMsgOk;
         }

//         inline auto pascal_string_at(
//               int offset, u8 maxlen) const -> std::string {
//            return do_pascal_string_at_(offset, maxlen);
//         }

//         inline auto set_pascal_string_at(
//               int offset, const std::string& value) -> void {
//            do_set_pascal_string_at_(offset, value);
//         }

//         inline auto string_at(int offset, int len) const -> std::string {
//            return do_string_at_(offset, len);
//         }

//         inline auto set_string_at(
//               int offset, const std::string& value) -> void {
//            do_set_string_at_(offset, value);
//         }

//         inline auto pascal_qstring_at(int offset, u32 len) const -> QString {
//            return do_pascal_qstring_at_(offset, len);
//         }

//         inline auto set_pascal_qstring_at(
//               int offset, const QString& value) -> void {
//            do_set_pascal_qstring_at_(offset, value);
//         }

//         inline auto qstring_at(int offset, int len) const -> QString {
//            return do_qstring_at_(offset, len);
//         }

//         inline auto set_qstring_at(int offset, const QString& value) -> void {
//            do_set_qstring_at_(offset, value);
//         }

//         inline auto i64_at(int offset) const -> i64 {
//            return do_i64_at_(offset);
//         }

//         inline auto set_i64_at(int offset, i64 value) -> void {
//            do_set_i64_at_(offset, value);
//         }

//         inline auto u64_at(int offset) const -> u64 {
//            return do_u64_at_(offset);
//         }

//         inline auto set_u64_at(int offset, u64 value) -> void {
//            do_set_u64_at_(offset, value);
//         }

//         inline auto i32_at(int offset) const -> i32 {
//            return do_i32_at_(offset);
//         }

//         inline auto set_i32_at(int offset, i32 value) -> void {
//            do_set_i32_at_(offset, value);
//         }

//         inline auto u32_at(int offset) const -> u32 {
//            return do_u32_at(offset);
//         }

//         inline auto set_u32_at(int offset, u32 value) -> void {
//            do_set_u32_at_(offset, value);
//         }

//         inline auto i16_at(int offset) const -> i16 {
//            return do_i16_at_(offset);
//         }

//         inline auto set_i16_at(int offset, i16 value) -> void {
//            do_set_i16_at_(offset, value);
//         }

//         inline auto u16_at(int offset) const -> u16 {
//            return do_u16_at_(offset);
//         }

//         inline auto set_u16_at(int offset, u16 value) -> void {
//            do_set_u16_at_(offset, value);
//         }

//         inline auto i8_at(int offset) const -> i8 {
//            return do_i8_at_(offset);
//         }

//         inline auto set_i8_at(int offset, i8 value) -> void {
//            do_set_i8_at_(offset, value);
//         }

//         inline auto u8_at(int offset) const -> u8 {
//            return do_u8_at_(offset);
//         }

//         inline auto set_u8_at(int offset, u8 value) -> void {
//            do_set_u8_at_(offset, value);
//         }

//         inline auto append_i64(i64 value) -> void {
//            do_append_i64_(value);
//         }

//         inline auto append_u64(u64 value) -> void {
//            do_append_u64_(value);
//         }

//         inline auto append_i32(i32 value) -> void {
//            do_append_i32_(value);
//         }

//         inline auto append_u32(u32 value) -> void {
//            do_append_u32_(value);
//         }

//         inline auto append_i16(i16 value) -> void {
//            do_append_i16_(value);
//         }

//         inline auto append_u16(u16 value) -> void {
//            do_append_u16_(value);
//         }

//         inline auto append_i8(i8 value) -> void {
//            do_append_i8_(value);
//         }

//         inline auto append_u8(u8 value) -> void {
//            do_append_u8_(value);
//         }

//      protected:
//         inline auto rep_ref_(void) const -> const ByteArray& {
//            return my_rep_;
//         }

//         inline auto rep_ref_mut_(void) -> ByteArray& {
//            return my_rep_;
//         }

//         inline auto set_rep_(ByteArray& value) -> void {
//            my_rep_ = value;
//         }

      private:
         //ByteArray my_rep_;
         HostByteOrder my_client_byte_order_;
         HostByteOrder my_server_byte_order_;
         bool my_swap_endian_flag_;

         inline auto do_maybe_swap_i64_(i64 value) const -> i64 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            i64 swapped_value = SwapI64(value);
            auto result =
                  static_cast<i64>(cond) * swapped_value |
                  static_cast<i64>(!cond) * value;

            return result;
         }

         inline auto do_maybe_swap_u64_(u64 value) const -> u64 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            u64 swapped_value = SwapU64(value);
            auto result =
                  static_cast<u64>(cond) * swapped_value |
                  static_cast<u64>(!cond) * value;

            return result;
         }

         inline auto do_maybe_swap_i32_(i32 value) const -> i32 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            auto swapped_value = SwapI32(value);
            auto result =
                  static_cast<i32>(cond) * swapped_value |
                  static_cast<i32>(!cond) * value;

            return result;
         }

         inline auto do_maybe_swap_u32_(u32 value) const -> u32 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            auto swapped_value = SwapU32(value);
            auto result =
                  static_cast<u32>(cond) * swapped_value |
                  static_cast<u32>(!cond) * value;

            return result;
         }

         inline auto do_maybe_swap_u16_(u16 value) const -> u16 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            auto swapped_value = SwapU16(value);
            auto result = static_cast<u16>(
                     static_cast<u16>(cond) * swapped_value |
                     static_cast<u16>(!cond) * value);

            return result;
         }

         inline auto do_maybe_swap_i16_(i16 value) const -> i16 {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = my_swap_endian_flag_;
            auto swapped_value = SwapI16(value);
            auto result = static_cast<i16>(
                     static_cast<i16>(cond) * swapped_value |
                     static_cast<i16>(!cond) * value);

            return result;
         }

//         inline auto do_pascal_string_at_(
//               int offset, u8 maxlen) const -> std::string {
//            return ByteArray::pascal_string_at(offset, maxlen);
//         }

//         inline auto do_set_pascal_string_at_(
//               int offset, const std::string& value) -> void {
//            ByteArray::set_pascal_string_at(offset, value);
//         }

//         inline auto do_set_string_at_(
//               int offset, const std::string& value) -> void {
//            ByteArray::set_string_at(offset, value);
//         }

//         inline auto do_string_at_(int offset, int len) const -> std::string {
//            return ByteArray::string_at(offset, len);
//         }

//         inline auto do_set_pascal_qstring_at_(
//               int offset, const QString& value) -> void {
//            ByteArray::set_pascal_qstring_at(offset, value);
//         }

//         inline auto do_pascal_qstring_at_(
//               int offset, u32 len) const -> QString {
//            return ByteArray::pascal_qstring_at(offset, len);
//         }

//         inline auto do_set_qstring_at_(
//               int offset, const QString& value) -> void {
//            ByteArray::set_qstring_at(offset, value);
//         }

//         inline auto do_qstring_at_(int offset, int len) const -> QString {
//            return ByteArray::qstring_at(offset, len);
//         }

//         inline auto do_i64_at_(int offset) const -> i64 {
//            return do_maybe_swap_i64_(ByteArray::i64_at(offset));
//         }

//         inline auto do_set_i64_at_(int offset, i64 value) -> void {
//            ByteArray::set_i64_at(offset, do_maybe_swap_i64_(value));
//         }

//         inline auto do_u64_at_(int offset) const -> u64 {
//            return do_maybe_swap_u64_(ByteArray::u64_at(offset));
//         }

//         inline auto do_set_u64_at_(int offset, u64 value) -> void {
//            ByteArray::set_u64_at(offset, do_maybe_swap_u64_(value));
//         }

//         inline auto do_i32_at_(int offset) const -> i32 {
//            return do_maybe_swap_i32_(ByteArray::i32_at(offset));
//         }

//         inline auto do_set_i32_at_(int offset, i32 value) -> void {
//            ByteArray::set_i32_at(offset, do_maybe_swap_i32_(value));
//         }

//         inline auto do_u32_at(int offset) const -> u32 {
//            return do_maybe_swap_u32_(ByteArray::u32_at(offset));
//         }

//         inline auto do_set_u32_at_(int offset, u32 value) -> void {
//            ByteArray::set_u32_at(offset, do_maybe_swap_u32_(value));
//         }

//         inline auto do_i16_at_(int offset) const -> i16 {
//            return do_maybe_swap_i16_(ByteArray::i16_at(offset));
//         }

//         inline auto do_set_i16_at_(int offset, i16 value) -> void {
//            ByteArray::set_i16_at(offset, do_maybe_swap_i16_(value));
//         }

//         inline auto do_u16_at_(int offset) const -> u16 {
//            return do_maybe_swap_u16_(ByteArray::u16_at(offset));
//         }

//         inline auto do_set_u16_at_(int offset, u16 value) -> void {
//            ByteArray::set_u16_at(offset, do_maybe_swap_u16_(value));
//         }

//         inline auto do_i8_at_(int offset) const -> i8 {
//            return ByteArray::i8_at(offset);
//         }

//         inline auto do_set_i8_at_(int offset, i8 value) -> void {
//            ByteArray::set_i8_at(offset, value);
//         }

//         inline auto do_u8_at_(int offset) const -> u8 {
//            return ByteArray::u8_at(offset);
//         }

//         inline auto do_set_u8_at_(int offset, u8 value) -> void {
//            ByteArray::set_u8_at(offset, value);
//         }

//         inline auto do_append_i64_(i64 value) -> void {
//            ByteArray::append_i64(do_maybe_swap_i64_(value));
//         }

//         inline auto do_append_u64_(u64 value) -> void {
//            ByteArray::append_u64(do_maybe_swap_u64_(value));
//         }

//         inline auto do_append_i32_(i32 value) -> void {
//            ByteArray::append_i32(do_maybe_swap_i32_(value));
//         }

//         inline auto do_append_u32_(u32 value) -> void {
//            ByteArray::append_u32(do_maybe_swap_u32_(value));
//         }

//         inline auto do_append_i16_(i16 value) -> void {
//            ByteArray::append_i16(do_maybe_swap_i16_(value));
//         }

//         inline auto do_append_u16_(u16 value) -> void {
//            ByteArray::append_u16(do_maybe_swap_u16_(value));
//         }

//         inline auto do_append_i8_(i8 value) -> void {
//            ByteArray::append_i8(value);
//         }

//         inline auto do_append_u8_(u8 value) -> void {
//            ByteArray::append_u8(value);
//         }

         inline auto do_read_in_netmsg_header_from_socket_ptr_(
               QTcpSocket* socket_ptr) -> int {
            auto n_expected_bytes_to_read =
                  NetMsgOffset::kPayloadOffset - size();

            //auto readHeaderOk = 0 <= nExpectedBytesToRead;
            if (0 == n_expected_bytes_to_read)
               return 1;
            else if (n_expected_bytes_to_read < 0)
               return 0;

            auto n_bytes_available = socket_ptr->bytesAvailable();
            auto should_read_partial =
                  n_bytes_available < n_expected_bytes_to_read;
            auto n_bytes_to_read =
                  (should_read_partial * n_bytes_available) |
                  (!should_read_partial * n_expected_bytes_to_read);

            append(socket_ptr->read(n_bytes_to_read));

            //auto netMsgSizeN = this->size();
            //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;
            if (NetMsgSize::kMinimumSize == size())
               reserve(NetMsgSize::kMinimumSize +
                       I32At(NetMsgOffset::kLenOffset));

            return NetMsgSize::kMinimumSize == size();
         }

         inline auto do_read_in_netmsg_body_from_socket_ptr_(
               QTcpSocket* socket_ptr) -> int {
            auto size_0 = size();
            if (size_0 < NetMsgSize::kMinimumSize)
               return 0;

            auto body_size = size_0 - NetMsgSize::kMinimumSize;
            auto n_bytes_expected = this->Len() - body_size;

            if (0 == n_bytes_expected)
               return 1;
            else if (n_bytes_expected < 0)
               return 0;

            auto n_bytes_available = socket_ptr->bytesAvailable();
            auto should_read_partial = n_bytes_available < n_bytes_expected;
            auto n_bytes_to_read =
                  (should_read_partial * n_bytes_available) |
                  (!should_read_partial * n_bytes_expected);

            // maybe reads should be chunked?
            append(socket_ptr->read(n_bytes_to_read));

            //auto sizeN = this->size();
            //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;

            auto n_expected_total_bytes = NetMsgOffset::kPayloadOffset + Len();

            return n_expected_total_bytes == size();
         /*
            if (0 < nNet::MsgBytesExpected)
            {
               doReadDataIntoNet::Msg(
                     netMsg, Net::Msg::kHeaderSize + nNet::MsgBytesExpected);
            }

            auto nNet::MsgBytesReadTotal = netMsg.size();
            auto nPayloadBytesReadTotal =
                  nNet::MsgBytesReadTotal - Net::Msg::kHeaderSize;
            return nPayloadBytesReadTotal == nNet::MsgBytesExpected;
            */
         }

         inline auto do_init(
               const char* data, int len, NetMsgOptions options) -> void {
            reset();

            //int *p = static_cast<int *>(data);
            ////id_ = static_cast<int *>(p)[kPalMsgIdOffset];
            //id_ = p[kPalMsgIdOffset];
            //len_ = p[kPalMsgLenOffset];
            //ref_ = p[kPalMsgRefOffset];
            my_swap_endian_flag_ = options & NetMsgOptions::kEndianSwap;
            append(data, len);
         }

//         inline auto do_init_(
//               const QByteArray& bytes_of_netmsg, NetMsgOptions options)
//               -> void {
//            do_reset_();

//            my_swap_endian_flag_ = options & NetMsgOptions::kEndianSwap;
//            append(bytes_of_netmsg);
//         }

//         inline auto do_init_(
//               const Host::ByteOrder& client_byte_order,
//               const Host::ByteOrder& server_byte_order,
//               NetMsgOptions options) -> void {
//            do_reset_();

//            my_client_byte_order_ = client_byte_order;
//            my_server_byte_order_ = server_byte_order;
//            my_swap_endian_flag_ = options & NetMsgOptions::kEndianSwap;
//         }

         inline auto do_init(NetMsgOptions options) -> void {
            reset();

            my_swap_endian_flag_ = options;
         }

//         inline auto do_init_(
//               const NetMsg& netmsg, NetMsgOptions options) -> void {
//            do_reset_();
//            //id_ = palMsg.id_;
//            //len_ = palMsg.len_;
//            //ref_ = palMsg.ref_;
//            my_swap_endian_flag_ = options & NetMsgOptions::kEndianSwap;
//            append(netmsg);
//         }

         inline auto reset(void) -> void {
            truncate(0);
            reserve(NetMsgSize::kMinimumSize);
         }
      };
   }
}

#endif // SEVILLE_PALACE_NETMSG_GENERIC_H_
