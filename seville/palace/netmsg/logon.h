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
            static const int kSizeOfIdentInBytes = 6;
            static const int kSizeOfUsernameMaxInBytes = 31;
            static const int kSizeOfWizardPasswordMaxInBytes = 31;

            static const int kOffsetOfRegCrcInBytes = kOffsetForNetMsgPayloadInBytes;
            static const int kOffsetOfRegCounterInBytes = kOffsetForNetMsgPayloadInBytes+4;
            static const int kOffsetOfUsernameLenInBytes = kOffsetForNetMsgPayloadInBytes+8;
            static const int kOffsetOfUsernameInBytes = kOffsetForNetMsgPayloadInBytes+9;
            static const int kOffsetOfWizardPasswordLenInBytes = kOffsetForNetMsgPayloadInBytes+40;
            static const int kOffsetOfWizardPasswordInBytes = kOffsetForNetMsgPayloadInBytes+41;
            static const int kOffsetOfFlagsInBytes = kOffsetForNetMsgPayloadInBytes+72;
            static const int kOffsetOfPuidCounterInBytes = kOffsetForNetMsgPayloadInBytes+76;
            static const int kOffsetOfPuidCrcInBytes = kOffsetForNetMsgPayloadInBytes+80;
            static const int kOffsetOfDemoElapsedInBytes = kOffsetForNetMsgPayloadInBytes+84;
            static const int kOffsetOfTotalElapsedInBytes = kOffsetForNetMsgPayloadInBytes+88;
            static const int kOffsetOfDemoLimitInBytes = kOffsetForNetMsgPayloadInBytes+92;
            static const int kOffsetOfInitialRoomIdInBytes = kOffsetForNetMsgPayloadInBytes+96;
            static const int kOffsetOfReservedInBytes = kOffsetForNetMsgPayloadInBytes+98; // size: 6
            static const int kOffsetOfUploadRequestedProtocolVersionInBytes = kOffsetForNetMsgPayloadInBytes+104;
            static const int kOffsetOfUploadCapabilitiesInBytes = kOffsetForNetMsgPayloadInBytes+108;
            static const int kOffsetOfDownloadCapabilitiesInBytes = kOffsetForNetMsgPayloadInBytes+112;
            static const int kOffsetOfEngineCapabilities2dInBytes = kOffsetForNetMsgPayloadInBytes+116;
            static const int kOffsetOfGraphicsCapabilities2dInBytes = kOffsetForNetMsgPayloadInBytes+120;
            static const int kOffsetOfGraphicsCapabilities3dInBytes = kOffsetForNetMsgPayloadInBytes+124;

            Logon(NetMsgOptions options = NetMsgOptions::kDoNotSwapEndianness);

            Logon(const GenericNetMsg& netMsg, \
                  NetMsgOptions options = NetMsgOptions::kDoNotSwapEndianness);

            virtual ~Logon(void);

            auto regCrc(void) const -> u32 {
               return u32_at(kOffsetOfRegCrcInBytes);
            }

            auto setRegCrc(u32 value) -> void {
               set_u32_at(kOffsetOfRegCrcInBytes, value);
            }

            auto regCounter(void) const -> u32 {
               return u32_at(kOffsetOfRegCounterInBytes);
            }

            auto setRegCounter(u32 value) -> void {
               set_u32_at(kOffsetOfRegCounterInBytes, value);
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

            auto setFlags(u32 value) -> void {
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
               truncate(kOffsetForNetMsgPayloadInBytes);
               reserve(kSizeOfLogonNetMsgInBytes);

               set_id(NetMsgKind::kLogonKind);
               set_len(kSizeOfLogonNetMsgInBytes);
               set_ref(0);

               setFlags(0);
               setRegCrc(0);
               setPuidCrc(0);
               setUsername("Seville User");
               setDemoLimit(0x00011940);
               setRegCounter(0);
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
