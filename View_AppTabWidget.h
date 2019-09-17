#ifndef UI_APPTABWIDGET_H
#define UI_APPTABWIDGET_H

#include <QTabWidget>

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Seville
{
   namespace View
   {
      class AppTabWidget : public QTabWidget
      {
         Q_OBJECT

         public:
            explicit AppTabWidget(QWidget* parent = nullptr);
            virtual ~AppTabWidget();
            //AppTabBar myAppTabBar;
            void addNewTab(QWidget* contents = nullptr);

         public slots:

         private:
            void doSetupView();
            void doAddNewTab(QWidget* contents = nullptr);

         private slots:
            void doOnTabMovedTriggered();
            void doOnTabClosedTriggered(int index);
            //void on_plusButton_clicked();
      };
   }
}

#endif // UI_APPTABWIDGET_H
