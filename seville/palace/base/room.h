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

      private:
         u16 my_id;
         QByteArray my_byteArrayBackgroundImage;
         u32 my_flags;
         u32 my_face;
         u16 my_imageCount;
         u16 my_drawCommandsCount;
         u16 my_userCount;
         u16 my_loosePropCount;
         u16 my_reserved;
         QString my_backgroundImageName;

         void do_setupEvents(void)
         {
         }

         void do_reset(void)
         {
            my_id = 0;
            my_flags = 0;
            my_face = 0;
            my_imageCount = 0;
            my_drawCommandsCount = 0;
            my_userCount = 0;
            my_loosePropCount = 0;
            my_reserved = 0;
            my_backgroundImageName = "";

            my_byteArrayBackgroundImage.truncate(0);
         }

         void do_setBackgroundImageBytes(const ByteArray& backgroundImageBytes)
         {
            my_byteArrayBackgroundImage.clear();
            my_byteArrayBackgroundImage.resize(0);
            my_byteArrayBackgroundImage.append(backgroundImageBytes);

            emit backgroundChanged();
         }

         void do_assign(const Room& room)
         {
            my_id = room.my_id;
            my_byteArrayBackgroundImage = room.my_byteArrayBackgroundImage;
            //my_backgroundImage = room.my_backgroundImage;
         }

         void do_deinit(void)
         {
         }

         void do_init(const Room& room)
         {
            do_assign(room);
            do_setupEvents();
         }

         void do_init(void)
         {
            do_reset();
            do_setupEvents();
         }

      signals:
         void backgroundChanged(void);

      public:
         u16 id(void) const { return my_id; }
         void setId(const u16& value) { my_id = value; }

         u32 flags(void) const { return my_flags; }
         void setFlags(const u32& value) { my_flags = value; }

         void setBackgroundImageName(const QString& backgroundImageName)
            { my_backgroundImageName = backgroundImageName; }
         QString backgroundImageName(void) { return my_backgroundImageName; }

         void setFromRoomDescription(const netmsg::RoomDescription& roomDescription)
         {
            my_id = roomDescription.roomId();
            my_flags = roomDescription.flags();
            my_face = roomDescription.face();
            my_reserved = roomDescription.reserved();
            my_userCount = roomDescription.roomUserCount();
            my_imageCount = roomDescription.roomImageCount();
            my_loosePropCount = roomDescription.loosePropCount();
            my_drawCommandsCount = roomDescription.drawCommandsCount();
            my_backgroundImageName = roomDescription.backgroundImageName();

            //do_setBackgroundImageBytes();
         }

         void setBackgroundImage(const QImage& backgroundImage)
         {
            auto bits = backgroundImage.bits();
            my_byteArrayBackgroundImage.clear();
            my_byteArrayBackgroundImage.resize(0);
            my_byteArrayBackgroundImage.append(reinterpret_cast<const char*>(bits));

            emit backgroundChanged();
         }

         QImage backgroundImage(void) const
         {
            return QImage::fromData(my_byteArrayBackgroundImage);
         }

         void setBackgroundImageBytes(const Zed::ByteArray& backgroundImageBytes)
         {
            do_setBackgroundImageBytes(backgroundImageBytes);
         }

         QByteArray backgroundImageBytes(void) const
         {
            return my_byteArrayBackgroundImage;
         }

         QByteArray* backgroundImageBytesP(void)
         {
            return &my_byteArrayBackgroundImage;
         }

         void reset(void)
         {
            do_reset();
         }

         virtual ~Room(void);

         explicit Room(QObject* pParent = nullptr);
         Room(const Room& room, QObject* pParent = nullptr);
      };
   }
}

#endif // SEVILLE_PALACE_ROOM_H_
