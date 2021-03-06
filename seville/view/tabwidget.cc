#include <QToolButton>
#include <QTabWidget>
#include <QCloseEvent>
#include <QObject>
#include <QApplication>

//#include "uiapptabbar.h"
#include "tabwidget.h"
#include "palaceclientwidget.h"

namespace seville
{
   namespace view
   {
      TabWidget::TabWidget(QWidget* parentWidgetPtr)
         : QTabWidget(parentWidgetPtr)
      {
          do_setupView();
          do_setupEvents();
      }

      TabWidget::~TabWidget(void)
      {
         // close all tabs?
      }

      void TabWidget::addNewTab(QWidget* contentWidgetPtr)
      {
         do_addNewTab(contentWidgetPtr);
      }

      auto TabWidget::removeTab(int index) -> void
      {
         QTabWidget::removeTab(index);

         if (count() < 1) {
            do_addNewTab();
         }
      }

      auto TabWidget::clear(void) -> void
      {
         do_clear();
      }

      auto TabWidget::do_clear(void) -> void
      {
         //my_currentPalaceClientPtr = nullptr;
      }

      auto do_mainWindowPtr(void) -> view::MainWindow*
      {
         foreach (auto w, qApp->topLevelWidgets())
            if (auto mainWindowPtr = qobject_cast<view::MainWindow*>(w))
               return mainWindowPtr;
         return nullptr;
      }

      auto do_logWidgetPtr(void) -> view::LogWidget*
      {
         foreach (auto w, qApp->topLevelWidgets())
            if (auto logWidgetPtr = qobject_cast<view::LogWidget*>(w))
               return logWidgetPtr;
         return nullptr;
      }

      auto TabWidget::do_setupView(void) -> void
      {
         setTabShape(QTabWidget::TabShape::Triangular);
         ////setTabBar(&this->appTabBar);
         setTabsClosable(true);
         setMovable(true);

         // Layout
         my_mainLayoutPtr = new QVBoxLayout(this);
         //auto layout_ptr = new QGridLayout(this);

         //my_mainLayoutPtr->setSizeConstraint(QLayout::SetFixedSize);
         //layout_ptr->addStretch(1);
         //setContentsMargins(0, 0, 0, 0);
         setLayout(my_mainLayoutPtr);
         //tabWidget_->setSizePolicy();
         //resize(minimumSizeHint());
         adjustSize();

         // setStyleSheet("border: 1px solid blue");
      }

      auto TabWidget::do_setupEvents(void) -> void
      {
         //connect(tb, SIGNAL(clicked()), this, SLOT(on_addTab_triggered()));
         //this->connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_closed_triggered(int)));
         connect(this, &seville::view::TabWidget::tabCloseRequested,
                 this, &seville::view::TabWidget::on_tabDidClose);
         ////connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
         ////connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
         //connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));

         //auto widget = this->widget(0); //currentWidget();
         //this->widget(0);

         connect(this, &seville::view::TabWidget::currentChanged,
                 this, &seville::view::TabWidget::on_currentTabDidChange);
      }

      auto TabWidget::do_addNewTab(QWidget* contentWidgetPtr) -> void
//               widget::ClientWidget* client_widget_ptr)
      {
         // new tab
         //QWidget *new_tab_ptr = content_widget_ptr;
         auto clientWidgetPtr =
               static_cast<PalaceClientWidget*>(contentWidgetPtr);
         //auto clientWidgetPtr = new PalaceClientWidget(this);

         if (clientWidgetPtr == nullptr)
            clientWidgetPtr = new PalaceClientWidget(this);

         this->addTab(clientWidgetPtr, tr("New Connection"));
                      //.arg(QString::number(this->count())));
         this->setCurrentWidget(clientWidgetPtr);

         auto mainWindowPtr = do_mainWindowPtr();
         if (mainWindowPtr != nullptr) {
            connect(
               clientWidgetPtr,
               &PalaceClientWidget::widgetBackgroundDidChangeEvent,
               this, &TabWidget::on_widgetBackgroundDidChange);
         }
      }

      auto TabWidget::do_setupSizing(void) -> void
      {
         my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);
         my_mainLayoutPtr->setContentsMargins(0, 0, 0, 0);
         my_mainLayoutPtr->setMargin(0);
      }

      void TabWidget::on_widgetBackgroundDidChange(void)
      {
         tabBar()->adjustSize();

         updateGeometry();
         adjustSize();
         window()->adjustSize();

         auto mainWindowPtr = static_cast<QMainWindow*>(parent());
         if (mainWindowPtr != nullptr) {
            mainWindowPtr->updateGeometry();
            mainWindowPtr->adjustSize();
            mainWindowPtr->window()->layout()->update();
            mainWindowPtr->window()->adjustSize();
         }
      }

      void TabWidget::on_tabDidMove(void)
      {
      }

      void TabWidget::on_tabDidClose(int index)
      {
         auto clientWidgetPtr =
               static_cast<PalaceClientWidget*>(widget(index));

//         disconnect(
//            clientWidgetPtr, &PalaceClientWidget::widgetDidResizeEvent,
//            this, &TabWidget::on_clientTabWidgetDidResize);

         removeTab(index);
         delete clientWidgetPtr;

         if (count() <= 0) {
            //QApplication::quit();
            do_addNewTab(new view::PalaceClientWidget(this));
         }
      }

//      void TabWidget::on_clientTabWidgetDidResize(int width, int height)
//      {
//         (void)width;
//         (void)height;
//         auto sender = QObject::sender();
//         auto widget = currentWidget(); // TODO this causes a segfault
//         if (widget != nullptr && widget == sender) {
//            //auto client_widget_ptr =
//            //      static_cast<widget::ClientWidget*>(sender);
//            //client_widget_ptr->;
//            //resize(width, height);
//            resize(minimumSizeHint());
//         }
//      }

      void TabWidget::on_currentTabDidChange(int index)
      {
         auto mainWindowPtr = do_mainWindowPtr();
         if (mainWindowPtr == nullptr)
            return;

         auto logWidgetPtr = mainWindowPtr->logWidgetPtr();
         if (logWidgetPtr == nullptr)
            return;

         auto clientWidgetPtr = static_cast<PalaceClientWidget*>(widget(index));
         if (clientWidgetPtr == nullptr)
            return;

         auto palaceClientPtr = clientWidgetPtr->palaceClientPtr();
         if (palaceClientPtr == nullptr)
            return;

         logWidgetPtr->setPalaceClientPtr(palaceClientPtr);
      }

      //void AppTabWidget::on_plusButton_clicked()
      //{
      //  this->addTab(new PalRoomWidget(), "PalClient");
      //}

//      auto TabWidget::currentChanged(int index) -> void
//      {
//         QTabBar *tabBar = this->tabBar();
//         int i = 0;
//         while (i < this->count() - 1) {
//            //tabBar->tabButton(i, QTabBar::RightSide)->show();
//            i++;
//         }
//        //tabBar->tabButton(i, QTabBar::RightSide)->hide();
//      }
   }
}
