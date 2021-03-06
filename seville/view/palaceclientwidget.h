#ifndef SEVILLE_PALACE_CLIENT_WIDGET_H_
#define SEVILLE_PALACE_CLIENT_WIDGET_H_

#include <QLabel>
#include <QScrollArea>
#include <QLineEdit>
#include <QVBoxLayout>

#include "seville/palace/client.h"
#include "seville/view/mainwindow.h"

namespace seville
{
   namespace view
   {
      class PalaceClientWidget : public QWidget
      {
      Q_OBJECT

      public:
         explicit PalaceClientWidget(QWidget* parentWidgetPtr = nullptr);
         //virtual ~ClientWidget(void);

         inline palace::Client* palaceClientPtr(void) {
            return my_palaceClientPtr;
         }

         inline auto promptOpenConnection(void) -> void {
            do_promptOpenConnection(this);
         }

         inline auto backgroundImagePixmapPtr(void) -> QPixmap* {
            return &my_backgroundImagePixmap;
         }

      protected:
         auto mousePressEvent(QMouseEvent* mouseEventPtr) -> void;
         //void resizeEvent(QResizeEvent* resize_event_ptr) override;
         auto currentChanged(int index) -> void;

      signals:
         //void widgetBackgroundDidChangeEvent(int width, int height);
         void widgetBackgroundDidChangeEvent(void);

      public slots:
         //void updateGeometry(void);

         //auto on_background_image_label_resized(
         // QResizeEvent* resize_event_ptr) -> void;

         //void on_background_image_did_change(int width, int height);
         void on_backgroundImageDidLoad(void);
         void on_connectionStateDidChange(
               palace::ConnectionState connectionState);
         //void on_chatLineEditDidPressReturn(void);
         //void on_currentTabDidChange(int index);

      private:
         palace::Client* my_palaceClientPtr;
         QLabel* my_backgroundImageLabelPtr;
         QScrollArea* my_scrollAreaPtr;
         QVBoxLayout* my_mainLayoutPtr;
         QPixmap my_backgroundImagePixmap;
         double my_scaleFactor;
         QLineEdit* my_lineEditAddressPtr;
         QLineEdit* my_chatLineEditPtr;

         view::MainWindow* do_mainWindowPtr(void);

         auto do_setupView(void) -> void;
         auto do_setupEvents(void) -> void;
         auto do_setupSizing(void) -> void;

         auto do_setBackgroundImage(const QPixmap& pixmap) -> void;
         auto do_setBackgroundImageFromFile(const QString& imagePath) -> void;
         //auto do_resizeBackgroundImage(QSize size) -> void;
         auto do_promptOpenConnection(QWidget* parentPtr) -> void;
         auto do_fetchBackgroundImage(const QString& fileUrl) -> void;
         //auto do_clearBackgroundImage(void) -> void;
      };
   }
}
#endif // SEVILLE_PALACE_CLIENT_WIDGET_H_
