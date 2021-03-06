#ifndef PALACE_PROP_H_
#define PALACE_PROP_H_

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      class Prop
      {
      public:
         Prop(void);

         inline auto id(void) -> i32 {
            return my_id;
         }

         inline auto setId(i32 value) {
            my_id = value;
         }

         inline auto crc(void) -> i32 {
            return my_crc;
         }

         inline auto setCrc(i32 value) -> void {
            my_crc = value;
         }

      private:
         i32 my_id;
         i32 my_crc;

         void do_init(void);
      };
   } // namespace Palace
}

#endif // PALACE_PROP_H_
