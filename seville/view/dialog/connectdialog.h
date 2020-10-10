#ifndef VIEW_DIALOG_CONNECTDIALOG_H_
#define VIEW_DIALOG_CONNECTDIALOG_H_

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

      private:
         QGridLayout* my_layout_main_pointer_;
         QLabel* my_label_username_pointer_;
         QLineEdit* my_lineedit_username_pointer_;
         QLabel* my_label_url_pointer_;
         QLineEdit* my_lineedit_url_pointer_;
         QPushButton* my_pushbutton_connect_pointer_;
         QPushButton* my_pushbutton_cancel_pointer_;
         QDialogButtonBox* my_buttonbox_pointer_;

      protected:
         auto ok_setupView(void) -> void;
         auto ok_setupEvents(void) -> void;

      public:
         static auto init(void) -> std::optional<ConnectDialog*>;

         explicit ConnectDialog(QWidget* parent_widget_pointer = nullptr);

         virtual ~ConnectDialog(void);

         inline auto address(void) -> QString {
            return my_lineedit_url_pointer_->text();
         }

         inline auto host(void) -> QString {
            QStringList sl = my_lineedit_url_pointer_->text().split(':');
            return sl.first();
         }

         inline auto port(void) -> quint16 {
            QStringList sl = my_lineedit_url_pointer_->text().split(':');
            return sl.last().toUShort();
         }

         inline auto username(void) -> QString {
            return my_lineedit_username_pointer_->text();
         }

      signals:

      public slots:
//       void on_pushButtonConnectClicked();
//       void on_pushButtonCancelClicked();

      };
   }
}

#endif  // VIEW_DIALOG_CONNECTDIALOG_H_
