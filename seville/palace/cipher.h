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

         static std::unique_ptr<Cipher> New(void);
         //std::optional<Cipher*>;

         QByteArray encipher(
               const QByteArray& plaintext,
               i32 plaintextLimit = kPlaintextLimit);

         QByteArray decipher(const QByteArray& ciphertext);

         i8 randomI8(i8 max);
         i16 randomI16(i16 max);
         i32 randomI32(void);
         f64 randomF64(void);

         inline i32 seed(void) const {
            return my_seed;
         }

         inline void setSeed(i32 seed) {
            /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
            do_setSeed(seed);
         }

         inline void clear(void) {
            do_clear();
         }

//         inline auto init(void) -> void {
//            do_init_();
//         }

      private:
         i16 my_lut[kLookupTableElementCount];
         i32 my_seed;

         void do_init(void);
         void do_setSeed(i32 seed);
         void do_deinit(void);
         void do_clear(void);
      };
   }
}

#endif  // SEVILLE_PALACE_CIPHER_H_
