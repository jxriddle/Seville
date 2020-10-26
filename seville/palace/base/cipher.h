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
               i32 byteLimit = kPlaintextLimit) -> QByteArray;

         auto decipher(const QByteArray& input) -> QByteArray;

         auto random_i8(i8 max) -> i8;
         auto random_i16(i16 max) -> i16;
         auto random_i32(void) -> i32;
         auto random_f64(void) -> f64;

         inline auto seed(void) const -> i32 {
            return my_seed_;
         }

         inline auto set_seed(i32 seed) -> void {
            /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
            my_seed_ = (!seed) | seed;
         }

         inline auto reset(void) -> void {
            do_reset_();
         }

//         inline auto init(void) -> void {
//            do_init_();
//         }

      private:
         short my_lookuptable_[kLookupTableElementCount];
         i32 my_seed_;

         auto do_init_(void) -> void;
         auto do_deinit_(void) -> void;
         auto do_reset_(void) -> void;
      };
   }
}

#endif  // SEVILLE_PALACE_CIPHER_H_
