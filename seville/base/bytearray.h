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
      // ByteArray(QByteArray& ref);

      // static std::unique_ptr<ByteArray> New(void);
      // std::optional<ByteArray*>;

      static inline i64 SwapI64(i64 value) {
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

      static inline u64 SwapU64(u64 value) {
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

      static inline i32 SwapI32(i32 value) {
         auto result =
               (((value >> 24) & 0x000000ff) |
                ((value <<  8) & 0x00ff0000) |
                ((value >>  8) & 0x0000ff00) |
                (static_cast<u32>(value << 24) & 0xff000000));

         return static_cast<i32>(result);
      }

      static inline u32 SwapU32(u32 unswapped) {
         return (((unswapped >> 24) & 0x000000ff) |
                 ((unswapped <<  8) & 0x00ff0000) |
                 ((unswapped >>  8) & 0x0000ff00) |
                 ((unswapped << 24) & 0xff000000));
      }

      static inline i16 SwapI16(i16 value) {
         return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
      }

      static u16 SwapU16(u16 value) {
         return (((value >> 8) & 0xff) | ((value << 8) & 0xff));
      }

      inline bool shouldSwapEndianness(void) const {
         return my_swapEndianFlag;
      }

      inline void setShouldSwapEndianness(bool value) {
         my_swapEndianFlag = value;
      }

      inline std::string pascalStringAt(i32 offset) const {
         return do_pascalStringAt(offset);
      }

      inline void setPascalStringAt(i32 offset, std::string value) {
         do_setPascalStringAt(offset, value);
      }

//      inline auto stringU32At(int offset, u32 maxlen) const -> std::string {
//         return do_stringU32At_(offset, maxlen);
//      }

//      inline auto setStringU32At(int offset, std::string value) -> void {
//         do_setStringU32At_(offset, value);
//      }

      inline std::string stringAt(i32 offset, i32 len) const {
         return do_stringAt(offset, len);
      }

      inline void setStringAt(i32 offset, const std::string& value) {
         do_setStringAt(offset, value);
      }

      inline std::string fixedStringAt(i32 offset, i32 len) const {
         return do_fixedStringAt(offset, len);
      }

      inline void setFixedStringAt(
            int offset, const std::string& value, i32 len) {
         do_setFixedStringAt(offset, value, len);
      }

      inline QString pascalQStringAt(i32 offset) const {
         return do_pascalQStringAt(offset);
      }

      inline void setPascalQStringAt(i32 offset, const QString& value) {
         do_setPascalStringAt(offset, value.toStdString());
      }

//      inline auto qStringU32At(int offset, u32 maxlen) const -> QString {
//         return do_qstring_u32_at_(offset, maxlen);
//      }

//      inline auto setQStringU32At(int offset, const QString& value) -> void {
//         do_set_qstring_u32_at_(offset, value);
//      }

      inline QString qstringAt(int offset, i32 maxlen) const {
         return do_qstringAt(offset, maxlen);
      }

      inline void setQStringAt(int offset, const QString& value) {
         do_setQStringAt(offset, value);
      }

      inline void appendFixedQString(const QString& value, i32 len) {
         do_setFixedStringAt(size(), value.toStdString(), len);
      }

      inline void appendFixedString(const std::string& value, i32 len) {
         do_setFixedStringAt(size(), value, len);
      }

      inline void appendQString(const QString& value) {
         do_setQStringAt(size(), value);
         //append(value.toStdString().c_str());
      }

      inline void appendString(const std::string& value) {
         do_setStringAt(size(), value);
         //append(value.c_str());
      }

      inline void appendPascalQString(const QString& value) {
         auto offset = size();
         do_setPascalStringAt(offset, value.toStdString());
      }

      inline void appendPascalString(const std::string& value) {
         auto offset = size();
         do_setPascalStringAt(offset, value);
      }

      inline void appendFixedPascalQString(
            const QString& value, i32 len) {
         auto offset = size();
         do_setFixedPascalStringAt(offset, value.toStdString(), len);
      }

      inline void appendFixedPascalString(const std::string& value, int len) {
         auto offset = size();
         do_setFixedPascalStringAt(offset, value, len);
      }

      inline i64 i64At(i32 offset) const {
         return do_i64At(offset);
      }

      inline void setI64At(i32 offset, i64 value) {
         do_setI64At(offset, value);
      }

      inline u64 u64At(i32 offset) const {
         return do_u64At(offset);
      }

      inline void setU64At(i32 offset, u64 value) {
         do_setU64At(offset, value);
      }

      inline i32 i32At(i32 offset) const {
         return do_i32At(offset);
      }

      inline void setI32At(i32 offset, i32 value) {
         do_setI32At(offset, value);
      }

      inline u32 u32At(i32 offset) const {
         return do_u32At(offset);
      }

      inline void setU32At(i32 offset, u32 value) {
         do_setU32At(offset, value);
      }

      inline i16 i16At(i32 offset) const {
         return do_i16At(offset);
      }

      inline void setI16At(i32 offset, i16 value) {
         do_setI16At(offset, value);
      }

      inline u16 u16At(i32 offset) const {
         return do_u16At(offset);
      }

      inline void setU16At(i32 offset, u16 value) {
         do_setU16At(offset, value);
      }

      inline i8 i8At(i32 offset) const {
         return do_i8At(offset);
      }

      inline void setI8At(i32 offset, i8 value) {
         do_setI8At(offset, value);
      }

      inline u8 u8At(i32 offset) const {
         return do_u8At(offset);
      }

      inline void setU8At(i32 offset, u8 value) {
         do_setU8At(offset, value);
      }

      inline void appendI64(i64 value) {
         do_appendI64(value);
      }

      inline void appendU64(u64 value) {
         do_appendU64(value);
      }

      inline void appendI32(i32 value) {
         do_appendI32(value);
      }

      inline void appendU32(u32 value) {
         do_appendU32(value);
      }

      inline void appendI16(i16 value) {
         do_appendI16(value);
      }

      inline void appendU16(u16 value) {
         do_appendU16(value);
      }

      inline void appendI8(i8 value) {
         do_appendI8(value);
      }

      inline void appendU8(u8 value) {
         do_appendU8(static_cast<unsigned char>(value));
      }

      inline i64 takeI64(void) {
         auto value = do_i64At(0);
         this->remove(0, sizeof(i64));
         return value;
      }

      inline u64 takeU64(void) {
         auto value = do_u64At(0);
         this->remove(0, sizeof(u64));
         return value;
      }

      inline i32 takeI32(void) {
         auto value = do_i32At(0);
         this->remove(0, sizeof(i32));
         return value;
      }

      inline u32 takeU32(void) {
         auto value = do_u32At(0);
         this->remove(0, sizeof(u32));
         return value;
      }

      inline i16 takeI16(void) {
         auto value = do_i16At(0);
         this->remove(0, sizeof(i16));
         return value;
      }

      inline u16 takeU16(void) {
         auto value = do_u16At(0);
         this->remove(0, sizeof(u16));
         return value;
      }

      inline i8 takeI8(void) {
         auto value = do_i8At(0);
         this->remove(0, sizeof(i8));
         return value;
      }

      inline u8 takeU8(void) {
         auto value = do_u8At(0);
         this->remove(0, sizeof(u8));
         return value;
      }

      inline std::string takePascalString(void) {
         auto value = do_pascalStringAt(0);
         auto z = static_cast<i32>(value.size());
         this->remove(0, z); // TODO stride
         return value;
      }

      inline QString takePascalQString(void) {
         auto value = QString::fromStdString(do_pascalStringAt(0));
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline std::string takeString(i32 maxlen) {
         auto value = do_stringAt(0, maxlen);
         auto z = static_cast<i32>(value.size());
         this->remove(0, z); // TODO stride
         return value;
      }

      inline QString takeQString(i32 maxlen) {
         auto value = QString::fromStdString(do_stringAt(0, maxlen));
         auto z = static_cast<i32>(value.size());
         this->remove(0, z); // TODO stride
         return value;
      }

      inline std::string takeFixedString(i32 len) {
         auto value = do_fixedStringAt(0, len);
         auto z = static_cast<i32>(value.size());
         this->remove(0, z); // TODO stride
         return value;
      }

      inline QString takeFixedQString(i32 len) {
         auto value = QString::fromStdString(do_fixedStringAt(0, len));
         this->remove(0, value.size()); // TODO stride
         return value;
      }

      inline void resetStreamCursorPosition(void) {
         my_streamCursorPosition = 0;
      }

      inline void streamSkip(int len) {
         my_streamCursorPosition += len;
      }

      inline QByteArray streamReadByteArray(i32 len) {
         auto value = do_byteArrayAt(my_streamCursorPosition, len);
         my_streamCursorPosition += len;
         return std::move(value);
      }

      inline i64 streamReadI64(void) {
         auto value = do_i64At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i64);
         return value;
      }

      inline u64 streamReadU64(void) {
         auto value = do_u64At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u64);
         return value;
      }

      inline i32 streamReadI32(void) {
         auto value = do_i32At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i32);
         return value;
      }

      inline u32 streamReadU32(void) {
         auto value = do_u32At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u32);
         return value;
      }

      inline i16 streamReadI16(void) {
         auto value = do_i16At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i16);
         return value;
      }

      inline u16 streamReadU16(void) {
         auto value = do_u16At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u16);
         return value;
      }

      inline i8 streamReadI8(void) {
         auto value = do_i8At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(i8);
         return value;
      }

      inline u8 streamReadU8(void) {
         auto value = do_u8At(my_streamCursorPosition);
         my_streamCursorPosition += sizeof(u8);
         return value;
      }

      inline std::string streamReadPascalString(void) {
         auto value = do_pascalStringAt(my_streamCursorPosition);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline QString streamReadPascalQString(void) {
         auto value =
               QString::fromStdString(
                  do_pascalStringAt(my_streamCursorPosition));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline std::string streamReadString(i32 maxlen) {
         auto value = do_stringAt(my_streamCursorPosition, maxlen);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline QString streamReadQString(i32 maxlen) {
         auto value =
               QString::fromStdString(
                  do_stringAt(my_streamCursorPosition, maxlen));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline std::string streamReadFixedString(i32 len) {
         auto value = do_fixedStringAt(my_streamCursorPosition, len);
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline QString streamReadFixedQString(i32 len) {
         auto value = QString::fromStdString(
                  do_fixedStringAt(my_streamCursorPosition, len));
         my_streamCursorPosition += value.size(); // TODO stride
         return value;
      }

      inline QString streamReadAndDecodeQString(i32 len) {
         auto byteArray = do_byteArrayAt(my_streamCursorPosition, len);
         auto value = QString::fromLatin1(byteArray);
         if (value.toLatin1() != byteArray) {
           value = QString::fromUtf8(byteArray);
         }
         my_streamCursorPosition += len;
         return value;
      }

//      inline auto streamReadUtf16QString(int len) -> QString {
//      }

      inline void streamWriteI64(i64 value) {
         do_setI64At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i64);
      }

      inline void streamWriteU64(u64 value) {
         do_setU64At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u64);
      }

      inline void streamWriteI32(i32 value) {
         do_setI32At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i32);
      }

      inline void streamWriteU32(u32 value) {
         do_setU32At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u32);
      }

      inline void streamWriteI16(i16 value) {
         do_setI16At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i16);
      }

      inline void streamWriteU16(u16 value) {
         do_setU16At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u16);
      }

      inline void streamWriteI8(i8 value) {
         do_setI8At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(i8);
      }

      inline void streamWriteU8(u8 value) {
         do_setU8At(my_streamCursorPosition, value);
         my_streamCursorPosition += sizeof(u8);
      }

      inline void streamWritePascalString(const std::string& value) {
         do_setPascalStringAt(my_streamCursorPosition, value);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline void streamWritePascalQString(const QString& value) {
         do_setPascalStringAt(my_streamCursorPosition, value.toStdString());
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline void streamWriteString(const std::string& value) {
         do_setStringAt(my_streamCursorPosition, value);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline void streamWriteQString(const QString& value) {
         do_setStringAt(my_streamCursorPosition, value.toStdString());
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline void streamWriteFixedString(const std::string& value, int len) {
         do_setFixedStringAt(my_streamCursorPosition, value, len);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline void streamWriteFixedQString(const QString& value, int len) {
         do_setFixedStringAt(my_streamCursorPosition, value.toStdString(), len);
         my_streamCursorPosition += value.size(); // TODO stride
      }

      inline int streamCursorPosition(void) {
         return my_streamCursorPosition;
      }

      inline void setStreamCursorPosition(int value) {
         my_streamCursorPosition = value;
      }

      inline void clear(void) {
         do_clear();
      }

   private:
      int my_streamCursorPosition;
      bool my_swapEndianFlag;

      inline i64 do_maybeSwapI64(i64 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         i64 swapped_value = SwapI64(value);
         auto result =
               static_cast<i64>(cond) * swapped_value |
               static_cast<i64>(!cond) * value;

         return result;
      }

      inline u64 do_maybeSwapU64(u64 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         u64 swapped_value = SwapU64(value);
         auto result =
               static_cast<u64>(cond) * swapped_value |
               static_cast<u64>(!cond) * value;

         return result;
      }

      inline i32 do_maybeSwapI32(i32 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapI32(value);
         auto result =
               static_cast<i32>(cond) * swapped_value |
               static_cast<i32>(!cond) * value;

         return result;
      }

      inline u32 do_maybeSwapU32(u32 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapU32(value);
         auto result =
               static_cast<u32>(cond) * swapped_value |
               static_cast<u32>(!cond) * value;

         return result;
      }

      inline u16 do_maybeSwapU16(u16 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapU16(value);
         auto result = static_cast<u16>(
                  static_cast<u16>(cond) * swapped_value |
                  static_cast<u16>(!cond) * value);

         return result;
      }

      inline i16 do_maybeSwapI16(i16 value) const {
         //auto cond = doDetermineShouldSwapEndianness();
         auto cond = my_swapEndianFlag;
         auto swapped_value = SwapI16(value);
         auto result = static_cast<i16>(
                  static_cast<i16>(cond) * swapped_value |
                  static_cast<i16>(!cond) * value);

         return result;
      }

      inline void do_setPascalStringAt(i32 offset, const std::string& value) {
         auto len = static_cast<u8>(value.length());
         do_setU8At(offset, len);

         for (auto i = u8{0}; i < len; i++)
            setU8At(offset+i+1, static_cast<u8>(value.at(i)));
      }

      inline void do_setFixedPascalStringAt(
            i32 offset, const std::string& value, i32 capacity) {
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

      inline std::string do_pascalStringAt(i32 offset) const {
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

      inline std::string do_stringAt(i32 offset, i32 maxlen) const {
         std::string value;

         auto i = 0;
         while (this->at(offset+i) && i < maxlen) {
            value.push_back(this->at(offset+i));
            i++;
         }

         return value;
      }

      inline void do_setStringAt(i32 offset, const std::string& value) {
         auto len = static_cast<u32>(value.length());
         for (auto i = u32{0}; i < len; i++)
            setU8At(offset+static_cast<i32>(i), static_cast<u8>(value.at(i)));
      }

      inline std::string do_fixedStringAt(i32 offset, i32 len) const {
         std::string value;

         for (auto i = 0; i < len; i++) {
            value.push_back(this->at(offset+i));
         }

         return value;
      }

      inline void do_setFixedStringAt(
            int offset, const std::string& value, i32 len) {
         for (auto i = i32{0}; i < len; i++) {
            auto z = static_cast<int>(value.length());
            if (i < z) {
               auto idx = static_cast<std::size_t>(i);
               auto c = value.at(idx);
               setU8At(offset+i, static_cast<u8>(c));
            }
            else {
               setU8At(offset+i, u8{'\0'});
            }
         }
      }

      inline QString do_pascalQStringAt(i32 offset) const {
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

      inline std::string do_stdStringFromQString(const QString& value) {
         std::string result;

         for (auto i = 0; i < value.length(); i++) {
            auto c = value[i].cell();
            auto ch = static_cast<char>(c);
            result += ch;
         }

         return result;
      }

      inline void do_setQStringAt(i32 offset, const QString& value) {
         //auto stdString = do_stdStringFromQString(value);
         do_setStringAt(offset, value.toStdString());
      }

      inline QString do_qstringAt(i32 offset, i32 maxlen) const {
         auto s = do_stringAt(offset, maxlen);
         return QString::fromStdString(s);
      }

      inline i64 do_i64At(i32 offset) const {
         i64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<i64>(bi.qwordValue);

         return value;
      }

      inline void do_setI64At(i32 offset, i64 value) {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i),
                   static_cast<char>(bi.ch[i]));
         }
      }

      inline u64 do_u64At(i32 offset) const {
         u64 value;
         bi64 bi;

         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            bi.ch[i] = static_cast<unsigned char>(at(offset+i));
         }
         value = static_cast<u64>(bi.qwordValue);

         return value;
      }

      inline void do_setU64At(i32 offset, u64 value) {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         auto z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      inline i32 do_i32At(i32 offset) const {
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

      inline void do_setI32At(i32 offset, i32 value) {
         bi32 bi;
         bi.dwordValue = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      inline u32 do_u32At(i32 offset) const {
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

      inline void do_setU32At(i32 offset, u32 value) {
         bi32 bi;
         bi.dwordValue = static_cast<i32>(value);
         auto z = static_cast<int>(sizeof(u32));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      inline i16 do_i16At(i32 offset) const {
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

      inline void do_setI16At(i32 offset, i16 value) {
         bi16 bi;
          //doCsboW(static_cast<u16>(value));
         bi.wordValue = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      inline u16 do_u16At(i32 offset) const {
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

      inline void do_setU16At(i32 offset, u16 value) {
         bi16 bi;
         bi.wordValue = static_cast<i16>(value);
         auto z = static_cast<int>(sizeof(u16));
         for (auto i = 0; i < z; i++) {
            insert(static_cast<int>(offset+i), static_cast<char>(bi.ch[i]));
         }
      }

      inline i8 do_i8At(int offset) const {
         i8 value = at(offset);
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      inline void do_setI8At(i32 offset, i8 value) {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      inline u8 do_u8At(int offset) const {
         u8 value = static_cast<u8>(at(static_cast<int>(offset)));
         /*
         QDataStream ds(this, QIODevice::ReadOnly);
         ds.skipRawData(offset);
         ds >> value;
         */
         return value;
      }

      inline void do_setU8At(i32 offset, u8 value) {
         insert(static_cast<int>(offset), static_cast<char>(value));
      }

      inline ByteArray do_byteArrayAt(int offset, i32 len) {
         auto b = ByteArray();
         for (auto i = 0; i < len; i++) {
            b.appendU8(static_cast<u8>(at(offset + i)));
         }
         return b;
      }

      inline void do_appendI64(i64 value) {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(i64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      inline void do_appendU64(u64 value) {
         bi64 bi;
         bi.qwordValue = static_cast<i64>(value);
         int z = static_cast<int>(sizeof(u64));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      inline void do_appendI32(i32 value) {
         bi32 bi;
         bi.dwordValue = static_cast<i32>(value);
         int z = static_cast<int>(sizeof(i32));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      inline void do_appendU32(u32 value) {
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

      inline void do_appendI16(i16 value) {
         bi16 bi;
         bi.wordValue = static_cast<i16>(value);
         int z = static_cast<int>(sizeof(i16));
         for (auto i = 0; i < z; i++) {
            append(static_cast<char>(bi.ch[i]));
         }
      }

      inline void do_appendU16(u16 value) {
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

      inline void do_appendI8(i8 value) {
         append(value);
      }

      inline void do_appendU8(u8 value) {
         append(static_cast<i8>(value));
      }

      inline void do_clear(void) {
         QByteArray::clear();
         my_streamCursorPosition = 0;
      }
   };
}

#endif   // SEVILLE_BYTE_ARRAY_H_
