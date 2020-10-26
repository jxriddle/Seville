//#include "uiapptabbar.h"
#include "tabwidget.h"

#include <iostream>

#include <QToolButton>
#include <QTabWidget>
#include <QCloseEvent>
#include <QObject>
#include <QApplication>

namespace seville
{
   namespace view
   {
      namespace widget
      {
         TabWidget::TabWidget(QWidget* widget_parent_pointer) \
            : QTabWidget(widget_parent_pointer)
         {
             do_setup_view_();
         }

         TabWidget::~TabWidget()
         {
            // close all tabs?
         }

         void TabWidget::add_new_tab(QWidget* widget_contents_pointer)
         {
            do_add_new_tab_(widget_contents_pointer);
         }

         void TabWidget::do_setup_view_()
         {
            setTabShape(QTabWidget::TabShape::Triangular);
            ////setTabBar(&this->appTabBar);
            setTabsClosable(true);
            setMovable(true);

            // Layout
            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setMargin(0);
            layout->addStretch(1);
            setLayout(layout);
            //tabWidget_->setSizePolicy();

            //connect(tb, SIGNAL(clicked()), this, SLOT(on_addTab_triggered()));
            connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_closed_triggered(int)));
            ////connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
            ////connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
            //connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));
         }

         void TabWidget::do_add_new_tab_(QWidget* widget_contents_pointer)
         {
            // new tab
            QWidget *newTab = widget_contents_pointer;
            if (newTab == nullptr) {
               newTab = new QWidget(this);
            }

            addTab(newTab, tr("Client %0").arg(QString::number(this->count())));
            setCurrentWidget(newTab);
         }

         void TabWidget::on_tab_moved_triggered()
         {
         }

         void TabWidget::on_tab_closed_triggered(int index)
         {
            delete widget(index);
            if (count() <= 0) {
               QApplication::quit();
            }
            removeTab(index);
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
