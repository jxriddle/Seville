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
            rep_ref_mut_().truncate(NetMsgSize::kMinimumSize);
            rep_ref_mut_().reserve(NetMsgSize::kHttpServerLocationSize);

            set_id(NetMsgSize::kHttpServerLocationSize);
            set_len(NetMsgSize::kHttpServerLocationPayloadSize);
            set_ref(0);
         }
      }
   }
}
