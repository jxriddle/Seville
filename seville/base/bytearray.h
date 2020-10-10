#ifndef SEVILLE_BYTEARRAY_H_
#define SEVILLE_BYTEARRAY_H_

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

      inline auto pascal_string_at(int offset, u8 maxlen) const -> std::string {
         return do_pascalStringAt(offset, maxlen);
      }

      inline auto set_pascal_string_at(int offset, std::string value) -> void {
         do_setPascalStringAt(offset, value);
      }

//      inline auto stringU32At(int offset, u32 maxlen) const -> std::string {
//         return do_stringU32At_(offset, maxlen);
//      }

//      inline auto setStringU32At(int offset, std::string value) -> void {
//         do_setStringU32At_(offset, value);
//      }

      inline auto string_at(int offset, int len) const -> std::string {
         return do_stringAt(offset, len);
      }

      inline auto set_string_at(int offset, const std::string& value) -> void {
         do_setStringAt(offset, value);
      }

      inline auto pascal_qstring_at(int offset, u8 maxlen) const -> QString {
         return do_pascalQStringAt(offset, maxlen);
      }

      inline auto set_pascal_qstring_at(int offset, const QString& value) -> void {
         do_setPascalQStringAt(offset, value);
      }

//      inline auto qStringU32At(int offset, u32 maxlen) const -> QString {
//         return qStringU32At_(offset, maxlen);
//      }

