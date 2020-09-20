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

namespace seville
{
   namespace view
   {
      class ConnectDialog : public QDialog
      {
      Q_OBJECT

      private:
         QGridLayout* my_pLayout;
         QLabel* my_pLabelUsername;
         QLineEdit* my_pLineEditUsername;
         QLabel* my_pLabelUrl;
         QLineEdit* my_pLineEditUrl;
         QPushButton* my_pPushButtonConnect;
         QPushButton* my_pPushButtonCancel;
         QDialogButtonBox* my_pButtonBox;

         void do_setupView()
         {
            // Window
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            //setWindowTitle(QGuiApplication::applicationDisplayName());
            setWindowTitle("Connect");

            // Layout
            my_pLayout = new QGridLayout(this);

            // ButtonBox
            my_pButtonBox = new QDialogButtonBox(this);

            // PushButtons
            my_pPushButtonCancel = new QPushButton(tr("&Cancel"), this);

            my_pPushButtonConnect = new QPushButton(tr("C&onnect"), this);
            my_pPushButtonConnect->setDefault(true);
            my_pPushButtonConnect->setEnabled(true);

            // LineEdit
            my_pLineEditUrl = new QLineEdit(this);

            my_pLineEditUsername = new QLineEdit(this);

            // Labels
            my_pLabelUrl = new QLabel(tr("&Address"), this);
            my_pLabelUrl->setBuddy(my_pLineEditUrl);

            my_pLabelUsername = new QLabel(tr("&Username"), this);
            my_pLabelUsername->setBuddy(my_pLineEditUsername);

            // Arrangement
            my_pButtonBox->addButton(my_pPushButtonConnect, QDialogButtonBox::ActionRole);
            my_pButtonBox->addButton(my_pPushButtonCancel, QDialogButtonBox::RejectRole);

            my_pLayout->addWidget(my_pLabelUrl, 0, 0);
            my_pLayout->addWidget(my_pLineEditUrl, 0, 1);

            my_pLayout->addWidget(my_pLabelUsername, 1, 0);
            my_pLayout->addWidget(my_pLineEditUsername, 1, 1);

            my_pLayout->addWidget(my_pButtonBox, 2, 0, 1, 2);

            setLayout(my_pLayout);

            //pushButtonConnect_.setFocus();
         }

         void do_setupEvents()
         {
            connect(my_pPushButtonConnect, &QPushButton::clicked, this, &QDialog::accept);
            connect(my_pPushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
         }

         void do_deinit()
         {

         }

         void do_init()
         {
            do_setupView();
            do_setupEvents();
         }

      public:
         QString address()
         {
            return my_pLineEditUrl->text();
         }

         QString host()
         {
            QStringList sl = my_pLineEditUrl->text().split(':');
            return sl.first();
         }

         quint16 port()
         {
            QStringList sl = my_pLineEditUrl->text().split(':');
            return sl.last().toUShort();
         }

         QString username()
         {
            return my_pLineEditUsername->text();
         }

         virtual ~ConnectDialog();
         explicit ConnectDialog(QWidget* parent = nullptr);

      signals:

      public slots:
//       void on_pushButtonConnectClicked();
//       void on_pushButtonCancelClicked();
      };
   }
}

#endif // VIEW_CONNECTDIALOG_H
