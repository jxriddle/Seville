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

      //LogMessage::LogMessage(void)
      //{
      //   do_reset_();
      //}

      LogMessage::LogMessage(const LogMessage& other)
      {
         do_assign_(other);
      }

      LogMessage::LogMessage(
            LogMessageKind kind,
            const QString& message,
            const QString& username_from,
            const QString& username_to)
      {
         my_kind_log_message_ = kind;
         my_message_ = message;
         my_username_from_ = username_from;
         my_username_to_ = username_to;
      }

      auto LogMessage::do_reset_(void) -> void
      {
         my_kind_log_message_ = LogMessageKind::kChatKind;
         my_message_ = "";
         my_username_from_ = "";
         my_username_to_ = "";
      }

      auto LogMessage::do_init_(
            LogMessageKind kind,
            const QString& message,
            const QString& username_from,
            const QString& username_to) -> void
      {
         my_kind_log_message_ = kind;
         my_message_ = message;
         my_username_from_ = username_from;
         my_username_to_ = username_to;
      }

      auto LogMessage::do_assign_(const LogMessage& other) -> void
      {
         my_kind_log_message_ = other.my_kind_log_message_;
         my_message_ = other.my_message_;
         my_username_from_ = other.my_username_from_;
         my_username_to_ = other.my_username_to_;
      }
   }
}
