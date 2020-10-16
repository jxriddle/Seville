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
         ClientWidget::ClientWidget(QWidget *parent) : QWidget(parent)
         {
            my_client_palace_ptr_ = new seville::palace::Client(this);

            do_setup_view_();
            do_setup_events_();
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

         void ClientWidget::do_setup_view_(void)
         {
            my_layout_widget_ptr_ = new QVBoxLayout(this);
            my_scrollarea_ptr_ = new QScrollArea(this);
            my_label_image_background_ptr_ = new QLabel(my_scrollarea_ptr_);
            my_layout_widget_ptr_->setMargin(0);
            my_scrollarea_ptr_->setHorizontalScrollBarPolicy(
                     Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_scrollarea_ptr_->setVerticalScrollBarPolicy(
                     Qt::ScrollBarPolicy::ScrollBarAsNeeded);
            my_scrollarea_ptr_->setSizePolicy(
                     QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            //myWidgetLayout->addStretch(1);
            my_layout_widget_ptr_->setContentsMargins(0, 0, 0, 0);
            my_layout_widget_ptr_->setSpacing(0);

            auto layout_scroll_area_ptr = new QHBoxLayout(my_scrollarea_ptr_);
            layout_scroll_area_ptr->addWidget(my_label_image_background_ptr_);

            layout_scroll_area_ptr->setMargin(0);
            layout_scroll_area_ptr->setContentsMargins(0, 0, 0, 0);
            layout_scroll_area_ptr->setSpacing(0);
            my_scrollarea_ptr_->setLayout(layout_scroll_area_ptr);
            my_layout_widget_ptr_->addWidget(my_scrollarea_ptr_);

            //PalRoom& room = myPalClient->currentRoom();
            auto filename_image = QString(":/assets/images/test-pattern.png"); //.arg("test-pattern.png");
            qCDebug(SevilleApp()->log_category()) << filename_image;
            do_set_backgroundimage_from_file_(filename_image);

            setLayout(my_layout_widget_ptr_);
         }

         void ClientWidget::do_setup_events_()
         {
            //connect(
               //this, &Seville::View::ClientWidget::mousePressEvent,
               //this, &Seville::View::ClientWidget::onMousePressEvent);

            connect( \
                  my_client_palace_ptr_, \
                  &seville::palace::Client::background_image_did_load, \
                  this, \
                  &seville::view::widget::ClientWidget::on_background_changed);
         }

         void ClientWidget::do_set_background_image_(QPixmap& pixmap)
         {
            my_label_image_background_ptr_->setBackgroundRole(QPalette::Base);
            my_label_image_background_ptr_->setSizePolicy( \
                     QSizePolicy::MinimumExpanding, \
                     QSizePolicy::MinimumExpanding);
            my_label_image_background_ptr_->setScaledContents(true);
            //my_label_background_image_ptr->resize(backgroundImage_.size());
            my_label_image_background_ptr_->setPixmap(pixmap);
            my_label_image_background_ptr_->setMask(pixmap.mask());
            //myBackgroundImageLabel->setGeometry(0, 0, 500, 500);

            my_scrollarea_ptr_->setBackgroundRole(QPalette::Dark);
            my_scrollarea_ptr_->setWidget(my_label_image_background_ptr_);
            //my_scrollarea_ptr_->setGeometry(pixmap.rect());
            //my_scrollarea_ptr_->setGeometry( \
            // my_label_background_image_ptr_->geometry());
            my_scrollarea_ptr_->setVisible(true);
         }

         void ClientWidget::do_set_background_image_from_file_( \
               QString imagePath)
         {
            QPixmap pixmap = QPixmap(imagePath);
            do_set_background_image_(pixmap);
         }

         void ClientWidget::do_prompt_new_connection_( \
               QWidget* widget_parent_ptr)
         {
            seville::view::dialog::ConnectDialog dialog(parent);
            if (dialog.exec() == QDialog::Accepted) {
               my_client_palace_ptr_->connectToHost(
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
            if (my_client_palace_ptr_->connection_state() ==
                palace::Client::ConnectionState::kDisconnected) {
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
}
