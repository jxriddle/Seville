#include <QApplication>
#include <QLoggingCategory>
#include <QString>
#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <QLoggingCategory>

#include "seville/base/sevilleapp.h"
#include "seville/view/mainwindow.h"

namespace seville
{
   Q_LOGGING_CATEGORY(seville_log, "seville.io")

   SevilleApp::SevilleApp(void) noexcept
   {
   }

//   SevilleApp::~SevilleApp(void)
//   {
//      delete my_mainWindowPtr;
//   }

   int SevilleApp::main(int argc, char* argv[])
   {
      QCoreApplication::setOrganizationName("Orkey");
      QCoreApplication::setApplicationName("Seville");
      QCoreApplication::setApplicationVersion(kAppVersion);

      QLoggingCategory::setFilterRules("*.debug=false\n"
                                       "seville.io.debug=true");
      QLoggingCategory::setFilterRules("seville.io.*=true");

      qCDebug(seville_log) << "Starting " << argv[0]; // << "...";

      QApplication qapp(argc, argv);
      view::MainWindow mainWindow;
      //my_mainWindowPtr = new view::MainWindow();
      //auto window_main_unique_ptr = std::make_unique<view::MainWindow>();
      //set_main_window_unique_ptr(std::move(window_main_unique_ptr));
      //my_mainWindowPtr->show();
      mainWindow.show();

      qCDebug(seville_log) << argv[0] << " Started.";

      auto appResult = qapp.exec();

      //delete my_mainWindowPtr;
      return appResult;
   }
}
