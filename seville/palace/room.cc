#include "room.h"

namespace seville
{
    namespace palace
    {
        Room::Room(QObject* object_parent_ptr)
         : QObject(object_parent_ptr)
        {
            do_init();
            do_clear();
        }

        Room::Room(const Room& room, QObject* parentObjectPtr)
            : QObject (parentObjectPtr)
        {
            do_init();
            do_clear();
            do_assign(room);
        }

        Room& Room::operator=(const Room& value)
        {
            do_assign(value);
            return *this;
        }

        Room::~Room(void)
        {
            do_deinit();
        }

        void Room::do_setupEvents(void)
        {
        }

        void Room::do_clear(void)
        {
            my_roomId = 0;
            my_flags = 0;
            my_face = 0;
            my_imageCount = 0;
            my_drawCommandsCount = 0;
            my_userCount = 0;
            my_loosePropCount = 0;
            my_hotspotCount = 0;
            my_reserved = 0;
            my_backgroundImageName = "";
            my_looseProps.clear();
            my_hotspots.clear();
            my_users.clear();

            my_backgroundImageByteArray.truncate(0);
        }

        void Room::do_setBackgroundImageByteArray(
                const QByteArray& backgroundImageByteArray)
        {
            my_backgroundImageByteArray.clear();
            my_backgroundImageByteArray.resize(0);
            my_backgroundImageByteArray.append(backgroundImageByteArray);

            emit backgroundDidChangeEvent();
        }

        void Room::do_assign(const Room& room)
        {
            my_roomId = room.my_roomId;
            my_backgroundImageByteArray =
                    room.my_backgroundImageByteArray;
            my_artistName = room.my_artistName;
            my_backgroundImageName = room.my_backgroundImageName;
            my_drawCommandsCount = room.my_drawCommandsCount;
            my_face = room.my_face;
            my_flags = room.my_flags;
            my_hotspotCount = room.my_hotspotCount;
            my_imageCount = room.my_imageCount;
            my_loosePropCount = room.my_loosePropCount;
            my_password = room.my_password;
            my_reserved = room.my_reserved;
            my_roomId = room.my_roomId;
            my_roomName = room.my_roomName;
            my_userCount = room.my_userCount;

            for (auto user: room.my_users) {
                my_users.push_back(user);
            }
        }

        void Room::do_init(void) //const Room& room)
        {
            //do_assign(room);
            do_setupEvents();
        }

        void Room::do_deinit(void)
        {

        }
    }
}
