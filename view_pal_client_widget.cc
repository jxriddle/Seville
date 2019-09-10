#include <QBitmap>
#include <QDebug>
#include <QLoggingCategory>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "applog.h"
#include "pal_client.h"
#include "view_connect_dialog.h"
#include "view_pal_client_widget.h"

namespace Seville
{
   namespace View
   {
      PalClientWidget::PalClientWidget(QWidget *parent) : QWidget(parent)
      {
         myPalClient = new PalClient(this);
         doSetupUi();
         doSetupEvents();
      }

      //UiPalRoomWidget::~UiPalRoomWidget()
      //{
         //delete palClient_;
      //}

      PalClient *PalClientWidget::PalaceClient()
      {
         return myPalClient;
      }

//      void PalRoomWidget::resizeEvent(QResizeEvent *event)
//      {
//         myScrollArea->resize(event->size());
//         //myBackgroundImageLabel->resize(event->size());
//         //doResizeBackgroundImage(event->size());
//         QWidget::resizeEvent(event);
//      }

      void PalClientWidget::doSetupUi()
      {
         myScrollArea = new QScrollArea(this);
         myBackgroundImageLabel = new QLabel(this);
         myWidgetLayout = new QVBoxLayout(this);
         myWidgetLayout->setMargin(0);
         myScrollArea->setHorizontalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);
         myScrollArea->setVerticalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);
         myScrollArea->setSizePolicy(
                  QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
         //myWidgetLayout->addStretch(1);
         myWidgetLayout->setContentsMargins(0, 0, 0, 0);
         myWidgetLayout->setSpacing(0);

         auto scrollAreaLayout = new QHBoxLayout(this);
         scrollAreaLayout->addWidget(myBackgroundImageLabel);

         scrollAreaLayout->setMargin(0);
         scrollAreaLayout->setContentsMargins(0, 0, 0, 0);
         scrollAreaLayout->setSpacing(0);
         myScrollArea->setLayout(scrollAreaLayout);
         myWidgetLayout->addWidget(myScrollArea);

         //PalRoom& room = myPalClient->currentRoom();
         auto imageFilename = QString(":/assets/images/test-pattern.png"); //.arg("test-pattern.png");
         qCDebug(appLcIo) << imageFilename;
         doSetBackgroundImage(imageFilename);

         setLayout(myWidgetLayout);
      }

      void PalClientWidget::doSetupEvents()
      {
         connect(
            this, &PalClientWidget::mousePressEvent,
            this, &Seville::View::PalClientWidget::mousePressEvent);
      }

      void PalClientWidget::doSetBackgroundImage(QString imagePath)
      {
         QPixmap pixmap = QPixmap(imagePath);
         myBackgroundImageLabel->setBackgroundRole(QPalette::Base);
         myBackgroundImageLabel->setSizePolicy(
                  QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
         myBackgroundImageLabel->setScaledContents(true);
         //myBackgroundImageLabel->resize(backgroundImage_.size());
         myBackgroundImageLabel->setPixmap(pixmap);
         myBackgroundImageLabel->setMask(pixmap.mask());
         //myBackgroundImageLabel->setGeometry(0, 0, 500, 500);

         myScrollArea->setBackgroundRole(QPalette::Dark);
         myScrollArea->setWidget(myBackgroundImageLabel);
         //myScrollArea->setGeometry(pixmap.rect());
         //myScrollArea->setGeometry(myBackgroundImageLabel->geometry());
         myScrollArea->setVisible(true);
      }

      void PalClientWidget::doPromptNewConnection()
      {
         ConnectDialog dialog;
         if (dialog.exec() == QDialog::Accepted) {
            myPalClient->connectToHost(
                     dialog.host(), dialog.port(), dialog.username());
         }
      }

//      void PalRoomWidget::doResizeBackgroundImage(QSize size)
//      {
//         myBackgroundImageLabel->setGeometry(
//                  myBackgroundImageLabel->x(),
//                  myBackgroundImageLabel->y(),
//                  size.rwidth(),
//                  size.rheight());
//      }

//      void PalRoomWidget::updateGeometry()
//      {
//         myBackgroundImageLabel->setGeometry(
//                  0, 0, size().height(), size().width());
//      }
      //void onBackgroundImageLabelResized(QResizeEvent *event)
      //{

      //}

      void PalClientWidget::mousePressEvent(QMouseEvent *event)
      {
         (void)event;
         if (myPalClient->state() == PalClient::ConnectionState::Disconnected) {
            doPromptNewConnection();
         }
      }
   }
}
