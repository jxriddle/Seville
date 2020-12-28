#include <QToolButton>
#include <QTabWidget>
#include <QCloseEvent>
#include <QObject>
#include <QApplication>

//#include "uiapptabbar.h"
#include "tabwidget.h"
#include "clientwidget.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         TabWidget::TabWidget(QWidget* parent_widget_ptr)
            : QTabWidget(parent_widget_ptr)
         {
             do_setup_view_();
             do_setup_events_();
         }

         TabWidget::~TabWidget(void)
         {
            // close all tabs?
         }

         void TabWidget::add_new_tab(QWidget* content_widget_ptr)
         {
            do_add_new_tab_(content_widget_ptr);
         }

         void TabWidget::do_setup_view_(void)
         {
            setTabShape(QTabWidget::TabShape::Triangular);
            ////setTabBar(&this->appTabBar);
            setTabsClosable(true);
            setMovable(true);

            // Layout
            QVBoxLayout *layout_ptr = new QVBoxLayout(this);
            layout_ptr->setContentsMargins(0, 0, 0, 0);
            layout_ptr->setMargin(0);
            layout_ptr->addStretch(1);
            this->setLayout(layout_ptr);
            //tabWidget_->setSizePolicy();

            setStyleSheet("border: 1px solid blue");
         }

         void TabWidget::do_setup_events_(void)
         {
            //connect(tb, SIGNAL(clicked()), this, SLOT(on_addTab_triggered()));
            //this->connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_closed_triggered(int)));
            connect(this, &seville::view::widget::TabWidget::
                    tabCloseRequested,
                    this, &seville::view::widget::TabWidget::
                    on_did_request_close_tab);
            ////connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
            ////connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
            //connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));

            //auto widget = this->widget(0); //currentWidget();
            //this->widget(0);
         }

         void TabWidget::do_add_new_tab_(QWidget* content_widget_ptr)
//               widget::ClientWidget* client_widget_ptr)
         {
            // new tab
            //QWidget *new_tab_ptr = content_widget_ptr;
            auto client_widget_ptr =
                  static_cast<widget::ClientWidget*>(content_widget_ptr);

            if (client_widget_ptr == nullptr) {
               client_widget_ptr = new ClientWidget(this);
            }

            this->addTab(client_widget_ptr, tr("New Connection"));
                         //.arg(QString::number(this->count())));
            this->setCurrentWidget(client_widget_ptr);

            connect(
               client_widget_ptr, &ClientWidget::did_resize,
               this, &TabWidget::on_client_tab_widget_did_resize);
         }

         void TabWidget::on_tab_moved_triggered(void)
         {
         }

         void TabWidget::on_did_request_close_tab(int index)
         {
            auto client_widget_ptr =
                  static_cast<widget::ClientWidget*>(widget(index));

            disconnect(
               client_widget_ptr, &ClientWidget::did_resize,
               this, &TabWidget::on_client_tab_widget_did_resize);

            this->removeTab(index);
            delete client_widget_ptr;

            if (count() <= 0) {
               QApplication::quit();
            }
         }

         void TabWidget::on_client_tab_widget_did_resize(int width, int height)
         {
            auto sender = QObject::sender();
            if (currentWidget() == sender) {
               //auto client_widget_ptr =
               //      static_cast<widget::ClientWidget*>(sender);
               //client_widget_ptr->;
               this->resize(width, height);
            }
         }

         //void AppTabWidget::on_plusButton_clicked()
         //{
         //  this->addTab(new PalRoomWidget(), "PalClient");
         //}

         /*
         void AppTabWidget::currentChanged(int index)
         {
            QTabBar *tabBar = this->tabBar();
            int i = 0;
            while (i < this->count() - 1) {
               //tabBar->tabButton(i, QTabBar::RightSide)->show();
               i++;
            }
           //tabBar->tabButton(i, QTabBar::RightSide)->hide();
         }
         */
      }
   }
}
