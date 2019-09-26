#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "Log.h"
#include "Palace_Client.h"
#include "Palace_Server.h"
#include "Palace_Room.h"
#include "Palace_User.h"

#include "Palace_NetMsg_Generic.h"
#include "Palace_NetMsg_Logon.h"
#include "Palace_NetMsg_Logon.h"

namespace Seville
{
   namespace Palace
   {
      const QString Client::kIdent = "PC4237";

//      int Client::getKMillisecondPongTimeout()
//      {
//         return kIntervalToTimeoutForPongInMs;
//      }

//      void Client::setKMillisecondPongTimeout(int value)
//      {
//         kIntervalToTimeoutForPongInMs = value;
//      }

      void Client::doSetupEvents()
      {
         // wire up events
         //connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
         //connect(&mySocket, SIGNAL())
         connect(&mySocket, &QIODevice::readyRead,
                 this, &Client::doOnReadyRead);

         connect(
            &mySocket,
            QOverload<QAbstractSocket::SocketError>
                  ::of(&QAbstractSocket::error),
            this, &Client::doOnSocketError
         );
      }

      void Client::doReset()
      {
         myTransferTimerId = 0;
         mySocket.disconnectFromHost();
         doDetermineClientByteOrder();
         myConnectionState = Client::ConnectionState::Disconnected;
         //myLog.resetState();
         myServer.reset();
         myUser.reset();
         myCurrentRoom.reset();
         myNetMsg.reset();
      }

      void Client::doResetReceiveTimer()
      {
         if (myTransferTimerId) {
            killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }
      }

      void Client::doDisconnectFromHost()
      {
         if (myConnectionState != Client::ConnectionState::Disconnected) {
            mySocket.disconnectFromHost();
         }
      }

      void Client::doConnectToHost(
           QString host, int port, QString username, int initialRoom)
      {
         int dHostTcpPortCond = (0 == port);
         u16 dHostTcpPort = static_cast<u16>(
                  (dHostTcpPortCond * kDefaultServerPort) |
                  ((!dHostTcpPortCond) * port)
         );

         u16 dInitialRoom = static_cast<u16>(initialRoom);
         qCDebug(appLcIo, "Connecting to palace://%s@%s:%d/%d",
                 username.toUtf8().data(),
                 host.toUtf8().data(),
                 dHostTcpPort, dInitialRoom);

         myUser.setUsername(username);
         myServer.setHost(host);
         myServer.setPort(dHostTcpPort);
         myCurrentRoom.setId(dInitialRoom);

         qCDebug(appLcIo) << "Client Connection is now in Handshaking State";
         myConnectionState = ConnectionState::Handshaking;
         mySocket.connectToHost(myServer.host(), myServer.port());
      }

      bool Client::doDetermineIsConnected() const
      {
         return mySocket.state() != QTcpSocket::ConnectedState ||
               myConnectionState != ConnectionState::Connected;
      }

      bool Client::doDetermineShouldSwapEndianness() const
      {
         int notUnknownClientByteOrder =
               myByteOrder != Host::ByteOrder::Unknown;

         int notUnknownServerByteOrder =
               myServer.byteOrder() != Host::ByteOrder::Unknown;

         int notSameByteOrder = myByteOrder != myServer.byteOrder();

         return notUnknownClientByteOrder &&
               notUnknownServerByteOrder &&
               notSameByteOrder;
      }

