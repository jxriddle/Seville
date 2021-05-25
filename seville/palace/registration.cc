#include "registration.h"

namespace seville
{
   namespace palace
   {
      // Registration::Registration(u32 seed, u32 puid)
      Registration::Registration(void)
      {
      }

      void Registration::setSeed(u32 seed, u32 puid)
      {
         my_crc = do_computeCrc(seed);
         my_counter = (seed ^ kRegMagic) ^ my_crc;
         my_puidCrc = do_computeCrc(puid);
         my_puidCounter = puid ^ my_puidCrc;
      }

      u32 Registration::do_computeCrc(u32 iv)
      {
         auto crc = kCrcMagic;
         auto i = kNumBytes;
         while (0 <= i)
            crc = ((crc << 1) | (!(crc & 0x80000000))) ^
                  my_crcMask[(iv >> (i << 3) & 0xff)];

         return crc;
      }
   }
}
