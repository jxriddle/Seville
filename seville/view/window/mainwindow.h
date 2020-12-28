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
            void on_top_level_changed(bool flag_should_be_visible);
            void on_new_host_connection_action_triggered(void);
            void on_close_host_connection_action_triggered(void);
            void on_quit_app_action_triggered(void);
            void on_about_app_action_triggered(void);
            void on_toggle_log_window_action_triggered(void);
            //void on_client_widget_background_did_change(void);
            void on_client_widget_did_resize(int width, int height);

         private:
            view::widget::TabWidget* my_tab_widget_ptr_;
            QDockWidget* my_dock_log_widget_ptr_;
            view::widget::LogWidget* my_log_widget_ptr_;
            QVBoxLayout* my_layout_main_ptr_;
            QTabBar* my_tab_bar_ptr_;
            QMenuBar* my_menu_bar_ptr_;
            QMenu* my_file_menu_ptr_;
            QMenu* my_edit_menu_ptr_;
            QMenu* my_window_menu_ptr_;
            QMenu* my_help_menu_ptr_;
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
            QStatusBar* my_status_bar_ptr_;

            auto do_setup_view_(void) -> void;
            auto do_setup_actions_(void) -> void;
            auto do_setup_menus_(void) -> void;
            auto do_setup_docks_(void) -> void;
            auto do_setup_events_(void) -> void;
            auto do_init_(void) -> void;

         protected:
            void resizeEvent(QResizeEvent* event_ptr) override;
            void closeEvent(QCloseEvent* event_ptr) override;
         };
      }
   }
}

#endif // SEVILLE_VIEW_WINDOW_MAINWINDOW_H_
