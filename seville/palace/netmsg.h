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
      enum NetMsgOptions: bool {
         kNoEndianSwap = 0,
         kEndianSwap = 1,
      };

      // : public QObject, public QByteArray {};
      class NetMsg : public ByteArray
      {
      public:
         //static const i32 kNumHeaderFlags = 3;

         enum NetMsgKind: i32 {
            // == Server ====================================================
            kNoKind                   = 0x00000000, // 0x6E696C20 // "nil "
            kUnknownServerKind        = 0x70736572, // "pser"
            kServerEndianMismatchKind = 0x72796974, // "ryit"
            kServerEndianMatchKind    = 0x74697972, // "tiyr"
            kAltLogonKind             = 0x72657032, // "rep2"
            kServerVersionKind        = 0x76657273, // "vers"
            kServerInfoKind           = 0x73696E66, // "sinf"
            kUserStatusKind           = 0x75537461, // "uSta"
            kUserLoggedOnAndMaxKind   = 0x6C6F6720, // "log "
            kHttpServerLocationKind   = 0x48545450, // "HTTP"
            kRoomDescriptionKind      = 0x726F6F6D, // "room"
            kAltRoomDescriptionKind   = 0x73526F6D, // "sRom"
            kRoomUserListKind         = 0x72707273, // "rprs"
            kServerRoomListKind       = 0x724C7374, // "rLst"
            kRoomDescendKind          = 0x656E6472, // "endr"
            kUserNewKind              = 0x6E707273, // "nprs"
            kPingKind                 = 0x70696E67, // "ping"
            kPongKind                 = 0x706F6E67, // "pong"
            kXTalkKind                = 0x78746C6B, // "xtlk" (enciphered)
            kXWhisperKind             = 0x78776973, // "xwis" (enciphered)
            kTalkKind                 = 0x74616C6B, // "talk" (plain)
            kWhisperKind              = 0x77686973, // "whis" (plain)
            kMovementKind             = 0x754C6F63, // "uLoc"
            kUserColorKind            = 0x75737243, // "usrC"
            kUserDescriptionKind      = 0x75737244, // "usrD"
            kUserFaceKind             = 0x75737246, // "usrF"
            kUserPropKind             = 0x75737250, // "usrP"
            kUserRenameKind           = 0x7573724E, // "usrN"
            kUserLeavingKind          = 0x62796520, // "bye "
            kFileIncomingKind         = 0x7346696C, // "sFil"
            kAssetIncomingKind        = 0x73417374, // "sAst"
            kUserExitRoomKind         = 0x65707273, // "eprs"
            kServerUserListKind       = 0x754C7374, // "uLst"
            kDoorLockKind             = 0x6C6F636B, // "lock"
            kDoorUnlockKind           = 0x756E6C6F, // "unlo"
            kSpotStateKind            = 0x73537461, // "sSta"
            kSpotMoveKind             = 0x636F4C73, // "coLs"
            kPictMoveKind             = 0x704C6F63, // "pLoc"
            kDrawKind                 = 0x64726177, // "draw"
            kPropMoveKind             = 0x6D507270, // "mPrp"
            kPropDeleteKind           = 0x64507270, // "dPrp"
            kPropNewKind              = 0x6E507270, // "nPrp"
            kAssetQueryKind           = 0x71417374, // "qAst"
            kNavErrorKind             = 0x73457272, // "sErr"
            kConnectionErrorKind      = 0x646F776E, // "down"
            kBlowThruKind             = 0x626C6F77, // "blow"
            kAuthenticateKind         = 0x61757468, // "auth"
            // == Room =======================================================
            kGotoRoomKind             = 0x6E617652, // "navR"
            kRoomKind                 = 0x30C6015D, // "0..]"
            kSuperUserKind            = 0x73757372, // "susr"
            kLogonKind                = 0x72656769, // "regi"
            kAssetRegiKind            = 0x72417374, // "rAst"
            kGlobalMessageKind        = 0x676D7367, // "gmsg"
            kRoomMessageKind          = 0x726D7367, // "rmsg"
            kSuperUserMessageKind     = 0x736D7367, // "smsg"
            kAuthResponseKind         = 0x61757472, // "autr"
         };

         // in i32
         //      static const i32 kSizeOfNetMsgHeaderInI32 = 3;
         //      static const i32 kOffsetForNetMsgIdInI32 = 0;
         //      static const i32 kOffsetForNetMsgLenInI32 = 1;
         //      static const i32 kOffsetForNetMsgRefInI32 = 2;
         //      static const i32 kOffsetForNetMsgPayloadInI32 = 3;

         // in Bytes
         //static const i32 kOffsetForPayloadInBytes = 12;

         // Record Sizes (excludes header size)
         //static const u32 kAuthResponseSize =
         enum NetMsgSize: i32 {
            kMinimumSize = 12,
            kHeaderSize = 12,
            kMaximumSize = 1024000,
            //kHeaderSize = kMinimumSize,
            kGenericSize = kMinimumSize,
            kLogonSize = 128,
            kAltLogonBodySize = 0,
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
            kRoomDescriptionSize = kMinimumSize + kRoomDescriptionBodySize,
            kHttpServerLocationBodySize = 0,
         };

         enum NetMsgLen: i32 {
            kNoLen = 0,
            kGotoRoomLen = 2,
            kMovementLen = 4,
         };

         enum NetMsgOffset: i32 {
            kIdOffset = 0,
            kLenOffset = 4,
            kRefOffset = 8,
            kBodyOffset = 12,
         };

         enum NetMsgLogonSize: i32 {
            kLogonIdentSize = 6,
            kLogonMaximumUsernameSize = 31,
            kLogonWizardPasswordSize = 31,
         };

         enum NetMsgRef: i32 {
            kNoRef = 0,
         };

         enum NetMsgRoomDescriptionSize: i32 {
            kRoomDescriptionMaximumDefaultNameSize = 255,
            kRoomDescriptionMaximumArtistNameSize = 255,
            kRoomDescriptionMaximumBackgroundImageNameSize = 255,
         };

         enum NetMsgLogonOffset: i32 {
            kLogonRegCrcOffset = NetMsg::kHeaderSize,
            kLogonRegCounterOffset = NetMsg::kHeaderSize+4,
            kLogonUsernameLenOffset = NetMsg::kHeaderSize+8,
            kLogonUsernameOffset = NetMsg::kHeaderSize+9,
            kLogonWizardPasswordLenOffset = NetMsg::kHeaderSize+40,
            kLogonWizardPasswordOffset = NetMsg::kHeaderSize+41,
            kLogonFlagsOffset = NetMsg::kHeaderSize+72,
            kLogonPuidCounterOffset = NetMsg::kHeaderSize+76,
            kLogonPuidCrcOffset = NetMsg::kHeaderSize+80,
            kLogonDemoElapsedOffset = NetMsg::kHeaderSize+84,
            kLogonTotalElapsedOffset = NetMsg::kHeaderSize+88,
            kLogonDemoLimitOffset = NetMsg::kHeaderSize+92,
            kLogonInitialRoomIdOffset = NetMsg::kHeaderSize+96,
            kLogonReservedOffset = NetMsg::kHeaderSize+98, // size: 6
            kLogonUploadRequestedProtocolVersionOffset =
                  NetMsg::kHeaderSize+104,
            kLogonUploadCapabilitiesOffset = NetMsg::kHeaderSize+108,
            kLogonDownloadCapabilitiesOffset = NetMsg::kHeaderSize+112,
            kLogonEngineCapabilities2dOffset = NetMsg::kHeaderSize+116,
            kLogonGraphicsCapabilities2dOffset = NetMsg::kHeaderSize+120,
            kLogonGraphicsCapabilities3dOffset = NetMsg::kHeaderSize+124,
         };

         enum NetMsgOffsetRoomDescription: i32 {
            kNetMsgOffsetRoomDescriptionFlagsOffset =
               NetMsg::kHeaderSize,
            kNetMsgOffsetRoomDescriptionFaceOffset =
               NetMsg::kHeaderSize+4,
            kNetMsgOffsetRoomDescriptionRoomIdOffset =
               NetMsg::kHeaderSize+8,
            kNetMsgOffsetRoomDescriptionRoomNameOffsetOffset =
               NetMsg::kHeaderSize+10,
            kNetMsgOffsetRoomDescriptionBackgroundImageNameOffsetOffset =
               NetMsg::kHeaderSize+12,
            kNetMsgOffsetRoomDescriptionArtistNameOffsetOffset =
               NetMsg::kHeaderSize+14,
            kNetMsgOffsetRoomDescriptionPasswordOffsetOffset =
               NetMsg::kHeaderSize+16,
            kNetMsgOffsetRoomDescriptionHotspotCountOffset =
               NetMsg::kHeaderSize+18,
            kNetMsgOffsetRoomDescriptionHotspotOffsetOffset =
               NetMsg::kHeaderSize+20,
            kNetMsgOffsetRoomDescriptionRoomImageCountOffset =
               NetMsg::kHeaderSize+22,
            kNetMsgOffsetRoomDescriptionBackgroundImageOffsetOffset =
               NetMsg::kHeaderSize+24,
            kNetMsgOffsetRoomDescriptionDrawCommandsCountOffset =
               NetMsg::kHeaderSize+26,
            kNetMsgOffsetRoomDescriptionFirstDrawCommandOffset =
               NetMsg::kHeaderSize+28,
            kNetMsgOffsetRoomDescriptionRoomUserCountOffset =
               NetMsg::kHeaderSize+30,
            kNetMsgOffsetRoomDescriptionLoosePropCountOffset =
               NetMsg::kHeaderSize+32,
            kNetMsgOffsetRoomDescriptionFirstLoosePropOffset =
               NetMsg::kHeaderSize+34,
            kNetMsgOffsetRoomDescriptionReservedOffset =
               NetMsg::kHeaderSize+36,
            kNetMsgOffsetRoomDescriptionRoomDataLenOffset =
               NetMsg::kHeaderSize+38,
            kRoomData = NetMsg::kHeaderSize+40,
         };

         //explicit NetMsg(void);

         NetMsg(NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

//         NetMsg(const char* data, int len,
//                NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

//         NetMsg(const QByteArray& ba,
//                NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

//         NetMsg(const NetMsg& netmsg,
//                NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         //virtual ~NetMsg(void);

         inline auto isValid(void) -> int {
            //return my_isValidFlag;
            return size() == NetMsg::kHeaderSize + static_cast<int>(do_len());
         }

         //inline auto setIsValid(int value) -> void {
         //   my_isValidFlag = value;
         //}

         inline auto id(void) -> u32 {
            return do_id();
         }

         inline auto setId(u32 value) -> void {
            do_setId(value);
         }

         inline auto len(void) -> u32 {
            return do_len();
         }

         inline auto setLen(u32 value) -> void {
            do_setLen(value);
         }

         inline auto ref(void) -> u32 {
            return do_ref();
         }

         inline auto setRef(u32 value) -> void {
            do_setRef(value);
         }

//         inline auto headerSize(void) -> u32 {
//            return do_headerSize();
//         }

         inline auto contentSize(void) -> i32 {
            return do_contentSize();
         }

//         inline auto header(void) -> ByteArray {
//            return static_cast<ByteArray>(mid(0, NetMsg::kHeaderSize));
//         }

//         inline auto appendHeader(ByteArray value) {
//            // TODO probably a better way to manage this
//            if (my_headerSize < value.size())
//               do_setStreamCursorToContentStart()
//            if (my_headerFlags == 0) {
//               value.stream
//            } else if (my_headerFlags == 1) {

//            }
//         }

         inline auto appendContent(u8 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteU8(value);
         }

         inline auto appendContent(i8 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteI8(value);
         }

         inline auto appendContent(u16 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteU16(value);
         }

         inline auto appendContent(i16 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteI16(value);
         }

         inline auto appendContent(u32 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteU32(value);
         }

         inline auto appendContent(i32 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteI32(value);
         }

         inline auto appendContent(u64 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteU64(value);
         }

         inline auto appendContent(i64 value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteI64(value);
         }

         inline auto appendContent(const QString& value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteQString(value);
         }

         inline auto appendContent(const std::string& value) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteString(value);
         }

         inline auto appendContent(const QString& value, int len) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteFixedQString(value, len);
         }

         inline auto appendContent(const std::string& value, int len) -> void {
            do_setStreamCursorToContentEnd();
            streamWriteFixedString(value, len);
         }

