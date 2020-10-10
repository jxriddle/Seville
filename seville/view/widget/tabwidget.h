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

namespace seville
{
   namespace view
   {
      class AppTabWidget : public QTabWidget
      {
         Q_OBJECT

         public:
            explicit AppTabWidget(QWidget* widget_parent_pointer = nullptr);
            virtual ~AppTabWidget();
            //AppTabBar myAppTabBar;
            void addNewTab(QWidget* widget_contents_pointer = nullptr);

         public slots:
            void on_tabMovedTriggered();
            void on_tabClosedTriggered(int index);
            //void on_plusButton_clicked();

         private:
            void do_setupView();
            void do_addNewTab(QWidget* widget_contents_pointer = nullptr);
      };
   }
}

#endif // UI_APPTABWIDGET_H
