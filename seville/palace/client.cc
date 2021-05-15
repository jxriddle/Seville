﻿#include <QImage>
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
      int Client::do_receiveNetMsgFromSocket(void) //std::tuple<int, NetMsg>
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

      int Client::do_readNetMsgHeaderFromSocket(void)
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

      int Client::do_readNetMsgContentFromSocket(void) //(i32 len) -> int
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

      void Client::do_setupEvents(void)
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
#endif

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

         connect(&my_roomBackgroundNetworkAccessManager,
                 &QNetworkAccessManager::finished,
                 this, &seville::palace::Client::on_roomBackgroundDidLoad);

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
         connect(&my_pingTimer, &QTimer::timeout,
                 this, &seville::palace::Client::do_sendPing);
      }

      //, const QMap<QString, QString>& mapOfHeaders)
      void Client::do_fetchBackgroundAsync(const QString& url)
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

         my_roomBackgroundNetworkAccessManager.get(request);
      }

      void Client::do_fetchPropAsync(const QString& url)
      {
         QNetworkRequest request;
         request.setUrl(url);
         request.setRawHeader("User-Agent", "Seville 1.0");

         my_roomBackgroundNetworkAccessManager.get(request);
      }

      void Client::do_clear(void)
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
         my_netMsgTTLCount = 0;
      }

      void Client::do_resetReceiveTimer(void)
      {
//         if (my_transferTimerId)
//         {
//            killTimer(my_transferTimerId);
//            my_transferTimerId = 0;
//         }
      }

      void Client::on_backgroundDidFetchAsync(
            QNetworkReply* replyPtr)
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

      void Client::on_roomBackgroundDidLoad(QNetworkReply* replyPtr)
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


      void Client::on_pingTimerDidTrigger(void)
      {
         do_sendPing();
      }

      void Client::on_socketErrorDidOccur(
            QAbstractSocket::SocketError socketError)
      {
         if (socketError == QAbstractSocket::AddressInUseError) {
            my_logger.appendErrorMessage("Address in use");
         }
         else if (socketError == QAbstractSocket::ConnectionRefusedError) {
            my_logger.appendErrorMessage("Connection refused");
         }
         else if (socketError == QAbstractSocket::DatagramTooLargeError) {
            my_logger.appendErrorMessage("Datagram too large");
         }
         else if (socketError == QAbstractSocket::HostNotFoundError) {
            my_logger.appendErrorMessage("Host not found");
         }
         else if (socketError == QAbstractSocket::NetworkError) {
            my_logger.appendErrorMessage("Network error");
         }
         else if (socketError == QAbstractSocket::OperationError) {
            my_logger.appendErrorMessage("Operation error");
         }
         else if (socketError ==
                  QAbstractSocket::ProxyAuthenticationRequiredError) {
            my_logger.appendErrorMessage("Proxy authentication required");
         }
         else if (socketError == QAbstractSocket::ProxyConnectionClosedError) {
            my_logger.appendErrorMessage("Proxy connection closed");
         }
         else if (socketError == QAbstractSocket::ProxyConnectionRefusedError) {
            my_logger.appendErrorMessage("Proxy connection refused");
         }
         else if (socketError == QAbstractSocket::ProxyConnectionTimeoutError) {
            my_logger.appendErrorMessage("Proxy connection timeout");
         }
         else if (socketError == QAbstractSocket::ProxyNotFoundError) {
            my_logger.appendErrorMessage("Proxy not found");
         }
         else if (socketError == QAbstractSocket::ProxyProtocolError) {
            my_logger.appendErrorMessage("Proxy protocol error");
         }
         else if (socketError == QAbstractSocket::RemoteHostClosedError) {
            my_logger.appendErrorMessage("Remote host closed connection");
         }
         else if (socketError == QAbstractSocket::SocketAccessError) {
            my_logger.appendErrorMessage("Socket access error");
         }
         else if (socketError ==
                  QAbstractSocket::SocketAddressNotAvailableError) {
            my_logger.appendErrorMessage("Socket address not available");
         }
         else if (socketError == QAbstractSocket::SocketResourceError) {
            my_logger.appendErrorMessage("Socket resource error");
         }
         else if (socketError == QAbstractSocket::SocketTimeoutError) {
            my_logger.appendErrorMessage("Socket timeout error");
         }
         else if (socketError ==
                  QAbstractSocket::SocketError::SslHandshakeFailedError) {
            my_logger.appendErrorMessage("SSL handshake failed");
         }
         else if (socketError == QAbstractSocket::SslInternalError) {
            my_logger.appendErrorMessage("SSL internal error");
         }
         else if (socketError == QAbstractSocket::SslInvalidUserDataError) {
            my_logger.appendErrorMessage("SSL invalid user data");
         }
         else if (socketError == QAbstractSocket::TemporaryError) {
            my_logger.appendErrorMessage("Temporary error");
         }
         else if (socketError ==
                  QAbstractSocket::UnfinishedSocketOperationError) {
            my_logger.appendErrorMessage("Unfinished socket operation error");
         }
         else if (socketError == QAbstractSocket::UnknownSocketError) {
            my_logger.appendErrorMessage("Unknown socket error");
         }
         else if (socketError ==
                  QAbstractSocket::UnsupportedSocketOperationError) {
            my_logger.appendErrorMessage("Unsupported socket operation");
         }
         else {
            my_logger.appendErrorMessage("Unknown Socket Error");
         }

         //qCDebug(seville_log) << "ERROR: Socket Error";

         do_setConnectionState(ConnectionState::kDisconnectedState);
      }

      void Client::on_readyReadDidOccur(void)
      {
         //while (my_socket.bytesAvailable())
         do_readSocket();

//         my_logger.appendDebugMessage(
//                  QString("%1 bytes remaining to read from socket")
//                  .arg(my_socket.bytesAvailable()));
      }

      void Client::do_readSocket(void)
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
               (void)netMsgIsRouted;

               auto netMsgTTLReached = my_netMsgTTLCount < kNetMsgTTLMax;
               auto shouldClearNetMsg = netMsgIsValid; // && netMsgIsRouted;
               if (shouldClearNetMsg) {
                  my_netMsg.clear();
                  my_netMsgTTLCount = 0;
               } else if (netMsgTTLReached) {
                  disconnectFromHost();
               }

               //auto lengthActual = my_netMsg.length();
               //auto lengthExpected = my_netMsg.len() + NetMsg::kHeaderSize;
               //if (NetMsg::kHeaderSize <= lengthActual &&
               //    static_cast<i32>(lengthExpected) <= lengthActual)
            }
         }
      }

      void Client::do_setConnectionState(ConnectionState connectionState)
      {
         if (connectionState != ConnectionState::kConnectedState)
            my_pingTimer.stop();

         my_connectionState = connectionState;

         // emit connectionStateDidChangeEvent(connectionState);
      }

      void Client::do_roomChat(const QString& text)
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return;

         if (text.length() <= 0)
            return;

         if (text.toStdString() == "'who") {
            auto userListPtr = my_room.userListPtr();
            my_logger.appendInfoMessage(QString("Listing %1/%2 (%3/%4) users in room...")
                  .arg(my_room.userCount())
                  .arg(my_server.userListPtr()->size())
                  .arg(my_room.userListPtr()->size())
                  .arg(my_server.userListPtr()->size()));

            for (auto i = u32{0}; i < userListPtr->size(); i++) {
               auto user = my_room.userListPtr()->at(i);
               my_logger.appendInfoMessage(
                        QString("%1 (%2) is here")
                        .arg(user.username())
                        .arg(user.id()));
            }
         }
         else if (text.left(QString("'webserver").length()) ==
                  QString("'webserver")) {
            my_logger.appendInfoMessage(
                     QString("My webserver location is: %1")
                     .arg(my_server.httpServerLocation()));
         }
         else if (text.left(QString("'propserver").length()) ==
                  QString("'propserver")) {
            my_logger.appendInfoMessage(
                     QString("My propserver location is: %1")
                     .arg(my_server.httpPropLocation()));
         }
         else if (text.left(QString("'usercount").length()) ==
                  QString("'usercount")) {
            my_logger.appendInfoMessage(
                     QString("Server user count: %1")
                     .arg(my_server.userListPtr()->size()));
         }
         else if (text.left(QString("'roomusercount").length()) ==
                  QString("'roomusercount")) {
            my_logger.appendInfoMessage(
                     QString("Room user count: %1")
                     .arg(my_room.userListPtr()->size()));
         }
         else if (text.left(QString("'propids ").length()) ==
                  QString("'propids ")) {
            auto split = text.split(' ');
            if (1 < split.length()) {
               auto ok = true;
               auto userId = split.at(1).toUShort(&ok);
               auto user = my_room.userWithId(userId);
               for (auto i = u32{0}; i < User::kNumPropCells; i++) {
                  auto prop = user.propListPtr()->at(i);
                  my_logger.appendInfoMessage(
                           QString("User %1 propId[%2]: %3 (crc: %4)")
                           .arg(userId)
                           .arg(i)
                           .arg(prop.id())
                           .arg(prop.crc()));
               }
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
               auto roomId = split.at(1).toUShort(&ok);
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

      void Client::do_connectToHost(
            QString host, int port, QString username, int initialRoom)
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
         emit connectionStateDidChangeEvent(ConnectionState::kHandshakingState);

         my_socket.connectToHost(host, actualHostTcpPort);
         my_pingTimer.setInterval(60000);
         my_pingTimer.start();
      }

      void Client::do_disconnectFromHost(void)
      {
         if (my_connectionState != ConnectionState::kDisconnectedState) {
            //my_logger.appendInfoMessage("Disconnected.");
            my_socket.disconnectFromHost();
            my_pingTimer.stop();
            do_setConnectionState(ConnectionState::kDisconnectedState);
            do_clear();
         }
      }

      void Client::do_determineClientByteOrder(void)
      {
         auto isLittleEndian =
               (HostByteOrder::kLittleEndian * !Client::isBigEndian());
         auto isBigEndian =
               (HostByteOrder::kBigEndian * Client::isBigEndian());
         my_byteOrder =
               static_cast<HostByteOrder>(isLittleEndian | isBigEndian);
      }

      int Client::do_determineServerByteOrder(void)
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

      bool Client::do_determineIsConnected(void) const
      {
         return my_socket.state() != QTcpSocket::ConnectedState ||
               my_connectionState != ConnectionState::kConnectedState;
      }

      NetMsgOptions Client::do_determineShouldSwapEndianness(void) const
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
      int Client::do_receiveAltLogon(void)
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

      int Client::do_receiveAltRoomDescription(void)
      {
         my_logger.appendDebugMessage("> AltRoomDescription");

         // stub
         return 1;
      }

      int Client::do_receiveAssetIncoming(void)
      {
         my_logger.appendDebugMessage("> AssetIncoming");

         // stub
         return 1;
      }

      int Client::do_receiveAssetQuery(void)
      {
         my_logger.appendDebugMessage("> AssetQuery");

         // stub
         return 1;
      }

      int Client::do_receiveAuthenticate(void)
      {
         my_logger.appendDebugMessage("> Authenticate");

         // stub
         return 1;
      }

      int Client::do_receiveBlowthru(void)
      {
         my_logger.appendDebugMessage("> BlowThru");

         // stub
         return 1;
      }

      int Client::do_receiveConnectionError(void)
      {
         my_logger.appendDebugMessage("> ConnectionError");

         do_disconnectFromHost();
         my_logger.appendInfoMessage("Disconnected.");

         return 1;
      }

      int Client::do_receiveDoorLock(void)
      {
         my_logger.appendDebugMessage("> DoorLock");

         // stub
         return 1;
      }

      int Client::do_receiveDoorUnlock(void)
      {
         my_logger.appendDebugMessage("> DoorUnlock");

         // stub
         return 1;
      }

      int Client::do_receiveDraw(void)
      {
         my_logger.appendDebugMessage("> Draw");

         // stub
         return 1;
      }

      int Client::do_receiveHttpServerLocation(void)
      {
         my_logger.appendDebugMessage("> HttpServerLocation");

         auto result = 1;
         //my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         auto url = my_netMsg.streamReadQString(256);
         my_server.setHttpServerLocation(url);
         my_logger.appendDebugMessage(QString("HTTP Server is %1").arg(url));

         return result;
      }

      int Client::do_receiveMovement(void)
      {
         my_logger.appendDebugMessage("> Movement");

         auto userId = my_netMsg.ref();
         auto y = my_netMsg.streamReadI16();
         auto x = my_netMsg.streamReadI16();

         auto userPtr = my_room.userPtrWithId(userId);
         userPtr->setX(x);
         userPtr->setY(y);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveNavigationError(void)
      {
         my_logger.appendDebugMessage("> NavError!");

         // stub
         return 1;
      }

      int Client::do_receivePictureMove(void)
      {
         my_logger.appendDebugMessage("> PictMove");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      int Client::do_receivePing(void)
      {
         my_logger.appendDebugMessage("Ping!");

         //my_logger.appendDebugMessage("> Ping!");

         do_sendPong();
         return 1;
      }

      int Client::do_receivePong(void)
      {
         my_logger.appendDebugMessage("> Pong");

         // stub
         // reset ping timer?
         //my_logger.appendDebugMessage("> Pong!");

         return 1;
      }

      int Client::do_receivePropDelete(void)
      {
         my_logger.appendDebugMessage("> PropDelete");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      int Client::do_receivePropMove(void)
      {
         my_logger.appendDebugMessage("> PropMove");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      int Client::do_receivePropNew(void)
      {
         my_logger.appendDebugMessage("> PropNew");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      int Client::do_receiveRoomDescend(void)
      {
         my_logger.appendDebugMessage("> RoomDescend");

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveRoomDescription(void)
      {
         my_logger.appendDebugMessage("> RoomDescription");

         my_room.setFlags(my_netMsg.streamReadU32());
         my_room.setFace(my_netMsg.streamReadU32());
         my_room.setId(my_netMsg.streamReadU16());

         auto roomNameOffset = my_netMsg.streamReadI16();
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

      int Client::do_receiveRoomUserList(void)
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

      int Client::do_receiveServerVersion(void)
      {
         my_logger.appendDebugMessage("> ServerVersion");

         auto serverVersion = my_netMsg.u32At(NetMsg::kRefOffset);
         my_server.setVersion(serverVersion);
         my_logger.appendDebugMessage(
                  QString("Sever version %1").arg(serverVersion));
         return 1;
      }

      int Client::do_receiveServerInfo(void)
      {
         my_logger.appendDebugMessage("> ServerInfo");

         // my_server.setVersion();

         // TODO STUB

         return 1;
      }

      User Client::do_processUserNew(void)
      {
         my_logger.appendDebugMessage("> UserNew");

         auto user = User();

         user.setId(my_netMsg.streamReadU32());
         user.setY(my_netMsg.streamReadU16());
         user.setX(my_netMsg.streamReadU16());

         for (auto i = 0; i < User::kNumPropCells; i++) {
            auto prop = Prop();
            prop.setId(my_netMsg.streamReadI32());
            prop.setCrc(my_netMsg.streamReadI32());
            user.propListPtr()->push_back(prop);
         }

         user.setRoomId(my_netMsg.streamReadI16());
         user.setFace(my_netMsg.streamReadU16());
         user.setColor(my_netMsg.streamReadU16());

         auto unknown1 = my_netMsg.streamReadI16();
         auto unknown2 = my_netMsg.streamReadI16();
         (void)unknown1;
         (void)unknown2;

         user.setPropNum(my_netMsg.streamReadI16());
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

      int Client::do_receiveUserNew(void)
      {
         auto user = do_processUserNew();

         my_logger.appendInfoMessage(
                  QString("%1 has entered the room").arg(user.username()));

         if (user.id() != my_userId)
            my_room.userListPtr()->push_back(user);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveUserColor(void)
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

      int Client::do_receiveUserExitRoom(void)
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

      int Client::do_receiveUserFace(void)
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

      int Client::do_receiveUserProp(void)
      {
         my_logger.appendDebugMessage("> UserProp");

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveUserDescription(void)
      {
         my_logger.appendDebugMessage("> UserDescription");

         // stub
         return 1;
      }

      int Client::do_receiveUserRename(void)
      {
         my_logger.appendDebugMessage("> UserRename");

         auto userId = my_netMsg.ref();
         auto userPtr = my_room.userPtrWithId(userId);
         if (userPtr == nullptr)
            return 0;

         auto oldUsername = userPtr->username();

         auto usernameLen = my_netMsg.streamReadU8();
         auto username = my_netMsg.streamReadFixedQString(usernameLen);
         my_logger.appendInfoMessage(QString("%1 has changed name to %2")
                                     .arg(oldUsername)
                                     .arg(username));

         userPtr->setUsername(username);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveUserLeaving(void)
      {
         my_logger.appendDebugMessage("> UserLeaving");

         auto userId = my_netMsg.ref();
         auto user = my_server.userWithId(userId);

         my_logger.appendInfoMessage(
                  QString("%1 has signed off.").arg(user.username()));

         user = my_server.userWithId(userId);
         my_server.removeUserWithId(userId);

         auto userCount = my_netMsg.streamReadI32();
         my_room.setUserCount(userCount);

         if (user.id() != my_userId)
            my_room.removeUserWithId(userId);

         emit viewNeedsUpdatingEvent();

         return 1;
      }

      int Client::do_receiveUserLoggedOnAndMax(void)
      {
         my_logger.appendDebugMessage("> UserLoggedOnAndMax");

         emit viewNeedsUpdatingEvent();

         // stub
         return 1;
      }

      int Client::do_receiveUserStatus(void)
      {
         my_logger.appendDebugMessage("> UserStatus");

         // stub
         return 1;
      }

      int Client::do_receiveRoomList(void)
      {
         my_logger.appendDebugMessage("> RoomList");

         auto roomListPtr = my_server.roomListPtr();
         roomListPtr->clear();

         auto roomCount = my_netMsg.ref();
         //if()

         for (auto i = u32{0}; i < roomCount; i++) {
            auto room = Room();
            room.setId(static_cast<u16>(my_netMsg.streamReadU32()));
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

      int Client::do_receiveUserList(void)
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
            user.setRoomId(my_netMsg.streamReadI16());
            auto usernameLen = my_netMsg.streamReadU8();
            //user.setUsername(my_netMsg.streamReadFixedQString(usernameLen));
            user.setUsername(my_netMsg.streamReadAndDecodeQString(usernameLen));
            auto usernamePaddedLen = (4 - (usernameLen & 3)) - 1; // + usernameLen;
            my_netMsg.streamSkip(usernamePaddedLen);

            userListPtr->push_back(user);
         }

         return 1;
      }

      int Client::do_receiveSpotMove(void)
      {
         my_logger.appendDebugMessage("> SpotMove");

         // stub
         return 1;
      }

      int Client::do_receiveSpotState(void)
      {
         my_logger.appendDebugMessage("> SpotState");

         // stub
         return 1;
      }

      int Client::do_receiveTalk(void)
      {
         my_logger.appendDebugMessage("> Talk");

         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);
         //auto message = my_netMsg.streamReadByteArray(my_netMsg.len());
         auto bodyLen = static_cast<i32>(my_netMsg.bodyLen());
         auto message =
               my_netMsg.streamReadAndDecodeQString(bodyLen);

         my_logger.appendChatMessage(user.username(), message);

         return 1;
      }

      int Client::do_receiveWhisper(void)
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
         auto bodyLen = static_cast<i32>(my_netMsg.bodyLen());
         auto message = my_netMsg.streamReadAndDecodeQString(bodyLen);

         auto username = user.username();
         QString usernameFrom = QString("%1 (%2)").arg(username).arg(userId);
         //auto id = user.id();
         my_logger.appendWhisperMessage(
//                  QString("%1 (%2)")
//                  .arg(username)
//                  .arg(id),
         usernameFrom,
         message);

         return 1;
      }

      int Client::do_receiveXTalk(void)
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

      int Client::do_receiveXWhisper(void)
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

      int Client::do_routeReceivedNetMsg(void)
      {
         auto result = 0;
         auto netMsgIdTmp = my_netMsg.id(); //u32At(NetMsg::kIdOffset);
         auto serverByteOrder = my_server.byteOrder();
         auto byteOrderMatch = (serverByteOrder == my_byteOrder);
         auto kind = (byteOrderMatch * netMsgIdTmp) |
               (!byteOrderMatch * NetMsg::SwapU32(netMsgIdTmp));

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

      int Client::do_sendLogon(void)
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
         auto bytesWritten = my_socket.write(logonMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendAuthenticate(
            const QString& username, const QString& password)
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

         auto bytesWritten = my_socket.write(authMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendTalk(const QString& text)
      {
         auto talkMsg = NetMsg(do_determineShouldSwapEndianness());

         talkMsg.setId(NetMsg::kTalkKind);
         talkMsg.setBodyLen(static_cast<u32>(text.length()));
         talkMsg.setRef(my_userId);

         talkMsg.append(text.toUtf8());
         talkMsg.append('\0');

         auto bytesWritten = my_socket.write(talkMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendXTalk(const QString& text)
      {
         auto xTalkMsg = NetMsg(do_determineShouldSwapEndianness());

         xTalkMsg.setId(NetMsg::kXTalkKind);
         auto len = static_cast<u16>(text.length() + 3);
         xTalkMsg.setBodyLen(len);
         xTalkMsg.setRef(my_userId);

         auto plaintext = text.toUtf8();
         auto ciphertext = my_cipher.encipher(plaintext);
         xTalkMsg.appendU16(len);
         xTalkMsg.append(ciphertext);
         xTalkMsg.append('\0');

         auto bytesWritten = my_socket.write(xTalkMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendXWhisper(const u32 userId, const QString& text)
      {
         auto msg = NetMsg(do_determineShouldSwapEndianness());

         msg.setId(NetMsg::kXWhisperKind);
         auto textLen = static_cast<u16>(text.length() + 3);
         auto len = static_cast<u16>(NetMsg::kXWhisperBodyLen + textLen);

         msg.setBodyLen(len);
         msg.setRef(my_userId);

         auto plaintext = text.toUtf8();
         auto ciphertext = my_cipher.encipher(plaintext);
         msg.appendU32(userId);
         msg.appendU16(textLen);
         msg.append(ciphertext);
         msg.append('\0');

         auto bytesWritten = my_socket.write(msg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendPing(void)
      {
         auto pingMsg = palace::NetMsg(do_determineShouldSwapEndianness());

         pingMsg.setId(NetMsg::kPingKind);
         pingMsg.setBodyLen(0);
         pingMsg.setRef(NetMsg::kNoRef);

         my_logger.appendDebugMessage(QString("Ping!"));
         auto bytesWritten = my_socket.write(pingMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendPong(void)
      {
         auto pongMsg = palace::NetMsg(do_determineShouldSwapEndianness());

         pongMsg.setId(NetMsg::kPongKind);
         pongMsg.setBodyLen(0);
         pongMsg.setRef(NetMsg::kNoRef);

         my_logger.appendDebugMessage(QString("Pong!"));
         auto bytesWritten = my_socket.write(pongMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendGotoRoom(u16 roomId)
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         my_room.userListPtr()->clear();
         my_room.setId(roomId);

         auto gotoroomMsg = palace::NetMsg(do_determineShouldSwapEndianness());
         gotoroomMsg.setId(NetMsg::kGotoRoomKind);
         gotoroomMsg.setBodyLen(NetMsg::kGotoRoomLen);
         gotoroomMsg.setRef(NetMsg::kNoRef);

         gotoroomMsg.appendU16(roomId);

         auto bytesWritten = my_socket.write(gotoroomMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendMove(i16 x, i16 y)
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
         auto bytesWritten = my_socket.write(moveMsg);
         return 0 < bytesWritten;
         //return my_socket.flush();
      }

      int Client::do_sendRequestRoomList(void)
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kServerRoomListKind);
         msg.setBodyLen(NetMsg::kNoLen);
         msg.setRef(NetMsg::kNoRef);

         auto bytesWritten = my_socket.write(msg);
         return 0 < bytesWritten;
      }

      int Client::do_sendRequestUserList(void)
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto msg = palace::NetMsg();
         msg.setId(NetMsg::kServerUserListKind);
         msg.setBodyLen(NetMsg::kNoLen);
         msg.setRef(NetMsg::kNoRef);

         auto bytesWritten = my_socket.write(msg);
         return 0 < bytesWritten;
      }

      i32 Client::do_sendRequestRoomAndUserLists(void)
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

         auto writeByteCount = my_socket.write(serverRoomListMsg);
         writeByteCount &= my_socket.write(serverUserListMsg);

         return 0 < writeByteCount;
      }

      i32 Client::do_sendFace(u16 faceId)
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

         auto writeByteCount = my_socket.write(msg);
         return 0 < writeByteCount;
      }

      i32 Client::do_sendFaceColor(u16 faceColorId)
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

         auto writeByteCount = my_socket.write(msg);
         return 0 < writeByteCount;
      }

      int Client::do_sendPropRequest(void)
      {
         return 0;
      }

      void Client::do_deinitEvents(void)
      {
         disconnect(&my_socket, &QTcpSocket::readyRead,
                    this, &seville::palace::Client::on_readyReadDidOccur);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
         disconnect(
            &my_socket,QOverload<QAbstractSocket::SocketError>
                  ::of(&QAbstractSocket::error),
            this, &Client::on_socketErrorDidOccur);
#else
         disconnect(&my_socket, QOverload<QAbstractSocket::SocketError>::of(
                    &QAbstractSocket::errorOccurred),
                    this, &Client::on_socketErrorDidOccur);
#endif

         disconnect(&my_roomBackgroundNetworkAccessManager,
                    &QNetworkAccessManager::finished,
                    this, &Client::on_backgroundDidFetchAsync);

         // disconnect(&my_propNetworkAccessManager,
         //            &QNetworkAccessManager::finished,
         //            this, &Client::on_propDidFetchAsync);

         disconnect(&my_pingTimer, &QTimer::timeout,
                    this, &Client::on_pingTimerDidTrigger);
      }

      void Client::do_deinit(void)
      {
         do_deinitEvents();

         do_disconnectFromHost();
      }

      void Client::do_initEvents(void)
      {
         connect(&my_socket, &QTcpSocket::readyRead,
                 this, &seville::palace::Client::on_readyReadDidOccur);

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
         connect(
            &my_socket,QOverload<QAbstractSocket::SocketError>
                  ::of(&QAbstractSocket::error),
            this, &Client::on_socketErrorDidOccur);
#else
         connect(&my_socket, QOverload<QAbstractSocket::SocketError>::of(
                    &QAbstractSocket::errorOccurred),
                 this, &Client::on_socketErrorDidOccur);
#endif

         connect(&my_roomBackgroundNetworkAccessManager, &QNetworkAccessManager::finished,
                 this, &Client::on_backgroundDidFetchAsync);

         connect(&my_pingTimer, &QTimer::timeout,
                 this, &Client::on_pingTimerDidTrigger);
      }

      void Client::do_init(void)
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
