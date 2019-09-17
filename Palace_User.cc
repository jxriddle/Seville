#include "Palace_User.h"

namespace Seville
{
   namespace Palace
   {
      void User::doReset()
      {
         myColor = 1;
         myFace = 1;
         myX = 0;
         myY = 0;
         myIdChanged = false;
         myIdCounter = 0xf5dc385e;
         myIdCrc = 0xc144c580;
         myRegCounter = 0xcf07309c;
         myRegCrc = 0x5905f923;
         myUsername = "Seville User";
         myWizpass = "";
      }

      User::~User()
      {

      }

      User::User()
      {
         doReset();
      }
   }
}
