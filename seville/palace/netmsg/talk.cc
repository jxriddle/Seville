#include "talk.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         Talk::Talk(NetMsgOptions options)
         {
            this->set_should_swap_endianness_flag(
                     options & NetMsgOptions::kEndianSwap);
         }

         Talk::~Talk(void)
         {

         }

         auto Talk::set_options(NetMsgOptions options) -> void
         {
            this->set_should_swap_endianness_flag(
                     options & NetMsgOptions::kEndianSwap);
         }

         auto Talk::do_init_(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kTalkSize);
            //set_id(NetMsgKind::kTalkKind);
         }
      }
   }
}