//      inline auto setQStringU32At(int offset, const QString& value) -> void {
//         setQStringU32At_(offset, value);
//      }

      inline auto qstring_at(int offset, int maxlen) const -> QString {
         return do_qStringAt(offset, maxlen);
      }

      inline auto set_qstring_at(int offset, const QString& value) -> void {
         do_setQStringAt(offset, value);
      }

      inline auto i64_at(int offset) const -> i64 {
         return do_i64At(offset);
      }

      inline auto set_i64_at(int offset, i64 value) -> void {
         do_setI64At(offset, value);
      }

      inline auto u64At(int offset) const -> u64 {
         return do_u64At(offset);
      }

      inline auto set_u64_at(int offset, u64 value) -> void {
         do_setU64At(offset, value);
      }

      inline auto i32At(int offset) const -> i32 {
         return do_i32At(offset);
      }

      inline auto set_i32_at(int offset, i32 value) -> void {
         do_setI32At(offset, value);
      }

      inline auto u32At(int offset) const -> u32 {
         return do_u32At(offset);
      }

      inline auto set_u32_at(int offset, u32 value) -> void {
         do_setU32At(offset, value);
      }

      inline auto i16At(int offset) const -> i16 {
         return do_i16At(offset);
      }

      inline auto set_i16_at(int offset, i16 value) -> void {
         do_setI16At(offset, value);
      }

      inline auto u16At(int offset) const -> u16 {
         return do_u16At(offset);
      }

      inline auto set_u16_at(int offset, u16 value) -> void {
         do_setU16At(offset, value);
      }

      inline auto i8_at(int offset) const -> i8 {
         return i8At_(offset);
      }

      inline auto set_i8_at(int offset, i8 value) -> void {
         do_setI8At(offset, value);
      }

      inline auto u8_at(int offset) const -> u8 {
         return u8At_(offset);
      }

      inline auto set_u8_at(int offset, u8 value) -> void {
         do_setU8At(offset, value);
      }

      inline auto append_i64(i64 value) -> void {
         do_appendI64(value);
      }

      inline auto append_u64(u64 value) -> void {
         do_appendU64(value);
      }

      inline auto append_i32(i32 value) -> void {
         do_appendI32(value);
      }

      inline auto append_u32(u32 value) -> void {
         do_appendU32(value);
      }

      inline auto append_i16(i16 value) -> void {
         do_appendI16(value);
      }

      inline auto append_u16(u16 value) -> void {
         do_appendU16(value);
      }

      inline auto append_i8(i8 value) -> void {
         do_appendI8(value);
      }

      inline auto append_u8(u8 value) -> void {
         do_appendU8(static_cast<char>(value));
      }

   private:
      auto do_setPascalStringAt(int offset, const std::string& value) -> void {
         auto len = static_cast<u8>(value.length());
         do_setU8At(offset, len);
         for (u8 i = 1; i < len; i++)
            set_u8_at(offset+i, static_cast<u8>(value.at(i)));
      }

      auto do_pascalStringAt(int offset, u8 maxlen) const -> std::string {
         std::string value;

         auto len = u8At_(offset);
         u8 i = 1;
         auto c = this->i8_at(offset+i);
         while (i < maxlen && i < len && c != '\0') {
            value.push_back(c);
            i++;
            c = i8At_(offset+i);
         }

         if (value.size() == sizeof(u8) || value.size() == maxlen)
            value.pop_back();

         value.push_back('\0');

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

      auto do_setStringAt(int offset, const std::string& value) -> void {
         auto len = static_cast<u32>(value.length());
         for (auto i = u32{0}; i < len; i++)
            set_u8_at(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      auto do_stringAt(int offset, int len) const -> std::string {
         std::string value;

         for (auto i = 0; i < len; i++)
            value.push_back(this->at(offset+i));

         return value;
      }

      auto do_setPascalQStringAt(int offset, const QString& value) -> void {
         do_setPascalStringAt(offset, value.toStdString());
      }

      auto do_pascalQStringAt(int offset, u8 maxlen) const -> QString {
         auto s = do_pascalStringAt(offset, maxlen);
         return QString::fromStdString(s);
      }

//      auto do_setQStringU32At(int offset, const QString& value) -> void {
//         do_setStringU32At(offset, value.toStdString());
//      }

//      auto do_qStringU32At(int offset, u32 maxlen) const -> QString {
//         auto s = do_stringU32At(offset, maxlen);
//         return QString::fromStdString(s);
//      }

      auto do_setQStringAt(int offset, const QString& value) -> void {
         do_setStringAt(offset, value.toStdString());
      }

      auto do_qStringAt(int offset, int len) const -> QString {
         auto s = do_stringAt(offset, len);
         return QString::fromStdString(s);
      }

      auto do_i64At(int offset) const -> i64 {
         i64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<i64>(bi.qword);

         return value;
      }

      auto do_setI64At(int offset, i64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto do_u64At(int offset) const -> u64 {
         u64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<u64>(bi.qword);

         return value;
      }

      auto do_setU64At(int offset, u64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto do_i32At(int offset) const -> i32 {
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

      auto do_setI32At(int offset, i32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto do_u32At(int offset) const -> u32 {
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

      auto do_setU32At(int offset, u32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto do_i16At(int offset) const -> i16 {
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

      auto do_setI16At(int offset, i16 value) -> void {
         bi16 bi;
          //doCsboW(static_cast<u16>(value));
         bi.word = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto do_u16At(int offset) const -> u16 {
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

      auto do_setU16At(int offset, u16 value) -> void {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(u16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      auto i8At_(int offset) const -> i8 {
         i8 value = at(offset);
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      auto do_setI8At(int offset, i8 value) -> void {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      auto u8At_(int offset) const -> u8 {
         u8 value = static_cast<u8>(at(static_cast<int>(offset)));
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      auto do_setU8At(int offset, u8 value) -> void {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      auto do_appendI64(i64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendU64(u64 value) -> void {
         bi64 bi;
         bi.qword = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendI32(i32 value) -> void {
         bi32 bi;
         bi.dword = static_cast<i32>(value);
         int z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendU32(u32 value) -> void {
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

      auto do_appendI16(i16 value) -> void {
         bi16 bi;
         bi.word = static_cast<i16>(value);
         int z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendU16(u16 value) -> void {
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

      auto do_appendI8(i8 value) -> void {
         append(value);
      }

      auto do_appendU8(u8 value) -> void {
         append(static_cast<i8>(value));
      }
   };
}

#endif   // SEVILLE_BYTEARRAY_H_
