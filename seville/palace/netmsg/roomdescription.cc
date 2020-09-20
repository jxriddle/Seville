#include "generic.h"
#include "roomdescription.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         RoomDescription::RoomDescription(bool shouldSwapEndianness)
            : Generic(shouldSwapEndianness)
         {
            do_init();
         }

         RoomDescription::RoomDescription(const Generic& netMsg, bool shouldSwapEndianness)
            : Generic(netMsg, shouldSwapEndianness)
         {
            do_init();
         }
      }
   }
}
