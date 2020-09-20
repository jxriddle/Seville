#ifndef ZED_BYTE_ARRAY_H
#define ZED_BYTE_ARRAY_H

#include "Zed_Types.h"

#include <QByteArray>
#include <QChar>
#include <QString>

namespace Zed
{
   class ByteArray : public QByteArray
   {
   private:
      void do_setStringU8At(int offset, const std::string& value)
      {
         auto len = static_cast<u8>(value.length());
         do_setU8At(offset, len);
         for (u8 i = 1; i < len; i++)
            setU8At(offset+i, static_cast<u8>(value.at(i)));
      }

      std::string do_stringU8At(int offset, u8 maxlen) const
      {
         std::string value;

         auto len = this->u8At(offset);
         u8 i = 1;
         auto c = this->i8At(offset+i);
         while (i < maxlen && i < len && c != '\0') {
            value.push_back(c);
            i++;
            c = this->i8At(offset+i);
         }

         if (value.size() == sizeof(u8) || value.size() == maxlen)
            value.pop_back();

         value.push_back('\0');

         return value;
      }

      void do_setStringU32At(int offset, const std::string& value)
      {
         auto len = static_cast<u32>(value.length());
         do_setU32At(offset, len);
         for (u32 i = 1; i < len; i++)
            setU8At(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      std::string do_stringU32At(int offset, u32 maxlen) const
      {
         std::string value;

         auto len = this->u32At(offset);
         u32 i = 1;
         auto c = this->i8At(offset+static_cast<i32>(i));
         while (i < maxlen && i < len && c != '\0') {
            value.push_back(c);
            i++;
            c = this->at(offset+static_cast<i32>(i));
         }

         if (value.size() == sizeof(u32) || value.size() == maxlen)
            value.pop_back();

         value.push_back('\0');

         return value;
      }

      void do_setStringAt(int offset, const std::string& value)
      {
         auto len = static_cast<u32>(value.length());
         for (u32 i = 0; i < len; i++)
            setU8At(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      std::string do_stringAt(int offset, int maxlen) const
      {
         std::string value;

         auto i = 0;
         auto c = this->at(offset);
         while (c != '\0' && i < maxlen)
         {
            value.push_back(c);
            i++;
            c = this->at(offset + i);
         }

         return value;
      }

      std::string do_stringAt(int offset) const
      {
         std::string value;

         auto i = 0;
         auto c = this->at(offset);
         while (c != '\0')
         {
            value.push_back(c);
            i++;
            c = this->at(offset + i);
         }

         return value;
      }

      void do_setQStringU8At(int offset, const QString& value)
      {
         do_setStringU8At(offset, value.toStdString());
      }

      QString do_qStringU8At(int offset, u8 maxlen) const
      {
         auto s = do_stringU8At(offset, maxlen);
         return QString::fromStdString(s);
      }

      void do_setQStringU32At(int offset, const QString& value)
      {
         do_setStringU32At(offset, value.toStdString());
      }

      QString do_qStringU32At(int offset, u32 maxlen) const
      {
         auto s = do_stringU32At(offset, maxlen);
         return QString::fromStdString(s);
      }

      void do_setQStringAt(int offset, const QString& value)
      {
         do_setStringAt(offset, value.toStdString());
      }

      QString do_qStringAt(int offset, int len) const
      {
         auto s = do_stringAt(offset, len);
         return QString::fromStdString(s);
      }

      QString do_qStringAt(int offset) const
      {
         auto s = do_stringAt(offset);
         return QString::fromStdString(s);
      }

      i64 do_i64At(int offset) const
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

      void do_setI64At(int offset, i64 value)
      {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      u64 do_u64At(int offset) const
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

      void do_setU64At(int offset, u64 value)
      {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      i32 do_i32At(int offset) const
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

      void do_setI32At(int offset, i32 value)
      {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      u32 do_u32At(int offset) const
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

      void do_setU32At(int offset, u32 value)
      {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      i16 do_i16At(int offset) const
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

      void do_setI16At(int offset, i16 value)
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

      u16 do_u16At(int offset) const
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

      void do_setU16At(int offset, u16 value)
      {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(u16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      i8 do_i8At(int offset) const
      {
         i8 value = at(offset);
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      void do_setI8At(int offset, i8 value)
      {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      u8 do_u8At(int offset) const
      {
         u8 value = static_cast<u8>(at(static_cast<int>(offset)));
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      void do_setU8At(int offset, u8 value)
      {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      void do_appendI64(i64 value)
      {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      void do_appendU64(u64 value)
      {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      void do_appendI32(i32 value)
      {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         int z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      void do_appendU32(u32 value)
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

      void do_appendI16(i16 value)
      {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         int z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      void do_appendU16(u16 value)
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

      void do_appendI8(i8 value)
      {
         append(value);
      }

      void do_appendU8(u8 value)
      {
         append(static_cast<i8>(value));
      }

      void do_init()
      {
      }

   public:
      void setStringU8At(int offset, std::string value)
         { do_setStringU8At(offset, value); }
      std::string stringU8At(int offset, u8 maxlen) const
         { return do_stringU8At(offset, maxlen); }

      void setStringU32At(int offset, std::string value)
         { do_setStringU32At(offset, value); }
      std::string stringU32At(int offset, u32 maxlen) const
         { return do_stringU32At(offset, maxlen); }

      void setStringAt(int offset, const std::string& value)
         { do_setStringAt(offset, value); }
      std::string stringAt(int offset, int len) const
         { return do_stringAt(offset, len); }

      void setQStringU8At(int offset, const QString& value)
         { do_setQStringU8At(offset, value); }
      QString qStringU8At(int offset, u8 maxlen) const
         { return do_qStringU8At(offset, maxlen); }

      void setQStringU32At(int offset, const QString& value)
         { do_setQStringU32At(offset, value); }
      QString qStringU32At(int offset, u32 maxlen) const
         { return do_qStringU32At(offset, maxlen); }

      void setQStringAt(int offset, const QString& value)
         { do_setQStringAt(offset, value); }
      QString qStringAt(int offset, int maxlen) const
         { return do_qStringAt(offset, maxlen); }
      QString qStringAt(int offset) const
         { return do_qStringAt(offset); }

      i64 i64At(int offset) const { return do_i64At(offset); }
      void setI64At(int offset, i64 value) { do_setI64At(offset, value); }

      u64 u64At(int offset) const { return do_u64At(offset); }
      void setU64At(int offset, u64 value) { do_setU64At(offset, value); }

      i32 i32At(int offset) const { return do_i32At(offset); }
      void setI32At(int offset, i32 value) { do_setI32At(offset, value); }

      u32 u32At(int offset) const { return do_u32At(offset); }
      void setU32At(int offset, u32 value) { do_setU32At(offset, value); }

      i16 i16At(int offset) const { return do_i16At(offset); }
      void setI16At(int offset, i16 value) { do_setI16At(offset, value); }

      u16 u16At(int offset) const { return do_u16At(offset); }
      void setU16At(int offset, u16 value) { do_setU16At(offset, value); }

      i8 i8At(int offset) const { return do_i8At(offset); }
      void setI8At(int offset, i8 value) { do_setI8At(offset, value); }

      u8 u8At(int offset) const { return do_u8At(offset); }
      void setU8At(int offset, u8 value) { do_setU8At(offset, value); }

      void appendI64(i64 value) { do_appendI64(value); }
      void appendU64(u64 value) { do_appendU64(value); }
      void appendI32(i32 value) { do_appendI32(value); }
      void appendU32(u32 value) { do_appendU32(value); }
      void appendI16(i16 value) { do_appendI16(value); }
      void appendU16(u16 value) { do_appendU16(value); }
      void appendI8(i8 value) { append(value); }
      void appendU8(u8 value) { append(static_cast<char>(value)); }

      ByteArray(void);
   };
}

#endif // QT_APP_BYTE_ARRAY_H
