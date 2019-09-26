#include "Palace_NetMsg_Generic.h"
#include "Palace_NetMsg_Room.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void Room::doInit()
         {
            setKind(NetMsg::Kind::RoomKind);
            setSizeExpected(NetMsg::kByteSizeOfRoomDescription);
            setObjectId(0);
         }

         Room::Room(bool shouldSwapEndianness)
            : Generic(shouldSwapEndianness)
         {
            doInit();
         }
      }
   }
}
