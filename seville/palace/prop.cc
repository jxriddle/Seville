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

      Prop::Prop(const Prop& value)
      {
         do_init(value);
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
         my_propImage = QImage();

         // if (my_propImagePtr == nullptr)
         //    delete my_propImagePtr;

         // my_propImagePtr = new QImage();
      }

      void Prop::do_init(void)
      {
         // my_propImage = new QImage();
      }

      void Prop::do_init(const Prop& value)
      {
         do_assign(value);
      }

      void Prop::do_assign(const Prop& value)
      {
         my_id = value.my_id;
         my_crc = value.my_crc;
         my_name = value.my_name;
         my_size = value.my_size;
         my_flags = value.my_flags;
         my_format = value.my_format;
         my_imgUrl = value.my_imgUrl;
         my_offset = value.my_offset;
         my_propImage = value.my_propImage;
         my_imageLoadedFlag = value.my_imageLoadedFlag;
      }

   } // namespace Palace
}
