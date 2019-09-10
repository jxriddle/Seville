#include <iostream>

#include <QApplication>

#include "main.h"
#include "ui_main_window.h"
#include "applog.h"

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
                                         "alcazarnik.io.debug=true");
    qCDebug(appLcIo) << "Starting " << argv[0] << "...";
    QApplication app(argc, argv);
    Seville::Ui::MainWindow win;
    win.show();
    qCDebug(appLcIo) << argv[0] << " Started.";

    return app.exec();
}