      void Client::doOnReadyRead()
      {
         //NetMsg::Generic& netMsg = myNetMsg;

         if (myTransferTimerId) {
            killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }

         while (myNetMsg.readFrom(&mySocket))
         {
            if (myNetMsg.size() < NetMsg::kByteSizeMinimum ||
                myNetMsg.size() < myNetMsg.sizeExpected())
            {
               myTransferTimerId = startTimer(kIntervalTimeoutForTransferInMs);
               continue;
            }

            if (myConnectionState == ConnectionState::Handshaking)
            {
               if (doDetermineServerByteOrder() < 0) //myNetMsg) < 0)
               {
                  qCDebug(appLcIo) << "ERROR: Could not determine server byte order!";
                  return;
               }

               u32 userId = myNetMsg.objectId();
               myUser.setId(userId);

               if (doSendLogon() < 0)
               {
                  qCDebug(appLcIo) << "ERROR: Could not send Logon!";
                  return;
               }

               myConnectionState = ConnectionState::Connected;
               qCDebug(appLcIo) << "Client is now in Connected State";

               // emit signal?
            }
            else if (myConnectionState == ConnectionState::Connected)
            {
               doRouteReceivedNetMsg();
            }

            //qCDebug(appLcIo) << "Resetting NetMsg";
            myNetMsg.reset();
         }

         myPingTimer.start();
         myPongTime.start();

         // TODO emit signal?
      }

      void Client::doOnSocketError()
      {
         qCDebug(appLcIo) << "ERROR: Socket Error!";
      }