//         inline auto appendContent(QByteArray value) -> void {
//            auto cursorPosition =
//                  (do_headerSize() < NetMsg::kHeaderSize) * NetMsg::kHeaderSize
//                  | (NetMsg::kHeaderSize <= do_headerSize()) * size();

//            setStreamCursorPosition(cursorPosition);
//            streamWriteByteArray(value);
//         }

      private:
         //i32 my_headerFlags;
         //i32 my_headerSize;
         //i32 my_contentSize;
         //ByteArray my_headerByteArray;
         //ByteArray my_bodyByteArray;
         //bool my_shouldSwapEndiannessFlag;
         int my_isValidFlag;

         auto do_id(void) -> u32;
         auto do_setId(u32 value) -> void;
         auto do_len(void) -> u32;
         auto do_setLen(u32 value) -> void;
         auto do_ref(void) -> u32;
         auto do_setRef(u32 value) -> void;

         //auto do_headerSize(void) -> u32;
         auto do_contentSize(void) -> u32;

         auto do_setStreamCursorToContentEnd(void) -> void;
         auto do_setStreamCursorToContentStart(void) -> void;

         auto do_clear(void) -> void;
         auto do_init(NetMsgOptions options = NetMsgOptions::kNoEndianSwap)
               -> void;
      };
   }
}

#endif // SEVILLE_PALACE_NETMSG_GENERIC_H_
