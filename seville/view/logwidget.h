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

         static LogWidget* New(QWidget* parentWidgetPtr = nullptr);

         inline palace::Client* palaceClientPtr(void) {
            return my_palaceClientPtr;
         }

         inline void setPalaceClientPtr(palace::Client* value) {
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

         inline QTextEdit* logTextEditPtr(void) {
            return my_logTextEditPtr;
         }

         inline QLineEdit* chatLineEditPtr(void) {
            return my_chatLineEditPtr;
         }

         inline bool chatLineEditIsVisible(void) {
            return my_chatLineEditPtr->isVisible();
         }

         inline void setChatLineEditIsVisible(bool value) {
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

         void do_setupView(void);
         void do_setupEvents(void);
         void do_setupSizing(void);
         void do_clear(void);
         void do_reloadFromPalaceClient(void);
      };
   }
}

#endif // SEVILLE_VIEW_LOG_WIDGET_H_
