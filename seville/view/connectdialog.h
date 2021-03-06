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
         static auto New(QWidget* parentWidgetPtr = nullptr)
            -> std::unique_ptr<ConnectDialog>;

         explicit ConnectDialog(QWidget* parentWidgetPtr = nullptr);
         //virtual ~ConnectDialog(void);

         inline auto address(void) -> QString {
            return my_urlLineEditPtr->text();
         }

         inline auto host(void) -> QString {
            QStringList sl = my_urlLineEditPtr->text().split(':');
            return sl.first();
         }

         inline auto port(void) -> quint16 {
            QStringList sl = my_urlLineEditPtr->text().split(':');
            return sl.last().toUShort();
         }

         inline auto username(void) -> QString {
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

         auto do_setupView(void) -> void;
         auto do_setupEvents(void) -> void;
      };
   }
}

#endif  // SEVILLE_VIEW_CONNECT_DIALOG_H_
