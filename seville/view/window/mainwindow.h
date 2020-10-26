#ifndef SEVILLE_VIEW_WINDOW_MAINWINDOW_H_
#define SEVILLE_VIEW_WINDOW_MAINWINDOW_H_

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
      namespace window
      {
         class MainWindow : public QMainWindow
         {
         Q_OBJECT

         public:
            explicit MainWindow(QWidget* widget_parent_ptr = nullptr);
            //virtual ~MainWindow(void) override;

         public slots:
            void on_top_level_changed(bool flag_should_be_visible)
            {
               my_widget_log_ptr_->set_line_edit_chat_visible(
                        flag_should_be_visible);
            }

            void on_new_host_connection_action_triggered(void)
            {
               auto widget_client_active_ptr =
                     static_cast<widget::ClientWidget *>(
                        my_widget_tab_ptr_->currentWidget());

               //connect(palTabWidget, resize);
               widget_client_active_ptr->prompt_new_connection();
            }

            void on_close_host_connection_action_triggered(void)
            {
               // if last tab is closed, quit application
               if (my_widget_tab_ptr_->count() <= 2) {
                  QApplication::quit();
               }

               my_widget_tab_ptr_->removeTab(
                        my_widget_tab_ptr_->currentIndex());
            }

            void on_quit_app_action_triggered(void)
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

            void on_about_app_action_triggered(void)
            {
               auto dialog_about = new dialog::AboutDialog(this);
               dialog_about->exec();
            }

            void on_toggle_log_window_action_triggered(void)
            {
               if (!my_widget_log_ptr_->isVisible())
                  my_widget_log_ptr_->show();
               else
                  my_widget_log_ptr_->hide();
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

         private:
            view::widget::TabWidget* my_widget_tab_ptr_;
            QDockWidget* my_widget_dock_log_ptr_;
            view::widget::LogWidget* my_widget_log_ptr_;
            QVBoxLayout* my_layout_main_ptr_;
            QTabBar* my_tabbar_ptr_;
            QMenuBar* my_menubar_ptr_;
            QMenu* my_menu_file_ptr_;
            QMenu* my_menu_edit_ptr_;
            QMenu* my_menu_window_ptr_;
            QMenu* my_menu_help_ptr_;
            QAction* my_action_new_host_connection_ptr_;
            QAction* my_action_close_host_connection_ptr_;
            QAction* my_action_undo_content_ptr_;
            QAction* my_action_redo_content_ptr_;
            QAction* my_action_cut_content_ptr_;
            QAction* my_action_copy_content_ptr_;
            QAction* my_action_paste_content_ptr_;
            QAction* my_action_quit_app_ptr_;
            QAction* my_action_about_app_ptr_;
            QAction* my_action_toggle_log_ptr_;
            QStatusBar* my_statusbar_ptr_;

            auto do_setup_view_(void) -> void
            {
               // Window
               setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
               setUnifiedTitleAndToolBarOnMac(true);

               QScreen* primary_screen_ptr = QGuiApplication::primaryScreen();
               auto screen = primary_screen_ptr->geometry();

               QSize window_size_default = QSize(
                        static_cast<int>(.67*screen.width()),
                        static_cast<int>(.67*screen.height()));

               setGeometry(QStyle::alignedRect(
                              Qt::LeftToRight,
                              Qt::AlignCenter,
                              window_size_default,
                              primary_screen_ptr->geometry()));

               setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
               adjustSize();

               //setContentsMargins(0, menuBar_->height(), 0, 0); // left, top, right, bottom
               //setContentsMargins(0, 0, 0, 0);
               //mainLayout_ = new QVBoxLayout; //(this);
               //mainLayout_->setMenuBar(menuBar_);
               //mainLayout_->addWidget(menuBar_, 0, 0);

               // Tab Widget
               my_widget_tab_ptr_ = new view::widget::TabWidget(this);
               my_widget_tab_ptr_->setGeometry(0, 0, width(), height()); // x, y, w, h
               layout()->setSpacing(0);
               //layout()->addWidget(tabWidget_);
               auto widget_client_ptr = new view::widget::ClientWidget(this);
               //connect(palRoomWidget, )
               my_widget_tab_ptr_->add_new_tab(widget_client_ptr);
               //mainLayout_->setSpacing(0);
               //mainLayout_->addWidget(tabWidget_, 1);

               //setLayout(mainLayout_);
               //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

               my_statusbar_ptr_ = new QStatusBar(this);
               //setStatusBar(my_pStatusBar);
               //my_pStatusBar->showMessage(tr("Ready"));
               //layout()->addWidget(my_pStatusBar);

               // Set Main Window to Tab Widget
               setCentralWidget(my_widget_tab_ptr_);

               QSettings settings;
               restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
               restoreState(settings.value("mainWindowState").toByteArray());
            }

            auto do_setup_actions_(void) -> void
            {
               // Create Actions
               my_action_new_host_connection_ptr_ = new QAction(tr("&Connect"), this);
               my_action_close_host_connection_ptr_ = new QAction(tr("&Disconnect"), this);
               my_action_quit_app_ptr_ = new QAction(tr("E&xit"), this);
               my_action_undo_content_ptr_ = new QAction(tr("&Undo"), this);
               my_action_redo_content_ptr_ = new QAction(tr("&Redo"), this);
               my_action_cut_content_ptr_ = new QAction(tr("&Cut"), this);
               my_action_copy_content_ptr_ = new QAction(tr("&Copy"), this);
               my_action_paste_content_ptr_ = new QAction(tr("&Paste"), this);
               my_action_about_app_ptr_ = new QAction(tr("&About Seville"), this);
               my_action_toggle_log_ptr_ = new QAction(tr("&Log"), this);

               // Assign Action Shortcut Keys
               my_action_new_host_connection_ptr_->setShortcut(QKeySequence::New);
               my_action_close_host_connection_ptr_->setShortcut(Qt::CTRL | Qt::Key_W);
               my_action_close_host_connection_ptr_->setEnabled(false);
               my_action_quit_app_ptr_->setShortcut(QKeySequence::Quit);
               my_action_undo_content_ptr_->setShortcut(QKeySequence::Undo);
               my_action_redo_content_ptr_->setShortcut(QKeySequence::Redo);
               my_action_cut_content_ptr_->setShortcut(QKeySequence::Cut);
               my_action_copy_content_ptr_->setShortcut(QKeySequence::Copy);
               my_action_paste_content_ptr_->setShortcut(QKeySequence::Paste);
               my_action_toggle_log_ptr_->setShortcut(Qt::CTRL | Qt::Key_L);
            }

            auto do_setup_menus_(void) -> void
            {
               // Create Menu Bar
               my_menubar_ptr_ = new QMenuBar(this);
               my_menu_file_ptr_ = my_menubar_ptr_->addMenu(tr("&File"));
               my_menu_edit_ptr_ = my_menubar_ptr_->addMenu(tr("&Edit"));
               my_menu_window_ptr_ = my_menubar_ptr_->addMenu(tr("&Window"));
               my_menu_help_ptr_ = my_menubar_ptr_->addMenu(tr("&Help"));

               // File Menu
               my_menu_file_ptr_->addAction(my_action_new_host_connection_ptr_);
               my_menu_file_ptr_->addAction(my_action_close_host_connection_ptr_);
               my_menu_file_ptr_->addSeparator();
               my_menu_file_ptr_->addAction(my_action_quit_app_ptr_);

               // Edit Menu
               my_menu_edit_ptr_->addAction(my_action_undo_content_ptr_);
               my_menu_edit_ptr_->addAction(my_action_redo_content_ptr_);
               my_menu_edit_ptr_->addSeparator();
               my_menu_edit_ptr_->addAction(my_action_cut_content_ptr_);
               my_menu_edit_ptr_->addAction(my_action_copy_content_ptr_);
               my_menu_edit_ptr_->addAction(my_action_paste_content_ptr_);
               my_menu_edit_ptr_->addSeparator();

               // Window Menu
               //myWindowMenu->addAction(myToggleLogAction);
               my_menu_window_ptr_->addAction(my_widget_dock_log_ptr_->toggleViewAction());

               // About Menu
               my_menu_help_ptr_->addSeparator();
               my_menu_help_ptr_->addAction(my_action_about_app_ptr_);

               setMenuBar(my_menubar_ptr_);
            }

            auto do_setup_docks_(void) -> void
            {
               auto widget_palace_client_ptr =
                     static_cast<widget::ClientWidget *>(
                        my_widget_tab_ptr_->currentWidget());
               auto palace_client_ptr =
                     widget_palace_client_ptr->palace_client_ptr();

               my_widget_log_ptr_ = new view::widget::LogWidget(this);
               my_widget_log_ptr_->set_palace_client_ptr(palace_client_ptr);

               my_widget_log_ptr_->set_line_edit_chat_visible(
                        my_widget_log_ptr_->isWindow());

               my_widget_dock_log_ptr_ = new QDockWidget(tr("Log"), this);
               my_widget_dock_log_ptr_->setAllowedAreas(
                        Qt::LeftDockWidgetArea |
                        Qt::BottomDockWidgetArea |
                        Qt::RightDockWidgetArea |
                        Qt::TopDockWidgetArea);

               my_widget_dock_log_ptr_->setWidget(my_widget_log_ptr_);
               addDockWidget(Qt::RightDockWidgetArea, my_widget_dock_log_ptr_);
            }

            auto do_setup_events_(void) -> void
            {
               // Connect Action Signals to Slots
               connect(my_widget_dock_log_ptr_, &QDockWidget::topLevelChanged,
                       this, &seville::view::window::MainWindow::on_top_level_changed);

               connect(my_action_new_host_connection_ptr_, &QAction::triggered,
                       this, &seville::view::window::MainWindow::on_new_host_connection_action_triggered);

               connect(my_action_close_host_connection_ptr_, &QAction::triggered,
                       this, &seville::view::window::MainWindow::on_close_host_connection_action_triggered);

               connect(my_action_quit_app_ptr_, &QAction::triggered,
                       this, &seville::view::window::MainWindow::on_quit_app_action_triggered);

               connect(my_action_about_app_ptr_, &QAction::triggered,
                       this, &seville::view::window::MainWindow::on_about_app_action_triggered);

               connect(my_action_toggle_log_ptr_, &QAction::triggered,
                       this, &seville::view::window::MainWindow::on_toggle_log_window_action_triggered);
            }

            auto do_init_(void) -> void
            {
               do_setup_view_();
               do_setup_actions_();
               do_setup_docks_();
               do_setup_menus_();
               do_setup_events_();
            }

         protected:
            void resizeEvent(QResizeEvent* event_ptr) override
            {
               //menuBar_->setGeometry(0, 0, width(), menuBar_->height());
               //tabWidget_->setGeometry(0, 0, width(), height());
               //tabWidget_->resize();

               //PalRoomWidget* palRoomWidget = static_cast<PalRoomWidget*>(myTabWidget->currentWidget());
               //palRoomWidget->resize(event->size());
               QMainWindow::resizeEvent(event_ptr);
            }

            void closeEvent(QCloseEvent* event_ptr) override
            {
               (void)event_ptr;
               QSettings settings;
               settings.setValue("mainWindowGeometry", saveGeometry());
               settings.setValue("mainWindowState", saveState());
               QApplication::quit();
            }
         };
      }
   }
}

#endif // SEVILLE_VIEW_WINDOW_MAINWINDOW_H_
