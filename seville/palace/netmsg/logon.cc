#include "logon.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         NetMsgLogon::NetMsgLogon(NetMsgOptions options)
            : NetMsg(options)
         {
            do_init_();
         }

         NetMsgLogon::NetMsgLogon(const NetMsg& netmsg, NetMsgOptions options)
            : NetMsg(netmsg, options)
         {
            do_init_();
         }

         auto NetMsgLogon::do_init_(void) -> void
         {
            rep_ref_mut_().truncate(NetMsgSize::kMinimumSize);
            rep_ref_mut_().reserve(NetMsgSize::kLogonSize);

            set_id(NetMsgKind::kLogonKind);
            set_len(NetMsgSize::kLogonSize);
            set_ref(0);

            set_flags(0);
            set_reg_crc(0);
            set_puid_crc(0);
            set_username("Seville User");
            set_demo_limit(0x00011940);
            set_reg_counter(0);
            set_demo_elapsed(0x00011940);
            set_puid_counter(0);
            set_total_elapsed(0x00011940);
            set_initial_room_id(0);
            set_upload_capabilities(0);
            set_download_capabilities(0);
            set_engine_capabilities_2d(0);
            set_graphics_capabilities_2d(0);
            set_graphics_capabilities_3d(0);
         }
      }
   }
}
