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
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QMessageBox>
#include <QWidgetAction>

#include "seville/palace/client.h"
#include "seville/view/palaceclientwidget.h"
#include "seville/view/aboutdialog.h"
#include "seville/view/connectdialog.h"
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
            void on_topLevelDidChange(bool shouldBeVisibleFlag);
            void on_currentTabDidChange(int index);
            void on_openConnectionWasRequested(void);
            void on_closeConnectionWasRequested(void);
            void on_quitAppWasRequested(void);
            void on_aboutAppWasRequested(void);
            void on_logWindowWasToggled(void);
            void on_newTabRequested(void);
            void on_closeTabRequested(void);
            void on_currentClientConnectionStateDidChange(
                    // QWidget* sender,
                    const palace::ConnectionState connectionState);
            void on_tabWasAddedWithClientWidgetPtr(
                    seville::view::PalaceClientWidget* palaceClientWidgetPtr);
            // void on_tabWasRemovedWithClientWidgetPtr(
            //         seville::view::PalaceClientWidget* palaceClientWidgetPtr);

        protected:
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
            QAction* my_newTabActionPtr;
            QAction* my_closeTabActionPtr;
            QAction* my_undoContentActionPtr;
            QAction* my_redoContentActionPtr;
            QAction* my_cutContentActionPtr;
            QAction* my_copyContentActionPtr;
            QAction* my_pasteContentActionPtr;
            QAction* my_quitAppActionPtr;
            QAction* my_aboutAppActionPtr;
            QAction* my_toggleLogActionPtr;
            QStatusBar* my_statusBarPtr;
            QToolBar* my_toolBarPtr;
            seville::view::PalaceClientWidget* my_currentPalaceClientWidgetPtr;

            void do_setupWindow(void);
            void do_setupView(void);
            void do_setupActions(void);
            void do_setupMenus(void);
            void do_setupToolbars(void);
            void do_setupDocks(void);
            void do_setupEvents(void);
            void do_teardownEvents(void);
            void do_setupSizing(void);

            void do_updateMenus(void);
            void do_updateView(void);

            PalaceClientWidget* do_currentPalaceClientWidgetPtr(void);

            void do_init(void);
            void do_deinit(void);
        };
    }
}

#endif // SEVILLE_VIEW_MAIN_WINDOW_H_
