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
            : GenericNetMsg(shouldSwapEndianness)
         {
            do_init_();
         }

         Logon::Logon(const GenericNetMsg& netMsg, bool shouldSwapEndianness)
            : GenericNetMsg(netMsg, shouldSwapEndianness)
         {
            do_init_();
         }
      }
   }
}
