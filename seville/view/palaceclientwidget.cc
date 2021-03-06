#include <memory>

#include <QDir>
#include <QImage>
#include <QNetworkAccessManager>
#include <QWidget>

#include <QBitmap>
#include <QDebug>
#include <QLoggingCategory>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "seville/base/sevilleapp.h"
#include "seville/palace/log.h"
#include "seville/palace/client.h"
#include "seville/view/connectdialog.h"
#include "seville/view/palaceclientwidget.h"
#include "seville/view/mainwindow.h"

namespace seville
{
   namespace view
   {
      view::MainWindow* PalaceClientWidget::do_mainWindowPtr(void)
      {
         foreach (auto w, qApp->topLevelWidgets())
            if (auto mainWindowPtr = qobject_cast<view::MainWindow*>(w))
               return mainWindowPtr;
         return nullptr;
      }

      PalaceClientWidget::PalaceClientWidget(QWidget* parentWidgetPtr)
         : QWidget(parentWidgetPtr)
      {
         my_palaceClientPtr = new seville::palace::Client(this);

         do_setupView();
         do_setupEvents();
         do_setupSizing();
      }

      //ClientWidget::~ClientWidget(void)
      //{
         //delete myPalaceClient;
      //}

//      void PalRoomWidget::resizeEvent(QResizeEvent* resize_event_ptr)
//      {
//         myScrollArea->resize(event->size());
//         //myBackgroundImageLabel->resize(event->size());
//         //doResizeBackgroundImage(event->size());
//         QWidget::resizeEvent(event);
//      }

      void PalaceClientWidget::do_setupView(void)
      {
         my_mainLayoutPtr = new QVBoxLayout(this);

         //my_address_line_edit_ptr_ = new QLineEdit(this);
         //my_layout_ptr_->addWidget(my_address_line_edit_ptr_);

         my_scrollAreaPtr = new QScrollArea(this);

         my_backgroundImageLabelPtr = new QLabel(this);

         my_scrollAreaPtr->setHorizontalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         my_scrollAreaPtr->setVerticalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         my_scrollAreaPtr->setSizePolicy(
                  QSizePolicy::Preferred, QSizePolicy::Preferred);

         my_scrollAreaPtr->setWidgetResizable(false);
         my_scrollAreaPtr->setWidget(my_backgroundImageLabelPtr);

         //my_scrollarea_ptr_->setStyleSheet("border 1px solid red");

//            auto layout_scroll_area_ptr = new QHBoxLayout(my_scrollarea_ptr_);
//            layout_scroll_area_ptr->addWidget(my_label_image_background_ptr_);
//            layout_scroll_area_ptr->setStretchFactor(my_scrollarea_ptr_, 1);

//            layout_scroll_area_ptr->setMargin(0);
//            layout_scroll_area_ptr->setContentsMargins(0, 0, 0, 0);
//            layout_scroll_area_ptr->setSpacing(0);
//            my_scrollarea_ptr_->setLayout(layout_scroll_area_ptr);

         my_mainLayoutPtr->addWidget(my_scrollAreaPtr);

         //PalRoom& room = myPalClient->currentRoom();
               //.arg("test-pattern.png");
         //qCDebug(SevilleApp()->log_category()) << filename_image;
         //qCDebug(log_seville) << image_filename;

         //my_chatLineEditPtr = new QLineEdit(this);
         //my_layoutPtr->addWidget(my_chatLineEditPtr);

         setLayout(my_mainLayoutPtr);

         //do_clearBackgroundImage();
         auto filename = QString(":/seville/assets/images/test-pattern.png");
         do_setBackgroundImageFromFile(filename);

         //setStyleSheet("border: 1px solid red");
      }

      void PalaceClientWidget::do_setupEvents(void)
      {
         //connect(
            //this, &Seville::View::ClientWidget::mousePressEvent,
            //this, &Seville::View::ClientWidget::onMousePressEvent);

         connect(my_palaceClientPtr,
                 &seville::palace::Client::
                 backgroundImageDidLoadEvent,
                 this,
                 &seville::view::PalaceClientWidget::
                 on_backgroundImageDidLoad);

         connect(my_palaceClientPtr,
                 &seville::palace::Client::
                 connectionStateDidChangeEvent,
                 this,
                 &seville::view::PalaceClientWidget::
                 on_connectionStateDidChange);

         //auto mainWindowPtr = static_cast<view::MainWindow*>(this->parent()->parent());
         auto mainWindowPtr = do_mainWindowPtr(); //sevilleApp.mainWindowPtr();
         connect(my_palaceClientPtr,
                 &seville::palace::Client::connectionStateDidChangeEvent,
                 mainWindowPtr,
                 &seville::view::MainWindow::on_clientConnectionStateDidChange);

//         connect(my_chatLineEditPtr,
//                 &QLineEdit::returnPressed,
//                 this,
//                 &seville::view::PalaceClientWidget::
//                 on_chatLineEditDidPressReturn);
      }

      auto PalaceClientWidget::do_setupSizing(void) -> void
      {
         my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);

