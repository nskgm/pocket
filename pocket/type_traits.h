#ifndef __POCKET_TYPE_TRAITS_H__
#define __POCKET_TYPE_TRAITS_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#ifdef _USE_CXX11
#include <type_traits>
#else
#include <cstddef>
#endif // _USE_CXX11

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
* const修飾を外す
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
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
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* volatile修飾を外す
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
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
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* cv修飾どちらも外す
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <typename T>
struct remove_cv
{
	typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};
#else

using std::remove_cv;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 参照修飾を外す
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
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
#endif // _USE_CXX11

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
#ifndef _USE_CXX11
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
#endif // _USE_CXX11


/*---------------------------------------------------------------------
* 整数型定数
*---------------------------------------------------------------------*/
#if _VCXX_VER(14) || _CXX_VER(11)
using std::integral_constant;

#else
// VC++12には存在していない
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
template <typename T, T N>
const T integral_constant<T, N>::value;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 常に真・偽を返す型
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
struct true_type : integral_constant<bool, true>
{

};
struct false_type : integral_constant<bool, false>
{

};
#else

using std::true_type;
using std::false_type;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 数学クラスで使用できる型か
*---------------------------------------------------------------------*/
template <typename T>
struct is_math_type : false_type
{

};
template <typename T>
struct is_math_floating_type : false_type
{

};

#ifndef _UNUSING_MATH_INT_FLOAT
template <>
struct is_math_type<int> : true_type
{

};
template <>
struct is_math_type<float> : true_type
{

};
template <>
struct is_math_floating_type<float> : true_type
{

};
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
template <>
struct is_math_type<double> : true_type
{

};
template <>
struct is_math_floating_type<double> : true_type
{

};
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
template <>
struct is_math_type<long double> : true_type
{

};
template <>
struct is_math_floating_type<long double> : true_type
{

};
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
* 整数の型か
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <typename T>
struct is_integral_base : false_type
{

};
template <>
struct is_integral_base<bool> : true_type
{

};
template <>
struct is_integral_base<char> : true_type
{

};
template <>
struct is_integral_base<unsigned char> : true_type
{

};
template <>
struct is_integral_base<short> : true_type
{

};
template <>
struct is_integral_base<unsigned short> : true_type
{

};
template <>
struct is_integral_base<int> : true_type
{

};
template <>
struct is_integral_base<unsigned int> : true_type
{

};
template <>
struct is_integral_base<long> : true_type
{

};
template <>
struct is_integral_base<unsigned long> : true_type
{

};
template <>
struct is_integral_base<long long> : true_type
{

};
template <>
struct is_integral_base<unsigned long long> : true_type
{

};

template <typename T>
struct is_integral : is_integral_base<typename remove_cv_reference<T>::type>
{

};
#else

using std::is_integral;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 少数の型か
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <typename T>
struct is_floating_point_base : false_type
{

};
template <>
struct is_floating_point_base<float> : true_type
{

};
template <>
struct is_floating_point_base<double> : true_type
{

};
template <>
struct is_floating_point_base<long double> : true_type
{

};
template <typename T>
struct is_floating_point : is_floating_point_base<typename remove_cv_reference<T>::type>
{

};
#else

using std::is_floating_point;
#endif // _USE_CXX11

/*---------------------------------------------------------------------
* 算術可能なクラス化
*---------------------------------------------------------------------*/
#ifndef _USE_CXX11
template <typename T>
struct is_arithmetic : integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value>
{

};

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

#endif // __POCKET_TYPE_TRAITS_H__
