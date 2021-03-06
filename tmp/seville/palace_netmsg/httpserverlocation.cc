#include "seville/base/types.h"

#include "seville/palace/netmsg.h"

#include "seville/palace_netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         HttpServerLocationObject::HttpServerLocationObject(NetMsgOptions options)
            : NetMsg(options)
         {
            do_init();
         }

         auto HttpServerLocationObject::do_init(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kHttpServerLocationSize);

            SetId(NetMsgKind::kHttpServerLocationKind);
            SetLen(NetMsgSize::kHttpServerLocationPayloadSize);
            SetRef(0);
         }
      }
   }
}
