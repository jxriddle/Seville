#ifndef SEVILLE_BYTEARRAYOBJECT_H_
#define SEVILLE_BYTEARRAYOBJECT_H_

#include <optional>

#include <QByteArray>
#include <QChar>
#include <QString>

#include "seville/base/types.h"

namespace seville
{
   class ByteArray : public QByteArray
   {
   public:
      ByteArray(void);

      static auto New(void) -> std::unique_ptr<ByteArray>;
      //std::optional<ByteArray*>;

      inline auto PascalStringAt(int offset) const -> std::string {
         return pascal_string_at(offset);
      }

      inline auto SetPascalStringAt(int offset, std::string value) -> void {
         set_pascal_string_at(offset, value);
      }

//      inline auto stringU32At(int offset, u32 maxlen) const -> std::string {
//         return do_stringU32At_(offset, maxlen);
//      }

//      inline auto setStringU32At(int offset, std::string value) -> void {
//         do_setStringU32At_(offset, value);
//      }

      inline auto StringAt(int offset, int len) const -> std::string {
         return string_at(offset, len);
      }

      inline auto SetStringAt(int offset, const std::string& value) -> void {
         set_string_at(offset, value);
      }

      inline auto PascalQStringAt(int offset) const -> QString {
         return pascal_qstring_at(offset);
      }

      inline auto SetPascalQStringAt(int offset, const QString& value) -> void {
         set_pascal_qstring_at(offset, value);
      }

//      inline auto qStringU32At(int offset, u32 maxlen) const -> QString {
//         return qStringU32At_(offset, maxlen);
//      }

//      inline auto setQStringU32At(int offset, const QString& value) -> void {
//         setQStringU32At_(offset, value);
//      }

      inline auto QStringAt(int offset, int maxlen) const -> QString {
         return qstring_at(offset, maxlen);
      }

      inline auto SetQStringAt(int offset, const QString& value) -> void {
         set_qstring_at(offset, value);
      }

      inline auto I64At(int offset) const -> i64 {
         return i64_at(offset);
      }

      inline auto SetI64At(int offset, i64 value) -> void {
         set_i64_at(offset, value);
      }

      inline auto U64At(int offset) const -> u64 {
         return u64_at(offset);
      }

      inline auto SetU64At(int offset, u64 value) -> void {
         set_u64_at(offset, value);
      }

      inline auto I32At(int offset) const -> i32 {
         return i32_at(offset);
      }

      inline auto SetI32At(int offset, i32 value) -> void {
         set_i32_at(offset, value);
      }

      inline auto U32At(int offset) const -> u32 {
         return u32_at(offset);
      }

      inline auto SetU32At(int offset, u32 value) -> void {
         set_u32_at(offset, value);
      }

      inline auto I16At(int offset) const -> i16 {
         return i16_at(offset);
      }

      inline auto SetI16At(int offset, i16 value) -> void {
         set_i16_at(offset, value);
      }

      inline auto U16At(int offset) const -> u16 {
         return u16_at(offset);
      }

      inline auto SetU16At(int offset, u16 value) -> void {
         set_u16_at(offset, value);
      }

      inline auto I8At(int offset) const -> i8 {
         return i8_at(offset);
      }

      inline auto SetI8At(int offset, i8 value) -> void {
         set_i8_at(offset, value);
      }

      inline auto U8At(int offset) const -> u8 {
         return u8_at(offset);
      }

      inline auto SetU8At(int offset, u8 value) -> void {
         set_u8_at(offset, value);
      }

      inline auto AppendI64(i64 value) -> void {
         append_i64(value);
      }

      inline auto AppendU64(u64 value) -> void {
         append_u64(value);
      }

      inline auto AppendI32(i32 value) -> void {
         append_i32(value);
      }

      inline auto AppendU32(u32 value) -> void {
         append_u32(value);
      }

      inline auto AppendI16(i16 value) -> void {
         append_i16(value);
      }

      inline auto AppendU16(u16 value) -> void {
         append_u16(value);
      }

      inline auto AppendI8(i8 value) -> void {
         append_i8(value);
      }

      inline auto AppendU8(u8 value) -> void {
         append_u8(static_cast<unsigned char>(value));
      }

