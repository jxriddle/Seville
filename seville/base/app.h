#ifndef SEVILLE_APP_H_
#define SEVILLE_APP_H_

#include <QString>

#include "seville/palace/base/logger.h"
#include "seville/view/window/mainwindow.h"

namespace seville
{
   const char app_kAppVersion[] = "1.0";
   extern view::MainWindow* app_pMainWindow;
}

#endif // SEVILLE_APP_H_
