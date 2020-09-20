#include "seville/base/types.h"
#include "seville/palace/base/cipher.h"

namespace seville
{
   namespace palace
   {
      Cipher::Cipher(void)
      {
         do_init();
      }

      Cipher::~Cipher(void)
      {
         do_deinit();
      }

      auto Cipher::encipher(const QByteArray& input, i32 byteLimit) -> QByteArray
      {
         auto output = QByteArray();
         auto byteSizeOfInput = input.length();
         if (byteSizeOfInput <= 0)
            return output;

         for (auto i = 0; i < byteSizeOfInput && i < byteLimit; i++)
            output.insert(i, input[i]);

         auto lutIdx = u32(0);
         char storedByte = 0;
         for (auto i = byteSizeOfInput - 1; 0 <= i; i--) {
            output[i] = static_cast<char>(
               static_cast<i16>(input[i]) ^ my_lut[lutIdx++] ^ static_cast<i16>(storedByte));
            storedByte = static_cast<char>(static_cast<i16>(output[i]) ^ my_lut[lutIdx++]);
         }
         output.append('\0');
         return output;
      }

      auto Cipher::decipher(const QByteArray& input) -> QByteArray
      {
         auto res = QByteArray();
         auto byteSizeOfInput = input.length();
         if (byteSizeOfInput <= 0) {
            return res;
         }

         auto lutIdx = u32(0);
         char storedByte = 0;
         for (auto i = byteSizeOfInput - 1; 0 <= i; i--) {
            res[i] = static_cast<char>(
               static_cast<i16>(input[i]) ^ my_lut[lutIdx++] ^ static_cast<i16>(storedByte));
            storedByte = static_cast<char>(static_cast<i16>(input[i]) ^ my_lut[lutIdx++]);
         }
         res.append('\0');
         return res;
      }

      auto Cipher::randomDouble(void) -> f64
      {
         return static_cast<f64>(static_cast<f64>(randomI32()) / static_cast<f64>(0x7fffffff));
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
         auto f64val = randomDouble();
         auto i32val = i32(f64val * double(max));

         return static_cast<i16>(i32val);
      }

      auto Cipher::randomI8(i8 max) -> i8
      {
         auto f64val = randomDouble();
         auto i16val = static_cast<i16>(f64val * double(max));

         return static_cast<i8>(i16val);
      }

      auto Cipher::do_reset(void) -> void
      {
         my_seed = 0xa2c2a;
         for (auto i = static_cast<u32>(0); i < kLutElementCount; i++) {
            my_lut[i] = randomI16(kLutValueMax);
         }

         my_seed = 1;
      }

      auto Cipher::do_init(void) -> void
      {
         reset();
      }

      auto Cipher::do_deinit(void) -> void
      {
      }
   }
}
