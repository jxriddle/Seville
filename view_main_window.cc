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

#include "view_app_tab_widget.h"
#include "view_main_window.h"
#include "view_connect_dialog.h"
#include "view_pal_client_widget.h"
#include "view_about_dialog.h"

namespace Seville
{
   namespace View
   {
      MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
      {
          doSetupUi();
          doSetupActions();
          doSetupMenus();
          doSetupEvents();
      }

      MainWindow::~MainWindow()
      {
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

      void MainWindow::doSetupUi()
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
         auto palRoomWidget = new PalClientWidget(this);
         //connect(palRoomWidget, )
         myTabWidget->addNewTab(palRoomWidget);
         //mainLayout_->setSpacing(0);
         //mainLayout_->addWidget(tabWidget_, 1);

         //setLayout(mainLayout_);
         //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

         // Set Main Window to Tab Widget
         setCentralWidget(myTabWidget);
      }

      void MainWindow::doSetupActions()
      {
         // Create Actions
         myNewHostConnectionAction = new QAction(tr("&New Connection"), this);
         myCloseHostConnectionAction = new QAction(tr("&Close Connection"), this);
         myQuitAppAction = new QAction(tr("E&xit"), this);
         myUndoContentAction = new QAction(tr("&Undo"), this);
         myRedoContentAction = new QAction(tr("&Redo"), this);
         myCutContentAction = new QAction(tr("&Cut"), this);
         myCopyContentAction = new QAction(tr("&Copy"), this);
         myPasteContentAction = new QAction(tr("&Paste"), this);
         myAboutAppAction = new QAction(tr("&About Seville"), this);

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
      }

      void MainWindow::doSetupMenus()
      {
         // Create Menu Bar
         myMenuBar = new QMenuBar(this);
         myFileMenu = myMenuBar->addMenu(tr("&File"));
         myEditMenu = myMenuBar->addMenu(tr("&Edit"));
         myViewMenu = myMenuBar->addMenu(tr("&View"));
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

         // About Menu
         myHelpMenu->addSeparator();
         myHelpMenu->addAction(myAboutAppAction);

         setMenuBar(myMenuBar);
      }

      void MainWindow::doSetupEvents()
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
      }

      void MainWindow::onNewHostConnectionActionTriggered()
      {
         PalClientWidget *activePalClientWidget = static_cast<PalClientWidget *>(myTabWidget->currentWidget());
         //connect(palTabWidget, resize);
         activePalClientWidget->promptNewConnection();
      }

      void MainWindow::onCloseHostConnectionActionTriggered()
      {
         // if last tab is closed, quit application
         if (myTabWidget->count() <= 2) {
            QApplication::quit();
         }
         myTabWidget->removeTab(myTabWidget->currentIndex());
      }

      void MainWindow::onQuitAppActionTriggered()
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
      void MainWindow::onAboutAppActionTriggered()
      {
         AboutDialog* aboutDialog = new AboutDialog(this);
         aboutDialog->exec();
      }

      //void MainWindow::onPalRoomWidgetClicked()
      //{
      //}
   }
}