   private:
      auto set_pascal_string_at(int offset, const std::string& value) -> void {
         auto len = static_cast<u8>(value.length());
         set_u8_at(offset, len);

         for (u8 i = 0; i < len; i++)
            SetU8At(offset+i+1, static_cast<u8>(value.at(i)));
      }

      auto pascal_string_at(int offset) const -> std::string {
         std::string value;
         auto len = u8_at(offset);
         for (u8 i = 0; i < len; i++)
            value.push_back(i8_at(offset+i+1));
         return value;
      }

//      auto do_setStringU32At(int offset, const std::string& value) -> void {
//         auto len = static_cast<u32>(value.length());
//         setU32At_(offset, len);
//         for (u32 i = 1; i < len; i++)
//            setU8At_(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
//      }

//      auto do_stringU32At(int offset, u32 maxlen) const -> std::string {
//         std::string value;

//         auto len = u32At_(offset);
//         u32 i = 1;
//         auto c = i8At_(offset+static_cast<i32>(i));
//         while (i < maxlen && i < len && c != '\0') {
//            value.push_back(c);
//            i++;
//            c = this->at(offset+static_cast<i32>(i));
//         }

//         if (value.size() == sizeof(u32) || value.size() == maxlen)
//            value.pop_back();

//         value.push_back('\0');

//         return value;
//      }

      auto set_string_at(int offset, const std::string& value) -> void {
         auto len = static_cast<u32>(value.length());
         for (auto i = u32{0}; i < len; i++)
            SetU8At(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      auto string_at(int offset, int maxlen) const -> std::string {
         std::string value;

         int i = 0;
         while (this->at(offset+i) && i < maxlen) {
            value.push_back(this->at(offset+i));
            i++;
         }

         return value;
      }

      auto set_pascal_qstring_at(int offset, const QString& value) -> void {
         set_pascal_string_at(offset, value.toStdString());
      }

      auto pascal_qstring_at(int offset) const -> QString {
         auto s = pascal_string_at(offset);
         return QString::fromStdString(s);
      }

//      auto do_setQStringU32At(int offset, const QString& value) -> void {
//         do_setStringU32At(offset, value.toStdString());
//      }

//      auto do_qStringU32At(int offset, u32 maxlen) const -> QString {
//         auto s = do_stringU32At(offset, maxlen);
//         return QString::fromStdString(s);
//      }

      auto set_qstring_at(int offset, const QString& value) -> void {
         set_string_at(offset, value.toStdString());
      }

      auto qstring_at(int offset, int maxlen) const -> QString {
         auto s = string_at(offset, maxlen);
         return QString::fromStdString(s);
      }

      auto i64_at(int offset) const -> i64 {
         i64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<i64>(bi.qword);

         return value;
      }

      auto set_i64_at(int offset, i64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto u64_at(int offset) const -> u64 {
         u64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<u64>(bi.qword);

         return value;
      }

      auto set_u64_at(int offset, u64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto i32_at(int offset) const -> i32 {
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

      auto set_i32_at(int offset, i32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto u32_at(int offset) const -> u32 {
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

      auto set_u32_at(int offset, u32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto i16_at(int offset) const -> i16 {
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

      auto set_i16_at(int offset, i16 value) -> void {
         bi16 bi;
          //doCsboW(static_cast<u16>(value));
         bi.word = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto u16_at(int offset) const -> u16 {
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

      auto set_u16_at(int offset, u16 value) -> void {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(u16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto i8_at(int offset) const -> i8 {
         i8 value = at(offset);
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      auto set_i8_at(int offset, i8 value) -> void {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      auto u8_at(int offset) const -> u8 {
         u8 value = static_cast<u8>(at(static_cast<int>(offset)));
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      auto set_u8_at(int offset, u8 value) -> void {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      auto append_i64(i64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto append_u64(u64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto append_i32(i32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         int z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto append_u32(u32 value) -> void {
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

      auto append_i16(i16 value) -> void {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         int z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto append_u16(u16 value) -> void {
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

      auto append_i8(i8 value) -> void {
         append(value);
      }

      auto append_u8(u8 value) -> void {
         append(static_cast<i8>(value));
      }
   };
}

#endif   // SEVILLE_BYTEARRAYOBJECT_H_
