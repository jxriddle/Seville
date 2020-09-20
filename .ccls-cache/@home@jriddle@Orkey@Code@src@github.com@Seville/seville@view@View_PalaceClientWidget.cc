#include "View_PalaceClientWidget.h"

namespace Seville
{
   namespace View
   {
      PalaceClientWidget::~PalaceClientWidget(void)
      {
         do_deinit();
      }

      PalaceClientWidget::PalaceClientWidget(QWidget *pParent) : QWidget(pParent)
      {
         do_init();
      }
   }
}
