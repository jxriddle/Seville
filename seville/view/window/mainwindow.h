#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLayout>
#include <QMessageBox>
#include <QSizePolicy>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>
#include <QScreen>

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QScrollArea>
#include <QStatusBar>
#include <QTabBar>
#include <QTabWidget>

#include "seville/view/dialog/aboutdialog.h"
#include "seville/view/dialog/connectdialog.h"

#include "seville/view/widget/clientwidget.h"
#include "seville/view/widget/logwidget.h"
#include "seville/view/widget/tabwidget.h"

namespace seville
{
   namespace view
   {
      class MainWindow : public QMainWindow
      {
      Q_OBJECT

      private:
         AppTabWidget* my_widget_tab_pointer_;
         QDockWidget* my_widget_dock_log_pointer_;
         LogWidget* my_widget_log_pointer_;
         QVBoxLayout* my_layout_main_pointer_;
         QTabBar* my_tabbar_pointer_;
         QMenuBar* my_menubar_pointer_;
         QMenu* my_menu_file_pointer_;
         QMenu* my_menu_edit_pointer_;
         QMenu* my_menu_window_pointer_;
         QMenu* my_menu_help_pointer_;
         QAction* my_action_new_host_connection_pointer_;
         QAction* my_action_close_host_connection_pointer_;
         QAction* my_action_undo_content_pointer_;
         QAction* my_action_redo_content_pointer_;
         QAction* my_action_cut_content_pointer_;
         QAction* my_action_copy_content_pointer_;
         QAction* my_action_paste_content_pointer_;
         QAction* my_action_quit_app_pointer_;
         QAction* my_action_about_app_pointer_;
         QAction* my_action_toggle_log_pointer_;
         QStatusBar* my_statusbar_pointer_;

         void do_setupView(void)
         {
            // Window
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            setUnifiedTitleAndToolBarOnMac(true);

            QScreen *primaryScreen = QGuiApplication::primaryScreen();
            auto screen = primaryScreen->geometry();

            QSize defaultWindowSize = QSize( \
                     static_cast<int>(.67*screen.width()), \
                     static_cast<int>(.67*screen.height()));

            setGeometry(QStyle::alignedRect( \
                           Qt::LeftToRight, \
                           Qt::AlignCenter, \
                           defaultWindowSize, \
                           primaryScreen->geometry()));

            setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            adjustSize();

            //setContentsMargins(0, menuBar_->height(), 0, 0); // left, top, right, bottom
            //setContentsMargins(0, 0, 0, 0);
            //mainLayout_ = new QVBoxLayout; //(this);
            //mainLayout_->setMenuBar(menuBar_);
            //mainLayout_->addWidget(menuBar_, 0, 0);

            // Tab Widget
            my_widget_tab_pointer_ = new AppTabWidget(this);
            my_widget_tab_pointer_->setGeometry(0, 0, width(), height()); // x, y, w, h
            layout()->setSpacing(0);
            //layout()->addWidget(tabWidget_);
            auto widget_client_pointer = new ClientWidget(this);
            //connect(palRoomWidget, )
            my_widget_tab_pointer_->addNewTab(widget_client_pointer);
            //mainLayout_->setSpacing(0);
            //mainLayout_->addWidget(tabWidget_, 1);

            //setLayout(mainLayout_);
            //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

            my_statusbar_pointer_ = new QStatusBar(this);
            //setStatusBar(my_pStatusBar);
            //my_pStatusBar->showMessage(tr("Ready"));
            //layout()->addWidget(my_pStatusBar);

            // Set Main Window to Tab Widget
            setCentralWidget(my_widget_tab_pointer_);

            QSettings settings;
            restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
            restoreState(settings.value("mainWindowState").toByteArray());
         }

