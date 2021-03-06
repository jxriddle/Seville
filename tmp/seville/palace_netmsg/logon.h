#ifndef SEVILLE_PALACE_NETMSG_LOGON_H_
#define SEVILLE_PALACE_NETMSG_LOGON_H_

#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class LogonObject : public NetMsg
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

            LogonObject(NetMsgOptions options = kNoEndianSwap);

            LogonObject(
                  const NetMsg& netMsg, NetMsgOptions options = kNoEndianSwap);

            //virtual ~NetMsgLogon(void);

            auto reg_crc(void) const -> u32 {
               return U32At(kRegCrcOffset);
            }

            auto set_reg_crc(u32 value) -> void {
               SetU32At(kRegCrcOffset, value);
            }

            auto reg_counter(void) const -> u32 {
               return U32At(kRegCounterOffset);
            }

            auto set_reg_counter(u32 value) -> void {
               SetU32At(kRegCounterOffset, value);
            }

            auto username(void) const -> QString {
               return PascalQStringAt(kUsernameOffset);
            }

            auto set_username(const QString& value) -> void {
               SetPascalQStringAt(kUsernameOffset, value);
            }

            auto username_len(void) const -> u8 {
               return U8At(kUsernameLenOffset);
            }

            auto set_username_len(u8 value) -> void {
               SetU8At(kUsernameLenOffset, value);
            }

            auto wizard_password(void) const -> QString {
               return QStringAt(
                        kWizardPasswordOffset, kMaximumWizardPasswordSize);
            }

            auto set_wizard_password(const QString& value) -> void {
               return SetPascalQStringAt(
                        kWizardPasswordOffset, value);
            }

            auto flags(void) const -> u32 {
               return U32At(kFlagsOffset);
            }

            auto set_flags(u32 value) -> void {
               SetU32At(kFlagsOffset, value);
            }

            auto puid_counter(void) const -> u32 {
               return U32At(kPuidCounterOffset);
            }

            auto set_puid_counter(u32 value) -> void {
               SetU32At(kPuidCounterOffset, value);
            }

            auto puid_crc(void) const -> u32 {
               return U32At(kPuidCrcOffset);
            }

            auto set_puid_crc(u32 value) -> void {
               SetU32At(kPuidCrcOffset, value);
            }

            auto demo_elapsed(void) const -> u32 {
               return U32At(kDemoElapsedOffset);
            }

            auto set_demo_elapsed(u32 value) -> void {
               SetU32At(kDemoElapsedOffset, value);
            }

            auto total_elapsed(void) const -> u32 {
               return U32At(kTotalElapsedOffset);
            }

            auto set_total_elapsed(u32 value) -> void {
               SetU32At(kTotalElapsedOffset, value);
            }

            auto demo_limit(void) const -> u32 {
               return U32At(kDemoLimitOffset);
            }

            auto set_demo_limit(u32 value) -> void {
               SetU32At(kDemoLimitOffset, value);
            }

            auto initial_room_id(void) const -> u16 {
               return U16At(kInitialRoomIdOffset);
            }

            auto set_initial_room_id(u16 value) -> void {
               SetU16At(kInitialRoomIdOffset, value);
            }

            auto reserved(void) const -> QString {
               return QStringAt(kReservedOffset, kIdentSize);
            }

            auto set_reserved(const QString& value) -> void {
               SetQStringAt(kReservedOffset, value);
            }

            auto upload_requested_protocol_version(void) const -> u32 {
               return U32At(kUploadRequestedProtocolVersionOffset);
            }

            auto set_upload_requested_protocol_version(u32 value) -> void {
               SetU32At(
                        kUploadRequestedProtocolVersionOffset, value);
            }

            auto upload_capabilities(void) const -> u32 {
               return U32At(kUploadCapabilitiesOffset);
            }

            auto set_upload_capabilities(u32 value) -> void {
               SetU32At(kUploadCapabilitiesOffset, value);
            }

            auto download_capabilities(void) const -> u32 {
               return U32At(kDownloadCapabilitiesOffset);
            }

            auto set_download_capabilities(u32 value) -> void {
               SetU32At(kDownloadCapabilitiesOffset, value);
            }

            auto engine_capabilities_2d(void) const -> u32 {
               return U32At(kEngineCapabilities2dOffset);
            }

            auto set_engine_capabilities_2d(u32 value) -> void {
               SetU32At(kEngineCapabilities2dOffset, value);
            }

            auto graphics_capabilities_2d(void) const -> u32 {
               return U32At(kGraphicsCapabilities2dOffset);
            }

            auto set_graphics_capabilities_2d(u32 value) -> void {
               SetU32At(kGraphicsCapabilities2dOffset, value);
            }

            auto graphics_capabilities_3d(void) const -> u32 {
               return U32At(kGraphicsCapabilities3dOffset);
            }

            auto set_graphics_capabilities_3d(u32 value) -> void {
               SetU32At(kGraphicsCapabilities3dOffset, value);
            }

         private:
            auto do_init(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_PALACE_NETMSG_LOGON_H_
