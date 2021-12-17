#ifndef SEVILLE_TYPES_H_
#define SEVILLE_TYPES_H_

#include <QtCore>

namespace seville
{
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

    union bi64 {
        i64 qwordValue;
        unsigned char ch[sizeof(i64)];
    };

    union bi32 {
        i32 dwordValue;
        unsigned char ch[sizeof(i32)];
    };

    union bi16 {
        i16 wordValue;
        unsigned char ch[sizeof(i16)];
    };
}

#endif  // SEVILLE_TYPES_H_
