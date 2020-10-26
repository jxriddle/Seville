#include "server.h"

namespace seville
{
   namespace palace
   {
      Server::~Server()
      {
         do_deinit_();
      }

      Server::Server()
         : QObject()
      {
         do_init_();
      }
   }
}
