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
            do_setupSizing();
            do_clear();
        }

        //      LogWidget* LogWidget::New(QWidget* parentWidgetPtr)
        //      {
        //         auto instance = new LogWidget(parentWidgetPtr);

        //         return instance;
        //      }

        void LogWidget::on_messageLoggedEvent(
            const seville::palace::LogEntry& logEntry)
        {
            if (logEntry.kind() == seville::palace::LogEntryKind::kChatKind) {
                my_logTextEditPtr->append(
                         QString("%1: %2")
                         .arg(logEntry.fromUsername())
                         .arg(logEntry.message()));
                //} else if (logMessage.kind() ==
                //           seville::palace::LogMessageKind::kInfoKind) {
            } else if (logEntry.kind() == seville::palace::LogEntryKind::kWhisperKind) {
                my_logTextEditPtr->append(
                         QString("Whisper from %1: %2")
                         .arg(logEntry.fromUsername())
                         .arg(logEntry.message()));
            } else {
                my_logTextEditPtr->append(logEntry.message());
                         //QString("(whisper) %1").arg(logEntry.message()));
            }
        }

        void LogWidget::do_refresh(void)
        {
            do_clear();

            auto palaceClientPtr = my_palaceClientPtr;
            if (palaceClientPtr == nullptr) {
                return;
            }

            auto loggerPtr = palaceClientPtr->loggerPtr();
            if (loggerPtr == nullptr) {
                return;
            }

            auto logEntriesPtr = loggerPtr->logEntriesPtr();
            if (logEntriesPtr == nullptr) {
                return;
            }

            for (auto& logEntry: *logEntriesPtr) {
                my_logTextEditPtr->append(logEntry.message());
            }
        }

        void LogWidget::do_clear(void)
        {
            //         my_mainLayoutPtr = nullptr;
            //         my_chatLineEditPtr = nullptr;
            //         my_logTextEditPtr = nullptr;
            //         my_palaceClientPtr = nullptr;
            my_logTextEditPtr->clear();
        }

        void LogWidget::do_setupView(void)
        {
            my_mainLayoutPtr = new QVBoxLayout(this);

            my_chatLineEditPtr = new QLineEdit(this);
            my_logTextEditPtr = new QTextEdit(this);

            my_logTextEditPtr->setReadOnly(true);
            // my_logTextEditPtr->setTextInteractionFlags(
            //          Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

            my_mainLayoutPtr->addWidget(my_logTextEditPtr);
            my_mainLayoutPtr->addWidget(my_chatLineEditPtr);
        }

        void LogWidget::do_setupEvents(void)
        {
        }

        void LogWidget::do_setupSizing(void)
        {
            setMinimumWidth(300);
        }
    }
}
