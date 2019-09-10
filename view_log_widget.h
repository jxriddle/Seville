#ifndef UI_LOGWIDGET_H
#define UI_LOGWIDGET_H

#include <QDockWidget>

namespace Seville
{
   namespace View
   {
      class LogWidget : public QDockWidget
      {
         public:
            explicit LogWidget(QWidget *parent = nullptr);
            virtual ~LogWidget();

         private:
            void doSetupUi();
      };
   }
}

#endif // UI_LOGWIDGET_H
