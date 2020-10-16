#ifndef SEVILLE_PALACE_CLIENT_WIDGET_H_
#define SEVILLE_PALACE_CLIENT_WIDGET_H_

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "seville/palace/base/client.h"

namespace seville
{
   namespace view
   {
      namespace widget
      {
         class ClientWidget : public QWidget
         {
            Q_OBJECT

         public:
            palace::Client* palaceClient(void) {
               return my_client_palace_ptr_;
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
            auto on_background_changed(void) -> void;

         private:
            palace::Client* my_client_palace_ptr_;
            QLabel* my_label_image_background_ptr_;
            QScrollArea* my_scrollarea_ptr_;
            QVBoxLayout* my_layout_widget_ptr_;
            QPixmap my_pixmap_background_image_;
            double my_scale_factor_;

            auto do_setup_view_() -> void;
            auto do_setup_events_() -> void;

            auto do_setBackgroundImage(QPixmap& pixmap) -> void;
            auto do_set_backgroundimage_from_file_(QString imagePath) -> void;
            //void doResizeBackgroundImage(QSize size);
            auto do_promptNewConnection(QWidget* parent) -> void;
            auto do_fetchBackgroundImage(QString fileUrl) -> void;
         };
      }
   }
}
#endif // SEVILLE_PALACE_CLIENT_WIDGET_H_
