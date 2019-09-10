#ifndef PALMSG_H
#define PALMSG_H

#include <stdint.h>

#include <QObject>
#include <QByteArray>
#include <QDataStream>

#include "pal_common.h"

namespace Seville
{
   class PalMsg : public QByteArray // public QObject, public QByteArray {};
   {
      //Q_OBJECT

      public:
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
          static const quint32 kLogonSize = 128; // 0x80
          //static const quint32 kAuthResponseSize =

          // in i32
          static const qint32 kPalMsgHeaderSizeInI32 = 3;
          static const qint32 kPalMsgIdOffsetInI32 = 0;
          static const qint32 kPalMsgLenOffsetInI32 = 1;
          static const qint32 kPalMsgRefOffsetInI32 = 2;
          static const qint32 kPalMsgPayloadOffsetInI32 = 3;

          // in Bytes
          static const qint32 kPalMsgMaxSize = 1024000;
          static const qint32 kHeaderSize = 12;

          static const qint32 kPalMsgIdOffset = 0;
          static const qint32 kPalMsgLenOffset = 4;
          static const qint32 kPalMsgRefOffset = 8;
          static const qint32 kPalMsgPayloadOffset = 12;

          bool puidChanged    = false;
          quint32 puidCounter = 0xf5dc385e;
          quint32 puidCrc     = 0xc144c580;
          quint32 regCounter  = 0xcf07309c;
          quint32 regCrc      = 0x5905f923;

          explicit PalMsg();
          PalMsg(
                PalCommon::ByteOrder clientByteOrder,
                PalCommon::ByteOrder serverByteOrder);
          PalMsg(const char* data);
          PalMsg(QByteArray& ba);
          PalMsg(PalMsg& palMsg);
          virtual ~PalMsg();

          void resetState();

          quint32 msgId() const;
          qint32 msgLen() const;
          quint32 msgRef() const;
          void setMsgId(quint32 msgId);
          void setMsgLen(qint32 msgLen);
          void setMsgRef(quint32 msgRef);

          qint8 i8At(int offset) const;
          quint8 u8At(int offset) const;
          qint16 i16At(int offset) const;
          quint16 u16At(int offset) const;
          qint32 i32At(int offset) const;
          quint32 u32At(int offset) const;

          //void insertI16(int offset, qint16 value);
          //void insertU16(int offset, quint16 value);
          //void insertI32(int offset, qint32 value);
          //void insertU32(int offset, quint32 value);
          void insertW(int offset, quint16 value);
          void insertDw(int offset, quint32 value);

          void appendW(quint16 value);
          void appendDw(quint32 value);

          void appendI8(qint8 value);
          void appendU8(quint8 value);
          void appendI16(qint16 value);
          void appendU16(quint16 value);
          void appendI32(qint32 value);
          void appendU32(quint32 value);

          PalCommon::ByteOrder clientByteOrder() const;
          void setClientByteOrder(PalCommon::ByteOrder byteOrder);

          PalCommon::ByteOrder serverByteOrder() const;
          void setServerByteOrder(PalCommon::ByteOrder byteOrder);

      private:
          void doResetState();
          qint8 doGetI8At(int offset) const;
          quint8 doGetU8At(int offset) const;
          qint16 doGetI16At(int offset) const;
          quint16 doGetU16At(int offset) const;
          qint32 doGetI32At(int offset) const;
          quint32 doGetU32At(int offset) const;

          //void insertI16(int offset, qint16 value);
          //void insertU16(int offset, quint16 value);
          //void insertI32(int offset, qint32 value);
          //void insertU32(int offset, quint32 value);
          void doInsertW(int offset, quint16 value);
          void doInsertDw(int offset, quint32 value);

          void doAppendW(quint16 value);
          void doAppendDw(quint32 value);

          static quint16 doSboW(quint16 unswapped);
          static quint32 doSboDw(quint32 unswapped);
          quint16 doCsboW(quint16 unswapped) const;
          qint16 doCsboI16(qint16 unswapped) const;
          quint16 doCsboU16(quint16 unswapped) const;
          quint32 doCsboDw(quint32 unswapped) const;
          qint32 doCsboI32(qint32 unswapped) const;
          quint32 doCsboU32(quint32 unswapped) const;
          quint16 doCsbolecW(quint16 unswapped) const;
          qint16 doCsbolecI16(qint16 unswapped) const;
          quint16 doCsbolecU16(quint16 unswapped) const;
          quint32 doCsbolecDw(quint32 unswapped) const;
          qint32 doCsbolecI32(qint32 unswapped) const;
          quint32 doCsbolecU32(quint32 unswapped) const;

          PalCommon::ByteOrder myClientByteOrder;
          PalCommon::ByteOrder myServerByteOrder;
   };
}

#endif // PALMSG_H
