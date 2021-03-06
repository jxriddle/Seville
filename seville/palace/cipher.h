#ifndef SEVILLE_PALACE_CIPHER_H_
#define SEVILLE_PALACE_CIPHER_H_

#include <optional>

#include <QObject>

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      class Cipher
      {
      public:
         static const i32 kLookupTableElementCount = 512;
         static const i32 kLookupTableValueMax = 256;
         static const i32 kPlaintextLimit = 254;

         explicit Cipher(void); // = delete;
         virtual ~Cipher(void);

         static auto New(void) -> std::unique_ptr<Cipher>;
         //std::optional<Cipher*>;

         auto encipher(
               const QByteArray& input,
               i32 byte_limit = kPlaintextLimit) -> QByteArray;

         auto decipher(const QByteArray& input) -> QByteArray;

         auto randomI8(i8 max) -> i8;
         auto randomI16(i16 max) -> i16;
         auto randomI32(void) -> i32;
         auto randomF64(void) -> f64;

         inline auto seed(void) const -> i32 {
            return my_seed;
         }

         inline auto setSeed(i32 seed) -> void {
            /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
            do_setSeed(seed);
         }

         inline auto clear(void) -> void {
            do_clear();
         }

//         inline auto init(void) -> void {
//            do_init_();
//         }

      private:
         short my_lut[kLookupTableElementCount];
         i32 my_seed;

         auto do_init(void) -> void;
         auto do_setSeed(int seed) -> void;
         auto do_deinit(void) -> void;
         auto do_clear(void) -> void;
      };
   }
}

#endif  // SEVILLE_PALACE_CIPHER_H_
