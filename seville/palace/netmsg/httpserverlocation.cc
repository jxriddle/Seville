#include "zog/base/types.h"

#include "seville/palace/netmsg/generic.h"
#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         HttpServerLocation::HttpServerLocation(const netmsg::GenericNetMsg& netMsg, bool shouldSwapEndianness)
            : netmsg::GenericNetMsg(netMsg, shouldSwapEndianness)
         {
            do_init();
         }

         void HttpServerLocation::do_init(void)
         {
            truncate(kOffsetForPayloadInBytes);
            reserve(kSizeOfRoomDescriptionInBytes);

            setId(netmsg::Kind::kHttpServerLocationKind);
            setLen(netmsg::kSizeOfHttpServerLocationInBytes);
            setRef(0);
         }
      }
   }
}