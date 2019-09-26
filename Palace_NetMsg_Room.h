#ifndef PALACE_NETMSG_ROOM_H
#define PALACE_NETMSG_ROOM_H

#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class Room : public Generic
         {
            private:
               void doInit();

            public:
               u32 flags() { return u32At(kByteSizeOfHeader); }
               u32 face() { return u32At(kByteSizeOfHeader+4); }
               u16 roomId() { return u16At(kByteSizeOfHeader+8); }
               u16 roomNameOffset() { return u16At(kByteSizeOfHeader+10); }
               u16 backgroundImageNameOffset() { return u16At(kByteSizeOfHeader+12); }
               u16 artistNameOffset() { return u16At(kByteSizeOfHeader+14); }
               u16 passwordOffset() { return u16At(kByteSizeOfHeader+16); }
               u16 hotspotCount() { return u16At(kByteSizeOfHeader+18); }
               u16 hotspotOffset() { return u16At(kByteSizeOfHeader+20); }
               u16 roomImageCount() { return u16At(kByteSizeOfHeader+22); }
               u16 backgroundImageOffset() { return u16At(kByteSizeOfHeader+24); }
               u16 drawCommandsCount() { return u16At(kByteSizeOfHeader+26); }
               u16 firstDrawCommand() { return u16At(kByteSizeOfHeader+28); }
               u16 roomUserCount() { return u16At(kByteSizeOfHeader+30); }
               u16 loosePropCount() { return u16At(kByteSizeOfHeader+32); }
               u16 firstLooseProp() { return u16At(kByteSizeOfHeader+34); }
               u16 reserved() { return u16At(kByteSizeOfHeader+36); }
               u16 roomDataLen() { return u16At(kByteSizeOfHeader+38); }

               Room(bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // PALACE_NETMSG_ROOM_H
