#ifndef SEVILLE_PALACE_CLIENT_H_
#define SEVILLE_PALACE_CLIENT_H_

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

#include "seville/base/app.h"

#include "seville/palace/base/server.h"
#include "seville/palace/base/user.h"
#include "seville/palace/base/room.h"
#include "seville/palace/base/cipher.h"
#include "seville/palace/base/logger.h"
#include "seville/palace/base/netmsg.h"

#include "seville/palace/netmsg/httpserverlocation.h"
#include "seville/palace/netmsg/logon.h"
#include "seville/palace/netmsg/roomdescription.h"

namespace seville
{
   namespace palace
   {
      enum class ClientState: u32 {
         kDisconnectedClientState = 0x00000000,
         kHandshakingClientState = 0x00000001,
         kConnectedClientState = 0x00000002,
      };

      enum class TerminateReason: u32 {
         kNoTerminateReason = 0x00000000,
         kErrorTerminateReason = 0x00000001,
         kCommErrorTerminateReason = 0x00000002,
         kFloodTerminateReason = 0x00000003,
         kKilledByPlayerTerminateReason = 0x00000004,
         kServerDownTerminateReason = 0x00000005,
         kUnresponsiveTerminateReason = 0x00000006,
         kKilledBySysopTerminateReason = 0x0000007,
         kServerFullTerminateReason = 0x00000008,
         kInvalidSerialNumberTerminateReason = 0x0000009,
         kDuplicateUserTerminateReason = 0x0000000a,
         kDeathPenaltyActiveTerminateReason = 0x0000000b,
         kBanishedTerminateReason = 0x0000000c,
         kBanishKillTerminateReason = 0x0000000d,
         kNoGuestsTerminateReason = 0x0000000e,
         kDemoExpiredTerminateReason = 0x0000000f,
         kUnknownTerminateReason = 0x00000010,
      };

      enum class NavigationError: u32 {
         kInternalNavigationError = 0x00000000,
         kUnknownRoomNavigationError = 0x00000001,
         kRoomFullNavigationError = 0x00000002,
         kRoomClosedNavigationError = 0x00000003,
         kCantAuthorNavigationError = 0x00000004,
         kPalaceFullNavigationError = 0x00000005,
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
         static const int kDefaultSizeOfChunkReadInBytes = 100; /* netmsg::kHeaderSize; */
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

      public:
         explicit Client(QObject* parent_object_ptr = nullptr);
         virtual ~Client(void);

         //const QString kIdent = "PC4237";

         inline auto byteorder(void) -> Host::ByteOrder {
            return my_byte_order_;
         }

         inline auto set_byte_order(const Host::ByteOrder value) -> void {
            my_byte_order_ = value;
         }

         inline auto connection_state(void) -> ClientState {
            return my_connection_state_;
         }

         inline auto username(void) const -> QString {
            return my_username_;
         }

         inline auto set_username(const QString &value) -> void {
            my_username_ = value;
         }

         inline auto logger_ptr(void) -> Logger* {
            return &my_logger_;
         }

         inline auto server_ptr(void) -> Server* {
            return &my_server_;
         }

         inline auto user_ptr(void) -> User* {
            return &my_user_;
         }

         inline auto current_room_ptr(void) -> Room* {
            return &my_current_room_;
         }

