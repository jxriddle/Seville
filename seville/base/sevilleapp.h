#ifndef SEVILLE_APP_H_
#define SEVILLE_APP_H_

#include <memory>

#include <QApplication>
#include <QLoggingCategory>


#include "seville/base/types.h"
//#include "seville/view/mainwindow.h"

namespace seville
{
   Q_DECLARE_LOGGING_CATEGORY(seville_log)

   namespace view
   {
      class MainWindow;
   }

   class SevilleApp
   {
   public:
      //extern const QLoggingCategory &name();
      const char kAppVersion[64] = "1.0";
      //const char kLogCategoryName[12] = "log_seville";

      SevilleApp(void) noexcept;

      auto main(int argc, char* argv[]) -> int;

   //   view::MainWindow* mainWindowPtr(void);

   //private:
   //   view::MainWindow* my_mainWindowPtr;
   };

   extern SevilleApp sevilleApp;
}

#endif   // SEVILLE_APP_H_
