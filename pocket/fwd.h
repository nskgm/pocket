#ifndef __POCKET_FORWARD_H__
#define __POCKET_FORWARD_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "type_traits.h"
#include <stdint.h>

namespace pocket
{
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef u32 bitfield;
typedef u8 byte;
typedef s8 sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef u16 f16;
typedef float f32;
typedef double f64;
typedef long double f128;
}

#endif // __POCKET_FORWARD_H__
