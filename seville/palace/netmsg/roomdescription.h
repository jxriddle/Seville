#ifndef PALACE_NETMSG_ROOM_H_
#define PALACE_NETMSG_ROOM_H_

#include "seville/palace/prop.h"

#include "generic.h"

#include <vector>

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class RoomDescription : public netmsg::Generic
         {
         public:
            static const zog::u32 kOffsetRoomPayload = kOffsetForPayloadInBytes + 40;

         private:
            void do_init(void)
            {
               truncate(kOffsetForPayloadInBytes);
               reserve(kSizeOfRoomDescriptionInBytes);

               setId(netmsg::Kind::RoomKind);
               setLen(netmsg::kSizeOfRoomDescriptionInBytes);
               setRef(0);
            }

         public:
            u32 flags(void) const { return u32At(kOffsetForPayloadInBytes); }
            u32 face(void) const { return u32At(kOffsetForPayloadInBytes+4); }
            u16 roomId(void) const { return u16At(kOffsetForPayloadInBytes+8); }
            u16 roomNameOffset(void) const { return u16At(kOffsetForPayloadInBytes+10); }
            u16 backgroundImageNameOffset(void) const { return u16At(kOffsetForPayloadInBytes+12); }
            u16 artistNameOffset(void) const { return u16At(kOffsetForPayloadInBytes+14); }
            u16 passwordOffset(void) const { return u16At(kOffsetForPayloadInBytes+16); }
            u16 hotspotCount(void) const { return u16At(kOffsetForPayloadInBytes+18); }
            u16 hotspotOffset(void) const { return u16At(kOffsetForPayloadInBytes+20); }
            u16 roomImageCount(void) const { return u16At(kOffsetForPayloadInBytes+22); }
            u16 backgroundImageOffset(void) const { return u16At(kOffsetForPayloadInBytes+24); }

            u16 drawCommandsCount(void) const { return u16At(kOffsetForPayloadInBytes+26); }
            u16 firstDrawCommand(void) const { return u16At(kOffsetForPayloadInBytes+28); }
            u16 roomUserCount(void) const { return u16At(kOffsetForPayloadInBytes+30); }
            u16 loosePropCount(void) const { return u16At(kOffsetForPayloadInBytes+32); }
            u16 firstLooseProp(void) const { return u16At(kOffsetForPayloadInBytes+34); }
            u16 reserved(void) const { return u16At(kOffsetForPayloadInBytes+36); }
            u16 roomDataLen(void) const { return u16At(kOffsetForPayloadInBytes+38); }
            //ByteArray roomData(void) { return byteArrayAt(kOffsetForPayloadInBytes + 40); }

//            std::vector<Palace::Prop> props(void)
//            {
//               auto value = std::vector<Palace::Prop>();

//               auto loosePropCount =  u16At(kOffsetForPayloadInBytes+32);
//               for (auto i = 0; i < loosePropCount; i++)
//               {
//                  Prop* p = ;
//                  value.push_back(*p);
//               }

//               return value;
//            }

            QString artistName(void) const
            {
               return qStringAt(kOffsetRoomPayload + u16At(kOffsetForPayloadInBytes+14), 1024);
            }

            QString backgroundImageName(void) const
            {
               return qStringAt(u16At(kOffsetRoomPayload + u16At(kOffsetForPayloadInBytes+24)), 1024);
            }

            //QByteArray backgroundImage(void) { return byteArrayAt(u16At(kOffsetForPayloadInBytes+24), 1024); }

            RoomDescription(bool shouldSwapEndianness = false);
            RoomDescription(const Generic& netMsg, bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // PALACE_NETMSG_ROOM_H_
