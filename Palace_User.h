#ifndef PALACE_USER_H
#define PALACE_USER_H

#include <QObject>

namespace Seville
{
   namespace Palace
   {
      class User : QObject
      {
         Q_OBJECT

         private:
            QString myUsername;
            QString myWizpass;
            quint32 myId;
            int myFace;
            int myX;
            int myY;
            int myColor;
            bool myIdChanged;
            quint32 myIdCounter;
            quint32 myIdCrc;
            quint32 myRegCounter;
            quint32 myRegCrc;

            void doReset();

         public:
            int color() const { return myColor; }
            void setColor(int value) { myColor = value; }

            int face() const { return myFace; }
            void setFace(int value) { myFace = value; }

            int x() const { return myX; }
            void setX(int value) { myX = value; }

            int y() const { return myY; }
            void setY(int value) { myY = value; }

            QString username() const { return myUsername; }
            void setUsername(const QString& value) { myUsername = value; }

            QString wizpass() const { return myWizpass; }
            void setWizpass(const QString& value) { myWizpass = value; }

            quint32 id() const { return myId; }
            void setId(quint32 value) { myId = value; }

            quint32 idCrc() const { return myIdCrc; }
            void setIdCrc(quint32 value) { myIdCrc = value; }

            quint32 idCounter() const { return myIdCounter; }
            void setIdCounter(quint32 value) { myIdCounter = value; }

            bool idChanged() const { return myIdChanged; }
            void setIdChanged(bool value) { myIdChanged = value; }

            quint32 regCrc() const { return myRegCrc; }
            void setRegCrc(quint32 value) { myRegCrc = value; }

            quint32 regCounter() const { return myRegCounter; }
            void setRegCounter(quint32 value) { myRegCounter = value; }

            void reset() { doReset(); }

            virtual ~User();
            User();
      };
   }
}

#endif // PALACE_USER_H
