#ifndef SEVILLE_LOGWIDGET_H_
#define SEVILLE_LOGWIDGET_H_

#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "seville/palace/base/client.h"

namespace seville
{
   namespace view
   {
      class LogWidget : public QWidget
      {
      public:
         explicit LogWidget(QWidget* widget_parent_pointer = nullptr);
         //virtual ~LogWidget();

         auto palaceClientPointer(void) -> palace::Client*;
         auto setPalaceClientPointer(palace::Client* palace_client_pointer) \
               -> void;

      private:
         palace::Client* my_client_palace_pointer_;
         QVBoxLayout* my_layout_base_;
         QTextEdit* my_textedit_;
         QLineEdit* my_lineedit_;

         void do_setup_view_(void);
         void do_setup_events_(void);
      };
   }
}

#endif // SEVILLE_LOGWIDGET_H_
