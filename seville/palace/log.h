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

            // static Log* New(void);

            inline LogMode mode(void) const {
            return my_mode;
            }

            inline void setMode(LogMode value) {
            my_mode = value;
            }

            inline std::vector<LogEntry>* logEntriesPtr(void) {
            // return my_logEntriesUniquePtr.get();
            return &my_logEntries;
            }

            //         inline auto takeLogEntriesPtr(void) ->
            //               std::unique_ptr<std::vector<LogEntry>> {
            //            return std::move(my_logEntriesUniquePtr);
            //         }

            inline void appendLogEntry(const LogEntry& message) {
                do_appendLogEntry(message);
            }

            inline void appendLogEntry(
                    LogEntryKind kind,
                    const QString& message,
                    const QString& usernameFrom,
                    const QString& usernameTo) {
                do_appendLogEntry(
                     LogEntry(kind, message, usernameFrom, usernameTo));
            }

            inline void appendWhisperMessage(
                    const QString& usernameFrom,
                    // const QString& usernameTo,
                    const QString& message) {
                do_appendLogEntry(
                            LogEntry(
                                LogEntryKind::kWhisperKind,
                                message,
                                usernameFrom));
                // usernameTo));
            }

            inline void appendChatMessage(
               const QString& usernameFrom,
               const QString& message) {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kChatKind,
                        message,
                        usernameFrom,
                        ""));
            }

            inline void appendGlobalMessage(const QString& message) {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kGlobalKind,
                        message,
                        "",
                        ""));
            }

            inline void appendInfoMessage(const QString& message) {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kInfoKind,
                        message,
                        "",
                        ""));
            }

            inline void appendWarningMessage(const QString& message) {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kWarningKind,
                        message,
                        "",
                        ""));
            }

            inline void appendErrorMessage(const QString& message) {
            do_appendLogEntry(
                     LogEntry(
                        LogEntryKind::kErrorKind,
                        message,
                        "",
                        ""));
            }

            inline void appendDebugMessage(const QString& message) {
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
            // std::unique_ptr<std::vector<LogEntry>> my_logEntriesUniquePtr;
            std::vector<LogEntry> my_logEntries;

            void do_appendLogEntry(const LogEntry& message);
            // auto do_log_message_unique_ptr_(
                //std::unique_ptr<LogMessage> message_unique_ptr) -> void;
            void do_init(LogMode mode = LogMode::kDefaultMode);
      };
    }
}

#endif  // SEVILLE_PALACE_LOG_H_
