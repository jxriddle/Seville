#include <iostream>

#include "seville/base/app.h"

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

seville::SevilleApp app_seville;

int main(int argc, char *argv[])
{
   return app_seville.main(argc, argv);
}
