#ifndef PALROOM_H
#define PALROOM_H

#include <QObject>
#include <QImage>

namespace Seville
{
   class PalRoom : QObject
   {
       Q_OBJECT

      public:
          PalRoom();
          virtual ~PalRoom();
          void resetState();

          void setId(quint16 id);
          quint16 id();

          void loadBackgroundImageFromUrl(QString url);
          void setBackgroundImage(QImage image);
          QImage backgroundImage();

      private:
          void doResetState();
          quint16 myId;
          QImage myBackgroundImage;
   };
}

#endif // PROOM_H
