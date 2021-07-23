#include "seville/palace/logentry.h"

namespace seville
{
   namespace palace
   {
//      std::unique_ptr<LogEntry> LogEntry::New(
//            LogEntryKind logEntryKind,
//            const QString& message,
//            const QString& fromUsername,
//            const QString& toUsername
//         )
//         //-> std::optional<LogMessage*>
//      {
//         //auto instance = std::make_optional<LogMessage*>();
//         auto instanceUniquePtr = std::make_unique<LogEntry>();

//         //if (instance_unique_ptr.has_value()) {
//         if (nullptr != instanceUniquePtr.get()) {
//            instanceUniquePtr->my_logEntryKind = logEntryKind;
//            instanceUniquePtr->my_message = message;
//            instanceUniquePtr->my_fromUsername = fromUsername;
//            instanceUniquePtr->my_toUsername = toUsername;
//         }

//         return instanceUniquePtr;
//      }

      // LogMessage::LogMessage(void)
      // {
      //    do_reset_();
      // }

      LogEntry::LogEntry(const LogEntry& other)
      {
         do_assign(other);
      }

      LogEntry::LogEntry(
            LogEntryKind kind,
            const QString& message,
            const QString& fromUsername,
            const QString& toUsername)
      {
         my_logEntryKind = kind;
         my_message = message;
         my_fromUsername = fromUsername;
         my_toUsername = toUsername;
      }

      void LogEntry::do_clear(void)
      {
         my_logEntryKind = LogEntryKind::kChatKind;
         my_message = "";
         my_fromUsername = "";
         my_toUsername = "";
      }

      void LogEntry::do_init(
            LogEntryKind kind,
            const QString& message,
            const QString& fromUsername,
            const QString& toUsername)
      {
         my_logEntryKind = kind;
         my_message = message;
         my_fromUsername = fromUsername;
         my_toUsername = toUsername;
      }

      void LogEntry::do_assign(const LogEntry& other)
      {
         my_logEntryKind = other.my_logEntryKind;
         my_message = other.my_message;
         my_fromUsername = other.my_fromUsername;
         my_toUsername = other.my_toUsername;
      }
   }
}
