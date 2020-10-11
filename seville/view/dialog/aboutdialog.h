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

      public:
         explicit AboutDialog(QWidget* parent_widget_ptr = nullptr);

         virtual ~AboutDialog(void);

      public slots:
         void on_button_ok_clicked(void)
         {
            close();
         }

      private:
         void do_setup_view_(void)
         {
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            QVBoxLayout* pLayout = new QVBoxLayout(this);
            QLabel* pLabelAbout =
                  new QLabel(QString("Seville v%1")
                             .arg(app()->kAppVersion), this);

            pLayout->addWidget(pLabelAbout);

            QPushButton *pButtonOk = new QPushButton(tr("&OK"), this);
            pLayout->addWidget(pButtonOk);
            setLayout(pLayout);

            connect(pButtonOk, &QPushButton::clicked, this, &view::AboutDialog::on_button_ok_clicked);
         }

         void do_deinit(void)
         {
         }

         void do_init(void)
         {
            do_setup_view_();
         }
      };
   }
}

#endif  // SEVILLE_VIEW_ABOUTDIALOG_H_
