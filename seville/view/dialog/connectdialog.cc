#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGuiApplication>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "connectdialog.h"

namespace seville
{
   namespace view
   {
      ConnectDialog::~ConnectDialog()
      {
         do_deinit();
      }

      ConnectDialog::ConnectDialog(QWidget* parent)
         : QDialog(parent)
      {
         do_init();
      }
   }
}
