#include "Common.h"
#include "Palace_Crypto.h"

namespace Seville
{
   namespace Palace
   {
      i32 Crypto::doGetSeed(void)
      {
         return mySeed;
      }

      /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
      void Crypto::doSetSeed(i32 seed)
      {
         mySeed = (!seed) | seed;
      }

      QByteArray Crypto::encrypt(const QByteArray& plaintext, i32 byteLimit)
      {
         auto res = QByteArray();
         auto byteSizeOfPlaintext = plaintext.length();
         if (byteSizeOfPlaintext <= 0)
            return res;

         for (auto i = 0; i < byteSizeOfPlaintext && i < byteLimit; i++)
            res.insert(i, plaintext[i]);

         u32 lutIdx = 0;
         char previousCipherByte = 0;
         for (auto i = byteSizeOfPlaintext - 1; 0 <= i; i--) {
            res[i] = static_cast<char>(
               static_cast<i16>(plaintext[i]) ^ myLut[lutIdx++] ^ static_cast<i16>(previousCipherByte));
            previousCipherByte = static_cast<char>(
               static_cast<i16>(res[i]) ^ myLut[lutIdx++]);
         }
         res.append('\0');

         return res;
      }

      QByteArray Crypto::decrypt(const QByteArray& ciphertext)
      {
         auto res = QByteArray();
         auto byteSizeOfCiphertext = ciphertext.length();
         if (byteSizeOfCiphertext <= 0)
            return res;

         u32 lutIdx = 0;
         char previousCipherByte = 0;
         for (auto i = byteSizeOfCiphertext - 1; 0 <= i; i--) {
            res[i] = static_cast<char>(
               static_cast<i16>(ciphertext[i]) ^ myLut[lutIdx++] ^ static_cast<i16>(previousCipherByte));
            previousCipherByte = static_cast<char>(
               static_cast<i16>(ciphertext[i]) ^ myLut[lutIdx++]);
         }
         res.append('\0');

         return res;
      }

      i32 Crypto::doGetRandomI32(void)
      {
         auto seed = doGetSeed();
         auto quotient = seed / 0x1f31d;
         auto remainder = seed % 0x1f31d;
         auto a = 0x41a7 * remainder - 0xb14 * quotient;
         setSeed(a + ((a <= 0) * 0x7fffffff));

         return doGetSeed();
      }

      f64 Crypto::doGetRandomDouble(void)
      {
         return static_cast<f64>(
            static_cast<f64>(doGetRandomI32()) / static_cast<f64>(0x7fffffff));
      }

      i16 Crypto::doGetRandomI16(i16 max)
      {
         f64 dbl = doGetRandomDouble();
         i32 i32val = i32(dbl * double(max));

         return static_cast<i16>(i32val);
      }

      i8 Crypto::doGetRandomI8(i8 max)
      {
         double dbl = doGetRandomDouble();
         i16 i16val = static_cast<i16>(dbl * double(max));

         return static_cast<i8>(i16val);
      }

      void Crypto::doReset(void)
      {
         doSetSeed(0xa2c2a);
         for (auto i = static_cast<u32>(0); i < kLutElementCount; i++)
            myLut[i] = doGetRandomI16(kLutValueMax);

         doSetSeed(0);
      }

      Crypto::Crypto()
      {
         doReset();
      }
   }
}
