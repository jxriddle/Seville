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
      ConnectDialog::ConnectDialog(QWidget* parent_widget_pointer_)
         : QDialog(parent_widget_pointer_) {}

      ConnectDialog::~ConnectDialog(void) {}

      auto ConnectDialog::init(void) -> std::optional<ConnectDialog*>
      {
         auto object = std::make_optional<ConnectDialog*>();

         if (object.has_value()) {
            object.value()->ok_setupView();
            object.value()->ok_setupEvents();
         }

         return object;
      }

      void ConnectDialog::ok_setupView(void)
      {
         // Window
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         //setWindowTitle(QGuiApplication::applicationDisplayName());
         setWindowTitle("Connect");

         // Layout
         my_layout_main_pointer_ = new QGridLayout(this);

         // ButtonBox
         my_buttonbox_pointer_ = new QDialogButtonBox(this);

         // PushButtons
         my_pushbutton_cancel_pointer_ = new QPushButton(tr("&Cancel"), this);

         my_pushbutton_connect_pointer_ = new QPushButton(tr("C&onnect"), this);
         my_pushbutton_connect_pointer_->setDefault(true);
         my_pushbutton_connect_pointer_->setEnabled(true);

         // LineEdit
         my_lineedit_url_pointer_ = new QLineEdit(this);

         my_lineedit_username_pointer_ = new QLineEdit(this);

         // Labels
         my_label_url_pointer_ = new QLabel(tr("&Address"), this);
         my_label_url_pointer_->setBuddy(my_lineedit_url_pointer_);

         my_label_username_pointer_ = new QLabel(tr("&Username"), this);
         my_label_username_pointer_->setBuddy(my_lineedit_username_pointer_);

         // Arrangement
         my_buttonbox_pointer_->addButton(my_pushbutton_connect_pointer_, QDialogButtonBox::ActionRole);
         my_buttonbox_pointer_->addButton(my_pushbutton_cancel_pointer_, QDialogButtonBox::RejectRole);

         my_layout_main_pointer_->addWidget(my_label_url_pointer_, 0, 0);
         my_layout_main_pointer_->addWidget(my_lineedit_url_pointer_, 0, 1);

         my_layout_main_pointer_->addWidget(my_label_username_pointer_, 1, 0);
         my_layout_main_pointer_->addWidget(my_lineedit_username_pointer_, 1, 1);

         my_layout_main_pointer_->addWidget(my_buttonbox_pointer_, 2, 0, 1, 2);

         setLayout(my_layout_main_pointer_);

         //pushButtonConnect_.setFocus();
      }

      void ConnectDialog::ok_setupEvents(void)
      {
          connect(my_pushbutton_connect_pointer_, &QPushButton::clicked, this, &QDialog::accept);
          connect(my_pushbutton_cancel_pointer_, &QPushButton::clicked, this, &QDialog::reject);
      }

   }
}
