#ifndef SEVILLE_PALACE_LOG_H_
#define SEVILLE_PALACE_LOG_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <QObject>
#include <QString>
#include <QWidget>

#include "seville/base/sevilleapp.h"
#include "seville/base/types.h"
#include "seville/palace/logentry.h"

namespace seville
{
   namespace palace
   {
      enum LogMode: i32 {
         kDebugMode = 0,
         kDefaultMode = 1,
         kProductionMode = 2,
      };

      class Log : public QObject
      {
      Q_OBJECT

      public:
         //Logger(bool isDebugMode = false);
         Log(void);

         //virtual ~Logger(void);

         static auto New(void) -> Log*;

         inline auto mode(void) const -> LogMode {
            return my_mode;
         }

         inline auto setMode(LogMode value) -> void {
            my_mode = value;
         }

         inline auto logEntriesPtr(void) -> std::vector<LogEntry>* {
            return my_logEntriesUniquePtr.get();
         }

//         inline auto takeLogEntriesPtr(void) ->
//               std::unique_ptr<std::vector<LogEntry>> {
//            return std::move(my_logEntriesUniquePtr);
//         }

         inline auto appendLogEntry(const LogEntry& message) -> void {
            do_appendLogEntry(message);
         }

         inline auto appendLogEntry(
               LogEntryKind kind,
               const QString& message,
               const QString& usernameFrom,
               const QString& usernameTo
               ) -> void {
            do_appendLogEntry(
                     LogEntry(kind, message, usernameFrom, usernameTo));
         }

         inline auto appendWhisperMessage(
               const QString& usernameFrom,
               // const QString& usernameTo,
               const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kWhisperKind,
                        message,
                        usernameFrom));
                        // usernameTo));
         }

         inline auto appendChatMessage(
               const QString& usernameFrom,
               const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kChatKind,
                        message,
                        usernameFrom,
                        ""));
         }

         inline auto appendGlobalMessage(const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kGlobalKind,
                        message,
                        "",
                        ""));
         }

         inline auto appendInfoMessage(const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kInfoKind,
                        message,
                        "",
                        ""));
         }

         inline auto appendWarningMessage(const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kWarningKind,
                        message,
                        "",
                        ""));
         }

         inline auto appendErrorMessage(const QString& message) -> void {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kErrorKind,
                        message,
                        "",
                        ""));
         }

         inline auto appendDebugMessage(const QString& message) -> void {
            qCDebug(seville_log) << message;
            if (LogMode::kDebugMode == my_mode)
               do_appendLogEntry(
                        LogEntry(
                           LogEntryKind::kDebugKind,
                           message,
                           "",
                           ""));
         }

      signals:
         // should be a generic messageLogged method with an enum type parameter instead? eh.
         void logEntryWasAppendedEvent(const LogEntry& logEntry);

      private:
         LogMode my_mode;
         std::unique_ptr<std::vector<LogEntry>> my_logEntriesUniquePtr;

         auto do_appendLogEntry(const LogEntry& message) -> void;
         // auto do_log_message_unique_ptr_(
                //std::unique_ptr<LogMessage> message_unique_ptr) -> void;
         auto do_init(LogMode mode = LogMode::kDefaultMode) -> void;
      };
   }
}

#endif  // SEVILLE_PALACE_LOG_H_
