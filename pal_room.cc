#include "pal_room.h"

namespace Seville
{
   PalRoom::PalRoom()
   {
       doResetState();
   }

   PalRoom::~PalRoom()
   {

   }

   void PalRoom::resetState()
   {
       doResetState();
   }

   void PalRoom::doResetState()
   {
       myId = 0;
   }

   quint16 PalRoom::id()
   {
       return myId;
   }

   void PalRoom::setId(quint16 id)
   {
       myId = id;
   }

   void PalRoom::setBackgroundImage(QImage image)
   {
      myBackgroundImage = image;
   }

   QImage PalRoom::backgroundImage()
   {
      return myBackgroundImage;
   }
}
