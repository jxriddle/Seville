#include "Common.h"
#include "QtApp_ByteArray.h"

namespace QtApp
{
   i64 ByteArray::doGetI64At(int offset) const
   {
      i64 value;
      bi64 bi;

      int z = static_cast<int>(sizeof(i64));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<i64>(bi.qword);

      return value;
   }

   void ByteArray::doSetI64At(int offset, i64 value)
   {
      bi64 bi;
      bi.qword = static_cast<i64>(value);
      auto z = static_cast<int>(sizeof(i64));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   u64 ByteArray::doGetU64At(int offset) const
   {
      u64 value;
      bi64 bi;

      int z = static_cast<int>(sizeof(i64));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<u64>(bi.qword);

      return value;
   }

   void ByteArray::doSetU64At(int offset, u64 value)
   {
      bi64 bi;
      bi.qword = static_cast<i64>(value);
      auto z = static_cast<int>(sizeof(u64));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   i32 ByteArray::doGetI32At(int offset) const
   {
      i32 value;
      //u32 value = at(offset) << 24;
      //value |= at(offset+1) << 16;
      //value |= at(offset+2) << 8;
      //value |= at(offset+3);
      bi32 bi;
      int z = static_cast<int>(sizeof(i32));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<i32>(bi.dword);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */

      //return doCsboI32(value);
      return value;
   }

   void ByteArray::doSetI32At(int offset, i32 value)
   {
      bi32 bi;
      bi.dword = static_cast<i32>(value);
      auto z = static_cast<int>(sizeof(i32));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   u32 ByteArray::doGetU32At(int offset) const
   {
      u32 value;
      //u32 value = at(offset) << 24;
      //value |= at(offset+1) << 16;
      //value |= at(offset+2) << 8;
      //value |= at(offset+3);
      bi32 bi;
      int z = static_cast<int>(sizeof(u32));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<u32>(bi.dword);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */

      //return doCsboU32(value);
      return value;
   }

   void ByteArray::doSetU32At(int offset, u32 value)
   {
      bi32 bi;
      bi.dword = static_cast<i32>(value);
      auto z = static_cast<int>(sizeof(u32));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   i16 ByteArray::doGetI16At(int offset) const
   {
      i16 value;
      //i16 value = at(offset) << 8;
      //value |= at(offset+1);

      bi16 bi;
      int z = static_cast<int>(sizeof(i16));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<i16>(bi.word);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      //return doCsboI16(value);
      return value;
   }

   void ByteArray::doSetI16At(int offset, i16 value)
   {
      bi16 bi;
       //doCsboW(static_cast<u16>(value));
      bi.word = static_cast<i16>(value);
      auto z = static_cast<int>(sizeof(i16));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   u16 ByteArray::doGetU16At(int offset) const
   {
      u16 value;
      //u16 value = at(offset) << 8;
      //value |= at(offset+1);

      bi16 bi;
      int z = static_cast<int>(sizeof(u16));
      for (auto i = 0; i < z; i++) {
         bi.ch[i] = static_cast<unsigned char>(at(offset+i));
      }
      value = static_cast<u16>(bi.word);

      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      //return doCsboU16(value);
      return value;
   }

   void ByteArray::doSetU16At(int offset, u16 value)
   {
      bi16 bi;
      bi.word = static_cast<i16>(value);
      auto z = static_cast<int>(sizeof(u16));
      for (auto i = 0; i < z; i++) {
         insert(static_cast<int>(offset+i),
                static_cast<char>(bi.ch[i]));
      }
   }

   i8 ByteArray::doGetI8At(int offset) const
   {
      i8 value = at(offset);
      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return value;
   }

   void ByteArray::doSetI8At(int offset, i8 value)
   {
      insert(static_cast<int>(offset), static_cast<char>(value));
   }

   u8 ByteArray::doGetU8At(int offset) const
   {
      u8 value = static_cast<u8>(at(static_cast<int>(offset)));
      /*
      QDataStream ds(this, QIODevice::ReadOnly);
      ds.skipRawData(offset);
      ds >> value;
      */
      return value;
   }

   void ByteArray::doSetU8At(int offset, u8 value)
   {
      insert(static_cast<int>(offset), static_cast<char>(value));
   }

   void ByteArray::doAppendI64(i64 value)
   {
      bi64 bi;
      bi.qword = static_cast<i64>(value);
      int z = static_cast<int>(sizeof(i64));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendU64(u64 value)
   {
      bi64 bi;
      bi.qword = static_cast<i64>(value);
      int z = static_cast<int>(sizeof(u64));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendI32(i32 value)
   {
      bi32 bi;
      bi.dword = static_cast<i32>(value);
      int z = static_cast<int>(sizeof(i32));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendU32(u32 value)
   {
      //append(static_cast<char>((value) & 0xff));
      //append(static_cast<char>((value >> 8) & 0xff));
      //append(static_cast<char>((value >> 16) & 0xff));
      //append(static_cast<char>((value >> 24) & 0xff));
      bi32 bi;
      bi.dword = static_cast<i32>(value);
      int z = static_cast<int>(sizeof(u32));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendI16(i16 value)
   {
      bi16 bi;
      bi.word = static_cast<i16>(value);
      int z = static_cast<int>(sizeof(i16));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendU16(u16 value)
   {
      //append(static_cast<char>(value & 0xff));
      //append(static_cast<char>((value >> 8) & 0xff));
      bi16 bi;
      //bi.word = doCsboW(value);
      bi.word = static_cast<i16>(value);
      int z = static_cast<int>(sizeof(u16));
      for (auto i = 0; i < z; i++) {
         append(static_cast<char>(bi.ch[i]));
      }
   }

   void ByteArray::doAppendI8(i8 value)
   {
      append(value);
   }

   void ByteArray::doAppendU8(u8 value)
   {
      append(static_cast<i8>(value));
   }

   ByteArray::ByteArray()
   {

   }
}
