#ifndef UI_CLIENT_WIDGET_H
#define UI_CLIENT_WIDGET_H

#include <QDir>
#include <QImage>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "Log.h"
#include "View_MainWindow.h"
#include "Palace_Client.h"

namespace Seville
{
   namespace View
   {
      class ClientWidget : public QWidget
      {
         Q_OBJECT

         private:
            Palace::Client* myPalaceClient;

            QLabel* myBackgroundImageLabel;
            QScrollArea* myScrollArea;
            QVBoxLayout* myWidgetLayout;

            QImage myBackgroundImage;
            double myScaleFactor;

            void doSetupView();
            void doSetupEvents();

            void doSetBackgroundImage(QString imagePath);
            //void doResizeBackgroundImage(QSize size);
            void doPromptNewConnection(QWidget* parent);
            void doFetchBackgroundImage(QString fileUrl);

         protected:
            void mousePressEvent(QMouseEvent* event);
            //void resizeEvent(QResizeEvent *event) override;

         signals:

         public slots:
            //void updateGeometry();
            //void onBackgroundImageLabelResized(QResizeEvent *event);

         public:
            Palace::Client* palaceClient() { return myPalaceClient; }

            void promptNewConnection()
               { doPromptNewConnection(this); }

            //virtual ~ClientWidget();
            explicit ClientWidget(QWidget* parent = nullptr);
      };
   }
}
#endif // UI_CLIENT_WIDGET_H
