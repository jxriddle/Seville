#include "pal_user.h"

namespace Seville
{
   PalUser::PalUser()
   {
       doResetState_();
   }

   PalUser::~PalUser()
   {

   }

   void PalUser::resetState()
   {
       doResetState_();
   }

   void PalUser::doResetState_()
   {
       color_ = 1;
       face_ = 1;
       x_ = 0;
       y_ = 0;
       userIdChanged_ = false;
       userIdCounter_ = 0xf5dc385e;
       userIdCrc_ = 0xc144c580;
       regCounter_ = 0xcf07309c;
       regCrc_ = 0x5905f923;
       username_ = "Seville User";
       wizpass_ = "";
   }

   int PalUser::color() const
   {
       return color_;
   }

   void PalUser::setColor(int value)
   {
       color_ = value;
   }

   int PalUser::face() const
   {
      return face_;
   }

   void PalUser::setFace(int value)
   {
       face_ = value;
   }

   int PalUser::x() const
   {
       return x_;
   }

   void PalUser::setX(int value)
   {
       x_ = value;
   }

   int PalUser::y() const
   {
       return y_;
   }

   void PalUser::setY(int value)
   {
       y_ = value;
   }

   QString PalUser::username() const
   {
       return username_;
   }

   void PalUser::setUsername(QString value)
   {
       username_ = value;
   }

   QString PalUser::wizpass() const
   {
       return wizpass_;
   }

   void PalUser::setWizpass(QString wizpass)
   {
       wizpass_ = wizpass;
   }

   quint32 PalUser::id() const
   {
       return id_;
   }

   void PalUser::setId(quint32 id)
   {
       id_ = id;
   }

   quint32 PalUser::idCrc() const
   {
       return userIdCrc_;
   }

   void PalUser::setIdCrc(quint32 userIdCrc)
   {
       userIdCrc_ = userIdCrc;
   }

   quint32 PalUser::idCounter() const
   {
       return userIdCounter_;
   }

   void PalUser::setIdCounter(quint32 userIdCounter)
   {
       userIdCounter_ = userIdCounter;
   }

   bool PalUser::idChanged() const
   {
       return userIdChanged_;
   }

   void PalUser::setIdChanged(bool userIdChanged)
   {
       userIdChanged_ = userIdChanged;
   }

   quint32 PalUser::regCrc() const
   {
       return regCrc_;
   }

   void PalUser::setRegCrc(quint32 regCrc)
   {
       regCrc_ = regCrc;
   }

   quint32 PalUser::regCounter() const
   {
       return regCounter_;
   }

   void PalUser::setRegCounter(quint32 regCounter)
   {
       regCounter_ = regCounter;
   }
}
