#include "Palace_Crypto.h"

namespace Seville
{
   namespace Palace
   {
      Crypto::Crypto()
      {
          doReset();
      }

      void Crypto::reset()
      {
          doReset();
      }

      void Crypto::doReset()
      {
          doSetSeed(0xa2c2a);
          for (int i = 0; i < kLutCount; i++) {
              myLut[i] = doGetRandomU16(kLutMax);
          }
          doSetSeed(0);
      }

      void Crypto::encrypt(const QString& fromPlaintext, QByteArray& toCipher, int byteLimit)
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
              toCipher[i] = plaintextBytes[i] ^ myLut[rc++] ^ quint16(lastChar);
              lastChar = toCipher[i] ^ myLut[rc++];
          }
      }

      void Crypto::decrypt(const QByteArray& fromCipher, QString& toPlaintext)
      {
          QByteArray plaintextBytes;
          int nCipherBytes = fromCipher.length();
          if (nCipherBytes <= 0) return;

          char lastChar = 0;
          quint32 rc = 0;
          for (int i = nCipherBytes - 1; 0 <= i; i--)
          {
              plaintextBytes[i] = fromCipher[i] ^ myLut[rc++] ^ lastChar;
              lastChar = fromCipher[i] ^ myLut[rc++];
          }
          plaintextBytes.append('\0');

          toPlaintext = QString::fromUtf8(plaintextBytes);
          if (toPlaintext.toUtf8() != plaintextBytes)
          {
              toPlaintext = QString::fromLatin1(plaintextBytes);
          }
      }

      quint32 Crypto::doGetRandomU32()
      {
          int seed = doGetSeed();
          quint32 quotient = seed / 0x1f31d;
          quint32 remainder = seed % 0x1f31d;
          quint32 a = 0x41a7 * remainder - 0xb14 * quotient;
          setSeed(a + ((a <= 0) * 0x7fffffff));
          return doGetSeed();
      }

      double Crypto::doGetRandomDouble()
      {
          return double(double(doGetRandomU32()) / double(0x7fffffff));
      }

      quint16 Crypto::doGetRandomU16(int max)
      {
          double dbl = doGetRandomDouble();
          quint32 u32 = quint32(dbl * double(max));
          return quint16(u32);
      }

      quint8 Crypto::doGetRandomU8(int max)
      {
          double dbl = doGetRandomDouble();
          quint16 u16 = quint16(dbl * double(max));
          return quint8(u16);
      }

      // should be inlined
      void Crypto::doSetSeed(quint32 seed) { mySeed = (!seed) | seed; } /* logical-not (!) changes non-zero -> 0, 0 -> 1 */
      quint32 Crypto::doGetSeed() { return mySeed; }

      void Crypto::setSeed(quint32 seed) { doSetSeed(seed); }
      quint32 Crypto::seed() { return doGetSeed(); }
   }
}
