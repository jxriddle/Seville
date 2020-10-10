#ifndef SEVILLE_APP_H_
#define SEVILLE_APP_H_

#include <QString>

#include "seville/palace/base/logger.h"
#include "seville/view/window/mainwindow.h"

namespace seville
{
   struct app_struct {

   } app_;

   const char app_kAppVersion[] = "1.0";
   extern view::MainWindow* app_window_main_ptr;
}

#endif   // SEVILLE_APP_H_
