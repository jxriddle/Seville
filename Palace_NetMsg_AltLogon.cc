#include "Palace_NetMsg_AltLogon.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         AltLogon::~AltLogon()
         {

         }

         AltLogon::AltLogon()
         {

         }

         AltLogon::AltLogon(const NetMsgBase& netMsg)
         {
            //TODO move
            reset();
            append(netMsg);
         }
      }
   }
}
