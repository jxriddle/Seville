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
#include <QScrollBar>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QFontMetrics>
#include <QGraphicsPathItem>
#include <QGraphicsDropShadowEffect>

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
         do_init();
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

      auto PalaceClientWidget::do_init(void) -> void
      {
         kNameColors[0] = QColor::fromRgb(240, 152, 152); // red
         kNameColors[1] = QColor::fromRgb(238, 171, 135); // orange red
         kNameColors[2] = QColor::fromRgb(237, 200, 126); // orange
         kNameColors[3] = QColor::fromRgb(238, 225, 135); // yellow
         kNameColors[4] = QColor::fromRgb(181, 237, 126); // light green
         kNameColors[5] = QColor::fromRgb(135, 238, 135); // green
         kNameColors[6] = QColor::fromRgb(135, 238, 186); // blue green
         kNameColors[7] = QColor::fromRgb(135, 238, 215); // green blue
         kNameColors[9] = QColor::fromRgb(126, 237, 237); // light blue
         kNameColors[10] = QColor::fromRgb(143, 193, 239); // blue
         kNameColors[11] = QColor::fromRgb(126, 254, 237); // purple
         kNameColors[12] = QColor::fromRgb(138, 126, 237); // violet
         kNameColors[13] = QColor::fromRgb(210, 100, 233); // light violet?
         kNameColors[14] = QColor::fromRgb(235, 117, 204); // pink
         kNameColors[15] = QColor::fromRgb(235, 117, 163); // light red

         my_palaceClientPtr = new seville::palace::Client(this);
         auto spritesheetPath =
               QString(":/seville/assets/images/defaultsmileys.png");
         //auto rect = QRect(0, 0, 45, 45);
         my_spritesheet = QImage(spritesheetPath);
         //my_sprite = my_spritesheet.copy(rect);
      }

      void PalaceClientWidget::do_setupView(void)
      {
         my_mainLayoutPtr = new QVBoxLayout(this);

         //my_address_line_edit_ptr_ = new QLineEdit(this);
         //my_layout_ptr_->addWidget(my_address_line_edit_ptr_);

         my_scrollAreaPtr = new QScrollArea(this);

         // my_backgroundImageLabelPtr = new QLabel(this);
         my_roomWidgetPtr = new QWidget(this);

         my_scrollAreaPtr->setHorizontalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         my_scrollAreaPtr->setVerticalScrollBarPolicy(
                  Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         my_scrollAreaPtr->setSizePolicy(
                  QSizePolicy::Preferred, QSizePolicy::Preferred);
         //my_scrollAreaPtr->setSizePolicy(
         //         QSizePolicy::Preferred, QSizePolicy::Preferred);
         auto scrollAreaLayoutPtr = new QVBoxLayout(my_scrollAreaPtr);
         my_scrollAreaPtr->setLayout(scrollAreaLayoutPtr);
         //my_scrollAreaPtr->setSizePolicy(
         //         QSizePolicy::Maximum, QSizePolicy::Maximum);

         my_scrollAreaPtr->setWidgetResizable(false);
         // my_scrollAreaPtr->setWidget(my_backgroundImageLabelPtr);
         my_scrollAreaPtr->setWidget(my_roomWidgetPtr);

         // my_scrollAreaPtr->setFixedSize(my_backgroundImageLabelPtr->size());
         my_scrollAreaPtr->setFixedSize(my_roomWidgetPtr->size());
         //my_scrollAreaPtr->setFixedWidth(my_backgroundImageLabelPtr->width() + 10);
         //my_scrollAreaPtr->setFixedHeight(my_backgroundImageLabelPtr->height() + 10);

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

         my_chatLineEditPtr = new QLineEdit(this);
         my_mainLayoutPtr->addWidget(my_chatLineEditPtr);

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

         connect(my_palaceClientPtr,
                 &seville::palace::Client::viewNeedsUpdatingEvent,
                 this,
                 &seville::view::PalaceClientWidget::on_viewNeedsUpdating);

         //auto mainWindowPtr =
         //static_cast<view::MainWindow*>(this->parent()->parent());
         //auto mainWindowPtr = do_mainWindowPtr(); //sevilleApp.mainWindowPtr();
         //connect(my_palaceClientPtr,
         //        &seville::palace::Client::connectionStateDidChangeEvent,
         //        mainWindowPtr,
         //        &seville::view::MainWindow::on_clientConnectionStateDidChange);

         connect(my_chatLineEditPtr,
                 &QLineEdit::returnPressed,
                 this,
                 &seville::view::PalaceClientWidget::
                 on_chatLineEditDidPressReturn);
      }

      auto PalaceClientWidget::do_setupSizing(void) -> void
      {
         //my_mainLayoutPtr->setSizeConstraint(QLayout::SetMinimumSize);

         //my_primary_layout_ptr_->setMargin(0);
         //my_primary_layout_ptr_->setContentsMargins(0, 0, 0, 0);
         //my_primary_layout_ptr_->setSpacing(0);
         //my_primary_layout_ptr_->addStretch(1);
         //my_mainLayoutPtr->setSizeConstraint(QLayout::SetFixedSize);
         //my_scrollAreaPtr->layout()->setSizeConstraint(QLayout::SetFixedSize);
      }

      //void PalaceClientWidget::do_setBackgroundImage(const QPixmap& pixmap)
      void PalaceClientWidget::do_setBackgroundImage(const QImage& image)
      {
         // QPainter painter;

         //auto backgroundImageLabelSize0 = my_backgroundImageLabelPtr->size();
         my_backgroundImage = image;
         //my_backgroundImage  = pixmap.toImage();

         //my_backgroundImageLabelPtr->setStyleSheet("border: 1px solid red");
         // my_roomWidgetPtr->setBackgroundRole(QPalette::Base);
         // painter.drawImage()
         // my_backgroundImageLabelPtr->setBackgroundRole(QPalette::Base);
         // my_backgroundImageLabelPtr->setPixmap(pixmap);
         // my_backgroundImageLabelPtr->setMask(pixmap.mask());
         //my_backgroundImageLabelPtr->setFixedSize(pixmap.size());
         // my_backgroundImageLabelPtr->adjustSize();
         //my_backgroundImageLabelPtr->resize(
         //       my_backgroundImageLabelPtr->minimumSizeHint());

         //nauto backgroundImageLabelSizeDelta =
         //      my_backgroundImageLabelPtr->size() - backgroundImageLabelSize0;

         //my_scrollAreaPtr->setSizePolicy(QSizePolicy::Fixed);
         //my_scrollAreaPtr->setBackgroundRole(QPalette::Dark);
         //my_scrollAreaPtr->resize(my_backgroundImageLabelPtr->size());
         //my_scrollAreaPtr->resize(my_scrollAreaPtr->minimumSizeHint());
         //my_scrollAreaPtr->setMaximumSize(pixmap.size());
         //my_scrollAreaPtr->setFixedSize(my_backgroundImageLabelPtr->size());
         //my_scrollAreaPtr->setFixedWidth(
         //         my_backgroundImageLabelPtr->width() + 4);
         my_scrollAreaPtr->setFixedWidth(
                  my_backgroundImage.width() + 4);
                  //my_backgroundImageLabelPtr->width() + 4);
         //auto frameWidth = my_backgroundImageLabelPtr->frameWidth();
         //(void)frameWidth;
                  //my_scrollAreaPtr->horizontalScrollBar()->size().width());
         my_scrollAreaPtr->setFixedHeight(
                  my_backgroundImage.height() + 4);
                  //my_backgroundImageLabelPtr->height() + 4);
         //my_scrollAreaPtr->setStyleSheet("border: 1px solid red");
         //my_scrollAreaPtr->updateGeometry();
         my_scrollAreaPtr->adjustSize();

         //updateGeometry();
         //adjustSize();

//         auto mainWindowPtr = static_cast<QMainWindow*>(parent());
//         if (mainWindowPtr != nullptr) {
//            //mainWindowPtr->resize(pixmap.size());
//            mainWindowPtr->updateGeometry();
//            mainWindowPtr->adjustSize();

//            mainWindowPtr->window()->updateGeometry();
//            mainWindowPtr->window()->adjustSize();

//            //mainWindowPtr->window()->layout()->update();
//            //mainWindowPtr->window()->adjustSize();
//         }

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

//         emit widgetBackgroundDidChangeEvent();
//                  pixmapSize.width(), pixmapSize.height());
      }

      void PalaceClientWidget::do_setBackgroundImageFromFile(
            const QString& imagePath)
      {
         // QPixmap pixmap = QPixmap(imagePath);
         auto image = QImage(imagePath);
         do_setBackgroundImage(image);
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

//      auto PalaceClientWidget::do_drawRoom(void) -> void
//      {
//         if (my_palaceClientPtr == nullptr)
//            return;

//         // my_palaceClientPtr->loggerPtr()->appendInfoMessage("Drawing users in room...");

//         QPainter painter;
//         painter.drawImage(0, 0, my_backgroundImage);

//         for (auto& user: *my_palaceClientPtr->roomPtr()->userListPtr()) {
//            auto rect =
//                  QRect(user.face() * kSmileyWidth,
//                        user.color() * kSmileyHeight,
//                        kSmileyWidth,
//                        kSmileyHeight);

//            QImage userImage = my_spritesheet.copy(rect);
//            painter.drawImage(user.x(), user.y(), userImage);

//            update();
//         }
//      }

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
         if (my_palaceClientPtr == nullptr)
            return;

         if (my_palaceClientPtr->connectionState() ==
             palace::ConnectionState::kDisconnectedState) {
            do_promptOpenConnection(this);
         }
         else if (my_palaceClientPtr->connectionState() ==
                  palace::ConnectionState::kConnectedState) {
            my_palaceClientPtr->move(mouseEventPtr->x() - kSmileyWidth/2,
                                     mouseEventPtr->y() - kSmileyHeight/2);
         }
      }

      void PalaceClientWidget::paintEvent(QPaintEvent* paintEventPtr)
      {
         (void)paintEventPtr;

         if (my_palaceClientPtr == nullptr)
            return;

         // my_palaceClientPtr->loggerPtr()->appendInfoMessage("Drawing users in room...");

         QPainter painter(this);
         painter.drawImage(8, 8, my_backgroundImage);

         for (auto& user: *my_palaceClientPtr->currentRoomPtr()->userListPtr())
         {
            auto rect =
                  QRect(user.face() * kSmileyWidth,
                        user.color() * kSmileyHeight,
                        kSmileyWidth,
                        kSmileyHeight);

            QImage userImage = my_spritesheet.copy(rect);
            painter.drawImage(user.x(), user.y(), userImage);

            // QBrush blackBrush(Qt::black);
            // QPen pen(Qt::gray);

            auto font = QFont("Sans-Serif", 10, QFont::Bold);
            // font.setWeight(3);

            auto fontMetrics = QFontMetrics(font);

            auto width = fontMetrics.boundingRect(
                     user.username()).width() + kDropShadowWidth;

            // auto nameTextRect = QRect();
            auto nameTextPoint =
                  QPoint(user.x() + (kSmileyWidth / 2) - (width / 2.0),
                         user.y() + 56);

            // nameTextRect.setX(user.x());
            // nameTextRect.setY(user.y());
            // nameTextRect.setWidth((kSmileyWidth / 2) - (width / 2));
            // nameTextRect.setHeight(user.y() + 58);


            // draw name drop shadow
            // painter.drawText(
            //    nameTextRect.adjusted(0, 2, 2, 0), user.username());

            // auto nameTextGraphic = QGraphicsTextItem();
            // auto dropShadowEffect = QGraphicsDropShadowEffect();
            // auto painterPath = QPainterPath();
            // painterPath.addText(
            //          nameTextPoint + QPoint(0, 1), font, user.username());

            // nameTextGraphic.set
            painter.setFont(font);
            painter.setPen(Qt::black);
            // painter.drawPath(painterPath);
            painter.drawText(nameTextPoint + QPoint(0, -1), user.username());
            painter.drawText(nameTextPoint + QPoint(-1, 0), user.username());
            painter.drawText(nameTextPoint + QPoint(1, 0), user.username());

            painter.setFont(font);
            painter.setPen(Qt::black);
            // painter.drawPath(painterPath);
            painter.drawText(nameTextPoint + QPoint(0, 2), user.username());

            // draw name
            // painter.setPen(Qt::Pen
            painter.setFont(font);
            if (0 <= user.face() && user.face() < kNumNameColors)
              painter.setPen(kNameColors[user.face()]);

            painter.drawText(nameTextPoint, user.username());
         }

         // update();
      }

      //auto ClientWidget::on_background_image_did_change(
      // int width, int height) -> void
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
               my_palaceClientPtr->currentRoomPtr()->backgroundImage();

         //auto pixmap = QPixmap::fromImage(backgroundImage);
         do_setBackgroundImage(backgroundImage);

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

      auto PalaceClientWidget::on_chatLineEditDidPressReturn(void) -> void
      {
         auto text = my_chatLineEditPtr->text();
         my_palaceClientPtr->chat(text);
         my_chatLineEditPtr->clear();
      }

      auto PalaceClientWidget::on_viewNeedsUpdating(void) -> void
      {
         update();
      }
   }
}
