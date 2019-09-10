#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QScrollArea>
#include <QTabBar>
#include <QTabWidget>
#include <QMenuBar>
#include <QGridLayout>

#include "view_app_tab_widget.h"

namespace Seville
{
   namespace View
   {
      class MainWindow : public QMainWindow
      {
         Q_OBJECT

         public:
            explicit MainWindow(QWidget *parent = nullptr);
            virtual ~MainWindow() override;

         public slots:
            void onNewHostConnectionActionTriggered();
            void onCloseHostConnectionActionTriggered();
            void onQuitAppActionTriggered();
            //void onPalRoomWidgetClicked();
            //void onTabWidgetTabBarClicked(int index);
            //void onLineEditAddressReturnPressed();
            //void onTabWidgetTabCloseRequested(int index);
            void onAboutAppActionTriggered();

         protected:
            void resizeEvent(QResizeEvent *event) override;
            void closeEvent(QCloseEvent *event) override;

         private:
            void doSetupUi();
            void doSetupActions();
            void doSetupMenus();
            void doSetupEvents();

            QVBoxLayout* myMainLayout;
            AppTabWidget* myTabWidget;
            QTabBar* myTabBar;
            QMenuBar* myMenuBar;
            QMenu* myFileMenu;
            QMenu* myEditMenu;
            QMenu* myViewMenu;
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
      };
   }
}

#endif // UI_MAINWINDOW_H
