#include <QVBoxLayout>

#include "seville/palace/base/client.h"
#include "seville/view/widget/logwidget.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         LogWidget::LogWidget(QWidget* widget_parent_pointer) \
            : QWidget(widget_parent_pointer) {}

         auto LogWidget::palaceClientPointer(void) -> palace::Client*
         {
            return my_client_palace_pointer_;
         }

         auto LogWidget::setPalaceClientPointer( \
               palace::Client* palace_client_pointer) -> void
         {
            my_client_palace_pointer_ = palace_client_pointer;
         }
      }
   }
}
