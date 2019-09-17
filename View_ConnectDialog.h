#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>

namespace Seville
{
   namespace View
   {
      class ConnectDialog : public QDialog
      {
         Q_OBJECT

      public:
         explicit ConnectDialog(QWidget *parent = nullptr);
         //virtual ~ConnectDialog();
         QString address();
         QString host();
         quint16 port();
         QString username();

         void doSetupView();
         QGridLayout* myLayout = nullptr;
         QLabel* myLabelUsername = nullptr;
         QLineEdit* myLineEditUsername = nullptr;
         QLabel* myLabelUrl = nullptr;
         QLineEdit* myLineEditUrl = nullptr;
         QPushButton* myPushButtonConnect = nullptr;
         QPushButton* myPushButtonCancel = nullptr;
         QDialogButtonBox* myButtonBox = nullptr;

      public slots:

      signals:

      private:

      private slots:
         void doOnPushButtonConnectClicked();
         void doOnPushButtonCancelClicked();
      };
   }
}

#endif // UICONNECTDIALOG_H
