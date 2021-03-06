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

//#include "seville/base/sevilleapp.h"
#include "seville/palace/server.h"
#include "seville/palace/user.h"
#include "seville/palace/room.h"
#include "seville/palace/cipher.h"
#include "seville/palace/log.h"
#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      enum ConnectionState: i32 {
         kDisconnectedState = 0x00000000,
         kHandshakingState = 0x00000001,
         kConnectedState = 0x00000002,
      };

      enum NetMsgReadState: i32 {
         kNetMsgReadHeaderState = 0x00000001,
         kNetMsgReadBodyState = 0x00000002,
      };

      enum TerminateReason: i32 {
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

      enum NavigationError: i32 {
         kInternalNavigationError = 0x00000000,
         kUnknownRoomNavigationError = 0x00000001,
         kRoomFullNavigationError = 0x00000002,
         kRoomClosedNavigationError = 0x00000003,
         kCantAuthorNavigationError = 0x00000004,
         kPalaceFullNavigationError = 0x00000005,
      };

      enum FlagAuxOptions: u32 {
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

      enum UploadCapabilities: i32 {
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

      enum DownloadCapabilities: i32 {
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

      enum EngineCapabilities2d: i32 {
         kNo2dEngine = 0x00000000,
         kPalace2dEngine = 0x00000001,
      };

      enum GraphicsCapabilities2d: i32 {
         kNo2dGraphics = 0x00000000,
         kGif87 = 0x00000001,
         kGif89a = 0x00000002,
         kJpg = 0x00000004,
         kTiff = 0x00000008,
         kTarga = 0x00000010,
         kBmp = 0x00000020,
         kPct = 0x00000040,
      };

      enum GraphicsCapabilities3d: i32 {
         kNo3dGraphics = 0x00000000,
         kVrml1 = 0x0000001,
         kVrml2 = 0x0000002,
      };

      class Client : public QObject
      {
      Q_OBJECT

      public:
         static const char* kIdent;
         static const int kIdentLen = 6;

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

         explicit Client(QObject* parentObjectPtr = nullptr);
         virtual ~Client(void);

         //const QString kIdent = "PC4237";

         inline auto byteOrderKind(void) -> HostByteOrder {
            return my_byteOrder;
         }

         inline auto setByteOrderKind(const HostByteOrder value) -> void {
            my_byteOrder = value;
         }

         inline auto connectionState(void) -> ConnectionState {
            return my_connectionState;
         }

         inline auto username(void) const -> QString {
            return my_username;
         }

         inline auto setUsername(const QString &value) -> void {
            my_username = value;
         }

         inline auto loggerPtr(void) -> Log* {
            return &my_logger;
         }

         inline auto serverPtr(void) -> Server* {
            return &my_server;
         }

         inline auto userPtr(void) -> User* {
            return &my_user;
         }

         inline auto roomPtr(void) -> Room* {
            return &my_room;
         }

         auto inline isBigEndian(void) -> bool {
            bi32 bi;
            bi.dwordValue = { 0x01020304 };

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

         inline auto clear(void) -> void {
            do_clear();
         }

         inline auto connectToHost(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initialRoom = 0) -> void {
            do_disconnectFromHost();
            do_connectToHost(host, port, username, initialRoom);
         }

         inline auto connectToHost(
               QUrl url,
               QString username = "Seville User",
               int initialRoom = 0) -> void {
            do_disconnectFromHost();
            do_connectToHost(url.host(), url.port(), username, initialRoom);
         }

         inline auto connectToHost(QUrl url, int initialRoom = 0) -> void {
            do_disconnectFromHost();
            do_connectToHost(
                     url.host(), url.port(), url.userName(), initialRoom);
         }

         inline auto disconnectFromHost(void) -> void {
            my_logger.appendInfoMessage("Disconnected.");
            do_disconnectFromHost();
            this->isBigEndian();
         }

         inline auto chat(const QString& text) -> void {
            do_roomChat(text);
         }

      signals:
         void backgroundImageDidLoadEvent(void);
         void connectionStateDidChangeEvent(ConnectionState);

      public slots:
         void on_readyReadDidOccur(void);
         void on_socketErrorDidOccur(QAbstractSocket::SocketError);

      private:
         //NetMsgReadState netMsgReadState;
         Log my_logger;
         //QImage my_background_img;
         QNetworkAccessManager my_networkAccessManager;
         //NetMsg my_netmsg_rx;
         //QDataStream *my_netmsg_rx_ds_ptr;
         //NetMsg my_netmsg_tx_ptr;
         //QDataStream *my_netmsg_tx_ds_ptr;
         //palace::NetMsg my_netmsg_in;
         //NetMsg my_incomingNetMsg;
         int my_transferTimerId;
         QTimer my_pingTimer;
         QTimer my_netmsgReceiveTimer;
         QTime my_pongTime;
         //QByteArray my_buffer;
         HostByteOrder my_byteOrder;
         ConnectionState my_connectionState;
         QTcpSocket my_socket;
         QString my_username;
         QString my_host;
         quint16 my_port;
         Server my_server;
         User my_user;
         Room my_room;
         Cipher my_cipher;
         QTimer my_timer;
         //int my_transfer_timer_id;
//         i32 my_netMsgId;
//         i32 my_netMsgLen;
//         i32 my_netMsgRef;
         //ByteArray my_netMsgHeader;
         //ByteArray my_netMsgBody;
         NetMsg my_netMsg;
         //i32 my_netMsgSize;

//         bool my_puidChangedFlag; //    = false;
//         u32 my_puidCounter; // = 0xf5dc385e;
//         u32 my_puidCrc; //     = 0xc144c580;
//         u32 my_regCounter; //  = 0xcf07309c;
//         u32 my_regCrc; //      = 0x5905f923;

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
         void do_setupEvents(void);
         void do_fetchBackgroundAsync(const QString &url);
         void do_clear(void);
         void do_resetReceiveTimer(void);
         void do_readSocket(void);

         void do_setConnectionState(ConnectionState clientState);

         int do_receiveNetMsgFromSocket(void);
         int do_readNetMsgHeaderFromSocket(void);
         int do_readNetMsgContentFromSocket(void);

         void do_connectToHost(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initialRoom = 0);
         void do_disconnectFromHost(void);

         auto do_roomChat(const QString& text) -> void;

         void do_determineClientByteOrder(void);
         int do_determineServerByteOrder(void);
         bool do_determineIsConnected(void) const;
         NetMsgOptions do_determineShouldSwapEndianness(void) const;

         int do_receiveAltLogon(void);
         int do_receiveAltRoomDescription(void);
         int do_receiveAssetIncoming(void);
         int do_receiveAssetQuery(void);
         int do_receiveAuthenticate(void);
         int do_receiveBlowthru(void);
         int do_receiveConnectionError(void);
         int do_receiveDoorLock(void);
         int do_receiveDoorUnlock(void);
         int do_receiveDraw(void);
         int do_receiveHttpServerLocation(void);
         int do_receiveMovement(void);
         int do_receiveNavigationError(void);
         int do_receivePictureMove(void);
         int do_receivePing(void);
         int do_receivePong(void);
         int do_receivePropDelete(void);
         int do_receivePropMove(void);
         int do_receivePropNew(void);
         int do_receiveRoomDescend(void);
         int do_receiveRoomDescription(void);
         int do_receiveRoomUserList(void);
         int do_receiveServerVersion(void);
         int do_receiveServerInfo(void);
         int do_receiveUserNew(void);
         int do_receiveUserColor(void);
         int do_receiveUserExitRoom(void);
         int do_receiveUserFace(void);
         int do_receiveUserProp(void);
         int do_receiveUserDescription(void);
         int do_receiveUserRename(void);
         int do_receiveUserLeaving(void);
         int do_receiveUserLoggedOnAndMax(void);
         int do_receiveUserStatus(void);
         int do_receiveServerRoomList(void);
         int do_receiveServerUserList(void);
         int do_receiveSpotMove(void);
         int do_receiveSpotState(void);
         int do_receiveTalk(void);
         int do_receiveWhisper(void);
         int do_receiveXtalk(void);
         int do_receiveXwhisper(void);

         int do_sendLogon(void);
         int do_sendAuthenticate(
               const QString &username, const QString &password);
         int do_sendTalk(const QString& text);
         int do_sendXTalk(const QString& text);
         int do_sendPing(void);
         int do_sendPong(void);
         int do_sendGotoRoom(i16 roomId);
         int do_sendMove(i16 x, i16 y);

         int do_routeReceivedNetMsg(void);

         void do_deinit(void);
         void do_init(void);
      };
   }
}

#endif   // SEVILLE_PALACE_CLIENT_H_
