#ifndef PALACE_CIPHER_H_
#define PALACE_CIPHER_H_

#include <QObject>

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      class Cipher
      {
      public:
         static const i32 kLutElementCount = 512;
         static const i32 kLutValueMax = 256;
         static const i32 kPlaintextLimit = 254;

         Cipher(void);
         virtual ~Cipher(void);

         inline auto seed(void) -> i32 { return my_seed; }
         inline auto setSeed(i32 seed) -> void
         {
            /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
            my_seed = (!seed) | seed;
         }

         inline auto reset(void) -> void { do_reset(); }

         auto encipher(const QByteArray& input, i32 byteLimit = kPlaintextLimit) -> QByteArray;
         auto decipher(const QByteArray& input) -> QByteArray;

         auto randomI8(i8 max) -> i8;
         auto randomI16(i16 max) -> i16;
         auto randomI32(void) -> i32;
         auto randomDouble(void) -> f64;

      private:
         short my_lut[kLutElementCount];
         i32 my_seed;

         auto do_init(void) -> void;
         auto do_deinit(void) -> void;
         auto do_reset(void) -> void;
      };
   }
}

#endif // PALACE_CIPHER_H_
