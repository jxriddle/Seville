#ifndef SEVILLE_PALACE_CLIENT_WIDGET_H_
#define SEVILLE_PALACE_CLIENT_WIDGET_H_

#include <QLabel>
#include <QScrollArea>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QComboBox>

#include "seville/palace/client.h"
//#include "seville/view/mainwindow.h"

namespace seville
{
    namespace view
    {
        class PalaceClientWidget : public QWidget
        {
        Q_OBJECT

        public:
            static const i32 kSmileyHeight = 45;
            static const i32 kSmileyWidth = 45;
            static const i32 kMaxPropWidth = 132;
            static const i32 kMaxPropHeight = 132;

            static const i32 kNumNameColors = 16;
            QColor kNameColors[kNumNameColors];

            static const i32 kDropShadowWidth = 0;

            explicit PalaceClientWidget(QWidget* parentWidgetPtr = nullptr);
            virtual ~PalaceClientWidget(void);

            inline palace::Client* palaceClientPtr(void) {
                return my_palaceClientPtr;
            }

            inline void promptOpenConnection(void) {
                do_promptOpenConnection(this);
            }

            //         inline auto backgroundImagePixmapPtr(void) -> QPixmap* {
            //            return &my_backgroundImage;
            //         }

            // inline auto drawRoom(void) -> void {
            //    do_drawRoom();
            // }

        protected:
            void paintEvent(QPaintEvent* eventPtr);
            void mousePressEvent(QMouseEvent* eventPtr);
            //void resizeEvent(QResizeEvent* resize_event_ptr) override;
            void currentChanged(int index);

        signals:
            //void widgetBackgroundDidChangeEvent(int width, int height);
            void widgetBackgroundDidChangeEvent(void);
            void serverNameWasSetEvent(QWidget* senderPtr, const QString& value);
            void connectionStateDidChangeEvent(
                    QWidget* senderPtr,
                    const palace::ConnectionState connectionState);

        public slots:
            //void updateGeometry(void);

            //auto on_background_image_label_resized(
            // QResizeEvent* resize_event_ptr) -> void;

            //void on_background_image_did_change(int width, int height);
            void on_backgroundImageDidLoad(void);
            void on_connectionStateDidChange(
                    palace::ConnectionState connectionState);
            void on_chatLineEditDidPressReturn(void);
            //void on_currentTabDidChange(int index);
            void on_viewNeedsUpdating(void);
            void on_serverNameWasSet(const QString& value);
            void on_roomListDidLoad(void);
            void on_roomWasSelected(int index);

        private:
            palace::Client* my_palaceClientPtr;
            // QLabel* my_backgroundImageLabelPtr;
            QLabel* my_roomWidgetPtr;
            // QScrollArea* my_scrollAreaPtr;
            QVBoxLayout* my_mainLayoutPtr;
            QLineEdit* my_lineEditAddressPtr;
            QLineEdit* my_chatLineEditPtr;
            QComboBox* my_roomUserListComboBoxPtr;

            double my_scaleFactor;
            // QPixmap my_backgroundImage;
            QImage my_backgroundImage;
            QImage my_sprite;
            QImage my_spritesheet;
            // QGraphicsView *my_view;

            view::MainWindow* do_mainWindowPtr(void);

            void do_init(void);
            void do_deinit(void);
            void do_setupView(void);
            void do_setupEvents(void);
            void do_teardownEvents(void);
            void do_setupSizing(void);

            // auto do_setBackgroundImage(const QPixmap& pixmap) -> void;
            void do_setBackgroundImage(const QImage& image);
            void do_setBackgroundImageFromFile(const QString& imagePath);
            // auto do_resizeBackgroundImage(QSize size) -> void;
            void do_promptOpenConnection(QWidget* parentPtr);
            void do_fetchBackgroundImage(const QString& fileUrl);
            // auto do_clearBackgroundImage(void) -> void;
            // auto do_drawRoom(void) -> void;
        };
    }
}
#endif // SEVILLE_PALACE_CLIENT_WIDGET_H_
