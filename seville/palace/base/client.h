#ifndef PALACE_CLIENT_H_
#define PALACE_CLIENT_H_

#include <string>

#include <QObject>
#include <QImage>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QTimer>
#include <QTime>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "seville/base/types.h"
#include "seville/base/log.h"

#include "seville/palace/base/server.h"
#include "seville/palace/base/user.h"
#include "seville/palace/base/room.h"
#include "seville/palace/base/cipher.h"
#include "seville/palace/base/logger.h"

#include "seville/palace/netmsg/generic.h"
#include "seville/palace/netmsg/httpserverlocation.h"
#include "seville/palace/netmsg/logon.h"
#include "seville/palace/netmsg/roomdescription.h"

namespace seville
{
   namespace palace
   {
      enum class ConnectionState: u32 {
         kDisconnected = 0x00000000,
         kHandshaking = 0x00000001,
         kConnected = 0x00000002,
      };

      enum class ReasonForTerminate: u32 {
         kNil = 0x00000000,
         kError = 0x00000001,
         kCommError = 0x00000002,
         kFlood = 0x00000003,
         kKilledByPlayer = 0x00000004,
         kServerDown = 0x00000005,
         kUnresponsive = 0x00000006,
         kKilledBySysop = 0x0000007,
         kServerFull = 0x00000008,
         kInvalidSerialNumber = 0x0000009,
         kDuplicateUser = 0x0000000a,
         kDeathPenaltyActive = 0x0000000b,
         kBanished = 0x0000000c,
         kBanishKill = 0x0000000d,
         kNoGuests = 0x0000000e,
         kDemoExpired = 0x0000000f,
         kUnknown = 0x00000010,
      };

      enum class ErrorForNavigation: u32 {
         kInternalError = 0x00000000,
         kUnknownRoom = 0x00000001,
         kRoomFull = 0x00000002,
         kRoomClosed = 0x00000003,
         kCantAuthor = 0x00000004,
         kPalaceFull = 0x00000005,
      };

      enum class FlagAuxOptions: u32 {
         kUnknownMachine = 0x00000000,
         kMac68k = 0x00000001,
         kMacPowerPc = 0x00000002,
         kWin16 = 0x00000003,
         kWin32 = 0x00000004,
         kJava = 0x00000005,
         kMacIntel = 0x00000006,
         kIOS = 0x00000010,
         kAndroid = 0x00000011,
         kLinux = 0x00000032,
         kFreeBsd = 0x00000080,
         kOSMask = 0x0000000F,
         kAuthenticate = 0x80000000,
      };

      enum class UploadCapabilities: u32 {
         kNoUpload = 0x00000000,
         kPalaceAssetUpload = 0x00000001,
         kFtpAssetUpload = 0x00000002,
         kHttpAssetUpload = 0x00000003,
         kOtherAssetUpload = 0x00000004,
         kPalaceFileUpload = 0x00000008,
         kFtpFileUpload = 0x000000010,
         kHttpFileUpload = 0x00000020,
         kOtherFileUpload = 0x00000080,
         kExtendedUpload = 0x00000100,
      };

      enum class DownloadCapabilities: u32 {
         kNoDownload = 0x00000000,
         kPalaceAssetDownload = 0x00000001,
         kFtpAssetDownload = 0x00000002,
         kHttpAssetDownload = 0x00000004,
         kOtherAssetDownload = 0x00000008,
         kPalaceFileDownload = 0x00000010,
         kFtpFileDownload = 0x00000020,
         kHttpFileDownload = 0x00000040,
         kOtherFileDownload = 0x00000080,
         kHttpFileExtendedDownload = 0x00000100,
         kExtendedDownload = 0x00000200,
      };

      enum class EngineCapabilities2d: u32 {
         kNo2dEngine = 0x00000000,
         kPalace2dEngine = 0x00000001,
      };

      enum class GraphicsCapabilities2d: u32 {
         kNo2dGraphics = 0x00000000,
         kGif87 = 0x00000001,
         kGif89a = 0x00000002,
         kJpg = 0x00000004,
         kTiff = 0x00000008,
         kTarga = 0x00000010,
         kBmp = 0x00000020,
         kPct = 0x00000040,
      };

      enum class GraphicsCapabilities3d: u32 {
         kNo3dGraphics = 0x00000000,
         kVrml1 = 0x0000001,
         kVrml2 = 0x0000002,
      };

      class Client : public QObject
      {
      Q_OBJECT

      signals:
         void backgroundImageDidLoad(void);

      public:
         static const char* kIdent;

         static const int kDefaultServerPort = 9998;

         static constexpr int kIntervalTimeoutForTransferInMs = 30 * 1000;
         static constexpr int kIntervalToTimeoutForPongInMs = 60 * 1000;
         static constexpr int kIntervalToPingInMs = 5 * 1000;

