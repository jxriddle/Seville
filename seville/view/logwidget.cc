#include <QVBoxLayout>

#include "seville/palace/logentry.h"
#include "seville/palace/client.h"
#include "seville/view/logwidget.h"

namespace seville
{
   namespace view
   {
      LogWidget::LogWidget(QWidget* parentWidgetPtr)
         : QWidget(parentWidgetPtr)
      {
         do_setupView();
         do_setupEvents();
         do_clear();
      }

      auto LogWidget::New(QWidget* parentWidgetPtr) -> LogWidget* {
         auto instance = new LogWidget(parentWidgetPtr);

         return instance;
      }

      void LogWidget::on_messageLoggedEvent(
            const seville::palace::LogEntry& logEntry)
      {
         if (logEntry.kind() == seville::palace::LogEntryKind::kChatKind ||
             logEntry.kind() == seville::palace::LogEntryKind::kWhisperKind) {
            my_logTextEditPtr->append(
                     QString("%1: %2")
                     .arg(logEntry.fromUsername())
                     .arg(logEntry.message()));
         //} else if (logMessage.kind() ==
         //           seville::palace::LogMessageKind::kInfoKind) {
         } else {
            my_logTextEditPtr->append(logEntry.message());
         }
      }

      auto LogWidget::do_reloadFromPalaceClient(void) -> void
      {
         do_clear();
         auto loggerPtr = my_palaceClientPtr->loggerPtr();
         auto logEntriesPtr = loggerPtr->logEntriesPtr();
         for (auto& logEntry: *logEntriesPtr) {
            my_logTextEditPtr->append(logEntry.message());
         }
      }

      auto LogWidget::do_clear(void) -> void
      {
//         my_mainLayoutPtr = nullptr;
//         my_chatLineEditPtr = nullptr;
//         my_logTextEditPtr = nullptr;
//         my_palaceClientPtr = nullptr;
         my_logTextEditPtr->clear();
      }

      auto LogWidget::do_setupView(void) -> void
      {
         my_mainLayoutPtr = new QVBoxLayout(this);

         my_chatLineEditPtr = new QLineEdit(this);
         my_logTextEditPtr = new QTextEdit(this);

         my_logTextEditPtr->setReadOnly(true);
         my_logTextEditPtr->setTextInteractionFlags(
                  Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

         my_mainLayoutPtr->addWidget(my_logTextEditPtr);
         my_mainLayoutPtr->addWidget(my_chatLineEditPtr);
      }

      auto LogWidget::do_setupEvents(void) -> void
      {
      }
   }
}
