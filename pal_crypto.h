#ifndef PALCRYPTO_H
#define PALCRYPTO_H

#include <QObject>

namespace Seville
{
   class PalCrypto
   {
      public:
         static const int kLutCount = 512;
         static const int kLutMax = 256;
         static const int kPlaintextLimit = 254;

         short lut_[kLutCount];
         int seed_;

         PalCrypto();
         void resetState();
         quint32 seed();
         void setSeed(quint32 seed);
         void encrypt(
               const QString& fromPlaintext,
               QByteArray& toCipher,
               int byteLimit = kPlaintextLimit);
         void decrypt(
               const QByteArray& fromCipher, QString& toPlaintext);

         quint32 doGetSeed_();
         void doSetSeed_(quint32 seed);
         void doResetState_();
         quint32 doGetRandomU32_();
         double doGetRandomDouble_();
         quint16 doGetRandomU16_(int max);
         quint8 doGetRandomU8_(int max);

      private:
   };
}

#endif // PALCRYPTO_H
