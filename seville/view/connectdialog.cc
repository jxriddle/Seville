#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGuiApplication>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "seville/view/connectdialog.h"

namespace seville
{
   namespace view
   {
      ConnectDialog::ConnectDialog(QWidget* widget_parent_ptr)
         : QDialog(widget_parent_ptr)
      {
         do_setupView();
         do_setupEvents();
      }

      auto ConnectDialog::New(QWidget* widget_parent_ptr)
            -> std::unique_ptr<ConnectDialog>
      {
         auto instance = std::make_unique<ConnectDialog>(widget_parent_ptr);
         //std::make_optional<ConnectDialog*>();

         //if (object.has_value()) {
            //object.value()->ok_setupView();
            //object.value()->ok_setupEvents();
         //}
         //instance->do_setup_view_();
         //instance->do_setup_events_();

         return instance;
      }

      void ConnectDialog::do_setupView(void)
      {
         // Window
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         //setWindowTitle(QGuiApplication::applicationDisplayName());
         setWindowTitle("Connect");

         // Layout
         my_mainLayoutPtr = new QGridLayout(this);

         // ButtonBox
         my_mainButtonBoxPtr = new QDialogButtonBox(this);

         // PushButtons
         my_cancelPushButtonPtr = new QPushButton(tr("&Cancel"), this);

         my_connectPushButtonPtr = new QPushButton(tr("C&onnect"), this);
         my_connectPushButtonPtr->setDefault(true);
         my_connectPushButtonPtr->setEnabled(true);

         // LineEdit
         my_urlLineEditPtr = new QLineEdit(this);

         my_usernameLineEditPtr = new QLineEdit(this);

         // Labels
         my_urlLabelPtr = new QLabel(tr("&Address"), this);
         my_urlLabelPtr->setBuddy(my_urlLineEditPtr);

         my_usernameLabelPtr = new QLabel(tr("&Username"), this);
         my_usernameLabelPtr->setBuddy(my_usernameLineEditPtr);

         // Arrangement
         my_mainButtonBoxPtr->addButton(
                  my_connectPushButtonPtr, QDialogButtonBox::ActionRole);
         my_mainButtonBoxPtr->addButton(
                  my_cancelPushButtonPtr, QDialogButtonBox::RejectRole);

         my_mainLayoutPtr->addWidget(my_urlLabelPtr, 0, 0);
         my_mainLayoutPtr->addWidget(my_urlLineEditPtr, 0, 1);

         my_mainLayoutPtr->addWidget(my_usernameLabelPtr, 1, 0);
         my_mainLayoutPtr->addWidget(my_usernameLineEditPtr, 1, 1);

         my_mainLayoutPtr->addWidget(my_mainButtonBoxPtr, 2, 0, 1, 2);

         setLayout(my_mainLayoutPtr);

         //pushButtonConnect_.setFocus();
      }

      void ConnectDialog::do_setupEvents(void)
      {
          connect(my_connectPushButtonPtr, &QPushButton::clicked,
                  this, &QDialog::accept);

          connect(my_cancelPushButtonPtr, &QPushButton::clicked,
                  this, &QDialog::reject);
      }
   }
}
