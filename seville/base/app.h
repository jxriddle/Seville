#ifndef SEVILLE_APP_H_
#define SEVILLE_APP_H_

#include <QString>

#include "seville/palace/base/logger.h"
#include "seville/view/window/mainwindow.h"

namespace seville
{
   struct app_struct {
      const char app_kAppVersion[] = "1.0";
      view::MainWindow* app_window_main_ptr;
   } my_app_;

   auto app(void) -> app_struct {
      return my_app_;
   }
}

#endif   // SEVILLE_APP_H_
