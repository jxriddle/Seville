#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTcpSocket>

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
#include "seville/palace/netmsg/talk.h"

namespace seville
{
   namespace palace
   {
      const char* Client::kIdent = "PC4237";

      Client::Client(QObject* parent_object_ptr)
         : QObject(parent_object_ptr)
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
         do_set_connection_state_(ClientState::kDisconnectedClientState);
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
            if (my_netmsg_.size() < NetMsgSize::kMinimumSize ||
                my_netmsg_.size() < my_netmsg_.len()) {
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

               do_set_connection_state_(ClientState::kConnectedClientState);
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
         //do_update_menus_();

         emit connection_state_did_change();
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

         auto logMsg = QString("Connecting to palace://%1@%2:%3/%4")
               .arg(username.toUtf8().data())
               .arg(host.toUtf8().data())
               .arg(actual_host_tcp_port)
               .arg(actual_initial_room);

         qCDebug(log_seville, "Connecting to palace://%s@%s:%d/%d",
                 username.toUtf8().data(),
                 host.toUtf8().data(),
                 actual_host_tcp_port, actual_initial_room);

         my_logger_.info(logMsg);

         my_user_.set_username(username);
         my_server_.set_host(host);
         my_server_.set_port(actual_host_tcp_port);
         my_current_room_.set_room_id(actual_initial_room);

         qCDebug(log_seville)
               << "Client Connection is now in Handshaking State";
         do_set_connection_state_(ClientState::kHandshakingClientState);
         my_socket_.connectToHost(my_server_.host(), my_server_.port());
      }

      auto Client::do_disconnect_from_host_(void) -> void
      {
         if (my_connection_state_ != ClientState::kDisconnectedClientState) {
            my_logger_.info("Disconnected.");
            my_socket_.disconnectFromHost();
            do_set_connection_state_(ClientState::kDisconnectedClientState);
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
         //auto netmsg_logon = static_cast<netmsg::NetMsgLogon>(my_netmsg_);
         auto puid_counter =
               my_netmsg_.u32_at(NetMsgOffset::kPayloadOffset+76);
         auto puid_crc =
               my_netmsg_.u32_at(NetMsgOffset::kPayloadOffset+80);
         if (my_user_.id_counter() != puid_counter ||
             my_user_.id_crc() != puid_crc)
         {
            my_user_.set_id_crc(puid_crc);
            my_user_.set_id_counter(puid_counter);
            my_user_.set_id_changed(true);
         }

         return 0;
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
         auto url = my_netmsg_.qstring_at(NetMsgOffset::kPayloadOffset, 256);
         my_server_.set_http_server_location(url);
         my_logger_.debug(QString("HTTP Server is %1").arg(url));
         return 1;
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
         auto pong_msg = palace::NetMsg();
         pong_msg.set_id(NetMsgKind::kPongKind);
         pong_msg.set_len(0);
         //pong_msg.set_ref(my_user_.id());
         pong_msg.set_ref(0);
         my_socket_.write(pong_msg);
         my_socket_.flush();
         my_logger_.debug(QString("Sent Pong"));
         return 0;
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
         auto data_offset = 40;

         auto flags = my_netmsg_.u32_at(NetMsgOffset::kPayloadOffset);
         my_current_room_.set_flags(flags);

         auto face = my_netmsg_.u32_at(NetMsgOffset::kPayloadOffset+4);
         my_current_room_.set_face(face);

         auto room_id = my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+8);
         my_current_room_.set_room_id(room_id);

         auto room_name_offset =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+10);

         auto background_image_name_offset =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+14);

