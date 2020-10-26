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
         void room_list_did_load(void);
         void user_list_did_load(void);

      public:
         Server(void);
         virtual ~Server(void);

         static const u16 kDefaultPort = 9998;

         inline auto reset(void) -> void {
            do_reset_();
         }

         inline auto byteorder(void) const -> Host::ByteOrder {
            return my_byteorder_;
         }

         inline auto set_byte_order(const Host::ByteOrder& byteorder) -> void {
            my_byteorder_ = byteorder;
         }

         inline auto room_list_ptr(void) -> QList<Room>* {
            return my_room_list_unique_ptr_.get();
         }

         inline auto set_room_list_unique_ptr(std::unique_ptr<QList<Room>> room_list_unque_ptr) -> void {
            my_room_list_unique_ptr_ = std::move(room_list_unque_ptr);
         }

         inline auto user_list_ptr(void) -> QList<User>* {
            return my_user_list_unique_ptr_.get();
         }

         inline auto set_user_list_unique_ptr(std::unique_ptr<QList<User>> user_list_unique_ptr) -> void {
            my_user_list_unique_ptr_ = std::move(user_list_unique_ptr);
         }

         inline auto host(void) const -> QString {
            return my_host_;
         }

         inline auto set_host(const QString& host) -> void {
            my_host_ = host;
         }

         inline auto port(void) const -> u16 {
            return my_port_;
         }

         inline auto set_port(const u16 port) -> void {
            my_port_ = port;
         }

         inline auto version(void) -> u32 {
            return my_version_;
         }

         inline auto set_version(const u32 version) -> void {
            my_version_ = version;
         }

         inline auto http_server_location(void) -> QString {
            return my_string_http_server_location_;
         }

         inline auto set_http_server_location(const QString& http_server_location) -> void {
            my_string_http_server_location_ = http_server_location;
         }

      private:
         Host::ByteOrder my_byteorder_;
         std::unique_ptr<QList<Room>> my_room_list_unique_ptr_;
         std::unique_ptr<QList<User>> my_user_list_unique_ptr_;
         QString my_host_;
         u16 my_port_;
         u32 my_version_;
         QString my_string_http_server_location_;

         inline auto do_reset_(void) -> void {
            my_room_list_unique_ptr_->clear();
            my_user_list_unique_ptr_->clear();
         }

         inline auto do_deinit_(void) -> void {
         }

         inline auto do_init_(void) -> void {
            my_room_list_unique_ptr_ = std::make_unique<QList<Room>>();
            my_user_list_unique_ptr_ = std::make_unique<QList<User>>();

            do_reset_();
         }
      };
   }
}

#endif // PALACE_SERVER_H_
