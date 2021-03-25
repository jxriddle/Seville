#ifndef SEVILLE_BYTE_ARRAY_H_
#define SEVILLE_BYTE_ARRAY_H_

#include <optional>

#include <QByteArray>
#include <QChar>
#include <QString>
#include <QDataStream>

#include "seville/base/types.h"

namespace seville
{
   class ByteArray : public QByteArray
   {
   public:
      ByteArray(void);
      //ByteArray(QByteArray& ref);

      static auto New(void) -> std::unique_ptr<ByteArray>;
      //std::optional<ByteArray*>;

      static inline auto SwapI64(i64 value) -> i64 {
         auto result =
               (((value >> 56) & 0x00000000000000ff) |
                ((value << 40) & 0x00ff000000000000) |
                ((value >> 24) & 0x0000000000ff0000) |
                ((value <<  8) & 0x000000ff00000000) |
                ((value >>  8) & 0x00000000ff000000) |
                ((value << 24) & 0x0000ff0000000000) |
                ((value >> 40) & 0x000000000000ff00) |
                (static_cast<u64>(value << 56) & 0xff00000000000000));

         return static_cast<i64>(result);
      }

      static inline auto SwapU64(u64 value) -> u64 {
         auto result =
               (((value >> 56) & 0x00000000000000ff) |
                ((value << 40) & 0x00ff000000000000) |
                ((value >> 24) & 0x0000000000ff0000) |
                ((value <<  8) & 0x000000ff00000000) |
                ((value >>  8) & 0x00000000ff000000) |
                ((value << 24) & 0x0000ff0000000000) |
                ((value >> 40) & 0x000000000000ff00) |
                (static_cast<u64>(value << 56) & 0xff00000000000000));

         return result;
      }

      static inline auto SwapI32(i32 value) -> i32 {
         auto result =
               (((value >> 24) & 0x000000ff) |
                ((value <<  8) & 0x00ff0000) |
                ((value >>  8) & 0x0000ff00) |
                (static_cast<u32>(value << 24) & 0xff000000));

         return static_cast<i32>(result);
      }

      static inline auto SwapU32(u32 unswapped) -> u32 {
         return (((unswapped >> 24) & 0x000000ff) |
                 ((unswapped <<  8) & 0x00ff0000) |
                 ((unswapped >>  8) & 0x0000ff00) |
                 ((unswapped << 24) & 0xff000000));
      }

      static inline auto SwapI16(i16 value) -> i16 {
         return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
      }

      static auto SwapU16(u16 value) -> u16 {
         return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
      }

      inline auto shouldSwapEndianness(void) const -> bool {
         return my_swapEndianFlag;
      }

      inline auto setShouldSwapEndianness(bool value) -> void {
         my_swapEndianFlag = value;
      }

      inline auto pascalStringAt(int offset) const -> std::string {
         return do_pascalStringAt(offset);
      }

      inline auto setPascalStringAt(int offset, std::string value) -> void {
         do_setPascalStringAt(offset, value);
      }

//      inline auto stringU32At(int offset, u32 maxlen) const -> std::string {
//         return do_stringU32At_(offset, maxlen);
//      }

//      inline auto setStringU32At(int offset, std::string value) -> void {
//         do_setStringU32At_(offset, value);
//      }

      inline auto stringAt(int offset, int len) const -> std::string {
         return do_stringAt(offset, len);
      }

      inline auto setStringAt(int offset, const std::string& value) -> void {
         do_setStringAt(offset, value);
      }

      inline auto fixedStringAt(int offset, int len) const -> std::string {
         return do_fixedStringAt(offset, len);
      }

      inline auto setFixedStringAt(
            int offset, const std::string& value, int len) -> void {
         do_setFixedStringAt(offset, value, len);
      }

      inline auto pascalQStringAt(int offset) const -> QString {
         return do_pascalQStringAt(offset);
      }

