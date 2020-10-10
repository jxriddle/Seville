#include "seville/base/types.h"
#include "seville/palace/base/cipher.h"

namespace seville
{
   namespace palace
   {
      // Cipher::Cipher(void) {}

      Cipher::~Cipher(void) {}

      auto Cipher::New(void) -> std::unique_ptr<Cipher> //std::optional<Cipher*>
      {
         //auto instance = std::make_optional<Cipher*>();
         auto instance_ptr = std::make_unique<Cipher>();

//         if (nullptr != instance_ptr) {
//            instance_ptr
//         }

         return instance_ptr;
      }

      auto Cipher::do_init_(void) -> void
      {
         my_seed_ = 0xa2c2a;
         for (auto i = u32{0}; i < kLookupTableElementCount; i++) {
            my_lookuptable_[i] = random_i16(kLookupTableValueMax);
         }
      }

      auto Cipher::do_reset_(void) -> void
      {
         my_seed_ = 1;
      }

      auto Cipher::encipher( \
            const QByteArray& input, \
            i32 bytelimit_input) -> QByteArray
      {
         auto output = QByteArray();
         auto bytesize_input = input.length();
         if (bytesize_input <= 0)
            return output;

         for (auto i = 0; i < bytesize_input && i < bytelimit_input; i++)
            output.insert(i, input[i]);

         auto i_lookuptable = u32{0};
         char stored_byte = 0;
         for (auto i = bytesize_input - 1; 0 <= i; i--) {
            output[i] = static_cast<char>( \
                        static_cast<i16>(input[i]) ^ \
                        my_lookuptable_[i_lookuptable++] ^ \
                        static_cast<i16>(stored_byte));

            stored_byte = static_cast<char>( \
                          static_cast<i16>(output[i]) ^ \
                          my_lookuptable_[i_lookuptable++]);
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
            output[i] = static_cast<char>( \
                        static_cast<i16>(input[i]) ^ \
                        my_lookuptable_[i_lookuptable++] ^ \
                        static_cast<i16>(stored_byte));

            stored_byte = static_cast<char>( \
                          static_cast<i16>(input[i]) ^ \
                          my_lookuptable_[i_lookuptable++]);
         }

         output.append('\0');
         return output;
      }

      auto Cipher::random_f64(void) -> f64
      {
         return static_cast<f64>(
               static_cast<f64>(random_i32()) / static_cast<f64>(0x7fffffff));
      }

      auto Cipher::random_i32(void) -> i32
      {
         auto seed = my_seed_;
         auto quotient = seed / 0x1f31d;
         auto remainder = seed % 0x1f31d;
         auto a = 0x41a7 * remainder - 0xb14 * quotient;
         set_seed(a + ((a <= 0) * 0x7fffffff));

         return my_seed_;
      }

      auto Cipher::random_i16(i16 max) -> i16
      {
         auto f64val = random_f64();
         auto i32val = i32(f64val * double(max));

         return static_cast<i16>(i32val);
      }

      auto Cipher::random_i8(i8 max) -> i8
      {
         auto f64_val = random_f64();
         auto i16_val = static_cast<i16>(f64_val * double(max));

         return static_cast<i8>(i16_val);
      }
   }
}
