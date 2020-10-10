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
         LogMessage(void) = delete;

         auto newInstance(
               LogMessageKind log_message_kind = LogMessageKind::kChatKind,
               const QString& message = "",
               const QString& username_from = "",
               const QString& username_to = "") -> std::optional<LogMessage*>;

         inline auto kind(void) const -> LogMessageKind {
            return my_kind_log_message_;
         }

         inline auto setKind(const LogMessageKind& value) -> void {
            my_kind_log_message_ = value;
         }

         inline auto message(void) const -> QString {
            return my_message_;
         }

         inline auto setMessage(const QString& value) -> void {
            my_message_ = value;
         }

         inline auto fromUsername(void) const -> QString {
            return my_username_from_;
         }

         inline auto setFromUsername(const QString& value) -> void {
            my_username_from_ = value;
         }

         inline auto toUsername(void) const -> QString {
            return my_username_to_;
         }

         inline auto setToUsername(const QString& value) -> void {
            my_username_to_ = value;
         }

      private:
         std::unique_ptr<std::vector<LogMessage>> my_messages_unique_pointer_;
         bool my_flag_is_debug_mode_;
         LogMessageKind my_kind_log_message_;
         QString my_message_;
         QString my_username_from_;
         QString my_username_to_;

         void do_init(LogMessageKind kind, const QString& message, const QString& usernameFrom, const QString& usernameTo);
      };
   }
}

#endif // SEVILLE_PALACE_LOGMESSAGE_H_
