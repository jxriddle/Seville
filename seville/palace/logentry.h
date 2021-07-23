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
//         static std::unique_ptr<LogEntry> New(
//               LogEntryKind logEntryKind = LogEntryKind::kChatKind,
//               const QString& message = "",
//               const QString& fromUsername = "",
//               const QString& toUsername = ""
//            );

         // LogMessage(void);

         LogEntry(const LogEntry& other);

         LogEntry(
               LogEntryKind logEntryKind = LogEntryKind::kChatKind,
               const QString& message = "",
               const QString& fromUsername = "",
               const QString& toUsername = "");

         inline LogEntryKind kind(void) const {
            return my_logEntryKind;
         }

         inline void setKind(const LogEntryKind& value) {
            my_logEntryKind = value;
         }

         inline QString message(void) const {
            return my_message;
         }

         inline void setMessage(const QString& value) {
            my_message = value;
         }

         inline QString fromUsername(void) const {
            return my_fromUsername;
         }

         inline void setFromUsername(const QString& value) {
            my_fromUsername = value;
         }

         inline QString toUsername(void) const {
            return my_toUsername;
         }

         inline void setToUsername(const QString& value) {
            my_toUsername = value;
         }

      private:
         // std::unique_ptr<std::vector<LogEntry>> my_messagesUniquePtr;
         std::vector<LogEntry> my_messages;
         bool my_debugModeFlag;
         LogEntryKind my_logEntryKind;
         QString my_message;
         QString my_fromUsername;
         QString my_toUsername;

         void do_init(
               LogEntryKind kind,
               const QString& message,
               const QString& fromUsername,
               const QString& toUsername);
         void do_clear(void);
         void do_assign(const LogEntry& other);
      };
   }
}

#endif // SEVILLE_PALACE_LOGMESSAGE_H_
