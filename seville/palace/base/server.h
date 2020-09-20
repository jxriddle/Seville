#ifndef PALACE_SERVER_H_
#define PALACE_SERVER_H_

#include <QObject>
#include <QList>

#include "seville/base/types.h"

#include "seville/palace/base/host.h"
#include "seville/palace/base/room.h"
#include "seville/palace/base/user.h"

namespace seville
{
   namespace palace
   {
      class Server : QObject
      {
      Q_OBJECT

      private:
         Host::ByteOrder my_byteOrder;
         std::unique_ptr<QList<Room>> my_puRoomList;
         std::unique_ptr<QList<User>> my_puUserList;
         QString my_host;
         u16 my_port;
         u32 my_version;
         QString my_httpServerLocation;

         void do_reset(void)
         {
            my_puRoomList->clear();
            my_puUserList->clear();
         }

         void do_deinit(void)
         {
         }

         void do_init(void)
         {
            my_puRoomList = std::make_unique<QList<Room>>();
            my_puUserList = std::make_unique<QList<User>>();

            do_reset();
         }

      signals:
         void roomListLoaded(void);
         void userListLoaded(void);

      public:
         static const u16 kDefaultPort = 9998;

         void reset(void) { do_reset(); }

         void setByteOrder(const Host::ByteOrder& byteOrder)
         {
            my_byteOrder = byteOrder;
         }

         Host::ByteOrder byteOrder() const
         {
            return my_byteOrder;
         }

         void setRoomList(std::unique_ptr<QList<Room>> puRoomList)
         {
            my_puRoomList = std::move(puRoomList);
         }

         QList<Room>* roomListP()
         {
            return my_puRoomList.get();
         }

         void setUserList(std::unique_ptr<QList<User>> puUserList)
         {
            my_puUserList = std::move(puUserList);
         }

         QList<User>* userListP()
         {
            return my_puUserList.get();
         }

         void setHost(const QString& host)
         {
            my_host = host;
         }

         QString host() const
         {
            return my_host;
         }

         void setPort(const u16 port)
         {
            my_port = port;
         }

         u16 port() const
         {
            return my_port;
         }

         void setVersion(const u32 version)
         {
            my_version = version;
         }

         u32 version(void)
         {
            return my_version;
         }

         void setHttpServerLocation(const QString& httpServerLocation)
         {
            my_httpServerLocation = httpServerLocation;
         }

         QString httpServerLocation(void)
         {
            return my_httpServerLocation;
         }

         virtual ~Server();
         Server();
      };
   }
}

#endif // PALACE_SERVER_H_
