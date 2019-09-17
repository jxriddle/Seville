#include "Palace_Room.h"

namespace Seville
{
   namespace Palace
   {
      void Room::doReset()
      {
          myId = 0;
      }

      QImage Room::backgroundImage()
      {
         return myBackgroundImage;
      }

      void Room::setBackgroundImage(QImage image)
      {
         myBackgroundImage = image;
      }

      Room::~Room()
      {

      }

      Room::Room()
      {
          doReset();
      }
   }
}
