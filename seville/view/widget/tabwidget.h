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
               explicit TabWidget(QWidget* parent_widget_ptr = nullptr);
               virtual ~TabWidget(void);

               auto add_new_tab(QWidget* content_widget_ptr = nullptr)
                     -> void;

            public slots:
               void on_tab_moved_triggered(void);
               void on_did_request_close_tab(int index);
               //auto on_plus_button_clicked(void) -> void;
               void on_client_tab_widget_did_resize(int width, int height);

            private:
               //AppTabBar myAppTabBar;

               auto do_setup_view_(void) -> void;
               auto do_add_new_tab_(QWidget* content_widget_ptr = nullptr)
                     -> void;
               auto do_setup_events_(void) -> void;
         };
      }
   }
}

#endif // SEVILLE_VIEW_WIDGET_TABWIDGET_H_
