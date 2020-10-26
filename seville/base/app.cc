#include <QApplication>
#include <QLoggingCategory>
#include <QString>
#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include "seville/base/app.h"

#include "seville/view/window/mainwindow.h"

namespace seville
{
   Q_LOGGING_CATEGORY(log_seville, "seville.log")

   SevilleApp::SevilleApp(void) noexcept
   {
   }

   auto SevilleApp::main(int argc, char* argv[]) -> int
   {
      QCoreApplication::setOrganizationName("Orkey");
      QCoreApplication::setApplicationName("Seville");
      QCoreApplication::setApplicationVersion(kAppVersion);

      QLoggingCategory::setFilterRules("*.debug=false\n"
                                           "seville.io.debug=true");

      qCDebug(log_seville) << "Starting " << argv[0]; // << "...";

      QApplication qapp(argc, argv);
      view::window::MainWindow window_main;
      //auto window_main_unique_ptr = std::make_unique<view::MainWindow>();
      //set_main_window_unique_ptr(std::move(window_main_unique_ptr));
      window_main.show();

      qCDebug(log_seville) << argv[0] << " Started.";

      return qapp.exec();
   }
}

auto SevilleApp(void) -> seville::SevilleApp* {
   return &app_seville;
}
