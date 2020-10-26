#include <QVBoxLayout>

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
            do_setup_view_();
            do_setup_events_();
         }

         auto LogWidget::do_setup_view_(void) -> void
         {
            my_layout_ptr_ = new QVBoxLayout(this);

            my_line_edit_ptr_ = new QLineEdit(this);
            my_text_edit_ptr_ = new QTextEdit(this);

            my_layout_ptr_->addWidget(my_text_edit_ptr_);
            my_layout_ptr_->addWidget(my_line_edit_ptr_);
         }

         auto LogWidget::do_setup_events_(void) -> void
         {
         }

         auto LogWidget::palace_client_ptr(void) -> palace::Client*
         {
            return my_client_palace_ptr_;
         }

         auto LogWidget::set_palace_client_ptr(
               palace::Client* palace_client_ptr) -> void
         {
            my_client_palace_ptr_ = palace_client_ptr;
         }

         auto LogWidget::line_edit_chat_visible(void) -> bool
         {
            return my_line_edit_ptr_->isVisible();
         }

         auto LogWidget::set_line_edit_chat_visible(bool value) -> void
         {
            my_line_edit_ptr_->setVisible(value);
         }
      }
   }
}
