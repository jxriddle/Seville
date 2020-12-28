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

#include "seville/base/app.h"

#include "seville/palace/base/logger.h"
#include "seville/palace/base/client.h"

#include "seville/view/dialog/connectdialog.h"
#include "seville/view/widget/clientwidget.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         ClientWidget::ClientWidget(QWidget* parent_widget_ptr)
            : QWidget(parent_widget_ptr)
         {
            my_palace_client_ptr_ = new seville::palace::Client(this);

            do_setup_view_();
            do_setup_events_();
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

         void ClientWidget::do_setup_view_(void)
         {
            my_primary_layout_ptr_ = new QVBoxLayout(this);
            my_primary_layout_ptr_->setMargin(0);
            my_primary_layout_ptr_->setContentsMargins(0, 0, 0, 0);
            my_primary_layout_ptr_->setSpacing(0);
            my_primary_layout_ptr_->addStretch(1);

            my_scroll_area_ptr_ = new QScrollArea(this);
            my_background_image_label_ptr_ = new QLabel(this);
            my_scroll_area_ptr_->setHorizontalScrollBarPolicy(
                     Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_scroll_area_ptr_->setVerticalScrollBarPolicy(
                     Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_scroll_area_ptr_->setSizePolicy(
                     QSizePolicy::Maximum, QSizePolicy::Maximum);
            my_scroll_area_ptr_->setWidgetResizable(true);
            my_scroll_area_ptr_->setWidget(my_background_image_label_ptr_);
            //my_scrollarea_ptr_->setStyleSheet("border 1px solid red");

//            auto layout_scroll_area_ptr = new QHBoxLayout(my_scrollarea_ptr_);
//            layout_scroll_area_ptr->addWidget(my_label_image_background_ptr_);
//            layout_scroll_area_ptr->setStretchFactor(my_scrollarea_ptr_, 1);

//            layout_scroll_area_ptr->setMargin(0);
//            layout_scroll_area_ptr->setContentsMargins(0, 0, 0, 0);
//            layout_scroll_area_ptr->setSpacing(0);
//            my_scrollarea_ptr_->setLayout(layout_scroll_area_ptr);

            my_primary_layout_ptr_->addWidget(my_scroll_area_ptr_);

            //PalRoom& room = myPalClient->currentRoom();
            auto image_filename =
                  QString(":/seville/assets/images/test-pattern.png");
                  //.arg("test-pattern.png");
            //qCDebug(SevilleApp()->log_category()) << filename_image;
            //qCDebug(log_seville) << image_filename;

            do_set_background_image_from_file_(image_filename);

            setLayout(my_primary_layout_ptr_);

            setStyleSheet("border: 1px solid red");
         }

         void ClientWidget::do_setup_events_(void)
         {
            //connect(
               //this, &Seville::View::ClientWidget::mousePressEvent,
               //this, &Seville::View::ClientWidget::onMousePressEvent);

            connect(
                  my_palace_client_ptr_,
                  &seville::palace::Client::background_image_did_load,
                  this,
                  &seville::view::widget::ClientWidget::
                     on_background_image_did_load);
         }

         void ClientWidget::do_set_background_image_(const QPixmap& pixmap)
         {
            my_background_image_label_ptr_->setBackgroundRole(QPalette::Base);
            my_background_image_label_ptr_->setSizePolicy(
                     QSizePolicy::MinimumExpanding,
                     QSizePolicy::MinimumExpanding);
            my_background_image_label_ptr_->setScaledContents(true);
            //my_label_background_image_ptr->resize(backgroundImage_.size());
            my_background_image_label_ptr_->setPixmap(pixmap);
            my_background_image_label_ptr_->setMask(pixmap.mask());
            //myBackgroundImageLabel->setGeometry(0, 0, 500, 500);

            my_scroll_area_ptr_->setBackgroundRole(QPalette::Dark);
            my_scroll_area_ptr_->setWidget(my_background_image_label_ptr_);
            //my_scrollarea_ptr_->setGeometry(pixmap.rect());
            //my_scrollarea_ptr_->setGeometry(
            // my_label_background_image_ptr_->geometry());
            my_scroll_area_ptr_->setVisible(true);

            //my_scrollarea_ptr_->setFixedSize(
            // my_label_image_background_ptr_->size());
            if (0 < my_background_image_label_ptr_->size().width() &&
                0 < my_background_image_label_ptr_->size().height()) {
               my_scroll_area_ptr_->resize(
                        my_background_image_label_ptr_->size());
               //this->setFixedSize(my_label_image_background_ptr_->size());
               this->resize(my_background_image_label_ptr_->size());
            }

            auto width = pixmap.width();
            auto height = pixmap.height();
            my_background_image_label_ptr_->setGeometry(0, 0, width, height);

            emit did_resize(width, height);
         }

         void ClientWidget::do_set_background_image_from_file_(
               QString image_path)
         {
            QPixmap pixmap = QPixmap(image_path);
            do_set_background_image_(pixmap);
         }

         void ClientWidget::do_prompt_new_connection_(
               QWidget* parent_widget_ptr)
         {
            seville::view::dialog::ConnectDialog dialog(parent_widget_ptr);
            if (dialog.exec() == QDialog::Accepted) {
               my_palace_client_ptr_->connect_to_host(
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

         void ClientWidget::mousePressEvent(QMouseEvent* mouse_event_ptr)
         {
            (void)mouse_event_ptr;
            if (my_palace_client_ptr_->connection_state() ==
                palace::ClientState::kDisconnectedClientState) {
               do_prompt_new_connection_(this);
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

         auto ClientWidget::on_background_image_did_load(void) -> void
         {
            auto background_image =
                  my_palace_client_ptr_->current_room_ptr()->background_image();
            auto pixmap = QPixmap::fromImage(background_image);
            do_set_background_image_(pixmap);
         }
      }
   }
}
