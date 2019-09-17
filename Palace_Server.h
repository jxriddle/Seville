#ifndef PALACE_SERVER_H
#define PALACE_SERVER_H

#include <QObject>
#include <QList>

#include "Palace_Host.h"
#include "Palace_Room.h"
#include "Palace_User.h"

namespace Seville
{
   namespace Palace
   {
      class Server : QObject
      {
          Q_OBJECT

         private:
            Host::ByteOrder myByteOrder;
            QList<Room> myRoomList;
            QList<User> myUserList;
            QString myHost;
            quint16 myPort;

            void doReset();

         public:
            static const int kDefaultPort = 9998;

            Host::ByteOrder byteOrder() const;
            void setByteOrder(Host::ByteOrder byteOrder);
            QList<Room> roomList() const;
            void setRoomList(QList<Room> roomList);
            QList<User> userList() const;
            void setUserList(QList<User> userList);
            QString host() const;
            void setHost(const QString& host);
            quint16 port() const;
            void setPort(quint16 port);

            void reset();

            virtual ~Server();
            Server();
      };
   }
}

#endif // PALACE_SERVER_H
