#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

//#include "seville/base/log.h"
#include "seville/palace/base/host.h"
#include "seville/palace/base/client.h"
#include "seville/palace/base/server.h"
#include "seville/palace/base/room.h"
#include "seville/palace/base/user.h"
#include "seville/palace/base/netmsg.h"

#include "seville/palace/netmsg/logon.h"
#include "seville/palace/netmsg/roomdescription.h"
#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      const char* Client::kIdent = "PC4237";

      Client::Client(QObject* object_parent_ptr)
         : QObject(object_parent_ptr)
      {
         do_init_();
      }

      Client::~Client(void)
      {
         do_deinit_();
      }

      auto Client::do_setup_events_(void) -> void
      {
         // wire up events
         //connect(&mySocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
         //connect(&mySocket, SIGNAL())
         connect(&my_socket_, &QIODevice::readyRead,
                 this, [this](void) {  //&Client::on_readyRead);
               do_read_socket_();
            }
         );

         connect(
            &my_socket_, QOverload<QAbstractSocket::SocketError>::of(
                     &QAbstractSocket::error),
                  this, [this](void) {  //&Client::on_socketError
               my_logger_.error("Socket Error");
               qCDebug(log_seville) << "ERROR: Socket Error";
            }
         );

//            connect(
//                  &my_socket_,
//                  &QTcpSocket::errorOccurred,
//                  this,
//                     [this](QAbstractSocket::SocketError socket_error) {
//               my_logger_.error("Socket error");
//               qCDebug(log_seville) << "ERROR: Socket Error";
//            });

         connect(
            &my_network_access_manager_, &QNetworkAccessManager::finished,
            this, [this](QNetworkReply* reply_ptr) {
               if (reply_ptr->error()) {
                  my_logger_.error(reply_ptr->errorString());
               } else {
                  auto q_byte_array = reply_ptr->readAll();
                  auto const byte_array_ptr =
                        reinterpret_cast<ByteArray*>(&q_byte_array);
                  my_current_room_.set_background_image_bytes(
                           *byte_array_ptr);

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
      auto Client::do_fetch_background_async_(const QString& url) -> void
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

         my_network_access_manager_.get(request);
      }

      auto Client::do_reset_(void) -> void
      {
         my_transfer_timer_id_ = 0;
         my_socket_.disconnectFromHost();
         do_determine_client_byte_order_();
         my_connection_state_ = ClientState::kDisconnectedClientState;
         //myLog.resetState();
         my_server_.reset();
         my_user_.reset();
         my_current_room_.reset();
         my_netmsg_.reset();
      }

      auto Client::do_reset_receive_timer_(void) -> void
      {
         if (my_transfer_timer_id_)
         {
            killTimer(my_transfer_timer_id_);
            my_transfer_timer_id_ = 0;
         }
      }

      auto Client::do_read_socket_(void) -> void
      {
         //netmsg::Generic& netMsg = myNetMsg;

         if (my_transfer_timer_id_) {
            killTimer(my_transfer_timer_id_);
            my_transfer_timer_id_ = 0;
         }

         while (my_netmsg_.read_in_netmsg_from_socket_ptr(&my_socket_)) {
            if (my_netmsg_.to_byte_array().size() < NetMsgSize::kMinimumSize ||
                my_netmsg_.to_byte_array().size() < my_netmsg_.len()) {
               my_transfer_timer_id_ =
                     startTimer(kIntervalTimeoutForTransferInMs);
               continue;
            }

            if (ClientState::kHandshakingClientState == my_connection_state_) {
               if (do_determine_server_byte_order_() < 0) { //myNetMsg) < 0)
                  qCDebug(log_seville) <<
                        "ERROR: Could not determine server byte order!";
                  return;
               }

               u32 user_id = my_netmsg_.ref();
               my_user_.set_id(user_id);

               if (do_send_logon_() < 0) {
                  qCDebug(log_seville) << "ERROR: Could not send Logon!";
                  return;
               }

               my_connection_state_ = ClientState::kConnectedClientState;
               qCDebug(log_seville) << "Client is now in Connected State";
            } else if (ClientState::kConnectedClientState ==
                     my_connection_state_) {
               do_route_received_netmsg_();
            }

            //qCDebug(log_seville) << "Resetting NetMsg";
            my_netmsg_.reset();
         }

         my_ping_timer_.start();
         my_pong_time_.start();
      }

      auto Client::do_set_connection_state_(ClientState client_state) -> void
      {
         my_connection_state_ = client_state;
         // TODO signal?
         //do_updateMenus();
      }

      auto Client::do_connect_to_host_(
            QString host, int port, QString username, int initial_room) -> void
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

         my_user_.set_username(username);
         my_server_.set_host(host);
         my_server_.set_port(actual_host_tcp_port);
         my_current_room_.set_id(actual_initial_room);

         qCDebug(log_seville)
               << "Client Connection is now in Handshaking State";
         my_connection_state_ = ClientState::kHandshakingClientState;
         my_socket_.connectToHost(my_server_.host(), my_server_.port());
      }

      auto Client::do_disconnect_from_host_(void) -> void
      {
         if (my_connection_state_ != ClientState::kDisconnectedClientState) {
            my_logger_.info("Disconnected.");
            my_socket_.disconnectFromHost();
            my_connection_state_ = ClientState::kDisconnectedClientState;
         }
      }

      auto Client::do_determine_client_byte_order_(void) -> void
      {
         // TODO
         auto is_little_endian =
               (Host::kLittleEndian * !Client::is_big_endian());
         auto is_big_endian =
               (Host::kBigEndian * Client::is_big_endian());
         my_byte_order_ = static_cast<Host::ByteOrder>(
                  is_little_endian | is_big_endian);
      }

      auto Client::do_determine_server_byte_order_(void) -> int
      {
         auto result = 0;
         // TODO more consideration here.
         // constants are in big endian byte order,
         // conditionally swapping them if we are a little endian system
         // seems off.
         // but shouldn't we have to swap them all? we don't.

         auto flag_little_endian = static_cast<u32>(
                  Host::kLittleEndian == my_byte_order_);
         auto kind = (flag_little_endian * Host::SwapU32(my_netmsg_.id())) |
                     (!flag_little_endian * my_netmsg_.id());

         if (NetMsgKind::kUnknownServerKind == kind) {
            qCDebug(log_seville) << "Server has Unknown Byte Order";
            my_server_.set_byte_order(Host::ByteOrder::kUnknownEndian);
            result = 0;
         } else if (NetMsgKind::kBigEndianServerKind == kind) {
            qCDebug(log_seville) << "Server has Big Endian Byte Order";
            my_server_.set_byte_order(Host::ByteOrder::kBigEndian);
            result = 1;
         } else if (NetMsgKind::kLittleEndianServerKind == kind) {
            qCDebug(log_seville) << "Server has Little Endian Byte Order";
            my_server_.set_byte_order(Host::ByteOrder::kLittleEndian);
            result = 1;
         } else {
            qCDebug(log_seville) << "Server did not indicate its Byte Order";
            my_server_.set_byte_order(Host::ByteOrder::kUnknownEndian);
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

      auto Client::do_determine_is_connected_(void) const -> bool {
         return my_socket_.state() != QTcpSocket::ConnectedState ||
               my_connection_state_ != ClientState::kConnectedClientState;
      }

      auto Client::do_determine_should_swap_endianness_(void) const ->
         NetMsgOptions
      {
         auto not_unknown_client_byteorder =
               my_byte_order_ != Host::ByteOrder::kUnknownEndian;

         auto not_unknown_server_byteorder =
               my_server_.byteorder() != Host::ByteOrder::kUnknownEndian;

         auto not_same_byteorder = my_byte_order_ != my_server_.byteorder();

         auto should_swap = not_unknown_client_byteorder &&
               not_unknown_server_byteorder && not_same_byteorder;

         //return NetMsgOptions::kEndianSwap * should_swap |
         //      NetMsgOptions::kNoEndianSwap * !should_swap;
         if (should_swap) {
            return NetMsgOptions::kEndianSwap;
         } else {
            return NetMsgOptions::kNoEndianSwap;
         }
      }

      /**
       * @brief do_receiveAltLogon
       * @return number of network messages handled
       */
      auto Client::do_receive_altlogon_(void) -> int
      {
         auto netmsg_logon = static_cast<netmsg::NetMsgLogon>(my_netmsg_);
         if (my_user_.id_counter() != netmsg_logon.puid_counter() ||
             my_user_.id_crc() != netmsg_logon.puid_crc())
         {
            my_user_.set_id_crc(netmsg_logon.puid_crc());
            my_user_.set_id_counter(netmsg_logon.puid_counter());
            my_user_.set_id_changed(true);
         }

         return 1;
      }

      auto Client::do_receive_alt_room_description_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_asset_incoming_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_asset_query_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_authenticate_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_blowthru_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_connection_error_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_door_lock_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_door_unlock_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_draw_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_http_server_location_(void) -> int
      {
         auto result = 0;

         auto netmsg_http_server_location =
               static_cast<const netmsg::HttpServerLocation&>(my_netmsg_);
         auto url = netmsg_http_server_location.url();
         my_server_.set_http_server_location(url);

         my_logger_.debug(QString("HTTP Server is %1").arg(url));

         result = 1;
         return result;
      }

      auto Client::do_receive_movement_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_navigation_error_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_picture_move_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_ping_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_pong_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_prop_delete_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_prop_move_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_prop_new_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_room_descend_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_room_description_(void) -> int
      {
         auto result = 0;

         auto netmsg_room_description =
               static_cast<const netmsg::RoomDescription&>(my_netmsg_);

         my_current_room_.set_from_room_description(netmsg_room_description);

         do_fetch_background_async_(
                  my_server_.http_server_location() + "/" +
                  my_current_room_.background_image_name());

         result = 1;
         return result;
      }

      auto Client::do_receive_room_user_list_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_server_version_(void) -> int
      {
         auto result = 0;
         auto netmsg_server_version =
               static_cast<NetMsg>(my_netmsg_);
         my_server_.set_version(netmsg_server_version.ref());

         return result;
      }

      auto Client::do_receive_server_info_(void) -> int
      {
         auto result = 0;
         // stub

         return result;
      }

      auto Client::do_receive_user_new_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_color_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_exit_room_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_face_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_prop_(void) -> int
      {
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_description_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_rename_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_leaving_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_logged_on_and_max_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_user_status_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_server_room_list_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_server_user_list_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_spot_move_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_spot_state_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_talk_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_whisper_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_xtalk_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_xwhisper_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_send_logon_(void) -> int
      {
         auto result = 0;
         netmsg::NetMsgLogon netmsg_logon(
                  do_determine_should_swap_endianness_());
         netmsg_logon.set_reg_crc(my_user_.regCrc());
         netmsg_logon.set_reg_counter(my_user_.regCounter());
         netmsg_logon.set_username(my_user_.username());
         netmsg_logon.set_wizard_password(my_user_.wizpass());

         netmsg_logon.set_flags(
                  static_cast<u32>(FlagAuxOptions::kAuthenticate) |
                  static_cast<u32>(FlagAuxOptions::kWin32));

         netmsg_logon.set_puid_counter(my_puid_counter_);
         netmsg_logon.set_puid_crc(my_puid_crc_);
         netmsg_logon.set_demo_elapsed(kMagicFromPChat);
         netmsg_logon.set_total_elapsed(kMagicFromPChat);
         netmsg_logon.set_demo_limit(kMagicFromPChat);
         netmsg_logon.set_initial_room_id(my_current_room_.id());
         netmsg_logon.set_reserved(kIdent);
         netmsg_logon.set_upload_requested_protocol_version(0);

         netmsg_logon.set_upload_capabilities(
                  static_cast<u32>(UploadCapabilities::kPalaceAssetUpload) |
                  static_cast<u32>(UploadCapabilities::kHttpFileUpload));

         netmsg_logon.set_download_capabilities(
                  static_cast<u32>(
                     DownloadCapabilities::kPalaceAssetDownload) |
                  static_cast<u32>(
                     DownloadCapabilities::kPalaceFileDownload) |
                  static_cast<u32>(
                     DownloadCapabilities::kHttpFileDownload) |
                  static_cast<u32>(
                     DownloadCapabilities::kHttpFileExtendedDownload));

         netmsg_logon.set_engine_capabilities_2d(
                  static_cast<u32>(EngineCapabilities2d::kPalace2dEngine));
         netmsg_logon.set_graphics_capabilities_2d(
                  static_cast<u32>(GraphicsCapabilities2d::kGif87));
         netmsg_logon.set_graphics_capabilities_3d(
                  static_cast<u32>(GraphicsCapabilities3d::kNo3dGraphics));

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

         my_socket_.write(netmsg_logon.to_byte_array());
         result = my_socket_.flush();
         do_set_connection_state_(ClientState::kConnectedClientState);

         return result;
      }

      auto Client::do_send_authenticate_(
            const QString& username, const QString& password) -> int
      {
         // TODO stub
         (void)username;
         (void)password;

         auto result = int{do_determine_is_connected_()};
         if (!result)
            return result;

         NetMsg netmsg_authenticate;

         // TODO stub
         //netMsg.appendU32(netmsg::Kind::AuthResponseKind);
         //netMsg.appendDw();
         netmsg_authenticate.set_id(NetMsgKind::kAuthResponseKind);
         my_socket_.write(netmsg_authenticate.to_byte_array());

         return result;
      }

      auto Client::do_route_received_netmsg_(void) -> int
      {
         auto result = 0;
         auto kind = my_netmsg_.id();
         auto size = my_netmsg_.to_byte_array().size();

         if (size < NetMsgSize::kMinimumSize) {
            my_logger_.debug("Received NetMsg too small!");
            qCDebug(log_seville)
                  << "Received NetMsg that is too small to interpret";
            qCDebug(log_seville)
                  << "kind: "
                  << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "size expected: "
                  << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "reference code: "
                  << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
            return -1;
         } else if (NetMsgSize::kMaximumSize < size) {
            my_logger_.debug("Received NetMsg too large!");
            qCDebug(log_seville)
                  << "Received NetMsg that is too large to interpret";
            qCDebug(log_seville)
                  << "kind: "
                  << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "size expected: "
                  << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "reference code: "
                  << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
            return -2;
         }

         //u32 zNetMsgId = doCsbolecDw_(netMsgId);
         //switch (id) {
            //case Net::Msg::LogonKind:
            //    res = sendLogon_();
            //    break;
         if (NetMsgKind::kAltLogonKind == kind) {
             //netmsg::sizes[netmsg::AltLogonKind] == size) {
            qCDebug(log_seville) << "Received AltLogon";
            my_logger_.debug("Received AltLogon");
            if (NetMsgSize::kLogonSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_altlogon_();
         } else if (NetMsgKind::kConnectionErrorKind == kind) {
            qCDebug(log_seville) << "Received ConnectionError";
            my_logger_.debug("Received ConnectionError");
            if (NetMsgSize::kConnectionErrorSize != size)
               my_logger_.debug("(but size did not match!)");
            //auto connectionError =
            //dynamic_cast<const netmsg::ConnectionError&>(myNetMsg);
            result = do_receive_connection_error_();
         } else if (NetMsgKind::kServerVersionKind == kind) {
            qCDebug(log_seville) << "Received ServerVersion";
            my_logger_.debug("Received ServerVersion");
            if (NetMsgSize::kServerVersionSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_server_version_();
         } else if (NetMsgKind::kServerInfoKind == kind) {
            qCDebug(log_seville) << "Received ServerInfo";
            my_logger_.debug("Received ServerInfo");
            if (NetMsgSize::kServerInfoSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_server_info_();
         } else if (NetMsgKind::kUserStatusKind == kind) {
            qCDebug(log_seville) << "Received UserStatus";
            my_logger_.debug("Received UserStatus");
            if (NetMsgSize::kUserStatusSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_status_();
         } else if (NetMsgKind::kUserLoggedOnAndMaxKind == kind) {
            qCDebug(log_seville) << "Received UserLoggedOnAndMax";
            my_logger_.debug("Received UserLoggedOnAndMax");
            if (NetMsgSize::kUserLoggedOnAndMaxSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_logged_on_and_max_();
         } else if (NetMsgKind::kHttpServerLocationKind == kind) {
            qCDebug(log_seville) << "Received HttpServerLocation";
            my_logger_.debug("Received HttpServerLocation");
            if (size < NetMsgSize::kHttpServerLocationSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_http_server_location_();
         } else if (NetMsgKind::kRoomUserListKind == kind) {
            qCDebug(log_seville) << "Received RoomUserList";
            my_logger_.debug("Received RoomUserList");
            if (size < NetMsgSize::kRoomUserListSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_room_user_list_();
         } else if (NetMsgKind::kServerUserListKind == kind) {
            qCDebug(log_seville) << "Received ServerUserList";
            my_logger_.debug("Received ServerUserList");
            if (size < NetMsgSize::kServerUserListSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_server_user_list_();
         } else if (NetMsgKind::kServerRoomListKind == kind) {
            qCDebug(log_seville) << "Received ServerRoomList";
            my_logger_.debug("Received ServerRoomList");
            if (size < NetMsgSize::kServerRoomListSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_server_room_list_();
         } else if (NetMsgKind::kRoomDescendKind == kind) {
            qCDebug(log_seville) << "Received RoomDescend";
            my_logger_.debug("Received RoomDescend");
            if (size < NetMsgSize::kRoomDescendedSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_room_descend_();
         } else if (NetMsgKind::kUserNewKind == kind) {
            qCDebug(log_seville) << "Received UserNew";
            my_logger_.debug("Received UserNew");
            if (NetMsgSize::kUserNewSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_new_();
         } else if (NetMsgKind::kPingKind == kind) {
            qCDebug(log_seville) << "Received Ping";
            my_logger_.debug("Received Ping");
            if (NetMsgSize::kPingSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_ping_();
         } else if (NetMsgKind::kPongKind == kind) {
            qCDebug(log_seville) << "Received Pong";
            my_logger_.debug("Received Pong");
            if (NetMsgSize::kPongSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_pong_();
         } else if (NetMsgKind::kXTalkKind == kind) {
            qCDebug(log_seville) << "Received XTalk";
            my_logger_.debug("Received XTalk");
            if (size < NetMsgSize::kXTalkSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_xtalk_();
         } else if (NetMsgKind::kXWhisperKind == kind) {
            qCDebug(log_seville) << "Received XWhisper";
            my_logger_.debug("Received XWhisper");
            if (size < NetMsgSize::kXWhisperSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_xwhisper_();
         } else if (NetMsgKind::kTalkKind == kind) {
            qCDebug(log_seville) << "Received Talk";
            my_logger_.debug("Received Talk");
            if (size < NetMsgSize::kTalkSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_talk_();
         } else if (NetMsgKind::kWhisperKind == kind) {
            qCDebug(log_seville) << "Recieved Whisper";
            my_logger_.debug("Received Whisper");
            if (size < NetMsgSize::kWhisperSize)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_whisper_();
         } else if (NetMsgKind::kAssetIncomingKind == kind) {
            qCDebug(log_seville) << "Received AssetIncoming";
            my_logger_.debug("Received AssetIncoming");
            if (NetMsgSize::kAssetIncomingSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_asset_incoming_();
         } else if (NetMsgKind::kAssetQueryKind == kind) {
            qCDebug(log_seville) << "Received AssetQuery";
            my_logger_.debug("Received AssetQuery");
            if (NetMsgSize::kAssetQuerySize != size)
               my_logger_.debug("(but size did not match!");
            result = do_receive_asset_query_();
         } else if (NetMsgKind::kMovementKind == kind) {
            qCDebug(log_seville) << "Received Movement";
            my_logger_.debug("Received Movement");
            if (NetMsgSize::kMovementSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_movement_();
         } else if (NetMsgKind::kUserColorKind == kind) {
            qCDebug(log_seville) << "Received UserColor";
            my_logger_.debug("Received UserColor");
            if (NetMsgSize::kUserColorSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_color_();
         } else if (NetMsgKind::kUserFaceKind == kind) {
            qCDebug(log_seville) << "Received UserFace";
            my_logger_.debug("Received UserFace");
            if (NetMsgSize::kUserFaceSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_face_();
         } else if (NetMsgKind::kUserPropKind == kind) {
            qCDebug(log_seville) << "Received UserProp";
            my_logger_.debug("Received UserProp");
            if (NetMsgSize::kUserPropSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_prop_();
         } else if (NetMsgKind::kUserDescriptionKind == kind) {
            qCDebug(log_seville) << "Received UserDescription";
            my_logger_.debug("Received UserDescription");
            if (NetMsgSize::kUserDescriptionSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_description_();
         } else if (NetMsgKind::kUserRenameKind == kind) {
            qCDebug(log_seville) << "Received UserRename";
            my_logger_.debug("Received UserRename");
            if (NetMsgSize::kUserRenameSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_rename_();
         } else if (NetMsgKind::kUserLeavingKind == kind) {
            qCDebug(log_seville) << "Received UserLeaving";
            my_logger_.debug("Received UserLeaving");
            if (NetMsgSize::kUserLeavingSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_leaving_();
         } else if (NetMsgKind::kUserExitRoomKind == kind) {
            qCDebug(log_seville) << "Received UserExitRoom";
            my_logger_.debug("Received UserExitRoom");
            if (NetMsgSize::kUserExitRoomSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_user_exit_room_();
         } else if (NetMsgKind::kPropMoveKind == kind) {
            qCDebug(log_seville) << "Received PropMove";
            my_logger_.debug("Received PropMove");
            if (NetMsgSize::kPropMoveSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_move_();
         } else if (NetMsgKind::kPropDeleteKind == kind) {
            qCDebug(log_seville) << "Received PropDelete";
            my_logger_.debug("Received PropDelete");
            if (NetMsgSize::kPropDeleteSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_delete_();
         } else if (NetMsgKind::kPropNewKind == kind) {
            qCDebug(log_seville) << "Received PropNew";
            my_logger_.debug("Received PropNew");
            if (NetMsgSize::kPropNewSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_new_();
         } else if (NetMsgKind::kDoorLockKind == kind) {
            qCDebug(log_seville) << "Received DoorLock";
            my_logger_.debug("Received DoorLock");
            if (NetMsgSize::kDoorLockSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_door_lock_();
         } else if (NetMsgKind::kDoorUnlockKind == kind) {
            my_logger_.debug("Received DoorUnlock");
            qCDebug(log_seville) << "Received DoorUnlock";
            if (NetMsgSize::kDoorUnlockSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_door_unlock_();
         } else if (NetMsgKind::kPictMoveKind == kind) {
            my_logger_.debug("Received PictMove");
            qCDebug(log_seville) << "Received PictMove";
            if (NetMsgSize::kPictMoveSize == size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_picture_move_();
         } else if (NetMsgKind::kSpotStateKind == kind) {
            my_logger_.debug("Received SpotState");
            qCDebug(log_seville) << "Received SpotState";
            if (NetMsgSize::kSpotStateSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_spot_state_();
         } else if (NetMsgKind::kSpotMoveKind == kind) {
            my_logger_.debug("Received SpotMove");
            qCDebug(log_seville) << "Received SpotMove";
            if (NetMsgSize::kSpotMoveSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_spot_move_();
         } else if (NetMsgKind::kDrawKind == kind) {
            my_logger_.debug("Received Draw");
            qCDebug(log_seville) << "Received Draw";
            if (NetMsgSize::kDrawSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_draw_();
         } else if (NetMsgKind::kNavErrorKind == kind) {
            my_logger_.debug("Received NavError");
            qCDebug(log_seville) << "Received NavError";
            if (NetMsgSize::kNavErrorSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_navigation_error_();
         } else if (NetMsgKind::kBlowThruKind == kind) {
            my_logger_.debug("Received BlowThru");
            qCDebug(log_seville) << "Received BlowThru";
            if (NetMsgSize::kBlowThruSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_blowthru_();
         } else if (NetMsgKind::kAuthenticateKind == kind) {
            my_logger_.debug("Received Authenticate");
            qCDebug(log_seville) << "Received Authenticate";
            if (NetMsgSize::kAuthenticateSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_authenticate_();
         } else if (NetMsgKind::kAltRoomDescriptionKind == kind) {
            my_logger_.debug("Received AltRoomDescription");
            qCDebug(log_seville) << "Received AltRoomDescription";
            if (NetMsgSize::kAltRoomDescriptionSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_room_description_();
         } else if (NetMsgKind::kRoomDescriptionKind == kind) {
            my_logger_.debug("Received RoomDescription");
            qCDebug(log_seville) << "Received RoomDescription";
            if (NetMsgSize::kRoomDescriptionSize != size)
               my_logger_.debug("(but size did not match!)");
            result = do_receive_room_description_();
         } else {
            my_logger_.debug(
                     QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }")
                     .arg(my_netmsg_.id(), 2, 16, QChar('0'))
                     .arg(my_netmsg_.len(), 2, 16, QChar('0'))
                     .arg(my_netmsg_.ref(), 2, 16, QChar('0')));
            qCDebug(log_seville) << "Received unknown NetMsg";
            qCDebug(log_seville)
                  << "kind:  "
                  << QString("0x%1").arg(my_netmsg_.id(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "expected size: "
                  << QString("0x%1").arg(my_netmsg_.len(), 2, 16, QChar('0'));
            qCDebug(log_seville)
                  << "reference code: "
                  << QString("0x%1").arg(my_netmsg_.ref(), 2, 16, QChar('0'));
            result = 0;
         }

         return result;
      }

      auto Client::do_deinit_(void) -> void
      {
         do_disconnect_from_host_();
      }

      auto Client::do_init_(void) -> void
      {
         do_setup_events_();
         do_reset_();
         my_logger_.set_mode(Logger::kDebugMode);
         //my_logger_.setIsDebugMode(true);
      }
   }
}
