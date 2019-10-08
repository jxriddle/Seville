#ifndef PALACE_CLIENT_H
#define PALACE_CLIENT_H

#include <QObject>
#include <QImage>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QTimer>
#include <QTime>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "Common.h"
#include "Palace_Server.h"
#include "Palace_User.h"
#include "Palace_Room.h"
#include "Palace_Crypto.h"
#include "Palace_NetMsg_Generic.h"
#include "Palace_NetMsg_Logon.h"

namespace Seville
{
   namespace Palace
   {
      class Client : public QObject
      {
         Q_OBJECT

         public:
            enum class ConnectionState: u32 {
               Disconnected = 0x00000000,
               Handshaking = 0x00000001,
               Connected = 0x00000002,
            };

            enum class ReasonForTerminate: u32 {
               Nil = 0x00000000,
               Error = 0x00000001,
               CommError = 0x00000002,
               Flood = 0x00000003,
               KilledByPlayer = 0x00000004,
               ServerDown = 0x00000005,
               Unresponsive = 0x00000006,
               KilledBySysop = 0x0000007,
               ServerFull = 0x00000008,
               InvalidSerialNumber = 0x0000009,
               DuplicateUser = 0x0000000a,
               DeathPenaltyActive = 0x0000000b,
               Banished = 0x0000000c,
               BanishKill = 0x0000000d,
               NoGuests = 0x0000000e,
               DemoExpired = 0x0000000f,
               Unknown = 0x00000010,
            };

            enum class ErrorForNavigation: u32 {
               InternalError = 0x00000000,
               UnknownRoom = 0x00000001,
               RoomFull = 0x00000002,
               RoomClosed = 0x00000003,
               CantAuthor = 0x00000004,
               PalaceFull = 0x00000005,
            };

            enum class FlagAuxOptions: u32 {
               UnknownMachine = 0x00000000,
               Mac68k = 0x00000001,
               MacPowerPc = 0x00000002,
               Win16 = 0x00000003,
               Win32 = 0x00000004,
               Java = 0x00000005,
               MacIntel = 0x00000006,
               IOS = 0x00000010,
               Android = 0x00000011,
               Linux = 0x00000032,
               OSMask = 0x0000000F,
               Authenticate = 0x80000000,
            };

            enum class CapabilitiesForUpload: u32 {
               NoUpload = 0x00000000,
               PalaceAssetUpload = 0x00000001,
               FtpAssetUpload = 0x00000002,
               HttpAssetUpload = 0x00000003,
               OtherAssetUpload = 0x00000004,
               PalaceFileUpload = 0x00000008,
               FtpFileUpload = 0x000000010,
               HttpFileUpload = 0x00000020,
               OtherFileUpload = 0x00000080,
               ExtendedUpload = 0x00000100,
            };

            enum class CapabilitiesForDownload: u32 {
               NoDownload = 0x00000000,
               PalaceAssetDownload = 0x00000001,
               FtpAssetDownload = 0x00000002,
               HttpAssetDownload = 0x00000004,
               OtherAssetDownload = 0x00000008,
               PalaceFileDownload = 0x00000010,
               FtpFileDownload = 0x00000020,
               HttpFileDownload = 0x00000040,
               OtherFileDownload = 0x00000080,
               HttpFileExtendedDownload = 0x00000100,
               ExtendedDownload = 0x00000200,
            };

            enum class CapabilitiesFor2dEngine: u32 {
               No2dEngine = 0x00000000,
               Palace2dEngine = 0x00000001,
            };

            enum class CapabilitiesFor2dGraphics: u32 {
               No2dGraphics = 0x00000000,
               Gif87 = 0x00000001,
               Gif89a = 0x00000002,
               Jpg = 0x00000004,
               Tiff = 0x00000008,
               Targa = 0x00000010,
               Bmp = 0x00000020,
               Pct = 0x00000040,
            };

            enum class CapabilitiesFor3dGraphics: u32 {
               No3dGraphics = 0x00000000,
               Vrml1 = 0x0000001,
               Vrml2 = 0x0000002,
            };

            static const int kDefaultServerPort = 9998;

            static constexpr int kIntervalTimeoutForTransferInMs = 30 * 1000;
            static constexpr int kIntervalToTimeoutForPongInMs = 60 * 1000;
            static constexpr int kIntervalToPingInMs = 5 * 1000;

            static const u32 magicFromPChat = 0x00011940;
            static const QString kIdent; // = "PC4237"
            static const int kByteSizeOfLongestUsername = 31;
            static const int kByteSizeOfLongestWizpass = 31;
            static const int kByteSizeOfIdent = 6;
            static const int kByteSizeOfDefaultChunkRead = 100; /* NetMsg::kHeaderSize; */
            static const int kByteSizeOfShortestChunkRead = 1;

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
            //QImage myBackgroundImg;
            QNetworkAccessManager myHttpGetMgr;
            //NetMsg myNetMsgRx;
            //QDataStream *myNetMsgRxDs;
            //NetMsg myNetMsgTx;
            //QDataStream *myNetMsgTxDs;
            NetMsg::Generic myNetMsg;
            int myPransferTimerId;
            QTimer myPingTimer;
            QTimer myReceiveNetMsgTimer;
            QTime myPongTime;
            //QByteArray myBuffer;
            Host::ByteOrder myByteOrder;
            ConnectionState myConnectionState;
            QTcpSocket mySocket;
            QString myUsername;
            QString myHost;
            quint16 myPort;
            Server myServer;
            User myUser;
            Room myCurrentRoom;
            Crypto myCrypto;
            QTimer myTimer;
            int myTransferTimerId;

