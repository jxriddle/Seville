#ifndef PALACE_LOGGER_H
#define PALACE_LOGGER_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <QObject>
#include <QString>
#include <QWidget>

#include "seville/base/types.h"
#include "seville/palace/base/logmessage.h"

namespace seville
{
   namespace palace
   {
      class Logger : public QObject
      {
      Q_OBJECT

      public:
         std::vector<LogMessage>* messagesP() { return my_puMessages.get(); }

         void log(const LogMessage& logMessage)
         {
            do_logMessage(logMessage);
         }

         void log(LogMessage::Kind kind, const QString& message, const QString& usernameFrom, const QString& usernameTo)
         {
            do_logMessage(LogMessage(kind, message, usernameFrom, usernameTo));
         }

         void whisper(const QString& usernameFrom, const QString& usernameTo, const QString& message)
         {
            do_logMessage(LogMessage(LogMessage::Kind::WhisperKind, message, usernameFrom, usernameTo));
         }

         void chat(const QString& usernameFrom, const QString& message)
         {
            do_logMessage(LogMessage(LogMessage::Kind::ChatKind, message, usernameFrom, ""));
         }

         void global(const QString& message)
         {
            do_logMessage(LogMessage(LogMessage::Kind::GlobalKind, message, "", ""));
         }

         void info(const QString& message)
         {
            do_logMessage(LogMessage(LogMessage::Kind::InfoKind, message, "", ""));
         }

         void warning(const QString& message)
         {
            do_logMessage(LogMessage(LogMessage::Kind::WarningKind, message, "", ""));
         }

         void error(const QString& message)
         {
            messageLogged(LogMessage(LogMessage::Kind::ErrorKind, message, "", ""));
         }

         void debug(const QString& message)
         {
            if (my_isDebugMode)
               messageLogged(LogMessage(LogMessage::Kind::DebugKind, message, "", ""));
         }

         void setIsDebugMode(const bool& isDebugMode)
         {
            my_isDebugMode = isDebugMode;
         }

         bool isDebugMode(void)
         {
            return my_isDebugMode;
         }

         virtual ~Logger(void);
         Logger(bool isDebugMode = false);

      signals:
         // should be a generic messageLogged method with an enum type parameter instead? eh.
         void messageLogged(const LogMessage& log);
      };
   }
}

#endif // PALACE_LOGGER_H
