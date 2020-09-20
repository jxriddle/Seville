#include <QVBoxLayout>
#include "View_PalaceLogWidget.h"

namespace seville
{
   namespace view
   {
      PalaceLogWidget::~PalaceLogWidget()
      {
         do_deinit();
      }

      PalaceLogWidget::PalaceLogWidget(Palace::Client* pPalaceClient, QWidget* pParent)
         : QWidget(pParent)
      {
         do_init(pPalaceClient);
      }
   }
}
