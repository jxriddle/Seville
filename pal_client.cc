#include <QImage>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "applog.h"
#include "pal_client.h"
#include "pal_msg.h"
#include "pal_room.h"
#include "pal_user.h"

namespace Seville
{
   const QString PalClient::kPalClientIdent = "PC4237";

   PalClient::PalClient(QObject* parent) : QObject(parent)
   {
      // wire up events
      //connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
      //connect(&mySocket, SIGNAL())
      connect(&mySocket, &QIODevice::readyRead, this, &PalClient::doOnReadyRead);
      connect(&mySocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &PalClient::doOnSocketError);

      doResetState();
   }

   PalClient::~PalClient()
   {
      mySocket.disconnectFromHost();
   }

   void PalClient::doResetState()
   {
      myTransferTimerId = 0;
      mySocket.disconnectFromHost();
      doDetermineClientByteOrder();
      myState = PalClient::ConnectionState::Disconnected;
      //myLog.resetState();
      myServer.resetState();
      myUser.resetState();
      myCurrentRoom.resetState();
      myPalMsg.resetState();
   }

   void PalClient::connectToHost(
         QString host, int port, QString username, int initialRoom)
   {
      doDisconnectFromHost();
      doConnectToHost(host, port, username, initialRoom);
   }

   void PalClient::connectToHost(QUrl url, QString username, int initialRoom)
   {
      doDisconnectFromHost();
      doConnectToHost(url.host(), url.port(), username, initialRoom);
   }

   void PalClient::connectToHost(QUrl url, int initialRoom)
   {
      doDisconnectFromHost();
      doConnectToHost(url.host(), url.port(), url.userName(), initialRoom);
   }

   void PalClient::disconnectFromHost()
   {
      doDisconnectFromHost();
   }

   void PalClient::doDisconnectFromHost()
   {
      if (myState != PalClient::ConnectionState::Disconnected) {
         mySocket.disconnectFromHost();
      }
   }

   PalClient::ConnectionState PalClient::state()
   {
      return myState;
   }

   PalRoom& PalClient::currentRoom()
   {
      return myCurrentRoom;
   }

   PalUser& PalClient::user()
   {
      return myUser;
   }

   void PalClient::doConnectToHost(
        QString host, int port, QString username, int initialRoom)
   {
      int cond = (0 == port);
      quint16 zHostPort = static_cast<quint16>(
               (cond * kPalClientDefaultPort) | ((!cond) * port));
      quint16 zInitialRoom = static_cast<quint16>(initialRoom);
      qCDebug(appLcIo, "Connecting to palace://%s@%s:%d/%d",
              username.toUtf8().data(),
              host.toUtf8().data(),
              zHostPort, zInitialRoom);

      myUser.setUsername(username);
      myServer.setHost(host);
      myServer.setPort(zHostPort);
      myCurrentRoom.setId(zInitialRoom);

      qCDebug(appLcIo) << "Client is now in Handshaking State";
      myState = ConnectionState::Handshaking;
      mySocket.connectToHost(myServer.host(), myServer.port());
   }

   bool PalClient::doDetermineIsConnected() const
   {
      return mySocket.state() != QTcpSocket::ConnectedState ||
            myState != ConnectionState::Connected;
   }

   void PalClient::doOnReadyRead()
   {
      PalMsg& palMsg = myPalMsg;

      if (myTransferTimerId) {
         killTimer(myTransferTimerId);
         myTransferTimerId = 0;
      }

      while (doReadPalMsg(palMsg))
      {
         if (myState == ConnectionState::Handshaking) {
            if (doDetermineServerByteOrder(palMsg) < 0) {
               qCDebug(appLcIo) << "ERROR: Could not determine server byte order!";
               return;
            }

            quint32 userId = palMsg.msgRef();
            myUser.setId(userId);

            if (doSendLogonPalMsg() < 0) {
               qCDebug(appLcIo) << "ERROR: Could not send Logon PalMsg!";
               return;
            }

            myState = ConnectionState::Connected;
            qCDebug(appLcIo) << "Client is now in Connected State";

            // emit signal?
         }
         else if (myState == ConnectionState::Connected) {
            doRoutePalMsg(palMsg);
         }

         //qCDebug(appLcIo) << "Resetting PalMsg";
         palMsg.resetState();
      }

      myPingTimer.start();
      myPongTime.start();

      // TODO emit signal?
   }

