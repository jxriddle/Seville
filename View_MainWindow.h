#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QScrollArea>
#include <QTabBar>
#include <QTabWidget>

#include "View_AppTabWidget.h"
#include "View_LogWidget.h"

namespace Seville
{
   namespace View
   {
      class MainWindow : public QMainWindow
      {
         Q_OBJECT

         private:
            QDockWidget* myDockWidgetForLog;
            LogWidget* myLogWidget;
            QVBoxLayout* myMainLayout;
            AppTabWidget* myTabWidget;
            QTabBar* myTabBar;
            QMenuBar* myMenuBar;
            QMenu* myFileMenu;
            QMenu* myEditMenu;
            QMenu* myWindowMenu;
            QMenu* myHelpMenu;
            QAction* myNewHostConnectionAction;
            QAction* myCloseHostConnectionAction;
            QAction* myUndoContentAction;
            QAction* myRedoContentAction;
            QAction* myCutContentAction;
            QAction* myCopyContentAction;
            QAction* myPasteContentAction;
            QAction* myQuitAppAction;
            QAction* myAboutAppAction;
            QAction* myToggleLogAction;

            void doSetupView(void);
            void doSetupActions(void);
            void doSetupMenus(void);
            void doSetupDocks(void);
            void doSetupEvents(void);

         protected:
            void resizeEvent(QResizeEvent* event) override;
            void closeEvent(QCloseEvent* event) override;

         public slots:
            void onNewHostConnectionActionTriggered(void);
            void onCloseHostConnectionActionTriggered(void);
            void onQuitAppActionTriggered(void);
            //void onPalRoomWidgetClicked(void);
            //void onTabWidgetTabBarClicked(int index);
            //void onLineEditAddressReturnPressed(void);
            //void onTabWidgetTabCloseRequested(int index);
            void onAboutAppActionTriggered(void);
            void onToggleLogWindowActionTriggered(void);

         public:
            virtual ~MainWindow(void) override;
            explicit MainWindow(QWidget* parent = nullptr);
      };
   }
}

#endif // UI_MAINWINDOW_H
