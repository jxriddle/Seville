#include "prop.h"

namespace seville
{
   namespace palace
   {
      Prop::Prop(void)
      {
         do_init();
      }

      auto Prop::do_init(void) -> void {
          my_id = 0;
          my_crc = 0;
      }

   } // namespace Palace
}
