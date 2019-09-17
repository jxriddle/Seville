#include "Palace_NetMsg_Logon.h"

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void Logon::doInit()
         {
            setId(NetMsg::NetMsgBase::LogonKind);
            setLen(NetMsg::NetMsgBase::kByteSizeOfLogon);
            setRef(0);
         }

         Logon::~Logon()
         {

         }

         Logon::Logon()
         {

         }

         Logon::Logon(Host::ByteOrder clientByteOrder,
                            Host::ByteOrder serverByteOrder)
         {
            setClientByteOrder(clientByteOrder);
            setServerByteOrder(serverByteOrder);
         }
      }
   }
}
