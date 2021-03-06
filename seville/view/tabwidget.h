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

            auto addNewTab(QWidget* contentWidgetPtr = nullptr) -> void;
            auto removeTab(int index) -> void;
            auto currentPalaceClientPtr(void) -> palace::Client*;
            auto clear(void) -> void;

         public slots:
            void on_tabDidMove(void);
            void on_tabDidClose(int index);
            //auto on_plusButtonClickedEvent(void) -> void;
            //void on_clientTabWidgetDidResize(int width, int height);
            void on_currentTabDidChange(int index);
            void on_widgetBackgroundDidChange(void);

         private:
            //AppTabBar my_appTabBar;
            QLayout* my_mainLayoutPtr;
            palace::Client* my_currentPalaceClientPtr;

            auto do_setupView(void) -> void;
            auto do_addNewTab(QWidget* contentWidgetPtr = nullptr) -> void;
            auto do_setupEvents(void) -> void;
            auto do_setupSizing(void) -> void;
            auto do_clear(void) -> void;
      };
   }
}

#endif // SEVILLE_VIEW_TAB_WIDGET_H_
