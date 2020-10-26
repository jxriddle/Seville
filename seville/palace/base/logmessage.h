#ifndef SEVILLE_PALACE_LOGMESSAGE_H_
#define SEVILLE_PALACE_LOGMESSAGE_H_

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      enum class LogMessageKind: u32
      {
         kGlobalKind,
         kChatKind,
         kWhisperKind,
         kInfoKind,
         kErrorKind,
         kWarningKind,
         kDebugKind,
      };

      class LogMessage
      {
      public:
         static auto New(
               LogMessageKind log_message_kind = LogMessageKind::kChatKind,
               const QString& message = "",
               const QString& username_from = "",
               const QString& username_to = ""
            ) -> std::unique_ptr<LogMessage>;

         //LogMessage(void);

         LogMessage(const LogMessage& other);

         LogMessage(
               LogMessageKind log_message_kind = LogMessageKind::kChatKind,
               const QString& message = "",
               const QString& username_from = "",
               const QString& username_to = "");

         inline auto kind(void) const -> LogMessageKind {
            return my_kind_log_message_;
         }

         inline auto set_kind(const LogMessageKind& value) -> void {
            my_kind_log_message_ = value;
         }

         inline auto message(void) const -> QString {
            return my_message_;
         }

         inline auto set_message(const QString& value) -> void {
            my_message_ = value;
         }

         inline auto username_from(void) const -> QString {
            return my_username_from_;
         }

         inline auto set_username_from(const QString& value) -> void {
            my_username_from_ = value;
         }

         inline auto username_to(void) const -> QString {
            return my_username_to_;
         }

         inline auto set_username_to(const QString& value) -> void {
            my_username_to_ = value;
         }

      private:
         std::unique_ptr<std::vector<LogMessage>> my_messages_unique_ptr_;
         bool my_flag_is_debug_mode_;
         LogMessageKind my_kind_log_message_;
         QString my_message_;
         QString my_username_from_;
         QString my_username_to_;

         auto do_init_(
               LogMessageKind kind,
               const QString& message,
               const QString& username_from,
               const QString& username_to) -> void;

         auto do_reset_(void) -> void;

         auto do_assign_(const LogMessage& other) -> void;
      };
   }
}

#endif // SEVILLE_PALACE_LOGMESSAGE_H_
