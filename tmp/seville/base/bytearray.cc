#include <optional>

#include "seville/base/types.h"
#include "seville/base/bytearray.h"

namespace seville
{
   ByteArray::ByteArray(void) {}

   auto ByteArray::New(void) -> std::unique_ptr<ByteArray>
   {
      auto instance = std::make_unique<ByteArray>();


      return instance;
   }
}
