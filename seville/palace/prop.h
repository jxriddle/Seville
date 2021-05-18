#ifndef PALACE_PROP_H_
#define PALACE_PROP_H_

#include <QImage>

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      enum PropFlags: u32 {
         PROP_HEAD = 2,
         PROP_GHOST = 4,
         PROP_RARE = 8,
         PROP_ANIMATED = 16,
         PROP_BOUNCE = 32,
         PROP_PNG = 1024,
      };

      class Prop
      {
      public:
         Prop(void);
         Prop(const Prop& value);

         inline i32 id(void) {
            return my_id;
         }

         inline void setId(i32 value) {
            my_id = value;
         }

         inline i32 crc(void) {
            return my_crc;
         }

         inline void setCrc(i32 value) {
            my_crc = value;
         }

         inline QString imgUrl(void) {
            return my_imgUrl;
         }

         inline void setImgUrl(const QString& value) {
            my_imgUrl = value;
         }

         inline QString format(void) {
            return my_format;
         }

         inline void setFormat(const QString& format) {
            my_format = format;
         }

         inline QSize size(void) {
            return my_size;
         }

         inline void setSize(const QSize& value) {
            my_size = value;
         }

         inline QPoint offset(void) {
            return my_offset;
         }

         inline void setOffset(const QPoint& value) {
            my_offset = value;
         }

         inline i32 flags(void) {
            return my_flags;
         }

         inline void setFlags(i32 flags) {
            my_flags = flags;
         }

         inline QImage* propImagePtr(void) {
            return &my_propImage;
         }

         inline void setPropImage(const QImage& value) {
            my_propImage = value;
         }

         inline i32 imageLoadedFlag(void) {
            return my_imageLoadedFlag;
         }

         inline void setImageLoadedFlag(i32 value) {
            my_imageLoadedFlag = value;
         }

         inline QString name(void) {
            return my_name;
         }

         inline void setName(const QString& value) {
            my_name = value;
         }

         Prop operator=(const Prop& value) {
            do_assign(value);
            return *this;
         }

      private:
         i32 my_id;
         i32 my_crc;

         QString my_imgUrl;
         QString my_format;
         QString my_name;
         QSize my_size;
         QPoint my_offset;
         i32 my_flags;
         i32 my_imageLoadedFlag;
         // QImage* my_propImagePtr;
         QImage my_propImage;

         void do_assign(const Prop& value);

         void do_init(const Prop& value);

         void do_clear(void);
         void do_init(void);
      };
   } // namespace Palace
}

#endif // PALACE_PROP_H_
