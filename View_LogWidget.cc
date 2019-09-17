#include "View_LogWidget.h"

namespace Seville
{
   namespace View
   {
      LogWidget::LogWidget(QWidget *parent) : QDockWidget(parent)
      {
         doSetupView();
      }

      LogWidget::~LogWidget()
      {
        // close all tabs?
      }

      void LogWidget::doSetupView()
      {

      }
   }
}
