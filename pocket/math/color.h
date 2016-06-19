#ifndef __POCKET_MATH_COLOR_H__
#define __POCKET_MATH_COLOR_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../container/array.h"
#include "math_traits.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct color;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef color<float> colorf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef color<double> colord;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef color<long double> colorld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct color
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	typedef typename math_type::value_int_type int_type;

	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS

			T r;
			T g;
			T b;
			T a;

#ifdef _USE_ANONYMOUS
		};

		array_type data;
	};
#endif // _USE_ANONYMOUS

	template <typename> friend struct color;

	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	static const color black; // [0.0, 0.0, 0.0, 1.0]
	static const color white; // [1.0, 1.0, 1.0, 1.0]
	static const color red; // [1.0, 0.0, 0.0, 1.0]
	static const color green; // [0.0, 1.0, 0.0, 1.0]
	static const color blue; // [0.0, 0.0, 1.0, 1.0]
	static const color yellow; // [1.0, 1.0, 0.0, 1.0]
	static const color cyan; // [0.0, 1.0, 1.0, 1.0]
	static const color magenta; // [1.0, 0.0, 1.0, 1.0]
	static const color gray; // [0.25, 0.25, 0.25, 1.0]
	static const color orange; // [1.0, 0.5, 0.0, 1.0]
	static const color clear; // [1.0, 1.0, 1.0, 0.25]
	static const color none; // [0.0, 0.0, 0.0, 0.0]

	static const T byte2float; // 1.0 / 255.0
	static const T float2byte; // 255.0

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(color);
	explicit color(const behavior::_noinitialize_t&)
	{

	}
	color(T r, T g, T b, T a) :
		r(r), g(g), b(b), a(a)
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
	color(U r, U1 g, U2 b, U3 a) :
		r(static_cast<T>(r)), g(static_cast<T>(g)), b(static_cast<T>(b)), a(static_cast<T>(a))
	{

	}
	template <typename U>
	color(const color<U>& c) :
		r(static_cast<T>(c.r)), g(static_cast<T>(c.g)), b(static_cast<U>(c.b)), a(static_cast<U>(c.a))
	{

	}
	color(int_type bytes) :
		r(color::byte_to_float_r(bytes)),
		g(color::byte_to_float_g(bytes)),
		b(color::byte_to_float_b(bytes)),
		a(color::byte_to_float_a(bytes))
	{

	}
	explicit color(T a) :
		r(math_type::one),
		g(math_type::one),
		b(math_type::one),
		a(a)
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	color& add(const color& c, color& result) const
	{
		result.r = r + c.r;
		result.g = g + c.g;
		result.b = b + c.b;
		result.a = a + c.a;
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	color& subtract(const color& c, color& result) const
	{
		result.r = r - c.r;
		result.g = g - c.g;
		result.b = b - c.b;
		result.a = a - c.a;
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	color& multiply(T f, color& result) const
	{
		result.r = r * f;
		result.g = g * f;
		result.b = b * f;
		result.a = a * f;
		return result;
	}
	color& multiply(const color& c, color& result) const
	{
		result.r = r * c.r;
		result.g = g * c.g;
		result.b = b * c.b;
		result.a = a * c.a;
		return result;
	}
	color& modulate(const color& c, color& result) const
	{
		return multiply(c, result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	color& divide(T f, color& result) const
	{
		_DEB_ASSERT(f != math_type::zero);
		return multiply(math_type::reciprocal(f), result);
	}

	/*---------------------------------------------------------------------
	* 値が等しいか
	*---------------------------------------------------------------------*/
	bool is_near(const color& q) const
	{
		return (math_type::is_near(r, q.r) && math_type::is_near(g, q.g) && math_type::is_near(b, q.b) && math_type::is_near(a, q.a));
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロに等しいか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(r) && math_type::is_near_zero(g) && math_type::is_near_zero(b) && math_type::is_near_zero(a));
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (r == math_type::zero && g == math_type::zero && b == math_type::zero && a == math_type::zero);
	}

	/*---------------------------------------------------------------------
	* すべての値を０～１へクランプ
	*---------------------------------------------------------------------*/
	color& saturate()
	{
		r = math_type::clamp01(r);
		g = math_type::clamp01(g);
		b = math_type::clamp01(b);
		a = math_type::clamp01(a);
		return *this;
	}
	color& saturate(color& result) const
	{
		result.r = math_type::clamp01(r);
		result.g = math_type::clamp01(g);
		result.b = math_type::clamp01(b);
		result.a = math_type::clamp01(a);
		return result;
	}
	color saturated() const
	{
		return color(math_type::clamp01(r),
			math_type::clamp01(g),
			math_type::clamp01(b),
			math_type::clamp01(a));
	}

	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	color lerp(const color& to, T t) const
	{
		return color(math_type::lerp(r, to.r, t),
			math_type::lerp(g, to.g, t),
			math_type::lerp(b, to.b, t),
			math_type::lerp(a, to.a, t));
	}
	color& lerp(const color& to, T t, color& result) const
	{
		result.r = math_type::lerp(r, to.r, t);
		result.g = math_type::lerp(g, to.g, t);
		result.b = math_type::lerp(b, to.b, t);
		result.a = math_type::lerp(a, to.a, t);
		return result;
	}
	color& lerp(const color& from, const color& to, T t)
	{
		return from.lerp(to, t, *this);
	}

	/*---------------------------------------------------------------------
	* html形式のバイトへ変換
	*---------------------------------------------------------------------*/
	int_type to_bytes() const
	{
		return ((color::float_to_byte(r) << 24) |
			(color::float_to_byte(g) << 16) |
			(color::float_to_byte(b) << 8) |
			color::float_to_byte(a));
	}

	/*---------------------------------------------------------------------
	* 浮動小数からバイトへ変換（0～255）
	*---------------------------------------------------------------------*/
	static inline uint8_t float_to_byte(T f)
	{
		return f >= math_type::one ? static_cast<uint8_t>(0xFFU) :
				f <= math_type::zero ? static_cast<uint8_t>(0x00U) : static_cast<uint8_t>(f * color::float2byte);
	}
	/*---------------------------------------------------------------------
	* html形式配色値から値の変換
	*---------------------------------------------------------------------*/
	static inline T byte_to_float_r(int_type bytes)
	{
		return static_cast<T>((bytes >> 24) & 0xFF) * color::byte2float;
	}
	static inline T byte_to_float_g(int_type bytes)
	{
		return static_cast<T>((bytes >> 16) & 0xFF) * color::byte2float;
	}
	static inline T byte_to_float_b(int_type bytes)
	{
		return static_cast<T>((bytes >> 8) & 0xFF) * color::byte2float;
	}
	static inline T byte_to_float_a(int_type bytes)
	{
		return static_cast<T>(bytes & 0xFF) * color::byte2float;
	}

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return data[i];
#else
		return (&r)[i];
#endif // _USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return data[i];
#else
		return (&r)[i];
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator color<U>() const
	{
		return color<U>(static_cast<U>(r), static_cast<U>(g), static_cast<U>(b), static_cast<U>(a));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &data[0];
#else
		return &r;
#endif // _USE_ANONYMOUS
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &data[0];
#else
		return &r;
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const color& c) const
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}
	bool operator != (const color& c) const
	{
		return !(*this == c);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	color operator + () const
	{
		return *this;
	}
	color operator - () const
	{
		return color(math_type::one - r, math_type::one - g, math_type::one - b, math_type::one - a);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	color operator + (const color& q) const
	{
		color result(behavior::noinitialize);
		return add(q, result);
	}
	color operator - (const color& q) const
	{
		color result(behavior::noinitialize);
		return subtract(q, result);
	}
	color operator * (T f) const
	{
		color result(behavior::noinitialize);
		return multiply(f, result);
	}
	color operator * (const color& q) const
	{
		color result(behavior::noinitialize);
		return multiply(q, result);
	}
	color operator / (T f) const
	{
		color result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	color& operator += (const color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}
	color& operator -= (const color& c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}
	color& operator *= (T f)
	{
		r *= f;
		g *= f;
		b *= f;
		a *= f;
		return *this;
	}
	color& operator *= (const color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
		return *this;
	}
	color& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::zero);
		return *this *= math_type::reciprocal(f);
	}
};

template <typename T>
const color<T> color<T>::black(math_type::zero, math_type::zero, math_type::zero, math_type::one);
template <typename T>
const color<T> color<T>::white(math_type::one, math_type::one, math_type::one, math_type::one);
template <typename T>
const color<T> color<T>::red(math_type::one, math_type::zero, math_type::zero, math_type::one);
template <typename T>
const color<T> color<T>::green(math_type::zero, math_type::one, math_type::zero, math_type::one);
template <typename T>
const color<T> color<T>::blue(math_type::zero, math_type::zero, math_type::one, math_type::one);
template <typename T>
const color<T> color<T>::yellow(math_type::one, math_type::one, math_type::zero, math_type::one);
template <typename T>
const color<T> color<T>::cyan(math_type::zero, math_type::one, math_type::one, math_type::one);
template <typename T>
const color<T> color<T>::magenta(math_type::one, math_type::zero, math_type::one, math_type::one);
template <typename T>
const color<T> color<T>::gray(math_type::half_of_half, math_type::half_of_half, math_type::half_of_half, math_type::one);
template <typename T>
const color<T> color<T>::orange(math_type::one, math_type::half, math_type::zero, math_type::one);
template <typename T>
const color<T> color<T>::clear(math_type::one, math_type::one, math_type::one, math_type::half_of_half);
template <typename T>
const color<T> color<T>::none(math_type::zero, math_type::zero, math_type::zero, math_type::zero);
#ifndef _UNUSING_MATH_INT_FLOAT
template <> const float color<float>::float2byte = 255.0f;
template <> const float color<float>::byte2float = 1.0f / 255.0f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
template <> const double color<double>::float2byte = 255.0;
template <> const double color<double>::byte2float = 1.0 / 255.0;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
template <> const long double color<long double>::float2byte = 255.0L;
template <> const long double color<long double>::byte2float = 1.0L / 255.0L;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const color<T>& c)
{
	os << io::parentheses_left << c.r << io::comma_space
		<< c.g << io::comma_space
		<< c.b << io::comma_space
		<< c.a << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, color<T>& c)
{
	is.ignore();
	is >> c.r;
	is.ignore();
	is >> c.g;
	is.ignore();
	is >> c.b;
	is.ignore();
	is >> c.a;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const color<T>& c)
{
	os << io::parentheses_left << c.r << io::comma_space
		<< c.g << io::comma_space
		<< c.b << io::comma_space
		<< c.a << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, color<T>& c)
{
	is.ignore();
	is >> c.r;
	is.ignore();
	is >> c.g;
	is.ignore();
	is >> c.b;
	is.ignore();
	is >> c.a;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_COLOR_H__
