#include <QMessageBox>

#include "mainwindow.h"

namespace seville
{
   namespace view
   {
      MainWindow::MainWindow(QWidget* parentWidgetPtr)
         : QMainWindow(parentWidgetPtr)
      {
         do_init();
      }

      void MainWindow::on_topLevelDidChange(bool shouldBeVisibleFlag)
      {
         my_logWidgetPtr->setChatLineEditIsVisible(shouldBeVisibleFlag);
      }

      void MainWindow::on_openHostConnectionDidTrigger(void)
      {
         auto activeClientWidgetPtr =
               static_cast<PalaceClientWidget *>(
                  my_tabWidgetPtr->currentWidget());

         //connect(palTabWidget, resize);
         activeClientWidgetPtr->promptOpenConnection();
      }

      void MainWindow::on_closeHostConnectionDidTrigger(void)
      {
         //auto palaceClientPtr = my_tabWidgetPtr->currentPalaceClientPtr();
         do_updateMenus();
         my_tabWidgetPtr->removeTab(my_tabWidgetPtr->currentIndex());
      }

      //void MainWindow::on_client_widget_background_did_change(void)
      //void MainWindow::on_clientWidgetDidResize(int width, int height)
      //{
         //resize(minimumSizeHint());
         //resize(width+10, height+10);
      //   adjustSize();
         //auto msgBox = new QMessageBox(this);
         //msgBox->setText("MainWindow resize called");
         //msgBox->open();
         //msgBox->exec();
      //}

      void MainWindow::
      on_clientConnectionStateDidChange(palace::ConnectionState connectionState)
      {
         (void)connectionState;
         do_updateMenus();
      }

      void MainWindow::on_quitAppDidTrigger(void)
      {
         QApplication::quit();
      }

      //void on_palRoomWidgetClicked(void);
      //void on_tabWidgetTabBarClicked(int index);
      //void on_lineEditAddressReturnPressed(void);
      //void on_tabWidgetTabCloseRequested(int index);
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

      void MainWindow::on_aboutAppDidTrigger(void)
      {
         auto dialog_about = new AboutDialog(this);
         dialog_about->exec();
      }

      void MainWindow::on_logWindowWasToggled(void)
      {
         my_logWidgetPtr->setVisible(!my_logWidgetPtr->isVisible());
         //if (!my_mainLogWidgetPtr->isVisible())
         //   my_mainLogWidgetPtr->show();
         //else
         //   my_mainLogWidgetPtr->hide();
      }

      //void do_deinit(void)
      //{
      //   if (my_widget_log_ptr_ != nullptr)
      //      delete(my_widget_log_ptr_);
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
      //}

      auto MainWindow::do_setupView(void) -> void
      {
         // Window
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         setUnifiedTitleAndToolBarOnMac(true);

//            auto primary_screen_ptr = QGuiApplication::primaryScreen();
//            auto screen = primary_screen_ptr->geometry();
//            auto default_window_size = QSize(
//                     static_cast<int>(.67*screen.width()),
//                     static_cast<int>(.67*screen.height()));

//            setGeometry(QStyle::alignedRect(
//                           Qt::LeftToRight,
//                           Qt::AlignCenter,
//                           default_window_size,
//                           primary_screen_ptr->geometry()));

         //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
         //setMinimumSize(10, 10);

         //setContentsMargins(0, menuBar_->height(), 0, 0); // left, top, right, bottom
         setContentsMargins(0, 0, 0, 0);

         //my_mainLayoutPtr = new QVBoxLayout(this);
         //mainLayout_ = new QVBoxLayout; //(this);
         //mainLayout_->setMenuBar(menuBar_);
         //mainLayout_->addWidget(menuBar_, 0, 0);

         // Tab Widget
         my_tabWidgetPtr = new view::TabWidget(this);
         //my_tabWidgetPtr->setGeometry(0, 0, width(), height()); // x, y, w, h

         my_mainLayoutPtr = layout();

         //layout()->addWidget(my_tabWidget_ptr);
         //auto clientWidget_ptr = new view::PalaceClientWidget(this);
         //connect(palRoomWidget, )
         //my_tabWidgetPtr->addNewTab(clientWidget_ptr);

         //mainLayout_->setSpacing(0);
         //mainLayout_->addWidget(tabWidget_, 1);

         //setLayout(mainLayout_);
         //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

         my_statusBarPtr = new QStatusBar(this);
         //setStatusBar(my_pStatusBar);
         //my_pStatusBar->showMessage(tr("Ready"));
         //layout()->addWidget(my_pStatusBar);

         // Set Main Window to Tab Widget
         setCentralWidget(my_tabWidgetPtr);

         //QSettings settings;
         //restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
         //restoreState(settings.value("mainWindowState").toByteArray());
      }

