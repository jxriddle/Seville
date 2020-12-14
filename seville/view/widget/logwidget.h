#ifndef SEVILLE_VIEW_WIDGET_LOGWIDGET_H_
#define SEVILLE_VIEW_WIDGET_LOGWIDGET_H_

#include <QObject>
#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "seville/palace/base/logmessage.h"
#include "seville/palace/base/client.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         class LogWidget : public QWidget
         {
         Q_OBJECT

         public:
            explicit LogWidget(QWidget* widget_parent_ptr = nullptr);
            //virtual ~LogWidget();

            static auto New(QWidget* widget_parent_ptr = nullptr) -> LogWidget*;

            inline auto palace_client_ptr(void) -> palace::Client* {
               return my_palace_client_ptr_;
            }

            inline auto set_palace_client_ptr(palace::Client* value) -> void {
               if (nullptr != my_palace_client_ptr_)
                  disconnect(
                     my_palace_client_ptr_->logger_ptr(),
                     &seville::palace::Logger::message_was_logged,
                     this,
                     &seville::view::widget::LogWidget::on_message_was_logged
                  );

               my_palace_client_ptr_ = value;

               connect(
                     my_palace_client_ptr_->logger_ptr(),
                     &seville::palace::Logger::message_was_logged,
                     this,
                     &seville::view::widget::LogWidget::on_message_was_logged
               );
            }

            inline auto line_edit_chat_visible(void) -> bool {
               return my_line_edit_ptr_->isVisible();
            }

            inline auto set_line_edit_chat_visible(bool value) -> void {
               my_line_edit_ptr_->setVisible(value);
            }

         public slots:
            void on_message_was_logged(
                  const seville::palace::LogMessage& logMessage);

         private:
            palace::Client* my_palace_client_ptr_;
            QVBoxLayout* my_layout_ptr_;
            QTextEdit* my_text_edit_ptr_;
            QLineEdit* my_line_edit_ptr_;

            auto do_setup_view_(void) -> void;
            auto do_setup_events_(void) -> void;
            auto do_reset_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_VIEW_WIDGET_LOGWIDGET_H_
