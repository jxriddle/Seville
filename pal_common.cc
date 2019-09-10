#include "pal_common.h"

#include <QtCore>

namespace Seville
{
   PalCommon::PalCommon()
   {

   }

   bool PalCommon::isBigEndian(void)
   {
       bi32 bi;
       bi.i32 = { 0x01020304 };

       return bi.ch[0] == 0x01;
   }

   quint16 PalCommon::doSwapWBo(const quint16 unswapped)
   {
       return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
   }

   quint32 PalCommon::doSwapDwBo(const quint32 unswapped)
   {
       return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
               ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
               ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
               ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
   }

   qint16 PalCommon::swapI16Bo(const qint16 unswapped)
   {
       return static_cast<qint16>(doSwapWBo(static_cast<quint16>(unswapped)));
   }

   quint16 PalCommon::swapU16Bo(const quint16 unswapped)
   {
       return doSwapWBo(unswapped);
   }

   qint32 PalCommon::swapI32Bo(const qint32 unswapped)
   {
       return static_cast<qint32>(doSwapDwBo(static_cast<quint32>(unswapped)));
   }

   quint32 PalCommon::swapU32Bo(const quint32 unswapped)
   {
       return doSwapDwBo(unswapped);
   }
}
