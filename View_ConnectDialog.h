#ifndef VIEW_CONNECTDIALOG_H
#define VIEW_CONNECTDIALOG_H

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

         private:
            QGridLayout* myLayout;
            QLabel* myLabelUsername;
            QLineEdit* myLineEditUsername;
            QLabel* myLabelUrl;
            QLineEdit* myLineEditUrl;
            QPushButton* myPushButtonConnect;
            QPushButton* myPushButtonCancel;
            QDialogButtonBox* myButtonBox;

            void doSetupView();
            void doSetupEvents();

         public:
            QString address();
            QString host();
            quint16 port();
            QString username();
            //virtual ~ConnectDialog();
            explicit ConnectDialog(QWidget* parent = nullptr);

         signals:

         public slots:
//            void onPushButtonConnectClicked();
//            void onPushButtonCancelClicked();
      };
   }
}

#endif // VIEW_CONNECTDIALOG_H
