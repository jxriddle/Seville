#include "logon.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         Logon::~Logon(void)
         {

         }

         Logon::Logon(bool shouldSwapEndianness)
            : Generic(shouldSwapEndianness)
         {
            do_init();
         }

         Logon::Logon(const Generic& netMsg, bool shouldSwapEndianness)
            : Generic(netMsg, shouldSwapEndianness)
         {
            do_init();
         }
      }
   }
}
