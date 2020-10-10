#ifndef SEVILLE_PALACE_CLIENT_WIDGET_H_
#define SEVILLE_PALACE_CLIENT_WIDGET_H_

#include <QDir>
#include <QImage>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "seville/view/widget/logwidget.h"
#include "seville/view/window/mainwindow.h"
#include "seville/palace/base/client.h"

namespace seville
{
   namespace view
   {
      class ClientWidget : public QWidget
      {
         Q_OBJECT

      public:
         palace::Client* palaceClient(void) {
            return my_client_palace_pointer_;
         }

         auto promptNewConnection(void) -> void {
            do_promptNewConnection(this);
         }

         //virtual ~ClientWidget();
         explicit ClientWidget(QWidget* widget_parent_pointer = nullptr);


      protected:
         auto mousePressEvent(QMouseEvent* event_mouse_pointer) -> void;
         //void resizeEvent(QResizeEvent *event) override;

      signals:

      public slots:
         //void updateGeometry();
         //void onBackgroundImageLabelResized(QResizeEvent *event);
         auto on_backgroundChanged(void) -> void;

      private:
         palace::Client* my_client_palace_pointer_;
         QLabel* my_label_background_image_pointer_;
         QScrollArea* my_scrollarea_pointer_;
         QVBoxLayout* my_layout_widget_pointer_;
         QPixmap my_pixmap_background_image_;
         double my_scale_factor_;

         auto do_setupView() -> void;
         auto do_setupEvents() -> void;

         auto do_setBackgroundImage(QPixmap& pixmap) -> void;
         auto do_setBackgroundImageFromFile(QString imagePath) -> void;
         //void doResizeBackgroundImage(QSize size);
         auto do_promptNewConnection(QWidget* parent) -> void;
         auto do_fetchBackgroundImage(QString fileUrl) -> void;
      };
   }
}
#endif // SEVILLE_PALACE_CLIENT_WIDGET_H_