         void do_setupActions(void)
         {
            // Create Actions
            my_action_new_host_connection_pointer_ = new QAction(tr("&Connect"), this);
            my_action_close_host_connection_pointer_ = new QAction(tr("&Disconnect"), this);
            my_action_quit_app_pointer_ = new QAction(tr("E&xit"), this);
            my_action_undo_content_pointer_ = new QAction(tr("&Undo"), this);
            my_action_redo_content_pointer_ = new QAction(tr("&Redo"), this);
            my_action_cut_content_pointer_ = new QAction(tr("&Cut"), this);
            my_action_copy_content_pointer_ = new QAction(tr("&Copy"), this);
            my_action_paste_content_pointer_ = new QAction(tr("&Paste"), this);
            my_action_about_app_pointer_ = new QAction(tr("&About Seville"), this);
            my_action_toggle_log_pointer_ = new QAction(tr("&Log"), this);

            // Assign Action Shortcut Keys
            my_action_new_host_connection_pointer_->setShortcut(QKeySequence::New);
            my_action_close_host_connection_pointer_->setShortcut(Qt::CTRL | Qt::Key_W);
            my_action_close_host_connection_pointer_->setEnabled(false);
            my_action_quit_app_pointer_->setShortcut(QKeySequence::Quit);
            my_action_undo_content_pointer_->setShortcut(QKeySequence::Undo);
            my_action_redo_content_pointer_->setShortcut(QKeySequence::Redo);
            my_action_cut_content_pointer_->setShortcut(QKeySequence::Cut);
            my_action_copy_content_pointer_->setShortcut(QKeySequence::Copy);
            my_action_paste_content_pointer_->setShortcut(QKeySequence::Paste);
            my_action_toggle_log_pointer_->setShortcut(Qt::CTRL | Qt::Key_L);
         }

         void do_setupMenus(void)
         {
            // Create Menu Bar
            my_menubar_pointer_ = new QMenuBar(this);
            my_menu_file_pointer_ = my_menubar_pointer_->addMenu(tr("&File"));
            my_menu_edit_pointer_ = my_menubar_pointer_->addMenu(tr("&Edit"));
            my_menu_window_pointer_ = my_menubar_pointer_->addMenu(tr("&Window"));
            my_menu_help_pointer_ = my_menubar_pointer_->addMenu(tr("&Help"));

            // File Menu
            my_menu_file_pointer_->addAction(my_action_new_host_connection_pointer_);
            my_menu_file_pointer_->addAction(my_action_close_host_connection_pointer_);
            my_menu_file_pointer_->addSeparator();
            my_menu_file_pointer_->addAction(my_action_quit_app_pointer_);

            // Edit Menu
            my_menu_edit_pointer_->addAction(my_action_undo_content_pointer_);
            my_menu_edit_pointer_->addAction(my_action_redo_content_pointer_);
            my_menu_edit_pointer_->addSeparator();
            my_menu_edit_pointer_->addAction(my_action_cut_content_pointer_);
            my_menu_edit_pointer_->addAction(my_action_copy_content_pointer_);
            my_menu_edit_pointer_->addAction(my_action_paste_content_pointer_);
            my_menu_edit_pointer_->addSeparator();

            // Window Menu
            //myWindowMenu->addAction(myToggleLogAction);
            my_menu_window_pointer_->addAction(my_widget_dock_log_pointer_->toggleViewAction());

            // About Menu
            my_menu_help_pointer_->addSeparator();
            my_menu_help_pointer_->addAction(my_action_about_app_pointer_);

            setMenuBar(my_menubar_pointer_);
         }

         void do_setupDocks(void)
         {
            auto widget_palace_client_pointer = \
                  static_cast<ClientWidget *>( \
                     my_widget_tab_pointer_->currentWidget());
            auto palace_client_pointer = \
                  widget_palace_client_pointer->palaceClientPointer();

            my_widget_log_pointer_ = new PalaceLogWidget(pPalaceClient, this);
            my_widget_log_pointer_->setLineEditChatVisible( \
                     my_widget_log_pointer_->isWindow());

            my_widget_dock_log_pointer_ = new QDockWidget(tr("Log"), this);
            my_widget_dock_log_pointer_->setAllowedAreas( \
                     Qt::LeftDockWidgetArea | \
                     Qt::BottomDockWidgetArea | \
                     Qt::RightDockWidgetArea | \
                     Qt::TopDockWidgetArea \
                  );

            my_widget_dock_log_pointer_->setWidget(my_widget_log_pointer_);
            addDockWidget(Qt::RightDockWidgetArea, my_widget_dock_log_pointer_);
         }

