#include <QtCore>

#include "seville/base/types.h"
#include "seville/palace/base/host.h"

namespace seville
{
   namespace palace
   {
      Host::Host(void)
      {
      }

      Host::~Host(void)
      {
      }

      auto Host::do_swapWord(const quint16 unswapped) -> quint16
      {
          return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
      }

      auto Host::do_swapDword(const quint32 unswapped) -> quint32
      {
          return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
                  ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
                  ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
                  ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
      }

      auto Host::swapI16(const qint16 unswapped) -> qint16
      {
          return static_cast<qint16>(do_swapWord(static_cast<quint16>(unswapped)));
      }

      auto Host::swapU16(const quint16 unswapped) -> quint16
      {
          return do_swapWord(unswapped);
      }

      auto Host::swapI32(const qint32 unswapped) -> qint32
      {
          return static_cast<qint32>(do_swapDword(static_cast<quint32>(unswapped)));
      }

      auto Host::swapU32(const quint32 unswapped) -> quint32
      {
          return do_swapDword(unswapped);
      }
   }
}
