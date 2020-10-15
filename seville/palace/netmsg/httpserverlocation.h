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
         public:
            HttpServerLocation( \
                  const GenericNetMsg& netmsg, \
                  NetMsgOptions options = \
                     NetMsgOptions::kDoNotSwapEndian            inline auto url(void) -> QString {
               return qstring_at( \
                        palace::kOffsetForNetMsgPayloadInBytes, 4096);
            }

            inline auto set_url(const QString& value) -> void {
               set_pascal_qstring_at( \
                        palace::kOffsetForNetMsgPayloadInBytes, value);
            }

            private:
               auto do_init_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H_
