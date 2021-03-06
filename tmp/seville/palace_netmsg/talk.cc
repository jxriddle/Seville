#include "talk.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         TalkNetMsg::TalkNetMsg(NetMsgOptions options)
         {
            this->SetShouldSwapEndianness(
                     options & NetMsgOptions::kEndianSwap);
         }

         TalkNetMsg::~TalkNetMsg(void)
         {

         }

         auto TalkNetMsg::set_options(NetMsgOptions options) -> void
         {
            this->SetShouldSwapEndianness(
                     options & NetMsgOptions::kEndianSwap);
         }

         auto TalkNetMsg::do_init(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kTalkSize);
            //set_id(NetMsgKind::kTalkKind);
         }
      }
   }
}
