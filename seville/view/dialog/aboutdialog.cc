#include "seville/view/dialog/aboutdialog.h"

namespace seville
{
   namespace view
   {
      AboutDialog::~AboutDialog()
      {
         do_deinit();
      }

      AboutDialog::AboutDialog(QWidget* pParent) : QDialog(pParent)
      {
          do_init();
      }
   }
}
