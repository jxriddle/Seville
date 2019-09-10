#include "ui_log_widget.h"

namespace Seville
{
   namespace Ui
   {
      LogWidget::LogWidget(QWidget *parent) : QDockWidget(parent)
      {
         doSetupUi();
      }

      LogWidget::~LogWidget()
      {
        // close all tabs?
      }

      void LogWidget::doSetupUi()
      {

      }
   }
}
