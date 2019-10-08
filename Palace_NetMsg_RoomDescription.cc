#include "Palace_NetMsg_Generic.h"
#include "Palace_NetMsg_RoomDescription.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void RoomDescription::doInit()
         {
            truncate(kByteSizeOfHeader);
            reserve(kByteSizeOfRoomDescription);

            setKind(NetMsg::Kind::RoomKind);
            setSizeExpected(NetMsg::kByteSizeOfRoomDescription);
            setObjectId(0);
         }

         RoomDescription::RoomDescription(bool shouldSwapEndianness)
            : Generic(shouldSwapEndianness)
         {
            doInit();
         }

         RoomDescription::RoomDescription(Generic& netMsg, bool shouldSwapEndianness)
            : Generic(netMsg, shouldSwapEndianness)
         {
            //doInit();
         }
      }
   }
}
