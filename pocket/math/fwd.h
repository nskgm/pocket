#ifndef __MATH_FORWARD_H__
#define __MATH_FORWARD_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include <complex>
#include <stdint.h>
#ifdef _USE_CXX11
#include <type_traits>
#endif // _USE_CXX11

// 簡易定義
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef u32 bitfield; // ビット演算で行わえることを明示
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

#ifndef _DECL_CHANGE_BEHAVIOR
// pragma once が使用できる場合はstruct
#	ifdef _USE_PRAGMA_ONCE
#		define _DECL_CHANGE_BEHAVIOR(NAME) struct _##NAME##_t {}; static const _##NAME##_t NAME = {}
// 以外はenumで型判定
#	else
#		define _DECL_CHANGE_BEHAVIOR(NAME) enum _##NAME##_t { NAME }
#	endif
#endif // _DECL_CHANGE_BEHAVIOR

namespace pocket
{

namespace behavior
{
_DECL_CHANGE_BEHAVIOR(noinitialize);
_DECL_CHANGE_BEHAVIOR(zero);
_DECL_CHANGE_BEHAVIOR(one);
_DECL_CHANGE_BEHAVIOR(half);
_DECL_CHANGE_BEHAVIOR(half_of_half);
_DECL_CHANGE_BEHAVIOR(identity);

_DECL_CHANGE_BEHAVIOR(plus);
_DECL_CHANGE_BEHAVIOR(negate);
_DECL_CHANGE_BEHAVIOR(add);
_DECL_CHANGE_BEHAVIOR(add_assign);
_DECL_CHANGE_BEHAVIOR(sub);
_DECL_CHANGE_BEHAVIOR(sub_assign);
_DECL_CHANGE_BEHAVIOR(mul);
_DECL_CHANGE_BEHAVIOR(mul_assign);
_DECL_CHANGE_BEHAVIOR(div);
_DECL_CHANGE_BEHAVIOR(div_assign);
_DECL_CHANGE_BEHAVIOR(rem);
_DECL_CHANGE_BEHAVIOR(rem_assign);

_DECL_CHANGE_BEHAVIOR(increment);
_DECL_CHANGE_BEHAVIOR(increment_back);
_DECL_CHANGE_BEHAVIOR(decrement);
_DECL_CHANGE_BEHAVIOR(decrement_back);

_DECL_CHANGE_BEHAVIOR(at);
_DECL_CHANGE_BEHAVIOR(pointer);

_DECL_CHANGE_BEHAVIOR(x);
_DECL_CHANGE_BEHAVIOR(y);
_DECL_CHANGE_BEHAVIOR(z);

_DECL_CHANGE_BEHAVIOR(roll);
_DECL_CHANGE_BEHAVIOR(yaw);
_DECL_CHANGE_BEHAVIOR(pitch);
_DECL_CHANGE_BEHAVIOR(roll_pitch_yaw);

#ifdef _USE_CXX11
_DECL_CHANGE_BEHAVIOR(sqr);
_DECL_CHANGE_BEHAVIOR(radians);
_DECL_CHANGE_BEHAVIOR(degrees);
_DECL_CHANGE_BEHAVIOR(sin);
_DECL_CHANGE_BEHAVIOR(cos);
_DECL_CHANGE_BEHAVIOR(tan);
_DECL_CHANGE_BEHAVIOR(asin);
_DECL_CHANGE_BEHAVIOR(acos);
_DECL_CHANGE_BEHAVIOR(atan);
_DECL_CHANGE_BEHAVIOR(sin_cos);

_DECL_CHANGE_BEHAVIOR(round);
_DECL_CHANGE_BEHAVIOR(abs);
_DECL_CHANGE_BEHAVIOR(ceil);
_DECL_CHANGE_BEHAVIOR(floor);
_DECL_CHANGE_BEHAVIOR(sqrt);
_DECL_CHANGE_BEHAVIOR(pot);
_DECL_CHANGE_BEHAVIOR(is_pot);
_DECL_CHANGE_BEHAVIOR(clamp01);

_DECL_CHANGE_BEHAVIOR(vec2);
_DECL_CHANGE_BEHAVIOR(vec3);
_DECL_CHANGE_BEHAVIOR(vec4);
#endif // _USE_CXX11

_DECL_CHANGE_BEHAVIOR(equal);
_DECL_CHANGE_BEHAVIOR(not_equal);
_DECL_CHANGE_BEHAVIOR(near);
_DECL_CHANGE_BEHAVIOR(near_zero);
_DECL_CHANGE_BEHAVIOR(length);
_DECL_CHANGE_BEHAVIOR(length_square);
_DECL_CHANGE_BEHAVIOR(dot);
_DECL_CHANGE_BEHAVIOR(normalize);
_DECL_CHANGE_BEHAVIOR(normalized);
_DECL_CHANGE_BEHAVIOR(cross);
_DECL_CHANGE_BEHAVIOR(lerp);
_DECL_CHANGE_BEHAVIOR(distance);
_DECL_CHANGE_BEHAVIOR(direction);
_DECL_CHANGE_BEHAVIOR(saturate);
_DECL_CHANGE_BEHAVIOR(saturated);
_DECL_CHANGE_BEHAVIOR(swizzle);

_DECL_CHANGE_BEHAVIOR(inner);
_DECL_CHANGE_BEHAVIOR(angle);
_DECL_CHANGE_BEHAVIOR(from_angle);

_DECL_CHANGE_BEHAVIOR(from_pitch_yaw);
_DECL_CHANGE_BEHAVIOR(transform);
_DECL_CHANGE_BEHAVIOR(transform_coord);
_DECL_CHANGE_BEHAVIOR(transform_normal);
_DECL_CHANGE_BEHAVIOR(rotate);

_DECL_CHANGE_BEHAVIOR(dot_normal);
_DECL_CHANGE_BEHAVIOR(dot_coord);
_DECL_CHANGE_BEHAVIOR(from_points);
_DECL_CHANGE_BEHAVIOR(from_normal_point);
_DECL_CHANGE_BEHAVIOR(point);
_DECL_CHANGE_BEHAVIOR(intersect);
_DECL_CHANGE_BEHAVIOR(intersect_line);
_DECL_CHANGE_BEHAVIOR(intersect_ray);

_DECL_CHANGE_BEHAVIOR(from_axis_angle);
_DECL_CHANGE_BEHAVIOR(from_matrix);
_DECL_CHANGE_BEHAVIOR(conjugate);
_DECL_CHANGE_BEHAVIOR(axis);
_DECL_CHANGE_BEHAVIOR(slerp);

_DECL_CHANGE_BEHAVIOR(right);
_DECL_CHANGE_BEHAVIOR(left);
_DECL_CHANGE_BEHAVIOR(up);
_DECL_CHANGE_BEHAVIOR(down);
_DECL_CHANGE_BEHAVIOR(back);
_DECL_CHANGE_BEHAVIOR(front);

_DECL_CHANGE_BEHAVIOR(position);
_DECL_CHANGE_BEHAVIOR(scale);
_DECL_CHANGE_BEHAVIOR(translate);
_DECL_CHANGE_BEHAVIOR(world);
_DECL_CHANGE_BEHAVIOR(perspective_field_of_view);
_DECL_CHANGE_BEHAVIOR(orthographics);
_DECL_CHANGE_BEHAVIOR(orthographics2d);
_DECL_CHANGE_BEHAVIOR(look_to);
_DECL_CHANGE_BEHAVIOR(look_at);
_DECL_CHANGE_BEHAVIOR(transpose);
_DECL_CHANGE_BEHAVIOR(transposed);
_DECL_CHANGE_BEHAVIOR(determinant);

_DECL_CHANGE_BEHAVIOR(inverse);
} // namespace behavior

#undef _DECL_CHANGE_BEHAVIOR

/*------------------------------------------------------------------------------------------
* Traits
*------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------
* Math
*------------------------------------------------------------------------------------------*/
template <typename> class Math;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Math<int> Mathi;
typedef Math<float> Mathf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Math<double> Mathd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Math<long double> Mathld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector2
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector2;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector2<int> Point2;
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector2<double> Vector2d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector2<long double> Vector2ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector3
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector3;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector3<int> Point3;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector3<double> Vector3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector3<long double> Vector3ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector4
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector4;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector4<int> Point4;
typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector4<double> Vector4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector4<long double> Vector4ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Matrix4x4
*------------------------------------------------------------------------------------------*/
template <typename> struct Matrix4x4;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Matrix4x4<float> Matrix4x4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Matrix4x4<double> Matrix4x4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Matrix4x4<long double> Matrix4x4ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Quaternion
*------------------------------------------------------------------------------------------*/
template <typename> struct Quaternion;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Quaternion<float> Quaternionf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Quaternion<double> Quaterniond;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Quaternion<long double> Quaternionld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Plane
*------------------------------------------------------------------------------------------*/
template <typename> struct Plane;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Plane<float> Planef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Plane<double> Planed;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Plane<long double> Planeld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Color
*------------------------------------------------------------------------------------------*/
template <typename> struct Color;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Color<float> Colorf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Color<double> Colord;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Color<long double> Colorld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Range, Rectangle
*------------------------------------------------------------------------------------------*/
template <typename> struct Range;
template <typename> struct Rectangle;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Range<int> Rangei;
typedef Range<float> Rangef;
typedef Rectangle<int> Rectanglei;
typedef Rectangle<float> Rectanglef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Range<double> Ranged;
typedef Rectangle<double> Rectangled;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Range<long double> Rangeld;
typedef Rectangle<long double> Rectangleld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Line
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct Line;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Line<float, Vector2> Line2f;
typedef Line<float, Vector3> Line3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Line<double, Vector2> Line2d;
typedef Line<double, Vector3> Line3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Line<long double, Vector2> Line2ld;
typedef Line<long double, Vector3> Line3ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Ray
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct Ray;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Ray<float, Vector2> Ray2f;
typedef Ray<float, Vector3> Ray3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Ray<double, Vector2> Ray2d;
typedef Ray<double, Vector3> Ray3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Ray<long double, Vector2> Ray2ld;
typedef Ray<long double, Vector3> Ray3ld;
#endif // _USING_MATH_LONG_DOUBLE

} // namespace pocket

#endif // __MATH_FORWARD_H__
