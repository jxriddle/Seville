#ifndef SEVILLE_PALACE_ROOM_H_
#define SEVILLE_PALACE_ROOM_H_

#include <QObject>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "seville/base/bytearray.h"
#include "seville/base/types.h"

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
         static auto New(QObject* object_parent_ptr = nullptr) -> Room*
         {
            auto instance_ptr = new Room(object_parent_ptr);

            instance_ptr->do_reset_();
            instance_ptr->do_setup_events_();

            return instance_ptr;
         }

         explicit Room(QObject* object_parent_ptr = nullptr);
         //Room(const Room& room, QObject* object_parent_ptr = nullptr);
         //virtual ~Room(void);

         inline auto room_id(void) const -> u16 {
            return my_room_id_;
         }

         inline auto set_room_id(u16 value) -> void {
            my_room_id_ = value;
         }

         inline auto flags(void) const -> u32 {
            return my_flags_;
         }

         inline auto set_flags(u32 value) -> void {
            my_flags_ = value;
         }

         inline auto face(void) const -> u32 {
            return my_face_;
         }

         inline auto set_face(u32 value) -> void {
            my_face_ = value;
         }

         inline auto reserved(void) const -> u16 {
            return my_reserved_;
         }

         inline auto set_reserved(u16 value) -> void {
            my_reserved_ = value;
         }

         inline auto user_count(void) const -> u16 {
            return my_user_count_;
         }

         inline auto set_user_count(u16 value) -> void {
            my_user_count_ = value;
         }

         inline auto hotspot_count(void) const -> u16 {
            return my_hotspot_count_;
         }

         inline auto set_hotspot_count(u16 value) -> void {
            my_hotspot_count_ = value;
         }

         inline auto image_count(void) const -> u16 {
            return my_image_count_;
         }

         inline auto set_image_count(u16 value) -> void {
            my_image_count_ = value;
         }

         inline auto loose_prop_count(void) const -> u16 {
            return my_loose_prop_count_;
         }

         inline auto set_loose_prop_count(u16 value) -> void {
            my_loose_prop_count_ = value;
         }

         inline auto draw_commands_count(void) const -> u16 {
            return my_draw_commands_count_;
         }

         inline auto set_draw_commands_count(u16 value) -> void {
            my_draw_commands_count_ = value;
         }

         inline auto room_name(void) const -> QString {
            return my_room_name_;
         }

         inline auto set_room_name(const QString& value) {
            my_room_name_ = value;
         }

         inline auto background_image_name(void) -> QString {
            return my_background_image_name_;
         }

         inline auto set_background_image_name(
               const QString& background_image_name) -> void {
            my_background_image_name_ = background_image_name;
         }

         inline auto background_image(void) const -> QImage {
            return QImage::fromData(my_background_image_byte_array_);
         }

         inline auto set_background_image(
               const QImage& background_image) -> void {
            auto bits = background_image.bits();
            my_background_image_byte_array_.clear();
            my_background_image_byte_array_.resize(0);
            my_background_image_byte_array_.append(
                     reinterpret_cast<const char*>(bits));

            emit background_did_change();
         }

         inline auto background_image_bytes(void) const -> QByteArray {
            return my_background_image_byte_array_;
         }

         inline auto set_background_image_bytes(
               const ByteArray& background_image_bytes) -> void {
            do_set_background_image_bytes_(background_image_bytes);
         }

         inline auto background_image_bytes_ptr(void) -> QByteArray* {
            return &my_background_image_byte_array_;
         }

         inline auto reset(void) -> void {
            do_reset_();
         }

      private:
         u32 my_flags_;
         u32 my_face_;
         u16 my_room_id_;
         u16 my_hotspot_count_;
         u16 my_image_count_;
         u16 my_draw_commands_count_;
         u16 my_user_count_;
         u16 my_loose_prop_count_;
         u16 my_reserved_;
         QByteArray my_background_image_byte_array_;
         QString my_background_image_name_;
         QString my_room_name_;

         auto do_setup_events_(void) -> void
         {
         }

         void do_reset_(void)
         {
            my_room_id_ = 0;
            my_flags_ = 0;
            my_face_ = 0;
            my_image_count_ = 0;
            my_draw_commands_count_ = 0;
            my_user_count_ = 0;
            my_loose_prop_count_ = 0;
            my_reserved_ = 0;
            my_background_image_name_ = "";

            my_background_image_byte_array_.truncate(0);
         }

         void do_set_background_image_bytes_(
               const ByteArray& backgroundImageBytes)
         {
            my_background_image_byte_array_.clear();
            my_background_image_byte_array_.resize(0);
            my_background_image_byte_array_.append(backgroundImageBytes);

            emit background_did_change();
         }

         void do_assign_(const Room& room)
         {
            my_room_id_ = room.my_room_id_;
            my_background_image_byte_array_ =
                  room.my_background_image_byte_array_;
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
