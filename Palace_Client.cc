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
#include "Palace_NetMsg_AltLogon.h"
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

      void Client::doOnReadyRead()
      {
         NetMsg::Generic& netMsg = myNetMsg;

         if (myTransferTimerId) {
            killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }

         // while (netMsg->read())
         while (doReadNetMsg(netMsg))
         {
            if (myConnectionState == ConnectionState::Handshaking) {
               if (doDetermineServerByteOrder(netMsg) < 0) {
                  qCDebug(appLcIo) << "ERROR: Could not determine server byte order!";
                  return;
               }

               u32 userId = netMsg.ref();
               myUser.setId(userId);

               if (doSendLogon() < 0) {
                  qCDebug(appLcIo) << "ERROR: Could not send Logon!";
                  return;
               }

               myConnectionState = ConnectionState::Connected;
               qCDebug(appLcIo) << "Client is now in Connected State";

               // emit signal?
            }
            else if (myConnectionState == ConnectionState::Connected) {
               doRouteNetMsg(netMsg);
            }

            //qCDebug(appLcIo) << "Resetting Net::Msg";
            netMsg.reset();
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
         if (pingTimeEvent->timerId() == myTransferTimerId) {
            //TODO killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }
      }

      int Client::doReadDataIntoNetMsg(NetMsg::Generic& netMsg, i32 maxSize)
      {
         if (NetMsg::kByteSizeOfLongestNetMsg < maxSize) {
            return 0;
         }

         auto nBytesBeforeRead = netMsg.size();
         if (nBytesBeforeRead == NetMsg::kByteSizeOfLongestNetMsg) {
            return 0;
         }

         auto nBytesAvailable = mySocket.bytesAvailable();
         auto netMsgSize = netMsg.size();
         int isHeader = netMsgSize < NetMsg::kByteSizeOfHeader;
         i64 chunkReadSize =
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

      int Client::doReadNetMsgHeader(NetMsg::Generic& incomingMsg)
      {
         if (myTransferTimerId) {
            killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }

         auto initialNetMsgSize = incomingMsg.size();
         auto nExpectedBytesToRead =
               NetMsg::kByteSizeOfHeader - initialNetMsgSize;
         if (0 == nExpectedBytesToRead)
            return 1;
         else if (nExpectedBytesToRead < 0)
            return 1;

         auto nBytesAvailable = mySocket.bytesAvailable();
         auto shouldReadPartial = nBytesAvailable < nExpectedBytesToRead;
         auto nBytesToRead = (nBytesAvailable * shouldReadPartial) |
               (nExpectedBytesToRead * !shouldReadPartial);
         incomingMsg.append(mySocket.read(nBytesToRead));

         auto finalNetMsgSize = incomingMsg.size();
         auto nBytesRead = finalNetMsgSize - initialNetMsgSize;
         if (nBytesRead < nExpectedBytesToRead) {
            myTransferTimerId = startTimer(kIntervalToTimeoutForTransferInMs);
         }

         return finalNetMsgSize == NetMsg::kByteSizeOfHeader;
      }

      int Client::doReadNetMsgBody(NetMsg::Generic& netMsg)
      {
         if (myTransferTimerId) {
            killTimer(myTransferTimerId);
            myTransferTimerId = 0;
         }

         auto initialNetMsgSize = netMsg.size();
         if (initialNetMsgSize < NetMsg::kByteSizeOfHeader)
            return 0;

         auto nExpectedBytesToRead = netMsg.len() -
               (initialNetMsgSize - NetMsg::kByteSizeOfHeader);
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
         netMsg.append(mySocket.read(nBytesToRead));

         auto finalNetMsgSize = netMsg.size();
         auto nBytesRead = finalNetMsgSize - initialNetMsgSize;
         if (nBytesRead < nExpectedBytesToRead) {
            myTransferTimerId = startTimer(kIntervalToTimeoutForTransferInMs);
         }

         auto nExpectedTotalBytes =
               NetMsg::kByteSizeOfHeader + netMsg.len();
         return finalNetMsgSize == nExpectedTotalBytes;
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

      int Client::doReadNetMsg(NetMsg::Generic& netMsg)
      {
         doResetReceiveTimer();
         // read in header
         if (netMsgBytesRead < NetMsg::kByteSizeOfHeader) {
            netMsg.append(mySocket.read);
            netMsgBytesExpected = len;
            // set timer?
         }

         int readBodyOk = doReadNetMsgBody(netMsg);
         int minNetMsgOk = NetMsg::kByteSizeOfHeader <= netMsg.size();
         return readHeaderOk && readBodyOk && minNetMsgOk;
      }

      void Client::doDetermineClientByteOrder()
      {
         // TODO
         int isLittleEndian = (Host::LittleEndian * !Client::isBigEndian());
         int isBigEndian = (Host::BigEndian * Client::isBigEndian());
         myByteOrder =
               static_cast<Host::ByteOrder>(isLittleEndian | isBigEndian);
      }

      int Client::doDetermineServerByteOrder(const NetMsg::Generic& netMsg)
      {
         auto res = 0;
         // TODO more consideration here.
         // constants are in big endian byte order,
         // conditionally swapping them if we are a little endian system seems off.
         // but shouldn't we have to swap them all? we don't.

         auto isLE = myByteOrder == Host::LittleEndian;
         auto netMsgId =
               (static_cast<u32>(isLE) *
                Host::swapUInt32(netMsg.id())) |
               (static_cast<u32>(!isLE) * netMsg.id());

         switch (netMsgId) {
            case NetMsg::Kind::UnknownServerKind: {
               qCDebug(appLcIo) << "Server has Unknown Byte Order";
               myServer.setByteOrder(Host::ByteOrder::Unknown);
               res = 0;
               break;
            }
            case NetMsg::Kind::BigEndianServerKind: {
               qCDebug(appLcIo) << "Server has Big Endian Byte Order";
               myServer.setByteOrder(Host::ByteOrder::BigEndian);
               res = 1;
               break;
            }
            case NetMsg::Kind::LittleEndianServerKind: {
               qCDebug(appLcIo) << "Server has Little Endian Byte Order";
               myServer.setByteOrder(Host::ByteOrder::LittleEndian);
               res = 1;
               break;
            }
            default: {
               qCDebug(appLcIo) << "Server did not indicate its Byte Order";
               myServer.setByteOrder(Host::ByteOrder::Unknown);
               res = 0;
               break;
            }
         }

         return res;
      }

      int Client::doRouteNetMsg(const NetMsg::Generic& netMsg)
      {
         auto res = 0;
         auto netMsgId = netMsg.id();
         //u32 znetMsgId = doCsbolecDw_(netMsgId);
         switch (netMsgId) {
            //case Net::Msg::LogonKind:
            //    res = sendLogon_();
            //    break;
            case NetMsg::Kind::AltLogonKind: {
               qCDebug(appLcIo) << "Received AltLogon Net::Msg";
               auto netMsgAltLogon = dynamic_cast<const NetMsg::AltLogon&>(netMsg);
               res = doReceiveAltLogon(netMsgAltLogon);
               break;
            }
            case NetMsg::Kind::ConnectionErrorKind: {
               qCDebug(appLcIo) << "Received ConnectionError Net::Msg";
               res = doReceiveConnectionError(netMsg);
               break;
            }
            case NetMsg::Kind::ServerVersionKind: {
               qCDebug(appLcIo) << "Received ServerVersion Net::Msg";
               res = doReceiveServerVersion(netMsg);
               break;
            }
            case NetMsg::Kind::ServerInfoKind: {
               qCDebug(appLcIo) << "Received ServerInfo Net::Msg";
               res = doReceiveServerInfo(netMsg);
               break;
            }
            case NetMsg::Kind::UserStatusKind: {
               qCDebug(appLcIo) << "Received UserStatus Net::Msg";
               res = doReceiveUserStatus(netMsg);
               break;
            }
            case NetMsg::Kind::UserLoggedOnAndMaxKind: {
               qCDebug(appLcIo) << "Received UserLoggedOnAndMax Net::Msg";
               res = doReceiveUserLoggedOnAndMax(netMsg);
               break;
            }
            case NetMsg::Kind::HttpServerLocationKind: {
               qCDebug(appLcIo) << "Received HttpServerLocation Net::Msg";
               res = doReceiveHttpServerLocation(netMsg);
               break;
            }
            case NetMsg::Kind::RoomUserListKind: {
               qCDebug(appLcIo) << "Received RoomUserList Net::Msg";
               res = doReceiveRoomUserList(netMsg);
               break;
            }
            case NetMsg::Kind::ServerUserListKind: {
               qCDebug(appLcIo) << "Received ServerUserList Net::Msg";
               res = doReceiveServerUserList(netMsg);
               break;
            }
            case NetMsg::Kind::ServerRoomListKind: {
               qCDebug(appLcIo) << "Received ServerRoomList Net::Msg";
               res = doReceiveServerRoomList(netMsg);
               break;
            }
            case NetMsg::Kind::RoomDescendKind: {
               qCDebug(appLcIo) << "Received RoomDescend Net::Msg";
               res = doReceiveRoomDescend(netMsg);
               break;
            }
            case NetMsg::Kind::UserNewKind: {
               qCDebug(appLcIo) << "Received UserNew Net::Msg";
               res = doReceiveUserNew(netMsg);
               break;
            }
            case NetMsg::Kind::PingKind: {
               qCDebug(appLcIo) << "Received Ping Net::Msg";
               res = doReceivePing(netMsg);
               break;
            }
            case NetMsg::Kind::PongKind: {
               qCDebug(appLcIo) << "Received Pong Net::Msg";
               res = doReceivePong(netMsg);
               break;
            }
            case NetMsg::Kind::XTalkKind: {
               qCDebug(appLcIo) << "Received XTalk Net::Msg";
               res = doReceiveXTalk(netMsg);
               break;
            }
            case NetMsg::Kind::XWhisperKind: {
               qCDebug(appLcIo) << "Received XWhisper Net::Msg";
               res = doReceiveXWhisper(netMsg);
               break;
            }
            case NetMsg::Kind::TalkKind: {
               qCDebug(appLcIo) << "Received Talk Net::Msg";
               res = doReceiveTalk(netMsg);
               break;
            }
            case NetMsg::Kind::WhisperKind: {
               qCDebug(appLcIo) << "Recieved Whisper Net::Msg";
               res = doReceiveWhisper(netMsg);
               break;
            }
            case NetMsg::Kind::AssetIncomingKind: {
               qCDebug(appLcIo) << "Received AssetIncoming Net::Msg";
               res = doReceiveAssetIncoming(netMsg);
               break;
            }
            case NetMsg::Kind::AssetQueryKind: {
               qCDebug(appLcIo) << "Received AssetQuery Net::Msg";
               res = doReceiveAssetQuery(netMsg);
               break;
            }
            case NetMsg::Kind::MovementKind: {
               qCDebug(appLcIo) << "Received Movement Net::Msg";
               res = doReceiveMovement(netMsg);
               break;
            }
            case NetMsg::Kind::UserColorKind: {
               qCDebug(appLcIo) << "Received UserColor Net::Msg";
               res = doReceiveUserColor(netMsg);
               break;
            }
            case NetMsg::Kind::UserFaceKind: {
               qCDebug(appLcIo) << "Received UserFace Net::Msg";
               res = doReceiveUserFace(netMsg);
               break;
            }
            case NetMsg::Kind::UserPropKind: {
               qCDebug(appLcIo) << "Received UserProp Net::Msg";
               res = doReceiveUserProp(netMsg);
               break;
            }
            case NetMsg::Kind::UserDescriptionKind: {
               qCDebug(appLcIo) << "Received UserDescription Net::Msg";
               res = doReceiveUserDescription(netMsg);
               break;
            }
            case NetMsg::Kind::UserRenameKind: {
               qCDebug(appLcIo) << "Received UserRename Net::Msg";
               res = doReceiveUserRename(netMsg);
               break;
            }
            case NetMsg::Kind::UserLeavingKind: {
               qCDebug(appLcIo) << "Received UserLeaving Net::Msg";
               res = doReceiveUserLeaving(netMsg);
               break;
            }
            case NetMsg::Kind::UserExitRoomKind: {
               qCDebug(appLcIo) << "Received UserExitRoom Net::Msg";
               res = doReceiveUserExitRoom(netMsg);
               break;
            }
            case NetMsg::Kind::PropMoveKind: {
               qCDebug(appLcIo) << "Received PropMove Net::Msg";
               res = doReceivePropMove(netMsg);
               break;
            }
            case NetMsg::Kind::PropDeleteKind: {
               qCDebug(appLcIo) << "Received PropDelete Net::Msg";
               res = doReceivePropDelete(netMsg);
               break;
            }
            case NetMsg::Kind::PropNewKind: {
               qCDebug(appLcIo) << "Received PropNew Net::Msg";
               res = doReceivePropNew(netMsg);
               break;
            }
            case NetMsg::Kind::DoorLockKind: {
               qCDebug(appLcIo) << "Received DoorLock Net::Msg";
               res = doReceiveDoorLock(netMsg);
               break;
            }
            case NetMsg::Kind::DoorUnlockKind: {
               qCDebug(appLcIo) << "Received DoorUnlock Net::Msg";
               res = doReceiveDoorUnlock(netMsg);
               break;
            }
            case NetMsg::Kind::PictMoveKind: {
               qCDebug(appLcIo) << "Received PictMove Net::Msg";
               res = doReceivePictMove(netMsg);
               break;
            }
            case NetMsg::Kind::SpotStateKind: {
               qCDebug(appLcIo) << "Received SpotState Net::Msg";
               res = doReceiveSpotState(netMsg);
               break;
            }
            case NetMsg::Kind::SpotMoveKind: {
               qCDebug(appLcIo) << "Received SpotMove Net::Msg";
               res = doReceiveSpotMove(netMsg);
               break;
            }
            case NetMsg::Kind::DrawKind: {
               qCDebug(appLcIo) << "Received Draw Net::Msg";
               res = doReceiveDraw(netMsg);
               break;
            }
            case NetMsg::Kind::NavErrorKind: {
               qCDebug(appLcIo) << "Received NavError Net::Msg";
               res = doReceiveNavError(netMsg);
               break;
            }
            case NetMsg::Kind::BlowThruKind: {
               qCDebug(appLcIo) << "Received BlowThru Net::Msg";
               res = doReceiveBlowThru(netMsg);
               break;
            }
            case NetMsg::Kind::AuthenticateKind: {
               qCDebug(appLcIo) << "Received Authenticate Net::Msg";
               res = doReceiveAuthenticate(netMsg);
               break;
            }
            case NetMsg::Kind::AltRoomDescriptionKind: {
               qCDebug(appLcIo) << "Received AltRoomDescription Net::Msg";
               res = doReceiveRoomDescription(netMsg);
               break;
            }
            case NetMsg::Kind::RoomDescriptionKind: {
               qCDebug(appLcIo) << "Received RoomDescription Net::Msg";
               res = doReceiveRoomDescription(netMsg);
               break;
            }
            default: {
               qCDebug(appLcIo) << "Received UNKNOWN Net::Msg";
               qCDebug(appLcIo) << "id:  " << QString("0x%1").arg(netMsg.id(), 2, 16, QChar('0'));
               qCDebug(appLcIo) << "len: " << QString("0x%1").arg(netMsg.len(), 2, 16, QChar('0'));
               qCDebug(appLcIo) << "ref: " << QString("0x%1").arg(netMsg.ref(), 2, 16, QChar('0'));
               res = 0;
               break;
            }
         }

         return res;
      }

      int Client::doReceiveAltLogon(const NetMsg::AltLogon& netMsg)
      {
         auto res = 0;
         //auto msgAltLogon = dynamic_cast<const NetMsg::AltLogon&>(netMsg);

         //QDataStream ds(netMsgRx_);
         /* Skip header */
         //ds.skipRawData(Net::Msg::kNet::MsgHeaderSize);
         //u32 crc; ds >> crc;
         //u32 counter; ds >> counter;
         //u8 usernameLen; ds >> usernameLen;
         int offset = NetMsg::kByteSizeOfHeader;
         u32 crc = netMsg.u32At(offset);
         offset += sizeof(crc);
         u32 counter = netMsg.u32At(offset);
         offset += sizeof(counter);

         /* Username */
         u8 nUsernameChars = netMsg.u8At(offset);
         offset += sizeof(nUsernameChars);

         u8 iUsernameChars = 0;
         while (iUsernameChars < nUsernameChars) {
            u8 ch = netMsg.u8At(offset);
            offset += sizeof(ch);
            myUser.username().append(ch);
            iUsernameChars++;
         }

         while (iUsernameChars < kByteSizeOfLongestUsername) {
            // TODO wizpass
            offset += sizeof(u8);
            iUsernameChars++;
         }

         u8 nWizpassChars = netMsg.u8At(offset);
         offset += sizeof(nWizpassChars);

         u8 iWizpassChars = 0;
         while (iWizpassChars < kByteSizeOfLongestWizpass) {
            offset += sizeof(u8);
            iWizpassChars++;
         }

         u32 auxFlags = netMsg.u32At(offset);
         offset += sizeof(auxFlags);
         u32 puidCounter = netMsg.u32At(offset);
         offset += sizeof(puidCounter);
         u32 puidCrc = netMsg.u32At(offset);
         offset += sizeof(puidCrc);
         u32 demoElapsed = netMsg.u32At(offset);
         offset += sizeof(demoElapsed);
         u32 demoLimit = netMsg.u32At(offset);
         offset += sizeof(demoLimit);
         u32 desiredRoom = netMsg.u32At(offset);
         offset += sizeof(desiredRoom);

         QString reserved;
         for (int i = 0; i < kByteSizeOfIdent; i++) {
            u8 ch = netMsg.u8At(offset);
            offset += sizeof(u8);
            reserved.append(ch);
         }

         u32 ulReqProtoVer = netMsg.u32At(offset);
         offset += sizeof(ulReqProtoVer);
         u32 ulUploadCaps = netMsg.u32At(offset);
         offset += sizeof(ulUploadCaps);
         u32 ulDownloadCaps = netMsg.u32At(offset);
         offset += sizeof(ulDownloadCaps);
         u32 ul2dEngineCaps = netMsg.u32At(offset);
         offset += sizeof(ul2dEngineCaps);
         u32 ul2dGraphicsCaps = netMsg.u32At(offset);
         offset += sizeof(ul2dGraphicsCaps);
         u32 ul3dEngineCaps = netMsg.u32At(offset);
         offset += sizeof(ul3dEngineCaps);

         if (myUser.idCounter() != puidCounter || myUser.idCrc() != puidCrc) {
            myUser.setIdCrc(puidCrc);
            myUser.setIdCounter(puidCounter);
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
         NetMsg::Logon msg(myByteOrder, myServer.byteOrder());
         //netMsgRxDs_->setByteOrder(myServer.byteOrder());
         //if (ds.skipRawData(Net::Msg::kNet::MsgHeaderSize) < 0) { return false; }

         /* Header */
         msg.appendU32(NetMsg::Kind::LogonKind);
         msg.appendI32(NetMsg::kByteSizeOfLogon);
         msg.appendU32(0);

         msg.appendU32(myUser.regCrc());
         msg.appendU32(myUser.regCounter());
         //logonMsg.appendDw(myCurrentRoom.id());

         /* Username */
         u8 nUsernameChars = static_cast<u8>(myUser.username().length());
         msg.appendU8(nUsernameChars);

         int iUsernameChars = 0;
         while (iUsernameChars < nUsernameChars) {
            msg.append(myUser.username().at(iUsernameChars));
            iUsernameChars++;
         }

         while (iUsernameChars < kByteSizeOfLongestUsername) {
            msg.append('\0');
            iUsernameChars++;
         }
         //logonMsg.append('\0');

         /* Wizard password */
         u8 nWizpassChars = 0;
         msg.appendU8(nWizpassChars);

         int iWizpassChars = 0;
         while (iWizpassChars < kByteSizeOfLongestWizpass) {
            //int z = myUser.
            msg.append('\0');
            iWizpassChars++;
         }
         //logonMsg.append('\0');

         msg.appendU32(myUser.idCounter());
         msg.appendU32(myUser.idCrc());
         msg.appendU32(kAuxFlagsAuthenticate | kAuxFlagsWin32);
         msg.appendU32(kDemoElapsed); /* demoElapsed*: */
         msg.appendU32(kTotalElapsed); /* totalElapsed*: */
         msg.appendU32(kDemoLimit); /* demoLimit*: */
         msg.appendU16(myCurrentRoom.id()); /* initialRoom: */

         /* ident */
         for (int i = 0; i < kByteSizeOfIdent; i++) {
            msg.append(kIdent[i]);
         }

         msg.appendU32(0); /* ulReqProtoVer */
         msg.appendU32(kUlCapsAssetsPalace | kUlCapsFilesHttp); /* ulUploadCaps */
         msg.appendU32(kDlCapsAssetsPalace | kDlCapsFilesPalace | kDlCapsFilesHttp | kDlCapsFilesHttpSvr); /* ulDownloadCaps */
         msg.appendU32(0); /* ul2dEngineCaps* */
         msg.appendU32(0); /* ul2dGraphicsCaps* */
         msg.appendU32(0); /* ul3dEngineCaps* */

         mySocket.write(msg);
         res = mySocket.flush();

         return res;
      }

      int Client::doSendAuthenticate(
            const QString& username, const QString& password)
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
