#include "View_AboutDialog.h"

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
