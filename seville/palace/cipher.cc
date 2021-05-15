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

      std::unique_ptr<Cipher> Cipher::New(void) //std::optional<Cipher*>
      {
         //auto instance = std::make_optional<Cipher*>();
         auto instance_ptr = std::make_unique<Cipher>();
         instance_ptr->do_init();

//         if (nullptr != instance_ptr) {
//            instance_ptr
//         }

         return instance_ptr;
      }

      QByteArray Cipher::encipher(
            const QByteArray& plaintext, i32 plaintextLimit)
      {
         auto cleanPlaintext = QByteArray();
         auto plaintextLength = plaintext.length();

         if (plaintextLength <= 0)
            return QByteArray();

         auto len = 0;
         while (len < plaintextLength && len < plaintextLimit) {
            //cleanPlaintext = plaintext.mid(0, plaintextLimit);
            cleanPlaintext.append(plaintext[len]);
            len++;
         }

         auto ciphertext = QByteArray();
         ciphertext.resize(len);
         auto j = i32{0};
         auto spareByte = u8{0};
         for (auto i = len - 1; 0 <= i; i--) {
            ciphertext[i] =
                  static_cast<i8>(cleanPlaintext[i] ^ my_lut[j++] ^ spareByte);
            spareByte = static_cast<u8>(ciphertext[i] ^ my_lut[j++]);
         }
         //ciphertext.append('\0');

         return ciphertext;
      }

      QByteArray Cipher::decipher(const QByteArray& ciphertext)
      {
         auto ciphertextLen = ciphertext.length();

         if (ciphertextLen <= 0)
            return QByteArray();

         auto plaintext = QByteArray();
         plaintext.resize(ciphertextLen);
         auto j = i32{0};
         auto spareByte = u8{0};
         for (auto i = ciphertextLen - 1; 0 <= i; i--) {
            plaintext[i] =
                  static_cast<i8>(ciphertext[i] ^ my_lut[j++] ^ spareByte);
            spareByte = static_cast<u8>(ciphertext[i] ^ my_lut[j++]);
         }
         plaintext.append('\0');

         return plaintext;
      }

      f64 Cipher::randomF64(void)
      {
         return static_cast<f64>(randomI32()) / static_cast<f64>(0x7fffffff);
      }

      i32 Cipher::randomI32(void)
      {
         auto seed = my_seed;
         auto quotient = seed / 0x1f31d;
         auto remainder = seed % 0x1f31d;
         auto a = 0x41a7 * remainder - 0xb14 * quotient;
         setSeed(a + (a <= 0) * 0x7fffffff);
         return my_seed;
      }

      i16 Cipher::randomI16(i16 max)
      {
         auto f64val = randomF64();
         auto i32val = i32(f64val * double(max));

         return static_cast<i16>(i32val);
      }

      i8 Cipher::randomI8(i8 max)
      {
         auto f64_val = randomF64();
         auto i16_val = static_cast<i16>(f64_val * double(max));

         return static_cast<i8>(i16_val);
      }

      void Cipher::do_clear(void)
      {
         my_seed = 0;
      }

      void Cipher::do_setSeed(int seed)
      {
         my_seed = seed | !seed;
      }

      void Cipher::do_init(void)
      {
         my_seed = 0xa2c2a;
         //for (auto i = u32{0}; i < kLookupTableElementCount; i++) {
         for (auto i = 0; i < kLookupTableElementCount; i++) {
            my_lut[i] = randomI16(kLookupTableValueMax);
         }

         my_seed = 1;
      }
   }
}
