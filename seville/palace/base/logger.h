#ifndef SEVILLE_PALACE_LOGGER_H_
#define SEVILLE_PALACE_LOGGER_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <QObject>
#include <QString>
#include <QWidget>

#include "seville/base/types.h"
#include "seville/palace/base/logmessage.h"

namespace seville
{
   namespace palace
   {
      class Logger : public QObject
      {
         Q_OBJECT

      public:
         enum LoggerMode: i32 {
            kNoMode = 0,
            kDefaultMode = 1,
            kProductionMode = 2,
            kDebugMode = 5,
         };

         //Logger(bool isDebugMode = false);
         Logger(void) = delete;
         virtual ~Logger(void);

         static auto New(void) -> std::unique_ptr<Logger>;

         inline auto mode(void) const -> LoggerMode {
            return my_logger_mode_;
         }

         inline auto setMode(LoggerMode value) -> void {
            my_logger_mode_ = value;
         }

         inline auto messagesPtr(void) -> std::vector<LogMessage>* {
            return my_messages_unique_ptr_.get();
         }

         inline auto takeMessagesPtr(void) -> \
               std::unique_ptr<std::vector<LogMessage>> {
            return std::move(my_messages_unique_ptr_);
         }

         inline auto log(const LogMessage& message) -> void {
            logMessage(message);
         }

         inline auto log(LogMessage::Kind kind, const QString& message, const QString& username_from, const QString& username_to) -> void {
            logMessage(LogMessage(kind, message, username_from, username_to));
         }

         inline auto whisper(const QString& username_from, const QString& username_to, const QString& message) -> void {
            logMessage(LogMessage(LogMessage::Kind::kWhisperKind, message, username_from, username_to));
         }

         inline auto chat(const QString& username_from, const QString& message) -> void {
            logMessage(LogMessage(LogMessage::Kind::kChatKind, message, username_from, ""));
         }

         inline auto global(const QString& message) -> void {
            logMessage(LogMessage(LogMessage::Kind::kGlobalKind, message, "", ""));
         }

         inline auto info(const QString& message) -> void {
            logMessage(LogMessage(LogMessage::Kind::kInfoKind, message, "", ""));
         }

         inline auto warning(const QString& message) -> void {
            logMessage(LogMessage(LogMessage::Kind::kWarningKind, message, "", ""));
         }

         inline auto error(const QString& message) -> void {
            messageLogged(LogMessage(LogMessage::Kind::kErrorKind, message, "", ""));
         }

         inline auto debug(const QString& message) -> void {
            if (LoggerMode::kDebugMode == my_logger_mode_)
               messageLogged(LogMessage(LogMessage::Kind::kDebugKind, message, "", ""));
         }

      signals:
         // should be a generic messageLogged method with an enum type parameter instead? eh.
         void messageLogged(const LogMessage& log);

      private:
         LoggerMode my_logger_mode_;
         std::unique_ptr<std::vector<LogMessage>> my_messages_unique_ptr_;
      };
   }
}

#endif  // SEVILLE_PALACE_LOGGER_H_