         static const u32 kMagicFromPChat = 0x00011940;
         //static const QString kIdent = "PC4237"
         static const int kMaxSizeOfUsernameInBytes = 31;
         static const int kMaxSizeOfWizpassInBytes = 31;
         static const int kSizeOfIdentInBytes = 6;
         static const int kDefaultSizeOfChunkReadInBytes = 100; /* NetMsg::kHeaderSize; */
         static const int kMinSizeOfShortestChunkReadInBytes = 1;

//            static const int kDemoElapsed = 0x00011940;
//            static const int kTotalElapsed = 0x00011940;
//            static const int kDemoLimit = 0x00011940;

//            static const quint32 kAuxFlagsUnknownMachine    = 0x00000000;
//            static const quint32 kAuxFlagsMac68k            = 0x00000001;
//            static const quint32 kAuxFlagsMacPpc            = 0x00000002;
//            static const quint32 kAuxFlagsWin16             = 0x00000003;
//            static const quint32 kAuxFlagsWin32             = 0x00000004;
//            static const quint32 kAuxFlagsJava              = 0x00000005;
//            static const quint32 kAuxFlagsBsd               = 0x00000006;
//            static const quint32 kAuxFlagsLinux             = 0x00000007;
//            static const quint32 kAuxFlagsOSMask            = 0x0000000F;
//            static const quint32 kAuxFlagsAuthenticate      = 0x80000000;

//            static const quint32 kUlCapsAssetsPalace        = 0x00000001;
//            static const quint32 kUlCapsAssetsFtp           = 0x00000002;
//            static const quint32 kUlCapsAssetsHttp          = 0x00000004;
//            static const quint32 kUlCapsAssetsOther         = 0x00000008;
//            static const quint32 kUlCapsFilesPalace         = 0x00000010;
//            static const quint32 kUlCapsFilesFtp            = 0x00000020;
//            static const quint32 kUlCapsFilesHttp           = 0x00000040;
//            static const quint32 kUlCapsFilesOther          = 0x00000080;
//            static const quint32 kUlCapsExtendPkt           = 0x00000100;

//            static const quint32 kDlCapsAssetsPalace        = 0x00000001;
//            static const quint32 kDlCapsAssetsFtp           = 0x00000002;
//            static const quint32 kDlCapsAssetsHttp          = 0x00000004;
//            static const quint32 kDlCapsAssetsOther         = 0x00000008;
//            static const quint32 kDlCapsFilesPalace         = 0x00000010;
//            static const quint32 kDlCapsFilesFtp            = 0x00000020;
//            static const quint32 kDlCapsFilesHttp           = 0x00000040;
//            static const quint32 kDlCapsFilesOther          = 0x00000080;
//            static const quint32 kDlCapsFilesHttpSvr        = 0x00000100;
//            static const quint32 kDlCapsExtendPkt           = 0x00000200;

      private:
         Logger my_logger;
         //QImage myBackgroundImg;
         QNetworkAccessManager my_networkAccessManager;
         //NetMsg myNetMsgRx;
         //QDataStream *myNetMsgRxDs;
         //NetMsg myNetMsgTx;
         //QDataStream *myNetMsgTxDs;
         netmsg::Generic my_netMsg;
         int my_TransferTimerId;
         QTimer my_pingTimer;
         QTimer my_receiveNetMsgTimer;
         QTime my_pongTime;
         //QByteArray myBuffer;
         Host::ByteOrder my_byteOrder;
         ConnectionState my_connectionState;
         QTcpSocket my_socket;
         QString my_username;
         QString my_host;
         quint16 my_port;
         Server my_server;
         User my_user;
         Room my_currentRoom;
         Cipher my_cipher;
         QTimer my_timer;
         int my_transferTimerId;

         bool my_puidChanged    = false;
         u32 my_puidCounter = 0xf5dc385e;
         u32 my_puidCrc     = 0xc144c580;
         u32 my_regCounter  = 0xcf07309c;
         u32 my_regCrc      = 0x5905f923;

         //      int Client::getKMillisecondPongTimeout()
         //      {
         //         return kIntervalToTimeoutForPongInMs;
         //      }

         //      void Client::setKMillisecondPongTimeout(int value)
         //      {
         //         kIntervalToTimeoutForPongInMs = value;
         //      }

