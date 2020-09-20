#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "seville/base/log.h"
#include "host.h"
#include "client.h"
#include "server.h"
#include "room.h"
#include "user.h"

#include "seville/palace/netmsg/generic.h"
#include "seville/palace/netmsg/logon.h"
#include "seville/palace/netmsg/roomdescription.h"
#include "seville/palace/netmsg/httpserverlocation.h"

namespace seville
{
   namespace palace
   {
      const char* Client::kIdent = "PC4237";

      Client::~Client(void)
      {
         do_deinit();
      }

      Client::Client(QObject* pParent)
         : QObject(pParent)
      {
         do_init();
      }
   }
}
