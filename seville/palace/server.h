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

         inline void clear(void) {
            do_clear();
         }

         inline HostByteOrder byteOrder(void) const {
            return my_byteOrder;
         }

         inline void setByteOrder(int byteOrder) {
            my_byteOrder = static_cast<HostByteOrder>(byteOrder);
         }

         inline void setByteOrder(const HostByteOrder& byteOrder) {
            my_byteOrder = byteOrder;
         }

         inline std::vector<Room>* roomListPtr(void) {
            return &my_roomList; // .get();
         }

//         inline auto setRoomListUniquePtr(
//               std::unique_ptr<QList<Room>> roomListUnquePtr) -> void {
//            my_roomList = std::move(roomListUnquePtr);
//         }

         inline std::vector<User>* userListPtr(void) {
            return &my_userList; // .get();
         }

//         inline auto setUserListUniquePtr(
//               std::unique_ptr<QList<User>> userListUniquePtr)
//               -> void {
//            my_userList = std::move(userListUniquePtr);
//         }

         inline User userWithId(u32 userId) {
            for (auto& user: my_userList) {
               if (user.id() == userId) {
                  return user;
               }
            }

            return User();
         }

         inline User* userPtrWithId(u32 userId) {
            for (auto& user: my_userList) {
               if (user.id() == userId) {
                  return &user;
               }
            }

            return nullptr;
         }

         inline void removeUserWithId(u32 userId) {
            auto i = u32{0};
            auto z = my_userList.size();
            while (i < z) {
               if (my_userList[i].id() == userId) {
                  my_userList.erase(my_userList.begin() + i);
               }
               i++;
            }
         }

         inline QString hostname(void) const {
            return my_host;
         }

         inline void setHost(const QString& host) {
            my_host = host;
         }

         inline u16 port(void) const {
            return my_port;
         }

         inline void setPort(const u16 port) {
            my_port = port;
         }

         inline u32 version(void) {
            return my_version;
         }

         inline void setVersion(const u32 version) {
            my_version = version;
         }

         inline QString httpServerLocation(void) {
            return my_httpServerLocation;
         }

         inline void setHttpServerLocation(
               const QString& httpServerLocation) {
            my_httpServerLocation = httpServerLocation;
         }

         inline QString httpPropLocation(void) {
            return QString("%1%2")
                  .arg(my_httpServerLocation)
                  .arg("webservice/props/get/");
         }

      private:
         HostByteOrder my_byteOrder;
         std::vector<Room> my_roomList;
         std::vector<User> my_userList;
         QString my_host;
         u16 my_port;
         u32 my_version;
         QString my_httpServerLocation;

         inline void do_clear(void) {
            my_roomList.clear();
            my_userList.clear();
         }

         inline void do_deinit(void) {
         }

         inline void do_init(void) {
            // my_roomList = std::make_unique<QList<Room>>();
            // my_userList = std::make_unique<QList<User>>();

            do_clear();
         }
      };
   }
}

#endif // PALACE_SERVER_H_
