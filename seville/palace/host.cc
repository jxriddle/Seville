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

        qint16 Host::SwapI16(const qint16 unswapped)
        {
            return static_cast<qint16>(do_swap_word(static_cast<quint16>(unswapped)));
        }

        quint16 Host::SwapU16(const quint16 unswapped)
        {
            return do_swap_word(unswapped);
        }

        qint32 Host::SwapI32(const qint32 unswapped)
        {
            return static_cast<qint32>(do_swap_dword(static_cast<quint32>(unswapped)));
        }

        quint32 Host::SwapU32(const quint32 unswapped)
        {
            return do_swap_dword(unswapped);
        }

        quint16 Host::do_swap_word(const quint16 unswapped)
        {
            return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
        }

        quint32 Host::do_swap_dword(const quint32 unswapped)
        {
            return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
                    ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
                    ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
                    ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
        }
    }
}
