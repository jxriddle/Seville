#include "logger.h"

namespace seville
{
   namespace palace
   {
      Logger::Logger(bool isDebugMode)
      {
         do_init(isDebugMode);
      }

      Logger::~Logger(void)
      {
         do_deinit();
      }

      void do_init(bool isDebugMode = false)
      {
         my_puMessages = std::make_unique<std::vector<LogMessage>>();
         my_isDebugMode = isDebugMode;
      }

      void do_logMessage(const LogMessage& message)
      {
         my_puMessages->push_back(message);
         emit messageLogged(message);
      }

   }
}
