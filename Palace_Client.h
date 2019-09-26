#ifndef PALCLIENT_H
#define PALCLIENT_H

#include <QObject>
#include <QImage>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QTimer>
#include <QTime>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "App_Fraction.h"
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
            enum class ConnectionState {
               Disconnected, Handshaking, Connected
            };

            static const int kDefaultServerPort = 9998;

            static constexpr int kIntervalTimeoutForTransferInMs = 30 * 1000;
            static constexpr int kIntervalToTimeoutForPongInMs = 60 * 1000;
            static constexpr int kIntervalToPingInMs = 5 * 1000;

            static const QString kIdent; // = "PC4237"
            static const int kByteSizeOfLongestUsername = 31;
            static const int kByteSizeOfLongestWizpass = 31;
            static const int kByteSizeOfIdent = 6;
            static const int kByteSizeOfDefaultChunkRead = 100; /* NetMsg::kHeaderSize; */
            static const int kByteSizeOfShortestChunkRead = 1;

            static const int kDemoElapsed = 0x00011940;
            static const int kTotalElapsed = 0x00011940;
            static const int kDemoLimit = 0x00011940;

            static const quint32 kAuxFlagsUnknownMachine    = 0x00000000;
            static const quint32 kAuxFlagsMac68k            = 0x00000001;
            static const quint32 kAuxFlagsMacPpc            = 0x00000002;
            static const quint32 kAuxFlagsWin16             = 0x00000003;
            static const quint32 kAuxFlagsWin32             = 0x00000004;
            static const quint32 kAuxFlagsJava              = 0x00000005;
            static const quint32 kAuxFlagsBsd               = 0x00000006;
            static const quint32 kAuxFlagsLinux             = 0x00000007;
            static const quint32 kAuxFlagsOSMask            = 0x0000000F;
            static const quint32 kAuxFlagsAuthenticate      = 0x80000000;

            static const quint32 kUlCapsAssetsPalace        = 0x00000001;
            static const quint32 kUlCapsAssetsFtp           = 0x00000002;
            static const quint32 kUlCapsAssetsHttp          = 0x00000004;
            static const quint32 kUlCapsAssetsOther         = 0x00000008;
            static const quint32 kUlCapsFilesPalace         = 0x00000010;
            static const quint32 kUlCapsFilesFtp            = 0x00000020;
            static const quint32 kUlCapsFilesHttp           = 0x00000040;
            static const quint32 kUlCapsFilesOther          = 0x00000080;
            static const quint32 kUlCapsExtendPkt           = 0x00000100;

            static const quint32 kDlCapsAssetsPalace        = 0x00000001;
            static const quint32 kDlCapsAssetsFtp           = 0x00000002;
            static const quint32 kDlCapsAssetsHttp          = 0x00000004;
            static const quint32 kDlCapsAssetsOther         = 0x00000008;
            static const quint32 kDlCapsFilesPalace         = 0x00000010;
            static const quint32 kDlCapsFilesFtp            = 0x00000020;
            static const quint32 kDlCapsFilesHttp           = 0x00000040;
            static const quint32 kDlCapsFilesOther          = 0x00000080;
            static const quint32 kDlCapsFilesHttpSvr        = 0x00000100;
            static const quint32 kDlCapsExtendPkt           = 0x00000200;

         private:
            QImage myBackgroundImg;
            QNetworkAccessManager myHttpGetMgr;
            //NetMsg myNetMsgRx;
            //QDataStream *myNetMsgRxDs;
            //NetMsg myNetMsgTx;
            //QDataStream *myNetMsgTxDs;
            NetMsg::Generic myNetMsg;
            int myPransferTimerId;
            QTimer myPingTimer;
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

            void doSetupEvents();
            void doReset();
            void doResetReceiveTimer();
            void doConnectToHost(
                  QString host,
                  int port = 9998,
                  QString username = "Seville User",
                  int initialRoom = 0);
            void doDisconnectFromHost();

            int doReadNetMsgHeader(NetMsg::Generic& netMsg);
            int doReadNetMsgBody(NetMsg::Generic& netMsg);
            int doReadNetMsg(NetMsg::Generic& netMsg);
            //int doReadDataIntoNetMsg(
            //      NetMsg::Generic& netMsg,
            //      i32 maxSize = NetMsg::kByteSizeMaximum);
            //bool doGetHasEnoughData();
            void doDetermineClientByteOrder();
            int doDetermineServerByteOrder();
            int doRouteReceivedNetMsg();
            bool doDetermineIsConnected() const;
            bool doDetermineShouldSwapEndianness() const;

            int doReceiveBlowThru(const NetMsg::Generic& netMsg);
            int doReceiveRoomDescription(const NetMsg::Generic& netMsg);
            int doReceiveServerVersion(const NetMsg::Generic& netMsg);
            int doReceiveServerInfo(const NetMsg::Generic& netMsg);
            int doReceiveUserStatus(const NetMsg::Generic& netMsg);
            int doReceiveUserLoggedOnAndMax(const NetMsg::Generic& netMsg);
            int doReceiveHttpServerLocation(const NetMsg::Generic& netMsg);
            int doReceiveAltRoomDescription(const NetMsg::Generic& netMsg);
            int doReceiveRoomUserList(const NetMsg::Generic& netMsg);
            int doReceiveServerUserList(const NetMsg::Generic& netMsg);
            int doReceiveServerRoomList(const NetMsg::Generic& netMsg);
            int doReceiveRoomDescend(const NetMsg::Generic& netMsg);
            int doReceiveUserNew(const NetMsg::Generic& netMsg);
            int doReceivePing(const NetMsg::Generic& netMsg);
            int doReceivePong(const NetMsg::Generic& netMsg);
            int doReceiveXTalk(const NetMsg::Generic& netMsg);
            int doReceiveXWhisper(const NetMsg::Generic& netMsg);
            int doReceiveTalk(const NetMsg::Generic& netMsg);
            int doReceiveWhisper(const NetMsg::Generic& netMsg);
            int doReceiveAssetIncoming(const NetMsg::Generic& netMsg);
            int doReceiveAssetQuery(const NetMsg::Generic& netMsg);
            int doReceiveMovement(const NetMsg::Generic& netMsg);
            int doReceiveUserColor(const NetMsg::Generic& netMsg);
            int doReceiveUserFace(const NetMsg::Generic& netMsg);
            int doReceiveUserProp(const NetMsg::Generic& netMsg);
            int doReceiveUserDescription(const NetMsg::Generic& netMsg);
            int doReceiveUserRename(const NetMsg::Generic& netMsg);
            int doReceiveUserLeaving(const NetMsg::Generic& netMsg);
            int doReceiveUserExitRoom(const NetMsg::Generic& netMsg);
            int doReceivePropMove(const NetMsg::Generic& netMsg);
            int doReceivePropDelete(const NetMsg::Generic& netMsg);
            int doReceivePropNew(const NetMsg::Generic& netMsg);
            int doReceiveDoorLock(const NetMsg::Generic& netMsg);
            int doReceiveDoorUnlock(const NetMsg::Generic& netMsg);
            int doReceivePictMove(const NetMsg::Generic& netMsg);
            int doReceiveSpotState(const NetMsg::Generic& netMsg);
            int doReceiveSpotMove(const NetMsg::Generic& netMsg);
            int doReceiveDraw(const NetMsg::Generic& netMsg);
            int doReceiveAltLogon(); // const NetMsg::AltLogon& netMsg);
            int doReceiveAuthenticate(const NetMsg::Generic& netMsg);
            int doReceiveNavError(const NetMsg::Generic& netMsg);
            int doReceiveConnectionError(const NetMsg::Generic& netMsg);

            int doSendLogon();
            int doSendAuthenticate(
                  const QString& username, const QString& password);

            void doGetBackgroundAsync(
                  const QString& url, QMap<QString, QString> headers);
            //int doHttpPutAsync_(QString& url);
            //int doHttpPostAsync_(QString& url);

         private slots:
            void doOnReadyRead();
            void doOnSocketError();
            void doOnPingTimer(QTimerEvent* pingTimeEvent);
            void onGotBackgroundAsync(QNetworkReply* reply);

         //signals:

         public slots:

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

            void reset() { doReset(); }

            void connectToHost(
                  QString host,
                  int port = 9998,
                  QString username = "Seville User",
                  int initialRoom = 0) {
               doDisconnectFromHost();
               doConnectToHost(host, port, username, initialRoom);
            }

            void connectToHost(
                  QUrl url,
                  QString username = "Seville User",
                  int initialRoom = 0) {
               doDisconnectFromHost();
               doConnectToHost(url.host(), url.port(), username, initialRoom);
            }

            void connectToHost(QUrl url, int initialRoom = 0) {
               doDisconnectFromHost();
               doConnectToHost(url.host(), url.port(), url.userName(), initialRoom);
            }

               //{ doConnectToHost(url.toString(), Server::kDefaultServerPort, Client::kDefaultUsername, initialRoom); }
            void disconnectFromHost() { doDisconnectFromHost(); }

            Host::ByteOrder byteOrder() { return myByteOrder; }

            void setByteOrder(const Host::ByteOrder value) {
               myByteOrder = value;
            }

            Client::ConnectionState connectionState() {
               return myConnectionState;
            }

            QString username() const { return myUsername; }
            void setUsername(QString &value) { myUsername = value; }

            Server& server() { return myServer; }
            User& user() { return myUser; }
            Room& currentRoom() { return myCurrentRoom; }

            virtual ~Client();
            explicit Client(QObject* parent = nullptr);
      };
   }
}

#endif
