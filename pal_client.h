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

#include "pal_server.h"
#include "pal_user.h"
#include "pal_room.h"
#include "pal_crypto.h"
#include "pal_msg.h"

namespace Seville
{
   class PalClient : public QObject
   {
      Q_OBJECT

      public:
         enum class ConnectionState {
            Disconnected, Handshaking, Connected };

         static constexpr int kPalClientTransferTimeout = 30 * 1000;
         static constexpr int kPalClientPongTimeout = 60 * 1000;
         static constexpr int kPalclientPingInterval = 5 * 1000;

         static const QString kPalClientIdent; // = "PC4237"
         static const int kPalClientMaxUsernameLen = 31;
         static const int kPalClientMaxWizpassLen = 31;
         static const int kPalClientIdentLen = 6;
         static const int kPalClientDefaultPort = 9998;
         static const int kPalClientDefaultChunkReadSize = 100; /* PalMsg::kHeaderSize; */
         static const int kPalClientMinChunkReadSize = 1;
         static const int kPalClientDemoElapsed = 0x00011940;
         static const int kPalClientTotalElapsed = 0x00011940;
         static const int kPalClientDemoLimit = 0x00011940;

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

         const QString kClientIdent = "PC4237";

         explicit PalClient(QObject *parent = nullptr);
         virtual ~PalClient();
         void resetState();
         void connectToHost(
               QString host,
               int port = 9998,
               QString username = "Seville User",
               int initialRoom = 0);
         void connectToHost(
               QUrl url,
               QString username = "Seville User",
               int initialRoom = 0);
         void connectToHost(QUrl url, int initialRoom = 0);
         void disconnectFromHost();

         PalCommon::ByteOrder byteOrder();
         void setByteOrder(PalCommon::ByteOrder byteOrder);
         PalClient::ConnectionState state();
         QString username() const;
         void setUsername(QString &username);
         //QString host();
         //qint16 port();
         PalServer& server();
         PalUser& user();
         PalRoom& currentRoom();

      //signals:

      public slots:

      private:
          void doResetState();
          void doConnectToHost(
                QString host,
                int port = 9998,
                QString username = "Seville User",
                int initialRoom = 0);
          void doDisconnectFromHost();
          int doReadPalMsgHeader(PalMsg& palMsg);
          int doReadPalMsgBody(PalMsg& palMsg);
          int doReadPalMsg(PalMsg& palMsg);
          int doReadDataIntoPalMsg(
                PalMsg& palMsg,
                qint32 maxSize = PalMsg::kPalMsgMaxSize);
          //bool doGetHasEnoughData();
          void doDetermineClientByteOrder();
          int doDetermineServerByteOrder(const PalMsg& palMsg);
          int doRoutePalMsg(const PalMsg& palMsg);
          bool doDetermineIsConnected() const;

          int doHandleBlowThruPalMsg(const PalMsg& palMsg);
          int doHandleRoomDescriptionPalMsg(const PalMsg& palMsg);
          int doHandleServerVersionPalMsg(const PalMsg& palMsg);
          int doHandleServerInfoPalMsg(const PalMsg& palMsg);
          int doHandleUserStatusPalMsg(const PalMsg& palMsg);
          int doHandleUserLoggedOnAndMaxPalMsg(const PalMsg& palMsg);
          int doHandleHttpServerLocationPalMsg(const PalMsg& palMsg);
          int doHandleAltRoomDescriptionPalMsg(const PalMsg& palMsg);
          int doHandleRoomUserListPalMsg(const PalMsg& palMsg);
          int doHandleServerUserListPalMsg(const PalMsg& palMsg);
          int doHandleServerRoomListPalMsg(const PalMsg& palMsg);
          int doHandleRoomDescendPalMsg(const PalMsg& palMsg);
          int doHandleUserNewPalMsg(const PalMsg& palMsg);
          int doHandlePingPalMsg(const PalMsg& palMsg);
          int doHandlePongPalMsg(const PalMsg& palMsg);
          int doHandleXTalkPalMsg(const PalMsg& palMsg);
          int doHandleXWhisperPalMsg(const PalMsg& palMsg);
          int doHandleTalkPalMsg(const PalMsg& palMsg);
          int doHandleWhisperPalMsg(const PalMsg& palMsg);
          int doHandleAssetIncomingPalMsg(const PalMsg& palMsg);
          int doHandleAssetQueryPalMsg(const PalMsg& palMsg);
          int doHandleMovementPalMsg(const PalMsg& palMsg);
          int doHandleUserColorPalMsg(const PalMsg& palMsg);
          int doHandleUserFacePalMsg(const PalMsg& palMsg);
          int doHandleUserPropPalMsg(const PalMsg& palMsg);
          int doHandleUserDescriptionPalMsg(const PalMsg& palMsg);
          int doHandleUserRenamePalMsg(const PalMsg& palMsg);
          int doHandleUserLeavingPalMsg(const PalMsg& palMsg);
          int doHandleUserExitRoomPalMsg(const PalMsg& palMsg);
          int doHandlePropMovePalMsg(const PalMsg& palMsg);
          int doHandlePropDeletePalMsg(const PalMsg& palMsg);
          int doHandlePropNewPalMsg(const PalMsg& palMsg);
          int doHandleDoorLockPalMsg(const PalMsg& palMsg);
          int doHandleDoorUnlockPalMsg(const PalMsg& palMsg);
          int doHandlePictMovePalMsg(const PalMsg& palMsg);
          int doHandleSpotStatePalMsg(const PalMsg& palMsg);
          int doHandleSpotMovePalMsg(const PalMsg& palMsg);
          int doHandleDrawPalMsg(const PalMsg& palMsg);
          int doHandleAltLogonPalMsg(const PalMsg& palMsg);
          int doHandleAuthenticatePalMsg(const PalMsg& palMsg);
          int doHandleNavErrorPalMsg(const PalMsg& palMsg);
          int doHandleConnectionErrorPalMsg(const PalMsg& palMsg);

          int doSendLogonPalMsg();
          int doSendAuthenticatePalMsg(
                const QString& username, const QString& password);

          void doGetBackgroundAsync(
                const QString& url, QMap<QString, QString> headers);
          //int doHttpPutAsync_(QString& url);
          //int doHttpPostAsync_(QString& url);

          QImage myBackgroundImg;
          QNetworkAccessManager myHttpGetMgr;
          //PalMsg myPalMsgRx;
          //QDataStream *myPalMsgRxDs;
          //PalMsg myPalMsgTx;
          //QDataStream *myPalMsgTxDs;
          PalMsg myPalMsg;
          int myPransferTimerId;
          QTimer myPingTimer;
          QTime myPongTime;
          //QByteArray myBuffer;
          PalCommon::ByteOrder myByteOrder;
          PalClient::ConnectionState myState;
          QTcpSocket mySocket;
          QString myUsername;
          QString myHost;
          quint16 myPort;
          PalServer myServer;
          PalUser myUser;
          PalRoom myCurrentRoom;
          PalCrypto myCrypto;
          QTimer myTimer;
          int myTransferTimerId;

      private slots:
          void doOnReadyRead();
          void doOnSocketError();
          void doOnPingTimer(QTimerEvent *pingTimeEvent);
          void doOnGotBackgroundAsync(QNetworkReply *reply);
   };
}

#endif
