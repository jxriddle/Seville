#include "Palace_Host.h"

#include <QtCore>

#include "Common.h"

namespace Seville
{
   namespace Palace
   {
      Host::Host()
      {

      }


      quint16 Host::doSwapWord(const quint16 unswapped)
      {
          return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
      }

      quint32 Host::doSwapDword(const quint32 unswapped)
      {
          return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
                  ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
                  ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
                  ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
      }

      qint16 Host::swapInt16(const qint16 unswapped)
      {
          return static_cast<qint16>(doSwapWord(static_cast<quint16>(unswapped)));
      }

      quint16 Host::swapUInt16(const quint16 unswapped)
      {
          return doSwapWord(unswapped);
      }

      qint32 Host::swapInt32(const qint32 unswapped)
      {
          return static_cast<qint32>(doSwapDword(static_cast<quint32>(unswapped)));
      }

      quint32 Host::swapUInt32(const quint32 unswapped)
      {
          return doSwapDword(unswapped);
      }
   }
}
