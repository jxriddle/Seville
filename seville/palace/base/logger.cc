#include "logger.h"

namespace seville
{
   namespace palace
   {
      Logger::~Logger(void) {}

      auto Logger::New(void) -> std::unique_ptr<Logger> {
         auto instance = std::make_unique<Logger>();

         return instance;
      }

      void do_init(bool isDebugMode = false)
      {
         my_messages_unique_ptr_ = std::make_unique<std::vector<LogMessage>>();
         my_flag_debug_mode_ = isDebugMode;
         auto logger_mode = logger.Mode();
         logger.Mode(kProductionMode);
         logger.MessagesUniquePtr();
         logger.MessagesPointer(messages_unique_ptr);
      }

      void do_logMessage(const LogMessage& message)
      {
         my_messages_unique_ptr_->push_back(message);
         emit messageLogged(message);
      }
   }
}
