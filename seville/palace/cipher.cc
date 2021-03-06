#include "seville/base/types.h"
#include "seville/palace/cipher.h"

namespace seville
{
   namespace palace
   {
      Cipher::Cipher(void)
      {
         do_init();
      }

      Cipher::~Cipher(void) {}

      auto Cipher::New(void) -> std::unique_ptr<Cipher> //std::optional<Cipher*>
      {
         //auto instance = std::make_optional<Cipher*>();
         auto instance_ptr = std::make_unique<Cipher>();
         instance_ptr->do_init();

//         if (nullptr != instance_ptr) {
//            instance_ptr
//         }

         return instance_ptr;
      }

      auto Cipher::encipher(
            const QByteArray& input,
            i32 bytelimit_input) -> QByteArray
      {
         auto output = QByteArray();
         auto bytesize_input = input.length();
         if (bytesize_input <= 0)
            return output;

         for (auto i = 0; i < bytesize_input && i < bytelimit_input; i++)
            output.insert(i, input[i]);

         auto lut_idx = u32{0};
         char stored_byte = 0;
         for (auto i = bytesize_input - 1; 0 <= i; i--) {
            output[i] = static_cast<char>(
                        static_cast<i16>(input[i]) ^
                        my_lut[lut_idx++] ^
                        static_cast<i16>(stored_byte));

            stored_byte =
                  static_cast<char>(
                     static_cast<i16>(output[i]) ^ my_lut[lut_idx++]);
         }

         output.append('\0');
         return output;
      }

      auto Cipher::decipher(const QByteArray& input) -> QByteArray
      {
         auto output = QByteArray();
         auto bytesize_input = input.length();
         if (bytesize_input <= 0) {
            return output;
         }

         auto i_lookuptable = u32{0};
         char stored_byte = 0;
         for (auto i = bytesize_input - 1; 0 <= i; i--) {
            output[i] =
                  static_cast<char>(
                     static_cast<i16>(input[i]) ^
                     my_lut[i_lookuptable++] ^
                  static_cast<i16>(stored_byte));

            stored_byte =
                  static_cast<char>(
                     static_cast<i16>(input[i]) ^
                     my_lut[i_lookuptable++]);
         }

         output.append('\0');
         return output;
      }

      auto Cipher::randomF64(void) -> f64
      {
         return static_cast<f64>(
               static_cast<f64>(randomI32()) / static_cast<f64>(0x7fffffff));
      }

      auto Cipher::randomI32(void) -> i32
      {
         auto seed = my_seed;
         auto quotient = seed / 0x1f31d;
         auto remainder = seed % 0x1f31d;
         auto a = 0x41a7 * remainder - 0xb14 * quotient;
         setSeed(a + ((a <= 0) * 0x7fffffff));

         return my_seed;
      }

      auto Cipher::randomI16(i16 max) -> i16
      {
         auto f64val = randomF64();
         auto i32val = i32(f64val * double(max));

         return static_cast<i16>(i32val);
      }

      auto Cipher::randomI8(i8 max) -> i8
      {
         auto f64_val = randomF64();
         auto i16_val = static_cast<i16>(f64_val * double(max));

         return static_cast<i8>(i16_val);
      }

      auto Cipher::do_clear(void) -> void
      {
         my_seed = 1;
      }

      auto Cipher::do_setSeed(int seed) -> void
      {
         my_seed = (!seed) | seed;
      }

      auto Cipher::do_init(void) -> void
      {
         my_seed = 0xa2c2a;
         for (auto i = u32{0}; i < kLookupTableElementCount; i++) {
            my_lut[i] = randomI16(kLookupTableValueMax);
         }
      }
   }
}
