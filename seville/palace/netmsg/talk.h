#ifndef SEVILLE_PALACE_NETMSG_TALK_H
#define SEVILLE_PALACE_NETMSG_TALK_H

#include "seville/palace/base/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class Talk : NetMsg
         {
         public:
            Talk(NetMsgOptions options = NetMsgOptions::kNoEndianSwap);
            virtual ~Talk(void);

            auto set_options(NetMsgOptions options) -> void;

         private:
            auto do_init_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_TALK_H
