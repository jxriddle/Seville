#ifndef SEVILLE_VIEW_MAIN_WINDOW_H_
#define SEVILLE_VIEW_MAIN_WINDOW_H_

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

#include "seville/palace/client.h"
#include "seville/view/aboutdialog.h"
#include "seville/view/connectdialog.h"
#include "seville/view/palaceclientwidget.h"
#include "seville/view/logwidget.h"
#include "seville/view/tabwidget.h"

namespace seville
{
   namespace view
   {
      class MainWindow : public QMainWindow
      {
      Q_OBJECT

      public:
         explicit MainWindow(QWidget* parentWidgetPtr = nullptr);
         //virtual ~MainWindow(void);

         inline view::TabWidget* tabWidgetPtr(void) {
            return my_tabWidgetPtr;
         }

         inline QDockWidget* logDockWidget(void) {
            return my_logDockWidgetPtr;
         }

         inline LogWidget* logWidgetPtr(void) {
            return my_logWidgetPtr;
         }

      public slots:
         void on_changeTopLevelEvent(bool shouldBeVisibleFlag);
         void on_openHostConnectionEvent(void);
         void on_closeHostConnectionEvent(void);
         void on_quitAppEvent(void);
         void on_aboutAppEvent(void);
         void on_toggleLogWindowEvent(void);
         //void on_client_widget_background_did_change(void);
         //void on_clientWidgetDidResize(int width, int height);
         void on_clientConnectionStateDidChange(
               palace::ConnectionState connectionState);

      protected:
         //void resizeEvent(QResizeEvent* resizeEventPtr) override;
         void closeEvent(QCloseEvent* closeEventPtr) override;

      private:
         view::TabWidget* my_tabWidgetPtr;
         QDockWidget* my_logDockWidgetPtr;
         view::LogWidget* my_logWidgetPtr;
         QLayout* my_mainLayoutPtr;
         QTabBar* my_tabBarPtr;
         QMenuBar* my_menuBarPtr;
         QMenu* my_fileMenuPtr;
         QMenu* my_editMenuPtr;
         QMenu* my_windowMenuPtr;
         QMenu* my_helpMenuPtr;
         QAction* my_openHostConnectionActionPtr;
         QAction* my_closeHostConnectionActionPtr;
         QAction* my_undoContentActionPtr;
         QAction* my_redoContentActionPtr;
         QAction* my_cutContentActionPtr;
         QAction* my_copyContentActionPtr;
         QAction* my_pasteContentActionPtr;
         QAction* my_quitAppActionPtr;
         QAction* my_aboutAppActionPtr;
         QAction* my_toggleLogActionPtr;
         QStatusBar* my_statusBarPtr;

         auto do_setupView(void) -> void;
         auto do_setupActions(void) -> void;
         auto do_setupMenus(void) -> void;
         auto do_setupDocks(void) -> void;
         auto do_setupEvents(void) -> void;
         auto do_setupSizing(void) -> void;

         auto do_init(void) -> void;
      };
   }
}

#endif // SEVILLE_VIEW_MAIN_WINDOW_H_
