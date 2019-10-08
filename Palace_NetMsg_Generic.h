#ifndef PALACE_NETMSG_GENERIC_H
#define PALACE_NETMSG_GENERIC_H

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>

#include "Common.h"
#include "Palace_Host.h"
#include "QtApp_ByteArray.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         enum Kind {
            // == Server ====================================================
            NoKind                 = 0x00000000, // 0x6E696C20 // "nil "
            UnknownServerKind      = 0x70736572, // "pser"
            LittleEndianServerKind = 0x72796974, // "ryit"
            BigEndianServerKind    = 0x74697972, // "tiyr"
            AltLogonKind           = 0x72657032, // "rep2"
            ServerVersionKind      = 0x76657273, // "vers"
            ServerInfoKind         = 0x73696E66, // "sinf"
            UserStatusKind         = 0x75537461, // "uSta"
            UserLoggedOnAndMaxKind = 0x6C6F6720, // "log "
            HttpServerLocationKind = 0x48545450, // "HTTP"
            RoomDescriptionKind    = 0x726F6F6D, // "room"
            AltRoomDescriptionKind = 0x73526F6D, // "sRom"
            RoomUserListKind       = 0x72707273, // "rprs"
            ServerRoomListKind     = 0x724C7374, // "rLst"
            RoomDescendKind        = 0x656E6472, // "endr"
            UserNewKind            = 0x6E707273, // "nprs"
            PingKind               = 0x70696E67, // "ping"
            PongKind               = 0x706F6E67, // "pong"
            XTalkKind              = 0x78746C6B, // "xtlk" (encrypted)
            XWhisperKind           = 0x78776973, // "xwis" (encrypted)
            TalkKind               = 0x74616C6B, // "talk" (unencrypted)
            WhisperKind            = 0x77686973, // "whis" (unencrypted)
            MovementKind           = 0x754C6F63, // "uLoc"
            UserColorKind          = 0x75737243, // "usrC"
            UserDescriptionKind    = 0x75737244, // "usrD"
            UserFaceKind           = 0x75737246, // "usrF"
            UserPropKind           = 0x75737250, // "usrP"
            UserRenameKind         = 0x7573724E, // "usrN"
            UserLeavingKind        = 0x62796520, // "bye "
            FileIncomingKind       = 0x7346696C, // "sFil"
            AssetIncomingKind      = 0x73417374, // "sAst"
            UserExitRoomKind       = 0x65707273, // "eprs"
            ServerUserListKind     = 0x754C7374, // "uLst"
            DoorLockKind           = 0x6C6F636B, // "lock"
            DoorUnlockKind         = 0x756E6C6F, // "unlo"
            SpotStateKind          = 0x73537461, // "sSta"
            SpotMoveKind           = 0x636F4C73, // "coLs"
            PictMoveKind           = 0x704C6F63, // "pLoc"
            DrawKind               = 0x64726177, // "draw"
            PropMoveKind           = 0x6D507270, // "mPrp"
            PropDeleteKind         = 0x64507270, // "dPrp"
            PropNewKind            = 0x6E507270, // "nPrp"
            AssetQueryKind         = 0x71417374, // "qAst"
            NavErrorKind           = 0x73457272, // "sErr"
            ConnectionErrorKind    = 0x646F776E, // "down"
            BlowThruKind           = 0x626C6F77, // "blow"
            AuthenticateKind       = 0x61757468, // "auth"
            // == Room =======================================================
            GotoRoomKind           = 0x6E617652, // "navR"
            RoomKind               = 0x30C6015D, // "0..]"
            SuperUserKind          = 0x73757372, // "susr"
            LogonKind              = 0x72656769, // "regi"
            AssetRegiKind          = 0x72417374, // "rAst"
            GlobalMessageKind      = 0x676D7367, // "gmsg"
            RoomMessageKind        = 0x726D7367, // "rmsg"
            SuperUserMessageKind   = 0x736D7367, // "smsg"
            AuthResponseKind       = 0x61757472, // "autr"
         };

         // Record Sizes (excludes header size)
         //static const u32 kAuthResponseSize =

         // in i32
         static const i32 kI32SizeOfHeader = 3;
         static const i32 kI32OffsetForId = 0;
         static const i32 kI32OffsetForLen = 1;
         static const i32 kI32OffsetForRef = 2;
         static const i32 kI32OffsetForPayload = 3;

         // in Bytes
         static const i32 kByteSizeOfHeader = 12;
         static const i32 kByteSizeMinimum = kByteSizeOfHeader;
         static const i32 kByteSizeMaximum = 1024000;

         static const i32 kByteOffsetForId = 0;
         static const i32 kByteOffsetForLen = 4;
         static const i32 kByteOffsetForRef = 8;
         static const i32 kByteOffsetForPayload = 12;

         static const i32 kByteSizeOfGeneric = kByteSizeOfHeader;
         static const i32 kByteSizeOfLogon = 128; // 0x80
         static const i32 kByteSizeOfAltLogon = 0;
         static const i32 kByteSizeOfConnectionError = kByteSizeOfGeneric;
         static const i32 kByteSizeOfServerVersion = kByteSizeOfGeneric;
         static const i32 kByteSizeOfServerInfo = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserStatus = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserLoggedOnAndMax = kByteSizeOfGeneric;
         static const i32 kByteSizeOfHttpServerLocation = kByteSizeOfGeneric;
         static const i32 kByteSizeOfRoomUserList = kByteSizeOfGeneric;
         static const i32 kByteSizeOfServerUserList = kByteSizeOfGeneric;
         static const i32 kByteSizeOfServerRoomList = kByteSizeOfGeneric;
         static const i32 kByteSizeOfRoomDescended = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserNew = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPing = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPong = kByteSizeOfGeneric;
         static const i32 kByteSizeOfXTalk = kByteSizeOfGeneric;
         static const i32 kByteSizeOfXWhisper = kByteSizeOfGeneric;
         static const i32 kByteSizeOfTalk = kByteSizeOfGeneric;
         static const i32 kByteSizeOfWhisper = kByteSizeOfGeneric;
         static const i32 kByteSizeOfAssetIncoming = kByteSizeOfGeneric;
         static const i32 kByteSizeOfAssetQuery = kByteSizeOfGeneric;
         static const i32 kByteSizeOfMovement = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserColor = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserFace = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserProp = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserDescription = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserRename = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserLeaving = kByteSizeOfGeneric;
         static const i32 kByteSizeOfUserExitRoom = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPropMove = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPropDelete = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPropNew = kByteSizeOfGeneric;
         static const i32 kByteSizeOfDoorLock = kByteSizeOfGeneric;
         static const i32 kByteSizeOfDoorUnlock = kByteSizeOfGeneric;
         static const i32 kByteSizeOfPictMove = kByteSizeOfGeneric;
         static const i32 kByteSizeOfSpotState = kByteSizeOfGeneric;
         static const i32 kByteSizeOfSpotMove = kByteSizeOfGeneric;
         static const i32 kByteSizeOfDraw = kByteSizeOfGeneric;
         static const i32 kByteSizeOfNavError = kByteSizeOfGeneric;
         static const i32 kByteSizeOfBlowThru = kByteSizeOfGeneric;
         static const i32 kByteSizeOfAuthenticate = kByteSizeOfGeneric;
         static const i32 kByteSizeOfAltRoomDescription = kByteSizeOfGeneric;
         static const i32 kByteSizeOfRoomDescriptionBody = 40;
         static const i32 kByteSizeOfRoomDescription = kByteSizeOfHeader + kByteSizeOfRoomDescriptionBody;

         class Generic : public QtApp::ByteArray // public QObject, public QByteArray {};
         {
            private:
               bool myShouldSwapEndiannessFlag;
               Host::ByteOrder myClientByteOrder;
               Host::ByteOrder myServerByteOrder;

               void doReset();

               i64 doGetI64At(int offset) const;
               void doSetI64At(int offset, i64 value);

               u64 doGetU64At(int offset) const;
               void doSetU64At(int offset, u64 value);

               i32 doGetI32At(int offset) const;
               void doSetI32At(int offset, i32 value);

               u32 doGetU32At(int offset) const;
               void doSetU32At(int offset, u32 value);

               i16 doGetI16At(int offset) const;
               void doSetI16At(int offset, i16 value);

               u16 doGetU16At(int offset) const;
               void doSetU16At(int offset, u16 value);

               i8 doGetI8At(int offset) const;
               void doSetI8At(int offset, i8 value);

               u8 doGetU8At(int offset) const;
               void doSetU8At(int offset, u8 value);

               void doAppendI64(i64 value);
               void doAppendU64(u64 value);
               void doAppendI32(i32 value);
               void doAppendU32(u32 value);
               void doAppendI16(i16 value);
               void doAppendU16(u16 value);
               void doAppendI8(i8 value);
               void doAppendU8(u8 value);

               static i64 doSwapI64(i64 value);
               static u64 doSwapU64(u64 value);
               static i32 doSwapI32(i32 value);
               static u32 doSwapU32(u32 value);
               static u16 doSwapU16(u16 value);
               static i16 doSwapI16(i16 value);

               i64 doMaybeSwapI64(i64 value) const;
               u64 doMaybeSwapU64(u64 value) const;
               i32 doMaybeSwapI32(i32 value) const;
               u32 doMaybeSwapU32(u32 value) const;
               i16 doMaybeSwapI16(i16 value) const;
               u16 doMaybeSwapU16(u16 value) const;

//               u16 doCsbolecW(u16 unswapped) const;
//               i16 doCsbolecI16(i16 unswapped) const;
//               u16 doCsbolecU16(u16 unswapped) const;
//               u32 doCsbolecDw(u32 unswapped) const;
//               i32 doCsbolecI32(i32 unswapped) const;
//               u32 doCsbolecU32(u32 unswapped) const;
//               i64 doCsbolecI64(i64 unswapped) const;
//               u64 doCsbolecU64(u64 unswapped) const;

               int doReadHeaderFrom(QTcpSocket* socket);
               int doReadBodyFrom(QTcpSocket* socket);

            public:
               bool shouldSwapEndiannessFlag()
                  { return myShouldSwapEndiannessFlag; }
               void setShouldSwapEndiannessFlag(bool value)
                  { myShouldSwapEndiannessFlag = value; }

               Host::ByteOrder clientByteOrder() const
                  { return myClientByteOrder; }
               void setClientByteOrder(Host::ByteOrder value)
                  { myClientByteOrder = value; }

               Host::ByteOrder serverByteOrder() const
                  { return myServerByteOrder; }
               void setServerByteOrder(Host::ByteOrder value)
                  { myServerByteOrder = value; }

               u32 kind() const { return doGetU32At(kByteOffsetForId); }
               void setKind(u32 value) { doSetU32At(kByteOffsetForId, value); }

               i32 sizeExpected() const { return doGetI32At(kByteOffsetForLen); }
               void setSizeExpected(i32 value)
                  { doSetI32At(kByteOffsetForLen, value); }

               u32 objectId() const { return doGetU32At(kByteOffsetForRef); }
               void setObjectId(u32 value) { doSetU32At(kByteOffsetForRef, value); }

               std::string shortStdStringAt(int offset);
               void setShortStdStringAt(int offset, std::string value);

               std::string doGetStdStringAt(int offset, int maxlen);
               void doSetStdStringAt(int offset, std::string value);

               i64 i64At(int offset) const
                  { return doGetI64At(offset); }
               void setI64At(int offset, i64 value)
                  { doSetI64At(offset, value); }

               u64 u64At(int offset) const
                  { return doGetU64At(offset); }
               void setU64At(int offset, u64 value)
                  { doSetU64At(offset, value); }

               i32 i32At(int offset) const
                  { return doGetI32At(offset); }
               void setI32At(int offset, i32 value)
                  { doSetI32At(offset, value); }

               u32 u32At(int offset) const
                  { return doGetU32At(offset); }
               void setU32At(int offset, u32 value)
                  { doSetU32At(offset, value); }

               i16 i16At(int offset) const
                  { return doGetI16At(offset); }
               void setI16At(int offset, i16 value)
                  { doSetI16At(offset, value); }

               u16 u16At(int offset) const
                  { return doGetU16At(offset); }
               void setU16At(int offset, u16 value)
                  { doSetU16At(offset, value); }

               i8 i8At(int offset) const
                  { return doGetI8At(offset); }
               void setI8At(int offset, i8 value)
                  { doSetI8At(offset, value); }

               u8 u8At(int offset) const
                  { return doGetU8At(offset); }
               void setU8At(int offset, u8 value)
                  { doSetU8At(offset, value); }

               void appendI64(i64 value) { doAppendI64(value); }
               void appendU64(u64 value) { doAppendU64(value); }
               void appendI32(i32 value) { doAppendI32(value); }
               void appendU32(u32 value) { doAppendU32(value); }
               void appendI16(i16 value) { doAppendI16(value); }
               void appendU16(u16 value) { doAppendU16(value); }
               void appendI8(i8 value) { doAppendI8(value); }
               void appendU8(u8 value) { doAppendU8(value); }

               int readFrom(QTcpSocket* socket);

               void reset() { doReset(); }

               virtual ~Generic();

               explicit Generic(bool shouldSwapEndianness = false);

               Generic(Host::ByteOrder clientByteOrder,
                       Host::ByteOrder serverByteOrder,
                       bool shouldSwapEndianness = false);

               Generic(const char* data, bool shouldSwapEndianness = false);

               Generic(QByteArray& ba, bool shouldSwapEndianness = false);

               Generic(Generic& netMsg, bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // PALACE_NETMSG_GENERIC_H
