#include <QByteArray>
#include <QDataStream>

#include "QtApp_ByteArray.h"
#include "Palace_Host.h"
#include "Palace_NetMsg_Generic.h"
#include <string>

namespace Seville
{
   namespace Palace
   {
      namespace NetMsg
      {
         void Generic::doReset()
         {
            truncate(0);
            reserve(NetMsg::kByteSizeOfHeader);
         }

         i64 Generic::doSwapI64(i64 value)
         {
            auto res =
                  (((value >> 56) & 0x00000000000000ff) |
                   ((value << 40) & 0x00ff000000000000) |
                   ((value >> 24) & 0x0000000000ff0000) |
                   ((value <<  8) & 0x000000ff00000000) |
                   ((value >>  8) & 0x00000000ff000000) |
                   ((value << 24) & 0x0000ff0000000000) |
                   ((value >> 40) & 0x000000000000ff00) |
                   (static_cast<u64>(value << 56) & 0xff00000000000000));

            return static_cast<i64>(res);
         }

         u64 Generic::doSwapU64(u64 value)
         {
            auto res =
                  (((value >> 56) & 0x00000000000000ff) |
                   ((value << 40) & 0x00ff000000000000) |
                   ((value >> 24) & 0x0000000000ff0000) |
                   ((value <<  8) & 0x000000ff00000000) |
                   ((value >>  8) & 0x00000000ff000000) |
                   ((value << 24) & 0x0000ff0000000000) |
                   ((value >> 40) & 0x000000000000ff00) |
                   (static_cast<u64>(value << 56) & 0xff00000000000000));

            return res;
         }

         i32 Generic::doSwapI32(i32 value)
         {
            auto res =
                  (((value >> 24) & 0x000000ff) |
                   ((value <<  8) & 0x00ff0000) |
                   ((value >>  8) & 0x0000ff00) |
                   (static_cast<u32>(value << 24) & 0xff000000));

            return static_cast<i32>(res);
         }

         u32 Generic::doSwapU32(u32 unswapped)
         {
            return (((unswapped >> 24) & 0x000000ff) |
                    ((unswapped <<  8) & 0x00ff0000) |
                    ((unswapped >>  8) & 0x0000ff00) |
                    ((unswapped << 24) & 0xff000000));
         }

         i16 Generic::doSwapI16(i16 value)
         {
            return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
         }

         u16 Generic::doSwapU16(u16 unswapped)
         {
            return (((unswapped >> 8) & 0xff) | ((unswapped << 8) & 0xff));
         }

         i64 Generic::doMaybeSwapI64(i64 value) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            i64 swappedValue = doSwapI64(value);
            auto result =
                  static_cast<i64>(cond) * swappedValue |
                  static_cast<i64>(!cond) * value;

            return result;
         }

         u64 Generic::doMaybeSwapU64(u64 value) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            u64 swappedValue = doSwapU64(value);
            auto result =
                  static_cast<u64>(cond) * swappedValue |
                  static_cast<u64>(!cond) * value;

            return result;
         }

         i32 Generic::doMaybeSwapI32(i32 value) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            i32 swappedValue = doSwapI32(value);
            auto result =
                  static_cast<i32>(cond) * swappedValue |
                  static_cast<i32>(!cond) * value;

            return result;
         }

         u32 Generic::doMaybeSwapU32(u32 unswapped) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            u32 swapped = doSwapU32(unswapped);
            auto value =
                  static_cast<u32>(cond) * swapped |
                  static_cast<u32>(!cond) * unswapped;

            return value;
         }

         u16 Generic::doMaybeSwapU16(u16 unswapped) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            auto swapped = doSwapU16(unswapped);
            auto value = static_cast<u16>(
                     static_cast<u16>(cond) * swapped |
                     static_cast<u16>(!cond) * unswapped);

            return value;
         }

         i16 Generic::doMaybeSwapI16(i16 unswapped) const
         {
            //auto cond = doDetermineShouldSwapEndianness();
            auto cond = myShouldSwapEndiannessFlag;
            auto swapped = doSwapI16(unswapped);
            auto value = static_cast<i16>(
                     static_cast<i16>(cond) * swapped |
                     static_cast<i16>(!cond) * unswapped);

            return value;
         }

//         qint32 Base::doCsboI32(qint32 unswapped) const
//         {
//            return static_cast<qint32>(doMaybeSwapU32(static_cast<quint32>(unswapped)));
//         }

//         quint32 Base::doCsboU32(quint32 unswapped) const
//         {
//            return doMaybeSwapU32(unswapped);
//         }

