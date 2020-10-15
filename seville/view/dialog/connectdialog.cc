#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGuiApplication>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "seville/view/dialog/connectdialog.h"

namespace seville
{
   namespace view
   {
      namespace dialog
      {
         ConnectDialog::ConnectDialog(QWidget* parent_widget_ptr_)
            : QDialog(parent_widget_ptr_) {}

         auto ConnectDialog::New(QWidget* widget_parent_ptr) \
               -> std::unique_ptr<ConnectDialog>
         {
            auto instance = std::make_unique<ConnectDialog>(widget_parent_ptr);
            //std::make_optional<ConnectDialog*>();

            //if (object.has_value()) {
               //object.value()->ok_setupView();
               //object.value()->ok_setupEvents();
            //}
            instance->do_setup_view_();
            instance->do_setup_events_();

            return instance;
         }

         void ConnectDialog::do_setup_view_(void)
         {
            // Window
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            //setWindowTitle(QGuiApplication::applicationDisplayName());
            setWindowTitle("Connect");

            // Layout
            my_layout_main_ptr_ = new QGridLayout(this);

            // ButtonBox
            my_buttonbox_ptr_ = new QDialogButtonBox(this);

            // PushButtons
            my_pushbutton_cancel_ptr_ = new QPushButton(tr("&Cancel"), this);

            my_pushbutton_connect_ptr_ = new QPushButton(tr("C&onnect"), this);
            my_pushbutton_connect_ptr_->setDefault(true);
            my_pushbutton_connect_ptr_->setEnabled(true);

            // LineEdit
            my_lineedit_url_ptr_ = new QLineEdit(this);

            my_lineedit_username_ptr_ = new QLineEdit(this);

            // Labels
            my_label_url_ptr_ = new QLabel(tr("&Address"), this);
            my_label_url_ptr_->setBuddy(my_lineedit_url_ptr_);

            my_label_username_ptr_ = new QLabel(tr("&Username"), this);
            my_label_username_ptr_->setBuddy(my_lineedit_username_ptr_);

            // Arrangement
            my_buttonbox_ptr_->addButton( \
                     my_pushbutton_connect_ptr_, QDialogButtonBox::ActionRole);
            my_buttonbox_ptr_->addButton( \
                     my_pushbutton_cancel_ptr_, QDialogButtonBox::RejectRole);

            my_layout_main_ptr_->addWidget(my_label_url_ptr_, 0, 0);
            my_layout_main_ptr_->addWidget(my_lineedit_url_ptr_, 0, 1);

            my_layout_main_ptr_->addWidget(my_label_username_ptr_, 1, 0);
            my_layout_main_ptr_->addWidget(my_lineedit_username_ptr_, 1, 1);

            my_layout_main_ptr_->addWidget(my_buttonbox_ptr_, 2, 0, 1, 2);

            setLayout(my_layout_main_ptr_);

            //pushButtonConnect_.setFocus();
         }

         void ConnectDialog::do_setup_events_(void)
         {
             connect(my_pushbutton_connect_ptr_, &QPushButton::clicked, \
                     this, &QDialog::accept);

             connect(my_pushbutton_cancel_ptr_, &QPushButton::clicked, \
                     this, &QDialog::reject);
         }
      }
   }
}
