#ifndef SEVILLE_PALACE_NETMSG_LOGON_H_
#define SEVILLE_PALACE_NETMSG_LOGON_H_

#include "seville/palace/base/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class NetMsgLogon : public NetMsg
         {
         public:
            enum NetMsgLogonSize {
               kIdentSize = 6,
               kMaximumUsernameSize = 31,
               kMaximumWizardPasswordSize = 31,
            };

            enum NetMsgLogonOffset {
               kRegCrcOffset = NetMsgOffset::kPayloadOffset,
               kRegCounterOffset = NetMsgOffset::kPayloadOffset+4,
               kUsernameLenOffset = NetMsgOffset::kPayloadOffset+8,
               kUsernameOffset = NetMsgOffset::kPayloadOffset+9,
               kWizardPasswordLenOffset = NetMsgOffset::kPayloadOffset+40,
               kWizardPasswordOffset = NetMsgOffset::kPayloadOffset+41,
               kFlagsOffset = NetMsgOffset::kPayloadOffset+72,
               kPuidCounterOffset = NetMsgOffset::kPayloadOffset+76,
               kPuidCrcOffset = NetMsgOffset::kPayloadOffset+80,
               kDemoElapsedOffset = NetMsgOffset::kPayloadOffset+84,
               kTotalElapsedOffset = NetMsgOffset::kPayloadOffset+88,
               kDemoLimitOffset = NetMsgOffset::kPayloadOffset+92,
               kInitialRoomIdOffset = NetMsgOffset::kPayloadOffset+96,
               kReservedOffset = NetMsgOffset::kPayloadOffset+98, // size: 6
               kUploadRequestedProtocolVersionOffset =
                     NetMsgOffset::kPayloadOffset+104,
               kUploadCapabilitiesOffset = NetMsgOffset::kPayloadOffset+108,
               kDownloadCapabilitiesOffset = NetMsgOffset::kPayloadOffset+112,
               kEngineCapabilities2dOffset = NetMsgOffset::kPayloadOffset+116,
               kGraphicsCapabilities2dOffset = NetMsgOffset::kPayloadOffset+120,
               kGraphicsCapabilities3dOffset = NetMsgOffset::kPayloadOffset+124,
            };

            NetMsgLogon(NetMsgOptions options = kNoEndianSwap);

            NetMsgLogon(
                  const NetMsg& netMsg, NetMsgOptions options = kNoEndianSwap);

            //virtual ~NetMsgLogon(void);

            auto reg_crc(void) const -> u32 {
               return rep_ref_().u32_at(kRegCrcOffset);
            }

            auto set_reg_crc(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kRegCrcOffset, value);
            }

            auto reg_counter(void) const -> u32 {
               return rep_ref_().u32_at(kRegCounterOffset);
            }

            auto set_reg_counter(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kRegCounterOffset, value);
            }

            auto username(void) const -> QString {
               return rep_ref_().pascal_qstring_at(
                        kUsernameOffset, kMaximumUsernameSize);
            }

            auto set_username(const QString& value) -> void {
               rep_ref_mut_().set_pascal_qstring_at(kUsernameOffset, value);
            }

            auto username_len(void) const -> u8 {
               return rep_ref_().u8_at(kUsernameLenOffset);
            }

            auto set_username_len(u8 value) -> void {
               rep_ref_mut_().set_u8_at(kUsernameLenOffset, value);
            }

            auto wizard_password(void) const -> QString {
               return rep_ref_().pascal_qstring_at(
                        kWizardPasswordOffset, kMaximumWizardPasswordSize);
            }

            auto set_wizard_password(const QString& value) -> void {
               return rep_ref_mut_().set_pascal_qstring_at(
                        kWizardPasswordOffset, value);
            }

            auto flags(void) const -> u32 {
               return rep_ref_().u32_at(kFlagsOffset);
            }

            auto set_flags(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kFlagsOffset, value);
            }

            auto puid_counter(void) const -> u32 {
               return rep_ref_().u32_at(kPuidCounterOffset);
            }

            auto set_puid_counter(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kPuidCounterOffset, value);
            }

            auto puid_crc(void) const -> u32 {
               return rep_ref_().u32_at(kPuidCrcOffset);
            }

            auto set_puid_crc(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kPuidCrcOffset, value);
            }

            auto demo_elapsed(void) const -> u32 {
               return rep_ref_().u32_at(kDemoElapsedOffset);
            }

            auto set_demo_elapsed(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kDemoElapsedOffset, value);
            }

            auto total_elapsed(void) const -> u32 {
               return rep_ref_().u32_at(kTotalElapsedOffset);
            }

            auto set_total_elapsed(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kTotalElapsedOffset, value);
            }

            auto demo_limit(void) const -> u32 {
               return rep_ref_().u32_at(kDemoLimitOffset);
            }

            auto set_demo_limit(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kDemoLimitOffset, value);
            }

            auto initial_room_id(void) const -> u16 {
               return rep_ref_().u16_at(kInitialRoomIdOffset);
            }

            auto set_initial_room_id(u16 value) -> void {
               rep_ref_mut_().set_u16_at(kInitialRoomIdOffset, value);
            }

            auto reserved(void) const -> QString {
               return rep_ref_().qstring_at(kReservedOffset, kIdentSize);
            }

            auto set_reserved(const QString& value) -> void {
               rep_ref_mut_().set_qstring_at(kReservedOffset, value);
            }

            auto upload_requested_protocol_version(void) const -> u32 {
               return rep_ref_().u32_at(kUploadRequestedProtocolVersionOffset);
            }

            auto set_upload_requested_protocol_version(u32 value) -> void {
               rep_ref_mut_().set_u32_at(
                        kUploadRequestedProtocolVersionOffset, value);
            }

            auto upload_capabilities(void) const -> u32 {
               return rep_ref_().u32_at(kUploadCapabilitiesOffset);
            }

            auto set_upload_capabilities(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kUploadCapabilitiesOffset, value);
            }

            auto download_capabilities(void) const -> u32 {
               return rep_ref_().u32_at(kDownloadCapabilitiesOffset);
            }

            auto set_download_capabilities(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kDownloadCapabilitiesOffset, value);
            }

            auto engine_capabilities_2d(void) const -> u32 {
               return rep_ref_().u32_at(kEngineCapabilities2dOffset);
            }

            auto set_engine_capabilities_2d(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kEngineCapabilities2dOffset, value);
            }

            auto graphics_capabilities_2d(void) const -> u32 {
               return rep_ref_().u32_at(kGraphicsCapabilities2dOffset);
            }

            auto set_graphics_capabilities_2d(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kGraphicsCapabilities2dOffset, value);
            }

            auto graphics_capabilities_3d(void) const -> u32 {
               return rep_ref_().u32_at(kGraphicsCapabilities3dOffset);
            }

            auto set_graphics_capabilities_3d(u32 value) -> void {
               rep_ref_mut_().set_u32_at(kGraphicsCapabilities3dOffset, value);
            }

         private:
            auto do_init_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_LOGON_H_
