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
