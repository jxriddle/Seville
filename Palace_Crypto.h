#ifndef PALACE_CRYPTO_H
#define PALACE_CRYPTO_H

#include <QObject>

namespace Seville
{
   namespace Palace
   {
      class Crypto
      {
         public:
            static const int kLutCount = 512;
            static const int kLutMax = 256;
            static const int kPlaintextLimit = 254;

            short myLut[kLutCount];
            int mySeed;

            Crypto();
            void reset();
            quint32 seed();
            void setSeed(quint32 seed);
            void encrypt(
                  const QString& fromPlaintext,
                  QByteArray& toCipher,
                  int byteLimit = kPlaintextLimit);
            void decrypt(
                  const QByteArray& fromCipher, QString& toPlaintext);

            quint32 doGetSeed();
            void doSetSeed(quint32 seed);
            void doReset();
            quint32 doGetRandomU32();
            double doGetRandomDouble();
            quint16 doGetRandomU16(int max);
            quint8 doGetRandomU8(int max);

         private:
      };
   }
}

#endif // PALACE_CRYPTO_H
