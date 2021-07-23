#ifndef SEVILLE_PALACE_CLIENT_H_
#define SEVILLE_PALACE_CLIENT_H_

#include <string>
#include <random>

#include <QObject>
#include <QImage>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QTimer>
#include <QTime>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "seville/palace/server.h"
#include "seville/palace/user.h"
#include "seville/palace/room.h"
#include "seville/palace/cipher.h"
#include "seville/palace/log.h"
#include "seville/palace/netmsg.h"
#include "seville/palace/registration.h"

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
         kHttpAssetUpload = 0x00000004,
         kOtherAssetUpload = 0x00000008,
         kPalaceFileUpload = 0x00000010,
         kFtpFileUpload = 0x00000020,
         kHttpFileUpload = 0x00000040,
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
         static const i32 kIdentLen = 6;

         static const i32 kDefaultPingInterval = 60 * 1000;
         // static const u32 kNumPropCells = 9;
         static const u32 kNetMsgTTLMax = 100;
         static const i32 kDefaultServerPort = 9998;

         static constexpr i32 kIntervalTimeoutForTransferInMs = 30 * 1000;
         static constexpr i32 kIntervalToTimeoutForPongInMs = 60 * 1000;
         static constexpr i32 kIntervalToPingInMs = 5 * 1000;

         static const u32 kMagicFromPChat = 0x00011940;
         //static const QString kIdent = "PC4237"
         static const i32 kMaxSizeOfUsernameSize = 31;
         static const i32 kMaxSizeOfWizpassSize = 31;
         static const i32 kSizeOfIdentSize = 6;
         static const i32 kDefaultSizeOfChunkReadSize = 100;
         static const i32 kMinSizeOfShortestChunkReadSize = 1;

         static const i32 kNumFaces = 13;
         static const i32 kNumFaceColors = 17;

         explicit Client(QObject* parentObjectPtr = nullptr);
         virtual ~Client(void);

         //const QString kIdent = "PC4237";

         inline HostByteOrder byteOrderKind(void) {
            return my_byteOrder;
         }

         inline void setByteOrderKind(const HostByteOrder value) {
            my_byteOrder = value;
         }

         inline ConnectionState connectionState(void) {
            return my_connectionState;
         }

         inline QString username(void) const {
            return my_username;
         }

         inline void setUsername(const QString &value) {
            my_username = value;
         }

         inline Log* loggerPtr(void) {
            return &my_logger;
         }

         inline Server* serverPtr(void) {
            return &my_server;
         }

         // inline auto userPtr(void) -> User* {
         //    return &my_user;
         // }

         inline Room* roomPtr(void) {
            return &my_room;
         }

         inline bool isBigEndian(void) {
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

         inline void clear(void) {
            do_clear();
         }

         inline void connectToHost(
               QString host,
               i32 port = 9998,
               QString username = "Seville User",
               i32 initialRoom = 0) {
            do_disconnectFromHost();
            do_connectToHost(host, port, username, initialRoom);
         }

         inline void connectToHost(
               QUrl url,
               QString username = "Seville User",
               i32 initialRoom = 0) {
            do_disconnectFromHost();
            do_connectToHost(url.host(), url.port(), username, initialRoom);
         }

         inline void connectToHost(QUrl url, i32 initialRoom = 0) {
            do_disconnectFromHost();
            do_connectToHost(
                     url.host(), url.port(), url.userName(), initialRoom);

            emit connectionStateDidChangeEvent(
                     ConnectionState::kDisconnectedState);
         }

         inline void disconnectFromHost(void) {
            //my_logger.appendInfoMessage("Disconnected.");
            do_disconnectFromHost();
            this->isBigEndian();

            emit connectionStateDidChangeEvent(
                     ConnectionState::kDisconnectedState);
         }

         inline void chat(const QString& text) {
            do_roomChat(text);
         }

         inline void move(i16 x, i16 y) {
            my_logger.appendDebugMessage(
                     QString("Movement to (%1, %2)").arg(x).arg(y));

            auto userPtr = my_room.userPtrWithId(my_userId);
            userPtr->setX(x);
            userPtr->setY(y);

            do_sendMove(x, y);

            emit viewNeedsUpdatingEvent();
         }

         inline void gotoRoom(u16 room_id) {
            my_logger.appendDebugMessage(
                     QString("GotoRoom %1").arg(room_id));

            do_sendGotoRoom(room_id);
         }

      signals:
         void backgroundImageDidLoadEvent(void);
         void connectionStateDidChangeEvent(ConnectionState);
         void viewNeedsUpdatingEvent(void);
         void serverNameWasSetEvent(const QString& value);
         void roomListWasReceivedEvent(void);
         void userListWasReceivedEvent(void);

      public slots:
         void on_networkAccessManagerDidFinish(QNetworkReply* replyPtr);
         void on_pingTimerDidTrigger(void);
         void on_readyReadDidOccur(void);
         void on_socketErrorDidOccur(QAbstractSocket::SocketError);

      private:
         Log my_logger;
         QNetworkAccessManager my_networkAccessManager;
         QTcpSocket* my_socketPtr;
         i32 my_transferTimerId;
         QTimer my_pingTimer;
         QTimer my_netmsgReceiveTimer;
         QTime my_pongTime;
         HostByteOrder my_byteOrder;
         ConnectionState my_connectionState;
         // QTcpSocket my_socket;
         // QString my_host;
         // u16 my_port;
         Server my_server;
         i32 my_userId;
         QString my_username;
         QString my_wizpass;
         Room my_room;
         Cipher my_cipher;
         QTimer my_timer;
         //i32 my_transfer_timer_id;
         NetMsg my_netMsg;
         u32 my_netMsgTTLCount;
         u32 my_idCrc;
         u32 my_idCounter;
         bool my_isUserIdChanged;
         u32 my_regCrc;
         u32 my_regCounter;
         u32 my_shouldRetryLogonFlag;
         u32 my_regMagic;
         Registration my_registration;
         std::random_device my_randDev;

//         bool my_puidChangedFlag; //    = false;
//         u32 my_puidCounter; // = 0xf5dc385e;
//         u32 my_puidCrc; //     = 0xc144c580;
//         u32 my_regCounter; //  = 0xcf07309c;
//         u32 my_regCrc; //      = 0x5905f923;

         //      i32 Client::getKMillisecondPongTimeout()
         //      {
         //         return kIntervalToTimeoutForPongInMs;
         //      }

         //      void Client::setKMillisecondPongTimeout(i32 value)
         //      {
         //         kIntervalToTimeoutForPongInMs = value;
         //      }

         void do_setupEvents(void);

         void do_fetchBackgroundAsync(const QString& url);
         void do_fetchPropsAsync(const std::vector<Prop>& props);

         void do_clear(void);
         void do_resetReceiveTimer(void);
         void do_readSocket(void);

         void do_setConnectionState(ConnectionState clientState);

         i32 do_receiveNetMsgFromSocket(void);
         i32 do_readNetMsgHeaderFromSocket(void);
         i32 do_readNetMsgContentFromSocket(void);

         void do_connectToHost(
               QString host,
               i32 port = 9998,
               QString username = "Seville User",
               i32 initialRoom = 0);
         void do_disconnectFromHost(void);

         void do_roomChat(const QString& text);

         void do_determineClientByteOrder(void);
         i32 do_determineServerByteOrder(void);
         bool do_determineIsConnected(void) const;
         NetMsgOptions do_determineShouldSwapEndianness(void) const;

         User do_processUserNew(void);
         i32 do_parseCommand(const QString& text);
         i32 do_routeReceivedNetMsg(void);

         i32 do_receiveAltLogon(void);
         i32 do_receiveAltRoomDescription(void);
         i32 do_receiveAssetIncoming(void);
         i32 do_receiveAssetQuery(void);
         i32 do_receiveAuthenticate(void);
         i32 do_receiveBlowthru(void);
         i32 do_receiveConnectionError(void);
         i32 do_receiveDoorLock(void);
         i32 do_receiveDoorUnlock(void);
         i32 do_receiveDraw(void);
         i32 do_receiveHttpServerLocation(void);
         i32 do_receiveMovement(void);
         i32 do_receiveNavigationError(void);
         i32 do_receivePictureMove(void);
         i32 do_receivePing(void);
         i32 do_receivePong(void);
         i32 do_receivePropDelete(void);
         i32 do_receivePropMove(void);
         i32 do_receivePropNew(void);
         i32 do_receiveRoomDescend(void);
         i32 do_receiveRoomDescription(void);
         i32 do_receiveRoomUserList(void);
         i32 do_receiveServerVersion(void);
         i32 do_receiveServerInfo(void);
         i32 do_receiveUserNew(void);
         i32 do_receiveUserColor(void);
         i32 do_receiveUserExitRoom(void);
         i32 do_receiveUserFace(void);
         i32 do_receiveUserProp(void);
         i32 do_receiveUserDescription(void);
         i32 do_receiveUserRename(void);
         i32 do_receiveUserLeaving(void);
         i32 do_receiveUserLoggedOnAndMax(void);
         i32 do_receiveUserStatus(void);
         i32 do_receiveRoomList(void);
         i32 do_receiveUserList(void);
         i32 do_receiveSpotMove(void);
         i32 do_receiveSpotState(void);
         i32 do_receiveTalk(void);
         i32 do_receiveWhisper(void);
         i32 do_receiveXTalk(void);
         i32 do_receiveXWhisper(void);

         i32 do_sendLogon(void);
         i32 do_sendAuthenticate(
               const QString &username, const QString &password);
         i32 do_sendTalk(const QString& text);
         i32 do_sendXTalk(const QString& text);
         i32 do_sendXWhisper(const u32 userId, const QString& text);
         i32 do_sendPing(void);
         i32 do_sendPong(void);
         i32 do_sendGotoRoom(u16 roomId);
         i32 do_sendMove(i16 x, i16 y);
         i32 do_sendRequestRoomList(void);
         i32 do_sendRequestUserList(void);
         i32 do_sendRequestRoomAndUserLists(void);
         i32 do_sendFaceColor(u16 colorId);
         i32 do_sendFace(u16 face);

         void do_teardownEvents(void);
         void do_initEvents(void);
         void do_deinit(void);
         void do_init(void);
      };
   }
}

#endif   // SEVILLE_PALACE_CLIENT_H_
