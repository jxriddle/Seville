#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGuiApplication>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "ui_connect_dialog.h"

namespace Seville
{
   namespace Ui
   {
      ConnectDialog::ConnectDialog(QWidget* parent)
         : QDialog(parent)
      {
         doSetupUi();
      }
      /*
      UiConnectDialog::~UiNewPalaceConnectionDialog()
      {

      }
      */
      void ConnectDialog::doSetupUi()
      {
         // Window
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         //setWindowTitle(QGuiApplication::applicationDisplayName());
         setWindowTitle("Connect");

         // Layout
         myLayout = new QGridLayout(this);

         // ButtonBox
         myButtonBox = new QDialogButtonBox(this);

         // PushButtons
         myPushButtonCancel = new QPushButton(tr("&Cancel"), this);
         myPushButtonConnect = new QPushButton(tr("C&onnect"), this);
         myPushButtonConnect->setDefault(true);
         myPushButtonConnect->setEnabled(true);

         // LineEdit
         myLineEditUsername = new QLineEdit(this);
         myLineEditUrl = new QLineEdit(this);

         // Labels
         myLabelUsername = new QLabel(tr("&Username"), this);
         myLabelUrl = new QLabel(tr("&Address"), this);
         myLabelUsername->setBuddy(myLineEditUsername);
         myLabelUrl->setBuddy(myLineEditUrl);

         // Arrangement
         myButtonBox->addButton(
                  myPushButtonConnect, QDialogButtonBox::ActionRole);
         myButtonBox->addButton(
                  myPushButtonCancel, QDialogButtonBox::RejectRole);

         myLayout->addWidget(myLabelUsername, 0, 0);
         myLayout->addWidget(myLineEditUsername, 0, 1);
         myLayout->addWidget(myLabelUrl, 1, 0);
         myLayout->addWidget(myLineEditUrl, 1, 1);
         myLayout->addWidget(myButtonBox, 2, 0, 1, 2);

         setLayout(myLayout);

         // Events
         connect(myPushButtonConnect, SIGNAL(clicked()), this, SLOT(doOnPushButtonConnectClicked()));
         connect(myPushButtonCancel, SIGNAL(clicked()), this, SLOT(doOnPushButtonCancelClicked()));

         //pushButtonConnect_.setFocus();
      }

      void ConnectDialog::doOnPushButtonConnectClicked()
      {
         this->setResult(QDialog::Accepted);
         this->accept();
      }

      void ConnectDialog::doOnPushButtonCancelClicked()
      {
         this->setResult(QDialog::Rejected);
         this->reject();
      }

      QString ConnectDialog::address()
      {
         return myLineEditUrl->text();
      }

      QString ConnectDialog::username()
      {
         return myLineEditUsername->text();
      }

      QString ConnectDialog::host()
      {
         QStringList sl = myLineEditUrl->text().split(':');
         return sl.first();
      }

      quint16 ConnectDialog::port()
      {
         QStringList sl = myLineEditUrl->text().split(':');
         return sl.last().toUShort();
      }
   }
}