      inline auto setPascalQStringAt(int offset, const QString& value) -> void {
         do_setPascalStringAt(offset, value.toStdString());
      }

//      inline auto qStringU32At(int offset, u32 maxlen) const -> QString {
//         return do_qstring_u32_at_(offset, maxlen);
//      }

//      inline auto setQStringU32At(int offset, const QString& value) -> void {
//         do_set_qstring_u32_at_(offset, value);
//      }

      inline auto qstringAt(int offset, int maxlen) const -> QString {
         return do_qstringAt(offset, maxlen);
      }

      inline auto setQStringAt(int offset, const QString& value) -> void {
         do_setQStringAt(offset, value);
      }

      inline auto appendFixedQString(const QString& value, int len) -> void {
         do_setFixedStringAt(size(), value.toStdString(), len);
      }

      inline auto appendFixedString(const std::string& value, int len) -> void {
         do_setFixedStringAt(size(), value, len);
      }

      inline auto appendQString(const QString& value) -> void {
         do_setQStringAt(size(), value);
         //append(value.toStdString().c_str());
      }

      inline auto appendString(const std::string& value) -> void {
         do_setStringAt(size(), value);
         //append(value.c_str());
      }

      inline auto appendPascalQString(const QString& value) -> void {
         auto offset = size();
         do_setPascalStringAt(offset, value.toStdString());
      }

      inline auto appendPascalString(const std::string& value) -> void {
         auto offset = size();
         do_setPascalStringAt(offset, value);
      }

      inline auto appendFixedPascalQString(
            const QString& value, int len) -> void {
         auto offset = size();
         do_setFixedPascalStringAt(offset, value.toStdString(), len);
      }

      inline auto appendFixedPascalString(const std::string& value, int len) -> void {
         auto offset = size();
         do_setFixedPascalStringAt(offset, value, len);
      }

      inline auto i64At(int offset) const -> i64 {
         return do_i64At(offset);
      }

      inline auto setI64At(int offset, i64 value) -> void {
         do_setI64At(offset, value);
      }

      inline auto u64At(int offset) const -> u64 {
         return do_u64At(offset);
      }

      inline auto setU64At(int offset, u64 value) -> void {
         do_setU64At(offset, value);
      }

      inline auto i32At(int offset) const -> i32 {
         return do_i32At(offset);
      }

      inline auto setI32At(int offset, i32 value) -> void {
         do_setI32At(offset, value);
      }

      inline auto u32At(int offset) const -> u32 {
         return do_u32At(offset);
      }

      inline auto setU32At(int offset, u32 value) -> void {
         do_setU32At(offset, value);
      }

      inline auto i16At(int offset) const -> i16 {
         return do_i16At(offset);
      }

      inline auto setI16At(int offset, i16 value) -> void {
         do_setI16At(offset, value);
      }

      inline auto u16At(int offset) const -> u16 {
         return do_u16At(offset);
      }

      inline auto setU16At(int offset, u16 value) -> void {
         do_setU16At(offset, value);
      }

      inline auto i8At(int offset) const -> i8 {
         return do_i8At(offset);
      }

      inline auto setI8At(int offset, i8 value) -> void {
         do_setI8At(offset, value);
      }

      inline auto u8At(int offset) const -> u8 {
         return do_u8At(offset);
      }

      inline auto setU8At(int offset, u8 value) -> void {
         do_setU8At(offset, value);
      }

      inline auto appendI64(i64 value) -> void {
         do_appendI64(value);
      }

      inline auto appendU64(u64 value) -> void {
         do_appendU64(value);
      }

      inline auto appendI32(i32 value) -> void {
         do_appendI32(value);
      }

      inline auto appendU32(u32 value) -> void {
         do_appendU32(value);
      }

      inline auto appendI16(i16 value) -> void {
         do_appendI16(value);
      }

      inline auto appendU16(u16 value) -> void {
         do_appendU16(value);
      }

      inline auto appendI8(i8 value) -> void {
         do_appendI8(value);
      }

      inline auto appendU8(u8 value) -> void {
         do_appendU8(static_cast<unsigned char>(value));
      }

