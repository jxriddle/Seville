#include <QVBoxLayout>

#include "seville/palace/base/logmessage.h"
#include "seville/palace/base/client.h"
#include "seville/view/widget/logwidget.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         LogWidget::LogWidget(QWidget* widget_parent_ptr)
            : QWidget(widget_parent_ptr)
         {
            do_reset_();
            do_setup_view_();
            do_setup_events_();
         }

         auto LogWidget::New(QWidget* widget_parent_ptr) -> LogWidget* {
            auto instance = new LogWidget(widget_parent_ptr);

            return instance;
         }

         void LogWidget::on_message_was_logged(
               const seville::palace::LogMessage& logMessage)
         {
            if (logMessage.kind() ==
                seville::palace::LogMessageKind::kChatKind ||
                logMessage.kind() ==
                seville::palace::LogMessageKind::kWhisperKind) {
               my_text_edit_ptr_->append(
                        QString("%1: %2")
                        .arg(logMessage.username_from())
                        .arg(logMessage.message()));
            //} else if (logMessage.kind() ==
            //           seville::palace::LogMessageKind::kInfoKind) {
            } else {
               my_text_edit_ptr_->append(logMessage.message());
            }
         }

         auto LogWidget::do_reset_(void) -> void
         {
            my_layout_ptr_ = nullptr;
            my_line_edit_ptr_ = nullptr;
            my_text_edit_ptr_ = nullptr;
            my_palace_client_ptr_ = nullptr;
         }

         auto LogWidget::do_setup_view_(void) -> void
         {
            my_layout_ptr_ = new QVBoxLayout(this);

            my_line_edit_ptr_ = new QLineEdit(this);
            my_text_edit_ptr_ = new QTextEdit(this);

            my_text_edit_ptr_->setReadOnly(true);
            my_text_edit_ptr_->setTextInteractionFlags(
                     Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

            my_layout_ptr_->addWidget(my_text_edit_ptr_);
            my_layout_ptr_->addWidget(my_line_edit_ptr_);
         }

         auto LogWidget::do_setup_events_(void) -> void
         {
         }
      }
   }
}
