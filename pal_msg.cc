#include <QByteArray>
#include <QDataStream>

#include "pal_common.h"
#include "pal_msg.h"

namespace Seville
{
   PalMsg::PalMsg() : QByteArray()
   {
       doResetState();
   }

   PalMsg::PalMsg(
         PalCommon::ByteOrder clientByteOrder,
         PalCommon::ByteOrder serverByteOrder)
   {
       doResetState();
       myClientByteOrder = clientByteOrder;
       myServerByteOrder = serverByteOrder;
   }

   PalMsg::PalMsg(const char *data) : QByteArray()
   {
       doResetState();
       //int *p = static_cast<int *>(data);
       ////id_ = static_cast<int *>(p)[kPalMsgIdOffset];
       //id_ = p[kPalMsgIdOffset];
       //len_ = p[kPalMsgLenOffset];
       //ref_ = p[kPalMsgRefOffset];
       append(data);
   }

   PalMsg::PalMsg(QByteArray &ba) : QByteArray()
   {
       doResetState();
       append(ba);
   }

   PalMsg::PalMsg(PalMsg &palMsg) : QByteArray()
   {
       doResetState();
       //id_ = palMsg.id_;
       //len_ = palMsg.len_;
       //ref_ = palMsg.ref_;
       append(palMsg);
   }

   PalMsg::~PalMsg()
   {

   }

   void PalMsg::resetState()
   {
      doResetState();
   }

   void PalMsg::doResetState()
   {
      truncate(0);
      reserve(PalMsg::kHeaderSize);
   }

   quint16 PalMsg::doSboW(quint16 unswapped)
   {
      return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
   }

   quint32 PalMsg::doSboDw(quint32 unswapped)
   {
      return (((unswapped >> 24) & 0x000000ff) |  /* byte 3 to byte 0 */
              ((unswapped <<  8) & 0x00ff0000) |  /* byte 1 to byte 2 */
              ((unswapped >>  8) & 0x0000ff00) |  /* byte 2 to byte 1 */
              ((unswapped << 24) & 0xff000000));  /* byte 0 to byte 3 */
   }

   quint16 PalMsg::doCsboW(quint16 unswapped) const
   {
      int notUnknownClientByteOrder = myClientByteOrder != PalCommon::ByteOrder::Unknown;
      int notUnknownServerByteOrder = myServerByteOrder != PalCommon::ByteOrder::Unknown;
      int notSameByteOrder = myClientByteOrder != myServerByteOrder;
      int cond = notUnknownClientByteOrder && notUnknownServerByteOrder && notSameByteOrder;
      quint16 swapped = doSboW(unswapped);

      return static_cast<quint16>(cond) * swapped | static_cast<quint16>(!cond) * unswapped;
   }

   qint16 PalMsg::doCsboI16(qint16 unswapped) const
   {
      return static_cast<qint16>(doCsboW(static_cast<quint16>(unswapped)));
   }

   quint16 PalMsg::doCsboU16(quint16 unswapped) const
   {
      return doCsboW(unswapped);
   }

   quint32 PalMsg::doCsboDw(quint32 unswapped) const
   {
      int notUnknownClientByteOrder = myClientByteOrder != PalCommon::ByteOrder::Unknown;
      int notUnknownServerByteOrder = myServerByteOrder != PalCommon::ByteOrder::Unknown;
      int notSameByteOrder = myClientByteOrder != myServerByteOrder;
      int cond = notUnknownClientByteOrder && notUnknownServerByteOrder && notSameByteOrder;
      quint32 swapped = doSboDw(unswapped);

      return static_cast<quint32>(cond) * swapped | static_cast<quint32>(!cond) * unswapped;
   }

   qint32 PalMsg::doCsboI32(qint32 unswapped) const
   {
      return static_cast<qint32>(doCsboDw(static_cast<quint32>(unswapped)));
   }

   quint32 PalMsg::doCsboU32(quint32 unswapped) const
   {
      return doCsboDw(unswapped);
   }

   quint16 PalMsg::doCsbolecW(quint16 unswapped) const
   {
      int cond = myClientByteOrder == PalCommon::ByteOrder::LittleEndian;
      quint16 swapped = doSboW(unswapped);

      return static_cast<quint16>(cond) * swapped | static_cast<quint16>(!cond) * unswapped;
   }

   qint16 PalMsg::doCsbolecI16(qint16 unswapped) const
   {
      return static_cast<qint16>(doCsbolecW(static_cast<quint16>(unswapped)));
   }

   quint16 PalMsg::doCsbolecU16(quint16 unswapped) const
   {
      return doCsbolecW(unswapped);
   }

   quint32 PalMsg::doCsbolecDw(quint32 unswapped) const
   {
      int cond = myClientByteOrder == PalCommon::LittleEndian;
      quint32 swapped = doSboDw(unswapped);

      return static_cast<quint32>(cond) * swapped | static_cast<quint32>(!cond) * unswapped;
   }

   qint32 PalMsg::doCsbolecI32(qint32 unswapped) const
   {
      return static_cast<qint32>(doCsbolecDw(static_cast<quint32>(unswapped)));
   }

   quint32 PalMsg::doCsbolecU32(quint32 unswapped) const
   {
      return doCsbolecDw(unswapped);
   }

   PalCommon::ByteOrder PalMsg::clientByteOrder() const
   {
      return myClientByteOrder;
   }

   void PalMsg::setClientByteOrder(PalCommon::ByteOrder byteOrder)
   {
      myClientByteOrder = byteOrder;
   }

   PalCommon::ByteOrder PalMsg::serverByteOrder() const
   {
      return myServerByteOrder;
   }

