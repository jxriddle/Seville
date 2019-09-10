#ifndef PALUSER_H
#define PALUSER_H

#include <QObject>

namespace Seville
{
   class PalUser : QObject
   {
      Q_OBJECT

      public:
         explicit PalUser();
         virtual ~PalUser();

         void resetState();
         int color() const;
         void setColor(int value);
         int face() const;
         void setFace(int value);
         int x() const;
         void setX(int value);
         int y() const;
         void setY(int value);
         QString username() const;
         void setUsername(QString value);
         QString wizpass() const;
         void setWizpass(QString wizpass);
         quint32 id() const;
         void setId(quint32 id);
         quint32 idCrc() const;
         void setIdCrc(quint32 idCrc);
         quint32 idCounter() const;
         void setIdCounter(quint32 idCounter);
         bool idChanged() const;
         void setIdChanged(bool userIdChanged);
         quint32 regCrc() const;
         void setRegCrc(quint32 regCrc);
         quint32 regCounter() const;
         void setRegCounter(quint32 regCounter);

      private:
         void doResetState_();

         QString username_;
         QString wizpass_;
         quint32 id_;
         int face_;
         int x_;
         int y_;
         int color_;
         bool userIdChanged_;
         quint32 userIdCounter_;
         quint32 userIdCrc_;
         quint32 regCounter_;
         quint32 regCrc_;
   };
}

#endif // PALUSER_H