   void PalClient::doOnSocketError()
   {
      qCDebug(appLcIo) << "ERROR: Socket Error!";
   }

   void PalClient::doOnPingTimer(QTimerEvent *pingTimeEvent)
   {
      if (pingTimeEvent->timerId() == myTransferTimerId) {
         killTimer(myTransferTimerId);
         myTransferTimerId = 0;
      }
   }

   int PalClient::doReadDataIntoPalMsg(PalMsg& palMsg, qint32 maxSize)
   {
      if (PalMsg::kPalMsgMaxSize < maxSize) {
         return 0;
      }

      auto nBytesBeforeRead = palMsg.size();
      if (nBytesBeforeRead == PalMsg::kPalMsgMaxSize) {
         return 0;
      }

      auto nBytesAvailable = mySocket.bytesAvailable();
      auto palMsgSize = palMsg.size();
      int isHeader = palMsgSize < PalMsg::kHeaderSize;
      qint64 chunkReadSize =
            (isHeader * PalMsg::kHeaderSize) |
            (!isHeader * kPalClientDefaultChunkReadSize);

      while (0 < nBytesAvailable && palMsgSize < maxSize) {
         //int remainingSize = (maxSize - palMsgSize);
         QByteArray chunk = mySocket.read(chunkReadSize);
         palMsg.append(chunk);
         //int shouldReadMinChunkSize = remainingSize < kPalClientDefaultChunkReadSize;
         //chunkReadSize = (!shouldReadMinChunkSize * kPalClientDefaultChunkReadSize) | (shouldReadMinChunkSize * kPalClientMinChunkReadSize);
         palMsgSize = palMsg.size();
         nBytesAvailable = mySocket.bytesAvailable();
         chunkReadSize = nBytesAvailable;
      }

      int nBytesRead = palMsgSize - nBytesBeforeRead;
      return nBytesRead;
   }

   int PalClient::doReadPalMsgHeader(PalMsg& palMsg)
   {
      if (myTransferTimerId) {
         killTimer(myTransferTimerId);
         myTransferTimerId = 0;
      }

      auto initialPalMsgSize = palMsg.size();
      auto nExpectedBytesToRead = PalMsg::kHeaderSize - initialPalMsgSize;
      if (0 == nExpectedBytesToRead)
         return 1;
      else if (nExpectedBytesToRead < 0)
         return 1;

      auto nBytesAvailable = mySocket.bytesAvailable();
      auto shouldReadPartial = nBytesAvailable < nExpectedBytesToRead;
      auto nBytesToRead = (nBytesAvailable * shouldReadPartial) | (nExpectedBytesToRead * !shouldReadPartial);
      palMsg.append(mySocket.read(nBytesToRead));

      auto finalPalMsgSize = palMsg.size();
      auto nBytesRead = finalPalMsgSize - initialPalMsgSize;
      if (nBytesRead < nExpectedBytesToRead) {
          myTransferTimerId = startTimer(kPalClientTransferTimeout);
      }

      return finalPalMsgSize == PalMsg::kHeaderSize;
   }

   int PalClient::doReadPalMsgBody(PalMsg& palMsg)
   {
      if (myTransferTimerId) {
         killTimer(myTransferTimerId);
         myTransferTimerId = 0;
      }

      auto initialPalMsgSize = palMsg.size();
      if (initialPalMsgSize < PalMsg::kHeaderSize)
         return 0;

       auto nExpectedBytesToRead = palMsg.msgLen() -
             (initialPalMsgSize - PalMsg::kHeaderSize);
       if (0 == nExpectedBytesToRead)
          return 1;
       else if (nExpectedBytesToRead < 0)
          return 0;

       auto nBytesAvailable = mySocket.bytesAvailable();
       auto shouldReadPartial = nBytesAvailable < nExpectedBytesToRead;
       auto nBytesToRead =
             (nBytesAvailable * shouldReadPartial) |
             (nExpectedBytesToRead * !shouldReadPartial);

       // maybe reads should be chunked?
       palMsg.append(mySocket.read(nBytesToRead));

       auto finalPalMsgSize = palMsg.size();
       auto nBytesRead = finalPalMsgSize - initialPalMsgSize;
       if (nBytesRead < nExpectedBytesToRead) {
           myTransferTimerId = startTimer(kPalClientTransferTimeout);
       }

       auto nExpectedTotalBytes = PalMsg::kHeaderSize + palMsg.msgLen();
       return finalPalMsgSize == nExpectedTotalBytes;
   /*
       if (0 < nPalMsgBytesExpected)
       {
           doReadDataIntoPalMsg(palMsg, PalMsg::kHeaderSize + nPalMsgBytesExpected);
       }

       auto nPalMsgBytesReadTotal = palMsg.size();
       auto nPayloadBytesReadTotal = nPalMsgBytesReadTotal - PalMsg::kHeaderSize;
       return nPayloadBytesReadTotal == nPalMsgBytesExpected;
       */
   }

