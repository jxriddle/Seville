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
         void backgroundChanged(void);

      public:
         static auto New(QObject* object_parent_ptr = nullptr) -> \
               std::unique_ptr<Room>
         {
            auto instance_ptr = std::make_unique<Room>(object_parent_ptr);

            instance_ptr->do_reset();
            instance_ptr->do_setupEvents();

            return instance_ptr;
         }

      public:
         explicit Room(QObject* object_parent_ptr = nullptr);
         Room(const Room& room, QObject* object_parent_ptr = nullptr);

         virtual ~Room(void);

         inline auto id(void) const -> u16 {
            return my_id_;
         }

         inline auto setId(const u16& value) -> void {
            my_id_ = value;
         }

         inline auto flags(void) const -> u32 {
            return my_flags_;
         }

         inline auto setFlags(const u32& value) -> void {
            my_flags_ = value;
         }

         inline auto setBackgroundImageName( \
               const QString& background_image_name) -> void {
            my_name_image_background_ = background_image_name;
         }

         inline auto backgroundImageName(void) -> QString {
            return my_name_image_background_;
         }

         inline auto setFromRoomDescription( \
               const netmsg::RoomDescription& room_description) -> void {
            my_id_ = room_description.roomId();
            my_flags_ = room_description.flags();
            my_face_ = room_description.face();
            my_reserved_ = room_description.reserved();
            my_count_users_ = room_description.roomUserCount();
            my_count_images_ = room_description.roomImageCount();
            my_count_props_loose_ = room_description.loosePropCount();
            my_count_commands_draw_ = room_description.drawCommandsCount();
            my_name_image_background_ = room_description.backgroundImageName();

            //do_setBackgroundImageBytes();
         }

         inline auto setBackgroundImage( \
               const QImage& background_image) -> void {
            auto bits = background_image.bits();
            my_bytearray_image_background_.clear();
            my_bytearray_image_background_.resize(0);
            my_bytearray_image_background_.append( \
                     reinterpret_cast<const char*>(bits));

            emit backgroundChanged();
         }

         inline auto backgroundImage(void) const -> QImage {
            return QImage::fromData(my_bytearray_image_background_);
         }

         inline auto setBackgroundImageBytes( \
               const ByteArray& backgroundImageBytes) -> void {
            do_setBackgroundImageBytes(backgroundImageBytes);
         }

         inline auto backgroundImageBytes(void) const -> QByteArray {
            return my_bytearray_image_background_;
         }

         inline auto backgroundImageBytesP(void) -> QByteArray* {
            return &my_bytearray_image_background_;
         }

         inline auto reset(void) -> void {
            do_reset();
         }

      private:
         u16 my_id_;
         QByteArray my_bytearray_image_background_;
         u32 my_flags_;
         u32 my_face_;
         u16 my_count_images_;
         u16 my_count_commands_draw_;
         u16 my_count_users_;
         u16 my_count_props_loose_;
         u16 my_reserved_;
         QString my_name_image_background_;

         auto do_setupEvents(void) -> void
         {
         }

         void do_reset(void)
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

            my_bytearray_image_background_.truncate(0);
         }

         void do_setBackgroundImageBytes(const ByteArray& backgroundImageBytes)
         {
            my_bytearray_image_background_.clear();
            my_bytearray_image_background_.resize(0);
            my_bytearray_image_background_.append(backgroundImageBytes);

            emit backgroundChanged();
         }

         void do_assign(const Room& room)
         {
            my_id_ = room.my_id_;
            my_bytearray_image_background_ = \
                  room.my_bytearray_image_background_;
            //my_backgroundImage = room.my_backgroundImage;
         }

         void do_init(const Room& room)
         {
            do_assign(room);
            do_setupEvents();
         }
      };
   }
}

#endif // SEVILLE_PALACE_ROOM_H_
