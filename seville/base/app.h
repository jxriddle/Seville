#ifndef SEVILLE_APP_H_
#define SEVILLE_APP_H_

#include <memory>

#include <QLoggingCategory>

#include "seville/base/types.h"
#include "seville/base/bytearray.h"

namespace seville
{
   Q_DECLARE_LOGGING_CATEGORY(log_seville)

   class SevilleApp
   {
   public:
      //extern const QLoggingCategory &name();
      const char kAppVersion[4] = "1.0";
      //const char kLogCategoryName[12] = "log_seville";

      SevilleApp(void) noexcept;

//      inline auto main_window_ptr(void) -> view::window::MainWindow* {
//         return my_window_main_unique_ptr_.get();
//      }

//      inline auto app_version(void) -> char {
//      }

      auto main(int argc, char* argv[]) -> int;

//   private:
//      std::unique_ptr<view::window::MainWindow> my_window_main_unique_ptr_;
   };
}

extern seville::SevilleApp app_seville;

auto SevilleApp(void) -> seville::SevilleApp*;

#endif   // SEVILLE_APP_H_
