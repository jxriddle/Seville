#include "logger.h"

namespace seville
{
   namespace palace
   {
      Logger::Logger(void)
      {
         do_init_();
      }

      // Logger::~Logger(void) {}

      auto Logger::New(void) -> Logger*
      {
         auto instance = new Logger();

         instance->my_messages_unique_ptr_ =
               std::make_unique<std::vector<LogMessage>>();
         instance->my_mode_ = LoggerMode::kDefaultMode;

         return instance;
      }

      auto Logger::do_init_(LoggerMode mode) -> void
      {
         my_messages_unique_ptr_ = std::make_unique<std::vector<LogMessage>>();
         my_mode_ = mode;
      }

      auto Logger::do_log_message_(const LogMessage& logMessage) -> void
      {
         my_messages_unique_ptr_->push_back(logMessage);
         emit message_was_logged(logMessage);
      }

//      auto Logger::do_log_message_unique_ptr_(
//            std::unique_ptr<LogMessage> message_unique_ptr) -> void
//      {
//         my_messages_unique_ptr_->push_back(message_unique_ptr);
//         emit message_logged(message);
//      }
   }
}
