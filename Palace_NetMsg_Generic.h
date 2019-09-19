#ifndef PALACE_NETMSG_NETMSGBASE_H
#define PALACE_NETMSG_NETMSGBASE_H

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QDataStream>

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
         static const i32 kByteSizeOfLongestNetMsg = 1024000;
         static const i32 kByteSizeOfHeader = 12;

         static const i32 kByteOffsetForId = 0;
         static const i32 kByteOffsetForLen = 4;
         static const i32 kByteOffsetForRef = 8;
         static const i32 kByteOffsetForPayload = 12;

         static const u32 kByteSizeOfLogon = 128; // 0x80
         static const u32 kByteSizeOfAltLogon = 0;

         class Generic : public QtApp::ByteArray // public QObject, public QByteArray {};
         {
            private:
               Host::ByteOrder myClientByteOrder;
               Host::ByteOrder myServerByteOrder;

               bool myPuidChanged    = false;
               u32 myPuidCounter = 0xf5dc385e;
               u32 myPuidCrc     = 0xc144c580;
               u32 myRegCounter  = 0xcf07309c;
               u32 myRegCrc      = 0x5905f923;

               void doReset();

               bool doDetermineShouldSwap() const {
                  int notUnknownClientByteOrder = myClientByteOrder != Host::ByteOrder::Unknown;
                  int notUnknownServerByteOrder = myServerByteOrder != Host::ByteOrder::Unknown;
                  int notSameByteOrder = myClientByteOrder != myServerByteOrder;
                  return notUnknownClientByteOrder && notUnknownServerByteOrder && notSameByteOrder;
               }

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

            public:
                u32 puidCounter() const { return myPuidCounter; }
                void setPuidCounter(u32 value) { myPuidCounter = value; }

                u32 regCounter() const { return myRegCounter; }
                void setRegCounter(u32 value) { myRegCounter = value; }

                u32 regCrc() const { return myRegCrc; }
                void setRegCrc(u32 value) { myRegCrc = value; }

                bool puidChanged() const { return myPuidChanged; }
                void setPuidChanged(bool value) { myPuidChanged = value; }

                u32 puidCrc() const { return myPuidCrc; }
                void setPuidCrc(u32 value) { myPuidCrc = value; }

                Host::ByteOrder clientByteOrder() const
                  { return myClientByteOrder; }
                void setClientByteOrder(Host::ByteOrder value)
                  { myClientByteOrder = value; }

                Host::ByteOrder serverByteOrder() const
                  { return myServerByteOrder; }
                void setServerByteOrder(Host::ByteOrder value)
                  { myServerByteOrder = value; }

                u32 id() const { return u32At(kByteOffsetForId); }
                void setId(u32 value) { setU32At(kByteOffsetForId, value); }

                i32 len() const { return i32At(kByteOffsetForLen); }
                void setLen(i32 value) { setI32At(kByteOffsetForLen, value); }

                u32 ref() const { return u32At(kByteOffsetForRef); }
                void setRef(u32 value) { setU32At(kByteOffsetForRef, value); }

                i64 i64At(int offset) const;
                void setI64At(int offset, i64 value);

                u64 u64At(int offset) const;
                void setU64At(int offset, u64 value);

                i32 i32At(int offset) const;
                void setI32At(int offset, i32 value);

                u32 u32At(int offset) const;
                void setU32At(int offset, u32 value);

                i16 i16At(int offset) const;
                void setI16At(int offset, i16 value);

                u16 u16At(int offset) const;
                void setU16At(int offset, u16 value);

                i8 i8At(int offset) const;
                void setI8At(int offset, i8 value);

                u8 u8At(int offset) const;
                void setU8At(int offset, u8 value);

                void appendI64(i64 value);
                void appendU64(u64 value);
                void appendI32(i32 value);
                void appendU32(u32 value);
                void appendI16(i16 value);
                void appendU16(u16 value);
                void appendI8(i8 value);
                void appendU8(u8 value);

                void reset() { doReset(); }

                virtual ~Generic();

                explicit Generic();

                Generic(Host::ByteOrder clientByteOrder,
                    Host::ByteOrder serverByteOrder);

                Generic(const char* data);

                Generic(QByteArray& ba);

                Generic(Generic& netMsg);
         };
      }
   }
}

#endif // PALACE_NETMSG_NETMSGBASE_H
