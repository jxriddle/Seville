#include <QImage>
#include <QObject>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "seville/base/log.h"
#include "seville/palace/base/host.h"
#include "seville/palace/base/client.h"
#include "seville/palace/base/server.h"
#include "seville/palace/base/room.h"
#include "seville/palace/base/user.h"

#include "seville/palace/base/genericnetmsg.h"

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

      Client::Client(QObject* parent_object_pointer) \
         : QObject(parent_object_pointer)
      {
         do_init();
      }
   }
}
