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

      QByteArray Room::backgroundImageBytes()
      {
         return myBackgroundImageBytes;
      }

      void Room::setBackgroundImageBytes(QByteArray imageBytes)
      {
         //myBackgroundImage
         myBackgroundImageBytes = imageBytes;
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
