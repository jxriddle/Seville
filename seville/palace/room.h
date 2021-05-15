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
         static Room* New(QObject* parentObjectPtr = nullptr)
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

         inline u16 roomId(void) const {
            return my_roomId;
         }

         inline void setId(u16 value) {
            my_roomId = value;
         }

         inline u32 flags(void) const {
            return my_flags;
         }

         inline void setFlags(u32 value) {
            my_flags = value;
         }

         inline u32 face(void) const {
            return my_face;
         }

         inline void setFace(u32 value) {
            my_face = value;
         }

         inline i32 reserved(void) const {
            return my_reserved;
         }

         inline void setReserved(i32 value) {
            my_reserved = value;
         }

         inline i32 userCount(void) const {
            //return my_userList.size();
            return my_userCount;
         }

         inline void setUserCount(i32 value) {
            my_userCount = value;
         }

         inline i32 hotspotCount(void) const {
            return my_hotspotCount;
         }

         inline void setHotspotCount(i32 value) {
            my_hotspotCount = value;
         }

         inline std::vector<Hotspot>* hotspotListPtr(void) {
            return &my_hotspotList;
         }

         inline std::vector<User>* userListPtr(void) {
            return &my_userList;
         }

         //inline auto setHotspotsUniquePtr(
         //      std::unique_ptr<QList<Hotspot>> hotspotsUniquePtr) -> void
         //{
         //   my_hotspotsUniquePtr = std::move(hotspotsUniquePtr);
         //}

         inline i32 imageCount(void) const {
            return my_imageCount;
         }

         inline void setImageCount(i32 value) {
            my_imageCount = value;
         }

         inline i32 loosePropCount(void) const {
            return my_loosePropCount;
         }

         inline void setLoosePropCount(i32 value) {
            my_loosePropCount = value;
         }

         inline i32 drawCommandsCount(void) const {
            return my_drawCommandsCount;
         }

         inline void setDrawCommandsCount(i32 value) {
            my_drawCommandsCount = value;
         }

         inline QString roomName(void) const {
            return my_roomName;
         }

         inline void setRoomName(const QString& value) {
            my_roomName = value;
         }

         inline QString backgroundImageName(void) {
            return my_backgroundImageName;
         }

         inline void setBackgroundImageName(
               const QString& backgroundImageName) {
            my_backgroundImageName = backgroundImageName;
         }

         inline QImage backgroundImage(void) const {
            return QImage::fromData(my_backgroundImageByteArray);
         }

         inline void setBackgroundImage(
               const QImage& backgroundImage) {
            auto bits = backgroundImage.bits();
            my_backgroundImageByteArray.clear();
            my_backgroundImageByteArray.resize(0);
            my_backgroundImageByteArray.append(
                     reinterpret_cast<const char*>(bits));

            emit backgroundDidChangeEvent();
         }

         inline QByteArray backgroundImageByteArray(void) const {
            return my_backgroundImageByteArray;
         }

         inline void setBackgroundImageByteArray(
               const QByteArray& backgroundImageBytes) {
            do_setBackgroundImageByteArray(backgroundImageBytes);
         }

         inline QByteArray* backgroundImageByteArrayPtr(void) {
            return &my_backgroundImageByteArray;
         }

         inline QString artistName(void) {
            return my_artistName;
         }

         inline void setArtistName(const QString& artistName) {
            my_artistName = artistName;
         }

         inline QString password(void) {
            return my_password;
         }

         inline void setPassword(const QString& password) {
            my_password = password;
         }

         inline void clear(void) {
            do_clear();
         }

      private:
         u32 my_flags;
         u32 my_face;
         u16 my_roomId;
         i32 my_hotspotCount;
         i32 my_imageCount;
         i32 my_drawCommandsCount;
         i32 my_userCount;
         i32 my_loosePropCount;
         i32 my_reserved;
         QByteArray my_backgroundImageByteArray;
         QString my_backgroundImageName;
         QString my_roomName;
         QString my_artistName;
         QString my_password;
         std::vector<Hotspot> my_hotspotList;
         std::vector<User> my_userList;
         std::vector<Prop> my_loosePropList;

         void do_setupEvents(void);
         void do_clear(void);
         void do_setBackgroundImageByteArray(
               const QByteArray& backgroundImageByteArray);
         void do_fetchWebPropsAsync(
               QString propServerUri, std::vector<i32>propIds);
         void do_assign(const Room& room);
         void do_init(void); //const Room& room)
         void do_deinit(void);
      };
   }
}

#endif // SEVILLE_PALACE_ROOM_H_