   int PalClient::doReadPalMsg(PalMsg& palMsg)
   {
       int readHeaderOk = doReadPalMsgHeader(palMsg);
       int readBodyOk = doReadPalMsgBody(palMsg);
       int minPalMsgOk = PalMsg::kHeaderSize <= palMsg.size();
       return readHeaderOk && readBodyOk && minPalMsgOk;
   }

   void PalClient::doDetermineClientByteOrder()
   {
       int isLittleEndian =
             (PalCommon::LittleEndian * !PalCommon::isBigEndian());
       int isBigEndian =
             (PalCommon::BigEndian * PalCommon::isBigEndian());
       myByteOrder =
             static_cast<PalCommon::ByteOrder>(isLittleEndian | isBigEndian);
   }

   int PalClient::doDetermineServerByteOrder(const PalMsg& palMsg)
   {
      int res = 0;
      // TODO more consideration here.
      // constants are in big endian byte order,
      // conditionally swapping them if we are a little endian system seems off.
      // but shouldn't we have to swap them all? we don't.

      quint32 palMsgId;
      int isLE = myByteOrder == PalCommon::LittleEndian;
      palMsgId =
            (static_cast<quint32>(isLE) *
             PalCommon::swapU32Bo(palMsg.msgId())) |
            (static_cast<quint32>(!isLE) * palMsg.msgId());

      switch (palMsgId) {
         case PalMsg::UnknownServerKind: {
            qCDebug(appLcIo) << "Server is Unknown Byte Order";
            myServer.setByteOrder(PalCommon::ByteOrder::Unknown);
            res = 0;
            break;
         }
         case PalMsg::BigEndianServerKind: {
            qCDebug(appLcIo) << "Server is Big Endian";
            myServer.setByteOrder(PalCommon::ByteOrder::BigEndian);
            res = 1;
            break;
         }
         case PalMsg::LittleEndianServerKind: {
            qCDebug(appLcIo) << "Server is Little Endian";
            myServer.setByteOrder(PalCommon::ByteOrder::LittleEndian);
            res = 1;
            break;
         }
         default: {
            qCDebug(appLcIo) << "Server declined to indicate its Byte Order";
            myServer.setByteOrder(PalCommon::ByteOrder::Unknown);
            res = 0;
            break;
         }
       }

       return res;
   }

