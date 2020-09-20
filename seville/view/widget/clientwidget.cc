#include <memory>

#include <QBitmap>
#include <QDebug>
#include <QLoggingCategory>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Log.h"
#include "Palace_Client.h"
#include "connectdialog.h"
#include "clientwidget.h"

namespace seville
{
   namespace view
   {
      ClientWidget::ClientWidget(QWidget *parent) : QWidget(parent)
      {
         myPalaceClient = new Palace::Client(this);

         doSetupView();
         doSetupEvents();
      }

      //ClientWidget::~ClientWidget()
      //{
         //delete myPalaceClient;
      //}

//      void PalRoomWidget::resizeEvent(QResizeEvent *event)
//      {
//         myScrollArea->resize(event->size());
//         //myBackgroundImageLabel->resize(event->size());
//         //doResizeBackgroundImage(event->size());
//         QWidget::resizeEvent(event);
//      }

      void ClientWidget::doSetupView()
      {
         myWidgetLayout = new QVBoxLayout(this);
         myScrollArea = new QScrollArea(this);
         myBackgroundImageLabel = new QLabel(myScrollArea);
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

         auto scrollAreaLayout = new QHBoxLayout(myScrollArea);
         scrollAreaLayout->addWidget(myBackgroundImageLabel);

         scrollAreaLayout->setMargin(0);
         scrollAreaLayout->setContentsMargins(0, 0, 0, 0);
         scrollAreaLayout->setSpacing(0);
         myScrollArea->setLayout(scrollAreaLayout);
         myWidgetLayout->addWidget(myScrollArea);

         //PalRoom& room = myPalClient->currentRoom();
         auto imageFilename = QString(":/assets/images/test-pattern.png"); //.arg("test-pattern.png");
         qCDebug(sevilleChat) << imageFilename;
         doSetBackgroundImageFromFile(imageFilename);

         setLayout(myWidgetLayout);
      }

      void ClientWidget::doSetupEvents()
      {
         //connect(
            //this, &Seville::View::ClientWidget::mousePressEvent,
            //this, &Seville::View::ClientWidget::onMousePressEvent);
         connect(
               myPalaceClient, &seville::Palace::Client::backgroundChanged,
               this, &seville::view::ClientWidget::onBackgroundChanged
            );
      }

      void ClientWidget::doSetBackgroundImage(QPixmap& pixmap)
      {
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

      void ClientWidget::doSetBackgroundImageFromFile(QString imagePath)
      {
         QPixmap pixmap = QPixmap(imagePath);
         doSetBackgroundImage(pixmap);
      }

      void ClientWidget::doPromptNewConnection(QWidget* parent)
      {
         ConnectDialog dialog(parent);
         if (dialog.exec() == QDialog::Accepted) {
            myPalaceClient->connectToHost(
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

      void ClientWidget::mousePressEvent(QMouseEvent* event)
      {
         (void)event;
         if (myPalaceClient->connectionState() ==
             Palace::Client::ConnectionState::Disconnected) {
            doPromptNewConnection(this);
         }
      }

      void ClientWidget::onBackgroundChanged()
      {
         auto backgroundImage = myPalaceClient->currentRoom().backgroundImage();
         myBackgroundImageLabel->setGeometry(
                  0, 0, backgroundImage.width(), backgroundImage.height());
         auto pixmap = QPixmap::fromImage(backgroundImage);
         doSetBackgroundImage(pixmap);
      }
   }
}
