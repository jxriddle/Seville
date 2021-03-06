#ifndef SEVILLE_PALACE_NETMSG_TALK_H
#define SEVILLE_PALACE_NETMSG_TALK_H

#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class TalkNetMsg : NetMsg
         {
         public:
            TalkNetMsg(NetMsgOptions options = NetMsgOptions::kNoEndianSwap);
            virtual ~TalkNetMsg(void);

            auto set_options(NetMsgOptions options) -> void;

         private:
            auto do_init(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_TALK_H
