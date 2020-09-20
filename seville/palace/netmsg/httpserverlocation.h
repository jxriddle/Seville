#ifndef SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
#define SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_

#include "generic.h"

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class HttpServerLocation : public netmsg::Generic
         {
            private:
               void do_init(void);

            public:
               void setUrl(const QString& value) { setQStringU8At(netmsg::kOffsetForPayloadInBytes, value); }
               QString url() { return qStringAt(netmsg::kOffsetForPayloadInBytes, 4096); }

               HttpServerLocation(const netmsg::Generic& netMsg, NetMsgOptions options = NetMsgOptions::kShouldNotSwapEndianness);
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
