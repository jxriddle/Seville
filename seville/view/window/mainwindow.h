#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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

#include "connectdialog.h"
#include "seville/view/base/clientwidget.h"
#include "aboutdialog.h"
#include "View_PalaceLogWidget.h"
#include "seville/view/tabwidget.h"

namespace seville
{
   namespace view
   {
      class MainWindow : public QMainWindow
      {
      Q_OBJECT

      private:
         Zed::View::ZedTabWidget* my_pTabWidget;
         QDockWidget* my_pDockWidgetForLog;
         PalaceLogWidget* my_pLogWidget;
         QVBoxLayout* my_pMainLayout;
         QTabBar* my_pTabBar;
         QMenuBar* my_pMenuBar;
         QMenu* my_pFileMenu;
         QMenu* my_pEditMenu;
         QMenu* my_pWindowMenu;
         QMenu* my_pHelpMenu;
         QAction* my_pNewHostConnectionAction;
         QAction* my_pCloseHostConnectionAction;
         QAction* my_pUndoContentAction;
         QAction* my_pRedoContentAction;
         QAction* my_pCutContentAction;
         QAction* my_pCopyContentAction;
         QAction* my_pPasteContentAction;
         QAction* my_pQuitAppAction;
         QAction* my_pAboutAppAction;
         QAction* my_pToggleLogAction;
         QStatusBar* my_pStatusBar;

         void do_setupView(void)
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
            my_pTabWidget = new Zed::View::ZedTabWidget(this);
            my_pTabWidget->setGeometry(0, 0, width(), height()); // x, y, w, h
            layout()->setSpacing(0);
            //layout()->addWidget(tabWidget_);
            auto pClientWidget = new PalaceClientWidget(this);
            //connect(palRoomWidget, )
            my_pTabWidget->addNewTab(pClientWidget);
            //mainLayout_->setSpacing(0);
            //mainLayout_->addWidget(tabWidget_, 1);

            //setLayout(mainLayout_);
            //mainLayout_->setSizeConstraint(QLayout::SetMinimumSize);

            my_pStatusBar = new QStatusBar(this);
            //setStatusBar(my_pStatusBar);
            //my_pStatusBar->showMessage(tr("Ready"));
            //layout()->addWidget(my_pStatusBar);

            // Set Main Window to Tab Widget
            setCentralWidget(my_pTabWidget);

            QSettings settings;
            restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
            restoreState(settings.value("mainWindowState").toByteArray());
         }

         void do_setupActions(void)
         {
            // Create Actions
            my_pNewHostConnectionAction = new QAction(tr("&Connect"), this);
            my_pCloseHostConnectionAction = new QAction(tr("&Disconnect"), this);
            my_pQuitAppAction = new QAction(tr("E&xit"), this);
            my_pUndoContentAction = new QAction(tr("&Undo"), this);
            my_pRedoContentAction = new QAction(tr("&Redo"), this);
            my_pCutContentAction = new QAction(tr("&Cut"), this);
            my_pCopyContentAction = new QAction(tr("&Copy"), this);
            my_pPasteContentAction = new QAction(tr("&Paste"), this);
            my_pAboutAppAction = new QAction(tr("&About Seville"), this);
            my_pToggleLogAction = new QAction(tr("&Log"), this);

            // Assign Action Shortcut Keys
            my_pNewHostConnectionAction->setShortcut(QKeySequence::New);
            my_pCloseHostConnectionAction->setShortcut(Qt::CTRL | Qt::Key_W);
            my_pCloseHostConnectionAction->setEnabled(false);
            my_pQuitAppAction->setShortcut(QKeySequence::Quit);
            my_pUndoContentAction->setShortcut(QKeySequence::Undo);
            my_pRedoContentAction->setShortcut(QKeySequence::Redo);
            my_pCutContentAction->setShortcut(QKeySequence::Cut);
            my_pCopyContentAction->setShortcut(QKeySequence::Copy);
            my_pPasteContentAction->setShortcut(QKeySequence::Paste);
            my_pToggleLogAction->setShortcut(Qt::CTRL | Qt::Key_L);
         }

