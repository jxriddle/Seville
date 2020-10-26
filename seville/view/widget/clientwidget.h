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
            explicit ClientWidget(QWidget* widget_parent_ptr = nullptr);
            //virtual ~ClientWidget(void);

            inline palace::Client* palace_client_ptr(void) {
               return my_client_palace_ptr_;
            }

            inline auto prompt_new_connection(void) -> void {
               do_prompt_new_connection_(this);
            }

         protected:
            auto mousePressEvent(QMouseEvent* event_ptr) -> void;

            //void resizeEvent(QResizeEvent *event_ptr) override;

         signals:

         public slots:
            //void updateGeometry(void);

            //auto on_background_image_label_resized(QResizeEvent* event_ptr)
            //    -> void;

            void on_background_changed(void);

         private:
            palace::Client* my_client_palace_ptr_;
            QLabel* my_label_image_background_ptr_;
            QScrollArea* my_scrollarea_ptr_;
            QVBoxLayout* my_layout_widget_ptr_;
            QPixmap my_pixmap_background_image_;
            double my_scale_factor_;

            auto do_setup_view_(void) -> void;
            auto do_setup_events_(void) -> void;

            auto do_set_background_image_(QPixmap& pixmap) -> void;
            auto do_set_background_image_from_file_(QString image_path) -> void;
            //auto do_resize_background_image(QSize size) -> void;
            auto do_prompt_new_connection_(QWidget* widget_parent_ptr) -> void;
            auto do_fetch_background_image_(QString file_url) -> void;
         };
      }
   }
}
#endif // SEVILLE_PALACE_CLIENT_WIDGET_H_
