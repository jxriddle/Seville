#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTcpSocket>

#include "seville/base/sevilleapp.h"
#include "seville/base/bytearray.h"
#include "seville/palace/host.h"
#include "seville/palace/client.h"
#include "seville/palace/server.h"
#include "seville/palace/room.h"
#include "seville/palace/user.h"
#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      const char* Client::kIdent = "PC4237";

      Client::Client(QObject* parentObjectPtr)
         : QObject(parentObjectPtr)
      {
         do_init();
      }

      Client::~Client(void)
      {
         do_deinit();
      }

      //auto Client::do_receiveNetMsgFromSocket(void) -> int
      auto Client::do_receiveNetMsgFromSocket(void) -> int //std::tuple<int, NetMsg>
      {
         auto readHeaderOk = 0;
         auto readContentOk = 0;
         if (my_netMsg.size() < NetMsg::kHeaderSize)
            readHeaderOk = do_readNetMsgHeaderFromSocket();
         if (NetMsg::kHeaderSize <= my_netMsg.size())
            readContentOk = do_readNetMsgContentFromSocket(); //my_netMsg.len());
         //return readHeaderOk && readContentOk;
         return readHeaderOk && readContentOk;
         //return my_netMsg;
      }

      auto Client::do_readNetMsgHeaderFromSocket(void) -> int
      { //_from_socket_ptr_(
            //QTcpSocket* socket_ptr) -> int {

         auto nBytesToCompleteHeader = i32(
               NetMsg::kHeaderSize - my_netMsg.size());

         //auto readHeaderOk = 0 <= nExpectedBytesToRead;
         if (nBytesToCompleteHeader == 0)
            return 1;
         else if (nBytesToCompleteHeader < 0)
            return 0;

         auto nBytesAvailable = my_socket.bytesAvailable();
         auto isPartialRead = nBytesAvailable < nBytesToCompleteHeader;
         auto nBytesToRead =
               (isPartialRead * nBytesAvailable) |
               (!isPartialRead * nBytesToCompleteHeader);

         //auto header = my_socket.read(nBytesToRead);
         //QDataStream headerStream(header);
         //my_netMsg.setHeader(header);
         //my_netMsg.setStreamCursorPosition(0);
         //my_netMsg.streamReadU32();
         my_netMsg.append(my_socket.read(nBytesToRead));

         auto netMsgHeaderSize = my_netMsg.size();

         if (NetMsg::kHeaderSize == netMsgHeaderSize)
            my_netMsg.resize(NetMsg::kHeaderSize); // + my_netMsg.len());

         return NetMsg::kHeaderSize == netMsgHeaderSize;
      }

      auto Client::do_readNetMsgContentFromSocket(void) -> int //(i32 len) -> int
      {
            //QTcpSocket* socket_ptr) -> int {
         auto len = i32(my_netMsg.bodyLen());
         auto contentSize_0 = my_netMsg.contentSize();
         //if (size_0 < NetMsg::kHeaderSize)
         //   return 0;

         //auto body_size = size_0 - NetMsg::kHeaderSize;

         auto nBytesToCompleteContent = i32(len - contentSize_0);
         //         my_netMsg.len() - contentSize_0);

         if (0 == nBytesToCompleteContent)
            return 1;
         else if (nBytesToCompleteContent < 0)
            return 0;

         auto nBytesAvailable = my_socket.bytesAvailable();
         auto isPartialRead = nBytesAvailable < nBytesToCompleteContent;
         auto nBytesToRead =
               (isPartialRead * nBytesAvailable) |
               (!isPartialRead * nBytesToCompleteContent);

         // maybe reads should be chunked?
         my_netMsg.append(my_socket.read(nBytesToRead));
         //my_netMsg.appendContent()

         //auto sizeN = this->size();
         //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;

         //auto nExpectedTotalBytesInBody = len;

         return len == my_netMsg.contentSize();
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

      auto Client::do_setupEvents(void) -> void
      {
         // wire up events
         connect(&my_socket, &QTcpSocket::readyRead, //&QIODevice::readyRead,
                 this, &seville::palace::Client::on_readyReadDidOccur);
                 //this, [this](void) {
               //do_readSocket();
            //});

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
         connect(
            &my_socket,QOverload<QAbstractSocket::SocketError>
                  ::of(&QAbstractSocket::error),
            this, &Client::on_socketErrorDidOccur);
#else
         connect(
            &my_socket, QOverload<QAbstractSocket::SocketError>::of(
                     &QAbstractSocket::errorOccurred),
                  this, &Client::on_socketErrorDidOccur);

                  //this, [this](QAbstractSocket::SocketError error) {
                  //&Client::on_socketError
               //QAbstractSocket::SocketError::

//            connect(
//                  &my_socket_,
//                  &QTcpSocket::errorOccurred,
//                  this,
//                     [this](QAbstractSocket::SocketError socket_error) {
//               my_logger_.error("Socket error");
//               qCDebug(log_seville) << "ERROR: Socket Error";
//            });
#endif

         connect(
            &my_networkAccessManager, &QNetworkAccessManager::finished,
            this, [this](QNetworkReply* replyPtr) {
               if (replyPtr->error()) {
                  my_logger.appendErrorMessage(
                           //QString("Background failed to load."));
                           QString("Background load error: %1")
                           .arg(replyPtr->errorString()));
               }
               else {
                  auto imageByteArray = replyPtr->readAll();
                  auto const imageByteArrayPtr =
                        reinterpret_cast<QByteArray*>(&imageByteArray);
                  my_room.setBackgroundImageByteArray(*imageByteArrayPtr);

                  emit backgroundImageDidLoadEvent();
               }

               replyPtr->deleteLater();
         });

//            connect(
//               &my_logger, &Logger::messageLogged,
//               this, [=](
//                        Logger::MessageKind kind,
//                        const QString& message,
//                        const QString* fromUsername,
//                        const QString* toUsername
//                     )
//               {
//                  do_logMessage(kind, message, fromUsername, toUsername);
//               }
//            );
         connect(
            &my_pingTimer, &QTimer::timeout,
                  this, [this](void) {
            do_sendPing();
            //my_pingTimer.start();
         });
      }

      //, const QMap<QString, QString>& mapOfHeaders)
      auto Client::do_fetchBackgroundAsync(const QString& url) -> void
      {
         // create HTTP request and set hostname
         QNetworkRequest request;
         request.setUrl(url);
         request.setRawHeader("User-Agent", "Seville 1.0");

//            if (!mapOfHeaders.isEmpty()) {
//               QMapIterator<QString, QString> iterator(mapOfHeaders);
//               while (iterator.hasNext())
//               {
//                  iterator.next();
//                  request.setRawHeader(
//                        QByteArray::fromStdString(
                           //iterator.key().toStdString()),
//                        QByteArray::fromStdString(
                           //iterator.value().toStdString())
//                  );
//               }
//            }

         my_networkAccessManager.get(request);
      }

      auto Client::do_clear(void) -> void
      {
         do_disconnectFromHost();

         //my_netMsgReadState = NetMsgReadState::kNetMsgReadHeaderState;
         //my_transferTimerId = 0;
         //do_setConnectionState(ConnectionState::kDisconnectedState);
         //myLog.resetState();

//         my_puidChangedFlag = false;
//         my_puidCounter = 0xf5dc385e;
//         my_puidCrc = 0xc144c580;
//         my_regCounter = 0xcf07309c;
//         my_regCrc = 0x5905f923;
         my_netMsg.clear();
         my_server.clear();
         // my_user.clear();
         my_room.clear();
         my_cipher.clear();

         my_isUserIdChanged = false;
         my_idCounter = 0xf5dc385e;
         my_idCrc = 0xc144c580;
         my_regCounter = 0xcf07309c;
         my_regCrc = 0x5905f923;
      }

      auto Client::do_resetReceiveTimer(void) -> void
      {
//         if (my_transferTimerId)
//         {
//            killTimer(my_transferTimerId);
//            my_transferTimerId = 0;
//         }
      }

      auto Client::on_backgroundDidFinishLoading(
            QNetworkReply* replyPtr) -> void
      {
            if (replyPtr->error()) {
               my_logger.appendErrorMessage(
                        //QString("Background failed to load."));
                        QString("Background load error: %1")
                        .arg(replyPtr->errorString()));
            }
            else {
               auto imageByteArray = replyPtr->readAll();
               auto const imageByteArrayPtr =
                     reinterpret_cast<QByteArray*>(&imageByteArray);
               my_room.setBackgroundImageByteArray(*imageByteArrayPtr);

               emit backgroundImageDidLoadEvent();
            }

            replyPtr->deleteLater();
      }

      auto Client::on_pingTimerDidTrigger(void) -> void
      {
         do_sendPing();
      }

      auto Client::on_socketErrorDidOccur(
            QAbstractSocket::SocketError socketError) -> void
      {
         switch (socketError) {
         case QAbstractSocket::SocketError::AddressInUseError:
            my_logger.appendErrorMessage("Address in use");
            break;
         case QAbstractSocket::SocketError::ConnectionRefusedError:
            my_logger.appendErrorMessage("Connection refused");
            break;
         case QAbstractSocket::SocketError::DatagramTooLargeError:
            my_logger.appendErrorMessage("Datagram too large");
            break;
         case QAbstractSocket::SocketError::HostNotFoundError:
            my_logger.appendErrorMessage("Host not found");
            break;
         case QAbstractSocket::SocketError::NetworkError:
            my_logger.appendErrorMessage("Network error");
            break;
         case QAbstractSocket::SocketError::OperationError:
            my_logger.appendErrorMessage("Operation error");
            break;
         case QAbstractSocket::SocketError::ProxyAuthenticationRequiredError:
            my_logger.appendErrorMessage("Proxy authentication required");
            break;
         case QAbstractSocket::SocketError::ProxyConnectionClosedError:
            my_logger.appendErrorMessage("Proxy connection closed");
            break;
         case QAbstractSocket::SocketError::ProxyConnectionRefusedError:
            my_logger.appendErrorMessage("Proxy connection refused");
            break;
         case QAbstractSocket::SocketError::ProxyConnectionTimeoutError:
            my_logger.appendErrorMessage("Proxy connection timeout");
            break;
         case QAbstractSocket::SocketError::ProxyNotFoundError:
            my_logger.appendErrorMessage("Proxy not found");
            break;
         case QAbstractSocket::SocketError::ProxyProtocolError:
            my_logger.appendErrorMessage("Proxy protocol error");
            break;
         case QAbstractSocket::SocketError::RemoteHostClosedError:
            my_logger.appendErrorMessage("Remote host closed connection");
            break;
         case QAbstractSocket::SocketError::SocketAccessError:
            my_logger.appendErrorMessage("Socket access error");
            break;
         case QAbstractSocket::SocketError::SocketAddressNotAvailableError:
            my_logger.appendErrorMessage("Socket address not available");
            break;
         case QAbstractSocket::SocketError::SocketResourceError:
            my_logger.appendErrorMessage("Socket resource error");
            break;
         case QAbstractSocket::SocketError::SocketTimeoutError:
            my_logger.appendErrorMessage("Socket timeout error");
            break;
         case QAbstractSocket::SocketError::SslHandshakeFailedError:
            my_logger.appendErrorMessage("SSL handshake failed");
            break;
         case QAbstractSocket::SocketError::SslInternalError:
            my_logger.appendErrorMessage("SSL internal error");
            break;
         case QAbstractSocket::SocketError::SslInvalidUserDataError:
            my_logger.appendErrorMessage("SSL invalid user data");
            break;
         case QAbstractSocket::SocketError::TemporaryError:
            my_logger.appendErrorMessage("Temporary error");
            break;
         case QAbstractSocket::SocketError::UnfinishedSocketOperationError:
            my_logger.appendErrorMessage("Unfinished socket operation error");
            break;
         case QAbstractSocket::SocketError::UnknownSocketError:
            my_logger.appendErrorMessage("Unknown socket error");
            break;
         case QAbstractSocket::SocketError::UnsupportedSocketOperationError:
            my_logger.appendErrorMessage("Unsupported socket operation");
            break;
         default:
            my_logger.appendErrorMessage("Unknown Socket Error");
         }

         //qCDebug(seville_log) << "ERROR: Socket Error";

         do_setConnectionState(ConnectionState::kDisconnectedState);
      }

      auto Client::on_readyReadDidOccur(void) -> void
      {
         //while (my_socket.bytesAvailable())
         do_readSocket();

//         my_logger.appendDebugMessage(
//                  QString("%1 bytes remaining to read from socket")
//                  .arg(my_socket.bytesAvailable()));
      }

      auto Client::do_readSocket(void) -> void
      {
         //netmsg::Generic& netMsg = myNetMsg;

//         if (my_transferTimerId)
//         {
//            killTimer(my_transferTimerId);
//            my_transferTimerId = 0;
//         }

         //while (do_readNetMsgHeaderFromSocket())
         //auto netMsg =

         while (0 < my_socket.bytesAvailable())
         {
            if (my_netMsg.size() < NetMsg::kHeaderSize) {
               if (my_socket.bytesAvailable() < NetMsg::kHeaderSize) {
                  break;
               }

               if (!do_readNetMsgHeaderFromSocket()) {
                  break;
               }
            }

            if (my_netMsg.size() < NetMsg::kHeaderSize)
               break;

            auto netMsgLen = my_netMsg.bodyLen();
            if (my_socket.bytesAvailable() < netMsgLen)
               break;

            if (!do_readNetMsgContentFromSocket())
               break;

            // set a timer to timeout netmsg receive
            // if (my_netMsgBody.size() < NetMsg::kHeaderSize)
            // {
            //   my_transferTimerId =
            //       startTimer(kIntervalTimeoutForTransferInMs);
            //   continue;
            // }
            // my_netMsgSize = my_netMsgBody.size();

            auto netMsgId = my_netMsg.id();
            auto netMsgRef = my_netMsg.ref();
            if (my_connectionState == ConnectionState::kHandshakingState)
            {
//               if (my_transferTimerId)
//               {
//                  killTimer(my_transferTimerId);
//                  my_transferTimerId = 0;
//               }

               if (do_determineServerByteOrder() < 0)  //myNetMsg) < 0)
               {
                  my_logger.appendDebugMessage(
                           "ERROR: Could not determine server byte order!");
                  return;
               }

               //QDataStream in(my_netMsgBody);
               //u32 user_id; //my_netmsg_in_.Ref();
               //in.skipRawData(NetMsg::kRefOffset);
               //in >> user_id;
               //my_user.setId(user_id);
               //auto userId = my_netMsgBody.ref();
               // my_user.setId(netMsgRef);
               my_userId = netMsgRef;

               if (do_sendLogon() < 0)
               {
                  my_logger.appendErrorMessage("Error sending logon");
                  return;
               }

               do_setConnectionState(ConnectionState::kConnectedState);
               my_logger.appendInfoMessage(
                        QString("Connected to %1").arg(my_server.hostname()));

               my_netMsg.clear();

               do_sendRequestRoomAndUserLists();
            }
            else if (my_connectionState == ConnectionState::kConnectedState &&
                     NetMsg::kHeaderSize <= my_netMsg.size() &&
                     netMsgId == NetMsg::kConnectionErrorKind)
            {
               do_setConnectionState(ConnectionState::kDisconnectedState);
               my_logger.appendErrorMessage("Disconnected by server");

               my_netMsg.clear();
               clear();
            }
            else if (my_connectionState == ConnectionState::kConnectedState)
            {
               auto netMsgIsValid = my_netMsg.isValid();
               auto netMsgIsRouted = do_routeReceivedNetMsg();

               auto shouldClearNetMsg = netMsgIsValid && netMsgIsRouted;
               if (shouldClearNetMsg)
                  my_netMsg.clear();
               //auto lengthActual = my_netMsg.length();
               //auto lengthExpected = my_netMsg.len() + NetMsg::kHeaderSize;
               //if (NetMsg::kHeaderSize <= lengthActual &&
               //    static_cast<i32>(lengthExpected) <= lengthActual)
            }
         }
      }

      auto Client::do_setConnectionState(
            ConnectionState connectionState) -> void
      {
         if (connectionState != ConnectionState::kConnectedState)
            my_pingTimer.stop();

         my_connectionState = connectionState;

         emit connectionStateDidChangeEvent(connectionState);
      }

      auto Client::do_roomChat(const QString& text) -> void
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return;

         if (text.length() <= 0)
            return;

         if (text.toStdString() == "'who") {
            auto userListPtr = my_room.userListPtr();
            for (auto i = u32{0}; i < userListPtr->size(); i++) {
               auto user = my_room.userListPtr()->at(i);
               my_logger.appendInfoMessage(
                        QString("%1 (%2) is here")
                        .arg(user.username())
                        .arg(user.id()));
            }
         }
         else if (text.left(QString("'w ").length()) ==
                  QString("'w ")) {
            auto split = text.split(' ');
            if (2 < split.length()) {
               auto ok = true;
               auto userId = split.at(1).toUInt(&ok);
               auto message = split.at(2);
               for (auto i = 3; i < split.length(); i++) {
                  message.append(QString(" %1").arg(split.at(i)));
               }

               if (ok) {
                  do_sendXWhisper(userId, message);
               }
            }
         }
         else if (text.left(QString("'face ").length()) ==
                  QString("'face ")) {
            auto split = text.split(' ');
            if (1 < split.length()) {
               auto ok = true;
               auto faceId = split.at(1).toUShort(&ok);
               if (ok && faceId < kNumFaces) {
                  do_sendFace(faceId);
               }
            }
         }
         else if (text.left(QString("'color ").length()) ==
                  QString("'color ")) {
            auto split = text.split(' ');
            if (1 < split.length()) {
               auto ok = true;
               auto faceColorId = split.at(1).toUShort(&ok);
               if (ok && faceColorId < kNumFaceColors) {
                  do_sendFaceColor(faceColorId);
               }
            }
         }
         else if (text.left(QString("'gotoroom ").length()) ==
                  QString("'gotoroom ")) {
            auto split = text.split(' ');
            if (1 < split.length()) {
               auto ok = true;
               auto roomId = split.at(1).toUInt(&ok);
               if (ok) {
                  do_sendGotoRoom(roomId);
               }
            }
         }
         else if (text.left(QString("'roomswithusers").length()) ==
                    QString("'roomswithusers")) {
            my_logger.appendInfoMessage("Listing rooms with users...");
            for (auto& room: *my_server.roomListPtr()) {
               auto roomName = room.roomName();
               auto roomUserCount = room.userCount();
               auto roomId = room.roomId();
               if (0 < roomUserCount) {
                  my_logger.appendInfoMessage(QString("%1 (%2), users: %3")
                                              .arg(roomName)
                                              .arg(roomId)
                                              .arg(roomUserCount));
               }
            }
         }
         else if (text.left(QString("'rooms").length()) ==
                  QString("'rooms")) {
            my_logger.appendInfoMessage("Listing all rooms...");
            for (auto& room: *my_server.roomListPtr()) {
               auto roomName = room.roomName();
               auto roomUserCount = room.userCount();
               auto roomId = room.roomId();
               my_logger.appendInfoMessage(QString("%1 (%2), users: %3")
                                           .arg(roomName)
                                           .arg(roomId)
                                           .arg(roomUserCount));
            }
         }
         else if (text.left(QString("'users").length()) ==
                  QString("'users")) {
            my_logger.appendInfoMessage("Listing all users...");
            for (auto& user: *my_server.userListPtr()) {
               auto username = user.username();
               auto userId = user.id();
               auto roomId = user.roomId();
               my_logger.appendInfoMessage(
                        QString("%1 (%2) is in room %3.")
                        .arg(username)
                        .arg(userId)
                        .arg(roomId));
            }
         }
         else {
            do_sendXTalk(text);
         }
      }

      auto Client::do_connectToHost(
            QString host, int port, QString username, int initialRoom) -> void
      {
         auto condHostTcpPort = (0 == port);
         auto actualHostTcpPort = static_cast<u16>(
                  (condHostTcpPort * kDefaultServerPort) |
                  ((!condHostTcpPort) * port)
         );

         auto actualInitialRoom = static_cast<u16>(initialRoom);

         auto logMsg = QString("Connecting to palace://%1@%2:%3/%4")
               .arg(username.toUtf8().data())
               .arg(host.toUtf8().data())
               .arg(actualHostTcpPort)
               .arg(actualInitialRoom);

         my_logger.appendInfoMessage(logMsg);

         // my_user.setUsername(username);
         my_username = username;
         my_server.setHost(host);
         my_server.setPort(actualHostTcpPort);
         my_room.setId(actualInitialRoom);

         my_logger.appendDebugMessage("Handshaking");
         do_setConnectionState(ConnectionState::kHandshakingState);
         my_socket.connectToHost(host, actualHostTcpPort);
         my_pingTimer.setInterval(60000);
         my_pingTimer.start();
      }

      auto Client::do_disconnectFromHost(void) -> void
      {
         if (my_connectionState != ConnectionState::kDisconnectedState) {
            //my_logger.appendInfoMessage("Disconnected.");
            my_socket.disconnectFromHost();
            my_pingTimer.stop();
            do_setConnectionState(ConnectionState::kDisconnectedState);
            do_clear();
         }
      }

      auto Client::do_determineClientByteOrder(void) -> void
      {
         auto isLittleEndian =
               (HostByteOrder::kLittleEndian * !Client::isBigEndian());
         auto isBigEndian =
               (HostByteOrder::kBigEndian * Client::isBigEndian());
         my_byteOrder =
               static_cast<HostByteOrder>(isLittleEndian | isBigEndian);
      }

      auto Client::do_determineServerByteOrder(void) -> int
      {
         auto result = 0;
         // TODO more consideration here.
         // constants are in <s>big</s> little endian byte order,
         // conditionally swapping them if we are a little endian system
         // seems off.
         // but shouldn't we have to swap them all? we don't.
         //QDataStream in(my_netMsgBody);
         //i32 id;
         //in >> id;
         //auto clientIsBigEndian = do_determineClientByteOrder();
         auto kind = my_netMsg.id();

         //auto littleEndianFlag = static_cast<u32>(
         //         HostByteOrder::kLittleEndian == my_byteOrderKind);
         //auto kind = (littleEndianFlag * Host::SwapU32(id)) |
         //            (!littleEndianFlag * id);

         auto clientIsBigEndian = (my_byteOrder == HostByteOrder::kBigEndian);
         auto serverByteOrderIfServerEndianMismatch =
               !clientIsBigEndian * HostByteOrder::kBigEndian |
               clientIsBigEndian * HostByteOrder::kLittleEndian;

         switch (kind) {
         case NetMsg::kUnknownServerKind:
            my_logger.appendDebugMessage("Server has Unknown Byte Order");
            my_server.setByteOrder(HostByteOrder::kUnknownEndian);
            result = 0;
            break;
         case NetMsg::kServerEndianMatchKind:
            my_logger.appendDebugMessage("Server endian matches client");
            my_server.setByteOrder(my_byteOrder);
            result = 1;
            break;
         case NetMsg::kServerEndianMismatchKind:
            my_logger.appendDebugMessage("Server endian does not match client");
            my_server.setByteOrder(serverByteOrderIfServerEndianMismatch);
            result = 1;
            break;
         default:
            my_logger.appendDebugMessage(
                     "Server did not indicate its Byte Order");
            my_server.setByteOrder(HostByteOrder::kUnknownEndian);
            result = 0;
         }

         return result;
      }

      /*
      int Client::do_readDataIntoNetMsg(netmsg::Generic& netMsg, i32 maxSize) {
         if (netmsg::kByteSizeMaximum < maxSize)
            return 0;

         auto nBytesBeforeRead = netMsg.size();
         if (nBytesBeforeRead == netmsg::kByteSizeMaximum)
            return 0;

         auto nBytesAvailable = mySocket.bytesAvailable();
         auto netMsgSize = netMsg.size();
         int isHeader = netMsgSize < netmsg::kByteSizeOfHeader;
         i32 chunkReadSize =
               (isHeader * netmsg::kByteSizeOfHeader) |
               (!isHeader * kByteSizeOfDefaultChunkRead);

         while (0 < nBytesAvailable && netMsgSize < maxSize) {
            //int remainingSize = (maxSize - netMsgSize);
            QByteArray chunk = mySocket.read(chunkReadSize);
            netMsg.append(chunk);
            //int shouldReadMinChunkSize =
            //    remainingSize < kPalClientDefaultChunkReadSize;
            //chunkReadSize =
            //(!shouldReadMinChunkSize * kPalClientDefaultChunkReadSize) |
            //(shouldReadMinChunkSize * kPalClientMinChunkReadSize);
            netMsgSize = netMsg.size();
            nBytesAvailable = mySocket.bytesAvailable();
            chunkReadSize = nBytesAvailable;
         }

         int nBytesRead = netMsgSize - nBytesBeforeRead;
         return nBytesRead;
      }
      */

      auto Client::do_determineIsConnected(void) const -> bool
      {
         return my_socket.state() != QTcpSocket::ConnectedState ||
               my_connectionState != ConnectionState::kConnectedState;
      }

      auto Client::do_determineShouldSwapEndianness(void) const
         -> NetMsgOptions
      {
         auto notUnknownClientByteOrder =
               my_byteOrder != HostByteOrder::kUnknownEndian;

         auto notUnknownServerByteOrder =
               my_server.byteOrder() != HostByteOrder::kUnknownEndian;

         auto notSameByteOrder = my_byteOrder != my_server.byteOrder();

         auto shouldSwap = notUnknownClientByteOrder &&
               notUnknownServerByteOrder && notSameByteOrder;

         //return NetMsgOptions::kEndianSwap * should_swap |
         //      NetMsgOptions::kNoEndianSwap * !should_swap;

         //return should_swap;

         if (shouldSwap) {
            return NetMsgOptions::kEndianSwap;
         }
         else {
            return NetMsgOptions::kNoEndianSwap;
         }
      }

      /**
       * @brief do_receiveAltLogon
       * @return number of network messages handled
       */
      auto Client::do_receiveAltLogon(void) -> int
      {
         my_logger.appendDebugMessage("> AltLogon");

         //auto netmsg_logon = static_cast<netmsg::NetMsgLogon>(my_netmsg_);
         //QDataStream in(my_netMsgBody);
         //in.skipRawData(76);
         //my_netmsg_in_.u32At(NetMsg::kPayloadOffset+80);
         //my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         auto puidCounter = my_netMsg.streamReadU32();
                  //NetMsg::kLogonPuidCounterOffset);
         //in >> puid_counter;
         //my_netmsg_in_.u32At(NetMsg::kPayloadOffset+80);
         auto puidCrc = my_netMsg.streamReadU32();
                  //NetMsg::kLogonPuidCrcOffset);
         //in >> puid_crc;

         if (my_idCounter != puidCounter ||
             my_idCrc != puidCrc)
         {
            // my_user.setIdCrc(puidCrc);
            my_idCrc = puidCrc;
            // my_user.setIdCounter(puidCounter);
            my_idCounter = puidCounter;
            // my_user.setIdChanged(true);
            my_isUserIdChanged = true;
            my_logger.appendDebugMessage("My user id changed by server");
         }
         else {
            my_logger.appendDebugMessage("My user id remains unchanged");
         }

         return 1;
      }

      auto Client::do_receiveAltRoomDescription(void) -> int
      {
         my_logger.appendDebugMessage("> AltRoomDescription");

         // stub
         return 1;
      }

      auto Client::do_receiveAssetIncoming(void) -> int
      {
         my_logger.appendDebugMessage("> AssetIncoming");

         // stub
         return 1;
      }

      auto Client::do_receiveAssetQuery(void) -> int
      {
         my_logger.appendDebugMessage("> AssetQuery");

         // stub
         return 1;
      }

      auto Client::do_receiveAuthenticate(void) -> int
      {
         my_logger.appendDebugMessage("> Authenticate");

         // stub
         return 1;
      }

      auto Client::do_receiveBlowthru(void) -> int
      {
         my_logger.appendDebugMessage("> BlowThru");

         // stub
         return 1;
      }

      auto Client::do_receiveConnectionError(void) -> int
      {
         my_logger.appendDebugMessage("> ConnectionError");

         do_disconnectFromHost();
         my_logger.appendInfoMessage("Disconnected.");

         return 1;
      }

      auto Client::do_receiveDoorLock(void) -> int
      {
         my_logger.appendDebugMessage("> DoorLock");

         // stub
         return 1;
      }

      auto Client::do_receiveDoorUnlock(void) -> int
      {
         my_logger.appendDebugMessage("> DoorUnlock");

         // stub
         return 1;
      }

      auto Client::do_receiveDraw(void) -> int
      {
         my_logger.appendDebugMessage("> Draw");

         // stub
         return 1;
      }

      auto Client::do_receiveHttpServerLocation(void) -> int
      {
         my_logger.appendDebugMessage("> HttpServerLocation");

         auto result = 1;
         //my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         auto url = my_netMsg.streamReadQString(256);
         my_server.setHttpServerLocation(url);
         my_logger.appendDebugMessage(QString("HTTP Server is %1").arg(url));

         return result;
      }

      auto Client::do_receiveMovement(void) -> int
      {
         my_logger.appendDebugMessage("> Movement");

         auto userId = my_netMsg.ref();
         auto y = my_netMsg.streamReadU16();
         auto x = my_netMsg.streamReadU16();

         auto userPtr = my_room.userPtrWithId(userId);
         userPtr->setX(x);
         userPtr->setY(y);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveNavigationError(void) -> int
      {
         my_logger.appendDebugMessage("> NavError!");

         // stub
         return 1;
      }

      auto Client::do_receivePictureMove(void) -> int
      {
         my_logger.appendDebugMessage("> PictMove");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      auto Client::do_receivePing(void) -> int
      {
         my_logger.appendDebugMessage("Ping!");

         //my_logger.appendDebugMessage("> Ping!");

         do_sendPong();
         return 1;
      }

      auto Client::do_receivePong(void) -> int
      {
         my_logger.appendDebugMessage("> Pong");

         // stub
         // reset ping timer?
         //my_logger.appendDebugMessage("> Pong!");

         return 1;
      }

      auto Client::do_receivePropDelete(void) -> int
      {
         my_logger.appendDebugMessage("> PropDelete");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      auto Client::do_receivePropMove(void) -> int
      {
         my_logger.appendDebugMessage("> PropMove");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      auto Client::do_receivePropNew(void) -> int
      {
         my_logger.appendDebugMessage("> PropNew");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      auto Client::do_receiveRoomDescend(void) -> int
      {
         my_logger.appendDebugMessage("> RoomDescend");

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveRoomDescription(void) -> int
      {
         my_logger.appendDebugMessage("> RoomDescription");

         my_room.setFlags(my_netMsg.streamReadU32());
         my_room.setFace(my_netMsg.streamReadU32());
         my_room.setId(my_netMsg.streamReadU16());

         auto roomNameOffset = my_netMsg.streamReadU16();
         auto backgroundImageNameOffset = my_netMsg.streamReadU16();
         auto artistNameOffset = my_netMsg.streamReadU16();
         auto passwordOffset = my_netMsg.streamReadU16();

         my_room.setHotspotCount(my_netMsg.streamReadU16());

         auto hotspotOffset = my_netMsg.streamReadU16();

         my_room.setImageCount(my_netMsg.streamReadU16());
         //auto backgroundImageOffset =
               my_netMsg.streamReadU16();

         my_room.setDrawCommandsCount(my_netMsg.streamReadU16());
         //auto firstDrawCommand =
               my_netMsg.streamReadU16();

         my_room.setUserCount(my_netMsg.streamReadU16());
         //my_netMsg.streamSkip(sizeof(u16));

         my_room.setLoosePropCount(my_netMsg.streamReadU16());
         //auto firstLooseProp =
               my_netMsg.streamReadU16();
         //auto unknown =
               my_netMsg.streamReadU16();
         //auto roomDataLen =
               my_netMsg.streamReadU16();

         auto dataOffset = my_netMsg.streamCursorPosition();

         auto roomName = my_netMsg.pascalQStringAt(dataOffset + roomNameOffset);
         my_room.setRoomName(roomName);

         auto backgroundImageName =
               my_netMsg.pascalQStringAt(
                  dataOffset + backgroundImageNameOffset);
         my_room.setBackgroundImageName(backgroundImageName);

         my_room.setArtistName(my_netMsg.pascalQStringAt(
                  dataOffset + artistNameOffset));

         my_room.setPassword(
                  my_netMsg.pascalQStringAt(dataOffset + passwordOffset));

         my_netMsg.setStreamCursorPosition(dataOffset + hotspotOffset);
         auto hotspotCount = my_room.hotspotCount();
         for (auto i = 0; i < hotspotCount; i++) {
            auto hotspot = Hotspot();
            // TODO fill out hotspot
            my_room.hotspotListPtr()->push_back(hotspot);
         }

         auto backgroundImageUri =
               my_server.httpServerLocation() + my_room.backgroundImageName();

         my_logger.appendDebugMessage(QString("Fetching background from %1")
                          .arg(backgroundImageUri));

         if (0 < backgroundImageUri.length())
            do_fetchBackgroundAsync(backgroundImageUri);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveRoomUserList(void) -> int
      {
         my_logger.appendDebugMessage("> RoomUserList");

         my_room.userListPtr()->clear();

         auto roomUserCount = i32(my_netMsg.ref());
         for (auto i = 0; i < roomUserCount; i++) {
            //auto user = User();
            do_processUserNew();
            //my_room.userListPtr()->push_back(user);
         }

         return 1;
      }

      auto Client::do_receiveServerVersion(void) -> int
      {
         my_logger.appendDebugMessage("> ServerVersion");

         auto serverVersion = my_netMsg.u32At(NetMsg::kRefOffset);
         my_server.setVersion(serverVersion);
         my_logger.appendDebugMessage(
                  QString("Sever version %1").arg(serverVersion));
         return 1;
      }

      auto Client::do_receiveServerInfo(void) -> int
      {
         my_logger.appendDebugMessage("> ServerInfo");

         // stub

         return 1;
      }

      auto Client::do_processUserNew(void) -> User
      {
         my_logger.appendDebugMessage("> UserNew");

         auto user = User();

         user.setId(my_netMsg.streamReadU32());
         user.setY(my_netMsg.streamReadU16());
         user.setX(my_netMsg.streamReadU16());

         for (auto i = 0; i < User::kNumPropCells; i++) {
            auto prop = Prop();
            prop.setId(my_netMsg.streamReadU32());
            prop.setCrc(my_netMsg.streamReadU32());
            user.propListPtr()->push_back(prop);
         }

         user.setRoomId(my_netMsg.streamReadU16());
         user.setFace(my_netMsg.streamReadU16());
         user.setColor(my_netMsg.streamReadU16());

         auto unknown1 = my_netMsg.streamReadU16();
         auto unknown2 = my_netMsg.streamReadU16();
         (void)unknown1;
         (void)unknown2;

         user.setPropNum(my_netMsg.streamReadU16());
         //if (user.propNum() < User::kNumPropCells) {
         //   auto propPtr = &user.propListPtr()->at(user.propNum());
         //   propPtr->setId(0);
         //   propPtr->setCrc(0);
         //   // TODO ...
         //}

         auto usernameStride = my_netMsg.streamReadU8();
         user.setUsername(
                  my_netMsg.streamReadAndDecodeQString(usernameStride));
         my_netMsg.streamSkip(
                  NetMsg::kLogonMaximumUsernameSize - usernameStride);

         //user.setUsername(my_netMsg.streamReadFixedQString(usernameLen));
         //auto usernamePaddedLen = (4 - (usernameLen & 3)) - 1; // + usernameLen;
         //my_netMsg.streamSkip(usernamePaddedLen);

         my_room.userListPtr()->push_back(user);
         my_room.setUserCount(my_room.userCount() + 1);

         return user;
      }

      auto Client::do_receiveUserNew(void) -> int
      {
         auto user = do_processUserNew();

         my_logger.appendInfoMessage(
                  QString("%1 has entered the room").arg(user.username()));

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserColor(void) -> int
      {
         my_logger.appendDebugMessage("> UserColor");

         auto userId = my_netMsg.ref();
         auto value = my_netMsg.streamReadU16();

         auto userPtr = my_server.userPtrWithId(userId);
         if (userPtr != nullptr) {
            userPtr->setColor(value);
            emit viewNeedsUpdatingEvent();
         }

         return 1;
      }

      auto Client::do_receiveUserExitRoom(void) -> int
      {
         my_logger.appendDebugMessage("> UserExitRoom");

         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);
         my_room.removeUserWithId(userId);
         my_logger.appendInfoMessage(
                  QString("%1 has left the room.").arg(user.username()));

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserFace(void) -> int
      {
         my_logger.appendDebugMessage("> UserFace");

         auto userId = my_netMsg.ref();
         auto value = my_netMsg.streamReadU16();

         auto userPtr = my_server.userPtrWithId(userId);
         if (userPtr != nullptr) {
            userPtr->setFace(value);
            emit viewNeedsUpdatingEvent();
         }

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserProp(void) -> int
      {
         my_logger.appendDebugMessage("> UserProp");

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserDescription(void) -> int
      {
         my_logger.appendDebugMessage("> UserDescription");

         // stub
         return 1;
      }

      auto Client::do_receiveUserRename(void) -> int
      {
         my_logger.appendDebugMessage("> UserRename");

         auto userId = my_netMsg.streamReadI32();
         auto userPtr = my_server.userPtrWithId(userId);
         if (userPtr == nullptr)
            return 0;

         auto oldUsername = userPtr->username();

         auto usernameLen = my_netMsg.streamReadU8();
         auto username = my_netMsg.streamReadFixedQString(usernameLen);
         userPtr->setUsername(username);

         my_logger.appendInfoMessage(QString("%1 has changed name to %2")
                                     .arg(oldUsername)
                                     .arg(username));

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserLeaving(void) -> int
      {
         my_logger.appendDebugMessage("> UserLeaving");

         auto userCount = my_netMsg.streamReadU32();
         my_room.setUserCount(userCount);

         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);
         my_logger.appendInfoMessage(
                  QString("%1 has signed off.").arg(user.username()));
         my_room.removeUserWithId(userId);

         user = my_server.userWithId(userId);
         my_server.removeUserWithId(userId);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      auto Client::do_receiveUserLoggedOnAndMax(void) -> int
      {
         my_logger.appendDebugMessage("> UserLoggedOnAndMax");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      auto Client::do_receiveUserStatus(void) -> int
      {
         my_logger.appendDebugMessage("> UserStatus");

         // stub
         return 1;
      }

      auto Client::do_receiveRoomList(void) -> int
      {
         my_logger.appendDebugMessage("> RoomList");

         auto roomListPtr = my_server.roomListPtr();
         roomListPtr->clear();

         auto roomCount = my_netMsg.ref();
         //if()

         for (auto i = u32{0}; i < roomCount; i++) {
            auto room = Room();
            room.setId(my_netMsg.streamReadU32());
            room.setFlags(my_netMsg.streamReadU16());
            room.setUserCount(my_netMsg.streamReadU16());
            // my_netMsg.streamSkip(sizeof(u16));
            auto roomNameLen = my_netMsg.streamReadU8();
            room.setRoomName(
                     my_netMsg.streamReadAndDecodeQString(roomNameLen));
                     //my_netMsg.streamReadFixedQString(roomNameLen));
            auto roomNamePaddedLen = (4 - (roomNameLen & 3)) - 1; // + roomNameLen;
            my_netMsg.streamSkip(roomNamePaddedLen);

            roomListPtr->push_back(room);
         }

         return 1;
      }

      auto Client::do_receiveUserList(void) -> int
      {
         my_logger.appendDebugMessage("> UserList");

         auto userListPtr = my_server.userListPtr();
         userListPtr->clear();

         auto userCount = my_netMsg.ref();
         //if()

         for (auto i = u32{0}; i < userCount; i++) {
            auto user = User();
            user.setId(my_netMsg.streamReadU32());
            user.setFlags(my_netMsg.streamReadU16());
            user.setRoomId(my_netMsg.streamReadU16());
            auto usernameLen = my_netMsg.streamReadU8();
            //user.setUsername(my_netMsg.streamReadFixedQString(usernameLen));
            user.setUsername(my_netMsg.streamReadAndDecodeQString(usernameLen));
            auto usernamePaddedLen = (4 - (usernameLen & 3)) - 1; // + usernameLen;
            my_netMsg.streamSkip(usernamePaddedLen);

            userListPtr->push_back(user);
         }

         return 1;
      }

      auto Client::do_receiveSpotMove(void) -> int
      {
         my_logger.appendDebugMessage("> SpotMove");

         // stub
         return 1;
      }

      auto Client::do_receiveSpotState(void) -> int
      {
         my_logger.appendDebugMessage("> SpotState");

         // stub
         return 1;
      }

      auto Client::do_receiveTalk(void) -> int
      {
         my_logger.appendDebugMessage("> Talk");

         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);
         //auto message = my_netMsg.streamReadByteArray(my_netMsg.len());
         auto message =
               my_netMsg.streamReadAndDecodeQString(my_netMsg.bodyLen());

         my_logger.appendChatMessage(user.username(), message);

         return 1;
      }

      auto Client::do_receiveWhisper(void) -> int
      {
         my_logger.appendDebugMessage("> Whisper");

         auto userId = my_netMsg.ref();
         auto user = my_server.userWithId(userId);

         // auto userIdTo = my_netMsg.streamReadU32();
         // (void)userIdTo;
         // auto messageLen = my_netMsg.streamReadU16();
         // auto cipherLen = messageLen - 3;
         // auto ciphertext = my_netMsg.streamReadByteArray(cipherLen);
         //auto ciphertext = my_netMsg.streamReadAndDecodeQString(cipherLen);
         // auto message = my_cipher.decipher(ciphertext);
         auto message = my_netMsg.streamReadAndDecodeQString(
                  my_netMsg.bodyLen());

         auto usernameFrom = user.username();
         //auto id = user.id();
         my_logger.appendWhisperMessage(
//                  QString("%1 (%2)")
//                  .arg(username)
//                  .arg(id),
         usernameFrom,
         message);

         return 1;
      }

      auto Client::do_receiveXTalk(void) -> int
      {
         my_logger.appendDebugMessage("> XTalk");

         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);

         auto messageLen = my_netMsg.streamReadU16();
         auto cipherLen = messageLen - 3;
         auto ciphertext = my_netMsg.streamReadByteArray(cipherLen);
         //auto ciphertext = my_netMsg.streamReadAndDecodeQString(cipherLen);
         auto message = my_cipher.decipher(ciphertext);

         auto usernameFrom = user.username();
         //auto id = user.id();
         my_logger.appendChatMessage(
//                  QString("%1 (%2)")
//                  .arg(username)
//                  .arg(id),
         usernameFrom,
         message);

         return 1;
      }

      auto Client::do_receiveXWhisper(void) -> int
      {
         my_logger.appendDebugMessage("> XWhisper");

         auto userId = my_netMsg.ref();
         auto user = my_server.userWithId(userId);

         // auto userIdTo = my_netMsg.streamReadU32();
         // (void)userIdTo;
         auto messageLen = my_netMsg.streamReadU16();
         auto cipherLen = messageLen - 3;
         auto ciphertext = my_netMsg.streamReadByteArray(cipherLen);
         //auto ciphertext = my_netMsg.streamReadAndDecodeQString(cipherLen);
         auto message = my_cipher.decipher(ciphertext);

         auto usernameFrom = user.username();
         //auto id = user.id();
         my_logger.appendWhisperMessage(
//                  QString("%1 (%2)")
//                  .arg(username)
//                  .arg(id),
         usernameFrom,
         message);

         return 1;
      }

      auto Client::do_routeReceivedNetMsg(void) -> int
      {
         auto result = 0;
         auto netMsgIdTmp = my_netMsg.id(); //u32At(NetMsg::kIdOffset);
         auto serverByteOrder = my_server.byteOrder();
         auto byteOrderMatch = (serverByteOrder == my_byteOrder);
         auto kind = (byteOrderMatch * netMsgIdTmp) |
               (!byteOrderMatch * NetMsg::SwapI32(netMsgIdTmp));

         //auto netMsgId = my_netMsg.id(); //u32At(NetMsg::kIdOffset);
         //auto netMsgLen = my_netMsg.len(); //u32At(NetMsg::kLenOffset);
         //auto netMsgRef = my_netMsg.ref(); //u32At(NetMsg::kRefOffset);

         //auto kind = my_netMsgHeader.id();
         //auto size = my_netMsgHeader.size();

//         if (size < NetMsg::kHeaderSize) {
//            my_logger.appendDebugMessage("Received NetMsg too small!");
//            qCDebug(seville_log)
//                  << "Received NetMsg that is too small to interpret";
//            qCDebug(seville_log)
//                  << "kind: "
//                  << QString("0x%1").arg(my_msgId, 2, 16, QChar('0'));
//            qCDebug(seville_log)
//                  << "size expected: "
//                  << QString("0x%1").arg(my_msgLen, 2, 16, QChar('0'));
//            qCDebug(seville_log)
//                  << "reference code: "
//                  << QString("0x%1").arg(my_msgRef, 2, 16, QChar('0'));
//            return -1;
//         }
//         else if (NetMsg::kMaximumSize < size) {
//            my_logger.appendDebugMessage("Received NetMsg too large!");
//            qCDebug(seville_log)
//                  << "Received NetMsg that is too large to interpret";
//            qCDebug(seville_log)
//                  << "kind: "
//                  << QString("0x%1").arg(my_msgId, 2, 16, QChar('0'));
//            qCDebug(seville_log)
//                  << "size expected: "
//                  << QString("0x%1").arg(my_msgLen, 2, 16, QChar('0'));
//            qCDebug(seville_log)
//                  << "reference code: "
//                  << QString("0x%1").arg(my_msgRef, 2, 16, QChar('0'));
//            return -2;
//         }

         //u32 zNetMsgId = doCsbolecDw_(netMsgId);
         //switch (id) {
            //case Net::Msg::LogonKind:
            //    res = sendLogon_();
            //    break;

         my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);
         switch(kind) {
         case NetMsg::kAltLogonKind:
            result += do_receiveAltLogon();
            break;
         case NetMsg::kConnectionErrorKind:
            result += do_receiveConnectionError();
            break;
         case NetMsg::kServerVersionKind:
            result += do_receiveServerVersion();
            break;
         case NetMsg::kServerInfoKind:
            result += do_receiveServerInfo();
            break;
         case NetMsg::kUserStatusKind:
            result += do_receiveUserStatus();
            break;
         case NetMsg::kUserLoggedOnAndMaxKind:
            result += do_receiveUserLoggedOnAndMax();
            break;
         case NetMsg::kHttpServerLocationKind:
            result += do_receiveHttpServerLocation();
            break;
         case NetMsg::kRoomUserListKind:
            result += do_receiveRoomUserList();
            break;
         case NetMsg::kServerUserListKind:
            result += do_receiveUserList();
            break;
         case NetMsg::kServerRoomListKind:
            result += do_receiveRoomList();
            break;
         case NetMsg::kRoomDescendKind:
            result += do_receiveRoomDescend();
            break;
         case NetMsg::kUserNewKind:
            result += do_receiveUserNew();
            break;
         case NetMsg::kPingKind:
            result += do_receivePing();
            break;
         case NetMsg::kPongKind:
            result += do_receivePong();
            break;
         case NetMsg::kXTalkKind:
            result += do_receiveXTalk();
            break;
         case NetMsg::kXWhisperKind:
            result += do_receiveXWhisper();
            break;
         case NetMsg::kTalkKind:
            result += do_receiveTalk();
            break;
         case NetMsg::kWhisperKind:
            result += do_receiveWhisper();
            break;
         case NetMsg::kAssetIncomingKind:
            result += do_receiveAssetIncoming();
            break;
         case NetMsg::kAssetQueryKind:
            result += do_receiveAssetQuery();
            break;
         case NetMsg::kMovementKind:
            result += do_receiveMovement();
            break;
         case NetMsg::kUserColorKind:
            result += do_receiveUserColor();
            break;
         case NetMsg::kUserFaceKind:
            result += do_receiveUserFace();
            break;
         case NetMsg::kUserPropKind:
            result += do_receiveUserProp();
            break;
         case NetMsg::kUserDescriptionKind:
            result += do_receiveUserDescription();
            break;
         case NetMsg::kUserRenameKind:
            result += do_receiveUserRename();
            break;
         case NetMsg::kUserLeavingKind:
            result += do_receiveUserLeaving();
            break;
         case NetMsg::kUserExitRoomKind:
            result += do_receiveUserExitRoom();
            break;
         case NetMsg::kPropMoveKind:
            result += do_receivePropMove();
            break;
         case NetMsg::kPropDeleteKind:
            result += do_receivePropDelete();
            break;
         case NetMsg::kPropNewKind:
            result += do_receivePropNew();
            break;
         case NetMsg::kDoorLockKind:
            result += do_receiveDoorLock();
            break;
         case NetMsg::kDoorUnlockKind:
            result += do_receiveDoorUnlock();
            break;
         case NetMsg::kPictMoveKind:
            result += do_receivePictureMove();
            break;
         case NetMsg::kSpotStateKind:
            result += do_receiveSpotState();
            break;
         case NetMsg::kSpotMoveKind:
            result += do_receiveSpotMove();
            break;
         case NetMsg::kDrawKind:
            result += do_receiveDraw();
            break;
         case NetMsg::kNavErrorKind:
            result += do_receiveNavigationError();
            break;
         case NetMsg::kBlowThruKind:
            result += do_receiveBlowthru();
            break;
         case NetMsg::kAuthenticateKind:
            result += do_receiveAuthenticate();
            break;
         case NetMsg::kAltRoomDescriptionKind:
            result += do_receiveAltRoomDescription();
            break;
         case NetMsg::kRoomDescriptionKind:
            result += do_receiveRoomDescription();
            break;
         default:
            auto logMessage =
                  QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }")
                  .arg(my_netMsg.id(), 2, 16, QChar('0'))
                  .arg(my_netMsg.bodyLen(), 2, 16, QChar('0'))
                  .arg(my_netMsg.ref(), 2, 16, QChar('0'));

            my_logger.appendDebugMessage(logMessage);
            //result = 0;
         }

         return result;
      }

      auto Client::do_sendLogon(void) -> int
      {
         auto shouldSwapEndianness = do_determineShouldSwapEndianness();
         auto logonMsg = NetMsg(shouldSwapEndianness);

         //QByteArray logon_netmsg;
         //QDataStream out(&logon_netmsg, QIODevice::WriteOnly);
         //buf.setByteOrder();
         //stream.setDevice

//         out << NetMsg::kLogonKind;
//         out << NetMsg::kLogonSize;
//         out << NetMsg::kNoRef;
         //logonNetmsg.setId(NetMsg::kLogonKind);
         //logonNetmsg.setLen(NetMsg::kLogonSize);
         //logonNetmsg.setRef(NetMsg::kNoRef);

         logonMsg.setId(NetMsg::kLogonKind);
         logonMsg.setBodyLen(NetMsg::kLogonSize);
         logonMsg.setRef(NetMsg::kNoRef);

         logonMsg.appendU32(my_regCrc);
         logonMsg.appendU32(my_regCounter);
         logonMsg.appendFixedPascalQString(
                  my_username, NetMsg::kLogonMaximumUsernameSize);
         logonMsg.appendFixedPascalQString(
                  my_wizpass, NetMsg::kLogonWizardPasswordSize);
         logonMsg.appendU32(
                  FlagAuxOptions::kAuthenticate | FlagAuxOptions::kWin32);
         logonMsg.appendU32(my_idCounter);
         logonMsg.appendU32(my_idCrc);
         logonMsg.appendU32(kMagicFromPChat);
         logonMsg.appendU32(kMagicFromPChat);
         logonMsg.appendU32(kMagicFromPChat);
         logonMsg.appendU16(my_room.roomId());
         logonMsg.appendFixedQString(kIdent, kIdentLen);
         logonMsg.appendU32(0);
         logonMsg.appendU32(
                  UploadCapabilities::kPalaceAssetUpload |
                  UploadCapabilities::kHttpFileUpload);
         logonMsg.appendU32(
                  DownloadCapabilities::kPalaceAssetDownload |
                  DownloadCapabilities::kPalaceFileDownload |
                  DownloadCapabilities::kHttpFileDownload |
                  DownloadCapabilities::kHttpFileExtendedDownload);
         logonMsg.appendU32(EngineCapabilities2d::kPalace2dEngine);
         logonMsg.appendU32(GraphicsCapabilities2d::kGif87);
         logonMsg.appendU32(GraphicsCapabilities3d::kNo3dGraphics);

               // Mark

//         netmsg::LogonObject netmsg_logon(
//                  do_determine_should_swap_endianness());
//         netmsg_logon.set_reg_crc(my_user_.RegCrc());
//         netmsg_logon.set_reg_counter(my_user_.RegCounter());
//         netmsg_logon.set_username(my_user_.Username());
//         netmsg_logon.set_wizard_password(my_user_.WizPass());

//         netmsg_logon.set_flags(
//                  static_cast<u32>(FlagAuxOptions::kAuthenticate) |
//                  static_cast<u32>(FlagAuxOptions::kWin32));

//         netmsg_logon.set_puid_counter(my_puid_counter_);
//         netmsg_logon.set_puid_crc(my_puid_crc_);
//         netmsg_logon.set_demo_elapsed(kMagicFromPChat);
//         netmsg_logon.set_total_elapsed(kMagicFromPChat);
//         netmsg_logon.set_demo_limit(kMagicFromPChat);
//         netmsg_logon.set_initial_room_id(my_room_.RoomId());
//         netmsg_logon.set_reserved(kIdent);
//         netmsg_logon.set_upload_requested_protocol_version(0);

//         netmsg_logon.set_upload_capabilities(
//                  static_cast<u32>(UploadCapabilities::kPalaceAssetUpload) |
//                  static_cast<u32>(UploadCapabilities::kHttpFileUpload));

//         netmsg_logon.set_download_capabilities(
//                  static_cast<u32>(
//                     DownloadCapabilities::kPalaceAssetDownload) |
//                  static_cast<u32>(
//                     DownloadCapabilities::kPalaceFileDownload) |
//                  static_cast<u32>(
//                     DownloadCapabilities::kHttpFileDownload) |
//                  static_cast<u32>(
//                     DownloadCapabilities::kHttpFileExtendedDownload));

//         netmsg_logon.set_engine_capabilities_2d(
//                  static_cast<u32>(EngineCapabilities2d::kPalace2dEngine));
//         netmsg_logon.set_graphics_capabilities_2d(
//                  static_cast<u32>(GraphicsCapabilities2d::kGif87));
//         netmsg_logon.set_graphics_capabilities_3d(
//                  static_cast<u32>(GraphicsCapabilities3d::kNo3dGraphics));

               // End Mark

         //netMsgRxDs_->setByteOrder(myServer.byteOrder());
         //if (ds.skipRawData(Net::Msg::kNet::MsgHeaderSize) < 0) {
         //return false; }

         /* Header */
//         msg.appendU32(netmsg::Kind::LogonKind);
//         msg.appendI32(netmsg::kByteSizeOfLogon);
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
//         msg.appendU32(kUlCapsAssetsPalace | kUlCapsFilesHttp);
         /* ulUploadCaps */
//         msg.appendU32(kDlCapsAssetsPalace | kDlCapsFilesPalace |
         //   kDlCapsFilesHttp | kDlCapsFilesHttpSvr); /* ulDownloadCaps */
//         msg.appendU32(0); /* ul2dEngineCaps* */
//         msg.appendU32(0); /* ul2dGraphicsCaps* */
//         msg.appendU32(0); /* ul3dEngineCaps* */

         //my_socket_.write(netmsg_logon);
         return my_socket.write(logonMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendAuthenticate(
            const QString& username, const QString& password) -> int
      {
         // TODO stub
         (void)username;
         (void)password;

         auto result = do_determineIsConnected();
         if (!result)
            return result;

         auto authMsg = NetMsg(do_determineShouldSwapEndianness());

         authMsg.setId(NetMsg::kAuthenticateKind);
         authMsg.setBodyLen(0);
         authMsg.setRef(NetMsg::kNoRef);

         return my_socket.write(authMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendTalk(const QString& text) -> int
      {
         auto talkMsg = NetMsg(do_determineShouldSwapEndianness());

         talkMsg.setId(NetMsg::kTalkKind);
         talkMsg.setBodyLen(text.length());
         talkMsg.setRef(my_userId);

         talkMsg.append(text.toUtf8());
         talkMsg.append('\0');

         return my_socket.write(talkMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendXTalk(const QString& text) -> int
      {
         auto xTalkMsg = NetMsg(do_determineShouldSwapEndianness());

         xTalkMsg.setId(NetMsg::kXTalkKind);
         auto len = text.length() + 3;
         xTalkMsg.setBodyLen(len);
         xTalkMsg.setRef(my_userId);

         auto plaintext = text.toUtf8();
         auto ciphertext = my_cipher.encipher(plaintext);
         xTalkMsg.appendU16(len);
         xTalkMsg.append(ciphertext);
         xTalkMsg.append('\0');

         return my_socket.write(xTalkMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendXWhisper(const u32 userId, const QString& text) -> int
      {
         auto msg = NetMsg(do_determineShouldSwapEndianness());

         msg.setId(NetMsg::kXWhisperKind);
         auto len = NetMsg::kXWhisperBodyLen + text.length() + 3;
         msg.setBodyLen(len);
         msg.setRef(my_userId);

         auto plaintext = text.toUtf8();
         auto ciphertext = my_cipher.encipher(plaintext);
         msg.appendU32(userId);
         msg.appendU16(text.length() + 3);
         msg.append(ciphertext);
         msg.append('\0');

         return my_socket.write(msg);
         //return my_socket.flush();
      }

      auto Client::do_sendPing(void) -> int
      {
         auto pingMsg = palace::NetMsg(do_determineShouldSwapEndianness());

         pingMsg.setId(NetMsg::kPingKind);
         pingMsg.setBodyLen(0);
         pingMsg.setRef(NetMsg::kNoRef);

         my_logger.appendDebugMessage(QString("Ping!"));
         return my_socket.write(pingMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendPong(void) -> int
      {
         auto pongMsg = palace::NetMsg(do_determineShouldSwapEndianness());

         pongMsg.setId(NetMsg::kPongKind);
         pongMsg.setBodyLen(0);
         pongMsg.setRef(NetMsg::kNoRef);

         my_logger.appendDebugMessage(QString("Pong!"));
         return my_socket.write(pongMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendGotoRoom(i16 roomId) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         my_room.userListPtr()->clear();
         my_room.setId(roomId);

         auto gotoroomMsg = palace::NetMsg(do_determineShouldSwapEndianness());
         gotoroomMsg.setId(NetMsg::kGotoRoomKind);
         gotoroomMsg.setBodyLen(NetMsg::kGotoRoomLen);
         gotoroomMsg.setRef(NetMsg::kNoRef);

         gotoroomMsg.appendI16(roomId);

         return my_socket.write(gotoroomMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendMove(i16 x, i16 y) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto moveMsg = palace::NetMsg();
         moveMsg.setId(NetMsg::kMovementKind);
         moveMsg.setBodyLen(NetMsg::kMovementLen);
         moveMsg.setRef(NetMsg::kNoRef);

         moveMsg.appendI16(y);
         moveMsg.appendI16(x);

         // emit userMove signal to invalidate view
         return my_socket.write(moveMsg);
         //return my_socket.flush();
      }

      auto Client::do_sendRequestRoomList(void) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kServerRoomListKind);
         msg.setBodyLen(NetMsg::kNoLen);
         msg.setRef(NetMsg::kNoRef);

         return my_socket.write(msg);
      }

      auto Client::do_sendRequestUserList(void) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kServerUserListKind);
         msg.setBodyLen(NetMsg::kNoLen);
         msg.setRef(NetMsg::kNoRef);

         return my_socket.write(msg);
      }

      auto Client::do_sendRequestRoomAndUserLists(void) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto serverRoomListMsg = palace::NetMsg();
         serverRoomListMsg.setId(NetMsg::kServerRoomListKind);
         serverRoomListMsg.setBodyLen(NetMsg::kNoLen);
         serverRoomListMsg.setRef(NetMsg::kNoRef);

         auto serverUserListMsg = palace::NetMsg();
         serverUserListMsg.setId(NetMsg::kServerUserListKind);
         serverUserListMsg.setBodyLen(NetMsg::kNoLen);
         serverUserListMsg.setRef(NetMsg::kNoRef);

         auto res = my_socket.write(serverRoomListMsg);
         res &= my_socket.write(serverUserListMsg);

         return res;
      }

      auto Client::do_sendFace(u16 faceId) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto userPtr = my_room.userPtrWithId(my_userId);
         if (userPtr != nullptr)
            userPtr->setFace(faceId);

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kUserFaceKind);
         msg.setBodyLen(NetMsg::kUserFaceBodyLen);
         msg.setRef(my_userId); // 0?

         msg.appendU16(faceId);

         emit viewNeedsUpdatingEvent();

         return my_socket.write(msg);
      }

      auto Client::do_sendFaceColor(u16 faceColorId) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto userPtr = my_room.userPtrWithId(my_userId);
         if (userPtr != nullptr)
            userPtr->setColor(faceColorId);

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kUserColorKind);
         msg.setBodyLen(NetMsg::kUserColorBodyLen);
         msg.setRef(my_userId); // 0?

         msg.appendU16(faceColorId);

         emit viewNeedsUpdatingEvent();

         return my_socket.write(msg);
      }

      auto Client::do_sendPropRequest(void) -> int
      {
         return 0;
      }

      auto Client::do_deinitEvents(void) -> void
      {
         disconnect(&my_socket, &QTcpSocket::readyRead,
                    this, &seville::palace::Client::on_readyReadDidOccur);

         disconnect(&my_socket, QOverload<QAbstractSocket::SocketError>::of(
                    &QAbstractSocket::errorOccurred),
                    this, &Client::on_socketErrorDidOccur);

         disconnect(&my_networkAccessManager, &QNetworkAccessManager::finished,
                    this, &Client::on_backgroundDidFinishLoading);

         disconnect(&my_pingTimer, &QTimer::timeout,
                    this, &Client::on_pingTimerDidTrigger);
      }

      auto Client::do_deinit(void) -> void
      {
         do_deinitEvents();

         disconnectFromHost();
      }

      auto Client::do_initEvents(void) -> void
      {
         connect(&my_socket, &QTcpSocket::readyRead,
                 this, &seville::palace::Client::on_readyReadDidOccur);

         connect(&my_socket, QOverload<QAbstractSocket::SocketError>::of(
                    &QAbstractSocket::errorOccurred),
                 this, &Client::on_socketErrorDidOccur);

         connect(&my_networkAccessManager, &QNetworkAccessManager::finished,
                 this, &Client::on_backgroundDidFinishLoading);

         connect(&my_pingTimer, &QTimer::timeout,
                 this, &Client::on_pingTimerDidTrigger);
      }

      auto Client::do_init(void) -> void
      {
         do_initEvents();

         do_clear();

         //my_logger.setMode(LogMode::kDebugMode);
         //my_logger_.setIsDebugMode(true);

         // TODO use a linked list or deque to communicate between threads
         // or just use a big buffer, as below.
         //my_socket.setReadBufferSize(1e7);
      }
   }
}