            bool myPuidChanged    = false;
            u32 myPuidCounter = 0xf5dc385e;
            u32 myPuidCrc     = 0xc144c580;
            u32 myRegCounter  = 0xcf07309c;
            u32 myRegCrc      = 0x5905f923;

            void doSetupEvents(void);
            void doReset(void);
            void doResetReceiveTimer(void);
            void doConnectToHost(
                  QString host,
                  int port = 9998,
                  QString username = "Seville User",
                  int initialRoom = 0);
            void doDisconnectFromHost(void);

            void doDetermineClientByteOrder(void);
            int doDetermineServerByteOrder(void);
            int doRouteReceivedNetMsg(void);
            bool doDetermineIsConnected(void) const;
            bool doDetermineIfShouldSwapEndianness(void) const;

            int doReceiveAltLogon(void);
            int doReceiveAltRoomDescription(void);
            int doReceiveAssetIncoming(void);
            int doReceiveAssetQuery(void);
            int doReceiveAuthenticate(void);
            int doReceiveBlowThru(void);
            int doReceiveConnectionError(void);
            int doReceiveDoorLock(void);
            int doReceiveDoorUnlock(void);
            int doReceiveDraw(void);
            int doReceiveHttpServerLocation(void);
            int doReceiveMovement(void);
            int doReceiveNavigationError(void);
            int doReceivePictMove(void);
            int doReceivePing(void);
            int doReceivePong(void);
            int doReceivePropDelete(void);
            int doReceivePropMove(void);
            int doReceivePropNew(void);
            int doReceiveRoomDescend(void);
            int doReceiveRoomDescription(void);
            int doReceiveRoomUserList(void);
            int doReceiveServerVersion(void);
            int doReceiveServerInfo(void);
            int doReceiveUserNew(void);
            int doReceiveUserColor(void);
            int doReceiveUserExitRoom(void);
            int doReceiveUserFace(void);
            int doReceiveUserProp(void);
            int doReceiveUserDescription(void);
            int doReceiveUserRename(void);
            int doReceiveUserLeaving(void);
            int doReceiveUserLoggedOnAndMax(void);
            int doReceiveUserStatus(void);
            int doReceiveServerRoomList(void);
            int doReceiveServerUserList(void);
            int doReceiveSpotMove(void);
            int doReceiveSpotState(void);
            int doReceiveTalk(void);
            int doReceiveWhisper(void);
            int doReceiveXTalk(void);
            int doReceiveXWhisper(void);

            int doSendLogon(void);
            int doSendAuthenticate(
                  const QString& username, const QString& password);

            void doFetchBackgroundAsync(const QString& url);
            void doFetchBackgroundAsync(
                  const QString& url, QMap<QString, QString>& headers);
            //int doHttpPutAsync_(QString& url);
            //int doHttpPostAsync_(QString& url);

         signals:
            //void backgroundChanged(QByteArray bytesOfBackgroundImage);
            void backgroundChanged();

         public slots:
            void onReadyRead(void);
            void onSocketError(void);
            void onPingTimer(QTimerEvent* pingTimeEvent);
            void onReceivedBackgroundAsync(QNetworkReply* reply);

         public:
            //const QString kIdent = "PC4237";

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

            void reset(void) { doReset(); }

            void connectToHost(
                  QString host,
                  int port = 9998,
                  QString username = "Seville User",
                  int initialRoom = 0)
            {
               doDisconnectFromHost();
               doConnectToHost(host, port, username, initialRoom);
            }

            void connectToHost(
                  QUrl url,
                  QString username = "Seville User",
                  int initialRoom = 0)
            {
               doDisconnectFromHost();
               doConnectToHost(url.host(), url.port(), username, initialRoom);
            }

            void connectToHost(QUrl url, int initialRoom = 0)
            {
               doDisconnectFromHost();
               doConnectToHost(url.host(), url.port(), url.userName(), initialRoom);
            }

            //{ doConnectToHost(url.toString(), Server::kDefaultServerPort, Client::kDefaultUsername, initialRoom); }
            void disconnectFromHost(void) { doDisconnectFromHost(); }

            Host::ByteOrder byteOrder(void) { return myByteOrder; }

            void setByteOrder(const Host::ByteOrder value)
               { myByteOrder = value; }

            Client::ConnectionState connectionState(void)
               { return myConnectionState; }

            QString username(void) const { return myUsername; }
            void setUsername(QString &value) { myUsername = value; }

            Server& server(void) { return myServer; }
            User& user(void) { return myUser; }
            Room& currentRoom(void) { return myCurrentRoom; }

            virtual ~Client(void);
            explicit Client(QObject* parent = nullptr);
      };
   }
}

#endif // PALACE_CLIENT_H
