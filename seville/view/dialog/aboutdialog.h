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
      class AboutDialog : public QDialog
      {
      Q_OBJECT

      protected:
         void ok_setupView()
         {
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            QVBoxLayout* pLayout = new QVBoxLayout(this);
            QLabel* pLabelAbout = new QLabel(QString("Seville v%1").arg(app_kAppVersion), this);
            pLayout->addWidget(pLabelAbout);

            QPushButton *pButtonOk = new QPushButton(tr("&OK"), this);
            pLayout->addWidget(pButtonOk);
            setLayout(pLayout);

            connect(pButtonOk, &QPushButton::clicked, this, &view::AboutDialog::on_buttonOkClicked);
         }

         void do_deinit()
         {
         }

         void do_init()
         {
            do_setupView();
         }

      public slots:
         void on_buttonOkClicked()
         {
            close();
         }

      public:
         virtual ~AboutDialog(void);
         explicit AboutDialog(QWidget* parent = nullptr);
      };
   }
}

#endif  // SEVILLE_VIEW_ABOUTDIALOG_H_
