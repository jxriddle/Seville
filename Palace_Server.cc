#include "Palace_Server.h"

namespace Seville
{
   namespace Palace
   {
      Server::Server()
      {
         doReset();
      }

      Server::~Server()
      {

      }

      void Server::reset()
      {
         doReset();
      }

      void Server::doReset()
      {
         myRoomList.clear();
         myUserList.clear();
      }

      Host::ByteOrder Server::byteOrder() const
      {
         return myByteOrder;
      }

      void Server::setByteOrder(Host::ByteOrder byteOrder)
      {
         myByteOrder = byteOrder;
      }

      QString Server::host() const
      {
         return myHost;
      }

      void Server::setHost(const QString& host)
      {
         myHost = host;
      }

      quint16 Server::port() const
      {
         return myPort;
      }

      void Server::setPort(quint16 port)
      {
         myPort = port;
      }
   }
}