         void do_setupEvents(void)
         {
            // wire up events
            //connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            //connect(&mySocket, SIGNAL())
            connect(&my_socket, &QIODevice::readyRead,
               this, [=](void)  //&Client::on_readyRead);
               {
                  do_readSocket();
               }
            );

            connect(
               &my_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
               this, [=](void) //&Client::on_socketError
               {
                  my_logger.error("Socket Error");
                  qCDebug(sevilleLog) << "ERROR: Socket Error";
               }
            );

            connect(
               &my_networkAccessManager, &QNetworkAccessManager::finished,
               this, [=](QNetworkReply* pReply)
               {
                  if (pReply->error())
                  {
                     my_logger.error(pReply->errorString());
                  }
                  else
                  {
                     auto qByteArray = pReply->readAll();
                     auto const byteArray = reinterpret_cast<ByteArray*>(&qByteArray);
                     my_currentRoom.setBackgroundImageBytes(*byteArray);

                     emit backgroundImageDidLoad();
                  }

                  pReply->deleteLater();
               }
            );

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
         }

         void fetchBackgroundAsync(const QString& url) //, const QMap<QString, QString>& mapOfHeaders)
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
//                        QByteArray::fromStdString(iterator.key().toStdString()),
//                        QByteArray::fromStdString(iterator.value().toStdString())
//                  );
//               }
//            }

            my_networkAccessManager.get(request);
         }

         void do_reset(void)
         {
            my_transferTimerId = 0;
            my_socket.disconnectFromHost();
            do_determineClientByteOrder();
            my_connectionState = Client::ConnectionState::Disconnected;
            //myLog.resetState();
            my_server.reset();
            my_user.reset();
            my_currentRoom.reset();
            my_netMsg.reset();
         }

         void do_resetReceiveTimer(void)
         {
            if (my_transferTimerId)
            {
               killTimer(my_transferTimerId);
               my_transferTimerId = 0;
            }
         }

         void do_readSocket(void)
         {
            //NetMsg::Generic& netMsg = myNetMsg;

            if (my_transferTimerId)
            {
               killTimer(my_transferTimerId);
               my_transferTimerId = 0;
            }

            while (my_netMsg.readInNetMsgFromSocketP(&my_socket))
            {
               if (my_netMsg.size() < NetMsg::kMinimumSizeInBytes ||
                   my_netMsg.size() < my_netMsg.len())
               {
                  my_transferTimerId = startTimer(kIntervalTimeoutForTransferInMs);
                  continue;
               }

               if (my_connectionState == ConnectionState::kHandshaking)
               {
                  if (do_determineServerByteOrder() < 0) //myNetMsg) < 0)
                  {
                     qCDebug(sevilleLog) <<
                           "ERROR: Could not determine server byte order!";
                     return;
                  }

                  u32 userId = my_netMsg.ref();
                  my_user.setId(userId);

                  if (do_sendLogon() < 0)
                  {
                     qCDebug(sevilleLog) << "ERROR: Could not send Logon!";
                     return;
                  }

                  my_connectionState = ConnectionState::kConnected;
                  qCDebug(sevilleLog) << "Client is now in Connected State";
               }
               else if (my_connectionState == ConnectionState::kConnected)
               {
                  do_routeReceivedNetMsg();
               }

               //qCDebug(sevilleLog) << "Resetting NetMsg";
               my_netMsg.reset();
            }

            my_pingTimer.start();
            my_pongTime.start();
         }

         void do_setConnectionState(ConnectionState connectionState)
         {
            my_connectionState = connectionState;
            // TODO signal?
            //do_updateMenus();
         }

         void do_connectToHost(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initialRoom = 0)
         {
            int dHostTcpPortCond = (0 == port);
            u16 dHostTcpPort = static_cast<u16>(
                     (dHostTcpPortCond * kDefaultServerPort) |
                     ((!dHostTcpPortCond) * port)
            );

            u16 dInitialRoom = static_cast<u16>(initialRoom);
            qCDebug(sevilleLog, "Connecting to palace://%s@%s:%d/%d",
                    username.toUtf8().data(),
                    host.toUtf8().data(),
                    dHostTcpPort, dInitialRoom);

            my_user.setUsername(username);
            my_server.setHost(host);
            my_server.setPort(dHostTcpPort);
            my_currentRoom.setId(dInitialRoom);

            qCDebug(sevilleLog) << "Client Connection is now in Handshaking State";
            my_connectionState = ConnectionState::kHandshaking;
            my_socket.connectToHost(my_server.host(), my_server.port());
         }

         void do_disconnectFromHost(void)
         {
            if (my_connectionState != Client::ConnectionState::Disconnected)
            {
               my_logger.info("Disconnected.");
               my_socket.disconnectFromHost();
               my_connectionState = ConnectionState::kDisconnected;
            }
         }

         void do_determineClientByteOrder(void)
         {
            // TODO
            auto isLittleEndian = (Host::LittleEndian * !Client::isBigEndian());
            auto isBigEndian = (Host::BigEndian * Client::isBigEndian());
            my_byteOrder = static_cast<Host::ByteOrder>(isLittleEndian | isBigEndian);
         }