      inline auto takeI64(void) -> i64 {
         auto value = do_i64At(0);
         this->remove(0, sizeof(i64));
         return value;
      }

      inline auto takeU64(void) -> u64 {
         auto value = do_u64At(0);
         this->remove(0, sizeof(u64));
         return value;
      }

      inline auto takeI32(void) -> i32 {
         auto value = do_i32At(0);
         this->remove(0, sizeof(i32));
         return value;
      }

      inline auto takeU32(void) -> u32 {
         auto value = do_u32At(0);
         this->remove(0, sizeof(u32));
         return value;
      }

      inline auto takeI16(void) -> i16 {
         auto value = do_i16At(0);
         this->remove(0, sizeof(i16));
         return value;
      }

      inline auto takeU16(void) -> u16 {
         auto value = do_u16At(0);
         this->remove(0, sizeof(u16));
         return value;
      }

      inline auto takeI8(void) -> i8 {
         auto value = do_i8At(0);
         this->remove(0, sizeof(i8));
         return value;
      }

      inline auto takeU8(void) -> u8 {
         auto value = do_u8At(0);
         this->remove(0, sizeof(u8));
         return value;
      }

      inline auto takePascalString(void) -> std::string {
         auto value = do_pascalStringAt(0);
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto takePascalQString(void) -> QString {
         auto value = QString::fromStdString(do_pascalStringAt(0));
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto takeString(int maxlen) -> std::string {
         auto value = do_stringAt(0, maxlen);
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto takeQString(int maxlen) -> QString {
         auto value = QString::fromStdString(do_stringAt(0, maxlen));
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto takeFixedString(int len) -> std::string {
         auto value = do_fixedStringAt(0, len);
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto takeFixedQString(int len) -> QString {
         auto value = QString::fromStdString(do_fixedStringAt(0, len));
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline auto resetStreamCursorPosition(void) -> void {
         my_streamCursorPosition = 0;
      }

      inline auto streamReadByteArray(i32 len) {
         auto value = do_byteArrayAt(my_streamCursorPosition, len);
         my_streamCursorPosition += len;
         return value;
      }

      inline auto streamReadI64(void) -> i64 {
         auto value = do_i64At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i64);
         return value;
      }

      inline auto streamReadU64(void) -> u64 {
         auto value = do_u64At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u64);
         return value;
      }

      inline auto streamReadI32(void) -> i32 {
         auto value = do_i32At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i32);
         return value;
      }

      inline auto streamReadU32(void) -> u32 {
         auto value = do_u32At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u32);
         return value;
      }

      inline auto streamReadI16(void) -> i16 {
         auto value = do_i16At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i16);
         return value;
      }

      inline auto streamReadU16(void) -> u16 {
         auto value = do_u16At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u16);
         return value;
      }

      inline auto streamReadI8(void) -> i8 {
         auto value = do_i8At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i8);
         return value;
      }

