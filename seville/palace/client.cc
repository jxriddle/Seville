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

      auto Client::do_receiveNetMsgFromSocket(void) -> int
      {
         auto readHeaderOk = 0;
         auto readContentOk = 0;
         if (my_netMsg.size() < NetMsg::kHeaderSize)
            readHeaderOk = do_readNetMsgHeaderFromSocket();
         if (NetMsg::kHeaderSize <= my_netMsg.size())
             readContentOk = do_readNetMsgContentFromSocket();
         return readHeaderOk && readContentOk;
      }

      int Client::do_readNetMsgHeaderFromSocket() { //_from_socket_ptr_(
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
            my_netMsg.reserve(NetMsg::kHeaderSize + my_netMsg.len());

         return NetMsg::kHeaderSize == netMsgHeaderSize;
      }

      int Client::do_readNetMsgContentFromSocket(void) {
            //QTcpSocket* socket_ptr) -> int {
         auto contentSize_0 = my_netMsg.contentSize();
         //if (size_0 < NetMsg::kHeaderSize)
         //   return 0;

         //auto body_size = size_0 - NetMsg::kHeaderSize;

         auto nBytesToCompleteContent = i32(
                  my_netMsg.len() - contentSize_0);

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

         auto nExpectedTotalBytesInBody = my_netMsg.len();

         return nExpectedTotalBytesInBody == my_netMsg.contentSize();
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

         connect(
            &my_socket, QOverload<QAbstractSocket::SocketError>::of(
                     &QAbstractSocket::errorOccurred),
                  this, &Client::on_socketErrorDidOccur);
                  //this, [this](QAbstractSocket::SocketError error) {  //&Client::on_socketError
               //QAbstractSocket::SocketError::

//            connect(
//                  &my_socket_,
//                  &QTcpSocket::errorOccurred,
//                  this,
//                     [this](QAbstractSocket::SocketError socket_error) {
//               my_logger_.error("Socket error");
//               qCDebug(log_seville) << "ERROR: Socket Error";
//            });

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
         my_user.clear();
         my_room.clear();
      }

      auto Client::do_resetReceiveTimer(void) -> void
      {
//         if (my_transferTimerId)
//         {
//            killTimer(my_transferTimerId);
//            my_transferTimerId = 0;
//         }
      }

      auto Client::on_socketErrorDidOccur(QAbstractSocket::SocketError error)
         -> void
      {
         if (error == QAbstractSocket::SocketError::AddressInUseError) {
            my_logger.appendErrorMessage("Address in use");
         }
         else if (error == QAbstractSocket::SocketError::ConnectionRefusedError) {
            my_logger.appendErrorMessage("Connection refused");
         }
         else if (error == QAbstractSocket::SocketError::DatagramTooLargeError) {
            my_logger.appendErrorMessage("Datagram too large");
         }
         else if (error == QAbstractSocket::SocketError::HostNotFoundError) {
            my_logger.appendErrorMessage("Host not found");
         }
         else if (error == QAbstractSocket::SocketError::NetworkError) {
            my_logger.appendErrorMessage("Network error");
         }
         else if (error == QAbstractSocket::SocketError::OperationError) {
            my_logger.appendErrorMessage("Operation error");
         }
         else if (error == QAbstractSocket::SocketError::ProxyAuthenticationRequiredError) {
            my_logger.appendErrorMessage("Proxy authentication required");
         }
         else if (error == QAbstractSocket::SocketError::ProxyConnectionClosedError) {
            my_logger.appendErrorMessage("Proxy connection closed");
         }
         else if (error == QAbstractSocket::SocketError::ProxyConnectionRefusedError) {
            my_logger.appendErrorMessage("Proxy connection refused");
         }
         else if (error == QAbstractSocket::SocketError::ProxyConnectionTimeoutError) {
            my_logger.appendErrorMessage("Proxy connection timeout");
         }
         else if (error == QAbstractSocket::SocketError::ProxyNotFoundError) {
            my_logger.appendErrorMessage("Proxy not found");
         }
         else if (error == QAbstractSocket::SocketError::ProxyProtocolError) {
            my_logger.appendErrorMessage("Proxy protocol error");
         }
         else if (error == QAbstractSocket::SocketError::RemoteHostClosedError) {
            my_logger.appendErrorMessage("Remote host closed connection");
         }
         else if (error == QAbstractSocket::SocketError::SocketAccessError) {
            my_logger.appendErrorMessage("Socket access error");
         }
         else if (error == QAbstractSocket::SocketError::SocketAddressNotAvailableError) {
            my_logger.appendErrorMessage("Socket address not available");
         }
         else if (error == QAbstractSocket::SocketError::SocketResourceError) {
            my_logger.appendErrorMessage("Socket resource error");
         }
         else if (error == QAbstractSocket::SocketTimeoutError) {
            my_logger.appendErrorMessage("Socket timeout error");
         }
         else if (error == QAbstractSocket::SocketError::SslHandshakeFailedError) {
            my_logger.appendErrorMessage("SSL handshake failed");
         }
         else if (error == QAbstractSocket::SocketError::SslInternalError) {
            my_logger.appendErrorMessage("SSL internal error");
         }
         else if (error == QAbstractSocket::SocketError::SslInvalidUserDataError) {
            my_logger.appendErrorMessage("SSL invalid user data");
         }
         else if (error == QAbstractSocket::SocketError::TemporaryError) {
            my_logger.appendErrorMessage("Temporary error");
         }
         else if (error == QAbstractSocket::SocketError::UnfinishedSocketOperationError) {
            my_logger.appendErrorMessage("Unfinished socket operation error");
         }
         else if (error == QAbstractSocket::SocketError::UnknownSocketError) {
            my_logger.appendErrorMessage("Unknown socket error");
         }
         else if (error == QAbstractSocket::SocketError::UnsupportedSocketOperationError) {
            my_logger.appendErrorMessage("Unsupported socket operation");
         }
         else {
            my_logger.appendErrorMessage("Unknown Socket Error");
         }

         qCDebug(seville_log) << "ERROR: Socket Error";

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

         while (do_receiveNetMsgFromSocket())
         {
//            if (my_netMsgBody.size() < NetMsg::kHeaderSize)
//            {
//               my_transferTimerId = startTimer(kIntervalTimeoutForTransferInMs);
//               continue;
//            }
            //my_netMsgSize = my_netMsgBody.size();

            auto netMsgId = my_netMsg.id();
            //auto netMsgLen = my_netMsgHeader.i32At(NetMsg::kLenOffset);
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
                  qCDebug(seville_log) <<
                        "ERROR: Could not determine server byte order!";
                  return;
               }

               //QDataStream in(my_netMsgBody);
               //u32 user_id; //my_netmsg_in_.Ref();
               //in.skipRawData(NetMsg::kRefOffset);
               //in >> user_id;
               //my_user.setId(user_id);
               //auto userId = my_netMsgBody.ref();
               my_user.setId(netMsgRef);

               if (do_sendLogon() < 0)
               {
                  qCDebug(seville_log) << "ERROR: Could not send Logon!";
                  my_logger.appendErrorMessage("Error sending logon");
                  return;
               }

               do_setConnectionState(ConnectionState::kConnectedState);
               my_logger.appendInfoMessage(
                        QString("Connected to %1").arg(my_server.hostname()));
               qCDebug(seville_log) << "Connected to " << my_server.hostname();
            }
            else if (my_connectionState == ConnectionState::kConnectedState &&
                     NetMsg::kHeaderSize <= my_netMsg.size() &&
                     netMsgId == NetMsg::kConnectionErrorKind)
            {
               do_setConnectionState(ConnectionState::kDisconnectedState);
               qCDebug(seville_log) << "Disconnected by server";
               my_logger.appendErrorMessage("Disconnected by server");
            }
            else if (my_connectionState == ConnectionState::kConnectedState)
            {
               do_routeReceivedNetMsg();
            }

            //qCDebug(log_seville) << "Resetting NetMsg";
            my_netMsg.clear();
         }

         //my_pingTimer.start();
         //my_pingTimer.setInterval(120000);
         //my_pingTimer.setInterval(60000);
         //my_pongTime.currentTime();
      }

      auto Client::do_setConnectionState(ConnectionState connectionState) -> void
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

         // TODO is this a client command? interpret it here.

         // TODO if not, send as a chat message.
         do_sendXTalk(text);

         // add the message to the log if it doesn't make a round trip.

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

         qCDebug(seville_log, "Connecting to palace://%s@%s:%d/%d",
                 username.toUtf8().data(),
                 host.toUtf8().data(),
                 actualHostTcpPort, actualInitialRoom);

         my_logger.appendInfoMessage(logMsg);

         my_user.setUsername(username);
         my_server.setHost(host);
         my_server.setPort(actualHostTcpPort);
         my_room.setId(actualInitialRoom);

         qCDebug(seville_log) << "Handshaking";
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
//         auto serverByteOrderIfServerEndianMatch =
//               clientIsBigEndian * HostByteOrder::kBigEndian |
//               !clientIsBigEndian * HostByteOrder::kLittleEndian;
         auto serverByteOrderIfServerEndianMismatch =
               !clientIsBigEndian * HostByteOrder::kBigEndian |
               clientIsBigEndian * HostByteOrder::kLittleEndian;

         if (kind == NetMsg::kUnknownServerKind) {
            qCDebug(seville_log) << "Server has Unknown Byte Order";
            my_server.setByteOrder(HostByteOrder::kUnknownEndian);
            result = 0;
         }
         else if (kind == NetMsg::kServerEndianMatchKind) {
            qCDebug(seville_log) << "Server endian matches client";
            my_server.setByteOrder(my_byteOrder);
            result = 1;
         }
         else if (kind == NetMsg::kServerEndianMismatchKind) {
            qCDebug(seville_log) << "Server endian does not match client";
            my_server.setByteOrder(serverByteOrderIfServerEndianMismatch);
            result = 1;
         }
         else {
            qCDebug(seville_log) << "Server did not indicate its Byte Order";
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

      auto Client::do_determineIsConnected(void) const -> bool {
         return my_socket.state() != QTcpSocket::ConnectedState ||
               my_connectionState != ConnectionState::kConnectedState;
      }

      auto Client::do_determineShouldSwapEndianness(void) const ->
         NetMsgOptions
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
         //auto netmsg_logon = static_cast<netmsg::NetMsgLogon>(my_netmsg_);
         //QDataStream in(my_netMsgBody);
         //in.skipRawData(76);
         //my_netmsg_in_.u32At(NetMsg::kPayloadOffset+80);
         my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         auto puidCounter = my_netMsg.streamReadU32();
                  //NetMsg::kLogonPuidCounterOffset);
         //in >> puid_counter;
         //my_netmsg_in_.u32At(NetMsg::kPayloadOffset+80);
         auto puidCrc = my_netMsg.streamReadU32();
                  //NetMsg::kLogonPuidCrcOffset);
         //in >> puid_crc;

         if (my_user.idCounter() != puidCounter ||
             my_user.idCrc() != puidCrc)
         {
            my_user.setIdCrc(puidCrc);
            my_user.setIdCounter(puidCounter);
            my_user.setIdChanged(true);
            my_logger.appendDebugMessage("My palace user id changed by server");
         }
         else {
            my_logger.appendDebugMessage("My palace user id unchanged");
         }

         return 0;
      }

      auto Client::do_receiveAltRoomDescription(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveAssetIncoming(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveAssetQuery(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveAuthenticate(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveBlowthru(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveConnectionError(void) -> int
      {
         do_disconnectFromHost();
         my_logger.appendInfoMessage("Disconnected.");

         auto result = 0;
         return result;
      }

      auto Client::do_receiveDoorLock(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveDoorUnlock(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveDraw(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveHttpServerLocation(void) -> int
      {
         auto result = 0;
         my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         auto url = my_netMsg.streamReadQString(256);
         my_server.setHttpServerLocation(url);
         my_logger.appendDebugMessage(QString("HTTP Server is %1").arg(url));

         return result;
      }

      auto Client::do_receiveMovement(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveNavigationError(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receivePictureMove(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receivePing(void) -> int
      {
         //my_logger.appendDebugMessage("Received Ping");

         do_sendPong();
         return 0;
      }

      auto Client::do_receivePong(void) -> int
      {
         // stub
         // reset ping timer?
         //my_logger.appendDebugMessage("Received Pong");

         auto result = 0;
         return result;
      }

      auto Client::do_receivePropDelete(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receivePropMove(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receivePropNew(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveRoomDescend(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveRoomDescription(void) -> int
      {
         my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

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

         return 0;
      }

      auto Client::do_receiveRoomUserList(void) -> int
      {
         auto result = 0;

         my_room.userListPtr()->clear();

         auto roomUserCount = i32(my_netMsg.ref());
         for (auto i = 0; i < roomUserCount; i++) {
            auto user = User();
            // streamRead UserNew...
            do_receiveUserNew();

            my_room.userListPtr()->push_back(user);
         }

         return result;
      }

      auto Client::do_receiveServerVersion(void) -> int
      {
         auto serverVersion = my_netMsg.u32At(NetMsg::kRefOffset);
         my_server.setVersion(serverVersion);
         my_logger.appendDebugMessage(
                  QString("Sever version %1").arg(serverVersion));
         return 0;
      }

      auto Client::do_receiveServerInfo(void) -> int
      {
         auto result = 0;
         // stub

         return result;
      }

      auto Client::do_receiveUserNew(void) -> int
      {
         auto user = User();

         my_netMsg.setStreamCursorPosition(NetMsg::kHeaderSize);

         user.setId(my_netMsg.streamReadI32());
         user.setY(my_netMsg.streamReadI16());
         user.setX(my_netMsg.streamReadI16());
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
         if (user.propNum() < User::kNumPropCells) {
            auto propPtr = &user.propListPtr()->at(user.propNum());
            propPtr->setId(0);
            propPtr->setCrc(0);
            // TODO ...
         }

         user.setUsername(my_netMsg.streamReadPascalQString());

         my_room.userListPtr()->push_back(user);

         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserColor(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserExitRoom(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserFace(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserProp(void) -> int
      {
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserDescription(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserRename(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserLeaving(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserLoggedOnAndMax(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveUserStatus(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveServerRoomList(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveServerUserList(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveSpotMove(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveSpotState(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveTalk(void) -> int
      {
         auto userId = my_netMsg.ref();
         auto user = my_room.userWithId(userId);
         auto message = my_netMsg.streamReadPascalQString();

         my_logger.appendChatMessage(user.username(), message);

         return 0;
      }

      auto Client::do_receiveWhisper(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveXtalk(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receiveXwhisper(void) -> int
      {
         // stub
         auto result = 0;
         return result;
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

         my_netMsg.resetStreamCursorPosition();

         if (NetMsg::kAltLogonKind == kind) {
             //netmsg::sizes[netmsg::AltLogonKind] == size) {
            auto message = QString("Received AltLogon"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kLogonSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveAltLogon();
         } else if (NetMsg::kConnectionErrorKind == kind) {
            auto message = QString("Received ConnectionError"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kConnectionErrorSize != size)
               //my_logger_.debug("(but size did not match!)");
            //auto connectionError =
            //dynamic_cast<const netmsg::ConnectionError&>(myNetMsg);
            result = do_receiveConnectionError();
         } else if (NetMsg::kServerVersionKind == kind) {
            auto message = QString("Received ServerVersion"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kServerVersionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveServerVersion();
         } else if (NetMsg::kServerInfoKind == kind) {
            auto message = QString("Received ServerInfo"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kServerInfoSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveServerInfo();
         } else if (NetMsg::kUserStatusKind == kind) {
            auto message = QString("Received UserStatus"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserStatusSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserStatus();
         } else if (NetMsg::kUserLoggedOnAndMaxKind == kind) {
            auto message =
                  QString("Received UserLoggedOnAndMax"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserLoggedOnAndMaxSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserLoggedOnAndMax();
         } else if (NetMsg::kHttpServerLocationKind == kind) {
            auto message =
                  QString("Received HttpServerLocation"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kHttpServerLocationSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveHttpServerLocation();
         } else if (NetMsg::kRoomUserListKind == kind) {
            auto message = QString("Received RoomUserList"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kRoomUserListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveRoomUserList();
         } else if (NetMsg::kServerUserListKind == kind) {
            auto message = QString("Received ServerUserList"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kServerUserListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveServerUserList();
         } else if (NetMsg::kServerRoomListKind == kind) {
            auto message = QString("Received ServerRoomList"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kServerRoomListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveServerRoomList();
         } else if (NetMsg::kRoomDescendKind == kind) {
            auto message = QString("Received RoomDescend"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kRoomDescendedSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveRoomDescend();
         } else if (NetMsg::kUserNewKind == kind) {
            auto message = QString("Received UserNew"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserNewSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserNew();
         } else if (NetMsg::kPingKind == kind) {
            auto message = QString("Received Ping"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePing();
         } else if (NetMsg::kPongKind == kind) {
            auto message = QString("Received Pong"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPongSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePong();
         } else if (NetMsg::kXTalkKind == kind) {
            auto message = QString("Received XTalk"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kXTalkSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveXtalk();
         } else if (NetMsg::kXWhisperKind == kind) {
            auto message = QString("Received XWhisper"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kXWhisperSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveXwhisper();
         } else if (NetMsg::kTalkKind == kind) {
            auto message = QString("Received Talk"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kTalkSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveTalk();
         } else if (NetMsg::kWhisperKind == kind) {
            auto message = QString("Received Whisper"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (size < NetMsgSize::kWhisperSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveWhisper();
         } else if (NetMsg::kAssetIncomingKind == kind) {
            auto message = QString("Received AssetIncoming"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kAssetIncomingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveAssetIncoming();
         } else if (NetMsg::kAssetQueryKind == kind) {
            auto message = QString("Received AssetQuery"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kAssetQuerySize != size)
               //my_logger.appendDebugMessage("(but size did not match!");
            result = do_receiveAssetQuery();
         } else if (NetMsg::kMovementKind == kind) {
            auto message = QString("Received Movement"); //.arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kMovementSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveMovement();
         } else if (NetMsg::kUserColorKind == kind) {
            auto message = QString("Received UserColor"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserColorSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserColor();
         } else if (NetMsg::kUserFaceKind == kind) {
            auto message = QString("Received UserFace"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserFaceSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserFace();
         } else if (NetMsg::kUserPropKind == kind) {
            auto message = QString("Received UserProp"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserPropSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserProp();
         } else if (NetMsg::kUserDescriptionKind == kind) {
            auto message = QString("Received UserDescription"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserDescription();
         } else if (NetMsg::kUserRenameKind == kind) {
            auto message = QString("Received UserRename"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserRenameSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserRename();
         } else if (NetMsg::kUserLeavingKind == kind) {
            auto message = QString("Received UserLeaving"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserLeavingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserLeaving();
         } else if (NetMsg::kUserExitRoomKind == kind) {
            auto message = QString("Received UserExitRoom"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kUserExitRoomSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveUserExitRoom();
         } else if (NetMsg::kPropMoveKind == kind) {
            auto message = QString("Received PropMove"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPropMoveSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePropMove();
         } else if (NetMsg::kPropDeleteKind == kind) {
            auto message = QString("Received PropDelete"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPropDeleteSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePropDelete();
         } else if (NetMsg::kPropNewKind == kind) {
            auto message = QString("Received PropNew"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPropNewSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePropNew();
         } else if (NetMsg::kDoorLockKind == kind) {
            auto message = QString("Received DoorLock"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kDoorLockSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveDoorLock();
         } else if (NetMsg::kDoorUnlockKind == kind) {
            auto message = QString("Received DoorUnlock"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kDoorUnlockSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveDoorUnlock();
         } else if (NetMsg::kPictMoveKind == kind) {
            auto message = QString("Received PictMove"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kPictMoveSize == size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receivePictureMove();
         } else if (NetMsg::kSpotStateKind == kind) {
            auto message = QString("Received SpotState"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kSpotStateSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveSpotState();
         } else if (NetMsg::kSpotMoveKind == kind) {
            auto message = QString("Received SpotMove"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kSpotMoveSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveSpotMove();
         } else if (NetMsg::kDrawKind == kind) {
            auto message = QString("Received Draw"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kDrawSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveDraw();
         } else if (NetMsg::kNavErrorKind == kind) {
            auto message = QString("Received NavError"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kNavErrorSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveNavigationError();
         } else if (NetMsg::kBlowThruKind == kind) {
            auto message = QString("Received BlowThru"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kBlowThruSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveBlowthru();
         } else if (NetMsg::kAuthenticateKind == kind) {
            auto message = QString("Received Authenticate"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kAuthenticateSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveAuthenticate();
         } else if (NetMsg::kAltRoomDescriptionKind == kind) {
            auto message = QString("Received AltRoomDescription"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kAltRoomDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveRoomDescription();
         } else if (NetMsg::kRoomDescriptionKind == kind) {
            auto message = QString("Received RoomDescription"); // (%1)").arg(size);
            qCDebug(seville_log) << message;
            my_logger.appendDebugMessage(message);
            //if (NetMsgSize::kRoomDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receiveRoomDescription();
         } else {
            auto message =
                  QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }")
                  .arg(my_netMsg.id(), 2, 16, QChar('0'))
                  .arg(my_netMsg.len(), 2, 16, QChar('0'))
                  .arg(my_netMsg.ref(), 2, 16, QChar('0'));

            my_logger.appendDebugMessage(message);
            qCDebug(seville_log) << "Received unknown NetMsg" << message;
//            qCDebug(log_seville)
//                  << "kind:  "
//                  << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
//            qCDebug(log_seville)
//                  << "expected size: "
//                  << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
//            qCDebug(log_seville)
//                  << "reference code: "
//                  << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
            result = 0;
         }

         return result;
      }

      auto Client::do_sendLogon(void) -> int
      {
         auto result = 0;
         auto logonMsg = NetMsg(do_determineShouldSwapEndianness());

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
         logonMsg.setLen(NetMsg::kLogonSize);
         logonMsg.setRef(NetMsg::kNoRef);

         logonMsg.appendU32(my_user.regCrc());
         logonMsg.appendU32(my_user.regCounter());
         logonMsg.appendFixedPascalQString(
                  my_user.username(), NetMsg::kLogonMaximumUsernameSize);
         logonMsg.appendFixedPascalQString(
                  my_user.wizPass(), NetMsg::kLogonWizardPasswordSize);
         logonMsg.appendU32(my_user.idCounter());
         logonMsg.appendU32(my_user.idCrc());
         logonMsg.appendU32(
                  FlagAuxOptions::kAuthenticate | FlagAuxOptions::kWin32);
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
         my_socket.write(logonMsg);
         result = my_socket.flush();

         return result;
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

         auto authMsg = NetMsg();
         authMsg.setId(NetMsg::kAuthenticateKind);
         authMsg.setLen(0);
         authMsg.setRef(NetMsg::kNoRef);

         my_socket.write(authMsg);
         my_socket.flush();

         return result;
      }

      auto Client::do_sendTalk(const QString& text) -> int
      {
         auto talkMsg = NetMsg();

         talkMsg.setId(NetMsg::kTalkKind);
         // TODO (do_maybeSwapByteOrder(NetMsg::kTalkKind));
         talkMsg.setLen(text.length());
         talkMsg.setRef(my_user.id());

         talkMsg.appendPascalQString(text);

         my_socket.write(talkMsg);
         return my_socket.flush();
      }

      auto Client::do_sendXTalk(const QString& text) -> int
      {
         auto xTalkMsg = NetMsg();

         xTalkMsg.setId(NetMsg::kXTalkKind);
         // TODO (do_maybeSwapByteOrder(NetMsg::kTalkKind));
         xTalkMsg.setLen(text.length() + 3);
         //xTalkMsg.setRef(my_user.id());
         xTalkMsg.setRef(0);

         auto cipherText = my_cipher.encipher(text.toUtf8());
         xTalkMsg.appendI16(cipherText.length() + 3);
         xTalkMsg.appendQString(cipherText);

         my_socket.write(xTalkMsg);
         return my_socket.flush();
      }

      auto Client::do_sendPing(void) -> int
      {
         auto pingMsg = palace::NetMsg();

//         pingMsg.setId(NetMsg::kPingKind);
//         pingMsg.setLen(0);
//         //ping_msg.setRef(my_user.id());
//         pingMsg.setRef(NetMsg::kNoRef);
         pingMsg.setId(NetMsg::kPingKind);
         pingMsg.setLen(0);
         pingMsg.setRef(NetMsg::kNoRef);

         my_socket.write(pingMsg);
         my_socket.flush();

         my_logger.appendDebugMessage(QString("Sent Ping"));

         return 0;
      }

      auto Client::do_sendPong(void) -> int
      {
         auto pongMsg = palace::NetMsg();

//         pong_msg.setId(NetMsg::kPongKind);
//         pong_msg.setLen(0);
//         //pong_msg.set_ref(my_user_.id());
//         pong_msg.setRef(0);
         pongMsg.appendI32(NetMsg::kPongKind);
         pongMsg.appendI32(0);
         pongMsg.appendI32(NetMsg::kNoRef);

         my_socket.write(pongMsg);
         my_socket.flush();

         my_logger.appendDebugMessage(QString("Sent Pong"));

         return 0;
      }

      auto Client::do_sendGotoRoom(i16 roomId) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         my_room.userListPtr()->clear();
         my_room.setId(roomId);

         auto gotoroomMsg = palace::NetMsg();
         gotoroomMsg.setId(NetMsg::kGotoRoomKind);
         gotoroomMsg.setLen(NetMsg::kGotoRoomLen);
         gotoroomMsg.setRef(NetMsg::kNoRef);

         gotoroomMsg.appendI16(roomId);

         my_socket.write(gotoroomMsg);
         my_socket.flush();

         return 0;
      }

      auto Client::do_sendMove(i16 x, i16 y) -> int
      {
         if (my_connectionState != ConnectionState::kConnectedState)
            return -1;

         auto moveMsg = palace::NetMsg();
         moveMsg.setId(NetMsg::kMovementKind);
         moveMsg.setLen(NetMsg::kMovementLen);
         moveMsg.setRef(NetMsg::kNoRef);

         moveMsg.appendI16(y);
         moveMsg.appendI16(x);

         my_socket.write(moveMsg);
         my_socket.flush();

         // emit userMove signal to invalidate view
         return 0;
      }

      auto Client::do_deinit(void) -> void
      {
         disconnectFromHost();
      }

      auto Client::do_init(void) -> void
      {
         do_setupEvents();

         do_clear();
         my_logger.setMode(LogMode::kDebugMode);
         //my_logger_.setIsDebugMode(true);
      }
   }
}
