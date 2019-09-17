#include <iostream>

#include <QApplication>

#include "Main.h"
#include "View_MainWindow.h"
#include "Log.h"

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

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false\n"
                                         "seville.io.debug=true");
    qCDebug(appLcIo) << "Starting " << argv[0] << "...";
    QApplication app(argc, argv);
    Seville::View::MainWindow mainWindow;
    mainWindow.show();
    qCDebug(appLcIo) << argv[0] << " Started.";

    return app.exec();
}
