#include "prop.h"

namespace seville
{
   namespace palace
   {
      Prop::Prop(void)
      {
         do_init();
         do_clear();
      }

      void Prop::do_clear(void) {
         my_id = 0;
         my_crc = 0;

         my_imgUrl = "";
         my_format = "";
         my_name = "";
         my_size = QSize(0, 0);
         my_offset = QPoint(0, 0);
         my_flags = 0;
         my_imageLoadedFlag = 0;
      }

      void Prop::do_init(void) {
      }

   } // namespace Palace
}
