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
#include <QComboBox>

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
//      view::MainWindow* PalaceClientWidget::do_mainWindowPtr(void)
//      {
//         foreach (auto w, qApp->topLevelWidgets())
//            if (auto mainWindowPtr = qobject_cast<view::MainWindow*>(w))
//               return mainWindowPtr;
//         return nullptr;
//      }

      PalaceClientWidget::PalaceClientWidget(QWidget* parentWidgetPtr)
         : QWidget(parentWidgetPtr)
      {
         do_init();
         do_setupView();
         do_setupEvents();
         do_setupSizing();
      }

      PalaceClientWidget::~PalaceClientWidget(void)
      {
         do_teardownEvents();
         do_deinit();
         //delete myPalaceClient;
      }

//      void PalRoomWidget::resizeEvent(QResizeEvent* resize_event_ptr)
//      {
//         myScrollArea->resize(event->size());
//         //myBackgroundImageLabel->resize(event->size());
//         //doResizeBackgroundImage(event->size());
//         QWidget::resizeEvent(event);
//      }

      void PalaceClientWidget::do_init(void)
      {
         kNameColors[0]  = QColor::fromRgb(0xe1, 0x2f, 0x2f); // red
         kNameColors[1]  = QColor::fromRgb(0xe0, 0x66, 0x26); // orange red
         kNameColors[2]  = QColor::fromRgb(0xdd, 0x9e, 0x20); // orange
         kNameColors[3]  = QColor::fromRgb(0xe3, 0xce, 0x38); // yellow
         kNameColors[4]  = QColor::fromRgb(0x82, 0xe0, 0x26); // light green
         kNameColors[5]  = QColor::fromRgb(0x28, 0xe0, 0x28); // green
         kNameColors[6]  = QColor::fromRgb(0x28, 0xe0, 0x83); // pale green
         kNameColors[7]  = QColor::fromRgb(0x2f, 0xe1, 0xba); // blue green
         kNameColors[9]  = QColor::fromRgb(0x38, 0xe3, 0xe3); // green blue
         kNameColors[10] = QColor::fromRgb(0x25, 0x86, 0xe1); // light blue
         kNameColors[11] = QColor::fromRgb(0x26, 0x55, 0xe0); // blue
         kNameColors[12] = QColor::fromRgb(0x4b, 0x38, 0xe3); // violet
         kNameColors[13] = QColor::fromRgb(0x87, 0x2f, 0xe1); // purple
         kNameColors[14] = QColor::fromRgb(0xbc, 0x20, 0xdd); // pink
         kNameColors[15] = QColor::fromRgb(0xe0, 0x26, 0xae); // red purple
         // kNameColors[16] = QColor::fromRgb(0xe0, 0x25, 0x6e);

         my_palaceClientPtr = new seville::palace::Client(this);
         auto spritesheetPath =
               QString(":/seville/assets/images/defaultsmileys.png");
         //auto rect = QRect(0, 0, 45, 45);
         my_spritesheet = QImage(spritesheetPath);
         //my_sprite = my_spritesheet.copy(rect);
      }

      void PalaceClientWidget::do_deinit(void)
      {

      }

      void PalaceClientWidget::do_setupView(void)
      {
         my_mainLayoutPtr = new QVBoxLayout(this);
         // my_mainLayoutPtr = new QVBoxLayout(this);

         //my_address_line_edit_ptr_ = new QLineEdit(this);
         //my_layout_ptr_->addWidget(my_address_line_edit_ptr_);

         // my_scrollAreaPtr = new QScrollArea(this);

         // my_backgroundImageLabelPtr = new QLabel(this);
         // my_roomWidgetPtr = new QLabel(my_scrollAreaPtr);
         my_roomWidgetPtr = new QLabel(this);

         // my_scrollAreaPtr->setHorizontalScrollBarPolicy(
         //          Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         // my_scrollAreaPtr->setVerticalScrollBarPolicy(
         //          Qt::ScrollBarPolicy::ScrollBarAsNeeded);

         // my_scrollAreaPtr->setSizePolicy(
         //          QSizePolicy::Preferred, QSizePolicy::Preferred);
         //my_scrollAreaPtr->setSizePolicy(
         //         QSizePolicy::Preferred, QSizePolicy::Preferred);
         // auto scrollAreaLayoutPtr = new QVBoxLayout(my_scrollAreaPtr);
         // my_scrollAreaPtr->setLayout(scrollAreaLayoutPtr);
         //my_scrollAreaPtr->setSizePolicy(
         //         QSizePolicy::Maximum, QSizePolicy::Maximum);

         // my_scrollAreaPtr->setWidgetResizable(false);
         // my_scrollAreaPtr->setWidget(my_backgroundImageLabelPtr);
         // my_scrollAreaPtr->setWidget(my_roomWidgetPtr);

         // my_scrollAreaPtr->setFixedSize(my_backgroundImageLabelPtr->size());
         // my_scrollAreaPtr->setFixedSize(my_roomWidgetPtr->size());
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

         // layout()->addWidget(my_scrollAreaPtr);
         layout()->addWidget(my_roomWidgetPtr);

         //PalRoom& room = myPalClient->currentRoom();
               //.arg("test-pattern.png");
         //qCDebug(SevilleApp()->log_category()) << filename_image;
         //qCDebug(log_seville) << image_filename;
         auto hBoxLayoutPtr = new QHBoxLayout();
         layout()->addItem(hBoxLayoutPtr);

         my_roomUserListComboBoxPtr = new QComboBox(this);
         my_roomUserListComboBoxPtr->setMinimumWidth(180);
         my_roomUserListComboBoxPtr->setMaximumWidth(180);
         // my_roomUserListComboBoxPtr->setStyleSheet("border: 1px solid red");
         hBoxLayoutPtr->addWidget(my_roomUserListComboBoxPtr);

         my_chatLineEditPtr = new QLineEdit(this);
         hBoxLayoutPtr->addWidget(my_chatLineEditPtr);

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

         connect(my_palaceClientPtr,
                 &seville::palace::Client::roomListWasReceivedEvent,
                 this,
                 &seville::view::PalaceClientWidget::on_roomListDidLoad);

         connect(my_roomUserListComboBoxPtr,
                 static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                 this,
                 &seville::view::PalaceClientWidget::on_roomWasSelected);

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

         connect(my_palaceClientPtr,
                 &seville::palace::Client::serverNameWasSetEvent,
                 this,
                 &seville::view::PalaceClientWidget::on_serverNameWasSet);
      }

      void PalaceClientWidget::do_teardownEvents(void)
      {
         //disconnect(
            //this, &Seville::View::ClientWidget::mousePressEvent,
            //this, &Seville::View::ClientWidget::onMousePressEvent);

         disconnect(my_palaceClientPtr,
                 &seville::palace::Client::
                 backgroundImageDidLoadEvent,
                 this,
                 &seville::view::PalaceClientWidget::
                 on_backgroundImageDidLoad);

        disconnect(my_palaceClientPtr,
                   &seville::palace::Client::
                   connectionStateDidChangeEvent,
                   this,
                   &seville::view::PalaceClientWidget::
                   on_connectionStateDidChange);

         disconnect(my_palaceClientPtr,
                 &seville::palace::Client::viewNeedsUpdatingEvent,
                 this,
                 &seville::view::PalaceClientWidget::on_viewNeedsUpdating);

         //auto mainWindowPtr =
         //static_cast<view::MainWindow*>(this->parent()->parent());
         //auto mainWindowPtr = do_mainWindowPtr(); //sevilleApp.mainWindowPtr();
         //disconnect(my_palaceClientPtr,
         //        &seville::palace::Client::connectionStateDidChangeEvent,
         //        mainWindowPtr,
         //        &seville::view::MainWindow::on_clientConnectionStateDidChange);

         disconnect(my_chatLineEditPtr,
                 &QLineEdit::returnPressed,
                 this,
                 &seville::view::PalaceClientWidget::
                 on_chatLineEditDidPressReturn);

         disconnect(my_palaceClientPtr,
                    &seville::palace::Client::serverNameWasSetEvent,
                    this,
                    &seville::view::PalaceClientWidget::on_serverNameWasSet);
      }

      void PalaceClientWidget::do_setupSizing(void)
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
         // my_scrollAreaPtr->resize(
         //          my_backgroundImage.width() + 4,
         //          my_backgroundImage.height() + 4);
         // my_scrollAreaPtr->setFixedWidth(
         //          );
                  //my_backgroundImageLabelPtr->width() + 4);
         //auto frameWidth = my_backgroundImageLabelPtr->frameWidth();
         //(void)frameWidth;
                  //my_scrollAreaPtr->horizontalScrollBar()->size().width());
         // my_scrollAreaPtr->setFixedHeight(

                  //my_backgroundImageLabelPtr->height() + 4);
         //my_scrollAreaPtr->setStyleSheet("border: 1px solid red");
         //my_scrollAreaPtr->updateGeometry();
         // my_scrollAreaPtr->adjustSize();
         my_roomWidgetPtr->setFixedWidth(my_backgroundImage.width() + 4);
         my_roomWidgetPtr->setFixedHeight(my_backgroundImage.height() + 4);
         my_roomWidgetPtr->adjustSize();

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
            my_palaceClientPtr->move(
                     static_cast<i16>(mouseEventPtr->x() - kSmileyWidth / 2),
                     static_cast<i16>(mouseEventPtr->y() - kSmileyHeight / 2));
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

         auto usersPtr = my_palaceClientPtr->roomPtr()->usersPtr();
         auto z = usersPtr->size();
         // for (auto& user: *my_palaceClientPtr->roomPtr()->usersPtr())
         // for (auto i = u32{0}; i < z; i++)
         for (auto i = u32{0}; i < z; i++)
         {
            // auto j = i - 1;
            auto j = i;
            if (usersPtr->at(j).propsPtr() != nullptr &&
                usersPtr->at(j).propsPtr()->at(0).imageLoadedFlag())
            {
               auto propInstance = usersPtr->at(j).propsPtr()->at(0);
               auto userImagePtr = propInstance.propImagePtr();
               auto propOffset = propInstance.offset();

               painter.drawImage(usersPtr->at(j).x() + propOffset.x(),
                                 usersPtr->at(j).y() + propOffset.y(),
                                 *userImagePtr);
            } else {
               auto face = usersPtr->at(j).face();
               auto color = usersPtr->at(j).color();
               auto rect = QRect(face * kSmileyWidth,
                                 color * kSmileyHeight,
                                 kSmileyWidth,
                                 kSmileyHeight);

               auto userImage = my_spritesheet.copy(rect);
               painter.drawImage(usersPtr->at(j).x(), //  - kSmileyWidth / 2,
                                 usersPtr->at(j).y(), // - kSmileyHeight / 2,
                                 userImage);
            }

            // QBrush blackBrush(Qt::black);
            // QPen pen(Qt::gray);
         }

         // for (auto& user: *my_palaceClientPtr->roomPtr()->usersPtr())
         // for (auto i = u32{0}; i < z; i++)
         for (auto i = u32{0}; i < z; i++)
         {
            // auto j = i - 1;
            auto j = i;
            auto font = QFont("Sans-Serif", 10, QFont::Bold);
            // font.setWeight(3);

            auto fontMetrics = QFontMetrics(font);

            auto width = fontMetrics.boundingRect(
                     usersPtr->at(j).username()).width() + kDropShadowWidth;

            // auto nameTextRect = QRect();
            auto nameTextPoint =
                  QPoint(usersPtr->at(j).x() + (kSmileyWidth / 2) - (width / 2),
                         usersPtr->at(j).y() + 56);

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
            painter.drawText(
                     nameTextPoint + QPoint(0, -1), usersPtr->at(j).username());
            painter.drawText(
                     nameTextPoint + QPoint(-1, 0), usersPtr->at(j).username());
            painter.drawText(
                     nameTextPoint + QPoint(1, 0), usersPtr->at(j).username());

            painter.setFont(font);
            painter.setPen(Qt::black);
            // painter.drawPath(painterPath);
            painter.drawText(
                     nameTextPoint + QPoint(0, 1), usersPtr->at(j).username());

            // draw name
            // painter.setPen(Qt::Pen
            painter.setFont(font);
            if (0 <= usersPtr->at(j).color() &&
                usersPtr->at(j).color() < kNumNameColors)
              painter.setPen(kNameColors[usersPtr->at(j).color()]);

            painter.drawText(nameTextPoint, usersPtr->at(j).username());
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

      void PalaceClientWidget::on_serverNameWasSet(const QString& value) {
         emit serverNameWasSetEvent(this, value);
      }

      void PalaceClientWidget::on_backgroundImageDidLoad(void)
      {
         auto backgroundImage =
               my_palaceClientPtr->roomPtr()->backgroundImage();

         //auto pixmap = QPixmap::fromImage(backgroundImage);
         do_setBackgroundImage(backgroundImage);

         adjustSize();

         //auto mainWindowPtr = do_mainWindowPtr();
         //if (mainWindowPtr != nullptr) {
         //   mainWindowPtr->adjustSize();
         //}
      }

      void PalaceClientWidget::on_connectionStateDidChange(
            palace::ConnectionState connectionState)
      {
         // TODO switch

         emit connectionStateDidChangeEvent(connectionState);
      }

      void PalaceClientWidget::on_chatLineEditDidPressReturn(void)
      {
         auto text = my_chatLineEditPtr->text();
         my_palaceClientPtr->chat(text);
         my_chatLineEditPtr->clear();
      }

      void PalaceClientWidget::on_viewNeedsUpdating(void)
      {
         update();
      }

      void PalaceClientWidget::on_roomListDidLoad(void)
      {
         my_roomUserListComboBoxPtr->clear();
         auto roomListPtr = my_palaceClientPtr->serverPtr()->roomListPtr();
         for (auto& room: *roomListPtr) {
            my_roomUserListComboBoxPtr->addItem(room.roomName(), room.id());
         }
      }

      void PalaceClientWidget::on_roomWasSelected(int index)
      {
         auto room_id_variant = my_roomUserListComboBoxPtr->itemData(index);
         auto room_id = room_id_variant.toInt();
         if (0 <= room_id && room_id <= 65535) {
            my_palaceClientPtr->gotoRoom(room_id);
         }
      }
   }
}