         auto inline is_big_endian(void) -> bool {
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

         inline auto reset(void) -> void {
            do_reset_();
         }

         inline auto connect_to_host(
               QString host, \
               int port = 9998, \
               QString username = "Seville User", \
               int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(host, port, username, initial_room);
         }

         inline auto connect_to_host(
               QUrl url, \
               QString username = "Seville User", \
               int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(url.host(), url.port(), username, initial_room);
         }

         inline auto connect_to_host(QUrl url, int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(url.host(), url.port(), url.userName(), initial_room);
         }

         //{ doConnectToHost(url.toString(), Server::kDefaultServerPort, Client::kDefaultUsername, initialRoom); }
         inline auto disconnect_from_host(void) -> void {
            do_disconnect_from_host_();
         }

         inline auto do_deinit_(void) -> void {
            do_disconnect_from_host_();
         }

         inline auto do_init_(void) -> void {
            do_setup_events_();
            do_reset_();

            my_logger_.set_mode(Logger::kDebugMode);
            //my_logger_.setIsDebugMode(true);
         }

      signals:
         auto background_image_did_load(void) -> void;

      private:
         Logger my_logger_;
         //QImage myBackgroundImg;
         QNetworkAccessManager my_network_access_manager_;
         //NetMsg myNetMsgRx;
         //QDataStream *myNetMsgRxDs;
         //NetMsg myNetMsgTx;
         //QDataStream *myNetMsgTxDs;
         palace::NetMsg my_netmsg_;
         int my_transfer_timer_id_;
         QTimer my_ping_timer_;
         QTimer my_receive_netmsg_timer_;
         QTime my_pong_time_;
         //QByteArray myBuffer;
         Host::ByteOrder my_byte_order_;
         ClientState my_connection_state_;
         QTcpSocket my_socket_;
         QString my_username_;
         QString my_host_;
         quint16 my_port_;
         Server my_server_;
         User my_user_;
         Room my_current_room_;
         Cipher my_cipher_;
         QTimer my_timer_;
         //int my_transfer_timer_id_;

         bool my_puid_changed_    = false;
         u32 my_puid_counter_ = 0xf5dc385e;
         u32 my_puid_crc_     = 0xc144c580;
         u32 my_reg_counter_  = 0xcf07309c;
         u32 my_reg_crc_      = 0x5905f923;

         //      int Client::getKMillisecondPongTimeout()
         //      {
         //         return kIntervalToTimeoutForPongInMs;
         //      }

         //      void Client::setKMillisecondPongTimeout(int value)
         //      {
         //         kIntervalToTimeoutForPongInMs = value;
         //      }

         auto do_setup_events_(void) -> void
         {
            // wire up events
            //connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            //connect(&mySocket, SIGNAL())
            connect(&my_socket_, &QIODevice::readyRead,
                    this, [this](void) {  //&Client::on_readyRead);
                  do_read_socket_();
               }
            );

//            connect(
//               &my_socket_, QOverload<QAbstractSocket::SocketError>::of( \
//                        &QAbstractSocket::error),
//                     this, [this](void) {  //&Client::on_socketError
//                  my_logger_.error("Socket Error");
//                  qCDebug(log_seville) << "ERROR: Socket Error";
//               }
//            );

            connect(
                  &my_socket_,
                  &QTcpSocket::errorOccurred,
                  this,
                     [this](QAbstractSocket::SocketError socket_error) {
               my_logger_.error("Socket error");
               qCDebug(log_seville) << "ERROR: Socket Error";

            });

            connect(
               &my_network_access_manager_, &QNetworkAccessManager::finished,
               this, [this](QNetworkReply* reply_ptr) {
                  if (reply_ptr->error()) {
                     my_logger_.error(reply_ptr->errorString());
                  }
                  else {
                     auto qbytearray = reply_ptr->readAll();
                     auto const bytearray_ptr = \
                           reinterpret_cast<ByteArray*>(&qbytearray);
                     my_current_room_.set_background_image_bytes(
                              *bytearray_ptr);

                     emit background_image_did_load();
                  }

                  reply_ptr->deleteLater();
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

         //, const QMap<QString, QString>& mapOfHeaders)
         auto fetch_background_async(const QString& url) -> void
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

            my_network_access_manager_.get(request);
         }

         void do_reset_(void)
         {
            my_transfer_timer_id_ = 0;
            my_socket_.disconnectFromHost();
            do_determine_client_byteorder_();
            my_connection_state_ = ClientState::kDisconnectedClientState;
            //myLog.resetState();
            my_server_.reset();
            my_user_.reset();
            my_current_room_.reset();
            my_netmsg_.reset();
         }

         void do_reset_receive_timer_(void)
         {
            if (my_transfer_timer_id_)
            {
               this->killTimer(my_transfer_timer_id_);
               my_transfer_timer_id_ = 0;
            }
         }

         void do_read_socket_(void)
         {
            //netmsg::Generic& netMsg = myNetMsg;

            if (my_transfer_timer_id_)
            {
               this->killTimer(my_transfer_timer_id_);
               my_transfer_timer_id_ = 0;
            }

            while (my_netmsg_.read_in_netmsg_from_socket_ptr(&my_socket_))
            {
               if (my_netmsg_.size() < netmsg::kMinimuNetMsgSizeInBytes ||
                   my_netmsg_.size() < my_netmsg_.len())
               {
                  my_transfer_timer_id_ = \
                        startTimer(kIntervalTimeoutForTransferInMs);
                  continue;
               }

               if (my_connection_state_ == ConnectionState::kHandshaking)
               {
                  if (do_determine_server_byteorder_() < 0) //myNetMsg) < 0)
                  {
                     qCDebug(log_seville) <<
                           "ERROR: Could not determine server byte order!";
                     return;
                  }

                  u32 userId = my_netmsg_.ref();
                  my_user_.setId(userId);

                  if (do_send_logon() < 0)
                  {
                     qCDebug(log_seville) << "ERROR: Could not send Logon!";
                     return;
                  }

                  my_connection_state_ = ConnectionState::kConnected;
                  qCDebug(log_seville) << "Client is now in Connected State";
               }
               else if (my_connection_state_ == ConnectionState::kConnected)
               {
                  do_route_received_netmsg_();
               }

               //qCDebug(log_seville) << "Resetting NetMsg";
               my_netmsg_.reset();
            }

            my_ping_timer_.start();
            my_pong_time_.start();
         }

         void do_setConnectionState(ConnectionState connection_state)
         {
            my_connection_state_ = connection_state;
            // TODO signal?
            //do_updateMenus();
         }

         void do_connect_to_host_(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initial_room = 0)
         {
            auto cond_host_tcp_port = (0 == port);
            auto actual_host_tcp_port = static_cast<u16>(
                     (cond_host_tcp_port * kDefaultServerPort) |
                     ((!cond_host_tcp_port) * port)
            );

            auto actual_initial_room = static_cast<u16>(initial_room);
            qCDebug(log_seville, "Connecting to palace://%s@%s:%d/%d",
                    username.toUtf8().data(),
                    host.toUtf8().data(),
                    actual_host_tcp_port, actual_initial_room);

            my_user_.setUsername(username);
            my_server_.setHost(host);
            my_server_.setPort(actual_host_tcp_port);
            my_current_room_.set_id(actual_initial_room);

            qCDebug(log_seville) << "Client Connection is now in Handshaking State";
            my_connection_state_ = ConnectionState::kHandshaking;
            my_socket_.connectToHost(my_server_.host(), my_server_.port());
         }

         void do_disconnect_from_host_(void)
         {
            if (my_connection_state_ != ConnectionState::kDisconnected)
            {
               my_logger_.info("Disconnected.");
               my_socket_.disconnectFromHost();
               my_connection_state_ = ConnectionState::kDisconnected;
            }
         }

         void do_determine_client_byteorder_(void)
         {
            // TODO
            auto is_little_endian = (Host::kLittleEndian * !Client::is_big_endian());
            auto is_big_endian = (Host::kBigEndian * Client::is_big_endian());
            my_byte_order_ = static_cast<Host::ByteOrder>(is_little_endian | is_big_endian);
         }

         int do_determine_server_byteorder_(void)
         {
            auto res = 0;
            // TODO more consideration here.
            // constants are in big endian byte order,
            // conditionally swapping them if we are a little endian system seems off.
            // but shouldn't we have to swap them all? we don't.

            auto flag_little_endian = static_cast<u32>(Host::kLittleEndian == my_byte_order_);
            auto kind = (flag_little_endian * Host::SwapU32(my_netmsg_.id())) |
                        (!flag_little_endian * my_netmsg_.id());

            if (netmsg::NetMsgKind::kkUnknownServerKind == kind)
            {
               qCDebug(log_seville) << "Server has Unknown Byte Order";
               my_server_.set_byte_order(Host::ByteOrder::kUnknownEndian);
               res = 0;
            }
            else if (netmsg::NetMsgKind::kkBigEndianServerKind == kind)
            {
               qCDebug(log_seville) << "Server has Big Endian Byte Order";
               my_server_.set_byte_order(Host::ByteOrder::kBigEndian);
               res = 1;
            }
            else if (netmsg::NetMsgKind::kkLittleEndianServerKind == kind)
            {
               qCDebug(log_seville) << "Server has Little Endian Byte Order";
               my_server_.set_byte_order(Host::ByteOrder::kLittleEndian);
               res = 1;
            }
            else
            {
               qCDebug(log_seville) << "Server did not indicate its Byte Order";
               my_server_.set_byte_order(Host::ByteOrder::kUnknownEndian);
               res = 0;
            }

            return res;
         }

         /*
         int Client::do_readDataIntoNetMsg(netmsg::Generic& netMsg, i32 maxSize)
         {
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

         bool do_determine_is_connected_(void) const
         {
            return my_socket_.state() != QTcpSocket::ConnectedState ||
                  my_connection_state_ != ConnectionState::kConnected;
         }

         bool do_determine_if_should_swap_endianness_(void) const
         {
            int notUnknownClientByteOrder =
                  my_byte_order_ != Host::ByteOrder::kUnknownEndian;

            int notUnknownServerByteOrder =
                  my_server_.byteOrder() != Host::ByteOrder::kUnknownEndian;

            int notSameByteOrder = my_byte_order_ != my_server_.byteOrder();

            return notUnknownClientByteOrder && notUnknownServerByteOrder && notSameByteOrder;
         }

         /**
          * @brief do_receiveAltLogon
          * @return number of network messages handled
          */
         int do_receive_altlogon_(void)
         {
            auto netMsgLogon = static_cast<netmsg::LogonNetMsg>(my_netmsg_);
            if (my_user_.idCounter() != netMsgLogon.puidCounter() ||
                my_user_.idCrc() != netMsgLogon.puidCrc())
            {
               my_user_.setIdCrc(netMsgLogon.puidCrc());
               my_user_.setIdCounter(netMsgLogon.puidCounter());
               my_user_.setIdChanged(true);
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

            auto httpServerLocation = static_cast<netmsg::HttpServerLocation&>(my_netmsg_);
            auto url = httpServerLocation.url();
            my_server_.setHttpServerLocation(url);

            my_logger_.debug(QString("HTTP Server is %1").arg(url));

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

            auto roomDescription = static_cast<netmsg::RoomDescription&>(my_netmsg_);
            my_current_room_.set_from_room_description(roomDescription);

            fetch_background_async(my_server_.httpServerLocation() + "/" + my_current_room_.background_image_name());

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

            auto netMsgServerVersion = static_cast<netmsg::Generic>(my_netmsg_);
            my_server_.setVersion(netMsgServerVersion.ref());

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

         int do_send_logon(void)
         {
            auto res = 0;
            netmsg::LogonNetMsg msgLogon(do_determine_if_should_swap_endianness_());

            msgLogon.setRegCrc(my_user_.regCrc());
            msgLogon.setRegCounter(my_user_.regCounter());
            msgLogon.setUsername(my_user_.username());
            msgLogon.setWizardPassword(my_user_.wizpass());

            msgLogon.setFlags(
                     static_cast<u32>(FlagAuxOptions::kAuthenticate) |
                     static_cast<u32>(FlagAuxOptions::kWin32)  // lies
                  );

            msgLogon.setPuidCounter(my_puid_counter_);
            msgLogon.setPuidCrc(my_puid_crc_);
            msgLogon.setDemoElapsed(kMagicFromPChat);
            msgLogon.setTotalElapsed(kMagicFromPChat);
            msgLogon.setDemoLimit(kMagicFromPChat);
            msgLogon.setInitialRoomId(my_current_room_.id());
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
   //         msg.appendU32(kUlCapsAssetsPalace | kUlCapsFilesHttp); /* ulUploadCaps */
   //         msg.appendU32(kDlCapsAssetsPalace | kDlCapsFilesPalace | kDlCapsFilesHttp | kDlCapsFilesHttpSvr); /* ulDownloadCaps */
   //         msg.appendU32(0); /* ul2dEngineCaps* */
   //         msg.appendU32(0); /* ul2dGraphicsCaps* */
   //         msg.appendU32(0); /* ul3dEngineCaps* */

            my_socket_.write(msgLogon);
            res = my_socket_.flush();

            do_setConnectionState(ConnectionState::kConnected);

            return res;
         }

         int do_sendAuthenticate(const QString& username, const QString& password)
         {
            // TODO stub
            (void)username;
            (void)password;

            int res = do_determine_is_connected_();
            if (!res)
               return res;

            netmsg::Generic msgAuthenticate;

            // TODO stub
            //netMsg.appendU32(netmsg::Kind::AuthResponseKind);
            //netMsg.appendDw();
            msgAuthenticate.setId(netmsg::NetMsgKind::kAuthResponseKind);
            my_socket_.write(msgAuthenticate);

            return res;
         }

         int do_route_received_netmsg_(void)
         {
            auto res = 0;
            auto kind = my_netmsg_.id();
            auto size = my_netmsg_.size();

            if (size < netmsg::kMinimumSizeInBytes)
            {
               my_logger_.debug("Received NetMsg too small!");
               qCDebug(log_seville) << "Received NetMsg that is too small to interpret";
               qCDebug(log_seville) << "kind: " << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "size expected: " << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "reference code: " << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
               return -1;
            }
            else if (netmsg::kMaximumSizeInBytes < size)
            {
               my_logger_.debug("Received NetMsg too large!");
               qCDebug(log_seville) << "Received NetMsg that is too large to interpret";
               qCDebug(log_seville) << "kind: " << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "size expected: " << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "reference code: " << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
               return -2;
            }

            //u32 zNetMsgId = doCsbolecDw_(netMsgId);
            //switch (id) {
               //case Net::Msg::LogonKind:
               //    res = sendLogon_();
               //    break;
            if (netmsg::NetMsgKind::kAltLogonKind == kind)
                //netmsg::sizes[netmsg::AltLogonKind] == size) {
            {
               qCDebug(log_seville) << "Received AltLogon";
               my_logger_.debug("Received AltLogon");
               if (netmsg::kSizeOfLogonInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receive_altlogon_();
            }
            else if (netmsg::NetMsgKind::kConnectionErrorKind == kind)

            {
               qCDebug(log_seville) << "Received ConnectionError";
               my_logger_.debug("Received ConnectionError");
               if (netmsg::kSizeOfConnectionErrorInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               //auto connectionError = dynamic_cast<const netmsg::ConnectionError&>(myNetMsg);

               res = do_receiveConnectionError();
            }
            else if (netmsg::NetMsgKind::kServerVersionKind == kind)
            {
               qCDebug(log_seville) << "Received ServerVersion";
               my_logger_.debug("Received ServerVersion");
               if (netmsg::kSizeOfServerVersionInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveServerVersion();
            }
            else if (netmsg::NetMsgKind::kServerInfoKind == kind)
            {
               qCDebug(log_seville) << "Received ServerInfo";
               my_logger_.debug("Received ServerInfo");
               if (netmsg::kSizeOfServerInfoInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveServerInfo();
            }
            else if (netmsg::NetMsgKind::kUserStatusKind == kind)
            {
               qCDebug(log_seville) << "Received UserStatus";
               my_logger_.debug("Received UserStatus");
               if (netmsg::kSizeOfUserStatusInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveUserStatus();
            }
            else if (netmsg::NetMsgKind::kUserLoggedOnAndMaxKind == kind)
            {
               qCDebug(log_seville) << "Received UserLoggedOnAndMax";
               my_logger_.debug("Received UserLoggedOnAndMax");
               if (netmsg::kSizeOfUserLoggedOnAndMaxInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveUserLoggedOnAndMax();
            }
            else if (netmsg::NetMsgKind::kHttpServerLocationKind == kind)
            {
               qCDebug(log_seville) << "Received HttpServerLocation";
               my_logger_.debug("Received HttpServerLocation");
               if (size < netmsg::kSizeOfHttpServerLocationInBytes)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveHttpServerLocation();
            }
            else if (netmsg::NetMsgKind::kRoomUserListKind == kind)
            {
               qCDebug(log_seville) << "Received RoomUserList";
               my_logger_.debug("Received RoomUserList");
               if (size < netmsg::kSizeOfRoomUserListInBytes)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveRoomUserList();
            }
            else if (netmsg::NetMsgKind::kServerUserListKind == kind)
            {
               qCDebug(log_seville) << "Received ServerUserList";
               my_logger_.debug("Received ServerUserList");
               if (size < netmsg::kSizeOfServerUserListInBytes)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveServerUserList();
            }
            else if (netmsg::NetMsgKind::kServerRoomListKind == kind)
            {
               qCDebug(log_seville) << "Received ServerRoomList";
               my_logger_.debug("Received ServerRoomList");
               if (size < netmsg::kSizeOfServerRoomListInBytes)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveServerRoomList();
            }
            else if (netmsg::NetMsgKind::kRoomDescendKind == kind)
            {
               qCDebug(log_seville) << "Received RoomDescend";
               my_logger_.debug("Received RoomDescend");
               if (size < netmsg::kSizeOfRoomDescendedInBytes)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveRoomDescend();
            }
            else if (netmsg::NetMsgKind::kUserNewKind == kind)
            {
               qCDebug(log_seville) << "Received UserNew";
               my_logger_.debug("Received UserNew");
               if (netmsg::kSizeOfUserNewInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserNew();
            }
            else if (netmsg::NetMsgKind::kPingKind == kind)
            {
               qCDebug(log_seville) << "Received Ping";
               my_logger_.debug("Received Ping");
               if (netmsg::kSizeOfPingInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receivePing();
            }
            else if (netmsg::NetMsgKind::kPongKind == kind)
            {
               qCDebug(log_seville) << "Received Pong";
               my_logger_.debug("Received Pong");
               if (netmsg::kSizeOfPongInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receivePong();
            }
            else if (netmsg::NetMsgKind::kXTalkKind == kind)
            {
               qCDebug(log_seville) << "Received XTalk";
               my_logger_.debug("Received XTalk");
               if (size < netmsg::kSizeOfXTalkInBytes)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveXTalk();
            }
            else if (netmsg::NetMsgKind::kXWhisperKind == kind)
            {
               qCDebug(log_seville) << "Received XWhisper";
               my_logger_.debug("Received XWhisper");
               if (size < netmsg::kSizeOfXWhisperInBytes)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveXWhisper();
            }
            else if (netmsg::NetMsgKind::kTalkKind == kind)
            {
               qCDebug(log_seville) << "Received Talk";
               my_logger_.debug("Received Talk");
               if (size < netmsg::kSizeOfTalkInBytes)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveTalk();
            }
            else if (netmsg::NetMsgKind::kWhisperKind == kind)

            {
               qCDebug(log_seville) << "Recieved Whisper";
               my_logger_.debug("Received Whisper");
               if (size < netmsg::kSizeOfWhisperInBytes)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveWhisper();
            }
            else if (netmsg::NetMsgKind::kAssetIncomingKind == kind)
            {
               qCDebug(log_seville) << "Received AssetIncoming";
               my_logger_.debug("Received AssetIncoming");
               if (netmsg::kSizeOfAssetIncomingInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveAssetIncoming();
            }
            else if (netmsg::NetMsgKind::kAssetQueryKind == kind)
            {
               qCDebug(log_seville) << "Received AssetQuery";
               my_logger_.debug("Received AssetQuery");
               if (netmsg::kSizeOfAssetQueryInBytes != size)
               {
                  my_logger_.debug("(but size did not match!");
               }

               res = do_receiveAssetQuery();
            }
            else if (netmsg::NetMsgKind::kMovementKind == kind)
            {
               qCDebug(log_seville) << "Received Movement";
               my_logger_.debug("Received Movement");
               if (netmsg::kSizeOfMovementInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveMovement();
            }
            else if (netmsg::NetMsgKind::kUserColorKind == kind)
            {
               qCDebug(log_seville) << "Received UserColor";
               my_logger_.debug("Received UserColor");
               if (netmsg::kSizeOfUserColorInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserColor();
            }
            else if (netmsg::NetMsgKind::kUserFaceKind == kind)
            {
               qCDebug(log_seville) << "Received UserFace";
               my_logger_.debug("Received UserFace");
               if (netmsg::kSizeOfUserFaceInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserFace();
            }
            else if (netmsg::NetMsgKind::kUserPropKind == kind)
            {
               qCDebug(log_seville) << "Received UserProp";
               my_logger_.debug("Received UserProp");
               if (netmsg::kSizeOfUserPropInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserProp();
            }
            else if (netmsg::NetMsgKind::kUserDescriptionKind == kind)
            {
               qCDebug(log_seville) << "Received UserDescription";
               my_logger_.debug("Received UserDescription");
               if (netmsg::kSizeOfUserDescriptionInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserDescription();
            }
            else if (netmsg::NetMsgKind::kUserRenameKind == kind)
            {
               qCDebug(log_seville) << "Received UserRename";
               my_logger_.debug("Received UserRename");
               if (netmsg::kSizeOfUserRenameInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserRename();
            }
            else if (netmsg::NetMsgKind::kUserLeavingKind == kind)
            {
               qCDebug(log_seville) << "Received UserLeaving";
               my_logger_.debug("Received UserLeaving");
               if (netmsg::kSizeOfUserLeavingInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserLeaving();
            }
            else if (netmsg::NetMsgKind::kUserExitRoomKind == kind)
            {
               qCDebug(log_seville) << "Received UserExitRoom";
               my_logger_.debug("Received UserExitRoom");
               if (netmsg::kSizeOfUserExitRoomInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receiveUserExitRoom();
            }
            else if (netmsg::NetMsgKind::kPropMoveKind == kind)
            {
               qCDebug(log_seville) << "Received PropMove";
               my_logger_.debug("Received PropMove");
               if (netmsg::kSizeOfPropMoveInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receivePropMove();
            }
            else if (netmsg::NetMsgKind::kPropDeleteKind == kind)
            {
               qCDebug(log_seville) << "Received PropDelete";
               my_logger_.debug("Received PropDelete");
               if (netmsg::kSizeOfPropDeleteInBytes != size)
               {
                  my_logger_.debug("(but size did not match!)");
               }

               res = do_receivePropDelete();
            }
            else if (netmsg::NetMsgKind::kPropNewKind == kind)
            {
               qCDebug(log_seville) << "Received PropNew";
               my_logger_.debug("Received PropNew");
               if (netmsg::kSizeOfPropNewInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receivePropNew();
            }
            else if (netmsg::NetMsgKind::kDoorLockKind == kind)
            {
               qCDebug(log_seville) << "Received DoorLock";
               my_logger_.debug("Received DoorLock");
               if (netmsg::kSizeOfDoorLockInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveDoorLock();
            }
            else if (netmsg::NetMsgKind::kDoorUnlockKind == kind)
            {
               my_logger_.debug("Received DoorUnlock");
               qCDebug(log_seville) << "Received DoorUnlock";
               if (netmsg::kSizeOfDoorUnlockInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveDoorUnlock();
            }
            else if (netmsg::NetMsgKind::kPictMoveKind == kind)
            {
               my_logger_.debug("Received PictMove");
               qCDebug(log_seville) << "Received PictMove";
               if (netmsg::kSizeOfPictMoveInBytes == size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receivePictMove();
            }
            else if (netmsg::NetMsgKind::kSpotStateKind == kind)
            {
               my_logger_.debug("Received SpotState");
               qCDebug(log_seville) << "Received SpotState";
               if (netmsg::kSizeOfSpotStateInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveSpotState();
            }
            else if (netmsg::NetMsgKind::kSpotMoveKind == kind)
            {
               my_logger_.debug("Received SpotMove");
               qCDebug(log_seville) << "Received SpotMove";
               if (netmsg::kSizeOfSpotMoveInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveSpotMove();
            }
            else if (netmsg::NetMsgKind::kDrawKind == kind)
            {
               my_logger_.debug("Received Draw");
               qCDebug(log_seville) << "Received Draw";
               if (netmsg::kSizeOfDrawInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveDraw();
            }
            else if (netmsg::NetMsgKind::kNavErrorKind == kind)
            {
               my_logger_.debug("Received NavError");
               qCDebug(log_seville) << "Received NavError";
               if (netmsg::kSizeOfNavErrorInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveNavigationError();
            }
            else if (netmsg::NetMsgKind::kBlowThruKind == kind)
            {
               my_logger_.debug("Received BlowThru");
               qCDebug(log_seville) << "Received BlowThru";
               if (netmsg::kSizeOfBlowThruInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveBlowThru();
            }
            else if (netmsg::NetMsgKind::kAuthenticateKind == kind)
            {
               my_logger_.debug("Received Authenticate");
               qCDebug(log_seville) << "Received Authenticate";
               if (netmsg::kSizeOfAuthenticateInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveAuthenticate();
            }
            else if (netmsg::NetMsgKind::kAltRoomDescriptionKind == kind)
            {
               my_logger_.debug("Received AltRoomDescription");
               qCDebug(log_seville) << "Received AltRoomDescription";
               if (netmsg::kSizeOfAltRoomDescriptionInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveRoomDescription();
            }
            else if (netmsg::NetMsgKind::kRoomDescriptionKind == kind)
            {
               my_logger_.debug("Received RoomDescription");
               qCDebug(log_seville) << "Received RoomDescription";
               if (netmsg::kSizeOfRoomDescriptionInBytes != size)
                  my_logger_.debug("(but size did not match!)");

               res = do_receiveRoomDescription();
            }
            else
            {
               my_logger_.debug(QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }").arg(my_netmsg_.id(), 2, 16, QChar('0')).arg(my_netmsg_.len(), 2, 16, QChar('0')).arg(my_netmsg_.ref(), 2, 16, QChar('0')));
               qCDebug(log_seville) << "Received unknown NetMsg";
               qCDebug(log_seville) << "kind:  " << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "expected size: " << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
               qCDebug(log_seville) << "reference code: " << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
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
      };
   }
}

#endif   // SEVILLE_PALACE_CLIENT_H_
