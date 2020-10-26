#ifndef SEVILLE_VIEW_DIALOG_CONNECTDIALOG_H_
#define SEVILLE_VIEW_DIALOG_CONNECTDIALOG_H_

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
      namespace dialog
      {
         class ConnectDialog : public QDialog
         {
         Q_OBJECT

         public:
            static auto New(QWidget* widget_parent_ptr = nullptr)
               -> std::unique_ptr<ConnectDialog>;

            explicit ConnectDialog(QWidget* widget_parent_ptr = nullptr);
            //virtual ~ConnectDialog(void);

            inline auto address(void) -> QString {
               return my_line_edit_url_ptr_->text();
            }

            inline auto host(void) -> QString {
               QStringList sl = my_line_edit_url_ptr_->text().split(':');
               return sl.first();
            }

            inline auto port(void) -> quint16 {
               QStringList sl = my_line_edit_url_ptr_->text().split(':');
               return sl.last().toUShort();
            }

            inline auto username(void) -> QString {
               return my_line_edit_username_ptr_->text();
            }

         signals:

         public slots:
   //       void on_pushButtonConnectClicked();
   //       void on_pushButtonCancelClicked();

         private:
            QGridLayout* my_layout_main_ptr_;
            QLabel* my_label_username_ptr_;
            QLineEdit* my_line_edit_username_ptr_;
            QLabel* my_label_url_ptr_;
            QLineEdit* my_line_edit_url_ptr_;
            QPushButton* my_push_button_connect_ptr_;
            QPushButton* my_push_button_cancel_ptr_;
            QDialogButtonBox* my_button_box_ptr_;

            auto do_setup_view_(void) -> void;
            auto do_setup_events_(void) -> void;
         };
      }
   }
}

#endif  // SEVILLE_VIEW_DIALOG_CONNECTDIALOG_H_
