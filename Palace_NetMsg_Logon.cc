#include "Palace_NetMsg_Logon.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void Logon::doInit()
         {
            setKind(NetMsg::Kind::LogonKind);
            setSizeExpected(kByteSizeOfLogon);
            setObjectId(0);

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

         Logon::~Logon()
         {

         }

         Logon::Logon(bool shouldSwapEndianness)
            : Generic(shouldSwapEndianness)
         {
            doInit();
         }

         Logon::Logon(const Generic& netMsg, bool shouldSwapEndianness)
            : Generic(netMsg, shouldSwapEndianness)
         {
            //TODO move
            doInit();
            //reset();
            //append(netMsg);
         }
      }
   }
}
