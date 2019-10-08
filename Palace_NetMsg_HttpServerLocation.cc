#include "Palace_NetMsg_Generic.h"
#include "Palace_NetMsg_HttpServerLocation.h"

#include "Common.h"
#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void HttpServerLocation::doInit(void)
         {
            truncate(kByteSizeOfHeader);
            reserve(kByteSizeOfRoomDescription);

            setKind(NetMsg::Kind::HttpServerLocationKind);
            setSizeExpected(NetMsg::kByteSizeOfHttpServerLocation);
            setObjectId(0);
         }

         HttpServerLocation::HttpServerLocation(Generic& netMsg, bool shouldSwapEndianness)
            : Generic(netMsg, shouldSwapEndianness)
         {
            //doInit();
         }
      }
   }
}
