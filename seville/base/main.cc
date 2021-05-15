#include <iostream>

#include "seville/base/logcategories.h"
#include "seville/base/sevilleapp.h"

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

namespace seville {
   SevilleApp app;
}

int main(int argc, char *argv[])
{
   return seville::app.main(argc, argv);
}
