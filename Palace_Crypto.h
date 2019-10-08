#ifndef PALACE_CRYPTO_H
#define PALACE_CRYPTO_H

#include "Common.h"
#include <QObject>

namespace Seville
{
   namespace Palace
   {
      class Crypto
      {
         public:
            static const i32 kLutElementCount = 512;
            static const i32 kLutValueMax = 256;
            static const i32 kPlaintextLimit = 254;

         private:
            short myLut[kLutElementCount];
            i32 mySeed;

            i32 doGetSeed(void);
            void doSetSeed(i32 seed);
            void doReset(void);
            i32 doGetRandomI32(void);
            double doGetRandomDouble(void);
            i16 doGetRandomI16(i16 max);
            i8 doGetRandomI8(i8 max);

         public:
            i32 seed(void) { return doGetSeed(); }
            void setSeed(i32 seed) { doSetSeed(seed); }

            QByteArray encrypt(
                  const QByteArray& plaintext,
                  i32 byteLimit = kPlaintextLimit);
            QByteArray decrypt(const QByteArray& cipherText);

            void reset(void) { doReset(); }

            Crypto(void);
      };
   }
}

#endif // PALACE_CRYPTO_H
