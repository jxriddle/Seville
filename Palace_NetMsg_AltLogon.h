#ifndef PALACE_NETMSG_ALTLOGON_H
#define PALACE_NETMSG_ALTLOGON_H

#include "Palace_NetMsg_NetMsgBase.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class AltLogon : public NetMsgBase
         {
            public:
               virtual ~AltLogon();

               AltLogon();
               AltLogon(const NetMsgBase& msg);
         };
      }
   }
}

#endif // PALACE_NETMSG_ALTLOGON_H
