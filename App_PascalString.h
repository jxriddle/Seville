#ifndef APP_PASCALSTRING_H
#define APP_PASCALSTRING_H

#include "Common.h"
#include <memory>
#include <string>

namespace App
{
   class PascalString
   {
      private:
         u8 myLength;
         std::string myString;

      public:
         u8 length() { return myLength; }
         void setLength(u8 value) { myLength = value; }
         std::string string() { return myString; }
         void setBytes(std::string value) { myString = value; }

         PascalString();
   };
}

#endif