         //my_primary_layout_ptr_->setMargin(0);
         //my_primary_layout_ptr_->setContentsMargins(0, 0, 0, 0);
         //my_primary_layout_ptr_->setSpacing(0);
         //my_primary_layout_ptr_->addStretch(1);
      }

      void PalaceClientWidget::do_setBackgroundImage(const QPixmap& pixmap)
      {
         my_backgroundImagePixmap = pixmap;

         my_backgroundImageLabelPtr->setBackgroundRole(QPalette::Base);
         my_backgroundImageLabelPtr->setPixmap(pixmap);
         my_backgroundImageLabelPtr->setMask(pixmap.mask());
         my_backgroundImageLabelPtr->adjustSize();
         //my_backgroundImageLabelPtr->resize(my_backgroundImageLabelPtr->minimumSizeHint());

         my_scrollAreaPtr->setBackgroundRole(QPalette::Dark);
         //my_scrollAreaPtr->resize(my_scrollAreaPtr->minimumSizeHint());
         my_scrollAreaPtr->updateGeometry();
         my_scrollAreaPtr->adjustSize();

         updateGeometry();
         adjustSize();

//         if (my_palaceClientPtr != nullptr) {
//            auto scrollAreaSizeHint = my_scrollAreaPtr->sizeHint();
//            my_palaceClientPtr->loggerPtr()->appendDebugMessage(
//                     QString("my_scrollAreaPtr->sizeHint(): %1x%2")
//                     .arg(scrollAreaSizeHint.width())
//                     .arg(scrollAreaSizeHint.height()));

//            auto backgroundImageLabelSizeHint =
//                  my_backgroundImageLabelPtr->sizeHint();
//            my_palaceClientPtr->loggerPtr()->appendDebugMessage(
//                     QString("my_backgroundImageLabelPtr->sizeHint(): %1x%2")
//                     .arg(backgroundImageLabelSizeHint.width())
//                     .arg(backgroundImageLabelSizeHint.height()));

//            auto palaceClientWidgetSizeHint = sizeHint();
//            my_palaceClientPtr->loggerPtr()->appendDebugMessage(
//                     QString("palaceClientWidget.sizeHint(): %1x%2")
//                     .arg(palaceClientWidgetSizeHint.width())
//                     .arg(palaceClientWidgetSizeHint.height()));
//         }

//         auto pixmapSize = my_backgroundImagePixmap.size();

         emit widgetBackgroundDidChangeEvent();
//                  pixmapSize.width(), pixmapSize.height());
      }

      void PalaceClientWidget::do_setBackgroundImageFromFile(
            const QString& imagePath)
      {
         QPixmap pixmap = QPixmap(imagePath);
         do_setBackgroundImage(pixmap);
      }

      void PalaceClientWidget::do_promptOpenConnection(
            QWidget* parentPtr)
      {
         seville::view::ConnectDialog dialog(parentPtr);
         if (dialog.exec() == QDialog::Accepted) {
            my_palaceClientPtr->connectToHost(
                     dialog.host(), dialog.port(), dialog.username());
         }
      }

//      void PalaceClientWidget::do_clearBackgroundImage(void)
//      {
//         auto filename = QString(":/seville/assets/images/test-pattern.png");
//         do_setBackgroundImageFromFile(filename);
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

      void PalaceClientWidget::mousePressEvent(QMouseEvent* mouseEventPtr)
      {
         (void)mouseEventPtr;
         if (my_palaceClientPtr->connectionState() ==
             palace::ConnectionState::kDisconnectedState) {
            do_promptOpenConnection(this);
         }
      }

      //auto ClientWidget::on_background_image_did_change(int width, int height) -> void
      //{
         //auto background_image =
         //      my_palace_client_ptr_->current_room_ptr()->background_image();
         //my_background_image_label_ptr_->setGeometry(
         //         0, 0, background_image.width(), background_image.height());
         //my_layout_widget_ptr_->setGeometry();
         //auto pixmap = QPixmap::fromImage(background_image);
         //do_set_background_image_(pixmap);
      //}

      auto PalaceClientWidget::on_backgroundImageDidLoad(void) -> void
      {
         auto backgroundImage =
               my_palaceClientPtr->roomPtr()->backgroundImage();

         auto pixmap = QPixmap::fromImage(backgroundImage);
         do_setBackgroundImage(pixmap);

         adjustSize();

         auto mainWindowPtr = do_mainWindowPtr();
         if (mainWindowPtr != nullptr) {
            mainWindowPtr->adjustSize();
         }


      }

      auto PalaceClientWidget::on_connectionStateDidChange(
            palace::ConnectionState connectionState) -> void
      {
         (void)connectionState;
         //if (palace::ConnectionState::kHandshakingState ==
         //      connectionState) {
         //   (void)connectionState;
         //}
         //if (connectionState == palace::ConnectionState::kDisconnectedState)
         //   do_clearBackgroundImage();
      }

//      auto PalaceClientWidget::on_chatLineEditDidPressReturn(void) -> void
//      {
//         auto text = my_chatLineEditPtr->text();
//         my_palaceClientPtr->chat(text);
//         my_chatLineEditPtr->clear();
//      }
   }
}
