#ifndef PALROOM_H
#define PALROOM_H

#include <QObject>
#include <QImage>

namespace Seville
{
   namespace Palace
   {
      class Room : QObject
      {
         Q_OBJECT

         private:
             quint16 myId;
             QImage myBackgroundImage;

             void doReset();

         public:
             quint16 id() { return myId; }
             void setId(quint16 value) { myId = value; }

             void loadBackgroundImageFromUrl(QString url);

             QImage backgroundImage();
             void setBackgroundImage(QImage image);

             void reset() { doReset(); }

             virtual ~Room();

             Room();
      };
   }
}

#endif // PROOM_H
