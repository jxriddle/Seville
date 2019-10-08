#include <iostream>

#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QSizePolicy>
#include <QStyle>
#include <QTabBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QScreen>

#include "View_AppTabWidget.h"
#include "View_MainWindow.h"
#include "View_ConnectDialog.h"
#include "View_ClientWidget.h"
#include "View_AboutDialog.h"
#include "View_LogWidget.h"

namespace Seville
{
   namespace View
   {
      void MainWindow::doSetupView(void)
      {
         // Window
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         setUnifiedTitleAndToolBarOnMac(true);

         QScreen *primaryScreen = QGuiApplication::primaryScreen();
         auto screen = primaryScreen->geometry();

         QSize defaultWindowSize = QSize(static_cast<int>(.67*screen.width()),
                                         static_cast<int>(.67*screen.height()));

         setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                         Qt::AlignCenter,
                                         defaultWindowSize,
                                         primaryScreen->geometry()));

         setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
         adjustSize();

         //setContentsMargins(0, menuBar_->height(), 0, 0); // left, top, right, bottom
         //setContentsMargins(0, 0, 0, 0);
         //mainLayout_ = new QVBoxLayout; //(this);
         //mainLayout_->setMenuBar(menuBar_);
         //mainLayout_->addWidget(menuBar_, 0, 0);

         // Tab Widget
         myTabWidget = new AppTabWidget(this);
         myTabWidget->setGeometry(0, 0, width(), height()); // x, y, w, h
         layout()->setSpacing(0);
         //layout()->addWidget(tabWidget_);
         auto clientWidget = new ClientWidget(this);
         //connect(palRoomWidget, )
         myTabWidget->addNewTab(clientWidget);
         //mainLayout_->setSpacing(0);
         //mainLayout_->addWidget(tabWidget_, 1);

         //setLayout(mainLayout_);
         //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

         // Set Main Window to Tab Widget
         setCentralWidget(myTabWidget);