      auto MainWindow::do_setupActions(void) -> void
      {
         // Create Actions
         my_openHostConnectionActionPtr = new QAction(tr("&Connect"), this);
         my_closeHostConnectionActionPtr = new QAction(tr("&Disconnect"), this);
         my_quitAppActionPtr = new QAction(tr("E&xit"), this);
         my_undoContentActionPtr = new QAction(tr("&Undo"), this);
         my_redoContentActionPtr = new QAction(tr("&Redo"), this);
         my_cutContentActionPtr = new QAction(tr("&Cut"), this);
         my_copyContentActionPtr = new QAction(tr("&Copy"), this);
         my_pasteContentActionPtr = new QAction(tr("&Paste"), this);
         my_aboutAppActionPtr = new QAction(tr("&About Seville"), this);
         my_toggleLogActionPtr = new QAction(tr("&Log"), this);

         // Assign Action Shortcut Keys
         my_openHostConnectionActionPtr->setShortcut(QKeySequence::New);
         my_closeHostConnectionActionPtr->setShortcut(Qt::CTRL | Qt::Key_W);
         my_closeHostConnectionActionPtr->setEnabled(false);
         my_quitAppActionPtr->setShortcut(QKeySequence::Quit);
         my_undoContentActionPtr->setShortcut(QKeySequence::Undo);
         my_redoContentActionPtr->setShortcut(QKeySequence::Redo);
         my_cutContentActionPtr->setShortcut(QKeySequence::Cut);
         my_copyContentActionPtr->setShortcut(QKeySequence::Copy);
         my_pasteContentActionPtr->setShortcut(QKeySequence::Paste);
         my_toggleLogActionPtr->setShortcut(Qt::CTRL | Qt::Key_L);
      }

      auto MainWindow::do_setupMenus(void) -> void
      {
         // Create Menu Bar
         my_menuBarPtr = new QMenuBar(this);
         my_fileMenuPtr = my_menuBarPtr->addMenu(tr("&File"));
         my_editMenuPtr = my_menuBarPtr->addMenu(tr("&Edit"));
         my_windowMenuPtr = my_menuBarPtr->addMenu(tr("&Window"));
         my_helpMenuPtr = my_menuBarPtr->addMenu(tr("&Help"));

         // File Menu
         my_fileMenuPtr->addAction(my_openHostConnectionActionPtr);
         my_fileMenuPtr->addAction(my_closeHostConnectionActionPtr);
         my_fileMenuPtr->addSeparator();
         my_fileMenuPtr->addAction(my_quitAppActionPtr);

         // Edit Menu
         my_editMenuPtr->addAction(my_undoContentActionPtr);
         my_editMenuPtr->addAction(my_redoContentActionPtr);
         my_editMenuPtr->addSeparator();
         my_editMenuPtr->addAction(my_cutContentActionPtr);
         my_editMenuPtr->addAction(my_copyContentActionPtr);
         my_editMenuPtr->addAction(my_pasteContentActionPtr);
         my_editMenuPtr->addSeparator();

         // Window Menu
         //my_windowMenuPtr->addAction(my_toggleLogActionPtr);
         my_windowMenuPtr->addAction(
                  my_logDockWidgetPtr->toggleViewAction());

         // About Menu
         my_helpMenuPtr->addSeparator();
         my_helpMenuPtr->addAction(my_aboutAppActionPtr);

         setMenuBar(my_menuBarPtr);
      }

