#ifndef SEVILLE_PALACE_LOG_ENTRY_H_
#define SEVILLE_PALACE_LOG_ENTRY_H_

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      enum LogEntryKind: u32
      {
         kGlobalKind,
         kChatKind,
         kWhisperKind,
         kInfoKind,
         kErrorKind,
         kWarningKind,
         kDebugKind,
      };

      class LogEntry
      {
      public:
         static auto New(
               LogEntryKind logEntryKind = LogEntryKind::kChatKind,
               const QString& message = "",
               const QString& fromUsername = "",
               const QString& toUsername = ""
            ) -> std::unique_ptr<LogEntry>;

         //LogMessage(void);

         LogEntry(const LogEntry& other);

         LogEntry(
               LogEntryKind logEntryKind = LogEntryKind::kChatKind,
               const QString& message = "",
               const QString& fromUsername = "",
               const QString& toUsername = "");

         inline auto kind(void) const -> LogEntryKind {
            return my_logEntryKind;
         }

         inline auto setKind(const LogEntryKind& value) -> void {
            my_logEntryKind = value;
         }

         inline auto message(void) const -> QString {
            return my_message;
         }

         inline auto setMessage(const QString& value) -> void {
            my_message = value;
         }

         inline auto fromUsername(void) const -> QString {
            return my_fromUsername;
         }

         inline auto setFromUsername(const QString& value) -> void {
            my_fromUsername = value;
         }

         inline auto toUsername(void) const -> QString {
            return my_toUsername;
         }

         inline auto setToUsername(const QString& value) -> void {
            my_toUsername = value;
         }

      private:
         std::unique_ptr<std::vector<LogEntry>> my_messagesUniquePtr;
         bool my_debugModeFlag;
         LogEntryKind my_logEntryKind;
         QString my_message;
         QString my_fromUsername;
         QString my_toUsername;

         auto do_init(
               LogEntryKind kind,
               const QString& message,
               const QString& fromUsername,
               const QString& toUsername) -> void;
         auto do_clear(void) -> void;
         auto do_assign(const LogEntry& other) -> void;
      };
   }
}

#endif // SEVILLE_PALACE_LOGMESSAGE_H_
