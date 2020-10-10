#ifndef SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
#define SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_

#include "seville/base/types.h"

#include "seville/palace/base/genericnetmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class HttpServerLocation : public GenericNetMsg
         {
            private:
               auto do_init(void) -> void;

            public:
               auto url(void) -> QString {
                  return qstring_at( \
                           palace::kOffsetForNetMsgPayloadInBytes, 4096);
               }

               auto setUrl(const QString& value) -> void {
                  set_pascal_qstring_at( \
                           palace::kOffsetForNetMsgPayloadInBytes, value);
               }

               HttpServerLocation( \
                     const GenericNetMsg& netmsg, \
                     NetMsgOptions options = \
                        NetMsgOptions::kDoNotSwapEndianness);
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
