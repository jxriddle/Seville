#ifndef UI_CLIENT_WIDGET_H
#define UI_CLIENT_WIDGET_H

#include <memory>

#include <QObject>

#include <QBitmap>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <QStatusBar>
#include <QTextEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "App.h"
#include "Log.h"

#include "Palace_Client.h"
#include "Palace_Logger.h"

//#include "View_MainWindow.h"
#include "connectdialog.h"

namespace Seville
{
   namespace View
   {
      class PalaceClientWidget : public QWidget
      {
      Q_OBJECT

      private:
         Palace::PalaceClient* my_pPalaceClient;

         QLabel* my_pBackgroundImageLabel;
         QScrollArea* my_pScrollArea;
         QVBoxLayout* my_pWidgetLayout;
         QLineEdit* my_pLineEditChat;
         //QStatusBar* my_pStatusBar;

         QPixmap my_backgroundImage;
         double my_scaleFactor;

//         void do_setBackgroundImageFromFile(QString imagePath)
//         {
//            QPixmap pixmap = QPixmap(imagePath);
//            do_setBackgroundImage(pixmap);
//         }

         //void doResizeBackgroundImage(QSize size)
         //{
         //}

         void do_promptNewConnection(QWidget* pParentWidget)
         {
            ConnectDialog dialog(pParentWidget);
            if (dialog.exec() == QDialog::Accepted) {
               my_pPalaceClient->connectToHost(dialog.host(), dialog.port(), dialog.username());
            }
         }

      protected:
         void mousePressEvent(QMouseEvent* pEvent)
         {
            (void)pEvent;
            if (my_pPalaceClient->connectionState() == Palace::PalaceClient::ConnectionState::Disconnected) {
               do_promptNewConnection(this);
            }
         }

         //void mousePressEvent(QMouseEvent* event);
         //void resizeEvent(QResizeEvent *event) override;

      protected slots:
         void on_roomBackgroundDidChange(void)
         {
            auto backgroundImage = my_pPalaceClient->currentRoomP()->backgroundImage();
            auto pixmap = QPixmap::fromImage(backgroundImage);

            my_pBackgroundImageLabel->setGeometry(0, 0, pixmap.width(), pixmap.height());
            my_pBackgroundImageLabel->setBackgroundRole(QPalette::Base);
            my_pBackgroundImageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            my_pBackgroundImageLabel->setScaledContents(true);
            //myBackgroundImageLabel->resize(backgroundImage_.size());
            my_pBackgroundImageLabel->setPixmap(pixmap);
            my_pBackgroundImageLabel->setMask(pixmap.mask());
            //myBackgroundImageLabel->setGeometry(0, 0, 500, 500);

            //my_pBackgroundImageLabel->setGeometry(0, 0, pixmap.width(), pixmap.height());
            //my_pScrollArea->setGeometry(0, 0, pixmap.width(), pixmap.height());

            my_pScrollArea->setBackgroundRole(QPalette::Dark);
            my_pScrollArea->setWidget(my_pBackgroundImageLabel);
            //setFixedSize(pixmap.width(), pixmap.height());

            //myScrollArea->setGeometry(pixmap.rect());
            //myScrollArea->setGeometry(myBackgroundImageLabel->geometry());
            my_pScrollArea->setVisible(true);

            setGeometry(this->x(), this->y(), pixmap.width(), pixmap.height());
            parentWidget()->resize(pixmap.width(), pixmap.height());

            app_pMainWindow->
         }

         void on_chatTextEntry(void)
         {

         }

      private:
         void do_setupView(void)
         {
            my_pWidgetLayout = new QVBoxLayout(this);
            my_pScrollArea = new QScrollArea(this);
            my_pBackgroundImageLabel = new QLabel(my_pScrollArea);
            my_pWidgetLayout->setMargin(0);
            my_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_pScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            //myWidgetLayout->addStretch(1);
            my_pWidgetLayout->setContentsMargins(0, 0, 0, 0);
            my_pWidgetLayout->setSpacing(0);

            auto pScrollAreaLayout = new QHBoxLayout(my_pScrollArea);
            pScrollAreaLayout->addWidget(my_pBackgroundImageLabel);

            pScrollAreaLayout->setMargin(0);
            pScrollAreaLayout->setContentsMargins(0, 0, 0, 0);
            pScrollAreaLayout->setSpacing(0);
            my_pScrollArea->setLayout(pScrollAreaLayout);
            my_pWidgetLayout->addWidget(my_pScrollArea);

            my_pLineEditChat = new QLineEdit(this);
            my_pWidgetLayout->addWidget(my_pLineEditChat);

            setLayout(my_pWidgetLayout);
         }

         void do_setupEvents(void)
         {
            //connect(
               //this, &Seville::View::ClientWidget::mousePressEvent,
               //this, &Seville::View::ClientWidget::onMousePressEvent);

            connect(my_pPalaceClient->currentRoomP(), &Seville::Palace::Room::backgroundChanged,
                  this, &Seville::View::PalaceClientWidget::on_roomBackgroundDidChange);

            connect(
               my_pLineEditChat, &QLineEdit::returnPressed,
               this, &Seville::View::PalaceClientWidget::on_chatTextEntry);
         }

         void do_setupRoom(void)
         {
            auto imageFilename = QString(":/assets/images/test-pattern.png"); //.arg("test-pattern.png");
            qCDebug(sevilleLog) << imageFilename;
            //PalRoom& room = myPalClient->currentRoom();
            //my_puPalaceClient->loggerP()->debug(imageFilename);
            //do_setBackgroundImageFromFile(imageFilename);
            QPixmap pixmap = QPixmap(imageFilename);
            Zed::ByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");

            //auto img = pixmap.toImage();
            auto pRoom = my_pPalaceClient->currentRoomP();
            pRoom->setBackgroundImageBytes(byteArray);
         }

         void do_setupLog(void)
         {
            my_pPalaceClient->loggerP()->info("Seville Ready");
         }

         void do_deinit(void)
         {
            //delete myPalaceClient;
         }

         void do_init(void)
         {
            my_pPalaceClient = new Palace::PalaceClient(this);

            do_setupView();
            do_setupEvents();

            do_setupRoom();
            do_setupLog();
         }

      public:
         Palace::PalaceClient* palaceClientP(void) { return my_pPalaceClient; }

         void promptNewConnection(void) { do_promptNewConnection(this); }

         //      void PalRoomWidget::resizeEvent(QResizeEvent *event)
         //      {
         //         myScrollArea->resize(event->size());
         //         //myBackgroundImageLabel->resize(event->size());
         //         //doResizeBackgroundImage(event->size());
         //         QWidget::resizeEvent(event);
         //      }

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

         virtual ~PalaceClientWidget(void);
         explicit PalaceClientWidget(QWidget* pParent = nullptr);
      };
   }
}

#endif // UI_CLIENT_WIDGET_H
