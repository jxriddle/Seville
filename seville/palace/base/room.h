#ifndef SEVILLE_PALACE_ROOM_H_
#define SEVILLE_PALACE_ROOM_H_

#include <QObject>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "seville/base/types.h"
#include "seville/palace/netmsg/roomdescription.h"

namespace seville
{
   namespace palace
   {
      class Room : public QObject
      {
      Q_OBJECT

      signals:
         void background_did_change(void);

      public:
         static auto New(QObject* object_parent_ptr = nullptr) ->
               std::unique_ptr<Room>
         {
            auto instance_ptr = std::make_unique<Room>(object_parent_ptr);

            instance_ptr->do_reset_();
            instance_ptr->do_setup_events_();

            return instance_ptr;
         }

         explicit Room(QObject* object_parent_ptr = nullptr);
         Room(const Room& room, QObject* object_parent_ptr = nullptr);
         //virtual ~Room(void);

         inline auto id(void) const -> u16 {
            return my_id_;
         }

         inline auto set_id(const u16& value) -> void {
            my_id_ = value;
         }

         inline auto flags(void) const -> u32 {
            return my_flags_;
         }

         inline auto set_flags(const u32& value) -> void {
            my_flags_ = value;
         }

         inline auto set_background_image_name(
               const QString& background_image_name) -> void {
            my_name_image_background_ = background_image_name;
         }

         inline auto background_image_name(void) -> QString {
            return my_name_image_background_;
         }

         inline auto set_from_room_description(
               const netmsg::RoomDescription& room_description) -> void {
            my_id_ = room_description.room_id();
            my_flags_ = room_description.flags();
            my_face_ = room_description.face();
            my_reserved_ = room_description.reserved();
            my_count_users_ = room_description.room_user_count();
            my_count_images_ = room_description.room_image_count();
            my_count_props_loose_ = room_description.loose_prop_count();
            my_count_commands_draw_ = room_description.draw_commands_count();
            my_name_image_background_ =
                  room_description.background_image_name();

            //do_setBackgroundImageBytes();
         }

         inline auto set_background_image(
               const QImage& background_image) -> void {
            auto bits = background_image.bits();
            my_byte_array_image_background_.clear();
            my_byte_array_image_background_.resize(0);
            my_byte_array_image_background_.append(
                     reinterpret_cast<const char*>(bits));

            emit background_did_change();
         }

         inline auto background_image(void) const -> QImage {
            return QImage::fromData(my_byte_array_image_background_);
         }

         inline auto set_background_image_bytes(
               const ByteArray& background_image_bytes) -> void {
            do_set_background_image_bytes_(background_image_bytes);
         }

         inline auto background_image_bytes(void) const -> QByteArray {
            return my_byte_array_image_background_;
         }

         inline auto background_image_bytes_ptr(void) -> QByteArray* {
            return &my_byte_array_image_background_;
         }

         inline auto reset(void) -> void {
            do_reset_();
         }

      private:
         u16 my_id_;
         QByteArray my_byte_array_image_background_;
         u32 my_flags_;
         u32 my_face_;
         u16 my_count_images_;
         u16 my_count_commands_draw_;
         u16 my_count_users_;
         u16 my_count_props_loose_;
         u16 my_reserved_;
         QString my_name_image_background_;

         auto do_setup_events_(void) -> void
         {
         }

         void do_reset_(void)
         {
            my_id_ = 0;
            my_flags_ = 0;
            my_face_ = 0;
            my_count_images_ = 0;
            my_count_commands_draw_ = 0;
            my_count_users_ = 0;
            my_count_props_loose_ = 0;
            my_reserved_ = 0;
            my_name_image_background_ = "";

            my_byte_array_image_background_.truncate(0);
         }

         void do_set_background_image_bytes_(
               const ByteArray& backgroundImageBytes)
         {
            my_byte_array_image_background_.clear();
            my_byte_array_image_background_.resize(0);
            my_byte_array_image_background_.append(backgroundImageBytes);

            emit background_did_change();
         }

         void do_assign_(const Room& room)
         {
            my_id_ = room.my_id_;
            my_byte_array_image_background_ =
                  room.my_byte_array_image_background_;
            //my_backgroundImage = room.my_backgroundImage;
         }

         void do_init_() //const Room& room)
         {
            //do_assign_(room);
            do_setup_events_();
         }
      };
   }
}

#endif // SEVILLE_PALACE_ROOM_H_
