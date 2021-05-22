#include <QToolButton>
#include <QTabWidget>
#include <QCloseEvent>
#include <QObject>
#include <QApplication>

//#include "uiapptabbar.h"
#include "tabwidget.h"
#include "logwidget.h"
#include "palaceclientwidget.h"

namespace seville
{
   namespace view
   {
      TabWidget::TabWidget(QWidget* parentWidgetPtr)
         : QTabWidget(parentWidgetPtr)
      {
         do_init();
      }

      TabWidget::~TabWidget(void)
      {
         // close all tabs?
      }

      void TabWidget::addNewTab(QWidget* contentWidgetPtr)
      {
         do_init(contentWidgetPtr);
      }

      void TabWidget::removeTab(int index)
      {
         QTabWidget::removeTab(index);

         if (count() < 1) {
            do_addNewTab();
         }
      }

      void TabWidget::closeCurrentTab(void)
      {

      }

      palace::Client* TabWidget::currentPalaceClientPtr(void)
      {
         auto currentPalaceClientWidgetPtr =
               static_cast<seville::view::PalaceClientWidget*>(currentWidget());

         seville::palace::Client* currentPalaceClientPtr = nullptr;
         if (currentPalaceClientWidgetPtr != nullptr)
            currentPalaceClientPtr =
                  currentPalaceClientWidgetPtr->palaceClientPtr();

         return currentPalaceClientPtr;
      }

      void TabWidget::clear(void)
      {
         do_clear();
      }

      void TabWidget::do_clear(void)
      {
         //my_currentPalaceClientPtr = nullptr;
      }

//      auto do_mainWindowPtr(void) -> view::MainWindow*
//      {
//         foreach (auto w, qApp->topLevelWidgets())
//            if (auto mainWindowPtr = qobject_cast<view::MainWindow*>(w))
//               return mainWindowPtr;
//         return nullptr;
//      }

      view::LogWidget* do_logWidgetPtr(void)
      {
         foreach (auto w, qApp->topLevelWidgets())
            if (auto logWidgetPtr = qobject_cast<view::LogWidget*>(w))
               return logWidgetPtr;
         return nullptr;
      }

      void TabWidget::do_setupView(void)
      {
         // setTabShape(QTabWidget::TabShape::Triangular);
         // setTabBar(&this->appTabBar);
         setTabsClosable(true);
         setMovable(true);

         // Layout
         my_mainLayoutPtr = new QVBoxLayout(this);
         //auto layout_ptr = new QGridLayout(this);

         // my_mainLayoutPtr->setSizeConstraint(QLayout::SetFixedSize);
         // layout_ptr->addStretch(1);
         // setContentsMargins(0, 0, 0, 0);
         setLayout(my_mainLayoutPtr);
         // tabWidget_->setSizePolicy();
         // resize(minimumSizeHint());
         adjustSize();

         // setStyleSheet("border: 1px solid blue");
      }

      void TabWidget::do_teardownEvents(void)
      {
         // connect(tb, SIGNAL(clicked()), this, SLOT(on_addTab_triggered()));
         // this->connect(this, SIGNAL(tabCloseRequested(int)),
         // this, SLOT(on_tab_closed_triggered(int)));
         disconnect(this, &seville::view::TabWidget::tabCloseRequested,
                    this, &seville::view::TabWidget::on_tabDidClose);
         //// connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
         //// connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
         // connect(this, SIGNAL(removeTab(int)),
         //         this, SLOT(on_tabClosed_triggered(int)));

         // auto widget = this->widget(0); //currentWidget();
         // this->widget(0);

         // disconnect(this, &seville::view::TabWidget::currentChanged,
         //            this, &seville::view::TabWidget::on_currentTabDidChange);

         // disconnect(this, &seville::view::TabWidget::tabBarDoubleClicked,
         //            this, &seville::view::TabWidget::on_tabBarDoubleClicked);
      }

      void TabWidget::do_setupEvents(void)
      {
         // connect(tb, SIGNAL(clicked()), this, SLOT(on_addTab_triggered()));
         // this->connect(this, SIGNAL(tabCloseRequested(int)),
         // this, SLOT(on_tab_closed_triggered(int)));
         connect(this, &seville::view::TabWidget::tabCloseRequested,
                 this, &seville::view::TabWidget::on_tabDidClose);
         //// connect(appTabBar.PlusButton(), SIGNAL(clicked()), this, SLOT(on_plusButton_clicked()));
         //// connect(&appTabBar, SIGNAL(tabMoved(int, int)), this, SLOT(on_tabMoved_triggered()));
         // connect(this, SIGNAL(removeTab(int)), this, SLOT(on_tabClosed_triggered(int)));

         // auto widget = this->widget(0); //currentWidget();
         // this->widget(0);

         // connect(this, &seville::view::TabWidget::currentChanged,
         //         this, &seville::view::TabWidget::on_currentTabDidChange);

         // connect(this, &seville::view::TabWidget::tabBarDoubleClicked,
         //         this, &seville::view::TabWidget::on_tabBarDoubleClicked);
      }