      auto MainWindow::do_setupDocks(void) -> void
      {
         auto widget_palaceClientPtr =
               static_cast<PalaceClientWidget *>(
                  my_tabWidgetPtr->currentWidget());
         auto palaceClientPtr =
               widget_palaceClientPtr->palaceClientPtr();

         my_logWidgetPtr = new view::LogWidget(this);
         my_logWidgetPtr->setPalaceClientPtr(palaceClientPtr);

         my_logWidgetPtr->setChatLineEditIsVisible(
                  my_logWidgetPtr->isWindow());

         my_logDockWidgetPtr = new QDockWidget(tr("Log"), this);
         my_logDockWidgetPtr->setAllowedAreas(
                  Qt::LeftDockWidgetArea |
                  Qt::BottomDockWidgetArea |
                  Qt::RightDockWidgetArea |
                  Qt::TopDockWidgetArea);

         my_logDockWidgetPtr->setWidget(my_logWidgetPtr);
         addDockWidget(Qt::RightDockWidgetArea, my_logDockWidgetPtr);
      }

      auto MainWindow::do_setupEvents(void) -> void
      {
         // Connect Action Signals to Slots
         connect(my_logDockWidgetPtr, &QDockWidget::topLevelChanged,
                 this, &seville::view::MainWindow::on_topLevelDidChange);

         connect(my_openHostConnectionActionPtr, &QAction::triggered,
                 this, &seville::view::MainWindow::on_openHostConnectionDidTrigger);

         connect(my_closeHostConnectionActionPtr, &QAction::triggered,
                 this, &seville::view::MainWindow::on_closeHostConnectionDidTrigger);

         connect(my_quitAppActionPtr, &QAction::triggered,
                 this, &seville::view::MainWindow::on_quitAppDidTrigger);

         connect(my_aboutAppActionPtr, &QAction::triggered,
                 this, &seville::view::MainWindow::on_aboutAppDidTrigger);

         connect(my_toggleLogActionPtr, &QAction::triggered,
                 this, &seville::view::MainWindow::on_logWindowWasToggled);
      }

      auto MainWindow::do_setupSizing(void) -> void
      {
         //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
         setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

         //my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);
         my_mainLayoutPtr->setSpacing(0);
         my_mainLayoutPtr->setMargin(0);
         my_mainLayoutPtr->setContentsMargins(0, 0, 0, 0);

         window()->adjustSize();

         centralWidget()->updateGeometry();
         centralWidget()->adjustSize();

         updateGeometry();
         adjustSize();
      }

      auto MainWindow::do_updateMenus(void) -> void
      {
         auto palaceClientPtr = my_tabWidgetPtr->currentPalaceClientPtr();

         if (palaceClientPtr != nullptr) {
            auto connectionState = palaceClientPtr->connectionState();

            switch (connectionState) {
            case palace::ConnectionState::kConnectedState:
               my_openHostConnectionActionPtr->setEnabled(false);
               my_closeHostConnectionActionPtr->setEnabled(true);
               break;
            case palace::ConnectionState::kHandshakingState:
               my_openHostConnectionActionPtr->setEnabled(false);
               my_closeHostConnectionActionPtr->setEnabled(true);
               break;
            //case palace::ConnectionState::kDisconnectedState:
            default:
               my_openHostConnectionActionPtr->setEnabled(true);
               my_closeHostConnectionActionPtr->setEnabled(false);
            }
         }
      }

      auto MainWindow::do_init(void) -> void
      {
         do_setupView();
         do_setupActions();
         do_setupDocks();
         do_setupMenus();
         do_setupEvents();
         do_setupSizing();
      }

//         void MainWindow::resizeEvent(QResizeEvent* event_ptr)
//         {
//            //menuBar_->setGeometry(0, 0, width(), menuBar_->height());
//            //tabWidget_->setGeometry(0, 0, width(), height());
//            //tabWidget_->resize();

//            //PalRoomWidget* palRoomWidget = static_cast<PalRoomWidget*>(myTabWidget->currentWidget());
//            //palRoomWidget->resize(event->size());
//            QMainWindow::resizeEvent(event_ptr);
//         }

      void MainWindow::closeEvent(QCloseEvent* closeEventPtr)
      {
         (void)closeEventPtr;
         QSettings settings;
         settings.setValue("mainWindowGeometry", saveGeometry());
         settings.setValue("mainWindowState", saveState());
         QApplication::quit();
      }
   }
}
