#ifndef PALACE_NETMSG_LOGON_H
#define PALACE_NETMSG_LOGON_H

#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class Logon : public Generic
         {
            private:
               void doInit();

            public:
               virtual ~Logon();

               Logon();
               Logon(Host::ByteOrder clientByteOrder,
                        Host::ByteOrder serverByteOrder);
         };
      }
   }
}

#endif // PALACE_NET_MSG_LOGON_H
