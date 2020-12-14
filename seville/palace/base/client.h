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
         static const int kMaxSizeOfUsernameSize = 31;
         static const int kMaxSizeOfWizpassSize = 31;
         static const int kSizeOfIdentSize = 6;
         static const int kDefaultSizeOfChunkReadSize = 100; /* netmsg::kHeaderSize; */
         static const int kMinSizeOfShortestChunkReadSize = 1;

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
         explicit Client(QObject* object_parent_ptr = nullptr);
         virtual ~Client(void);

         //const QString kIdent = "PC4237";

         inline auto byte_order(void) -> Host::ByteOrder {
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
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(host, port, username, initial_room);
         }

         inline auto connect_to_host(
               QUrl url,
               QString username = "Seville User",
               int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(url.host(), url.port(), username, initial_room);
         }

         inline auto connect_to_host(QUrl url, int initial_room = 0) -> void {
            do_disconnect_from_host_();
            do_connect_to_host_(
                     url.host(), url.port(), url.userName(), initial_room);
         }

         inline auto disconnect_from_host(void) -> void {
            do_disconnect_from_host_();
         }

      signals:
         void background_image_did_load(void);
         void connection_state_did_change(void);

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

      //public slots:
         //void on_readyRead(void);
         //void on_socketError(void);
         //void on_pingTimer(QTimerEvent* pingTimeEvent);
//         void on_(QNetworkReply* reply)
//         {

         //         }
         void do_setup_events_();
         void do_fetch_background_async_(const QString &url);
         void do_reset_();
         void do_reset_receive_timer_();
         void do_read_socket_();
         void do_set_connection_state_(ClientState client_state);
         void do_connect_to_host_(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initial_room = 0);
         void do_disconnect_from_host_();
         void do_determine_client_byte_order_();
         int do_determine_server_byte_order_();
         bool do_determine_is_connected_() const;
         NetMsgOptions do_determine_should_swap_endianness_() const;
         int do_receive_altlogon_();
         int do_receive_alt_room_description_();
         int do_receive_asset_incoming_();
         int do_receive_asset_query_();
         int do_receive_authenticate_();
         int do_receive_blowthru_();
         int do_receive_connection_error_();
         int do_receive_door_lock_();
         int do_receive_door_unlock_();
         int do_receive_draw_();
         int do_receive_http_server_location_();
         int do_receive_movement_();
         int do_receive_navigation_error_();
         int do_receive_picture_move_();
         int do_receive_ping_();
         int do_receive_pong_();
         int do_receive_prop_delete_();
         int do_receive_prop_move_();
         int do_receive_prop_new_();
         int do_receive_room_descend_();
         int do_receive_room_description_();
         int do_receive_room_user_list_();
         int do_receive_server_version_();
         int do_receive_server_info_();
         int do_receive_user_new_();
         int do_receive_user_color_();
         int do_receive_user_exit_room_();
         int do_receive_user_face_();
         int do_receive_user_prop_();
         int do_receive_user_description_();
         int do_receive_user_rename_();
         int do_receive_user_leaving_();
         int do_receive_user_logged_on_and_max_();
         int do_receive_user_status_();
         int do_receive_server_room_list_();
         int do_receive_server_user_list_();
         int do_receive_spot_move_();
         int do_receive_spot_state_();
         int do_receive_talk_();
         int do_receive_whisper_();
         int do_receive_xtalk_();
         int do_receive_xwhisper_();
         int do_send_logon_();
         int do_send_authenticate_(
               const QString &username, const QString &password);
         int do_route_received_netmsg_();
         void do_deinit_();
         void do_init_();
      };
   }
}

#endif   // SEVILLE_PALACE_CLIENT_H_
