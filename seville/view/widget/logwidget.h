#ifndef SEVILLE_VIEW_WIDGET_LOGWIDGET_H_
#define SEVILLE_VIEW_WIDGET_LOGWIDGET_H_

#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "seville/palace/base/client.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         class LogWidget : public QWidget
         {
         public:
            static auto New(QWidget* widget_parent_ptr = nullptr) {
               auto instance = std::make_unique<LogWidget>(widget_parent_ptr);
               return instance;
            }

            explicit LogWidget(QWidget* widget_parent_ptr = nullptr);
            //virtual ~LogWidget();

            auto palace_client_ptr(void) -> palace::Client*;
            auto set_palace_client_ptr(palace::Client* value) -> void;

            auto line_edit_chat_visible(void) -> bool;
            auto set_line_edit_chat_visible(bool value) -> void;

         private:
            palace::Client* my_client_palace_ptr_;
            QVBoxLayout* my_layout_ptr_;
            QTextEdit* my_text_edit_ptr_;
            QLineEdit* my_line_edit_ptr_;

            void do_setup_view_(void);
            void do_setup_events_(void);
         };
      }
   }
}

#endif // SEVILLE_VIEW_WIDGET_LOGWIDGET_H_
