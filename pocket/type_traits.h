#ifndef __POCKET_TYPE_TRAITS_H__
#define __POCKET_TYPE_TRAITS_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#ifdef POCKET_USE_CXX11
#include <type_traits>
#endif // POCKET_USE_CXX11
#include <cstddef>

#ifndef POCKET_TEMPLATE_TYPE_VALIDATE
#	ifdef POCKET_USE_CXX11
#		define POCKET_TEMPLATE_TYPE_VALIDATE(VALID_TYPE, T) typename = typename pocket::type_traits::enable_if< VALID_TYPE < T >::value, T >::type
#	else
#		define POCKET_TEMPLATE_TYPE_VALIDATE(VALID_TYPE, T) typename pocket::type_traits::enable_if< VALID_TYPE < T >::value, T >::type
#	endif
#endif // POCKET_TEMPLATE_TYPE_VALIDATE

/*---------------------------------------------------------------------------------------
* テンプレートの型チェックの設定（算術型か？）
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC
#	define POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(T) POCKET_TEMPLATE_TYPE_VALIDATE(pocket::type_traits::is_arithmetic, T)
#endif // POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC

/*---------------------------------------------------------------------------------------
* テンプレートの型チェックの設定（数学クラスで使用できるか？）
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE
#	define POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T) POCKET_TEMPLATE_TYPE_VALIDATE(pocket::type_traits::is_math_type, T)
#endif // POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE

namespace pocket
{
namespace type_traits
{
/*---------------------------------------------------------------------
* const修飾を外す
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct remove_const
{
	typedef T type;
};
template <typename T>
struct remove_const<const T>
{
	typedef T type;
};
#else

using std::remove_const;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* volatile修飾を外す
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct remove_volatile
{
	typedef T type;
};
template <typename T>
struct remove_volatile<volatile T>
{
	typedef T type;
};
#else

using std::remove_volatile;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* cv修飾どちらも外す
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct remove_cv
{
	typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};
#else

using std::remove_cv;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 参照修飾を外す
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct remove_reference
{
	typedef T type;
};
template <typename T>
struct remove_reference<T&>
{
	typedef T type;
};
#else

using std::remove_reference;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* const volatile&を外す
*---------------------------------------------------------------------*/
template <typename T>
struct remove_cv_reference
{
	typedef typename remove_cv<typename remove_reference<T>::type>::type type;
	//typedef typename remove_reference<typename remove_cv<T>::type>::type type;
};

/*---------------------------------------------------------------------
* ポインターを外す
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct remove_pointer
{
	typedef T type;
};
template <typename T>
struct remove_pointer<T*>
{
	typedef T type;
};
template <typename T>
struct remove_pointer<T* const>
{
	typedef T type;
};
template <typename T>
struct remove_pointer<T* volatile>
{
	typedef T type;
};
template <typename T>
struct remove_pointer<T* const volatile>
{
	typedef T type;
};
#else

using std::remove_pointer;
#endif // POCKET_USE_CXX11


/*---------------------------------------------------------------------
* 整数型定数
*---------------------------------------------------------------------*/
#if POCKET_VCXX_VER(14) || POCKET_CXX_VER(11)
using std::integral_constant;

#else
// VC++12には存在していない
template <typename T, T N>
struct integral_constant
{
	POCKET_STATICAL_CONSTANT T value = N;

	typedef T value_type;
	typedef integral_constant<T, N> type;
	typedef type self;

	POCKET_CXX11_CONSTEXPR operator T () const
	{
		return value;
	}
	POCKET_CXX11_CONSTEXPR T operator () () const
	{
		return value;
	}
};
template <typename T, T N>
const T integral_constant<T, N>::value;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 常に真・偽を返す型
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;
#else

using std::true_type;
using std::false_type;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 数学クラスで使用できる型か
*---------------------------------------------------------------------*/
template <typename T>
struct is_math_type : false_type
{};
template <typename T>
struct is_math_floating_type : false_type
{};

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
#ifndef POCKET_USE_CXX11
template <bool C, typename T>
struct enable_if
{};
template <typename T>
struct enable_if<true, T>
{
	typedef T type;
};
#else

using std::enable_if;
#endif // POCKET_USE_CXX11


/*---------------------------------------------------------------------
* 整数の型か
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct is_integral_base : false_type
{};
template <>
struct is_integral_base<bool> : true_type
{};
template <>
struct is_integral_base<char> : true_type
{};
template <>
struct is_integral_base<unsigned char> : true_type
{};
template <>
struct is_integral_base<short> : true_type
{};
template <>
struct is_integral_base<unsigned short> : true_type
{};
template <>
struct is_integral_base<int> : true_type
{};
template <>
struct is_integral_base<unsigned int> : true_type
{};
template <>
struct is_integral_base<long> : true_type
{};
template <>
struct is_integral_base<unsigned long> : true_type
{};
template <>
struct is_integral_base<long long> : true_type
{};
template <>
struct is_integral_base<unsigned long long> : true_type
{};

template <typename T>
struct is_integral : is_integral_base<typename remove_cv_reference<T>::type>
{};
#else

using std::is_integral;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 少数の型か
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct is_floating_point_base : false_type
{};
template <>
struct is_floating_point_base<float> : true_type
{};
template <>
struct is_floating_point_base<double> : true_type
{};
template <>
struct is_floating_point_base<long double> : true_type
{};
template <typename T>
struct is_floating_point : is_floating_point_base<typename remove_cv_reference<T>::type>
{};
#else

using std::is_floating_point;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 算術可能なクラス化
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template <typename T>
struct is_arithmetic : integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value>
{};

#else

using std::is_arithmetic;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 型が同じか
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
template<typename T, typename U>
struct is_same : false_type
{};
template<typename T>
struct is_same<T, T> : true_type
{};

#else

using std::is_same;
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 真の時にT1、偽の時にT2の型となる型
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
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
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------
* 真の時にN1、偽の時にN2のサイズとなる型
*---------------------------------------------------------------------*/
template <bool C, size_t N1, size_t N2>
struct conditional_size
{
	POCKET_STATICAL_CONSTANT size_t value = N1;
};
template <size_t N1, size_t N2>
struct conditional_size<false, N1, N2>
{
	POCKET_STATICAL_CONSTANT size_t value = N2;
};

/*---------------------------------------------------------------------
* 配列の数を取得
*---------------------------------------------------------------------*/
template <typename T>
struct extent_count_base : integral_constant<int, 0>
{};
template <typename T, int N>
struct extent_count_base<T[N]> : integral_constant<int, N>
{};

template <typename T>
struct extent_count : extent_count_base<typename remove_cv_reference<T>::type>
{};

} // namespace type_traits
} // namespace pocket

#endif // __POCKET_TYPE_TRAITS_H__
