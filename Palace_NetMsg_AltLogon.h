#ifndef PALACE_NETMSG_ALTLOGON_H
#define PALACE_NETMSG_ALTLOGON_H

#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class AltLogon : public Generic
         {
            public:
               static const u32 kByteSize = 128; // 0x80

            public:
               virtual ~AltLogon();

               AltLogon();
               AltLogon(const Generic& msg);
         };
      }
   }
}

#endif // PALACE_NETMSG_ALTLOGON_H
