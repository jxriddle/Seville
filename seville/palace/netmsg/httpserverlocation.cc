#include "seville/base/types.h"

#include "seville/palace/base/netmsg.h"

#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         HttpServerLocation::HttpServerLocation( \
               const NetMsg& netmsg, NetMsgOptions options) \
            : NetMsg(netmsg, options)
         {
            do_init_();
         }

         void HttpServerLocation::do_init_(void)
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kHttpServerLocationSize);

            set_id(NetMsgKind::kHttpServerLocationSize);
            set_len(kSizeOfNetMsgHttpServerLocationPayloadInBytes);
            set_ref(0);
         }
      }
   }
}