         int do_determineServerByteOrder(void)
         {
            auto res = 0;
            // TODO more consideration here.
            // constants are in big endian byte order,
            // conditionally swapping them if we are a little endian system seems off.
            // but shouldn't we have to swap them all? we don't.

            auto isLE = static_cast<u32>(my_byteOrder == Host::LittleEndian);
            auto kind = (isLE * Host::swapU32(my_netMsg.id())) |
                        (!isLE * my_netMsg.id());

            if (NetMsg::Kind::UnknownServerKind == kind)
            {
               qCDebug(sevilleLog) << "Server has Unknown Byte Order";
               my_server.setByteOrder(Host::ByteOrder::Unknown);
               res = 0;
            }
            else if (NetMsg::Kind::BigEndianServerKind == kind)
            {
               qCDebug(sevilleLog) << "Server has Big Endian Byte Order";
               my_server.setByteOrder(Host::ByteOrder::BigEndian);
               res = 1;
            }
            else if (NetMsg::Kind::LittleEndianServerKind == kind)
            {
               qCDebug(sevilleLog) << "Server has Little Endian Byte Order";
               my_server.setByteOrder(Host::ByteOrder::LittleEndian);
               res = 1;
            }
            else
            {
               qCDebug(sevilleLog) << "Server did not indicate its Byte Order";
               my_server.setByteOrder(Host::ByteOrder::Unknown);
               res = 0;
            }

            return res;
         }

         /*
         int Client::do_readDataIntoNetMsg(NetMsg::Generic& netMsg, i32 maxSize)
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

         bool do_determineIsConnected(void) const
         {
            return my_socket.state() != QTcpSocket::ConnectedState ||
                  my_connectionState != ConnectionState::kConnected;
         }

         bool do_determineIfShouldSwapEndianness(void) const
         {
            int notUnknownClientByteOrder =
                  my_byteOrder != Host::ByteOrder::Unknown;

            int notUnknownServerByteOrder =
                  my_server.byteOrder() != Host::ByteOrder::Unknown;

            int notSameByteOrder = my_byteOrder != my_server.byteOrder();

            return notUnknownClientByteOrder && notUnknownServerByteOrder && notSameByteOrder;
         }

         /**
          * @brief do_receiveAltLogon
          * @return number of network messages handled
          */
         int do_receiveAltLogon(void)
         {
            auto netMsgLogon = static_cast<NetMsg::Logon>(my_netMsg);
            if (my_user.idCounter() != netMsgLogon.puidCounter() ||
                my_user.idCrc() != netMsgLogon.puidCrc())
            {
               my_user.setIdCrc(netMsgLogon.puidCrc());
               my_user.setIdCounter(netMsgLogon.puidCounter());
               my_user.setIdChanged(true);
            }

            return 1;
         }