   void PalMsg::setServerByteOrder(PalCommon::ByteOrder byteOrder)
   {
      myServerByteOrder = byteOrder;
   }

   quint32 PalMsg::msgId() const
   {
      return doGetU32At(kPalMsgIdOffset);
   }

   void PalMsg::setMsgId(quint32 value)
   {
      doInsertDw(kPalMsgIdOffset, value);
   }

   qint32 PalMsg::msgLen() const
   {
      return doGetI32At(kPalMsgLenOffset);
   }

   void PalMsg::setMsgLen(qint32 value)
   {
      doInsertDw(kPalMsgLenOffset, static_cast<quint32>(value));
   }

   quint32 PalMsg::msgRef() const
   {
      return doGetU32At(kPalMsgRefOffset);
   }

   void PalMsg::setMsgRef(quint32 value)
   {
      doInsertDw(kPalMsgRefOffset, value);
   }

   qint32 PalMsg::i32At(int offset) const { return doGetI32At(offset); }
   quint32 PalMsg::u32At(int offset) const { return doGetU32At(offset); }
   qint16 PalMsg::i16At(int offset) const { return doGetI16At(offset); }
   quint16 PalMsg::u16At(int offset) const { return doGetU16At(offset); }
   qint8 PalMsg::i8At(int offset) const { return doGetI8At(offset); }
   quint8 PalMsg::u8At(int offset) const { return doGetU8At(offset); }

   qint32 PalMsg::doGetI32At(int offset) const
   {
      qint32 value;
      //quint32 value = at(offset) << 24;
      //value |= at(offset+1) << 16;
      //value |= at(offset+2) << 8;
      //value |= at(offset+3);
      bi32 bi;
      int z = static_cast<int>(sizeof(qint32));
      for (int i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<qint32>(bi.i32);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */

      return doCsboI32(value);
   }

   quint32 PalMsg::doGetU32At(int offset) const
   {
      quint32 value;
      //quint32 value = at(offset) << 24;
      //value |= at(offset+1) << 16;
      //value |= at(offset+2) << 8;
      //value |= at(offset+3);
      bi32 bi;
      int z = static_cast<int>(sizeof(quint32));
      for (int i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = bi.i32;

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */

      return doCsboU32(value);
   }

   qint16 PalMsg::doGetI16At(int offset) const
   {
      qint16 value;
      //qint16 value = at(offset) << 8;
      //value |= at(offset+1);

      bi16 bi;
      int z = static_cast<int>(sizeof(qint16));
      for (int i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<qint16>(bi.i16);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return doCsboI16(value);
   }

   quint16 PalMsg::doGetU16At(int offset) const
   {
      quint16 value;
      //quint16 value = at(offset) << 8;
      //value |= at(offset+1);

      bi16 bi;
      int z = static_cast<int>(sizeof(quint16));
      for (int i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = bi.i16;

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return doCsboU16(value);
   }

   qint8 PalMsg::doGetI8At(int offset) const
   {
      qint8 value = at(offset);
      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return value;
   }

   quint8 PalMsg::doGetU8At(int offset) const
   {
      quint8 value = static_cast<quint8>(at(offset));
      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return value;
   }

   void PalMsg::doInsertW(int offset, quint16 value)
   {
      //insert(offset, static_cast<char>(value & 0xff));
      //insert(offset+1, static_cast<char>((value >> 8) & 0xff));
      bi16 bi;
      bi.i16 = doCsboW(value);
      int z = static_cast<int>(sizeof(quint16));
      for (int i = 0; i < z; i++) {
         insert(offset+i, static_cast<char>(bi.ch[i]));
      }
   }

   void PalMsg::doInsertDw(int offset, quint32 value)
   {
      //insert(offset, static_cast<char>(value & 0xff));
      //insert(offset+1, static_cast<char>((value >> 8) & 0xff));
      //insert(offset+2, static_cast<char>((value >> 16) & 0xff));
      //insert(offset+3, static_cast<char>((value >> 24) & 0xff));
      bi32 bi;
      bi.i32 = doCsboDw(value);
      int z = static_cast<int>(sizeof(quint32));
      for (int i = 0; i < z; i++) {
         insert(offset+i, static_cast<char>(bi.ch[i]));
      }
   }

   void PalMsg::doAppendW(quint16 value)
   {
      //append(static_cast<char>(value & 0xff));
      //append(static_cast<char>((value >> 8) & 0xff));
      bi16 bi;
      bi.i16 = doCsboW(value);
      int z = static_cast<int>(sizeof(quint16));
      for (int i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void PalMsg::doAppendDw(quint32 value)
   {
      //append(static_cast<char>((value) & 0xff));
      //append(static_cast<char>((value >> 8) & 0xff));
      //append(static_cast<char>((value >> 16) & 0xff));
      //append(static_cast<char>((value >> 24) & 0xff));
      bi32 bi;
      bi.i32 = doCsboDw(value);
      int z = static_cast<int>(sizeof(quint32));
      for (int i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void PalMsg::appendW(quint16 value) { doAppendW(value); }
   void PalMsg::appendDw(quint32 value) { doAppendDw(value); }
   void PalMsg::appendI8(qint8 value) { append(value); }
   void PalMsg::appendU8(quint8 value) { append(static_cast<char>(value)); }
   void PalMsg::appendI16(qint16 value) { doAppendW(static_cast<quint16>(value)); }
   void PalMsg::appendU16(quint16 value) { doAppendW(value); }
   void PalMsg::appendI32(qint32 value) { doAppendDw(static_cast<quint32>(value)); }
   void PalMsg::appendU32(quint32 value) { doAppendDw(value); }
}
