#include "seville/view/dialog/aboutdialog.h"

namespace seville
{
   namespace view
   {
      AboutDialog::~AboutDialog(void)
      {
         do_deinit();
      }

      AboutDialog::AboutDialog(QWidget* parent_widget_ptr) \
         : QDialog(parent_widget_ptr)
      {
          do_init();
      }
   }
}
