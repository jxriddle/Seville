#include <iostream>

#include <QApplication>
#include <QCoreApplication>

#include "Main.h"
#include "Log.h"
#include "View_MainWindow.h"

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

namespace Seville
{
   int main(int argc, char* argv[])
   {
      QCoreApplication::setOrganizationName("Orkey");
      QCoreApplication::setApplicationName("Seville");
      QCoreApplication::setApplicationVersion(kAppVersion);

      QLoggingCategory::setFilterRules("*.debug=false\n"
                                           "seville.io.debug=true");
      qCDebug(appLcIo) << "Starting " << argv[0]; // << "...";
      QApplication app(argc, argv);
      View::MainWindow mainWindow;
      mainWindow.show();

      qCDebug(appLcIo) << argv[0] << " Started.";

      return app.exec();
   }
}

int main(int argc, char *argv[])
{
   return Seville::main(argc, argv);
}
