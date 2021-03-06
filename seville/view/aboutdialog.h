#ifndef SEVILLE_VIEW_ABOUT_DIALOG_H_
#define SEVILLE_VIEW_ABOUT_DIALOG_H_

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "seville/base/sevilleapp.h"

namespace seville
{
   namespace view
   {
      class AboutDialog : public QDialog
      {
      Q_OBJECT

      public:
         explicit AboutDialog(QWidget* parentWidgetPtr = nullptr);
         virtual ~AboutDialog(void);

      public slots:
         void on_okButtonWasClickedEvent(void);

      private:
         auto do_setupView(void) -> void;
         auto do_init(void) -> void;
      };
   }
}

#endif  // SEVILLE_VIEW_ABOUT_DIALOG_H_
