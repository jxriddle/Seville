#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "main.h"

namespace Seville
{
   namespace View
   {
      class AboutDialog : public QDialog
      {
         Q_OBJECT

         public:
            explicit AboutDialog(QWidget* parent = nullptr);

         public slots:
            void doOnClickOkButton();

         private:
            void doSetupUi();
      };
   }
}

#endif // UI_ABOUTDIALOG_H
