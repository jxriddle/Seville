#include "seville/palace/base/logmessage.h"

namespace seville
{
   namespace palace
   {
      LogMessage::LogMessage(LogMessageKind kind, const QString& message, const QString& usernameFrom, const QString& usernameTo)
      {
         do_init(kind, message, usernameFrom, usernameTo);
      }

      void LogMessage::do_init(LogMessageKind kind, const QString& message, const QString& usernameFrom, const QString& usernameTo)
      {
         my_kind = kind;
         my_message = message;
         my_usernameFrom = usernameFrom;
         my_usernameTo = usernameTo;
      }
   }
}
