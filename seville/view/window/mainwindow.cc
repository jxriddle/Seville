#include "mainwindow.h"

namespace seville
{
   namespace view
   {
      namespace window
      {
         MainWindow::MainWindow(QWidget* widget_parent_ptr)
            : QMainWindow(widget_parent_ptr)
         {
            do_init_();
         }
      }
   }
}
