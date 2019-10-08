#ifndef SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H
#define SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H

#include "Common.h"
#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class HttpServerLocation : public Generic
         {
            private:
               void doInit(void);

            public:
               std::string url()
                  { return stdStringAt(NetMsg::kByteSizeOfHeader, NetMsg::kByteSizeMaximum); }
               void setUrl(const std::string& value)
                  { setStdStringAt(NetMsg::kByteSizeOfHeader, value); }

               HttpServerLocation(Generic& netMsg, bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_HTTPSERVERLOCATION_H