      void Client::doOnPingTimer(QTimerEvent* pingTimeEvent)
      {
         if (pingTimeEvent->timerId() == myTransferTimerId)
         {
            //TODO killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }
      }

      /*
      int Client::doReadDataIntoNetMsg(NetMsg::Generic& netMsg, i32 maxSize)
      {
         if (NetMsg::kByteSizeMaximum < maxSize)
            return 0;

         auto nBytesBeforeRead = netMsg.size();
         if (nBytesBeforeRead == NetMsg::kByteSizeMaximum)
            return 0;

         auto nBytesAvailable = mySocket.bytesAvailable();
         auto netMsgSize = netMsg.size();
         int isHeader = netMsgSize < NetMsg::kByteSizeOfHeader;
         i32 chunkReadSize =
               (isHeader * NetMsg::kByteSizeOfHeader) |
               (!isHeader * kByteSizeOfDefaultChunkRead);

         while (0 < nBytesAvailable && netMsgSize < maxSize) {
            //int remainingSize = (maxSize - netMsgSize);
            QByteArray chunk = mySocket.read(chunkReadSize);
            netMsg.append(chunk);
            //int shouldReadMinChunkSize = remainingSize < kPalClientDefaultChunkReadSize;
            //chunkReadSize = (!shouldReadMinChunkSize * kPalClientDefaultChunkReadSize) | (shouldReadMinChunkSize * kPalClientMinChunkReadSize);
            netMsgSize = netMsg.size();
            nBytesAvailable = mySocket.bytesAvailable();
            chunkReadSize = nBytesAvailable;
         }

         int nBytesRead = netMsgSize - nBytesBeforeRead;
         return nBytesRead;
      }
      */

      void Client::doDetermineClientByteOrder()
      {
         // TODO
         int isLittleEndian = (Host::LittleEndian * !Client::isBigEndian());
         int isBigEndian = (Host::BigEndian * Client::isBigEndian());
         myByteOrder =
               static_cast<Host::ByteOrder>(isLittleEndian | isBigEndian);
      }

      int Client::doDetermineServerByteOrder() //const NetMsg::Generic& netMsg)
      {
         auto res = 0;
         // TODO more consideration here.
         // constants are in big endian byte order,
         // conditionally swapping them if we are a little endian system seems off.
         // but shouldn't we have to swap them all? we don't.

         auto isLE = static_cast<u32>(myByteOrder == Host::LittleEndian);
         auto kind = (isLE * Host::swapU32(myNetMsg.kind())) |
                     (!isLE * myNetMsg.kind());

         if (NetMsg::Kind::UnknownServerKind == kind)
         {
            qCDebug(appLcIo) << "Server has Unknown Byte Order";
            myServer.setByteOrder(Host::ByteOrder::Unknown);
            res = 0;
         }
         else if (NetMsg::Kind::BigEndianServerKind == kind)
         {
            qCDebug(appLcIo) << "Server has Big Endian Byte Order";
            myServer.setByteOrder(Host::ByteOrder::BigEndian);
            res = 1;
         }
         else if (NetMsg::Kind::LittleEndianServerKind == kind)
         {
            qCDebug(appLcIo) << "Server has Little Endian Byte Order";
            myServer.setByteOrder(Host::ByteOrder::LittleEndian);
            res = 1;
         }
         else {
            qCDebug(appLcIo) << "Server did not indicate its Byte Order";
            myServer.setByteOrder(Host::ByteOrder::Unknown);
            res = 0;
         }

         return res;
      }

      int Client::doRouteReceivedNetMsg()
      {
         auto res = 0;
         auto kind = myNetMsg.kind();
         auto size = myNetMsg.size();

         if (size < NetMsg::kByteSizeMinimum)
         {
            qCDebug(appLcIo) << "Received NetMsg that is too small to interpret";
            qCDebug(appLcIo) << "kind: " << QString("0x%1").arg(myNetMsg.kind(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "size expected: " << QString("0x%1").arg(myNetMsg.sizeExpected(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "reference code: " << QString("0x%1").arg(myNetMsg.objectId(), 2, 16, QChar('0'));
            return -1;
         }
         else if (NetMsg::kByteSizeMaximum < size)
         {
            qCDebug(appLcIo) << "Received NetMsg that is too large to interpret";
            qCDebug(appLcIo) << "kind: " << QString("0x%1").arg(myNetMsg.kind(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "size expected: " << QString("0x%1").arg(myNetMsg.sizeExpected(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "reference code: " << QString("0x%1").arg(myNetMsg.objectId(), 2, 16, QChar('0'));
            return -2;
         }

         //u32 znetMsgId = doCsbolecDw_(netMsgId);
         //switch (id) {
            //case Net::Msg::LogonKind:
            //    res = sendLogon_();
            //    break;
         if (NetMsg::Kind::AltLogonKind == kind &&
             //NetMsg::sizes[NetMsg::AltLogonKind] == size) {
             NetMsg::kByteSizeOfLogon == size)
         {
            qCDebug(appLcIo) << "Received AltLogon";
            res = doReceiveAltLogon();
         }
         else if (NetMsg::Kind::ConnectionErrorKind == kind &&
                  NetMsg::kByteSizeOfConnectionError == size)
         {
            qCDebug(appLcIo) << "Received ConnectionError";
            //auto connectionError = dynamic_cast<const NetMsg::ConnectionError&>(myNetMsg);

            res = doReceiveConnectionError(myNetMsg);
         }
         else if (NetMsg::Kind::ServerVersionKind == kind &&
                  NetMsg::kByteSizeOfServerVersion == size)
         {
            qCDebug(appLcIo) << "Received ServerVersion";
            res = doReceiveServerVersion(myNetMsg);
         }
         else if (NetMsg::Kind::ServerInfoKind == kind &&
                  NetMsg::kByteSizeOfServerInfo == size)
         {
            qCDebug(appLcIo) << "Received ServerInfo";
            res = doReceiveServerInfo(myNetMsg);
         }
         else if (NetMsg::Kind::UserStatusKind == kind &&
                  NetMsg::kByteSizeOfUserStatus == size)
         {
            qCDebug(appLcIo) << "Received UserStatus";
            res = doReceiveUserStatus(myNetMsg);
         }
         else if (NetMsg::Kind::UserLoggedOnAndMaxKind == kind &&
                  NetMsg::kByteSizeOfUserLoggedOnAndMax == size)
         {
            qCDebug(appLcIo) << "Received UserLoggedOnAndMax";
            res = doReceiveUserLoggedOnAndMax(myNetMsg);
         }
         else if (NetMsg::Kind::HttpServerLocationKind == kind &&
                  NetMsg::kByteSizeOfHttpServerLocation == size)
         {
            qCDebug(appLcIo) << "Received HttpServerLocation";
            res = doReceiveHttpServerLocation(myNetMsg);
         }
         else if (NetMsg::Kind::RoomUserListKind == kind &&
                  NetMsg::kByteSizeOfRoomUserList == size)
         {
            qCDebug(appLcIo) << "Received RoomUserList";
            res = doReceiveRoomUserList(myNetMsg);
         }
         else if (NetMsg::Kind::ServerUserListKind == kind &&
                  NetMsg::kByteSizeOfServerUserList == size)
         {
            qCDebug(appLcIo) << "Received ServerUserList";
            res = doReceiveServerUserList(myNetMsg);
         }
         else if (NetMsg::Kind::ServerRoomListKind == kind &&
                  NetMsg::kByteSizeOfServerRoomList == size)
         {
            qCDebug(appLcIo) << "Received ServerRoomList";
            res = doReceiveServerRoomList(myNetMsg);
         }
         else if (NetMsg::Kind::RoomDescendKind == kind &&
                  NetMsg::kByteSizeOfRoomDescended == size)
         {
            qCDebug(appLcIo) << "Received RoomDescend";
            res = doReceiveRoomDescend(myNetMsg);
         }
         else if (NetMsg::Kind::UserNewKind == kind &&
                  NetMsg::kByteSizeOfUserNew == size)
         {
            qCDebug(appLcIo) << "Received UserNew";
            res = doReceiveUserNew(myNetMsg);
         }
         else if (NetMsg::Kind::PingKind == kind &&
                  NetMsg::kByteSizeOfPing == size &&
                  size <= NetMsg::kByteSizeMaximum)
         {
            qCDebug(appLcIo) << "Received Ping";
            res = doReceivePing(myNetMsg);
         }
         else if (NetMsg::Kind::PongKind == kind &&
                  NetMsg::kByteSizeOfPong == size)
         {
            qCDebug(appLcIo) << "Received Pong";
            res = doReceivePong(myNetMsg);
         }
         else if (NetMsg::Kind::XTalkKind == kind &&
                  NetMsg::kByteSizeOfXTalk <= size)
         {
            qCDebug(appLcIo) << "Received XTalk";
            res = doReceiveXTalk(myNetMsg);
         }
         else if (NetMsg::Kind::XWhisperKind == kind &&
                  NetMsg::kByteSizeOfXWhisper <= size)
         {
            qCDebug(appLcIo) << "Received XWhisper";
            res = doReceiveXWhisper(myNetMsg);
         }
         else if (NetMsg::Kind::TalkKind == kind &&
                  NetMsg::kByteSizeOfTalk <= size)
         {
            qCDebug(appLcIo) << "Received Talk";
            res = doReceiveTalk(myNetMsg);
         }
         else if (NetMsg::Kind::WhisperKind == kind &&
                  NetMsg::kByteSizeOfWhisper <= size)
         {
            qCDebug(appLcIo) << "Recieved Whisper";
            res = doReceiveWhisper(myNetMsg);
         }
         else if (NetMsg::Kind::AssetIncomingKind == kind &&
                  NetMsg::kByteSizeOfAssetIncoming == size)
         {
            qCDebug(appLcIo) << "Received AssetIncoming";
            res = doReceiveAssetIncoming(myNetMsg);
         }
         else if (NetMsg::Kind::AssetQueryKind == kind &&
                  NetMsg::kByteSizeOfAssetQuery == size)
         {
            qCDebug(appLcIo) << "Received AssetQuery";
            res = doReceiveAssetQuery(myNetMsg);
         }
         else if (NetMsg::Kind::MovementKind == kind &&
                  NetMsg::kByteSizeOfMovement == size)
         {
            qCDebug(appLcIo) << "Received Movement";
            res = doReceiveMovement(myNetMsg);
         }
         else if (NetMsg::Kind::UserColorKind == kind &&
                  NetMsg::kByteSizeOfUserColor == size)
         {
            qCDebug(appLcIo) << "Received UserColor";
            res = doReceiveUserColor(myNetMsg);
         }
         else if (NetMsg::Kind::UserFaceKind == kind &&
                  NetMsg::kByteSizeOfUserFace == size)
         {
            qCDebug(appLcIo) << "Received UserFace";
            res = doReceiveUserFace(myNetMsg);
         }
         else if (NetMsg::Kind::UserPropKind == kind &&
                  NetMsg::kByteSizeOfUserProp == size)
         {
            qCDebug(appLcIo) << "Received UserProp";
            res = doReceiveUserProp(myNetMsg);
         }
         else if (NetMsg::Kind::UserDescriptionKind == kind &&
                  NetMsg::kByteSizeOfUserDescription == size)
         {
            qCDebug(appLcIo) << "Received UserDescription";
            res = doReceiveUserDescription(myNetMsg);
         }
         else if (NetMsg::Kind::UserRenameKind == kind &&
                  NetMsg::kByteSizeOfUserRename == size)
         {
            qCDebug(appLcIo) << "Received UserRename";
            res = doReceiveUserRename(myNetMsg);
         }
         else if (NetMsg::Kind::UserLeavingKind == kind &&
                  NetMsg::kByteSizeOfUserLeaving == size)
         {
            qCDebug(appLcIo) << "Received UserLeaving";
            res = doReceiveUserLeaving(myNetMsg);
         }
         else if (NetMsg::Kind::UserExitRoomKind == kind &&
                  NetMsg::kByteSizeOfUserExitRoom == size)
         {
            qCDebug(appLcIo) << "Received UserExitRoom";
            res = doReceiveUserExitRoom(myNetMsg);
         }
         else if (NetMsg::Kind::PropMoveKind == kind &&
                  NetMsg::kByteSizeOfPropMove == size)
         {
            qCDebug(appLcIo) << "Received PropMove";
            res = doReceivePropMove(myNetMsg);
         }
         else if (NetMsg::Kind::PropDeleteKind == kind &&
                  NetMsg::kByteSizeOfPropDelete == size)
         {
            qCDebug(appLcIo) << "Received PropDelete";
            res = doReceivePropDelete(myNetMsg);
         }
         else if (NetMsg::Kind::PropNewKind == kind &&
                  NetMsg::kByteSizeOfPropNew == size)
         {
            qCDebug(appLcIo) << "Received PropNew";
            res = doReceivePropNew(myNetMsg);
         }
         else if (NetMsg::Kind::DoorLockKind == kind &&
                  NetMsg::kByteSizeOfDoorLock == size)
         {
            qCDebug(appLcIo) << "Received DoorLock";
            res = doReceiveDoorLock(myNetMsg);
         }
         else if (NetMsg::Kind::DoorUnlockKind == kind &&
                  NetMsg::kByteSizeOfDoorUnlock == size)
         {
            qCDebug(appLcIo) << "Received DoorUnlock";
            res = doReceiveDoorUnlock(myNetMsg);
         }
         else if (NetMsg::Kind::PictMoveKind == kind &&
                  NetMsg::kByteSizeOfPictMove == size)
         {
            qCDebug(appLcIo) << "Received PictMove";
            res = doReceivePictMove(myNetMsg);
         }
         else if (NetMsg::Kind::SpotStateKind == kind &&
                  NetMsg::kByteSizeOfSpotState == size)
         {
            qCDebug(appLcIo) << "Received SpotState";
            res = doReceiveSpotState(myNetMsg);
         }
         else if (NetMsg::Kind::SpotMoveKind == kind &&
                  NetMsg::kByteSizeOfSpotMove == size)
         {
            qCDebug(appLcIo) << "Received SpotMove";
            res = doReceiveSpotMove(myNetMsg);
         }
         else if (NetMsg::Kind::DrawKind == kind &&
                  NetMsg::kByteSizeOfDraw == size)
         {
            qCDebug(appLcIo) << "Received Draw";
            res = doReceiveDraw(myNetMsg);
         }
         else if (NetMsg::Kind::NavErrorKind == kind &&
                  NetMsg::kByteSizeOfNavError == size)
         {
            qCDebug(appLcIo) << "Received NavError";
            res = doReceiveNavError(myNetMsg);
         }
         else if (NetMsg::Kind::BlowThruKind == kind &&
                  NetMsg::kByteSizeOfBlowThru == size)
         {
            qCDebug(appLcIo) << "Received BlowThru";
            res = doReceiveBlowThru(myNetMsg);
         }
         else if (NetMsg::Kind::AuthenticateKind == kind &&
                  NetMsg::kByteSizeOfAuthenticate == size)
         {
            qCDebug(appLcIo) << "Received Authenticate";
            res = doReceiveAuthenticate(myNetMsg);
         }
         else if (NetMsg::Kind::AltRoomDescriptionKind == kind &&
                  NetMsg::kByteSizeOfAltRoomDescription == size)
         {
            qCDebug(appLcIo) << "Received AltRoomDescription";
            res = doReceiveRoomDescription(myNetMsg);
         }
         else if (NetMsg::Kind::RoomDescriptionKind == kind &&
                  NetMsg::kByteSizeOfRoomDescription == size)
         {
            qCDebug(appLcIo) << "Received RoomDescription";
            res = doReceiveRoomDescription(myNetMsg);
         }
         else {
            qCDebug(appLcIo) << "Received unknown NetMsg";
            qCDebug(appLcIo) << "kind:  " << QString("0x%1").arg(myNetMsg.kind(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "expected size: " << QString("0x%1").arg(myNetMsg.sizeExpected(), 2, 16, QChar('0'));
            qCDebug(appLcIo) << "reference code: " << QString("0x%1").arg(myNetMsg.objectId(), 2, 16, QChar('0'));
            res = 0;
         }

         return res;
      }

      int Client::doReceiveAltLogon()
      {
         //auto altLogon = dynamic_cast<const NetMsg::AltLogon&>(myNetMsg);
         auto logon = static_cast<const NetMsg::Logon&>(myNetMsg);
         auto res = 0;
         //auto msgAltLogon = dynamic_cast<const NetMsg::AltLogon&>(netMsg);

         //QDataStream ds(netMsgRx_);
         /* Skip header */
         //ds.skipRawData(Net::Msg::kNet::MsgHeaderSize);
         //u32 crc; ds >> crc;
         //u32 counter; ds >> counter;
         //u8 usernameLen; ds >> usernameLen;
//         int offset = NetMsg::kByteSizeOfHeader;
//         u32 crc = netMsg.u32At(offset);
//         offset += sizeof(crc);
//         u32 counter = netMsg.u32At(offset);
//         offset += sizeof(counter);

//         /* Username */
//         u8 nUsernameChars = netMsg.u8At(offset);
//         offset += sizeof(nUsernameChars);

//         u8 iUsernameChars = 0;
//         while (iUsernameChars < nUsernameChars) {
//            u8 ch = netMsg.u8At(offset);
//            offset += sizeof(ch);
//            myUser.username().append(ch);
//            iUsernameChars++;
//         }

//         while (iUsernameChars < kByteSizeOfLongestUsername) {
//            // TODO wizpass
//            offset += sizeof(u8);
//            iUsernameChars++;
//         }

//         u8 nWizpassChars = netMsg.u8At(offset);
//         offset += sizeof(nWizpassChars);

//         u8 iWizpassChars = 0;
//         while (iWizpassChars < kByteSizeOfLongestWizpass) {
//            offset += sizeof(u8);
//            iWizpassChars++;
//         }

//         u32 auxFlags = netMsg.u32At(offset);
//         offset += sizeof(auxFlags);
//         u32 puidCounter = netMsg.u32At(offset);
//         offset += sizeof(puidCounter);
//         u32 puidCrc = netMsg.u32At(offset);
//         offset += sizeof(puidCrc);
//         u32 demoElapsed = netMsg.u32At(offset);
//         offset += sizeof(demoElapsed);
//         u32 demoLimit = netMsg.u32At(offset);
//         offset += sizeof(demoLimit);
//         u32 desiredRoom = netMsg.u32At(offset);
//         offset += sizeof(desiredRoom);

//         QString reserved;
//         for (int i = 0; i < kByteSizeOfIdent; i++) {
//            u8 ch = netMsg.u8At(offset);
//            offset += sizeof(u8);
//            reserved.append(ch);
//         }

//         u32 ulReqProtoVer = netMsg.u32At(offset);
//         offset += sizeof(ulReqProtoVer);
//         u32 ulUploadCaps = netMsg.u32At(offset);
//         offset += sizeof(ulUploadCaps);
//         u32 ulDownloadCaps = netMsg.u32At(offset);
//         offset += sizeof(ulDownloadCaps);
//         u32 ul2dEngineCaps = netMsg.u32At(offset);
//         offset += sizeof(ul2dEngineCaps);
//         u32 ul2dGraphicsCaps = netMsg.u32At(offset);
//         offset += sizeof(ul2dGraphicsCaps);
//         u32 ul3dEngineCaps = netMsg.u32At(offset);
//         offset += sizeof(ul3dEngineCaps);

         if (myUser.idCounter() != logon.puidCounter() ||
             myUser.idCrc() != logon.puidCrc())
         {
            myUser.setIdCrc(logon.puidCrc());
            myUser.setIdCounter(logon.puidCounter());
            myUser.setIdChanged(true);
         }

         res = 1;
         return res;
      }

      int Client::doReceiveAuthenticate(const NetMsg::Generic& netMsg)
      {
         (void)netMsg;
         int res = 0;
         // raise event?
         //return doSendAuthenticateNet::Msg();

         return res;
      }

      int Client::doReceiveConnectionError(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveServerVersion(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveServerInfo(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserStatus(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserLoggedOnAndMax(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveHttpServerLocation(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveRoomUserList(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveServerUserList(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveServerRoomList(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveRoomDescend(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserNew(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePing(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePong(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveXTalk(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveXWhisper(const NetMsg::Generic& netMsg)
      {
         // TODO stub
        (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveTalk(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveWhisper(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveAssetIncoming(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveAssetQuery(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveMovement(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserColor(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserFace(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserProp(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserDescription(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserRename(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserLeaving(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveUserExitRoom(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePropMove(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePropDelete(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePropNew(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveDoorLock(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveDoorUnlock(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceivePictMove(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveSpotState(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveSpotMove(const NetMsg::Generic& netMsg)
      {
         //TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveDraw(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveNavError(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveBlowThru(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doReceiveRoomDescription(const NetMsg::Generic& netMsg)
      {
         // TODO stub
         (void)netMsg;
         int res = 0;
         return res;
      }

      int Client::doSendLogon()
      {
         auto res = 0;
         NetMsg::Logon logon(doDetermineShouldSwapEndianness());
         logon.setRegCrc(myUser.regCrc());
         logon.setRegCounter(myUser.regCounter());
         logon.setUsername(myUser.username());
         logon.setWizpass(myUser.wizpass());
         logon.setFlags();
         logon.setPuidCounter(myPuidCounter);
         logon.setPuidCrc(myPuidCrc);
         logon.setDemoElapsed();
         logon.setTotalElapsed();
         logon.setDemoLimit();
         logon.setInitialRoomId(myCurrentRoom.id());
         logon.setReserved();
         logon.setUploadRequestedProtocolVersion();
         logon.setUploadCapabilities();
         logon.setDownloadCapabilities();
         logon.setEngineCapabilities2d();
         logon.setGraphicsCapabilities2d();
         logon.setGraphicsCapabilities3d();

         //netMsgRxDs_->setByteOrder(myServer.byteOrder());
         //if (ds.skipRawData(Net::Msg::kNet::MsgHeaderSize) < 0) { return false; }

         /* Header */
//         msg.appendU32(NetMsg::Kind::LogonKind);
//         msg.appendI32(NetMsg::kByteSizeOfLogon);
//         msg.appendU32(0);

//         msg.appendU32(myUser.regCrc());
//         msg.appendU32(myUser.regCounter());
         //logonMsg.appendDw(myCurrentRoom.id());

         /* Username */
//         u8 nUsernameChars = static_cast<u8>(myUser.username().length());
//         msg.appendU8(nUsernameChars);

//         int iUsernameChars = 0;
//         while (iUsernameChars < nUsernameChars) {
//            msg.append(myUser.username().at(iUsernameChars));
//            iUsernameChars++;
//         }

//         while (iUsernameChars < kByteSizeOfLongestUsername) {
//            msg.append('\0');
//            iUsernameChars++;
//         }
//         //logonMsg.append('\0');

//         /* Wizard password */
//         u8 nWizpassChars = 0;
//         msg.appendU8(nWizpassChars);

//         int iWizpassChars = 0;
//         while (iWizpassChars < kByteSizeOfLongestWizpass) {
//            //int z = myUser.
//            msg.append('\0');
//            iWizpassChars++;
//         }
//         //logonMsg.append('\0');

//         msg.appendU32(myUser.idCounter());
//         msg.appendU32(myUser.idCrc());
//         msg.appendU32(kAuxFlagsAuthenticate | kAuxFlagsWin32);
//         msg.appendU32(kDemoElapsed); /* demoElapsed*: */
//         msg.appendU32(kTotalElapsed); /* totalElapsed*: */
//         msg.appendU32(kDemoLimit); /* demoLimit*: */
//         msg.appendU16(myCurrentRoom.id()); /* initialRoom: */

//         /* ident */
//         for (int i = 0; i < kByteSizeOfIdent; i++) {
//            msg.append(kIdent[i]);
//         }

//         msg.appendU32(0); /* ulReqProtoVer */
//         msg.appendU32(kUlCapsAssetsPalace | kUlCapsFilesHttp); /* ulUploadCaps */
//         msg.appendU32(kDlCapsAssetsPalace | kDlCapsFilesPalace | kDlCapsFilesHttp | kDlCapsFilesHttpSvr); /* ulDownloadCaps */
//         msg.appendU32(0); /* ul2dEngineCaps* */
//         msg.appendU32(0); /* ul2dGraphicsCaps* */
//         msg.appendU32(0); /* ul3dEngineCaps* */

         mySocket.write(logon);
         res = mySocket.flush();

         return res;
      }

      int Client::doSendAuthenticate(
            const QString& username,
            const QString& password)
      {
         // TODO stub
         (void)username;
         (void)password;

         int res = doDetermineIsConnected();
         if (!res)
            return res;

         NetMsg::Generic netMsg;

         // TODO stub
         netMsg.appendU32(NetMsg::Kind::AuthResponseKind);
         //netMsg.appendDw();

         return res;
      }

      void Client::onGotBackgroundAsync(QNetworkReply* reply)
      {
         QByteArray ba = reply->readAll();
         myCurrentRoom.backgroundImage().loadFromData(ba);
      }

      void Client::doGetBackgroundAsync(const QString& url, QMap<QString, QString> headers)
      {
         //myHttpGetMgr = new QNetworkAccessManager(this);

         // create HTTP request and set hostname
         QNetworkRequest request;
         request.setUrl(url);
         request.setRawHeader("User-Agent", "Seville 1.0");

         // setup error handling
         //connect(&request, SIGNAL(onError(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));

         connect(&myHttpGetMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGotBackgroundAsync(QNetworkReply*)));

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

      Client::~Client()
      {
         doDisconnectFromHost();
      }

      Client::Client(QObject* parent) : QObject(parent)
      {
         doSetupEvents();

         doReset();
      }
   }
}
