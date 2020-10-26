#ifndef SEVILLE_VIEW_ABOUTDIALOG_H_
#define SEVILLE_VIEW_ABOUTDIALOG_H_

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "seville/base/app.h"

namespace seville
{
   namespace view
   {
      namespace dialog
      {
         class AboutDialog : public QDialog
         {
         Q_OBJECT

         public:
            explicit AboutDialog(QWidget* parent_widget_ptr = nullptr);

            virtual ~AboutDialog(void);

         public slots:
            void on_button_ok_clicked(void);

         private:
            auto do_setup_view_(void) -> void;
            auto do_init_(void) -> void;
         };
      }
   }
}

#endif  // SEVILLE_VIEW_ABOUTDIALOG_H_
