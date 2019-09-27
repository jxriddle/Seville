#ifndef PALACE_NETMSG_LOGON_H
#define PALACE_NETMSG_LOGON_H

#include "Palace_NetMsg_Generic.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         class Logon : public Generic
         {
            //public:
            //   static const u32 kByteSize = 128; // 0x80

            public:
               static const int kByteSizeOfIdent = 6;
               static const int kByteSizeOfUsernameMax = 31;
               static const int kByteSizeOfWizardPasswordMax = 31;

               static const int kByteOffsetOfRegCrc = kByteSizeOfHeader;
               static const int kByteOffsetOfRegCounter = kByteSizeOfHeader+4;
               static const int kByteOffsetOfUsernameLen = kByteSizeOfHeader+8;
               static const int kByteOffsetOfUsername = kByteSizeOfHeader+9;
               static const int kByteOffsetOfWizardPasswordLen = kByteSizeOfHeader+40;
               static const int kByteOffsetOfWizardPassword = kByteSizeOfHeader+41;
               static const int kByteOffsetOfFlags = kByteSizeOfHeader+72;
               static const int kByteOffsetOfPuidCounter = kByteSizeOfHeader+76;
               static const int kByteOffsetOfPuidCrc = kByteSizeOfHeader+80;
               static const int kByteOffsetOfDemoElapsed = kByteSizeOfHeader+84;
               static const int kByteOffsetOfTotalElapsed = kByteSizeOfHeader+88;
               static const int kByteOffsetOfDemoLimit = kByteSizeOfHeader+92;
               static const int kByteOffsetOfInitialRoomId = kByteSizeOfHeader+96;
               static const int kByteOffsetOfReserved = kByteSizeOfHeader+98; // size: 6
               static const int kByteOffsetOfUploadRequestedProtocolVersion = kByteSizeOfHeader+104;
               static const int kByteOffsetOfUploadCapabilities = kByteSizeOfHeader+108;
               static const int kByteOffsetOfDownloadCapabilities = kByteSizeOfHeader+112;
               static const int kByteOffsetOfEngineCapabilities2d = kByteSizeOfHeader+116;
               static const int kByteOffsetOfGraphicsCapabilities2d = kByteSizeOfHeader+120;
               static const int kByteOffsetOfGraphicsCapabilities3d = kByteSizeOfHeader+124;

            private:
               void doInit();

            public:
               u32 regCrc() { return u32At(kByteOffsetOfRegCrc); }
               void setRegCrc(u32 value) { setU32At(kByteOffsetOfRegCrc, value); }
               u32 regCounter() { return u32At(kByteOffsetOfRegCounter); }
               void setRegCounter(u32 value) { setU32At(kByteOffsetOfRegCounter, value); }
               std::string username() { return stdStringAt(kByteOffsetOfUsername, kByteSizeOfUsernameMax); }
               void setUsername(std::string value) { setStdStringAt(kByteOffsetOfUsername, value); }
               std::string usernameAsShortStdString()
                  { return shortStdStringAt(kByteOffsetOfUsernameLen); }
               void setUsernameAsShortStdString(const std::string value)
                  { setShortStdStringAt(kByteOffsetOfUsernameLen, value); }
               u8 usernameLen() { return u8At(kByteOffsetOfUsernameLen); }
               void setUsernameLen(u8 value) { setU8At(kByteOffsetOfUsernameLen, value); }
               std::string wizardPassword() { return stdStringAt(kByteOffsetOfWizardPassword, kByteSizeOfWizardPasswordMax); }
               void setWizardPassword(std::string value) { return setStdStringAt(kByteOffsetOfWizardPassword, value); }
               u32 flags() { return u32At(kByteOffsetOfFlags); }
               void setFlags(u32 value) { setU32At(kByteOffsetOfFlags, value); }
               u32 puidCounter() { return u32At(kByteOffsetOfPuidCounter); }
               void setPuidCounter(u32 value) { setU32At(kByteOffsetOfPuidCounter, value); }
               u32 puidCrc() { return u32At(kByteOffsetOfPuidCrc); }
               void setPuidCrc(u32 value) { setU32At(kByteOffsetOfPuidCrc, value); }
               u32 demoElapsed() { return u32At(kByteOffsetOfDemoElapsed); }
               void setDemoElapsed(u32 value) { setU32At(kByteOffsetOfDemoElapsed, value); }
               u32 totalElapsed() { return u32At(kByteOffsetOfTotalElapsed); }
               void setTotalElapsed(u32 value) { setU32At(kByteOffsetOfTotalElapsed, value); }
               u32 demoLimit() { return u32At(kByteOffsetOfDemoLimit); }
               void setDemoLimit(u32 value) { setU32At(kByteOffsetOfDemoLimit, value); }
               u16 initialRoomId() { return u16At(kByteOffsetOfInitialRoomId); }
               void setInitialRoomId(u16 value) { setU16At(kByteOffsetOfInitialRoomId, value); }
               std::string reserved() { return stdStringAt(kByteOffsetOfReserved, kByteSizeOfIdent); }
               void setReserved(std::string value) { setStdStringAt(kByteOffsetOfReserved, value); }
               u32 uploadRequestedProtocolVersion() { return u32At(kByteOffsetOfUploadRequestedProtocolVersion); }
               void setUploadRequestedProtocolVersion(u32 value) { setU32At(kByteOffsetOfUploadRequestedProtocolVersion, value); }
               u32 uploadCapabilities() { return u32At(kByteOffsetOfUploadCapabilities); }
               void setUploadCapabilities(u32 value) { setU32At(kByteOffsetOfUploadCapabilities, value); }
               u32 downloadCapabilities() { return u32At(kByteOffsetOfDownloadCapabilities); }
               void setDownloadCapabilities(u32 value) { setU32At(kByteOffsetOfDownloadCapabilities, value); }
               u32 engineCapabilities2d() { return u32At(kByteOffsetOfEngineCapabilities2d); }
               void setEngineCapabilities2d(u32 value) { setU32At(kByteOffsetOfEngineCapabilities2d, value); }
               u32 graphicsCapabilities2d() { return u32At(kByteOffsetOfGraphicsCapabilities2d); }
               void setGraphicsCapabilities2d(u32 value) { setU32At(kByteOffsetOfGraphicsCapabilities2d, value); }
               u32 graphicsCapabilities3d() { return u32At(kByteOffsetOfGraphicsCapabilities3d); }
               void setGraphicsCapabilities3d(u32 value) { setU32At(kByteOffsetOfGraphicsCapabilities3d, value); }

               virtual ~Logon();

               Logon(bool shouldSwapEndianness = false);
               Logon(const Generic& netMsg, bool shouldSwapEndianness = false);
         };
      }
   }
}

#endif // PALACE_NETMSG_LOGON_H