         auto artist_name_offset =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+16);

         auto password_offset =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+18);

         auto hotspot_count =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+20);
         my_current_room_.set_hotspot_count(hotspot_count);

         auto hotspot_offset =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+22);

         auto image_count = my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+24);
         my_current_room_.set_image_count(image_count);

         auto loose_prop_count =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+26);
         my_current_room_.set_loose_prop_count(loose_prop_count);

         auto draw_commands_count =
               my_netmsg_.u16_at(NetMsgOffset::kPayloadOffset+28);
         my_current_room_.set_draw_commands_count(draw_commands_count);

         auto room_name = my_netmsg_.pascal_qstring_at(
                  data_offset + room_name_offset);
         my_current_room_.set_room_name(room_name);

         auto background_image_name = my_netmsg_.pascal_qstring_at(
                  data_offset + background_image_name_offset);
         my_current_room_.set_background_image_name(background_image_name);

         auto background_image_uri =
               my_server_.http_server_location() + "/" +
               my_current_room_.background_image_name();

         my_logger_.debug(QString("Fetching background from %1")
                          .arg(background_image_uri));

         if (0 < background_image_uri.length())
            do_fetch_background_async_(background_image_uri);

         return 0;
      }

      auto Client::do_receive_room_user_list_(void) -> int
      {
         // stub
         auto result = 0;
         return result;
      }

      auto Client::do_receive_server_version_(void) -> int
      {
         auto server_version = my_netmsg_.u32_at(NetMsgOffset::kRefOffset);
         my_server_.set_version(server_version);
         return 0;
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
         //auto msgChat = dynamic_cast<netmsg::Talk*>(&my_netmsg_);
         //msgChat->set_
         auto user_id = my_netmsg_.u32_at(NetMsgOffset::kRefOffset);
         auto len = my_netmsg_.u32_at(NetMsgOffset::kLenOffset);
         auto user_name = QString::number(user_id); //"<someone>"; //my_current_room_.userData(user_id);
         auto message =
               //my_netmsg_.pascal_qstring_at(NetMsgOffset::kPayloadOffset);
               my_netmsg_.qstring_at(NetMsgOffset::kPayloadOffset, len);
         my_logger_.chat(user_name, message);
         return 0;
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
         netmsg_logon.set_initial_room_id(my_current_room_.room_id());
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

         my_socket_.write(netmsg_logon);
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
         my_socket_.write(netmsg_authenticate);

         return result;
      }

      auto Client::do_route_received_netmsg_(void) -> int
      {
         auto result = 0;
         auto kind = my_netmsg_.id();
         auto size = my_netmsg_.size();

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
            auto message = QString("Received AltLogon (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kLogonSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_altlogon_();
         } else if (NetMsgKind::kConnectionErrorKind == kind) {
            auto message = QString("Received ConnectionError (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kConnectionErrorSize != size)
               //my_logger_.debug("(but size did not match!)");
            //auto connectionError =
            //dynamic_cast<const netmsg::ConnectionError&>(myNetMsg);
            result = do_receive_connection_error_();
         } else if (NetMsgKind::kServerVersionKind == kind) {
            auto message = QString("Received ServerVersion (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kServerVersionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_server_version_();
         } else if (NetMsgKind::kServerInfoKind == kind) {
            auto message = QString("Received ServerInfo (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kServerInfoSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_server_info_();
         } else if (NetMsgKind::kUserStatusKind == kind) {
            auto message = QString("Received UserStatus (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserStatusSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_status_();
         } else if (NetMsgKind::kUserLoggedOnAndMaxKind == kind) {
            auto message =
                  QString("Received UserLoggedOnAndMax (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserLoggedOnAndMaxSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_logged_on_and_max_();
         } else if (NetMsgKind::kHttpServerLocationKind == kind) {
            auto message =
                  QString("Received HttpServerLocation (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kHttpServerLocationSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_http_server_location_();
         } else if (NetMsgKind::kRoomUserListKind == kind) {
            auto message = QString("Received RoomUserList (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kRoomUserListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_room_user_list_();
         } else if (NetMsgKind::kServerUserListKind == kind) {
            auto message = QString("Received ServerUserList (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kServerUserListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_server_user_list_();
         } else if (NetMsgKind::kServerRoomListKind == kind) {
            auto message = QString("Received ServerRoomList (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kServerRoomListSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_server_room_list_();
         } else if (NetMsgKind::kRoomDescendKind == kind) {
            auto message = QString("Received RoomDescend (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kRoomDescendedSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_room_descend_();
         } else if (NetMsgKind::kUserNewKind == kind) {
            auto message = QString("Received UserNew (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserNewSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_new_();
         } else if (NetMsgKind::kPingKind == kind) {
            auto message = QString("Received Ping (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_ping_();
         } else if (NetMsgKind::kPongKind == kind) {
            auto message = QString("Received Pong (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPongSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_pong_();
         } else if (NetMsgKind::kXTalkKind == kind) {
            auto message = QString("Received XTalk (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kXTalkSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_xtalk_();
         } else if (NetMsgKind::kXWhisperKind == kind) {
            auto message = QString("Received XWhisper (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kXWhisperSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_xwhisper_();
         } else if (NetMsgKind::kTalkKind == kind) {
            auto message = QString("Received Talk (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kTalkSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_talk_();
         } else if (NetMsgKind::kWhisperKind == kind) {
            auto message = QString("Recieved Whisper (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (size < NetMsgSize::kWhisperSize)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_whisper_();
         } else if (NetMsgKind::kAssetIncomingKind == kind) {
            auto message = QString("Received AssetIncoming (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kAssetIncomingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_asset_incoming_();
         } else if (NetMsgKind::kAssetQueryKind == kind) {
            auto message = QString("Received AssetQuery (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kAssetQuerySize != size)
               my_logger_.debug("(but size did not match!");
            result = do_receive_asset_query_();
         } else if (NetMsgKind::kMovementKind == kind) {
            auto message = QString("Received Movement").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kMovementSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_movement_();
         } else if (NetMsgKind::kUserColorKind == kind) {
            auto message = QString("Received UserColor (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserColorSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_color_();
         } else if (NetMsgKind::kUserFaceKind == kind) {
            auto message = QString("Received UserFace (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserFaceSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_face_();
         } else if (NetMsgKind::kUserPropKind == kind) {
            auto message = QString("Received UserProp (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserPropSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_prop_();
         } else if (NetMsgKind::kUserDescriptionKind == kind) {
            auto message = QString("Received UserDescription (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_description_();
         } else if (NetMsgKind::kUserRenameKind == kind) {
            auto message = QString("Received UserRename (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserRenameSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_rename_();
         } else if (NetMsgKind::kUserLeavingKind == kind) {
            auto message = QString("Received UserLeaving (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserLeavingSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_leaving_();
         } else if (NetMsgKind::kUserExitRoomKind == kind) {
            auto message = QString("Received UserExitRoom (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kUserExitRoomSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_user_exit_room_();
         } else if (NetMsgKind::kPropMoveKind == kind) {
            auto message = QString("Received PropMove (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPropMoveSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_move_();
         } else if (NetMsgKind::kPropDeleteKind == kind) {
            auto message = QString("Received PropDelete (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPropDeleteSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_delete_();
         } else if (NetMsgKind::kPropNewKind == kind) {
            auto message = QString("Received PropNew (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPropNewSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_prop_new_();
         } else if (NetMsgKind::kDoorLockKind == kind) {
            auto message = QString("Received DoorLock (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kDoorLockSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_door_lock_();
         } else if (NetMsgKind::kDoorUnlockKind == kind) {
            auto message = QString("Received DoorUnlock (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kDoorUnlockSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_door_unlock_();
         } else if (NetMsgKind::kPictMoveKind == kind) {
            auto message = QString("Received PictMove (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kPictMoveSize == size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_picture_move_();
         } else if (NetMsgKind::kSpotStateKind == kind) {
            auto message = QString("Received SpotState (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kSpotStateSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_spot_state_();
         } else if (NetMsgKind::kSpotMoveKind == kind) {
            auto message = QString("Received SpotMove (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kSpotMoveSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_spot_move_();
         } else if (NetMsgKind::kDrawKind == kind) {
            auto message = QString("Received Draw (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kDrawSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_draw_();
         } else if (NetMsgKind::kNavErrorKind == kind) {
            auto message = QString("Received NavError (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kNavErrorSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_navigation_error_();
         } else if (NetMsgKind::kBlowThruKind == kind) {
            auto message = QString("Received BlowThru (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kBlowThruSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_blowthru_();
         } else if (NetMsgKind::kAuthenticateKind == kind) {
            auto message = QString("Received Authenticate (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kAuthenticateSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_authenticate_();
         } else if (NetMsgKind::kAltRoomDescriptionKind == kind) {
            auto message = QString("Received AltRoomDescription (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kAltRoomDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_room_description_();
         } else if (NetMsgKind::kRoomDescriptionKind == kind) {
            auto message = QString("Received RoomDescription (%1)").arg(size);
            qCDebug(log_seville) << message;
            my_logger_.debug(message);
            //if (NetMsgSize::kRoomDescriptionSize != size)
               //my_logger_.debug("(but size did not match!)");
            result = do_receive_room_description_();
         } else {
            auto message =
                  QString("Received Unknown NetMsg: { 0x%1, 0x%2, 0x%3 }")
                  .arg(my_netmsg_.id(), 2, 16, QChar('0'))
                  .arg(my_netmsg_.len(), 2, 16, QChar('0'))
                  .arg(my_netmsg_.ref(), 2, 16, QChar('0'));

            my_logger_.debug(message);
            qCDebug(log_seville) << "Received unknown NetMsg" << message;
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