      inline auto streamReadU8(void) -> u8 {
         auto value = do_u8At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u8);
         return value;
      }

      inline auto streamReadPascalString(void) -> std::string {
         auto value = do_pascalStringAt(my_streamCursorPosition);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamReadPascalQString(void) -> QString {
         auto value =
               QString::fromStdString(
                  do_pascalStringAt(my_streamCursorPosition));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamReadString(int maxlen) -> std::string {
         auto value = do_stringAt(my_streamCursorPosition, maxlen);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamReadQString(int maxlen) -> QString {
         auto value =
               QString::fromStdString(
                  do_stringAt(my_streamCursorPosition, maxlen));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamReadFixedString(int len) -> std::string {
         auto value = do_fixedStringAt(my_streamCursorPosition, len);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamReadFixedQString(int len) -> QString {
         auto value = QString::fromStdString(
                  do_fixedStringAt(my_streamCursorPosition, len));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline auto streamWriteI64(i64 value) -> void {
         do_setI64At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i64);
      }

      inline auto streamWriteU64(u64 value) -> void {
         do_setU64At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u64);
      }

      inline auto streamWriteI32(i32 value) -> void {
         do_setI32At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i32);
      }

      inline auto streamWriteU32(u32 value) -> void {
         do_setU32At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u32);
      }

      inline auto streamWriteI16(i16 value) -> void {
         do_setI16At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i16);
      }

      inline auto streamWriteU16(u16 value) -> void {
         do_setU16At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u16);
      }

      inline auto streamWriteI8(i8 value) -> void {
         do_setI8At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i8);
      }

      inline auto streamWriteU8(u8 value) -> void {
         do_setU8At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u8);
      }

      inline auto streamWritePascalString(const std::string& value) -> void {
         do_setPascalStringAt(my_streamCursorPosition, value);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamWritePascalQString(const QString& value) -> void {
         do_setPascalStringAt(my_streamCursorPosition, value.toStdString());
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamWriteString(const std::string& value)
            -> void {
         do_setStringAt(my_streamCursorPosition, value);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamWriteQString(const QString& value) -> void {
         do_setStringAt(my_streamCursorPosition, value.toStdString());
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamWriteFixedString(const std::string& value, int len)
            -> void {
         do_setFixedStringAt(my_streamCursorPosition, value, len);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamWriteFixedQString(const QString& value, int len)
            -> void {
         do_setFixedStringAt(my_streamCursorPosition, value.toStdString(), len);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline auto streamCursorPosition(void) -> int {
         return my_streamCursorPosition;
      }

      inline auto setStreamCursorPosition(int value) {
         my_streamCursorPosition = value;
      }

      inline auto clear(void) {
         do_clear();
      }

   private:
      int my_streamCursorPosition;
      bool my_swapEndianFlag;

      inline auto do_maybeSwapI64(i64 value) const -> i64 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         i64 swapped_value = SwapI64(value);
         auto result =
               static_cast<i64>(cond) * swapped_value |
               static_cast<i64>(!cond) * value;

         return result;
      }

      inline auto do_maybeSwapU64(u64 value) const -> u64 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         u64 swapped_value = SwapU64(value);
         auto result =
               static_cast<u64>(cond) * swapped_value |
               static_cast<u64>(!cond) * value;

         return result;
      }

      inline auto do_maybeSwapI32(i32 value) const -> i32 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapI32(value);
         auto result =
               static_cast<i32>(cond) * swapped_value |
               static_cast<i32>(!cond) * value;

         return result;
      }

      inline auto do_maybeSwapU32(u32 value) const -> u32 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapU32(value);
         auto result =
               static_cast<u32>(cond) * swapped_value |
               static_cast<u32>(!cond) * value;

         return result;
      }

      inline auto do_maybeSwapU16(u16 value) const -> u16 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapU16(value);
         auto result = static_cast<u16>(
                  static_cast<u16>(cond) * swapped_value |
                  static_cast<u16>(!cond) * value);

         return result;
      }

      inline auto do_maybeSwapI16(i16 value) const -> i16 {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapI16(value);
         auto result = static_cast<i16>(
                  static_cast<i16>(cond) * swapped_value |
                  static_cast<i16>(!cond) * value);

         return result;
      }

      auto do_setPascalStringAt(int offset, const std::string& value) ->
            void {
         auto len = static_cast<u8>(value.length());
         do_setU8At(offset, len);

         for (auto i = u8{0}; i < len; i++)
            setU8At(offset+i+1, static_cast<u8>(value.at(i)));
      }

      auto do_setFixedPascalStringAt(
            int offset, const std::string& value, int capacity) -> void
      {
         auto len = static_cast<u8>(value.length());
         do_setU8At(offset, len);

         for (auto i = u8{0}; i < capacity; i++) {
            if (i < len) {
               setU8At(offset+i+1, static_cast<u8>(value.at(i)));
            }
            else {
               setU8At(offset+i+1, u8{'\0'});
            }
         }
      }

      auto do_pascalStringAt(int offset) const -> std::string {
         std::string value;
         auto len = do_u8At(offset);
         for (auto i = u8{0}; i < len; i++)
            value.push_back(do_i8At(offset+i+1));
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

      auto do_stringAt(int offset, int maxlen) const -> std::string {
         std::string value;

         auto i = 0;
         while (this->at(offset+i) && i < maxlen) {
            value.push_back(this->at(offset+i));
            i++;
         }

         return value;
      }

      auto do_setStringAt(int offset, const std::string& value) -> void {
         auto len = static_cast<u32>(value.length());
         for (auto i = u32{0}; i < len; i++)
            setU8At(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      auto do_fixedStringAt(int offset, int len) const -> std::string {
         std::string value;

         for (auto i = 0; i < len; i++) {
            value.push_back(this->at(offset+i));
         }

         return value;
      }

      auto do_setFixedStringAt(
            int offset, const std::string& value, int len) -> void {
         for (auto i = 0; i < len; i++) {
            auto z = static_cast<int>(value.length());
            if (i < z) {
               setU8At(offset+i, static_cast<u8>(value.at(i)));
            }
            else {
               setU8At(offset+i, u8{'\0'});
            }
         }
      }

      auto do_pascalQStringAt(int offset) const -> QString {
         auto s = do_pascalStringAt(offset);
         return QString::fromStdString(s);
      }

//      auto do_set_pascal_qstring_at(int offset, const QString& value) -> void {
//         do_set_pascal_string_at(offset, value.toStdString());
//      }

//      auto do_setQStringU32At(int offset, const QString& value) -> void {
//         do_setStringU32At(offset, value.toStdString());
//      }

//      auto do_qStringU32At(int offset, u32 maxlen) const -> QString {
//         auto s = do_stringU32At(offset, maxlen);
//         return QString::fromStdString(s);
//      }

      auto do_stdStringFromQString(const QString& value)
         -> std::string {
         std::string result;

         for (auto i = 0; i < value.length(); i++) {
            auto c = value[i].cell();
            result += c;
         }

         return result;
      }

      auto do_setQStringAt(int offset, const QString& value) -> void {
         //auto stdString = do_stdStringFromQString(value);
         do_setStringAt(offset, value.toStdString());
      }

      auto do_qstringAt(int offset, int maxlen) const -> QString {
         auto s = do_stringAt(offset, maxlen);
         return QString::fromStdString(s);
      }

      auto do_i64At(int offset) const -> i64 {
         i64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<i64>(bi.qwordValue);

         return value;
      }

      auto do_setI64At(int offset, i64 value) -> void {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
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
         value = static_cast<u64>(bi.qwordValue);

         return value;
      }

      auto do_setU64At(int offset, u64 value) -> void {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
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
         value = static_cast<i32>(bi.dwordValue);

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
         bi.dwordValue = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
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
         value = static_cast<u32>(bi.dwordValue);

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
         bi.dwordValue = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
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
         value = static_cast<i16>(bi.wordValue);

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
         bi.wordValue = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
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
         value = static_cast<u16>(bi.wordValue);

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
         bi.wordValue = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(u16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      auto do_i8At(int offset) const -> i8 {
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

      auto do_u8At(int offset) const -> u8 {
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

      auto do_byteArrayAt(int offset, i32 len) -> ByteArray {
         auto b = ByteArray();
         for (auto i = 0; i < len; i++) {
            b.appendU8(static_cast<u8>(at(offset + i)));
         }
         return b;
      }

      auto do_appendI64(i64 value) -> void {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendU64(u64 value) -> void {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendI32(i32 value) -> void {
         bi32 bi;
         bi.dwordValue = static_cast<i32>(value);
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
         bi.dwordValue = static_cast<i32>(value);
         int z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      auto do_appendI16(i16 value) -> void {
         bi16 bi;
         bi.wordValue = static_cast<i16>(value);
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
         bi.wordValue = static_cast<i16>(value);
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

      auto do_clear(void) -> void {
         QByteArray::clear();
         my_streamCursorPosition = 0;
      }
   };
}

#endif   // SEVILLE_BYTE_ARRAY_H_
