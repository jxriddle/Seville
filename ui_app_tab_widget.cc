//#include "uiapptabbar.h"
#include "ui_app_tab_widget.h"

#include <iostream>

#include <QToolButton>
#include <QTabWidget>
#include <QCloseEvent>
#include <QObject>
#include <QApplication>

namespace Seville
{
   namespace Ui
   {
      AppTabWidget::AppTabWidget(QWidget* parent) : QTabWidget(parent)
      {
          doSetupUi();
      }

      AppTabWidget::~AppTabWidget()
      {
         // close all tabs?
      }

      void AppTabWidget::addNewTab(QWidget* contents)
      {
         doAddNewTab(contents);
      }

      void AppTabWidget::doSetupUi()
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
         connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(doOnTabClosedTriggered(int)));
         ////connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
         ////connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
         //connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));
      }

      void AppTabWidget::doAddNewTab(QWidget* widget)
      {
         // new tab
         QWidget *newTab = widget;
         if (newTab == nullptr) {
            newTab = new QWidget(this);
         }

         addTab(newTab, tr("Client %0").arg(QString::number(this->count())));
         setCurrentWidget(newTab);
      }

      void AppTabWidget::doOnTabMovedTriggered()
      {
      }

      void AppTabWidget::doOnTabClosedTriggered(int index)
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
