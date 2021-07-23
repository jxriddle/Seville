#ifndef SEVILLE_VIEW_CONNECT_DIALOG_H_
#define SEVILLE_VIEW_CONNECT_DIALOG_H_

#include <memory>

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>

namespace seville
{
   namespace view
   {
      class ConnectDialog : public QDialog
      {
      Q_OBJECT

      public:
//         static std::unique_ptr<ConnectDialog>New(
//               QWidget* parentWidgetPtr = nullptr);

         explicit ConnectDialog(QWidget* parentWidgetPtr = nullptr);
         //virtual ~ConnectDialog(void);

         inline QString address(void) {
            return my_urlLineEditPtr->text();
         }

         inline QString host(void) {
            QStringList sl = my_urlLineEditPtr->text().split(':');
            return sl.first();
         }

         inline quint16 port(void) {
            QStringList sl = my_urlLineEditPtr->text().split(':');
            return sl.last().toUShort();
         }

         inline QString username(void) {
            return my_usernameLineEditPtr->text();
         }

      signals:

      public slots:
//       void on_pushButtonConnectClicked();
//       void on_pushButtonCancelClicked();

      private:
         QGridLayout* my_mainLayoutPtr;
         QLabel* my_usernameLabelPtr;
         QLineEdit* my_usernameLineEditPtr;
         QLabel* my_urlLabelPtr;
         QLineEdit* my_urlLineEditPtr;
         QPushButton* my_connectPushButtonPtr;
         QPushButton* my_cancelPushButtonPtr;
         QDialogButtonBox* my_mainButtonBoxPtr;

         void do_setupView(void);
         void do_setupEvents(void);
      };
   }
}

#endif  // SEVILLE_VIEW_CONNECT_DIALOG_H_
