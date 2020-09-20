#include "room.h"

namespace seville
{
   namespace palace
   {
      Room::~Room(void)
      {
         do_deinit();
      }

      Room::Room(QObject* pParent)
         : QObject(pParent)
      {
         do_init();
      }
   }
}
