#include "server.h"

namespace seville
{
   namespace palace
   {
      Server::~Server(void)
      {
         do_deinit();
      }

      Server::Server(void)
         : QObject()
      {
         do_init();
         do_clear();
      }
   }
}
