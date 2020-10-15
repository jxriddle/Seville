#include "logger.h"

namespace seville
{
   namespace palace
   {
      Logger::Logger(void) {}

      // Logger::~Logger(void) {}

      auto Logger::New(void) -> std::unique_ptr<Logger> {
         auto instance = std::make_unique<Logger>();

         return instance;
      }

      auto Logger::do_init_(LoggerMode mode) -> void
      {
         my_messages_unique_ptr_ = std::make_unique<std::vector<LogMessage>>();
         my_mode_ = mode;
      }

      auto Logger::do_log_message_(const LogMessage& message) -> void
      {
         my_messages_unique_ptr_->push_back(message);
         emit message_logged(message);
      }

//      auto Logger::do_log_message_unique_ptr_(
//            std::unique_ptr<LogMessage> message_unique_ptr) -> void
//      {
//         my_messages_unique_ptr_->push_back(message_unique_ptr);
//         emit message_logged(message);
//      }
   }
}
