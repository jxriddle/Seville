#include <iostream>

#include <QApplication>
#include <QCoreApplication>

#include "app.h"
#include "log.h"

#include "seville/view/window/mainwindow.h"

/*
void msgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg(symbols[type]).arg(msg);
    //std::cerr << output.toStdString() << std::endl;
    std::cout << '[' << context.category << ']' << output.toStdString() << std::endl;
    if (type == QtFatalMsg) { abort(); }
}
*/

namespace seville
{
   int main(int argc, char* argv[])
   {
      QCoreApplication::setOrganizationName("Orkey");
      QCoreApplication::setApplicationName("Seville");
      QCoreApplication::setApplicationVersion(app_kAppVersion);

      QLoggingCategory::setFilterRules("*.debug=false\n"
                                           "seville.io.debug=true");
      qCDebug(log_seville) << "Starting " << argv[0]; // << "...";
      QApplication app(argc, argv);
      view::MainWindow window_main;
      app_window_main_ptr = &window_main;
      window_main.show();

      qCDebug(log_seville) << argv[0] << " Started.";

      return app.exec();
   }
}

int main(int argc, char *argv[])
{
   return seville::main(argc, argv);
}
