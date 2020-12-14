#include "seville/base/types.h"

#include "seville/palace/base/netmsg.h"

#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         HttpServerLocation::HttpServerLocation(NetMsgOptions options)
            : NetMsg(options)
         {
            do_init_();
         }

         auto HttpServerLocation::do_init_(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kHttpServerLocationSize);

            set_id(NetMsgKind::kHttpServerLocationKind);
            set_len(NetMsgSize::kHttpServerLocationPayloadSize);
            set_ref(0);
         }
      }
   }
}
