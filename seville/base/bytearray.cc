#include <optional>

#include "seville/base/types.h"
#include "seville/base/bytearray.h"

namespace seville
{
   ByteArray::ByteArray(void)
   {
      do_clear();
   }

   std::unique_ptr<ByteArray> ByteArray::New(void)
   {
      auto instance = std::make_unique<ByteArray>();


      return instance;
   }
}
