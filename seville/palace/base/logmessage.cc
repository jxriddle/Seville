#include "seville/palace/base/logmessage.h"

namespace seville
{
   namespace palace
   {
      auto LogMessage::New( \
            LogMessageKind log_message_kind, \
            const QString& message, \
            const QString& username_from, \
            const QString& username_to \
         ) -> std::unique_ptr<LogMessage>
         //-> std::optional<LogMessage*>
      {
         //auto instance = std::make_optional<LogMessage*>();
         auto instance_unique_ptr = std::make_unique<LogMessage>();

         //if (instance_unique_ptr.has_value()) {
         if (nullptr != instance_unique_ptr.get()) {
            instance_unique_ptr->my_kind_log_message_ = log_message_kind;
            instance_unique_ptr->my_message_ = message;
            instance_unique_ptr->my_username_from_ = username_from;
            instance_unique_ptr->my_username_to_ = username_to;
         }

         return instance_unique_ptr;
      }
   }
}