         void do_setupMenus(void)
         {
            // Create Menu Bar
            my_pMenuBar = new QMenuBar(this);
            my_pFileMenu = my_pMenuBar->addMenu(tr("&File"));
            my_pEditMenu = my_pMenuBar->addMenu(tr("&Edit"));
            my_pWindowMenu = my_pMenuBar->addMenu(tr("&Window"));
            my_pHelpMenu = my_pMenuBar->addMenu(tr("&Help"));

            // File Menu
            my_pFileMenu->addAction(my_pNewHostConnectionAction);
            my_pFileMenu->addAction(my_pCloseHostConnectionAction);
            my_pFileMenu->addSeparator();
            my_pFileMenu->addAction(my_pQuitAppAction);

            // Edit Menu
            my_pEditMenu->addAction(my_pUndoContentAction);
            my_pEditMenu->addAction(my_pRedoContentAction);
            my_pEditMenu->addSeparator();
            my_pEditMenu->addAction(my_pCutContentAction);
            my_pEditMenu->addAction(my_pCopyContentAction);
            my_pEditMenu->addAction(my_pPasteContentAction);
            my_pEditMenu->addSeparator();

            // Window Menu
            //myWindowMenu->addAction(myToggleLogAction);
            my_pWindowMenu->addAction(my_pDockWidgetForLog->toggleViewAction());

            // About Menu
            my_pHelpMenu->addSeparator();
            my_pHelpMenu->addAction(my_pAboutAppAction);

            setMenuBar(my_pMenuBar);
         }

         void do_setupDocks(void)
         {
            auto my_pPalaceClientWidget = static_cast<PalaceClientWidget *>(my_pTabWidget->currentWidget());
            auto pPalaceClient = my_pPalaceClientWidget->palaceClientP();
            my_pLogWidget = new PalaceLogWidget(pPalaceClient, this);
            my_pLogWidget->setLineEditChatVisible(my_pLogWidget->isWindow());

            my_pDockWidgetForLog = new QDockWidget(tr("Log"), this);
            my_pDockWidgetForLog->setAllowedAreas(
                     Qt::LeftDockWidgetArea |
                     Qt::BottomDockWidgetArea |
                     Qt::RightDockWidgetArea |
                     Qt::TopDockWidgetArea
            );

            my_pDockWidgetForLog->setWidget(my_pLogWidget);
            addDockWidget(Qt::RightDockWidgetArea, my_pDockWidgetForLog);
         }

         void do_setupEvents(void)
         {
            // Connect Action Signals to Slots
            connect(my_pDockWidgetForLog, &QDockWidget::topLevelChanged,
                    this, &seville::view::MainWindow::on_topLevelChanged);

            connect(my_pNewHostConnectionAction, &QAction::triggered,
                    this, &seville::view::MainWindow::on_newHostConnectionActionTriggered);

            connect(my_pCloseHostConnectionAction, &QAction::triggered,
                    this, &seville::view::MainWindow::on_closeHostConnectionActionTriggered);

            connect(my_pQuitAppAction, &QAction::triggered,
                    this, &seville::view::MainWindow::on_quitAppActionTriggered);

            connect(my_pAboutAppAction, &QAction::triggered,
                    this, &seville::view::MainWindow::on_aboutAppActionTriggered);

            connect(my_pToggleLogAction, &QAction::triggered,
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
            my_pLogWidget->setLineEditChatVisible(shouldBeVisible);
         }

         void on_newHostConnectionActionTriggered(void)
         {
            PalaceClientWidget* pActiveClientWidget = static_cast<PalaceClientWidget *>(my_pTabWidget->currentWidget());
            //connect(palTabWidget, resize);
            pActiveClientWidget->promptNewConnection();
         }

         void on_closeHostConnectionActionTriggered(void)
         {
            // if last tab is closed, quit application
            if (my_pTabWidget->count() <= 2) {
               QApplication::quit();
            }

            my_pTabWidget->removeTab(my_pTabWidget->currentIndex());
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
            if (!my_pLogWidget->isVisible())
               my_pLogWidget->show();
            else
               my_pLogWidget->hide();
         }

         void do_deinit(void)
         {
            if (my_pLogWidget != nullptr)
               delete(my_pLogWidget);
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

#endif // UI_MAINWINDOW_H
