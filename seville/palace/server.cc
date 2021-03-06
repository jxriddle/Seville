#include "server.h"

namespace seville
{
   namespace palace
   {
      Server::~Server()
      {
         do_deinit();
      }

      Server::Server()
         : QObject()
      {
         do_init();
      }
   }
}
