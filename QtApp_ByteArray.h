#ifndef QT_APP_BYTE_ARRAY_H
#define QT_APP_BYTE_ARRAY_H

#include "Common.h"

#include <QByteArray>

namespace QtApp
{
   class ByteArray : public QByteArray
   {
      private:
         i64 doGetI64At(int offset) const;
         void doSetI64At(int offset, i64 value);

         u64 doGetU64At(int offset) const;
         void doSetU64At(int offset, u64 value);

         i32 doGetI32At(int offset) const;
         void doSetI32At(int offset, i32 value);

         u32 doGetU32At(int offset) const;
         void doSetU32At(int offset, u32 value);

         i16 doGetI16At(int offset) const;
         void doSetI16At(int offset, i16 value);

         u16 doGetU16At(int offset) const;
         void doSetU16At(int offset, u16 value);

         i8 doGetI8At(int offset) const;
         void doSetI8At(int offset, i8 value);

         u8 doGetU8At(int offset) const;
         void doSetU8At(int offset, u8 value);

         void doAppendI64(i64 value);
         void doAppendU64(u64 value);
         void doAppendI32(i32 value);
         void doAppendU32(u32 value);
         void doAppendI16(i16 value);
         void doAppendU16(u16 value);
         void doAppendI8(i8 value);
         void doAppendU8(u8 value);

      public:
         i64 i64At(int offset) const { return doGetI64At(offset); }
         void setI64At(int offset, i64 value) { doSetI64At(offset, value); }

         u64 u64At(int offset) const { return doGetU64At(offset); }
         void setU64At(int offset, u64 value) { doSetU64At(offset, value); }

         i32 i32At(int offset) const { return doGetI32At(offset); }
         void setI32At(int offset, i32 value) { doSetI32At(offset, value); }

         u32 u32At(int offset) const { return doGetU32At(offset); }
         void setU32At(int offset, u32 value) { doSetU32At(offset, value); }

         i16 i16At(int offset) const { return doGetI16At(offset); }
         void setI16At(int offset, i16 value) { doSetI16At(offset, value); }

         u16 u16At(int offset) const { return doGetU16At(offset); }
         void setU16At(int offset, u16 value) { doSetU16At(offset, value); }

         i8 i8At(int offset) const { return doGetI8At(offset); }
         void setI8At(int offset, i8 value) { doSetI8At(offset, value); }

         u8 u8At(int offset) const { return doGetU8At(offset); }
         void setU8At(int offset, u8 value) { doSetU8At(offset, value); }

         void appendI64(i64 value) { doAppendI64(value); }
         void appendU64(u64 value) { doAppendU64(value); }
         void appendI32(i32 value) { doAppendI32(value); }
         void appendU32(u32 value) { doAppendU32(value); }
         void appendI16(i16 value) { doAppendI16(value); }
         void appendU16(u16 value) { doAppendU16(value); }
         void appendI8(i8 value) { append(value); }
         void appendU8(u8 value) { append(static_cast<char>(value)); }

         ByteArray();
   };
}

#endif // QT_APP_BYTE_ARRAY_H
