#include "mainwindow.h"

namespace seville
{
   namespace view
   {
      MainWindow::~MainWindow(void)
      {
         do_deinit();
      }

      MainWindow::MainWindow(QWidget* pParentWidget)
         : QMainWindow(pParentWidget)
      {
         do_init();
      }
   }
}
