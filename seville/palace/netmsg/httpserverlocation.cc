#include "seville/base/types.h"

#include "seville/palace/base/genericnetmsg.h"

#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         HttpServerLocation::HttpServerLocation( \
               const GenericNetMsg& netmsg, NetMsgOptions options) \
            : GenericNetMsg(netmsg, options)
         {
            do_init_();
         }

         void HttpServerLocation::do_init_(void)
         {
            truncate(kOffsetForNetMsgPayloadInBytes);
            reserve(kSizeOfNetMsgHttpServerLocationInBytes);

            set_id(NetMsgKind::kHttpServerLocationKind);
            set_len(kSizeOfNetMsgHttpServerLocationPayloadInBytes);
            set_ref(0);
         }
      }
   }
}
