#include <QVBoxLayout>
#include "View_LogWidget.h"

namespace Seville
{
   namespace View
   {
      void LogWidget::doSetupView()
      {
         //setGeometry(100, 100, 100, 500);
         //myBaseLayout = new QVBoxLayout(this);

         //setLayout(myBaseLayout);
      }

      void LogWidget::doSetupEvents()
      {

      }

      LogWidget::~LogWidget()
      {
        // close all tabs?
      }

      LogWidget::LogWidget(QWidget *parent)
         : QWidget(parent)
      {
         doSetupView();
         doSetupEvents();
      }
   }
}
