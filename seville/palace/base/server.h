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

      signals:
         auto roomListLoaded(void) -> void;
         auto userListLoaded(void) -> void;

      public:
         Server(void);
         virtual ~Server(void);

         static const u16 kDefaultPort = 9998;

         inline auto reset(void) -> void {
            do_reset();
         }

         inline auto set_byte_order(const Host::ByteOrder& byteOrder) -> void {
            my_byteorder_ = byteOrder;
         }

         inline auto byteOrder() const -> Host::ByteOrder {
            return my_byteorder_;
         }

         inline auto setRoomList(std::unique_ptr<QList<Room>> puRoomList) -> void {
            my_room_list_unique_pointer_ = std::move(puRoomList);
         }

         inline auto roomListPointer() -> QList<Room>* {
            return my_room_list_unique_pointer_.get();
         }

         inline auto setUserList(std::unique_ptr<QList<User>> puUserList) -> void {
            my_user_list_unique_pointer_ = std::move(puUserList);
         }

         auto userListPointer() -> QList<User>* {
            return my_user_list_unique_pointer_.get();
         }

         inline auto setHost(const QString& host) -> void {
            my_host_ = host;
         }

         inline auto host() const -> QString {
            return my_host_;
         }

         inline auto setPort(const u16 port) -> void {
            my_port_ = port;
         }

         inline auto port() const -> u16 {
            return my_port_;
         }

         inline auto setVersion(const u32 version) -> void {
            my_version_ = version;
         }

         inline auto version(void) -> u32 {
            return my_version_;
         }

         inline auto setHttpServerLocation(const QString& httpServerLocation) -> void {
            my_http_server_location_string_ = httpServerLocation;
         }

         inline auto httpServerLocation(void) -> QString {
            return my_http_server_location_string_;
         }

      private:
         Host::ByteOrder my_byteorder_;
         std::unique_ptr<QList<Room>> my_room_list_unique_pointer_;
         std::unique_ptr<QList<User>> my_user_list_unique_pointer_;
         QString my_host_;
         u16 my_port_;
         u32 my_version_;
         QString my_http_server_location_string_;

         inline auto do_reset(void) -> void {
            my_room_list_unique_pointer_->clear();
            my_user_list_unique_pointer_->clear();
         }

         inline auto do_deinit(void) -> void {
         }

         inline auto do_init(void) -> void {
            my_room_list_unique_pointer_ = std::make_unique<QList<Room>>();
            my_user_list_unique_pointer_ = std::make_unique<QList<User>>();

            do_reset();
         }
      };
   }
}

#endif // PALACE_SERVER_H_
