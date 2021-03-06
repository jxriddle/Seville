#include "log.h"
#include "logentry.h"

namespace seville
{
   namespace palace
   {
      Log::Log(void)
      {
         do_init();
      }

      // Logger::~Logger(void) {}

      auto Log::New(void) -> Log*
      {
         auto instance = new Log();

         instance->my_logEntriesUniquePtr =
               std::make_unique<std::vector<LogEntry>>();
         instance->my_mode = LogMode::kDefaultMode;

         return instance;
      }

      auto Log::do_init(LogMode mode) -> void
      {
         my_logEntriesUniquePtr = std::make_unique<std::vector<LogEntry>>();
         my_mode = mode;
      }

      auto Log::do_appendLogEntry(const LogEntry& logEntry) -> void
      {
         my_logEntriesUniquePtr->push_back(logEntry);
         emit logEntryWasAppendedEvent(logEntry);
      }

//      auto LoggerObject::do_log_message_unique_ptr_(
//            std::unique_ptr<LogMessageObject> message_unique_ptr) -> void
//      {
//         my_messages_unique_ptr_->push_back(message_unique_ptr);
//         emit message_logged(message);
//      }
   }
}