         void do_setupEvents(void)
         {
            // Connect Action Signals to Slots
            connect(my_widget_dock_log_pointer_, &QDockWidget::topLevelChanged,
                    this, &seville::view::MainWindow::on_topLevelChanged);

            connect(my_action_new_host_connection_pointer_, &QAction::triggered,
                    this, &seville::view::MainWindow::on_newHostConnectionActionTriggered);

            connect(my_action_close_host_connection_pointer_, &QAction::triggered,
                    this, &seville::view::MainWindow::on_closeHostConnectionActionTriggered);

            connect(my_action_quit_app_pointer_, &QAction::triggered,
                    this, &seville::view::MainWindow::on_quitAppActionTriggered);

            connect(my_action_about_app_pointer_, &QAction::triggered,
                    this, &seville::view::MainWindow::on_aboutAppActionTriggered);

            connect(my_action_toggle_log_pointer_, &QAction::triggered,
                    this, &seville::view::MainWindow::on_toggleLogWindowActionTriggered);
         }

      protected:
         void resizeEvent(QResizeEvent* pEvent) override
         {
            //menuBar_->setGeometry(0, 0, width(), menuBar_->height());
            //tabWidget_->setGeometry(0, 0, width(), height());
            //tabWidget_->resize();

            //PalRoomWidget* palRoomWidget = static_cast<PalRoomWidget*>(myTabWidget->currentWidget());
            //palRoomWidget->resize(event->size());
            QMainWindow::resizeEvent(pEvent);
         }

         void closeEvent(QCloseEvent* pEvent) override
         {
            (void)pEvent;
            QSettings settings;
            settings.setValue("mainWindowGeometry", saveGeometry());
            settings.setValue("mainWindowState", saveState());
            QApplication::quit();
         }

      public slots:
         void on_topLevelChanged(bool shouldBeVisible)
         {
            my_widget_log_pointer_->setLineEditChatVisible(shouldBeVisible);
         }

         void on_newHostConnectionActionTriggered(void)
         {
            auto widget_client_active_pointer = \
                  static_cast<PalaceClientWidget *>( \
                     my_widget_tab_pointer_->currentWidget());

            //connect(palTabWidget, resize);
            pActiveClientWidget->promptNewConnection();
         }

         void on_closeHostConnectionActionTriggered(void)
         {
            // if last tab is closed, quit application
            if (my_widget_tab_pointer_->count() <= 2) {
               QApplication::quit();
            }

            my_widget_tab_pointer_->removeTab(my_widget_tab_pointer_->currentIndex());
         }

         void on_quitAppActionTriggered(void)
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

         void on_aboutAppActionTriggered(void)
         {
            auto aboutDialog = new AboutDialog(this);
            aboutDialog->exec();
         }

         void on_toggleLogWindowActionTriggered(void)
         {
            if (!my_widget_log_pointer_->isVisible())
               my_widget_log_pointer_->show();
            else
               my_widget_log_pointer_->hide();
         }

         void do_deinit(void)
         {
            if (my_widget_log_pointer_ != nullptr)
               delete(my_widget_log_pointer_);
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

         void do_init(void)
         {
            do_setupView();
            do_setupActions();
            do_setupDocks();
            do_setupMenus();
            do_setupEvents();
         }

      public:
         virtual ~MainWindow(void) override;
         explicit MainWindow(QWidget* pParentWidget = nullptr);
      };
   }
}

#endif // UI_MAINWINDOW_H_
