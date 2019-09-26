#ifndef APP_PASCALSTRING_H
#define APP_PASCALSTRING_H

#include "Common.h"

namespace App
{
   class U8PascalString
   {
      private:
         u8 length;
         char* myBytes;

      public:
         U8PascalString();
   };
}

#endif
