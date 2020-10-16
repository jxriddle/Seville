#ifndef SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
#define SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_

//#include "seville/base/app.h"

#include "seville/palace/base/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class HttpServerLocation : public NetMsg {
         public:
            enum NetMsgHttpServerLocationSize {
               kMaximumUrlSize = 4096,
            };

            enum NetMsgHttpServerLocationOffset {
               kUrlOffset = NetMsgOffset::kPayloadOffset,
            };

            HttpServerLocation(
                  const NetMsg& netmsg, NetMsgOptions options = kNoEndianSwap);

            inline auto url(void) -> QString {
               return rep_ref_().qstring_at(kUrlOffset, kMaximumUrlSize);
            }

            inline auto set_url(const QString& value) -> void {
               rep_ref_mut_().set_pascal_qstring_at(kUrlOffset, value);
            }

            private:
               auto do_init_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
