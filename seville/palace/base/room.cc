#include "room.h"

namespace seville
{
   namespace palace
   {
//      Room::Room(QObject* parent_object_pointer)
//         : QObject(parent_object_pointer) {}

      auto Room::newInstance(QObject* parent_object_pointer) -> std::optional<Room*>
      {
         auto instance =

         my_parent_object_pointer_ = parent_object_pointer;
         QObject::setParent(my_parent_object_pointer);

         return instance;
      }
   }
}
