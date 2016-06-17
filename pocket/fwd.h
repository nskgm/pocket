#ifndef __POCKET_FORWARD_H__
#define __POCKET_FORWARD_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include <stdint.h>
#ifdef _USE_CXX11
#include <type_traits>
#endif // _USE_CXX11

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

#ifndef _TEMPLATE_TYPE_VALIDATE
#	ifdef _USE_CXX11
#		define _TEMPLATE_TYPE_VALIDATE(VALID_TYPE, T) typename = typename pocket::type_traits::enable_if< VALID_TYPE < T >::value, T >::type
#	else
#		define _TEMPLATE_TYPE_VALIDATE(VALID_TYPE, T) typename pocket::type_traits::enable_if< VALID_TYPE < T >::value, T >::type
#	endif
#endif // _TEMPLATE_TYPE_VALIDATE

/*---------------------------------------------------------------------------------------
* テンプレートの型チェックの設定（算術型か？）
*---------------------------------------------------------------------------------------*/
#ifndef _TEMPLATE_TYPE_VALIDATE_ARITHMETIC
#	define _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(T) _TEMPLATE_TYPE_VALIDATE(pocket::type_traits::is_arithmetic, T)
#endif // _TEMPLATE_TYPE_VALIDATE_ARITHMETIC

/*---------------------------------------------------------------------------------------
* テンプレートの型チェックの設定（数学クラスで使用できるか？）
*---------------------------------------------------------------------------------------*/
#ifndef _TEMPLATE_TYPE_VALIDATE_MATH_TYPE
#	define _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T) _TEMPLATE_TYPE_VALIDATE(pocket::type_traits::is_math_type, T)
#endif // _TEMPLATE_TYPE_VALIDATE_MATH_TYPE

namespace pocket
{
namespace type_traits
{

/*---------------------------------------------------------------------
* 整数型定数
*---------------------------------------------------------------------*/
template <typename T, T N>
struct integral_constant
{
	_STATICAL_CONSTANT T value = N;

	typedef T value_type;
	typedef integral_constant<T, N> type;
	typedef type self;

	_CXX11_CONSTEXPR operator T () const
	{
		return value;
	}
	_CXX11_CONSTEXPR T operator () () const
	{
		return value;
	}
};

/*---------------------------------------------------------------------
* 常に真・偽を返す型
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
struct true_type : integral_constant<bool, true>
{};
struct false_type : integral_constant<bool, false>
{};
#else

using std::true_type;
using std::false_type;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 数学クラスで使用できる型か
*---------------------------------------------------------------------*/
template <typename T>
struct is_math_type : false_type
{};
template <typename T>
struct is_math_floating_type : false_type
{};
#if 0
template <typename T>
struct is_math_type<std::complex<T> > : is_math_type<T>
{};
template <typename T>
struct is_math_floating_type<std::complex<T> > : is_math_floating_type<T>
{};
#endif

#ifndef _UNUSING_MATH_INT_FLOAT
template <>
struct is_math_type<int> : true_type
{};
template <>
struct is_math_type<float> : true_type
{};
template <>
struct is_math_floating_type<float> : true_type
{};
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
template <>
struct is_math_type<double> : true_type
{};
template <>
struct is_math_floating_type<double> : true_type
{};
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
template <>
struct is_math_type<long double> : true_type
{};
template <>
struct is_math_floating_type<long double> : true_type
{};
#endif // _USING_MATH_LONG_DOUBLE

/*---------------------------------------------------------------------
* bool Cが真の時のみ型を定義、コンパイル時エラーチェック
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <bool C, typename T>
struct enable_if
{

};
template <typename T>
struct enable_if<true, T>
{
	typedef T type;
};
#else

using std::enable_if;
#endif // _USE_CXX11


/*---------------------------------------------------------------------
* 算術可能なクラス化
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <typename T>
struct is_arithmetic : false_type
{};

template <>
struct is_arithmetic<bool> : true_type
{};
template <>
struct is_arithmetic<char> : true_type
{};
template <>
struct is_arithmetic<unsigned char> : true_type
{};
template <>
struct is_arithmetic<short> : true_type
{};
template <>
struct is_arithmetic<unsigned short> : true_type
{};
template <>
struct is_arithmetic<int> : true_type
{};
template <>
struct is_arithmetic<unsigned int> : true_type
{};
template <>
struct is_arithmetic<long> : true_type
{};
template <>
struct is_arithmetic<unsigned long> : true_type
{};
template <>
struct is_arithmetic<long long> : true_type
{};
template <>
struct is_arithmetic<unsigned long long> : true_type
{};
template <>
struct is_arithmetic<float> : true_type
{};
template <>
struct is_arithmetic<double> : true_type
{};
template <>
struct is_arithmetic<long double> : true_type
{};
#else

using std::is_arithmetic;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 真の時にT1、偽の時にT2の型となる型
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <bool C, typename T1, typename T2>
struct conditional
{
	typedef T1 type;
};
template <typename T1, typename T2>
struct conditional<false, T1, T2>
{
	typedef T2 type;
};
#else

using std::conditional;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 真の時にN1、偽の時にN2のサイズとなる型
*---------------------------------------------------------------------*/
template <bool C, size_t N1, size_t N2>
struct conditional_size
{
	_STATICAL_CONSTANT size_t value = N1;
};
template <size_t N1, size_t N2>
struct conditional_size<false, N1, N2>
{
	_STATICAL_CONSTANT size_t value = N2;
};

} // namespace type_traits
} // namespace pocket

#endif // __POCKET_FORWARD_H__
