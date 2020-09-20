#ifndef SEVILLE_PALACE_NETMSG_GENERIC_H_
#define SEVILLE_PALACE_NETMSG_GENERIC_H_

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>

#include "seville/base/types.h"
#include "seville/base/bytearray.h"

#include "seville/palace/base/host.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         enum NetMsgOptions {
            kShouldSwapEndianness = true,
            kShouldNotSwapEndianness = false,
         };

         enum Kind {
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

         // Record Sizes (excludes header size)
         //static const u32 kAuthResponseSize =

         // in i32
         static const i32 kSizeOfHeaderInI32 = 3;
         static const i32 kOffsetForIdInI32 = 0;
         static const i32 kOffsetForLenInI32 = 1;
         static const i32 kOffsetForRefInI32 = 2;
         static const i32 kOffsetForPayloadInI32 = 3;

         // in Bytes
         static const i32 kOffsetForIdInBytes = 0;
         static const i32 kOffsetForLenInBytes = 4;
         static const i32 kOffsetForRefInBytes = 8;

         static const i32 kOffsetForPayloadInBytes = 12;

         static const i32 kMinimumSizeInBytes = kOffsetForPayloadInBytes;
         static const i32 kMaximumSizeInBytes = 1024000;

         //static const i32 kOffsetForPayloadInBytes = 12;

         static const i32 kSizeOfGenericInBytes = kOffsetForPayloadInBytes;
         static const i32 kSizeOfLogonInBytes = 128; // 0x80
         static const i32 kSizeOfAltLogonInBytes = 0;
         static const i32 kSizeOfConnectionErrorInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfServerVersionInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfServerInfoInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserStatusInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserLoggedOnAndMaxInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfHttpServerLocationInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfRoomUserListInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfServerUserListInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfServerRoomListInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfRoomDescendedInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserNewInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPingInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPongInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfXTalkInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfXWhisperInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfTalkInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfWhisperInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfAssetIncomingInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfAssetQueryInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfMovementInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserColorInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserFaceInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserPropInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserDescriptionInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserRenameInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserLeavingInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfUserExitRoomInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPropMoveInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPropDeleteInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPropNewInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfDoorLockInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfDoorUnlockInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfPictMoveInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfSpotStateInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfSpotMoveInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfDrawInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfNavErrorInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfBlowThruInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfAuthenticateInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfAltRoomDescriptionInBytes = kSizeOfGenericInBytes;
         static const i32 kSizeOfRoomDescriptionBodyInBytes = 40;
         static const i32 kSizeOfRoomDescriptionInBytes = kOffsetForPayloadInBytes + kSizeOfRoomDescriptionBodyInBytes;

         class  Generic : public ByteArray // public QObject, public QByteArray {};
         {
         public:
            static i64 swapI64(i64 value)
            {
               auto res =
                     (((value >> 56) & 0x00000000000000ff) |
                      ((value << 40) & 0x00ff000000000000) |
                      ((value >> 24) & 0x0000000000ff0000) |
                      ((value <<  8) & 0x000000ff00000000) |
                      ((value >>  8) & 0x00000000ff000000) |
                      ((value << 24) & 0x0000ff0000000000) |
                      ((value >> 40) & 0x000000000000ff00) |
                      (static_cast<u64>(value << 56) & 0xff00000000000000));

               return static_cast<i64>(res);
            }

            static u64 swapU64(u64 value)
            {
               auto res =
                     (((value >> 56) & 0x00000000000000ff) |
                      ((value << 40) & 0x00ff000000000000) |
                      ((value >> 24) & 0x0000000000ff0000) |
                      ((value <<  8) & 0x000000ff00000000) |
                      ((value >>  8) & 0x00000000ff000000) |
                      ((value << 24) & 0x0000ff0000000000) |
                      ((value >> 40) & 0x000000000000ff00) |
                      (static_cast<u64>(value << 56) & 0xff00000000000000));

               return res;
            }

            static i32 swapI32(i32 value)
            {
               auto res =
                     (((value >> 24) & 0x000000ff) |
                      ((value <<  8) & 0x00ff0000) |
                      ((value >>  8) & 0x0000ff00) |
                      (static_cast<u32>(value << 24) & 0xff000000));

               return static_cast<i32>(res);
            }

            static u32 swapU32(u32 unswapped)
            {
               return (((unswapped >> 24) & 0x000000ff) |
                       ((unswapped <<  8) & 0x00ff0000) |
                       ((unswapped >>  8) & 0x0000ff00) |
                       ((unswapped << 24) & 0xff000000));
            }

            static i16 swapI16(i16 value)
            {
               return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
            }

            static u16 swapU16(u16 unswapped)
            {
               return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
            }

         private:
            bool my_shouldSwapEndiannessFlag;
            Host::ByteOrder my_clientByteOrder;
            Host::ByteOrder my_serverByteOrder;

//               u16 doCsbolecW(u16 unswapped) const;
//               i16 doCsbolecI16(i16 unswapped) const;
//               u16 doCsbolecU16(u16 unswapped) const;
//               u32 doCsbolecDw(u32 unswapped) const;
//               i32 doCsbolecI32(i32 unswapped) const;
//               u32 doCsbolecU32(u32 unswapped) const;
//               i64 doCsbolecI64(i64 unswapped) const;
//               u64 doCsbolecU64(u64 unswapped) const;

            void do_reset(void)
            {
               truncate(0);
               reserve(netmsg::kOffsetForPayloadInBytes);
            }

            i64 do_maybeSwapI64(i64 value) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               i64 swappedValue = swapI64(value);
               auto result =
                     static_cast<i64>(cond) * swappedValue |
                     static_cast<i64>(!cond) * value;

               return result;
            }

            u64 do_maybeSwapU64(u64 value) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               u64 swappedValue = swapU64(value);
               auto result =
                     static_cast<u64>(cond) * swappedValue |
                     static_cast<u64>(!cond) * value;

               return result;
            }

            i32 do_maybeSwapI32(i32 value) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               i32 swappedValue = swapI32(value);
               auto result =
                     static_cast<i32>(cond) * swappedValue |
                     static_cast<i32>(!cond) * value;

               return result;
            }

            u32 do_maybeSwapU32(u32 unswapped) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               u32 swapped = swapU32(unswapped);
               auto value =
                     static_cast<u32>(cond) * swapped |
                     static_cast<u32>(!cond) * unswapped;

               return value;
            }

            u16 do_maybeSwapU16(u16 unswapped) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               auto swapped = swapU16(unswapped);
               auto value = static_cast<u16>(
                        static_cast<u16>(cond) * swapped |
                        static_cast<u16>(!cond) * unswapped);

               return value;
            }

            i16 do_maybeSwapI16(i16 unswapped) const
            {
               //auto cond = doDetermineShouldSwapEndianness();
               auto cond = my_shouldSwapEndiannessFlag;
               auto swapped = swapI16(unswapped);
               auto value = static_cast<i16>(
                        static_cast<i16>(cond) * swapped |
                        static_cast<i16>(!cond) * unswapped);

               return value;
            }

   //         qint32 Base::doCsboI32(qint32 unswapped) const
   //         {
   //            return static_cast<qint32>(doMaybeSwapU32(static_cast<quint32>(unswapped)));
   //         }

   //         quint32 Base::doCsboU32(quint32 unswapped) const
   //         {
   //            return doMaybeSwapU32(unswapped);
   //         }

   //         quint16 Base::doCsbolecW(quint16 unswapped) const
   //         {
   //            int cond = myClientByteOrder == Host::ByteOrder::LittleEndian;
   //            quint16 swapped = doSwapU16(unswapped);

   //            return static_cast<quint16>(cond) * swapped | static_cast<quint16>(!cond) * unswapped;
   //         }

   //         qint16 Base::doCsbolecI16(qint16 unswapped) const
   //         {
   //            return static_cast<qint16>(doCsbolecW(static_cast<quint16>(unswapped)));
   //         }

   //         quint16 Base::doCsbolecU16(quint16 unswapped) const
   //         {
   //            return doCsbolecW(unswapped);
   //         }

   //         quint32 Base::doCsbolecDw(quint32 unswapped) const
   //         {
   //            int cond = myClientByteOrder == Host::LittleEndian;
   //            quint32 swapped = doSwapU32(unswapped);

   //            return static_cast<quint32>(cond) * swapped | static_cast<quint32>(!cond) * unswapped;
   //         }

   //         qint32 Base::doCsbolecI32(qint32 unswapped) const
   //         {
   //            return static_cast<qint32>(doCsbolecDw(static_cast<quint32>(unswapped)));
   //         }

   //         quint32 Base::doCsbolecU32(quint32 unswapped) const
   //         {
   //            return doCsbolecDw(unswapped);
   //         }

            void do_setStringU8At(int offset, const std::string& value)
            {
               ByteArray::setStringU32At(offset, value);
            }

            std::string do_stringU8At(int offset, u8 maxlen) const
            {
               return ByteArray::stringU8At(offset, maxlen);
            }

            void do_setStringU32At(int offset, const std::string& value)
            {
               ByteArray::setStringU32At(offset, value);
            }

            std::string do_stringU32At(int offset, u32 maxlen) const
            {
               return ByteArray::stringU32At(offset, maxlen);
            }

            void do_setStringAt(int offset, const std::string& value)
            {
               ByteArray::setStringAt(offset, value);
            }

            std::string do_stringAt(int offset, int len) const
            {
               return ByteArray::stringAt(offset, len);
            }

            void do_setQStringU8At(int offset, const QString& value)
            {
               ByteArray::setQStringU32At(offset, value);
            }

            QString do_qStringU8At(int offset, u32 len) const
            {
               return ByteArray::qStringU32At(offset, len);
            }

            void do_setQStringU32At(int offset, const QString& value)
            {
               ByteArray::setQStringU32At(offset, value);
            }

            QString do_qStringU32At(int offset, u32 maxlen) const
            {
               return ByteArray::qStringU32At(offset, maxlen);
            }

            void do_setQStringAt(int offset, const QString& value)
            {
               ByteArray::setQStringAt(offset, value);
            }

            QString do_qStringAt(int offset, int len) const
            {
               return ByteArray::qStringAt(offset, len);
            }

            i64 do_i64At(int offset) const
            {
               return do_maybeSwapI64(ByteArray::i64At(offset));
            }

            void do_setI64At(int offset, i64 value)
            {
               ByteArray::setI64At(offset, do_maybeSwapI64(value));
            }

            u64 do_u64At(int offset) const
            {
               return do_maybeSwapU64(ByteArray::u64At(offset));
            }

            void do_setU64At(int offset, u64 value)
            {
               ByteArray::setU64At(offset, do_maybeSwapU64(value));
            }

            i32 do_i32At(int offset) const
            {
               return do_maybeSwapI32(ByteArray::i32At(offset));
            }

            void do_setI32At(int offset, i32 value)
            {
               ByteArray::setI32At(offset, do_maybeSwapI32(value));
            }

            u32 do_u32At(int offset) const
            {
               return do_maybeSwapU32(ByteArray::u32At(offset));
            }

            void do_setU32At(int offset, u32 value)
            {
               ByteArray::setU32At(offset, do_maybeSwapU32(value));
            }

            i16 do_i16At(int offset) const
            {
               return do_maybeSwapI16(ByteArray::i16At(offset));
            }

            void do_setI16At(int offset, i16 value)
            {
               ByteArray::setI16At(offset, do_maybeSwapI16(value));
            }

            u16 do_u16At(int offset) const
            {
               return do_maybeSwapU16(ByteArray::u16At(offset));
            }

            void do_setU16At(int offset, u16 value)
            {
               ByteArray::setU16At(offset, do_maybeSwapU16(value));
            }

            i8 do_i8At(int offset) const
            {
               return ByteArray::i8At(offset);
            }

            void do_setI8At(int offset, i8 value)
            {
               ByteArray::setI8At(offset, value);
            }

            u8 do_u8At(int offset) const
            {
               return ByteArray::u8At(offset);
            }

            void do_setU8At(int offset, u8 value)
            {
               ByteArray::setU8At(offset, value);
            }

            void do_appendI64(i64 value)
            {
               ByteArray::appendI64(do_maybeSwapI64(value));
            }

            void do_appendU64(u64 value)
            {
               ByteArray::appendU64(do_maybeSwapU64(value));
            }

            void do_appendI32(i32 value)
            {
               ByteArray::appendI32(do_maybeSwapI32(value));
            }

            void do_appendU32(u32 value)
            {
               ByteArray::appendU32(do_maybeSwapU32(value));
            }

            void do_appendI16(i16 value)
            {
               ByteArray::appendI16(do_maybeSwapI16(value));
            }

            void do_appendU16(u16 value)
            {
               ByteArray::appendU16(do_maybeSwapU16(value));
            }

            void do_appendI8(i8 value)
            {
               ByteArray::appendI8(value);
            }

            void do_appendU8(u8 value)
            {
               ByteArray::appendU8(value);
            }

            int do_readInNetMsgHeaderFromSocketP(QTcpSocket* socket)
            {
               auto nExpectedBytesToRead =
                     netmsg::kOffsetForPayloadInBytes - this->size();

               //auto readHeaderOk = 0 <= nExpectedBytesToRead;
               if (0 == nExpectedBytesToRead)
                  return 1;
               else if (nExpectedBytesToRead < 0)
                  return 0;

               auto nBytesAvailable = socket->bytesAvailable();
               auto shouldReadPartial = nBytesAvailable < nExpectedBytesToRead;
               auto nBytesToRead =
                     (shouldReadPartial * nBytesAvailable) |
                     (!shouldReadPartial * nExpectedBytesToRead);

               this->append(socket->read(nBytesToRead));

               //auto netMsgSizeN = this->size();
               //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;
               if (netmsg::kOffsetForPayloadInBytes == this->size())
                  this->reserve(netmsg::kOffsetForPayloadInBytes +
                                do_i32At(netmsg::kOffsetForLenInBytes));

               return netmsg::kOffsetForPayloadInBytes == this->size();
            }

            int do_readInNetMsgBodyFromSocketP(QTcpSocket* socket)
            {
               auto size0 = this->size();
               if (size0 < netmsg::kOffsetForPayloadInBytes)
                  return 0;

               auto bodySize = size0 - netmsg::kOffsetForPayloadInBytes;
               auto nBytesExpected = this->len() - bodySize;

               if (0 == nBytesExpected)
                  return 1;
               else if (nBytesExpected < 0)
                  return 0;

               auto nBytesAvailable = socket->bytesAvailable();
               auto shouldReadPartial = nBytesAvailable < nBytesExpected;
               auto nBytesToRead =
                     (shouldReadPartial * nBytesAvailable) |
                     (!shouldReadPartial * nBytesExpected);

               // maybe reads should be chunked?
               this->append(socket->read(nBytesToRead));

               //auto sizeN = this->size();
               //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;

               auto nExpectedTotalBytes =
                     netmsg::kOffsetForPayloadInBytes + this->len();

               return nExpectedTotalBytes == this->size();
            /*
               if (0 < nNet::MsgBytesExpected)
               {
                  doReadDataIntoNet::Msg(netMsg, Net::Msg::kHeaderSize + nNet::MsgBytesExpected);
               }

               auto nNet::MsgBytesReadTotal = netMsg.size();
               auto nPayloadBytesReadTotal = nNet::MsgBytesReadTotal - Net::Msg::kHeaderSize;
               return nPayloadBytesReadTotal == nNet::MsgBytesExpected;
               */
            }

            void do_deinit()
            {
            }

            void do_init(const char* data, int len, bool shouldSwapEndianness)
            {
               do_reset();

               //int *p = static_cast<int *>(data);
               ////id_ = static_cast<int *>(p)[kPalMsgIdOffset];
               //id_ = p[kPalMsgIdOffset];
               //len_ = p[kPalMsgLenOffset];
               //ref_ = p[kPalMsgRefOffset];
               my_shouldSwapEndiannessFlag = shouldSwapEndianness;
               append(data, len);
            }

            void do_init(const QByteArray& bytesOfNetMsg, bool shouldSwapEndianness)
            {
               do_reset();

               my_shouldSwapEndiannessFlag = shouldSwapEndianness;
               append(bytesOfNetMsg);
            }

            void do_init(const Host::ByteOrder& clientByteOrder, const Host::ByteOrder& serverByteOrder, bool shouldSwapEndianness)
            {
               do_reset();

               my_clientByteOrder = clientByteOrder;
               my_serverByteOrder = serverByteOrder;
               my_shouldSwapEndiannessFlag = shouldSwapEndianness;
            }

            void do_init(bool shouldSwapEndianness)
            {
               do_reset();

               my_shouldSwapEndiannessFlag = shouldSwapEndianness;
            }

            void do_init(const Generic& netMsg, bool shouldSwapEndianness)
            {
               do_reset();
               //id_ = palMsg.id_;
               //len_ = palMsg.len_;
               //ref_ = palMsg.ref_;
               my_shouldSwapEndiannessFlag = shouldSwapEndianness;
               append(netMsg);
            }

         public:
            void setShouldSwapEndiannessFlag(bool value)
               { my_shouldSwapEndiannessFlag = value; }
            bool shouldSwapEndiannessFlag(void) const
               { return my_shouldSwapEndiannessFlag; }

            void setClientByteOrder(Host::ByteOrder value)
               { my_clientByteOrder = value; }
            Host::ByteOrder clientByteOrder(void) const
               { return my_clientByteOrder; }

            void setServerByteOrder(Host::ByteOrder value)
               { my_serverByteOrder = value; }
            Host::ByteOrder serverByteOrder(void) const
               { return my_serverByteOrder; }

            void setId(u32 value) { do_setU32At(kOffsetForIdInBytes, value); }
            u32 id(void) const { return do_u32At(kOffsetForIdInBytes); }

            void setLen(i32 value)
               { do_setI32At(kOffsetForLenInBytes, value); }
            i32 len(void) const { return do_i32At(kOffsetForLenInBytes); }

            void setRef(u32 value) { do_setU32At(kOffsetForRefInBytes, value); }
            u32 ref(void) const { return do_u32At(kOffsetForRefInBytes); }