//         quint16 Base::doCsbolecW(quint16 unswapped) const
//         {
//            int cond = myClientByteOrder == Host::ByteOrder::LittleEndian;
//            quint16 swapped = doSwapU16(unswapped);

//            return static_cast<quint16>(cond) * swapped | static_cast<quint16>(!cond) * unswapped;
//         }

//         qint16 Base::doCsbolecI16(qint16 unswapped) const
//         {
//            return static_cast<qint16>(doCsbolecW(static_cast<quint16>(unswapped)));
//         }

//         quint16 Base::doCsbolecU16(quint16 unswapped) const
//         {
//            return doCsbolecW(unswapped);
//         }

//         quint32 Base::doCsbolecDw(quint32 unswapped) const
//         {
//            int cond = myClientByteOrder == Host::LittleEndian;
//            quint32 swapped = doSwapU32(unswapped);

//            return static_cast<quint32>(cond) * swapped | static_cast<quint32>(!cond) * unswapped;
//         }

//         qint32 Base::doCsbolecI32(qint32 unswapped) const
//         {
//            return static_cast<qint32>(doCsbolecDw(static_cast<quint32>(unswapped)));
//         }

//         quint32 Base::doCsbolecU32(quint32 unswapped) const
//         {
//            return doCsbolecDw(unswapped);
//         }

         std::string Generic::stdStringAt(int offset, int maxlen)
         {
            return ByteArray::stdStringAt(offset, maxlen);
         }

         void Generic::setStdStringAt(int offset, std::string value)
         {
            ByteArray::setStdStringAt(offset, value);
         }

         i64 Generic::i64At(int offset) const
         {
            return doMaybeSwapI64(ByteArray::i64At(offset));
         }

         void Generic::setI64At(int offset, i64 value)
         {
            ByteArray::setI64At(offset, doMaybeSwapI64(value));
         }

         u64 Generic::u64At(int offset) const
         {
            return doMaybeSwapU64(ByteArray::u64At(offset));
         }

         void Generic::setU64At(int offset, u64 value)
         {
            ByteArray::setU64At(offset, doMaybeSwapU64(value));
         }

         i32 Generic::i32At(int offset) const
         {
            return doMaybeSwapI32(ByteArray::i32At(offset));
         }

         void Generic::setI32At(int offset, i32 value)
         {
            ByteArray::setI32At(offset, doMaybeSwapI32(value));
         }

         u32 Generic::u32At(int offset) const
         {
            return doMaybeSwapU32(ByteArray::u32At(offset));
         }

         void Generic::setU32At(int offset, u32 value)
         {
            ByteArray::setU32At(offset, doMaybeSwapU32(value));
         }

         i16 Generic::i16At(int offset) const
         {
            return doMaybeSwapI16(ByteArray::i16At(offset));
         }

         void Generic::setI16At(int offset, i16 value)
         {
            ByteArray::setI16At(offset, doMaybeSwapI16(value));
         }

         u16 Generic::u16At(int offset) const
         {
            return doMaybeSwapU16(ByteArray::u16At(offset));
         }

         void Generic::setU16At(int offset, u16 value)
         {
            ByteArray::setU16At(offset, doMaybeSwapU16(value));
         }

         i8 Generic::i8At(int offset) const
         {
            return ByteArray::i8At(offset);
         }

         void Generic::setI8At(int offset, i8 value)
         {
            ByteArray::setI8At(offset, value);
         }

         u8 Generic::u8At(int offset) const
         {
            return ByteArray::u8At(offset);
         }

         void Generic::setU8At(int offset, u8 value)
         {
            ByteArray::setU8At(offset, value);
         }

         void Generic::appendI64(i64 value)
         {
            ByteArray::appendI64(doMaybeSwapI64(value));
         }

         void Generic::appendU64(u64 value)
         {
            ByteArray::appendU64(doMaybeSwapU64(value));
         }

         void Generic::appendI32(i32 value)
         {
            ByteArray::appendI32(doMaybeSwapI32(value));
         }

         void Generic::appendU32(u32 value)
         {
            ByteArray::appendU32(doMaybeSwapU32(value));
         }

         void Generic::appendI16(i16 value)
         {
            ByteArray::appendI16(doMaybeSwapI16(value));
         }

         void Generic::appendU16(u16 value)
         {
            ByteArray::appendU16(doMaybeSwapU16(value));
         }

         void Generic::appendI8(i8 value)
         {
            ByteArray::appendI8(value);
         }

         void Generic::appendU8(u8 value)
         {
            ByteArray::appendU8(value);
         }

         int Generic::doReadHeaderFrom(QTcpSocket* socket)
         {
            auto nExpectedBytesToRead =
                  NetMsg::kByteSizeOfHeader - this->size();
            if (0 == nExpectedBytesToRead)
               return 1;
            else if (nExpectedBytesToRead < 0)
               return 1;

            auto nBytesAvailable = socket->bytesAvailable();
            auto shouldReadPartial = nBytesAvailable < nExpectedBytesToRead;
            auto nBytesToRead =
                  (shouldReadPartial * nBytesAvailable) |
                  (!shouldReadPartial * nExpectedBytesToRead);

            this->append(socket->read(nBytesToRead));

            //auto netMsgSizeN = this->size();
            //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;

            return this->size() == NetMsg::kByteSizeOfHeader;
         }

         int Generic::doReadBodyFrom(QTcpSocket* socket)
         {
            auto size0 = this->size();
            if (size0 < NetMsg::kByteSizeOfHeader)
               return 0;

            auto bodySize = size0 - NetMsg::kByteSizeOfHeader;
            auto nBytesExpected = this->sizeExpected() - bodySize;

            if (0 == nBytesExpected)
               return 1;
            else if (nBytesExpected < 0)
               return 0;

            auto nBytesAvailable = socket->bytesAvailable();
            auto shouldReadPartial = nBytesAvailable < nBytesExpected;
            auto nBytesToRead =
                  (shouldReadPartial * nBytesAvailable) |
                  (!shouldReadPartial * nBytesExpected);

            // maybe reads should be chunked?
            this->append(socket->read(nBytesToRead));

            //auto sizeN = this->size();
            //auto nBytesRead = finalNetMsgSize - initialNetMsgSize;

            auto nExpectedTotalBytes =
                  NetMsg::kByteSizeOfHeader + this->sizeExpected();

            return this->size() == nExpectedTotalBytes;
         /*
            if (0 < nNet::MsgBytesExpected)
            {
               doReadDataIntoNet::Msg(netMsg, Net::Msg::kHeaderSize + nNet::MsgBytesExpected);
            }

            auto nNet::MsgBytesReadTotal = netMsg.size();
            auto nPayloadBytesReadTotal = nNet::MsgBytesReadTotal - Net::Msg::kHeaderSize;
            return nPayloadBytesReadTotal == nNet::MsgBytesExpected;
            */
         }

         int Generic::readFrom(QTcpSocket* socket)
         {
            // read in header
            auto readHeaderOk = false;
            if (this->size() < NetMsg::kByteSizeOfHeader)
               readHeaderOk = doReadHeaderFrom(socket);

            int readBodyOk = doReadBodyFrom(socket);
            int minNetMsgOk = NetMsg::kByteSizeOfHeader <= this->size();

            return readHeaderOk && readBodyOk && minNetMsgOk;
         }

         Generic::~Generic()
         {

         }

         Generic::Generic(bool shouldSwapEndianness)
            : ByteArray::ByteArray()
         {
            doReset();

            myShouldSwapEndiannessFlag = shouldSwapEndianness;
         }

         Generic::Generic(
               Host::ByteOrder clientByteOrder,
               Host::ByteOrder serverByteOrder,
               bool shouldSwapEndianness)
         {
            doReset();

            myClientByteOrder = clientByteOrder;
            myServerByteOrder = serverByteOrder;
            myShouldSwapEndiannessFlag = shouldSwapEndianness;
         }

         Generic::Generic(const char* data, bool shouldSwapEndianness)
            : ByteArray()
         {
            doReset();
            //int *p = static_cast<int *>(data);
            ////id_ = static_cast<int *>(p)[kPalMsgIdOffset];
            //id_ = p[kPalMsgIdOffset];
            //len_ = p[kPalMsgLenOffset];
            //ref_ = p[kPalMsgRefOffset];
            myShouldSwapEndiannessFlag = shouldSwapEndianness;
            append(data);
         }

         Generic::Generic(QByteArray& ba, bool shouldSwapEndianness)
            : ByteArray()
         {
            doReset();
            myShouldSwapEndiannessFlag = shouldSwapEndianness;
            append(ba);
         }

         Generic::Generic(Generic& netMsg, bool shouldSwapEndianness)
            : ByteArray()
         {
            doReset();
            //id_ = palMsg.id_;
            //len_ = palMsg.len_;
            //ref_ = palMsg.ref_;
            myShouldSwapEndiannessFlag = shouldSwapEndianness;
            append(netMsg);
         }
      }
   }
}
