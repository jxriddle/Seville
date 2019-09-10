#include "pal_server.h"

namespace Seville
{
   PalServer::PalServer()
   {
      doResetState_();
   }

   PalServer::~PalServer()
   {

   }

   void PalServer::resetState()
   {
      doResetState_();
   }

   void PalServer::doResetState_()
   {
      roomList_.clear();
      userList_.clear();
   }

   PalCommon::ByteOrder PalServer::byteOrder() const
   {
      return byteOrder_;
   }

   void PalServer::setByteOrder(PalCommon::ByteOrder byteOrder)
   {
      byteOrder_ = byteOrder;
   }

   QString PalServer::host() const
   {
      return host_;
   }

   void PalServer::setHost(const QString& host)
   {
      host_ = host;
   }

   quint16 PalServer::port() const
   {
      return port_;
   }

   void PalServer::setPort(quint16 port)
   {
      port_ = port;
   }
}