//            void setStringU8At(int offset, const std::string& value)
//               { ByteArray::setStringU8At(offset, value); }
//            std::string stringU8At(int offset, u8 maxlen) const
//               { return ByteArray::stringU8At(offset, maxlen); }

//            void setStringU32At(int offset, const std::string& value)
//               { ByteArray::setStringU32At(offset, value); }
//            std::string stringU32At(int offset, u32 maxlen) const
//               { return ByteArray::stringU32At(offset, maxlen); }

//            void setQStringU8At(int offset, const QString& value)
//               { ByteArray::setQStringU8At(offset, value); }
//            QString qStringU8At(int offset, u8 maxlen)
//               { return ByteArray::qStringU8At(offset, maxlen); }

//            void setQStringU32At(int offset, const QString& value)
//               { ByteArray::setQStringU32At(offset, value); }
//            QString qStringU32At(int offset, u32 maxlen) const
//               { return ByteArray::qStringU32At(offset, maxlen); }

            void setI64At(int offset, i64 value)
               { do_setI64At(offset, value); }
            i64 i64At(int offset) const
               { return do_i64At(offset); }

            void setU64At(int offset, u64 value)
               { do_setU64At(offset, value); }
            u64 u64At(int offset) const
               { return do_u64At(offset); }

            void setI32At(int offset, i32 value)
               { do_setI32At(offset, value); }
            i32 i32At(int offset) const
               { return do_i32At(offset); }

            void setU32At(int offset, u32 value)
               { do_setU32At(offset, value); }
            u32 u32At(int offset) const
               { return do_u32At(offset); }

            void setI16At(int offset, i16 value)
               { do_setI16At(offset, value); }
            i16 i16At(int offset) const
               { return do_i16At(offset); }

            void setU16At(int offset, u16 value)
               { do_setU16At(offset, value); }
            u16 u16At(int offset) const
               { return do_u16At(offset); }

            void setI8At(int offset, i8 value)
               { do_setI8At(offset, value); }
            i8 i8At(int offset) const
               { return do_i8At(offset); }

            void setU8At(int offset, u8 value)
               { do_setU8At(offset, value); }
            u8 u8At(int offset) const
               { return do_u8At(offset); }

            void appendI64(i64 value) { do_appendI64(value); }
            void appendU64(u64 value) { do_appendU64(value); }
            void appendI32(i32 value) { do_appendI32(value); }
            void appendU32(u32 value) { do_appendU32(value); }
            void appendI16(i16 value) { do_appendI16(value); }
            void appendU16(u16 value) { do_appendU16(value); }
            void appendI8(i8 value) { do_appendI8(value); }
            void appendU8(u8 value) { do_appendU8(value); }

            void reset(void) { do_reset(); }

            int readInNetMsgFromSocketP(QTcpSocket* pSocket)
            {
               // read in header
               auto readHeaderOk = 0;
               auto size = this->size();
               if (size < netmsg::kOffsetForPayloadInBytes)
                  readHeaderOk = do_readInNetMsgHeaderFromSocketP(pSocket);

               int readBodyOk = do_readInNetMsgBodyFromSocketP(pSocket);
               //int minNetMsgOk = NetMsg::kByteSizeOfHeader <= this->size();

               return readHeaderOk && readBodyOk; //&& minNetMsgOk;
            }

            virtual ~Generic(void);

            Generic(bool shouldSwapEndianness = false);

            Generic(const Host::ByteOrder clientByteOrder,
                    const Host::ByteOrder serverByteOrder,
                    bool shouldSwapEndianness = false);

            Generic(const char* data, int len, bool shouldSwapEndianness = false);
            Generic(const QByteArray& ba, bool shouldSwapEndianness = false);
            Generic(const Generic& netMsg, bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_GENERIC_H_
