#ifndef COMMON_H
#define COMMON_H

// Yes, this depends on Qt but doesn't live in QApp namespace
// The frame dependency here is semi-flexible and the typedefs
// are intended to be global.

#include <QtCore>

typedef qint8 i8;
typedef quint8 u8;
typedef qint16 i16;
typedef quint16 u16;
typedef qint32 i32;
typedef quint32 u32;
typedef qint64 i64;
typedef quint64 u64;
typedef float f32;
typedef double f64;

union bi64
{
   i64 qword;
   unsigned char ch[sizeof(i64)];
};

union bi32
{
   i32 dword;
   unsigned char ch[sizeof(i32)];
};

union bi16
{
   i16 word;
   unsigned char ch[sizeof(i16)];
};

#endif // COMMON_H
