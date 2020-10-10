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
      AppTabWidget::AppTabWidget(QWidget* widget_parent_pointer) \
         : QTabWidget(widget_parent_pointer)
      {
          do_setupView();
      }

      AppTabWidget::~AppTabWidget()
      {
         // close all tabs?
      }

      void AppTabWidget::addNewTab(QWidget* widget_contents_pointer)
      {
         do_addNewTab(widget_contents_pointer);
      }

      void AppTabWidget::do_setupView()
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
         connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tabClosedTriggered(int)));
         ////connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
         ////connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
         //connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));
      }

      void AppTabWidget::do_addNewTab(QWidget* widget_contents_pointer)
      {
         // new tab
         QWidget *newTab = widget_contents_pointer;
         if (newTab == nullptr) {
            newTab = new QWidget(this);
         }

         addTab(newTab, tr("Client %0").arg(QString::number(this->count())));
         setCurrentWidget(newTab);
      }

      void AppTabWidget::on_tabMovedTriggered()
      {
      }

      void AppTabWidget::on_tabClosedTriggered(int index)
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
