#ifndef SEVILLE_PALACE_LOGMESSAGE_H_
#define SEVILLE_PALACE_LOGMESSAGE_H_

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      enum class LogMessageKind: u32
      {
         kGlobalKind,
         kChatKind,
         kWhisperKind,
         kInfoKind,
         kErrorKind,
         kWarningKind,
         kDebugKind,
      };

      class LogMessage
      {
      public:
         LogMessage(LogMessageKind kind = LogMessageKind::kChatKind, const QString& message = "", const QString& usernameFrom = "", const QString& usernameTo = "");

         inline LogMessageKind kind() const { return my_kind; }
         inline void setKind(const LogMessageKind& value) { my_kind = value; }

         inline QString message() const { return my_message; }
         inline void setMessage(const QString& value) { my_message = value; }

         inline QString fromUsername() const { return my_usernameFrom; }
         inline void setFromUsername(const QString& value) { my_usernameFrom = value; }

         inline QString toUsername() const { return my_usernameTo; }
         inline void setToUsername(const QString& value) { my_usernameTo = value; }

      private:
         std::unique_ptr<std::vector<LogMessage>> my_puMessages;
         bool my_isDebugModeFlag;

         LogMessageKind my_kind;
         QString my_message;
         QString my_usernameFrom;
         QString my_usernameTo;

         void do_init(LogMessageKind kind, const QString& message, const QString& usernameFrom, const QString& usernameTo);
         void do_deinit(void);
      };
   }
}

#endif // SEVILLE_PALACE_LOGMESSAGE_H_