   int PalClient::doRoutePalMsg(const PalMsg& palMsg)
   {
      int res = 0;
      quint32 palMsgId = palMsg.msgId();
      //quint32 zpalMsgId = doCsbolecDw_(palMsgId);
      switch (palMsgId) {
         //case PalMsg::LogonKind:
         //    res = sendLogon_();
         //    break;
         case PalMsg::AltLogonKind: {
            qCDebug(appLcIo) << "Received AltLogon PalMsg";
            res = doHandleAltLogonPalMsg(palMsg);
            break;
         }
         case PalMsg::ConnectionErrorKind: {
            qCDebug(appLcIo) << "Received ConnectionError PalMsg";
            res = doHandleConnectionErrorPalMsg(palMsg);
            break;
         }
         case PalMsg::ServerVersionKind: {
            qCDebug(appLcIo) << "Received ServerVersion PalMsg";
            res = doHandleServerVersionPalMsg(palMsg);
            break;
         }
         case PalMsg::ServerInfoKind: {
            qCDebug(appLcIo) << "Received ServerInfo PalMsg";
            res = doHandleServerInfoPalMsg(palMsg);
            break;
         }
         case PalMsg::UserStatusKind: {
            qCDebug(appLcIo) << "Received UserStatus PalMsg";
            res = doHandleUserStatusPalMsg(palMsg);
            break;
         }
         case PalMsg::UserLoggedOnAndMaxKind: {
            qCDebug(appLcIo) << "Received UserLoggedOnAndMax PalMsg";
            res = doHandleUserLoggedOnAndMaxPalMsg(palMsg);
            break;
         }
         case PalMsg::HttpServerLocationKind: {
            qCDebug(appLcIo) << "Received HttpServerLocation PalMsg";
            res = doHandleHttpServerLocationPalMsg(palMsg);
            break;
         }
         case PalMsg::RoomUserListKind: {
            qCDebug(appLcIo) << "Received RoomUserList PalMsg";
            res = doHandleRoomUserListPalMsg(palMsg);
            break;
         }
         case PalMsg::ServerUserListKind: {
            qCDebug(appLcIo) << "Received ServerUserList PalMsg";
            res = doHandleServerUserListPalMsg(palMsg);
            break;
         }
         case PalMsg::ServerRoomListKind: {
            qCDebug(appLcIo) << "Received ServerRoomList PalMsg";
            res = doHandleServerRoomListPalMsg(palMsg);
            break;
         }
         case PalMsg::RoomDescendKind: {
            qCDebug(appLcIo) << "Received RoomDescend PalMsg";
            res = doHandleRoomDescendPalMsg(palMsg);
            break;
         }
         case PalMsg::UserNewKind: {
            qCDebug(appLcIo) << "Received UserNew PalMsg";
            res = doHandleUserNewPalMsg(palMsg);
            break;
         }
         case PalMsg::PingKind: {
            qCDebug(appLcIo) << "Received Ping PalMsg";
            res = doHandlePingPalMsg(palMsg);
            break;
         }
         case PalMsg::PongKind: {
            qCDebug(appLcIo) << "Received Pong PalMsg";
            res = doHandlePongPalMsg(palMsg);
            break;
         }
         case PalMsg::XTalkKind: {
            qCDebug(appLcIo) << "Received XTalk PalMsg";
            res = doHandleXTalkPalMsg(palMsg);
            break;
         }
         case PalMsg::XWhisperKind: {
            qCDebug(appLcIo) << "Received XWhisper PalMsg";
            res = doHandleXWhisperPalMsg(palMsg);
            break;
         }
         case PalMsg::TalkKind: {
            qCDebug(appLcIo) << "Received Talk PalMsg";
            res = doHandleTalkPalMsg(palMsg);
            break;
         }
         case PalMsg::WhisperKind: {
            qCDebug(appLcIo) << "Recieved Whisper PalMsg";
            res = doHandleWhisperPalMsg(palMsg);
            break;
         }
         case PalMsg::AssetIncomingKind: {
            qCDebug(appLcIo) << "Received AssetIncoming PalMsg";
            res = doHandleAssetIncomingPalMsg(palMsg);
            break;
         }
         case PalMsg::AssetQueryKind: {
            qCDebug(appLcIo) << "Received AssetQuery PalMsg";
            res = doHandleAssetQueryPalMsg(palMsg);
            break;
         }
         case PalMsg::MovementKind: {
            qCDebug(appLcIo) << "Received Movement PalMsg";
            res = doHandleMovementPalMsg(palMsg);
            break;
         }
         case PalMsg::UserColorKind: {
            qCDebug(appLcIo) << "Received UserColor PalMsg";
            res = doHandleUserColorPalMsg(palMsg);
            break;
         }
         case PalMsg::UserFaceKind: {
            qCDebug(appLcIo) << "Received UserFace PalMsg";
            res = doHandleUserFacePalMsg(palMsg);
            break;
         }
         case PalMsg::UserPropKind: {
            qCDebug(appLcIo) << "Received UserProp PalMsg";
            res = doHandleUserPropPalMsg(palMsg);
            break;
         }
         case PalMsg::UserDescriptionKind: {
            qCDebug(appLcIo) << "Received UserDescription PalMsg";
            res = doHandleUserDescriptionPalMsg(palMsg);
            break;
         }
         case PalMsg::UserRenameKind: {
            qCDebug(appLcIo) << "Received UserRename PalMsg";
            res = doHandleUserRenamePalMsg(palMsg);
            break;
         }
         case PalMsg::UserLeavingKind: {
            qCDebug(appLcIo) << "Received UserLeaving PalMsg";
            res = doHandleUserLeavingPalMsg(palMsg);
            break;
         }
         case PalMsg::UserExitRoomKind: {
            qCDebug(appLcIo) << "Received UserExitRoom PalMsg";
            res = doHandleUserExitRoomPalMsg(palMsg);
            break;
         }
         case PalMsg::PropMoveKind: {
            qCDebug(appLcIo) << "Received PropMove PalMsg";
            res = doHandlePropMovePalMsg(palMsg);
            break;
         }
         case PalMsg::PropDeleteKind: {
            qCDebug(appLcIo) << "Received PropDelete PalMsg";
            res = doHandlePropDeletePalMsg(palMsg);
            break;
         }
         case PalMsg::PropNewKind: {
            qCDebug(appLcIo) << "Received PropNew PalMsg";
            res = doHandlePropNewPalMsg(palMsg);
            break;
         }
         case PalMsg::DoorLockKind: {
            qCDebug(appLcIo) << "Received DoorLock PalMsg";
            res = doHandleDoorLockPalMsg(palMsg);
            break;
         }
         case PalMsg::DoorUnlockKind: {
            qCDebug(appLcIo) << "Received DoorUnlock PalMsg";
            res = doHandleDoorUnlockPalMsg(palMsg);
            break;
         }
         case PalMsg::PictMoveKind: {
            qCDebug(appLcIo) << "Received PictMove PalMsg";
            res = doHandlePictMovePalMsg(palMsg);
            break;
         }
         case PalMsg::SpotStateKind: {
            qCDebug(appLcIo) << "Received SpotState PalMsg";
            res = doHandleSpotStatePalMsg(palMsg);
            break;
         }
         case PalMsg::SpotMoveKind: {
            qCDebug(appLcIo) << "Received SpotMove PalMsg";
            res = doHandleSpotMovePalMsg(palMsg);
            break;
         }
         case PalMsg::DrawKind: {
            qCDebug(appLcIo) << "Received Draw PalMsg";
            res = doHandleDrawPalMsg(palMsg);
            break;
         }
         case PalMsg::NavErrorKind: {
            qCDebug(appLcIo) << "Received NavError PalMsg";
            res = doHandleNavErrorPalMsg(palMsg);
            break;
         }
         case PalMsg::BlowThruKind: {
            qCDebug(appLcIo) << "Received BlowThru PalMsg";
            res = doHandleBlowThruPalMsg(palMsg);
            break;
         }
         case PalMsg::AuthenticateKind: {
            qCDebug(appLcIo) << "Received Authenticate PalMsg";
            res = doHandleAuthenticatePalMsg(palMsg);
            break;
         }
         case PalMsg::AltRoomDescriptionKind: {
            qCDebug(appLcIo) << "Received AltRoomDescription PalMsg";
            res = doHandleRoomDescriptionPalMsg(palMsg);
            break;
         }
         case PalMsg::RoomDescriptionKind: {
            qCDebug(appLcIo) << "Received RoomDescription PalMsg";
            res = doHandleRoomDescriptionPalMsg(palMsg);
            break;
         }
         default: {
            qCDebug(appLcIo) << "Received UNKNOWN PalMsg";
            qCDebug(appLcIo) << "id:  " << QString("0x%1").arg(palMsg.msgId(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "len: " << QString("0x%1").arg(palMsg.msgLen(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "ref: " << QString("0x%1").arg(palMsg.msgRef(), 2, 16, QChar('0'));
            res = 0;
            break;
         }
      }

      return res;
   }

   int PalClient::doHandleAltLogonPalMsg(const PalMsg& palMsg)
   {
      int res = 0;

      //QDataStream ds(palMsgRx_);
      /* Skip header */
      //ds.skipRawData(PalMsg::kPalMsgHeaderSize);
      //quint32 crc; ds >> crc;
      //quint32 counter; ds >> counter;
      //quint8 usernameLen; ds >> usernameLen;
      int offset = PalMsg::kHeaderSize;
      quint32 crc = palMsg.u32At(offset);
      offset += sizeof(crc);
      quint32 counter = palMsg.u32At(offset);
      offset += sizeof(counter);

      /* Username */
      quint8 nUsernameChars = palMsg.u8At(offset);
      offset += sizeof(nUsernameChars);

      quint8 iUsernameChars = 0;
      while (iUsernameChars < nUsernameChars) {
         quint8 ch = palMsg.u8At(offset);
         offset += sizeof(ch);
         myUser.username().append(ch);
         iUsernameChars++;
      }

      while (iUsernameChars < kPalClientMaxUsernameLen) {
         // TODO wizpass
         offset += sizeof(quint8);
         iUsernameChars++;
      }

      quint8 nWizpassChars = palMsg.u8At(offset);
      offset += sizeof(nWizpassChars);

      quint8 iWizpassChars = 0;
      while (iWizpassChars < kPalClientMaxWizpassLen) {
         offset += sizeof(quint8);
         iWizpassChars++;
      }

      quint32 auxFlags = palMsg.u32At(offset);
      offset += sizeof(auxFlags);
      quint32 puidCounter = palMsg.u32At(offset);
      offset += sizeof(puidCounter);
      quint32 puidCrc = palMsg.u32At(offset);
      offset += sizeof(puidCrc);
      quint32 demoElapsed = palMsg.u32At(offset);
      offset += sizeof(demoElapsed);
      quint32 demoLimit = palMsg.u32At(offset);
      offset += sizeof(demoLimit);
      quint32 desiredRoom = palMsg.u32At(offset);
      offset += sizeof(desiredRoom);

      QString reserved;
      for (int i = 0; i < kPalClientIdentLen; i++) {
         quint8 ch = palMsg.u8At(offset);
         offset += sizeof(quint8);
         reserved.append(ch);
      }

      quint32 ulReqProtoVer = palMsg.u32At(offset);
      offset += sizeof(ulReqProtoVer);
      quint32 ulUploadCaps = palMsg.u32At(offset);
      offset += sizeof(ulUploadCaps);
      quint32 ulDownloadCaps = palMsg.u32At(offset);
      offset += sizeof(ulDownloadCaps);
      quint32 ul2dEngineCaps = palMsg.u32At(offset);
      offset += sizeof(ul2dEngineCaps);
      quint32 ul2dGraphicsCaps = palMsg.u32At(offset);
      offset += sizeof(ul2dGraphicsCaps);
      quint32 ul3dEngineCaps = palMsg.u32At(offset);
      offset += sizeof(ul3dEngineCaps);

      if (myUser.idCounter() != puidCounter || myUser.idCrc() != puidCrc) {
         myUser.setIdCrc(puidCrc);
         myUser.setIdCounter(puidCounter);
         myUser.setIdChanged(true);
      }

      res = 1;
      return res;
   }

   int PalClient::doHandleAuthenticatePalMsg(const PalMsg& palMsg)
   {
      (void)palMsg;
      int res = 0;
      // raise event?
      //return doSendAuthenticatePalMsg();

      return res;
   }

   int PalClient::doHandleConnectionErrorPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleServerVersionPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleServerInfoPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserStatusPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserLoggedOnAndMaxPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleHttpServerLocationPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleRoomUserListPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleServerUserListPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleServerRoomListPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleRoomDescendPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserNewPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePingPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePongPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleXTalkPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleXWhisperPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
     (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleTalkPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleWhisperPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleAssetIncomingPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleAssetQueryPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleMovementPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserColorPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserFacePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserPropPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserDescriptionPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserRenamePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserLeavingPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleUserExitRoomPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePropMovePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePropDeletePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePropNewPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleDoorLockPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleDoorUnlockPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandlePictMovePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleSpotStatePalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleSpotMovePalMsg(const PalMsg& palMsg)
   {
      //TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleDrawPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleNavErrorPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleBlowThruPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doHandleRoomDescriptionPalMsg(const PalMsg& palMsg)
   {
      // TODO stub
      (void)palMsg;
      int res = 0;
      return res;
   }

   int PalClient::doSendLogonPalMsg()
   {
      int res = 0;

      PalMsg logonMsg(myByteOrder, myServer.byteOrder());
      //palMsgRxDs_->setByteOrder(myServer.byteOrder());
      //if (ds.skipRawData(PalMsg::kPalMsgHeaderSize) < 0) { return false; }

      /* Header */
      logonMsg.appendU32(PalMsg::LogonKind);
      logonMsg.appendI32(PalMsg::kLogonSize);
      logonMsg.appendU32(0);

      logonMsg.appendDw(myUser.regCrc());
      logonMsg.appendDw(myUser.regCounter());
      //logonMsg.appendDw(myCurrentRoom.id());

      /* Username */
      quint8 nUsernameChars = static_cast<quint8>(myUser.username().length());
      logonMsg.appendU8(nUsernameChars);

      int iUsernameChars = 0;
      while (iUsernameChars < nUsernameChars) {
         logonMsg.append(myUser.username().at(iUsernameChars));
         iUsernameChars++;
      }

      while (iUsernameChars < kPalClientMaxUsernameLen) {
         logonMsg.append('\0');
         iUsernameChars++;
      }
      //logonMsg.append('\0');

      /* Wizard password */
      quint8 nWizpassChars = 0;
      logonMsg.appendU8(nWizpassChars);

      int iWizpassChars = 0;
      while (iWizpassChars < kPalClientMaxWizpassLen) {
         //int z = myUser.
         logonMsg.append('\0');
         iWizpassChars++;
      }
      //logonMsg.append('\0');

      logonMsg.appendU32(myUser.idCounter());
      logonMsg.appendU32(myUser.idCrc());
      logonMsg.appendU32(kAuxFlagsAuthenticate | kAuxFlagsWin32);
      logonMsg.appendU32(kPalClientDemoElapsed); /* demoElapsed*: */
      logonMsg.appendU32(kPalClientTotalElapsed); /* totalElapsed*: */
      logonMsg.appendU32(kPalClientDemoLimit); /* demoLimit*: */
      logonMsg.appendU16(myCurrentRoom.id()); /* initialRoom: */

      /* ident */
      for (int i = 0; i < kPalClientIdentLen; i++) {
         logonMsg.append(kPalClientIdent[i]);
      }

      logonMsg.appendU32(0); /* ulReqProtoVer */
      logonMsg.appendU32(kUlCapsAssetsPalace | kUlCapsFilesHttp); /* ulUploadCaps */
      logonMsg.appendU32(kDlCapsAssetsPalace | kDlCapsFilesPalace | kDlCapsFilesHttp | kDlCapsFilesHttpSvr); /* ulDownloadCaps */
      logonMsg.appendU32(0); /* ul2dEngineCaps* */
      logonMsg.appendU32(0); /* ul2dGraphicsCaps* */
      logonMsg.appendU32(0); /* ul3dEngineCaps* */

      mySocket.write(logonMsg);
      res = mySocket.flush();

      return res;
   }

   int PalClient::doSendAuthenticatePalMsg(
         const QString& username, const QString& password)
   {
      // TODO stub
      (void)username;
      (void)password;

      int res = doDetermineIsConnected();
      if (!res)
         return res;

      PalMsg palMsg;

      // TODO stub
      palMsg.appendDw(PalMsg::AuthResponseKind);
      //palMsg.appendDw();

      return res;
   }

   void PalClient::doOnGotBackgroundAsync(QNetworkReply* reply)
   {
      QByteArray ba = reply->readAll();
      myCurrentRoom.backgroundImage().loadFromData(ba);
   }

   void PalClient::doGetBackgroundAsync(const QString& url, QMap<QString, QString> headers)
   {
      //myHttpGetMgr = new QNetworkAccessManager(this);

      // create HTTP request and set hostname
      QNetworkRequest request;
      request.setUrl(url);
      request.setRawHeader("User-Agent", "Seville 1.0");

      // setup error handling
      //connect(&request, SIGNAL(onError(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));

      connect(&myHttpGetMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(doOnGotBackgroundAsync(QNetworkReply*)));

      // add headers
      //headers

      if (!headers.isEmpty()) {
         QMapIterator<QString, QString> iterator(headers);
         while (iterator.hasNext()) {
            iterator.next();
            request.setRawHeader(QByteArray::fromStdString(iterator.key().toStdString()), QByteArray::fromStdString(iterator.value().toStdString()));
         }
      }

      myHttpGetMgr.get(request);
   }
}
