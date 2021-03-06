#ifndef SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
#define SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_

//#include "seville/app.h"

#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class HttpServerLocationObject : public NetMsg {
         public:
            enum NetMsgHttpServerLocationSize {
               kMaximumUrlSize = 4096,
            };

            enum NetMsgHttpServerLocationOffset {
               kUrlOffset = NetMsgOffset::kPayloadOffset,
            };

            HttpServerLocationObject(NetMsgOptions options = kNoEndianSwap);

            inline auto url(void) -> QString {
               return qstringAt(kUrlOffset, kMaximumUrlSize);
            }

            inline auto set_url(const QString& value) -> void {
               setPascalQStringAt(kUrlOffset, value);
            }

            private:
               auto do_init(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
