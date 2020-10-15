#ifndef SEVILLE_PALACE_NETMSG_LOGON_H_
#define SEVILLE_PALACE_NETMSG_LOGON_H_

#include "seville/palace/base/genericnetmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class Logon : public GenericNetMsg
         {
         public:
            enum NetMsgLogonSize {
               kIdent = 6,
               kMaximumUsernameLength = 31,
               kMaximumWizardPasswordLength = 31,
            };

            enum NetMsgLogonOffset {
               kRegCrc = NetMsgOffset::kPayload,
               kRegCounter = NetMsgOffset::kPayload+4,
               kUsernameLen = NetMsgOffset::kPayload+8,
               kUsername = NetMsgOffset::kPayload+9,
               kWizardPasswordLen = NetMsgOffset::kPayload+40,
               kWizardPassword = NetMsgOffset::kPayload+41,
               kFlags = NetMsgOffset::kPayload+72,
               kPuidCounter = NetMsgOffset::kPayload+76,
               kPuidCrc = NetMsgOffset::kPayload+80,
               kDemoElapsed = NetMsgOffset::kPayload+84,
               kTotalElapsed = NetMsgOffset::kPayload+88,
               kDemoLimit = NetMsgOffset::kPayload+92,
               kInitialRoomId = NetMsgOffset::kPayload+96,
               kReserved = NetMsgOffset::kPayload+98, // size: 6
               kUploadRequestedProtocolVersion = NetMsgOffset::kPayload+104,
               kUploadCapabilities = NetMsgOffset::kPayload+108,
               kDownloadCapabilities = NetMsgOffset::kPayload+112,
               kEngineCapabilities2d = NetMsgOffset::kPayload+116,
               kGraphicsCapabilities2d = NetMsgOffset::kPayload+120,
               kGraphicsCapabilities3d = NetMsgOffset::kPayload+124,
            };

            Logon(NetMsgOptions options = NetMsgOptions::kDoNotSwapEndian);

            Logon(const GenericNetMsg& netMsg, NetMsgOptions options = NetMsgOptions::kDoNotSwapEndian);

            virtual ~Logon(void);

            auto reg_crc(void) const -> u32 {
               return u32_at(NetMsgLogonOffset::kRegCrc);
            }

            auto set_reg_crc(u32 value) -> void {
               set_u32_at(NetMsgLogonOffset::kRegCrc, value);
            }

            auto reg_counter(void) const -> u32 {
               return u32_at(NetMsgLogonOffset::kRegCounter);
            }

            auto set_reg_counter(u32 value) -> void {
               set_u32_at(NetMsgLogonOffset::kRegCounter, value);
            }

            auto username(void) const -> QString {
               return pascal_qstring_at(kOffsetOfUsernameInBytes, kSizeOfUsernameMaxInBytes);
            }

            auto setUsername(const QString& value) -> void {
               set_pascal_qstring_at(kOffsetOfUsernameInBytes, value);
            }

            auto usernameLen(void) const -> u8 {
               return u8_at(kOffsetOfUsernameLenInBytes);
            }

            auto setUsernameLen(u8 value) -> void {
               set_u8_at(kOffsetOfUsernameLenInBytes, value);
            }

            auto wizardPassword(void) const -> QString {
               return pascal_qstring_at(kOffsetOfWizardPasswordInBytes, kSizeOfWizardPasswordMaxInBytes);
            }

            auto setWizardPassword(const QString& value) -> void {
               return set_pascal_qstring_at(kOffsetOfWizardPasswordInBytes, value);
            }

            auto flags(void) const -> u32 {
               return u32_at(kOffsetOfFlagsInBytes);
            }

            auto set_flags(u32 value) -> void {
               set_u32_at(kOffsetOfFlagsInBytes, value);
            }

            auto puidCounter(void) const -> u32 {
               return u32_at(kOffsetOfPuidCounterInBytes);
            }

            auto setPuidCounter(u32 value) -> void {
               set_u32_at(kOffsetOfPuidCounterInBytes, value);
            }

            auto puidCrc(void) const -> u32 {
               return u32_at(kOffsetOfPuidCrcInBytes);
            }

            auto setPuidCrc(u32 value) -> void {
               set_u32_at(kOffsetOfPuidCrcInBytes, value);
            }

            auto demoElapsed(void) const -> u32 {
               return u32_at(kOffsetOfDemoElapsedInBytes);
            }

            auto setDemoElapsed(u32 value) -> void {
               set_u32_at(kOffsetOfDemoElapsedInBytes, value);
            }

            auto totalElapsed(void) const -> u32 {
               return u32_at(kOffsetOfTotalElapsedInBytes);
            }

            auto setTotalElapsed(u32 value) -> void {
               set_u32_at(kOffsetOfTotalElapsedInBytes, value);
            }

            auto demoLimit(void) const -> u32 {
               return u32_at(kOffsetOfDemoLimitInBytes);
            }

            auto setDemoLimit(u32 value) -> void {
               set_u32_at(kOffsetOfDemoLimitInBytes, value);
            }

            auto initialRoomId(void) const -> u16 {
               return u16_at(kOffsetOfInitialRoomIdInBytes);
            }

            auto setInitialRoomId(u16 value) -> void {
               set_u16_at(kOffsetOfInitialRoomIdInBytes, value);
            }

            auto reserved(void) const -> QString {
               return qstring_at(kOffsetOfReservedInBytes, kSizeOfIdentInBytes);
            }

            auto setReserved(const QString& value) -> void {
               set_qstring_at(kOffsetOfReservedInBytes, value);
            }

            auto uploadRequestedProtocolVersion(void) const -> u32 {
               return u32_at(kOffsetOfUploadRequestedProtocolVersionInBytes);
            }

            auto setUploadRequestedProtocolVersion(u32 value) -> void {
               set_u32_at(kOffsetOfUploadRequestedProtocolVersionInBytes, value);
            }

            auto uploadCapabilities(void) const -> u32 {
               return u32_at(kOffsetOfUploadCapabilitiesInBytes);
            }

            auto setUploadCapabilities(u32 value) -> void {
               set_u32_at(kOffsetOfUploadCapabilitiesInBytes, value);
            }

            auto downloadCapabilities(void) const -> u32 {
               return u32_at(kOffsetOfDownloadCapabilitiesInBytes);
            }

            auto setDownloadCapabilities(u32 value) -> void {
               set_u32_at(kOffsetOfDownloadCapabilitiesInBytes, value);
            }

            auto engineCapabilities2d(void) const -> u32 {
               return u32_at(kOffsetOfEngineCapabilities2dInBytes);
            }

            auto setEngineCapabilities2d(u32 value) -> void {
               set_u32_at(kOffsetOfEngineCapabilities2dInBytes, value);
            }

            auto graphicsCapabilities2d(void) const -> u32 {
               return u32_at(kOffsetOfGraphicsCapabilities2dInBytes);
            }

            auto setGraphicsCapabilities2d(u32 value) -> void {
               set_u32_at(kOffsetOfGraphicsCapabilities2dInBytes, value);
            }

            auto graphicsCapabilities3d(void) const -> u32 {
               return u32_at(kOffsetOfGraphicsCapabilities3dInBytes);
            }

            auto setGraphicsCapabilities3d(u32 value) -> void {
               set_u32_at(kOffsetOfGraphicsCapabilities3dInBytes, value);
            }

         private:
            void do_init_(void)
            {
               truncate(NetMsgSize::kMinimum);
               reserve(NetMsgSize::kLogon);

               set_id(NetMsgKind::kLogonKind);
               set_len(NetMsgSize::kLogon);
               set_ref(0);

               set_flags(0);
               set_reg_crc(0);
               setPuidCrc(0);
               setUsername("Seville User");
               setDemoLimit(0x00011940);
               set_reg_counter(0);
               setDemoElapsed(0x00011940);
               setPuidCounter(0);
               setTotalElapsed(0x00011940);
               setInitialRoomId(0);
               setUploadCapabilities(0);
               setDownloadCapabilities(0);
               setEngineCapabilities2d(0);
               setGraphicsCapabilities2d(0);
               setGraphicsCapabilities3d(0);
            }
         };
      }
   }
}

#endif // PALACE_NETMSG_LOGON_H_
