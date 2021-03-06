#ifndef SEVILLE_VIEW_LOG_WIDGET_H_
#define SEVILLE_VIEW_LOG_WIDGET_H_

#include <QObject>
#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "seville/palace/logentry.h"
#include "seville/palace/client.h"

namespace seville
{
   namespace view
   {
      class LogWidget : public QWidget
      {
      Q_OBJECT

      public:
         explicit LogWidget(QWidget* parentWidgetPtr = nullptr);
         //virtual ~LogWidget();

         static auto New(QWidget* parentWidgetPtr = nullptr) -> LogWidget*;

         inline auto palaceClientPtr(void) -> palace::Client* {
            return my_palaceClientPtr;
         }

         inline auto setPalaceClientPtr(palace::Client* value) -> void {
            auto previousPalaceClientPtr = my_palaceClientPtr;
            my_palaceClientPtr = value;

            if (nullptr != previousPalaceClientPtr) {
               disconnect(
                  my_palaceClientPtr->loggerPtr(),
                  &seville::palace::Log::logEntryWasAppendedEvent,
                  this, &seville::view::LogWidget::on_messageLoggedEvent
               );
            }

            if (nullptr != my_palaceClientPtr) {
               do_reloadFromPalaceClient();

               connect(
                     my_palaceClientPtr->loggerPtr(),
                     &seville::palace::Log::logEntryWasAppendedEvent,
                     this, &seville::view::LogWidget::on_messageLoggedEvent
               );
            }
         }

         inline auto logTextEditPtr(void) -> QTextEdit* {
            return my_logTextEditPtr;
         }

         inline auto chatLineEditPtr(void) -> QLineEdit* {
            return my_chatLineEditPtr;
         }

         inline auto chatLineEditIsVisible(void) -> bool {
            return my_chatLineEditPtr->isVisible();
         }

         inline auto setChatLineEditIsVisible(bool value) -> void {
            my_chatLineEditPtr->setVisible(value);
         }

      public slots:
         void on_messageLoggedEvent(
               const seville::palace::LogEntry& log_message);

      private:
         palace::Client* my_palaceClientPtr;
         QVBoxLayout* my_mainLayoutPtr;
         QTextEdit* my_logTextEditPtr;
         QLineEdit* my_chatLineEditPtr;

         auto do_setupView(void) -> void;
         auto do_setupEvents(void) -> void;
         auto do_clear(void) -> void;
         auto do_reloadFromPalaceClient(void) -> void;
      };
   }
}

#endif // SEVILLE_VIEW_LOG_WIDGET_H_
