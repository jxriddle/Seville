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

         inline void setByteOrder(i32 value) {
            my_byteOrder = static_cast<HostByteOrder>(value);
         }

         inline void setByteOrder(const HostByteOrder& value) {
            my_byteOrder = value;
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

         inline User userWithId(i32 value) {
            for (auto& user: my_userList) {
               if (user.id() == value) {
                  return user;
               }
            }

            return User();
         }

         inline User* userPtrWithId(i32 value) {
            for (auto& user: my_userList) {
               if (user.id() == value) {
                  return &user;
               }
            }

            return nullptr;
         }

         inline void removeUserWithId(i32 value) {
            auto i = u32{0};
            auto z = my_userList.size();
            while (i < z) {
               if (my_userList[i].id() == value) {
                  my_userList.erase(my_userList.begin() + i);
               }
               i++;
            }
         }

         inline QString hostname(void) const {
            return my_hostname;
         }

         inline void setHostname(const QString& value) {
            my_hostname = value;
         }

         inline u16 port(void) const {
            return my_port;
         }

         inline void setPort(const u16 value) {
            my_port = value;
         }

         inline u32 version(void) {
            return my_version;
         }

         inline void setVersion(const u32 value) {
            my_version = value;
         }

         inline i16 info(void) {
            return my_info;
         }

         void setInfo(const i16 value) {
            my_info = value;
         }

         inline QString httpServerLocation(void) {
            return my_httpServerLocation;
         }

         inline void setHttpServerLocation(const QString& value) {
            my_httpServerLocation = value;
         }

         inline QString httpPropListLocation(void) {
            return QString("%1%2")
                  .arg(my_httpServerLocation)
                  .arg("webservice/props/get/");
         }

         inline QString httpPropStorageLocation(void) {
            return my_httpPropStorageLocation;
         }

         inline void setHttpPropStorageLocation(const QString& value) {
            my_httpPropStorageLocation = value;
         }

         inline i32 charset(void) {
            return my_charset;
         }

         inline void setCharset(i32 value) {
            my_charset = value;
         }

         inline QString name(void) {
            return my_name;
         }

         inline void setName(const QString& value) {
            my_name = value;
         }

      private:
         HostByteOrder my_byteOrder;
         std::vector<Room> my_roomList;
         std::vector<User> my_userList;
         QString my_hostname;
         u16 my_port;
         u32 my_version;
         QString my_httpServerLocation;
         QString my_httpPropStorageLocation;
         i16 my_info;
         i32 my_charset;
         QString my_name;

         inline void do_clear(void) {
            my_byteOrder = HostByteOrder::kUnknownEndian;
            my_roomList.clear();
            my_userList.clear();
            my_hostname = "";
            my_port = 9998;
            my_httpServerLocation = "";
            my_httpPropStorageLocation = "";
            my_info = 0;
            my_charset = 0;
         }

         inline void do_deinit(void) {
         }

         inline void do_init(void) {
         }
      };
   }
}

#endif // PALACE_SERVER_H_
