#include "room.h"

namespace seville
{
   namespace palace
   {
      Room::Room(QObject* object_parent_ptr)
         : QObject(object_parent_ptr)
      {
         do_init_();
         do_reset_();
      }

      Room::Room(const Room& room, QObject* object_parent_ptr)
         : QObject (object_parent_ptr)
      {
         do_init_();
         do_reset_();
         do_assign_(room);
      }
   }
}
