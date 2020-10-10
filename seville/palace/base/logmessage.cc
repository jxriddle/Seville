#include "seville/palace/base/logmessage.h"

namespace seville
{
   namespace palace
   {
      auto LogMessage::newInstance(
            LogMessageKind log_message_kind,
            const QString& message,
            const QString& username_from,
            const QString& username_to)
      -> std::optional<LogMessage*>
      {
         auto instance = std::make_optional<LogMessage*>();

         if (instance.has_value()) {
            my_kind_log_message_ = log_message_kind;
            my_message_ = message;
            my_username_from_ = username_from;
            my_username_to_ = username_to;
         }

         return instance;
      }
   }
}
