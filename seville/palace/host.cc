#include <QtCore>

#include "seville/base/types.h"
#include "seville/palace/host.h"

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

      auto Host::SwapI16(const qint16 unswapped) -> qint16
      {
          return static_cast<qint16>(do_swap_word(static_cast<quint16>(unswapped)));
      }

      auto Host::SwapU16(const quint16 unswapped) -> quint16
      {
          return do_swap_word(unswapped);
      }

      auto Host::SwapI32(const qint32 unswapped) -> qint32
      {
          return static_cast<qint32>(do_swap_dword(static_cast<quint32>(unswapped)));
      }

      auto Host::SwapU32(const quint32 unswapped) -> quint32
      {
          return do_swap_dword(unswapped);
      }

      auto Host::do_swap_word(const quint16 unswapped) -> quint16
      {
          return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
      }

      auto Host::do_swap_dword(const quint32 unswapped) -> quint32
      {
          return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
                  ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
                  ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
                  ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
      }
   }
}
