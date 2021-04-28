#ifndef SEVILLE_PALACE_ROOM_H_
#define SEVILLE_PALACE_ROOM_H_

#include <vector>

#include <QObject>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "seville/base/types.h"
#include "seville/palace/hotspot.h"
#include "seville/palace/user.h"

namespace seville
{
   namespace palace
   {
      class Room : public QObject
      {
      Q_OBJECT

      signals:
         void backgroundDidChangeEvent(void);

      public:
         static auto New(QObject* parentObjectPtr = nullptr) -> Room*
         {
            auto instance_ptr = new Room(parentObjectPtr);

            instance_ptr->do_clear();
            instance_ptr->do_setupEvents();

            return instance_ptr;
         }

         Room(QObject* parentObjectPtr = nullptr);
         Room(const Room& room, QObject* parentObjectPtr = nullptr);

         virtual ~Room(void);

         Room& operator=(const Room& value);

         inline auto userWithId(u32 userId) -> User {
            for (auto& user: my_userList) {
               if (user.id() == userId) {
                  return user;
               }
            }

            return User();
         }

         inline auto roomId(void) const -> u16 {
            return my_roomId;
         }

         inline auto setId(u16 value) -> void {
            my_roomId = value;
         }

         inline auto flags(void) const -> u32 {
            return my_flags;
         }

         inline auto setFlags(u32 value) -> void {
            my_flags = value;
         }

         inline auto face(void) const -> u32 {
            return my_face;
         }

         inline auto setFace(u32 value) -> void {
            my_face = value;
         }

         inline auto reserved(void) const -> u16 {
            return my_reserved;
         }

         inline auto setReserved(u16 value) -> void {
            my_reserved = value;
         }

         inline auto userCount(void) const -> u16 {
            return my_userCount;
         }

         inline auto setUserCount(u16 value) -> void {
            my_userCount = value;
         }

         inline auto hotspotCount(void) const -> u16 {
            return my_hotspotCount;
         }

         inline auto setHotspotCount(u16 value) -> void {
            my_hotspotCount = value;
         }

         inline auto hotspotListPtr(void) -> std::vector<Hotspot>* {
            return &my_hotspotList;
         }

         inline auto userListPtr(void) -> std::vector<User>* {
            return &my_userList;
         }

         //inline auto setHotspotsUniquePtr(
         //      std::unique_ptr<QList<Hotspot>> hotspotsUniquePtr) -> void
         //{
         //   my_hotspotsUniquePtr = std::move(hotspotsUniquePtr);
         //}

         inline auto imageCount(void) const -> u16 {
            return my_imageCount;
         }

         inline auto setImageCount(u16 value) -> void {
            my_imageCount = value;
         }

         inline auto loosePropCount(void) const -> u16 {
            return my_loosePropCount;
         }

         inline auto setLoosePropCount(u16 value) -> void {
            my_loosePropCount = value;
         }

         inline auto drawCommandsCount(void) const -> u16 {
            return my_drawCommandsCount;
         }

         inline auto setDrawCommandsCount(u16 value) -> void {
            my_drawCommandsCount = value;
         }

         inline auto roomName(void) const -> QString {
            return my_roomName;
         }

         inline auto setRoomName(const QString& value) {
            my_roomName = value;
         }

         inline auto backgroundImageName(void) -> QString {
            return my_backgroundImageName;
         }

         inline auto setBackgroundImageName(
               const QString& backgroundImageName) -> void {
            my_backgroundImageName = backgroundImageName;
         }

         inline auto backgroundImage(void) const -> QImage {
            return QImage::fromData(my_backgroundImageByteArray);
         }

         inline auto setBackgroundImage(
               const QImage& backgroundImage) -> void {
            auto bits = backgroundImage.bits();
            my_backgroundImageByteArray.clear();
            my_backgroundImageByteArray.resize(0);
            my_backgroundImageByteArray.append(
                     reinterpret_cast<const char*>(bits));

            emit backgroundDidChangeEvent();
         }

         inline auto backgroundImageByteArray(void) const -> QByteArray {
            return my_backgroundImageByteArray;
         }

         inline auto setBackgroundImageByteArray(
               const QByteArray& backgroundImageBytes) -> void {
            do_setBackgroundImageByteArray(backgroundImageBytes);
         }

         inline auto backgroundImageByteArrayPtr(void) -> QByteArray* {
            return &my_backgroundImageByteArray;
         }

         inline auto artistName(void) -> QString {
            return my_artistName;
         }

         inline auto setArtistName(const QString& artistName) -> void {
            my_artistName = artistName;
         }

         inline auto password(void) -> QString {
            return my_password;
         }

         inline auto setPassword(const QString& password) -> void {
            my_password = password;
         }

         inline auto clear(void) -> void {
            do_clear();
         }

      private:
         u32 my_flags;
         u32 my_face;
         u16 my_roomId;
         u16 my_hotspotCount;
         u16 my_imageCount;
         u16 my_drawCommandsCount;
         u16 my_userCount;
         u16 my_loosePropCount;
         u16 my_reserved;
         QByteArray my_backgroundImageByteArray;
         QString my_backgroundImageName;
         QString my_roomName;
         QString my_artistName;
         QString my_password;
         std::vector<Hotspot> my_hotspotList;
         std::vector<User> my_userList;

         auto do_setupEvents(void) -> void;
         auto do_clear(void) -> void;
         auto do_setBackgroundImageByteArray(
               const QByteArray& backgroundImageByteArray) -> void;
         auto do_assign(const Room& room) -> void;
         auto do_init(void) -> void; //const Room& room)
         auto do_deinit(void) -> void;
      };
   }
}

#endif // SEVILLE_PALACE_ROOM_H_
