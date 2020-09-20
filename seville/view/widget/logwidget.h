#ifndef UI_LOGWIDGET_H
#define UI_LOGWIDGET_H

#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

namespace Seville
{
   namespace View
   {
      class LogWidget : public QWidget
      {
         private:
            QVBoxLayout* myBaseLayout;
            QTextEdit* myTextEdit;
            QLineEdit* myLineEdit;

            void doSetupView();
            void doSetupEvents();

         public:
            virtual ~LogWidget();
            explicit LogWidget(QWidget *parent = nullptr);
      };
   }
}

#endif // UI_LOGWIDGET_H