      void TabWidget::do_addNewTab(QWidget* contentWidgetPtr)
//               widget::ClientWidget* client_widget_ptr)
      {
         // new tab
         //QWidget *new_tab_ptr = content_widget_ptr;
         auto palaceClientWidgetPtr =
                        qobject_cast<PalaceClientWidget*>(contentWidgetPtr);

         if (palaceClientWidgetPtr == nullptr)
            palaceClientWidgetPtr = new PalaceClientWidget(this);

         // auto clientWidgetPtr = new PalaceClientWidget(this);
         connect(palaceClientWidgetPtr,
                 &PalaceClientWidget::serverNameWasSetEvent,
                 this, &seville::view::TabWidget::on_serverNameWasSet);

         addTab(palaceClientWidgetPtr, tr("New Connection"));
                      // .arg(QString::number(this->count())));
         setCurrentWidget(palaceClientWidgetPtr);
         // my_currentPalaceClientPtr = palaceClientWidgetPtr->palaceClientPtr();
      }

      void TabWidget::do_closeCurrentTab(void)
      {
         auto index = this->currentIndex();
         do_closeTab(index);
      }

      void TabWidget::do_closeTab(int index)
      {
         auto palaceClientWidgetPtr =
               static_cast<PalaceClientWidget*>(widget(index));

         // disconnect(
         //     clientWidgetPtr, &PalaceClientWidget::widgetDidResizeEvent,
         //     this, &TabWidget::on_clientTabWidgetDidResize);

         if (palaceClientWidgetPtr != nullptr) {
            disconnect(palaceClientWidgetPtr,
                       &PalaceClientWidget::serverNameWasSetEvent,
                       this, &seville::view::TabWidget::on_serverNameWasSet);
         }

         removeTab(index);
         delete palaceClientWidgetPtr;

         if (count() <= 0) {
            // QApplication::quit();
            do_addNewTab(new view::PalaceClientWidget(this));         }
      }

      void TabWidget::do_setupSizing(void)
      {
         my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);
         my_mainLayoutPtr->setContentsMargins(0, 0, 0, 0);
         my_mainLayoutPtr->setMargin(0);
      }

      void TabWidget::do_init(QWidget* contentWidgetPtr)
      {
         do_setupView();
         do_setupEvents();
         do_addNewTab(contentWidgetPtr);
         //my_currentPalaceClientPtr = this->
      }

      void TabWidget::on_serverNameWasSet(
            QWidget* senderPtr, const QString& value)
      {
         auto index = indexOf(senderPtr);
         setTabText(index, value);
      }

      void TabWidget::on_tabBarDoubleClicked(void)
      {
         addNewTab();
      }

//      void TabWidget::on_widgetBackgroundDidChange(void)
//      {
//         tabBar()->adjustSize();

//         updateGeometry();
//         adjustSize();
//         window()->adjustSize();

//         auto mainWindowPtr = static_cast<QMainWindow*>(parent());
//         if (mainWindowPtr != nullptr) {
//            mainWindowPtr->updateGeometry();
//            mainWindowPtr->adjustSize();

//            mainWindowPtr->window()->layout()->update();
//            mainWindowPtr->window()->adjustSize();
//         }
//      }

      void TabWidget::on_tabDidMove(void)
      {
      }

      void TabWidget::on_tabDidClose(int index)
      {
         do_closeTab(index);
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

//      void TabWidget::on_currentTabDidChange(int index)
//      {
//         auto mainWindowPtr = do_mainWindowPtr();
//         if (mainWindowPtr == nullptr)
//            return;

//         auto logWidgetPtr = mainWindowPtr->logWidgetPtr();
//         if (logWidgetPtr == nullptr)
//            return;

//         // the following assignment is intentional
//         if (auto palaceClientWidgetPtr =
//             qobject_cast<PalaceClientWidget*>(widget(index))) {

//            if (palaceClientWidgetPtr == nullptr)
//               return;

//            auto palaceClientPtr = palaceClientWidgetPtr->palaceClientPtr();
//            if (palaceClientPtr == nullptr)
//               return;

////            my_currentPalaceClientPtr =
////                  palaceClientWidgetPtr->palaceClientPtr();

//            if (palaceClientPtr == nullptr)
//               return;

//            logWidgetPtr->setPalaceClientPtr(palaceClientPtr);
//         }
//      }

      void TabWidget::on_addNewTabRequested(void)
      {
         do_addNewTab();
      }

      void TabWidget::on_closeCurrentTabRequested(void)
      {
         do_closeCurrentTab();
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
