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
            kDebugMode = 0,
            kDefaultMode = 1,
            kProductionMode = 2,
         };

         //Logger(bool isDebugMode = false);
         Logger(void);

         //virtual ~Logger(void);

         static auto New(void) -> std::unique_ptr<Logger>;

         inline auto mode(void) const -> LoggerMode {
            return my_mode_;
         }

         inline auto set_mode(LoggerMode value) -> void {
            my_mode_ = value;
         }

         inline auto messages_ptr(void) -> std::vector<LogMessage>* {
            return my_messages_unique_ptr_.get();
         }

         inline auto take_messages_ptr(void) ->
               std::unique_ptr<std::vector<LogMessage>> {
            return std::move(my_messages_unique_ptr_);
         }

         inline auto log(const LogMessage& message) -> void {
            do_log_message_(message);
         }

         inline auto log(
               LogMessageKind kind,
               const QString& message,
               const QString& username_from,
               const QString& username_to
               ) -> void {
            do_log_message_(
                     LogMessage(kind, message, username_from, username_to));
         }

         inline auto whisper(const QString& username_from, const QString& username_to, const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kWhisperKind,
                        message,
                        username_from,
                        username_to));
         }

         inline auto chat(const QString& username_from, const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kChatKind,
                        message,
                        username_from,
                        ""));
         }

         inline auto global(const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kGlobalKind,
                        message,
                        "",
                        ""));
         }

         inline auto info(const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kInfoKind,
                        message,
                        "",
                        ""));
         }

         inline auto warning(const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kWarningKind,
                        message,
                        "",
                        ""));
         }

         inline auto error(const QString& message) -> void {
            do_log_message_(
                     LogMessage(
                        LogMessageKind::kErrorKind,
                        message,
                        "",
                        ""));
         }

         inline auto debug(const QString& message) -> void {
            if (LoggerMode::kDebugMode == my_mode_)
               do_log_message_(
                        LogMessage(
                           LogMessageKind::kDebugKind,
                           message,
                           "",
                           ""));
         }

      signals:
         // should be a generic messageLogged method with an enum type parameter instead? eh.
         void message_logged(const LogMessage& log);

      private:
         LoggerMode my_mode_;
         std::unique_ptr<std::vector<LogMessage>> my_messages_unique_ptr_;

         auto do_log_message_(const LogMessage& message) -> void;
         // auto do_log_message_unique_ptr_(
                //std::unique_ptr<LogMessage> message_unique_ptr) -> void;
         auto do_init_(LoggerMode mode = LoggerMode::kDefaultMode) -> void;
      };
   }
}

#endif  // SEVILLE_PALACE_LOGGER_H_
