#ifndef SEVILLE_VIEW_WIDGET_TABWIDGET_H_
#define SEVILLE_VIEW_WIDGET_TABWIDGET_H_

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTabWidget>

namespace seville
{
   namespace view
   {
      namespace widget
      {
         class TabWidget : public QTabWidget
         {
            Q_OBJECT

            public:
               explicit TabWidget(QWidget* widget_parent_ptr = nullptr);
               virtual ~TabWidget(void);

               auto add_new_tab(QWidget* widget_contents_ptr = nullptr)
                     -> void;

            public slots:
               void on_tab_moved_triggered(void);
               void on_tab_closed_triggered(int index);
               //auto on_plus_button_clicked(void) -> void;

            private:
               //AppTabBar myAppTabBar;

               auto do_setup_view_(void) -> void;
               auto do_add_new_tab_(QWidget* widget_contents_ptr = nullptr)
                     -> void;
         };
      }
   }
}

#endif // SEVILLE_VIEW_WIDGET_TABWIDGET_H_
