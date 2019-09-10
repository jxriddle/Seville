#ifndef PALSERVER_H
#define PALSERVER_H

#include <QObject>
#include <QList>

#include "pal_common.h"
#include "pal_room.h"
#include "pal_user.h"

namespace Seville
{
   class PalServer : QObject
   {
       Q_OBJECT

      public:
         explicit PalServer();
         virtual ~PalServer();
         void resetState();
         PalCommon::ByteOrder byteOrder() const;
         void setByteOrder(PalCommon::ByteOrder byteOrder);
         QList<PalRoom> roomList() const;
         void setRoomList(QList<PalRoom> roomList);
         QList<PalUser> userList() const;
         void setUserList(QList<PalUser> userList);
         QString host() const;
         void setHost(const QString& host);
         quint16 port() const;
         void setPort(quint16 port);

      private:
         void doResetState_();

         PalCommon::ByteOrder byteOrder_;
         QList<PalRoom> roomList_;
         QList<PalUser> userList_;
         QString host_;
         quint16 port_;
   };
}

#endif // PALSERVER_H
