#ifndef UI_PAL_CLIENT_WIDGET_H
#define UI_PAL_CLIENT_WIDGET_H
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "applog.h"
#include "view_main_window.h"
#include "pal_client.h"

namespace Seville
{
   namespace View
   {
      class PalClientWidget : public QWidget
      {
         Q_OBJECT

         private:
            void doSetupUi();
            void doSetupEvents();

            void doSetBackgroundImage(QString imagePath);
            //void doResizeBackgroundImage(QSize size);
            void doPromptNewConnection();

            PalClient* myPalClient;
            QLabel* myBackgroundImageLabel;
            QScrollArea* myScrollArea;
            QVBoxLayout* myWidgetLayout;

            QImage myBackgroundImage;
            double myScaleFactor;

         protected:
            //void resizeEvent(QResizeEvent *event) override;

         public:
            explicit PalClientWidget(QWidget *parent = nullptr);
            //virtual ~PalRoomWidget();
            PalClient* PalaceClient();
            void promptNewConnection()
               { doPromptNewConnection(); }

         signals:

         public slots:
            //void updateGeometry();
            //void onBackgroundImageLabelResized(QResizeEvent *event);
            void mousePressEvent(QMouseEvent *event);
      };
   }
}
#endif // UI_PAL_CLIENT_WIDGET_H
