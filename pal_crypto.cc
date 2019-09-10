#include "pal_crypto.h"

namespace Seville
{
   PalCrypto::PalCrypto()
   {
       doResetState_();
   }

   void PalCrypto::resetState()
   {
       doResetState_();
   }

   void PalCrypto::doResetState_()
   {
       doSetSeed_(0xa2c2a);
       for (int i = 0; i < kLutCount; i++) {
           lut_[i] = doGetRandomU16_(kLutMax);
       }
       doSetSeed_(0);
   }

   void PalCrypto::encrypt(const QString& fromPlaintext, QByteArray& toCipher, int byteLimit)
   {
       QByteArray plaintextBytes(fromPlaintext.toUtf8());
       int nPlaintextBytes = plaintextBytes.length();
       if (nPlaintextBytes <= 0) return;

       for (int i = 0; i < nPlaintextBytes && i < byteLimit; i++)
       {
           toCipher.insert(i, plaintextBytes[i]);
       }

       quint32 rc = 0;
       char lastChar = 0;
       for (int i = nPlaintextBytes - 1; 0 <= i; i--)
       {
           toCipher[i] = plaintextBytes[i] ^ lut_[rc++] ^ quint16(lastChar);
           lastChar = toCipher[i] ^ lut_[rc++];
       }
   }

   void PalCrypto::decrypt(const QByteArray& fromCipher, QString& toPlaintext)
   {
       QByteArray plaintextBytes;
       int nCipherBytes = fromCipher.length();
       if (nCipherBytes <= 0) return;

       char lastChar = 0;
       quint32 rc = 0;
       for (int i = nCipherBytes - 1; 0 <= i; i--)
       {
           plaintextBytes[i] = fromCipher[i] ^ lut_[rc++] ^ lastChar;
           lastChar = fromCipher[i] ^ lut_[rc++];
       }
       plaintextBytes.append('\0');

       toPlaintext = QString::fromUtf8(plaintextBytes);
       if (toPlaintext.toUtf8() != plaintextBytes)
       {
           toPlaintext = QString::fromLatin1(plaintextBytes);
       }
   }

   quint32 PalCrypto::doGetRandomU32_()
   {
       int seed = doGetSeed_();
       quint32 quotient = seed / 0x1f31d;
       quint32 remainder = seed % 0x1f31d;
       quint32 a = 0x41a7 * remainder - 0xb14 * quotient;
       setSeed(a + ((a <= 0) * 0x7fffffff));
       return doGetSeed_();
   }

   double PalCrypto::doGetRandomDouble_()
   {
       return double(double(doGetRandomU32_()) / double(0x7fffffff));
   }

   quint16 PalCrypto::doGetRandomU16_(int max)
   {
       double dbl = doGetRandomDouble_();
       quint32 u32 = quint32(dbl * double(max));
       return quint16(u32);
   }

   quint8 PalCrypto::doGetRandomU8_(int max)
   {
       double dbl = doGetRandomDouble_();
       quint16 u16 = quint16(dbl * double(max));
       return quint8(u16);
   }

   // should be inlined
   void PalCrypto::doSetSeed_(quint32 seed) { seed_ = (!seed) | seed; } /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
   quint32 PalCrypto::doGetSeed_() { return seed_; }

   void PalCrypto::setSeed(quint32 seed) { doSetSeed_(seed); }
   quint32 PalCrypto::seed() { return doGetSeed_(); }
}