         QSettings settings;
         restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
         restoreState(settings.value("mainWindowState").toByteArray());
      }

      void MainWindow::doSetupActions(void)
      {
         // Create Actions
         myNewHostConnectionAction =
               new QAction(tr("&New Connection"), this);
         myCloseHostConnectionAction =
               new QAction(tr("&Close Connection"), this);
         myQuitAppAction = new QAction(tr("E&xit"), this);
         myUndoContentAction = new QAction(tr("&Undo"), this);
         myRedoContentAction = new QAction(tr("&Redo"), this);
         myCutContentAction = new QAction(tr("&Cut"), this);
         myCopyContentAction = new QAction(tr("&Copy"), this);
         myPasteContentAction = new QAction(tr("&Paste"), this);
         myAboutAppAction = new QAction(tr("&About Seville"), this);
         myToggleLogAction = new QAction(tr("&Log"), this);

         // Assign Action Shortcut Keys
         myNewHostConnectionAction->setShortcut(QKeySequence::New);
         myCloseHostConnectionAction->setShortcut(Qt::CTRL | Qt::Key_W);
         myCloseHostConnectionAction->setEnabled(false);
         myQuitAppAction->setShortcut(QKeySequence::Quit);
         myUndoContentAction->setShortcut(QKeySequence::Undo);
         myRedoContentAction->setShortcut(QKeySequence::Redo);
         myCutContentAction->setShortcut(QKeySequence::Cut);
         myCopyContentAction->setShortcut(QKeySequence::Copy);
         myPasteContentAction->setShortcut(QKeySequence::Paste);
         myToggleLogAction->setShortcut(Qt::CTRL | Qt::Key_L);
      }

      void MainWindow::doSetupMenus(void)
      {
         // Create Menu Bar
         myMenuBar = new QMenuBar(this);
         myFileMenu = myMenuBar->addMenu(tr("&File"));
         myEditMenu = myMenuBar->addMenu(tr("&Edit"));
         myWindowMenu = myMenuBar->addMenu(tr("&Window"));
         myHelpMenu = myMenuBar->addMenu(tr("&Help"));

         // File Menu
         myFileMenu->addAction(myNewHostConnectionAction);
         myFileMenu->addAction(myCloseHostConnectionAction);
         myFileMenu->addSeparator();
         myFileMenu->addAction(myQuitAppAction);

         // Edit Menu
         myEditMenu->addAction(myUndoContentAction);
         myEditMenu->addAction(myRedoContentAction);
         myEditMenu->addSeparator();
         myEditMenu->addAction(myCutContentAction);
         myEditMenu->addAction(myCopyContentAction);
         myEditMenu->addAction(myPasteContentAction);
         myEditMenu->addSeparator();

         // Window Menu
         //myWindowMenu->addAction(myToggleLogAction);
         myWindowMenu->addAction(myDockWidgetForLog->toggleViewAction());

         // About Menu
         myHelpMenu->addSeparator();
         myHelpMenu->addAction(myAboutAppAction);

         setMenuBar(myMenuBar);
      }

      void MainWindow::doSetupDocks(void)
      {
         myLogWidget = new LogWidget(this);

         myDockWidgetForLog = new QDockWidget(tr("Log"), this);
         myDockWidgetForLog->setAllowedAreas(
                  Qt::LeftDockWidgetArea |
                  Qt::BottomDockWidgetArea |
                  Qt::RightDockWidgetArea
         );

         myDockWidgetForLog->setWidget(myLogWidget);
         addDockWidget(Qt::RightDockWidgetArea, myDockWidgetForLog);
      }

      void MainWindow::doSetupEvents(void)
      {
         // Connect Action Signals to Slots
         connect(
            myNewHostConnectionAction, &QAction::triggered,
            this, &Seville::View::MainWindow::onNewHostConnectionActionTriggered);

         connect(
            myCloseHostConnectionAction, &QAction::triggered,
            this, &Seville::View::MainWindow::onCloseHostConnectionActionTriggered);

         connect(
            myQuitAppAction, &QAction::triggered,
            this, &Seville::View::MainWindow::onQuitAppActionTriggered);

         connect(
            myAboutAppAction, &QAction::triggered,
            this, &Seville::View::MainWindow::onAboutAppActionTriggered);

         connect(
            myToggleLogAction, &QAction::triggered,
            this, &Seville::View::MainWindow::onToggleLogWindowActionTriggered);
      }

      void MainWindow::resizeEvent(QResizeEvent* event)
      {
         //menuBar_->setGeometry(0, 0, width(), menuBar_->height());
         //tabWidget_->setGeometry(0, 0, width(), height());
         //tabWidget_->resize();

         //PalRoomWidget* palRoomWidget = static_cast<PalRoomWidget*>(myTabWidget->currentWidget());
         //palRoomWidget->resize(event->size());
         QMainWindow::resizeEvent(event);
      }

      void MainWindow::closeEvent(QCloseEvent* event)
      {
         (void)event;
         QSettings settings;
         settings.setValue("mainWindowGeometry", saveGeometry());
         settings.setValue("mainWindowState", saveState());
         QApplication::quit();
      }

      void MainWindow::onNewHostConnectionActionTriggered(void)
      {
         ClientWidget *activeClientWidget = static_cast<ClientWidget *>(myTabWidget->currentWidget());
         //connect(palTabWidget, resize);
         activeClientWidget->promptNewConnection();
      }

      void MainWindow::onCloseHostConnectionActionTriggered(void)
      {
         // if last tab is closed, quit application
         if (myTabWidget->count() <= 2) {
            QApplication::quit();
         }
         myTabWidget->removeTab(myTabWidget->currentIndex());
      }

      void MainWindow::onQuitAppActionTriggered(void)
      {
         QApplication::quit();
      }

      /*
      void MainWindow::doOnTabWidgetTabBarClicked(int index)
      {
         // New tab
         if (index == tabWidget_->count() - 1) {
            //QMessageBox::information(this, "New Tab", "New tab");
            QWidget *tabPage = new QWidget(tabWidget_);
            myTabWidget->insertTab(myTabWidget->count() - 1, tabPage, tr("New Tab"));
         }
      }

      void MainWindow::doOnLineEditAddressReturnPressed()
      {
         // connect(
      }

      void MainWindow::doOnTabWidgetTabCloseRequested(int index)
      {
         (void)index;
         if (index == myTabWidget->count() - 1) {
            myTabWidget->setCurrentIndex(myTabWidget->count() - 2);
         }
      }
      */
      void MainWindow::onAboutAppActionTriggered(void)
      {
         auto aboutDialog = new AboutDialog(this);
         aboutDialog->exec();
      }

      void MainWindow::onToggleLogWindowActionTriggered(void)
      {
         if (!myLogWidget->isVisible())
            myLogWidget->show();
         else
            myLogWidget->hide();
      }

      MainWindow::~MainWindow(void)
      {
         if (myLogWidget != nullptr)
            delete(myLogWidget);
         /*
         if (this->menuBar != nullptr)
         {
            delete this->menuBar;
         }
         if (this->tabWidget != nullptr)
         {
            delete this->tabWidget;
         }
         */
      }

      //void MainWindow::onPalRoomWidgetClicked()
      //{
      //}
      MainWindow::MainWindow(QWidget* parent)
         : QMainWindow(parent)
      {
          doSetupView();
          doSetupActions();
          doSetupDocks();
          doSetupMenus();
          doSetupEvents();
      }
   }
}
