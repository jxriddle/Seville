#ifndef PALACE_SERVER_H_
#define PALACE_SERVER_H_

#include <QObject>
#include <QList>

#include "seville/base/types.h"

#include "seville/palace/host.h"
#include "seville/palace/room.h"
#include "seville/palace/user.h"

namespace seville
{
   namespace palace
   {
      class Server : QObject
      {
      Q_OBJECT

      signals:
         void roomListDidLoadEvent(void);
         void userListDidLoadEvent(void);

      public:
         Server(void);
         virtual ~Server(void);

         static const u16 kDefaultPort = 9998;

         inline auto clear(void) -> void {
            do_clear();
         }

         inline auto byteOrder(void) const -> HostByteOrder {
            return my_byteOrder;
         }

         inline auto setByteOrder(int byteOrder) -> void {
            my_byteOrder = static_cast<HostByteOrder>(byteOrder);
         }

         inline auto setByteOrder(const HostByteOrder& byteOrder) -> void {
            my_byteOrder = byteOrder;
         }

         inline auto roomListPtr(void) -> QList<Room>* {
            return my_roomListUniquePtr.get();
         }

         inline auto setRoomListUniquePtr(
               std::unique_ptr<QList<Room>> roomListUnquePtr) -> void {
            my_roomListUniquePtr = std::move(roomListUnquePtr);
         }

         inline auto userListPtr(void) -> QList<User>* {
            return my_userListUniquePtr.get();
         }

         inline auto setUserListUniquePtr(
               std::unique_ptr<QList<User>> userListUniquePtr)
               -> void {
            my_userListUniquePtr = std::move(userListUniquePtr);
         }

         inline auto hostname(void) const -> QString {
            return my_host;
         }

         inline auto setHost(const QString& host) -> void {
            my_host = host;
         }

         inline auto port(void) const -> u16 {
            return my_port;
         }

         inline auto setPort(const u16 port) -> void {
            my_port = port;
         }

         inline auto version(void) -> u32 {
            return my_version;
         }

         inline auto setVersion(const u32 version) -> void {
            my_version = version;
         }

         inline auto httpServerLocation(void) -> QString {
            return my_httpServerLocation;
         }

         inline auto setHttpServerLocation(
               const QString& http_server_location) -> void {
            my_httpServerLocation = http_server_location;
         }

      private:
         HostByteOrder my_byteOrder;
         std::unique_ptr<QList<Room>> my_roomListUniquePtr;
         std::unique_ptr<QList<User>> my_userListUniquePtr;
         QString my_host;
         u16 my_port;
         u32 my_version;
         QString my_httpServerLocation;

         inline auto do_clear(void) -> void {
            my_roomListUniquePtr->clear();
            my_userListUniquePtr->clear();
         }

         inline auto do_deinit(void) -> void {
         }

         inline auto do_init(void) -> void {
            my_roomListUniquePtr = std::make_unique<QList<Room>>();
            my_userListUniquePtr = std::make_unique<QList<User>>();

            do_clear();
         }
      };
   }
}

#endif // PALACE_SERVER_H_
