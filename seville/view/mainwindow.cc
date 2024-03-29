#include <QMainWindow>

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

        // void MainWindow::on_currentClientWidgetBackgroundImageDidLoad(void)
        // {
        //     adjustSize();
        // }

        void MainWindow::on_topLevelDidChange(bool shouldBeVisibleFlag)
        {
            my_logWidgetPtr->setChatLineEditIsVisible(shouldBeVisibleFlag);
        }

        void MainWindow::on_openConnectionWasRequested(void)
        {
            auto activeClientWidgetPtr =
                    static_cast<PalaceClientWidget *>(
                        my_tabWidgetPtr->currentWidget());

            activeClientWidgetPtr->promptOpenConnection();
        }

        void MainWindow::on_closeConnectionWasRequested(void)
        {
            auto palaceClientPtr =
                    do_currentPalaceClientWidgetPtr()->palaceClientPtr();
            if (palaceClientPtr->connectionState() ==
                    seville::palace::ConnectionState::kDisconnectedState)
            return;

            palaceClientPtr->disconnectFromHost();

            // do_updateMenus();
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

      void MainWindow::on_currentClientConnectionStateDidChange(
            //QWidget* senderPtr,
            const palace::ConnectionState connectionState)
      {
         //auto currentPalaceClientWidgetPtr = my_tabWidgetPtr->currentWidget();

//         if (senderPtr != nullptr &&
//             currentPalaceClientWidgetPtr != nullptr &&
//             senderPtr == currentPalaceClientWidgetPtr) {
            // auto isDisconnected =
            //       connectionState == palace::ConnectionState::kDisconnectedState;
        switch (connectionState) {
        case palace::ConnectionState::kConnectedState:
           my_openHostConnectionActionPtr->setEnabled(false);
           my_closeHostConnectionActionPtr->setEnabled(true);
           break;
        case palace::ConnectionState::kHandshakingState:
           my_openHostConnectionActionPtr->setEnabled(false);
           my_closeHostConnectionActionPtr->setEnabled(false);
           break;
        default:
           my_openHostConnectionActionPtr->setEnabled(true);
           my_closeHostConnectionActionPtr->setEnabled(false);
        }
//         }
      }

      void MainWindow::on_quitAppWasRequested(void)
      {
         do_deinit();
         QApplication::quit();
      }

      //void on_palRoomWidgetClicked(void);
      //void on_tabWidgetTabBarClicked(int index);
      //void on_lineEditAddressReturnPressed(void);
      //void on_tabWidgetTabCloseRequested(int index);

//      void MainWindow::on_tabWidgetTabBarClicked(int index)
//      {
//         // New tab
//         if (index == my_tabWidgetPtr->count() - 1) {
//            //QMessageBox::information(this, "New Tab", "New tab");
//            QWidget *tabPage = new QWidget(my_tabWidgetPtr);
//            my_tabWidgetPtr->insertTab(my_tabWidgetPtr->count() - 1, tabPage, tr("New Tab"));
//         }
//        }

//        void MainWindow::on_lineEditAddressReturnPressed()
//        {
//         // connect(
//        }

//        void MainWindow::on_tabWidgetTabCloseRequested(int index)
//        {
//         (void)index;
//         if (index == my_tabWidgetPtr->count() - 1) {
//            my_tabWidgetPtr->setCurrentIndex(my_tabWidgetPtr->count() - 2);
//         }
//      }

      void MainWindow::on_aboutAppWasRequested(void)
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

      void MainWindow::on_newTabRequested(void)
      {
         //auto tabBar = my_tabBarPtr;
         //auto palaceClientWidgetPtr = new PalaceClientWidget();
         //palaceClientWidgetPtr->setParent(tabBar);
         // connect(palaceClientWidgetPtr,
         //         &seville::view::PalaceClientWidget::connectionStateDidChangeEvent,
         //         this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

         //my_tabWidgetPtr->addNewTab(palaceClientWidgetPtr);
         my_tabWidgetPtr->addNewTab();
      }

      void MainWindow::on_closeTabRequested(void)
      {
          disconnect(my_currentPalaceClientWidgetPtr->palaceClientPtr(),
                     &seville::palace::Client::connectionStateDidChangeEvent,
                     this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

          // my_tabWidgetPtr->closeCurrentTab();
          my_tabWidgetPtr->removeTab(my_tabWidgetPtr->currentIndex());
      }

      void MainWindow::on_tabWasAddedWithClientWidgetPtr(
            PalaceClientWidget* palaceClientWidgetPtr)
      {
         // auto palaceClientWidgetPtr = new PalaceClientWidget(my_tabBarPtr);
         connect(palaceClientWidgetPtr,
                 &seville::view::PalaceClientWidget::
                 connectionStateDidChangeEvent,
                 this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

         my_tabWidgetPtr->addNewTab(palaceClientWidgetPtr);
         // connect(palaceClientWidgetPtr,
         //         &PalaceClientWidget::widgetBackgroundDidChangeEvent,
         //         my_tabWidgetPtr, &TabWidget::on_widgetBackgroundDidChange);
         // auto palaceClientPtr = palaceClientWidgetPtr->palaceClientPtr();

         // connect(palaceClientPtr,
         //         &seville::palace::Client::connectionStateDidChangeEvent,
         //         this, &seville::view::MainWindow::on_connectionStateDidChange);

         //my_tabWidgetPtr->setWidget
      }

      // void MainWindow::on_tabWasRemovedWithClientWidgetPtr(
      //       PalaceClientWidget* palaceClientWidgetPtr)
      // {
      //    (void)palaceClientWidgetPtr;
         // disconnect(palaceClientWidgetPtr,
         //            &PalaceClientWidget::widgetBackgroundDidChangeEvent,
         //            my_tabWidgetPtr, &TabWidget::on_widgetBackgroundDidChange);
         // auto palaceClientPtr = palaceClientWidgetPtr->palaceClientPtr();

         // disconnect(
         //       palaceClientPtr,
         //       &seville::palace::Client::connectionStateDidChangeEvent,
         //       this, &seville::view::MainWindow::on_connectionStateDidChange);
      // }

      void MainWindow::on_currentTabDidChange(int index)
      {
          (void)index;
         auto newCurrentPalaceClientWidgetPtr =
                 do_currentPalaceClientWidgetPtr();
               //qobject_cast<seville::view::PalaceClientWidget*>(
               //   my_tabWidgetPtr->widget(index));

         auto newCurrentPalaceClientPtr = static_cast<seville::palace::Client*>(nullptr);
         if (newCurrentPalaceClientWidgetPtr == nullptr) {
             return;
         }

         auto currentPalaceClientPtr = my_currentPalaceClientWidgetPtr->palaceClientPtr();
         my_currentPalaceClientWidgetPtr = newCurrentPalaceClientWidgetPtr;

         newCurrentPalaceClientPtr = newCurrentPalaceClientWidgetPtr->palaceClientPtr();
         if (newCurrentPalaceClientPtr == nullptr) {
             return;
         }

          //disconnect(my_currentPalaceClientWidgetPtr->palaceClientPtr(),
         disconnect(currentPalaceClientPtr,
                     &seville::palace::Client::connectionStateDidChangeEvent,
                     this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

         //connect(my_currentPalaceClientWidgetPtr->palaceClientPtr(),
         connect(newCurrentPalaceClientPtr,
                 &seville::palace::Client::connectionStateDidChangeEvent,
                 this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

//             auto currentPalaceClientPtr =
//                   my_currentPalaceClientWidgetPtr->palaceClientPtr();

         //my_logWidgetPtr->setPalaceClientPtr(my_currentPalaceClientWidgetPtr->palaceClientPtr());
         do_updateMenus();
         my_tabWidgetPtr->adjustSize();
         adjustSize();
         my_logWidgetPtr->setPalaceClientPtr(newCurrentPalaceClientPtr);
      }

//      void MainWindow::on_viewNeedsUpdating(void)
//      {
//         do_updateView();
//      }

      void MainWindow::do_setupWindow(void)
      {
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         setUnifiedTitleAndToolBarOnMac(true);
         // setContentsMargins(0, 0, 0, 0);
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
      }

      void MainWindow::do_setupView(void)
      {
         my_currentPalaceClientWidgetPtr = do_currentPalaceClientWidgetPtr();

         // Set Main Window to Tab Widget
         setCentralWidget(my_tabWidgetPtr);

         //my_mainLayoutPtr = this->layout();

         //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
         //setMinimumSize(10, 10);

         //setContentsMargins(0, menuBar_->height(), 0, 0); // left, top, right, bottom
         //my_mainLayoutPtr = new QVBoxLayout(this);
         //mainLayout_ = new QVBoxLayout; //(this);
         //mainLayout_->setMenuBar(menuBar_);
         //mainLayout_->addWidget(menuBar_, 0, 0);
         //my_toolBarPtr->addWidget(comboBox);

         //my_tabWidgetPtr->setGeometry(0, 0, width(), height()); // x, y, w, h

         //my_mainLayoutPtr = layout();
         //my_mainLayoutPtr->addWidget(my_toolBarPtr);
         //addToolBar(my_toolBarPtr);

         //my_mainLayoutPtr->addWidget(my_toolBarPtr);
         //layout()->addWidget(my_tabWidget_ptr);
         //auto clientWidget_ptr = new view::PalaceClientWidget(this);
         //connect(palRoomWidget, )
         //my_tabWidgetPtr->addNewTab(clientWidget_ptr);

         //mainLayout_->setSpacing(0);
         //mainLayout_->addWidget(tabWidget_, 1);

         //setLayout(mainLayout_);
         //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

         //setStatusBar(my_pStatusBar);
         //my_pStatusBar->showMessage(tr("Ready"));
         //layout()->addWidget(my_pStatusBar);

         //QSettings settings;
         //restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
         //restoreState(settings.value("mainWindowState").toByteArray());
      }

      void MainWindow::do_setupActions(void)
      {
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

      void MainWindow::do_setupMenus(void)
      {
         // Create Menu Bar
         //my_menuBarPtr = new QMenuBar(this);
         my_menuBarPtr = this->menuBar();

         my_fileMenuPtr = my_menuBarPtr->addMenu(tr("&File"));
         my_editMenuPtr = my_menuBarPtr->addMenu(tr("&Edit"));
         my_windowMenuPtr = my_menuBarPtr->addMenu(tr("&Window"));
         my_helpMenuPtr = my_menuBarPtr->addMenu(tr("&Help"));

         // File Menu
         my_fileMenuPtr->addAction(my_openHostConnectionActionPtr);
         my_fileMenuPtr->addAction(my_closeHostConnectionActionPtr);
         my_fileMenuPtr->addSeparator();
         my_fileMenuPtr->addAction(my_newTabActionPtr);
         my_fileMenuPtr->addAction(my_closeTabActionPtr);
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
         my_windowMenuPtr->addAction(my_toggleLogActionPtr);
         //my_windowMenuPtr->addAction(
         //         my_logDockWidgetPtr->toggleViewAction());

         // About Menu
         my_helpMenuPtr->addSeparator();
         my_helpMenuPtr->addAction(my_aboutAppActionPtr);

//         auto comboBoxPtr = new QComboBox(my_fileMenuPtr);
//         comboBoxPtr->addItem("Test1");
//         comboBoxPtr->addItem("Test2");
//         comboBoxPtr->addItem("Test3");
//         auto checkableActionPtr = new QWidgetAction(my_fileMenuPtr);
//         checkableActionPtr->setDefaultWidget(comboBoxPtr);
//         my_fileMenuPtr->addAction(checkableActionPtr);

         //setMenuBar(my_menuBarPtr);
         auto layoutPtr = this->layout();
         layoutPtr->addWidget(my_menuBarPtr);
      }

      void MainWindow::do_setupToolbars(void)
      {
         //my_toolBarPtr->setStyleSheet("border: 1px solid red");
         //my_toolBarPtr->setMovable(false);
         //my_toolBarPtr->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

//         auto comboBoxPtr = new QComboBox; //(my_toolBarPtr);
//         comboBoxPtr->addItem("Test1");
//         comboBoxPtr->addItem("Test2");
//         comboBoxPtr->addItem("Test3");
//         my_toolBarPtr->addWidget(comboBoxPtr);
      }

      void MainWindow::do_setupDocks(void)
      {
         auto palaceClientWidgetPtr =
               static_cast<PalaceClientWidget *>(
                  my_tabWidgetPtr->currentWidget());

            auto palaceClientPtr =
               palaceClientWidgetPtr->palaceClientPtr();

            my_logWidgetPtr->setPalaceClientPtr(palaceClientPtr);

            my_logWidgetPtr->setChatLineEditIsVisible(
                  my_logWidgetPtr->isWindow());

            my_logDockWidgetPtr->setAllowedAreas(
                  Qt::LeftDockWidgetArea |
                  Qt::BottomDockWidgetArea |
                  Qt::RightDockWidgetArea |
                  Qt::TopDockWidgetArea);

            my_logDockWidgetPtr->setWidget(my_logWidgetPtr);
            addDockWidget(Qt::RightDockWidgetArea, my_logDockWidgetPtr);
        }

        void MainWindow::do_setupEvents(void)
        {
            // Connect Action Signals to Slots
            connect(my_logDockWidgetPtr, &QDockWidget::topLevelChanged,
                    this, &seville::view::MainWindow::on_topLevelDidChange);

            connect(my_openHostConnectionActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_openConnectionWasRequested);

            connect(my_closeHostConnectionActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_closeConnectionWasRequested);

            connect(my_newTabActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_newTabRequested);

            connect(my_closeTabActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_closeTabRequested);

            connect(my_quitAppActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_quitAppWasRequested);

            connect(my_aboutAppActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_aboutAppWasRequested);

            connect(my_toggleLogActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_logWindowWasToggled);

            connect(my_tabWidgetPtr, &seville::view::TabWidget::currentChanged,
                    this, &seville::view::MainWindow::on_currentTabDidChange);

            connect(my_tabWidgetPtr,
                    &seville::view::TabWidget::tabWasAddedWithWidgetPtrEvent,
                    this,
                    &seville::view::MainWindow::on_tabWasAddedWithClientWidgetPtr);

            // connect(my_tabWidgetPtr,
            //         &seville::view::TabWidget::tabWasRemovedWithWidgetPtrEvent,
            //         this,
            //         &seville::view::MainWindow::on_tabWasRemovedWithClientWidgetPtr);

            connect(// my_tabWidgetPtr->currentPalaceClientPtr(),
                    do_currentPalaceClientWidgetPtr()->palaceClientPtr(),
                    &seville::palace::Client::connectionStateDidChangeEvent,
                    this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

            // auto currentPalaceClientWidgetPtr = do_currentPalaceClientWidgetPtr();
            // connect(currentPalaceClientWidgetPtr,
            //         &seville::view::PalaceClientWidget::widgetBackgroundDidChangeEvent,
            //         this, &seville::view::MainWindow::on_currentClientWidgetBackgroundImageDidLoad);

            // auto palaceClientPtr = my_tabWidgetPtr->currentPalaceClientPtr();
            // auto palaceClient0Ptr = qobject_cast<view::PalaceClientWidget*>(
            //          my_tabWidgetPtr->widget(0));
            // if (palaceClient0Ptr != nullptr) {
            //    connect(palaceClient0Ptr,
            //            &seville::view::PalaceClientWidget::
            //            connectionStateDidChangeEvent,
            //            this,
            //            &seville::view::MainWindow::on_connectionStateDidChange);
            // }
          }

          void MainWindow::do_teardownEvents(void)
          {
             // Disconnect Action Signals from Slots

             // auto palaceClientPtr = my_tabWidgetPtr->currentPalaceClientPtr();
             // disconnect(palaceClientPtr,
             //         &seville::view::PalaceClientWidget::
             //            connectionStateDidChangeEvent,
             //         this, &seville::view::MainWindow::on_connectionStateDidChange);
             // disconnect(my_currentPalaceClientWidgetPtr,
             //          &seville::view::PalaceClientWidget::widgetBackgroundDidChangeEvent,
             //          this, &seville::view::MainWindow::on_currentClientWidgetBackgroundImageDidLoad);

             //disconnect(my_tabWidgetPtr->currentPalaceClientPtr(),
             disconnect(do_currentPalaceClientWidgetPtr()->palaceClientPtr(),
                      &seville::palace::Client::connectionStateDidChangeEvent,
                      this, &seville::view::MainWindow::on_currentClientConnectionStateDidChange);

             disconnect(my_logDockWidgetPtr, &QDockWidget::topLevelChanged,
                     this, &seville::view::MainWindow::on_topLevelDidChange);

             // disconnect(my_tabWidgetPtr,
             //            &seville::view::TabWidget::tabWasRemovedWithWidgetPtrEvent,
             //            this,
             //            &seville::view::MainWindow::on_tabWasRemovedWithClientWidgetPtr);

            disconnect(my_tabWidgetPtr,
                    &seville::view::TabWidget::tabWasAddedWithWidgetPtrEvent,
                    this, &seville::view::MainWindow::on_tabWasAddedWithClientWidgetPtr);

            disconnect(my_tabWidgetPtr, &seville::view::TabWidget::currentChanged,
                    this, &seville::view::MainWindow::on_currentTabDidChange);

            disconnect(my_openHostConnectionActionPtr, &QAction::triggered,
                    this,
                    &seville::view::MainWindow::on_openConnectionWasRequested);

            disconnect(my_closeHostConnectionActionPtr, &QAction::triggered,
                    this,
                    &seville::view::MainWindow::on_closeConnectionWasRequested);

            disconnect(my_newTabActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_newTabRequested);

            disconnect(my_closeTabActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_closeTabRequested);

            disconnect(my_quitAppActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_quitAppWasRequested);

            disconnect(my_aboutAppActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_aboutAppWasRequested);

            disconnect(my_toggleLogActionPtr, &QAction::triggered,
                    this, &seville::view::MainWindow::on_logWindowWasToggled);
        }

        void MainWindow::do_setupSizing(void)
        {
            //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

            //my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);
            //layout()->setSpacing(0);
            //layout()->setMargin(0);
            //layout()->setContentsMargins(0, 0, 0, 0);

            window()->adjustSize();

            centralWidget()->updateGeometry();
            centralWidget()->adjustSize();

            updateGeometry();
            adjustSize();
        }

        void MainWindow::do_updateMenus(void)
        {
            //auto palaceClientPtr = my_tabWidgetPtr->currentPalaceClientPtr();
            auto currentPalaceClientWidgetPtr = my_currentPalaceClientWidgetPtr;
            if (currentPalaceClientWidgetPtr == nullptr) {
                return;
            }

            auto currentPalaceClientPtr =
                    currentPalaceClientWidgetPtr->palaceClientPtr();

            if (currentPalaceClientPtr == nullptr) {
                return;
            }

            auto connectionState = currentPalaceClientPtr->connectionState();
            switch (connectionState) {
                case palace::ConnectionState::kConnectedState:
                    my_openHostConnectionActionPtr->setEnabled(false);
                    my_closeHostConnectionActionPtr->setEnabled(true);
                    break;
                case palace::ConnectionState::kHandshakingState:
                    my_openHostConnectionActionPtr->setEnabled(false);
                    my_closeHostConnectionActionPtr->setEnabled(false);
                    break;
                default:
                    my_openHostConnectionActionPtr->setEnabled(true);
                    my_closeHostConnectionActionPtr->setEnabled(false);
            }
        }

        void MainWindow::do_updateView(void)
        {
            do_updateMenus();

            my_currentPalaceClientWidgetPtr->update();
            // my_currentPalaceClientWidgetPtr->drawRoom();
        }

        void MainWindow::do_init(void)
        {
            // Create Actions
            my_openHostConnectionActionPtr = new QAction(tr("&Connect"), this);
            my_closeHostConnectionActionPtr = new QAction(tr("&Disconnect"), this);
            my_newTabActionPtr = new QAction(tr("&New Tab"), this);
            my_closeTabActionPtr = new QAction(tr("&Close Tab"), this);
            my_quitAppActionPtr = new QAction(tr("E&xit"), this);
            my_undoContentActionPtr = new QAction(tr("&Undo"), this);
            my_redoContentActionPtr = new QAction(tr("&Redo"), this);
            my_cutContentActionPtr = new QAction(tr("&Cut"), this);
            my_copyContentActionPtr = new QAction(tr("&Copy"), this);
            my_pasteContentActionPtr = new QAction(tr("&Paste"), this);
            my_aboutAppActionPtr = new QAction(tr("&About Seville"), this);
            my_toggleLogActionPtr = new QAction(tr("&Log"), this);

            my_toolBarPtr = new QToolBar(); //my_toolBarPtr->addToolBar("Rooms");
            my_tabBarPtr = new QTabBar(this);

            my_logWidgetPtr = new view::LogWidget(this);
            my_logDockWidgetPtr = new QDockWidget(tr("Log"), this);

            my_tabWidgetPtr = new view::TabWidget(this);
            my_statusBarPtr = new QStatusBar(this);

            do_setupWindow();

            do_setupActions();
            do_setupMenus();
            do_setupToolbars();

            do_setupEvents();
            do_setupView();
            do_setupDocks();

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

        void MainWindow::do_deinit(void)
        {
            do_teardownEvents();

            //if (my_logWidgetPtr != nullptr)
            //   delete(my_widget_log_ptr_);
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

//         if (my_openHostConnectionActionPtr != nullptr) {
//            delete my_openHostConnectionActionPtr;
//            my_openHostConnectionActionPtr = nullptr;
//         }

//         if (my_closeHostConnectionActionPtr != nullptr) {
//            delete my_closeHostConnectionActionPtr;
//            my_closeHostConnectionActionPtr = nullptr;
//         }

//         if (my_newTabActionPtr != nullptr) {
//            delete my_newTabActionPtr;
//            my_newTabActionPtr = nullptr;
//         }

//         if (my_closeTabActionPtr != nullptr) {
//            delete my_closeTabActionPtr;
//            my_closeTabActionPtr = nullptr;
//         }

//         if (my_quitAppActionPtr != nullptr) {
//            delete my_quitAppActionPtr;
//            my_quitAppActionPtr = nullptr;
//         }

//         if (my_undoContentActionPtr != nullptr) {
//            delete my_undoContentActionPtr;
//            my_undoContentActionPtr = nullptr;
//         }

//         if (my_redoContentActionPtr != nullptr) {
//            delete my_redoContentActionPtr;
//            my_redoContentActionPtr = nullptr;
//         }

//         if (my_cutContentActionPtr != nullptr) {
//            delete my_cutContentActionPtr;
//            my_cutContentActionPtr = nullptr;
//         }

//         if (my_copyContentActionPtr != nullptr) {
//            delete my_copyContentActionPtr;
//            my_copyContentActionPtr = nullptr;
//         }

//         if (my_pasteContentActionPtr != nullptr) {
//            delete my_pasteContentActionPtr;
//            my_pasteContentActionPtr = nullptr;
//         }

//         if (my_aboutAppActionPtr != nullptr) {
//            delete my_aboutAppActionPtr;
//            my_aboutAppActionPtr = nullptr;
//         }

//         if (my_toggleLogActionPtr != nullptr) {
//            delete my_toggleLogActionPtr;
//            my_toggleLogActionPtr = nullptr;
//         }

//         if (my_toolBarPtr != nullptr) {
//            delete my_toolBarPtr;
//            my_toolBarPtr = nullptr;
//         }

//         if (my_logWidgetPtr != nullptr) {
//            delete my_logWidgetPtr;
//            my_logWidgetPtr = nullptr;
//         }

//         if (my_logDockWidgetPtr != nullptr) {
//            delete my_logDockWidgetPtr;
//            my_logDockWidgetPtr = nullptr;
//         }

//         if (my_tabWidgetPtr != nullptr) {
//            delete my_tabWidgetPtr;
//            my_tabWidgetPtr = nullptr;
//         }

//         if (my_statusBarPtr != nullptr) {
//            delete my_statusBarPtr;
//            my_statusBarPtr = nullptr;
//         }
      }

      PalaceClientWidget* MainWindow::do_currentPalaceClientWidgetPtr(void)
      {
          auto palaceClientWidgetPtr =
                static_cast<PalaceClientWidget *>(
                   my_tabWidgetPtr->currentWidget());
          return palaceClientWidgetPtr;
      }

      void MainWindow::closeEvent(QCloseEvent* closeEventPtr)
      {
         (void)closeEventPtr;
         QSettings settings;
         settings.setValue("mainWindowGeometry", saveGeometry());
         settings.setValue("mainWindowState", saveState());
         do_deinit();
         QApplication::quit();
      }
   }
}
