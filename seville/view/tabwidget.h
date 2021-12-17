#ifndef SEVILLE_VIEW_TAB_WIDGET_H_
#define SEVILLE_VIEW_TAB_WIDGET_H_

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTabWidget>

#include "seville/palace/client.h"
#include "seville/view/palaceclientwidget.h"

namespace seville
{
    namespace view
    {
        class TabWidget : public QTabWidget
        {
         Q_OBJECT

         public:
            explicit TabWidget(QWidget* parentWidgetPtr = nullptr);
            virtual ~TabWidget(void);

            void addNewTab(QWidget* contentWidgetPtr = nullptr);
            void closeCurrentTab(void);
            void removeTab(int index);
            palace::Client* currentPalaceClientPtr(void);
            // auto setBackground(void);
            void clear(void);

         signals:
            void tabWasAddedWithWidgetPtrEvent(
                  seville::view::PalaceClientWidget* palaceClientWidgetPtr);
            void tabWasRemovedWithWidgetPtrEvent(
                  seville::view::PalaceClientWidget* palaceClientWidgetPtr);

         public slots:
            void on_tabDidMove(void);
            void on_tabDidClose(int index);
            void on_addNewTabRequested(void);
            void on_closeCurrentTabRequested(void);
            void on_serverNameWasSet(
                  QWidget* senderPtr, const QString& value);
            // auto on_plusButtonClickedEvent(void) -> void;
            // void on_clientTabWidgetDidResize(int width, int height);
            // void on_currentTabDidChange(int index);
            // void on_widgetBackgroundDidChange(void);
            void on_tabBarDoubleClicked(void);

         private:
            //AppTabBar my_appTabBar;
            QLayout* my_mainLayoutPtr;
            //palace::Client* my_currentPalaceClientPtr;

            void do_setupView(void);
            void do_setupEvents(void);
            void do_setupSizing(void);

            void do_addNewTab(QWidget* contentWidgetPtr = nullptr);
            void do_closeTab(int index);
            void do_closeCurrentTab(void);

            void do_teardownEvents(void);

            void do_clear(void);
            void do_init(QWidget* contentWidgetPtr = nullptr);
        };
    }
}

#endif // SEVILLE_VIEW_TAB_WIDGET_H_
