#include "View_AboutDialog.h"

namespace Seville
{
   namespace View
   {
      AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
      {
          doSetupView();
      }

      void AboutDialog::doSetupView()
      {
          setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
          QVBoxLayout* layout = new QVBoxLayout(this);
          QLabel* aboutLabel = new QLabel(QString("Seville v%1").arg(kAppVersion), this);
          layout->addWidget(aboutLabel);

          QPushButton *okButton = new QPushButton(tr("&OK"), this);
          layout->addWidget(okButton);
          setLayout(layout);

          connect(okButton, SIGNAL(clicked()), this, SLOT(doOnClickOkButton()));
      }

      void AboutDialog::doOnClickOkButton()
      {
          close();
      }
   }
}