         int do_receiveAltRoomDescription(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveAssetIncoming(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveAssetQuery(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveAuthenticate(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveBlowThru(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveConnectionError(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveDoorLock(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveDoorUnlock(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveDraw(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveHttpServerLocation(void)
         {
            int res = 0;

            auto httpServerLocation = static_cast<NetMsg::HttpServerLocation&>(my_netMsg);
            auto url = httpServerLocation.url();
            my_server.setHttpServerLocation(url);

            my_logger.debug(QString("HTTP Server is %1").arg(url));

            res = 1;
            return res;
         }

         int do_receiveMovement(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveNavigationError(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePictMove(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePing(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePong(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePropDelete(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePropMove(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receivePropNew(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveRoomDescend(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveRoomDescription(void)
         {
            int value = 0;

            auto roomDescription = static_cast<NetMsg::RoomDescription&>(my_netMsg);
            my_currentRoom.setFromRoomDescription(roomDescription);

            fetchBackgroundAsync(my_server.httpServerLocation() + "/" + my_currentRoom.backgroundImageName());

            value = 1;
            return value;
         }

         int do_receiveRoomUserList(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveServerVersion(void)
         {
            auto value = 0;

            auto netMsgServerVersion = static_cast<NetMsg::Generic>(my_netMsg);
            my_server.setVersion(netMsgServerVersion.ref());

            return value;
         }

         int do_receiveServerInfo(void)
         {
            auto value = 0;
            // stub

            return value;
         }

         int do_receiveUserNew(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveUserColor(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveUserExitRoom(void)
         {
            // stub
            int value = 0;
            return value;
         }

         int do_receiveUserFace(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveUserProp(void)
         {
            int res = 0;
            return res;
         }

         int do_receiveUserDescription(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveUserRename(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveUserLeaving(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveUserLoggedOnAndMax(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveUserStatus(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveServerRoomList(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveServerUserList(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveSpotMove(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveSpotState(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveTalk(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveWhisper(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveXTalk(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_receiveXWhisper(void)
         {
            // stub
            int res = 0;
            return res;
         }

         int do_sendLogon(void)
         {
            auto res = 0;
            NetMsg::Logon msgLogon(do_determineIfShouldSwapEndianness());

            msgLogon.setRegCrc(my_user.regCrc());
            msgLogon.setRegCounter(my_user.regCounter());
            msgLogon.setUsername(my_user.username());
            msgLogon.setWizardPassword(my_user.wizpass());

            msgLogon.setFlags(
                     static_cast<u32>(FlagAuxOptions::kAuthenticate) |
                     static_cast<u32>(FlagAuxOptions::kWin32)  // lies
                  );

            msgLogon.setPuidCounter(my_puidCounter);
            msgLogon.setPuidCrc(my_puidCrc);
            msgLogon.setDemoElapsed(kMagicFromPChat);
            msgLogon.setTotalElapsed(kMagicFromPChat);
            msgLogon.setDemoLimit(kMagicFromPChat);
            msgLogon.setInitialRoomId(my_currentRoom.id());
            msgLogon.setReserved(kIdent);
            msgLogon.setUploadRequestedProtocolVersion(0);

            msgLogon.setUploadCapabilities(
                     static_cast<u32>(UploadCapabilities::kPalaceAssetUpload) |
                     static_cast<u32>(UploadCapabilities::kHttpFileUpload)
                  );

            msgLogon.setDownloadCapabilities(
                     static_cast<u32>(DownloadCapabilities::kPalaceAssetDownload) |
                     static_cast<u32>(DownloadCapabilities::kPalaceFileDownload) |
                     static_cast<u32>(DownloadCapabilities::kHttpFileDownload) |
                     static_cast<u32>(DownloadCapabilities::kHttpFileExtendedDownload)
                  );

            msgLogon.setEngineCapabilities2d(static_cast<u32>(EngineCapabilities2d::kPalace2dEngine));
            msgLogon.setGraphicsCapabilities2d(static_cast<u32>(GraphicsCapabilities2d::kGif87));
            msgLogon.setGraphicsCapabilities3d(static_cast<u32>(GraphicsCapabilities3d::kNo3dGraphics));

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

            my_socket.write(msgLogon);
            res = my_socket.flush();

            do_setConnectionState(ConnectionState::kConnected);

            return res;
         }

         int do_sendAuthenticate(const QString& username, const QString& password)
         {
            // TODO stub
            (void)username;
            (void)password;

            int res = do_determineIsConnected();
            if (!res)
               return res;

            NetMsg::Generic msgAuthenticate;

            // TODO stub
            //netMsg.appendU32(NetMsg::Kind::AuthResponseKind);
            //netMsg.appendDw();
            msgAuthenticate.setId(NetMsg::Kind::AuthResponseKind);
            my_socket.write(msgAuthenticate);

            return res;
         }

         int do_routeReceivedNetMsg(void)
         {
            auto res = 0;
            auto kind = my_netMsg.id();
            auto size = my_netMsg.size();

            if (size < NetMsg::kMinimumSizeInBytes)
            {
               my_logger.debug("Received NetMsg too small!");
               qCDebug(sevilleLog) << "Received NetMsg that is too small to interpret";
               qCDebug(sevilleLog) << "kind: " << QString("0x%1").arg(my_netMsg.id(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "size expected: " << QString("0x%1").arg(my_netMsg.len(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "reference code: " << QString("0x%1").arg(my_netMsg.ref(), 2, 16, QChar('0'));
               return -1;
            }
            else if (NetMsg::kMaximumSizeInBytes < size)
            {
               my_logger.debug("Received NetMsg too large!");
               qCDebug(sevilleLog) << "Received NetMsg that is too large to interpret";
               qCDebug(sevilleLog) << "kind: " << QString("0x%1").arg(my_netMsg.id(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "size expected: " << QString("0x%1").arg(my_netMsg.len(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "reference code: " << QString("0x%1").arg(my_netMsg.ref(), 2, 16, QChar('0'));
               return -2;
            }

            //u32 zNetMsgId = doCsbolecDw_(netMsgId);
            //switch (id) {
               //case Net::Msg::LogonKind:
               //    res = sendLogon_();
               //    break;
            if (NetMsg::Kind::AltLogonKind == kind)
                //NetMsg::sizes[NetMsg::AltLogonKind] == size) {
            {
               qCDebug(sevilleLog) << "Received AltLogon";
               my_logger.debug("Received AltLogon");
               if (NetMsg::kSizeOfLogonInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveAltLogon();
            }
            else if (NetMsg::Kind::ConnectionErrorKind == kind)

            {
               qCDebug(sevilleLog) << "Received ConnectionError";
               my_logger.debug("Received ConnectionError");
               if (NetMsg::kSizeOfConnectionErrorInBytes != size)
                  my_logger.debug("(but size did not match!)");

               //auto connectionError = dynamic_cast<const NetMsg::ConnectionError&>(myNetMsg);

               res = do_receiveConnectionError();
            }
            else if (NetMsg::Kind::ServerVersionKind == kind)
            {
               qCDebug(sevilleLog) << "Received ServerVersion";
               my_logger.debug("Received ServerVersion");
               if (NetMsg::kSizeOfServerVersionInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveServerVersion();
            }
            else if (NetMsg::Kind::ServerInfoKind == kind)
            {
               qCDebug(sevilleLog) << "Received ServerInfo";
               my_logger.debug("Received ServerInfo");
               if (NetMsg::kSizeOfServerInfoInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveServerInfo();
            }
            else if (NetMsg::Kind::UserStatusKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserStatus";
               my_logger.debug("Received UserStatus");
               if (NetMsg::kSizeOfUserStatusInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveUserStatus();
            }
            else if (NetMsg::Kind::UserLoggedOnAndMaxKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserLoggedOnAndMax";
               my_logger.debug("Received UserLoggedOnAndMax");
               if (NetMsg::kSizeOfUserLoggedOnAndMaxInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveUserLoggedOnAndMax();
            }
            else if (NetMsg::Kind::HttpServerLocationKind == kind)
            {
               qCDebug(sevilleLog) << "Received HttpServerLocation";
               my_logger.debug("Received HttpServerLocation");
               if (size < NetMsg::kSizeOfHttpServerLocationInBytes)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveHttpServerLocation();
            }
            else if (NetMsg::Kind::RoomUserListKind == kind)
            {
               qCDebug(sevilleLog) << "Received RoomUserList";
               my_logger.debug("Received RoomUserList");
               if (size < NetMsg::kSizeOfRoomUserListInBytes)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveRoomUserList();
            }
            else if (NetMsg::Kind::ServerUserListKind == kind)
            {
               qCDebug(sevilleLog) << "Received ServerUserList";
               my_logger.debug("Received ServerUserList");
               if (size < NetMsg::kSizeOfServerUserListInBytes)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveServerUserList();
            }
            else if (NetMsg::Kind::ServerRoomListKind == kind)
            {
               qCDebug(sevilleLog) << "Received ServerRoomList";
               my_logger.debug("Received ServerRoomList");
               if (size < NetMsg::kSizeOfServerRoomListInBytes)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveServerRoomList();
            }
            else if (NetMsg::Kind::RoomDescendKind == kind)
            {
               qCDebug(sevilleLog) << "Received RoomDescend";
               my_logger.debug("Received RoomDescend");
               if (size < NetMsg::kSizeOfRoomDescendedInBytes)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveRoomDescend();
            }
            else if (NetMsg::Kind::UserNewKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserNew";
               my_logger.debug("Received UserNew");
               if (NetMsg::kSizeOfUserNewInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserNew();
            }
            else if (NetMsg::Kind::PingKind == kind)
            {
               qCDebug(sevilleLog) << "Received Ping";
               my_logger.debug("Received Ping");
               if (NetMsg::kSizeOfPingInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receivePing();
            }
            else if (NetMsg::Kind::PongKind == kind)
            {
               qCDebug(sevilleLog) << "Received Pong";
               my_logger.debug("Received Pong");
               if (NetMsg::kSizeOfPongInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receivePong();
            }
            else if (NetMsg::Kind::XTalkKind == kind)
            {
               qCDebug(sevilleLog) << "Received XTalk";
               my_logger.debug("Received XTalk");
               if (size < NetMsg::kSizeOfXTalkInBytes)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveXTalk();
            }
            else if (NetMsg::Kind::XWhisperKind == kind)
            {
               qCDebug(sevilleLog) << "Received XWhisper";
               my_logger.debug("Received XWhisper");
               if (size < NetMsg::kSizeOfXWhisperInBytes)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveXWhisper();
            }
            else if (NetMsg::Kind::TalkKind == kind)
            {
               qCDebug(sevilleLog) << "Received Talk";
               my_logger.debug("Received Talk");
               if (size < NetMsg::kSizeOfTalkInBytes)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveTalk();
            }
            else if (NetMsg::Kind::WhisperKind == kind)

            {
               qCDebug(sevilleLog) << "Recieved Whisper";
               my_logger.debug("Received Whisper");
               if (size < NetMsg::kSizeOfWhisperInBytes)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveWhisper();
            }
            else if (NetMsg::Kind::AssetIncomingKind == kind)
            {
               qCDebug(sevilleLog) << "Received AssetIncoming";
               my_logger.debug("Received AssetIncoming");
               if (NetMsg::kSizeOfAssetIncomingInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveAssetIncoming();
            }
            else if (NetMsg::Kind::AssetQueryKind == kind)
            {
               qCDebug(sevilleLog) << "Received AssetQuery";
               my_logger.debug("Received AssetQuery");
               if (NetMsg::kSizeOfAssetQueryInBytes != size)
               {
                  my_logger.debug("(but size did not match!");
               }

               res = do_receiveAssetQuery();
            }
            else if (NetMsg::Kind::MovementKind == kind)
            {
               qCDebug(sevilleLog) << "Received Movement";
               my_logger.debug("Received Movement");
               if (NetMsg::kSizeOfMovementInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveMovement();
            }
            else if (NetMsg::Kind::UserColorKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserColor";
               my_logger.debug("Received UserColor");
               if (NetMsg::kSizeOfUserColorInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserColor();
            }
            else if (NetMsg::Kind::UserFaceKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserFace";
               my_logger.debug("Received UserFace");
               if (NetMsg::kSizeOfUserFaceInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserFace();
            }
            else if (NetMsg::Kind::UserPropKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserProp";
               my_logger.debug("Received UserProp");
               if (NetMsg::kSizeOfUserPropInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserProp();
            }
            else if (NetMsg::Kind::UserDescriptionKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserDescription";
               my_logger.debug("Received UserDescription");
               if (NetMsg::kSizeOfUserDescriptionInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserDescription();
            }
            else if (NetMsg::Kind::UserRenameKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserRename";
               my_logger.debug("Received UserRename");
               if (NetMsg::kSizeOfUserRenameInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserRename();
            }
            else if (NetMsg::Kind::UserLeavingKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserLeaving";
               my_logger.debug("Received UserLeaving");
               if (NetMsg::kSizeOfUserLeavingInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserLeaving();
            }
            else if (NetMsg::Kind::UserExitRoomKind == kind)
            {
               qCDebug(sevilleLog) << "Received UserExitRoom";
               my_logger.debug("Received UserExitRoom");
               if (NetMsg::kSizeOfUserExitRoomInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receiveUserExitRoom();
            }
            else if (NetMsg::Kind::PropMoveKind == kind)
            {
               qCDebug(sevilleLog) << "Received PropMove";
               my_logger.debug("Received PropMove");
               if (NetMsg::kSizeOfPropMoveInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receivePropMove();
            }
            else if (NetMsg::Kind::PropDeleteKind == kind)
            {
               qCDebug(sevilleLog) << "Received PropDelete";
               my_logger.debug("Received PropDelete");
               if (NetMsg::kSizeOfPropDeleteInBytes != size)
               {
                  my_logger.debug("(but size did not match!)");
               }

               res = do_receivePropDelete();
            }
            else if (NetMsg::Kind::PropNewKind == kind)
            {
               qCDebug(sevilleLog) << "Received PropNew";
               my_logger.debug("Received PropNew");
               if (NetMsg::kSizeOfPropNewInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receivePropNew();
            }
            else if (NetMsg::Kind::DoorLockKind == kind)
            {
               qCDebug(sevilleLog) << "Received DoorLock";
               my_logger.debug("Received DoorLock");
               if (NetMsg::kSizeOfDoorLockInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveDoorLock();
            }
            else if (NetMsg::Kind::DoorUnlockKind == kind)
            {
               my_logger.debug("Received DoorUnlock");
               qCDebug(sevilleLog) << "Received DoorUnlock";
               if (NetMsg::kSizeOfDoorUnlockInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveDoorUnlock();
            }
            else if (NetMsg::Kind::PictMoveKind == kind)
            {
               my_logger.debug("Received PictMove");
               qCDebug(sevilleLog) << "Received PictMove";
               if (NetMsg::kSizeOfPictMoveInBytes == size)
                  my_logger.debug("(but size did not match!)");

               res = do_receivePictMove();
            }
            else if (NetMsg::Kind::SpotStateKind == kind)
            {
               my_logger.debug("Received SpotState");
               qCDebug(sevilleLog) << "Received SpotState";
               if (NetMsg::kSizeOfSpotStateInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveSpotState();
            }
            else if (NetMsg::Kind::SpotMoveKind == kind)
            {
               my_logger.debug("Received SpotMove");
               qCDebug(sevilleLog) << "Received SpotMove";
               if (NetMsg::kSizeOfSpotMoveInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveSpotMove();
            }
            else if (NetMsg::Kind::DrawKind == kind)
            {
               my_logger.debug("Received Draw");
               qCDebug(sevilleLog) << "Received Draw";
               if (NetMsg::kSizeOfDrawInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveDraw();
            }
            else if (NetMsg::Kind::NavErrorKind == kind)
            {
               my_logger.debug("Received NavError");
               qCDebug(sevilleLog) << "Received NavError";
               if (NetMsg::kSizeOfNavErrorInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveNavigationError();
            }
            else if (NetMsg::Kind::BlowThruKind == kind)
            {
               my_logger.debug("Received BlowThru");
               qCDebug(sevilleLog) << "Received BlowThru";
               if (NetMsg::kSizeOfBlowThruInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveBlowThru();
            }
            else if (NetMsg::Kind::AuthenticateKind == kind)
            {
               my_logger.debug("Received Authenticate");
               qCDebug(sevilleLog) << "Received Authenticate";
               if (NetMsg::kSizeOfAuthenticateInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveAuthenticate();
            }
            else if (NetMsg::Kind::AltRoomDescriptionKind == kind)
            {
               my_logger.debug("Received AltRoomDescription");
               qCDebug(sevilleLog) << "Received AltRoomDescription";
               if (NetMsg::kSizeOfAltRoomDescriptionInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveRoomDescription();
            }
            else if (NetMsg::Kind::RoomDescriptionKind == kind)
            {
               my_logger.debug("Received RoomDescription");
               qCDebug(sevilleLog) << "Received RoomDescription";
               if (NetMsg::kSizeOfRoomDescriptionInBytes != size)
                  my_logger.debug("(but size did not match!)");

               res = do_receiveRoomDescription();
            }
            else
            {
               my_logger.debug(QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }").arg(my_netMsg.id(), 2, 16, QChar('0')).arg(my_netMsg.len(), 2, 16, QChar('0')).arg(my_netMsg.ref(), 2, 16, QChar('0')));
               qCDebug(sevilleLog) << "Received unknown NetMsg";
               qCDebug(sevilleLog) << "kind:  " << QString("0x%1").arg(my_netMsg.id(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "expected size: " << QString("0x%1").arg(my_netMsg.len(), 2, 16, QChar('0'));
               qCDebug(sevilleLog) << "reference code: " << QString("0x%1").arg(my_netMsg.ref(), 2, 16, QChar('0'));
               res = 0;
            }

            return res;
         }

      //public slots:
         //void on_readyRead(void);
         //void on_socketError(void);
         //void on_pingTimer(QTimerEvent* pingTimeEvent);
//         void on_(QNetworkReply* reply)
//         {

//         }

      public:
         //const QString kIdent = "PC4237";

         Host::ByteOrder byteOrder(void) { return my_byteOrder; }
         void setByteOrder(const Host::ByteOrder value) { my_byteOrder = value; }

         Client::ConnectionState connectionState(void) { return my_connectionState; }

         QString username(void) const { return my_username; }
         void setUsername(const QString &value) { my_username = value; }

         Logger* loggerP(void) { return &my_logger; }
         Server* serverP(void) { return &my_server; }
         User* userP(void) { return &my_user; }
         Room* currentRoomP(void) { return &my_currentRoom; }

         bool isBigEndian(void)
         {
            bi32 bi;
            bi.dword = { 0x01020304 };

            return bi.ch[0] == 0x01;
         }

//            u32 puidCounter() const { return myPuidCounter; }
//            void setPuidCounter(u32 value) { myPuidCounter = value; }

//            u32 regCounter() const { return myRegCounter; }
//            void setRegCounter(u32 value) { myRegCounter = value; }

//            u32 regCrc() const { return myRegCrc; }
//            void setRegCrc(u32 value) { myRegCrc = value; }

//            bool puidChanged() const { return myPuidChanged; }
//            void setPuidChanged(bool value) { myPuidChanged = value; }

//            u32 puidCrc() const { return myPuidCrc; }
//            void setPuidCrc(u32 value) { myPuidCrc = value; }

         void reset(void)
         {
            do_reset();
         }

         void connectToHost(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initialRoom = 0)
         {
            do_disconnectFromHost();
            do_connectToHost(host, port, username, initialRoom);
         }

         void connectToHost(
               QUrl url,
               QString username = "Seville User",
               int initialRoom = 0)
         {
            do_disconnectFromHost();
            do_connectToHost(url.host(), url.port(), username, initialRoom);
         }

         void connectToHost(QUrl url, int initialRoom = 0)
         {
            do_disconnectFromHost();
            do_connectToHost(url.host(), url.port(), url.userName(), initialRoom);
         }

         //{ doConnectToHost(url.toString(), Server::kDefaultServerPort, Client::kDefaultUsername, initialRoom); }
         void disconnectFromHost(void)
         {
            do_disconnectFromHost();
         }

         void do_deinit(void)
         {
            do_disconnectFromHost();
         }

         void do_init(void)
         {
            do_setupEvents();
            do_reset();

            my_logger.setIsDebugMode(true);
         }

         virtual ~Client(void);
         explicit Client(QObject* pParent = nullptr);
      };
   }
}

#endif // PALACE_CLIENT_H_
