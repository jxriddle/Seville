#ifndef SEVILLE_PALACE_NETMSG_LOGON_H_
#define SEVILLE_PALACE_NETMSG_LOGON_H_

#include "generic.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class Logon : public netmsg::Generic
         {
            public:
               static const int kByteSizeOfIdent = 6;
               static const int kByteSizeOfUsernameMax = 31;
               static const int kByteSizeOfWizardPasswordMax = 31;

               static const int kByteOffsetOfRegCrc = kOffsetForPayloadInBytes;
               static const int kByteOffsetOfRegCounter = kOffsetForPayloadInBytes+4;
               static const int kByteOffsetOfUsernameLen = kOffsetForPayloadInBytes+8;
               static const int kByteOffsetOfUsername = kOffsetForPayloadInBytes+9;
               static const int kByteOffsetOfWizardPasswordLen = kOffsetForPayloadInBytes+40;
               static const int kByteOffsetOfWizardPassword = kOffsetForPayloadInBytes+41;
               static const int kByteOffsetOfFlags = kOffsetForPayloadInBytes+72;
               static const int kByteOffsetOfPuidCounter = kOffsetForPayloadInBytes+76;
               static const int kByteOffsetOfPuidCrc = kOffsetForPayloadInBytes+80;
               static const int kByteOffsetOfDemoElapsed = kOffsetForPayloadInBytes+84;
               static const int kByteOffsetOfTotalElapsed = kOffsetForPayloadInBytes+88;
               static const int kByteOffsetOfDemoLimit = kOffsetForPayloadInBytes+92;
               static const int kByteOffsetOfInitialRoomId = kOffsetForPayloadInBytes+96;
               static const int kByteOffsetOfReserved = kOffsetForPayloadInBytes+98; // size: 6
               static const int kByteOffsetOfUploadRequestedProtocolVersion = kOffsetForPayloadInBytes+104;
               static const int kByteOffsetOfUploadCapabilities = kOffsetForPayloadInBytes+108;
               static const int kByteOffsetOfDownloadCapabilities = kOffsetForPayloadInBytes+112;
               static const int kByteOffsetOfEngineCapabilities2d = kOffsetForPayloadInBytes+116;
               static const int kByteOffsetOfGraphicsCapabilities2d = kOffsetForPayloadInBytes+120;
               static const int kByteOffsetOfGraphicsCapabilities3d = kOffsetForPayloadInBytes+124;

               Logon(netmsg::NetMsgOptions shouldSwapEndianness = netmsg::NetMsgOptions::kShouldNotSwapEndianness);
               Logon(const netmsg::Generic& netMsg, netmsg::NetMsgOptions shouldSwapEndianness = netmsg::NetMsgOptions::kShouldNotSwapEndianness);

               virtual ~Logon(void);

            private:
               void do_init(void)
               {
                  truncate(kOffsetForPayloadInBytes);
                  reserve(kSizeOfLogonInBytes);

                  setId(netmsg::Kind::kLogonKind);
                  setLen(kSizeOfLogonInBytes);
                  setRef(0);

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

            public:
               u32 regCrc(void) const { return u32At(kByteOffsetOfRegCrc); }
               void setRegCrc(u32 value) { setU32At(kByteOffsetOfRegCrc, value); }

               u32 regCounter(void) const { return u32At(kByteOffsetOfRegCounter); }
               void setRegCounter(u32 value) { setU32At(kByteOffsetOfRegCounter, value); }

               QString username(void) const { return qStringU8At(kByteOffsetOfUsername, kByteSizeOfUsernameMax); }
               void setUsername(const QString& value) { setQStringU8At(kByteOffsetOfUsername, value); }

               u8 usernameLen(void) const { return u8At(kByteOffsetOfUsernameLen); }
               void setUsernameLen(u8 value) { setU8At(kByteOffsetOfUsernameLen, value); }

               QString wizardPassword(void) const { return qStringU8At(kByteOffsetOfWizardPassword, kByteSizeOfWizardPasswordMax); }
               void setWizardPassword(const QString& value) { return setQStringU8At(kByteOffsetOfWizardPassword, value); }

               u32 flags(void) const { return u32At(kByteOffsetOfFlags); }
               void setFlags(u32 value) { setU32At(kByteOffsetOfFlags, value); }

               u32 puidCounter(void) const { return u32At(kByteOffsetOfPuidCounter); }
               void setPuidCounter(u32 value) { setU32At(kByteOffsetOfPuidCounter, value); }

               u32 puidCrc(void) const { return u32At(kByteOffsetOfPuidCrc); }
               void setPuidCrc(u32 value) { setU32At(kByteOffsetOfPuidCrc, value); }

               u32 demoElapsed(void) const { return u32At(kByteOffsetOfDemoElapsed); }
               void setDemoElapsed(u32 value) { setU32At(kByteOffsetOfDemoElapsed, value); }

               u32 totalElapsed(void) const { return u32At(kByteOffsetOfTotalElapsed); }
               void setTotalElapsed(u32 value) { setU32At(kByteOffsetOfTotalElapsed, value); }

               u32 demoLimit(void) const { return u32At(kByteOffsetOfDemoLimit); }
               void setDemoLimit(u32 value) { setU32At(kByteOffsetOfDemoLimit, value); }

               u16 initialRoomId(void) const { return u16At(kByteOffsetOfInitialRoomId); }
               void setInitialRoomId(u16 value) { setU16At(kByteOffsetOfInitialRoomId, value); }

               QString reserved(void) const { return qStringAt(kByteOffsetOfReserved, kByteSizeOfIdent); }
               void setReserved(const QString& value) { setQStringAt(kByteOffsetOfReserved, value); }

               u32 uploadRequestedProtocolVersion(void) const { return u32At(kByteOffsetOfUploadRequestedProtocolVersion); }
               void setUploadRequestedProtocolVersion(u32 value) { setU32At(kByteOffsetOfUploadRequestedProtocolVersion, value); }

               u32 uploadCapabilities(void) const { return u32At(kByteOffsetOfUploadCapabilities); }
               void setUploadCapabilities(u32 value) { setU32At(kByteOffsetOfUploadCapabilities, value); }

               u32 downloadCapabilities(void) const { return u32At(kByteOffsetOfDownloadCapabilities); }
               void setDownloadCapabilities(u32 value) { setU32At(kByteOffsetOfDownloadCapabilities, value); }

               u32 engineCapabilities2d(void) const { return u32At(kByteOffsetOfEngineCapabilities2d); }
               void setEngineCapabilities2d(u32 value) { setU32At(kByteOffsetOfEngineCapabilities2d, value); }

               u32 graphicsCapabilities2d(void) const { return u32At(kByteOffsetOfGraphicsCapabilities2d); }
               void setGraphicsCapabilities2d(u32 value) { setU32At(kByteOffsetOfGraphicsCapabilities2d, value); }

               u32 graphicsCapabilities3d(void) const { return u32At(kByteOffsetOfGraphicsCapabilities3d); }
               void setGraphicsCapabilities3d(u32 value) { setU32At(kByteOffsetOfGraphicsCapabilities3d, value); }
         };
      }
   }
}

#endif // PALACE_NETMSG_LOGON_H_
