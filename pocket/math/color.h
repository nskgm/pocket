﻿#ifndef __POCKET_MATH_COLOR_H__
#define __POCKET_MATH_COLOR_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../call.h"
#include "../container/array.h"
#include "math_traits.h"
#ifdef POCKET_USE_SIMD_ANONYMOUS
#include "simd_traits.h"
#endif // POCKET_USE_SIMD_ANONYMOUS
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct color;
#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef color<float> colorf;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef color<double> colord;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef color<long double> colorld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
struct color
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

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

#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef simd_traits<T> simd;
	typedef typename simd::type simd_type;
#endif // POCKET_USE_SIMD_ANONYMOUS

	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			T r;
			T g;
			T b;
			T a;

#ifdef POCKET_USE_ANONYMOUS
		};

#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type mm;
#endif // POCKET_USE_SIMD_ANONYMOUS

		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct color;

	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

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

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(color);
	explicit color(const call::noinitialize_t&)
	{}
	color(T r, T g, T b, T a) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(r, g, b, a))
#else
		r(r), g(g), b(b), a(a)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{}
	template <typename U, typename U1, typename U2, typename U3,
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
	color(U r, U1 g, U2 b, U3 a) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(r), static_cast<T>(g), static_cast<T>(b), static_cast<T>(a)))
#else
		r(static_cast<T>(r)), g(static_cast<T>(g)), b(static_cast<T>(b)), a(static_cast<T>(a))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{}
	template <typename U>
	color(const color<U>& c) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(c.r), static_cast<T>(c.g), static_cast<T>(c.b), static_cast<T>(c.a)))
#else
		r(static_cast<T>(c.r)), g(static_cast<T>(c.g)), b(static_cast<U>(c.b)), a(static_cast<U>(c.a))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{}
	color(int_type bytes) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(color::byte_to_float_r(bytes),
			color::byte_to_float_g(bytes),
			color::byte_to_float_b(bytes),
			color::byte_to_float_a(bytes)))
#else
		r(color::byte_to_float_r(bytes)),
		g(color::byte_to_float_g(bytes)),
		b(color::byte_to_float_b(bytes)),
		a(color::byte_to_float_a(bytes))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{}
#ifdef POCKET_USE_SIMD_ANONYMOUS
	explicit color(simd_type mm) :
		mm(mm)
	{}
#endif // POCKET_USE_SIMD_ANONYMOUS

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
	color& add(const color& c, color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::add(mm, c.mm);
#else
		result.r = r + c.r;
		result.g = g + c.g;
		result.b = b + c.b;
		result.a = a + c.a;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
	color& subtract(const color& c, color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::sub(mm, c.mm);
#else
		result.r = r - c.r;
		result.g = g - c.g;
		result.b = b - c.b;
		result.a = a - c.a;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
	color& multiply(T f, color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::mul(mm, f);
#else
		result.r = r * f;
		result.g = g * f;
		result.b = b * f;
		result.a = a * f;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	color& multiply(const color& c, color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::mul(mm, c.mm);
#else
		result.r = r * c.r;
		result.g = g * c.g;
		result.b = b * c.b;
		result.a = a * c.a;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	color& modulate(const color& c, color& result) const
	{
		return multiply(c, result);
	}
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	color& divide(T f, color& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::div(mm, f);
#else
		f = math_type::reciprocal(f);
		result.r = r * f;
		result.g = g * f;
		result.b = b * f;
		result.a = a * f;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}

	//---------------------------------------------------------------------
	// 値が等しいか
	//---------------------------------------------------------------------
	bool near_equal(const color& c) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal(mm, c.mm);
#else
		return (math_type::near_equal(r, c.r) && math_type::near_equal(g, c.g) && math_type::near_equal(b, c.b) && math_type::near_equal(a, c.a));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	//---------------------------------------------------------------------
	// 値がすべてゼロに等しいか
	//---------------------------------------------------------------------
	bool near_equal_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal_zero(mm);
#else
		return (math_type::near_equal_zero(r) && math_type::near_equal_zero(g) && math_type::near_equal_zero(b) && math_type::near_equal_zero(a));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	//---------------------------------------------------------------------
	// 値がすべてゼロか
	//---------------------------------------------------------------------
	bool equal_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::equal(mm, simd::zero());
#else
		return (r == math_type::zero && g == math_type::zero && b == math_type::zero && a == math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// aを0に設定
	//---------------------------------------------------------------------
	color& a0()
	{
		a = math_type::zero;
		return *this;
	}

	//---------------------------------------------------------------------
	// aを1に設定
	//---------------------------------------------------------------------
	color& a1()
	{
		a = math_type::one;
		return *this;
	}

	//---------------------------------------------------------------------
	// すべての値を０～１へクランプ
	//---------------------------------------------------------------------
	color& saturate()
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::clamp01(mm);
#else
		r = math_type::clamp01(r);
		g = math_type::clamp01(g);
		b = math_type::clamp01(b);
		a = math_type::clamp01(a);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	color& saturate(color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::clamp01(mm);
#else
		result.r = math_type::clamp01(r);
		result.g = math_type::clamp01(g);
		result.b = math_type::clamp01(b);
		result.a = math_type::clamp01(a);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	color saturated() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::clamp01(mm));
#else
		return color(math_type::clamp01(r),
			math_type::clamp01(g),
			math_type::clamp01(b),
			math_type::clamp01(a));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 線形補間
	//---------------------------------------------------------------------
	color lerp(const color& to, T t) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::lerp(mm, to.mm, t));
#else
		return color(math_type::lerp(r, to.r, t),
			math_type::lerp(g, to.g, t),
			math_type::lerp(b, to.b, t),
			math_type::lerp(a, to.a, t));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	color& lerp(const color& to, T t, color& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::lerp(mm, to.mm, t);
#else
		result.r = math_type::lerp(r, to.r, t);
		result.g = math_type::lerp(g, to.g, t);
		result.b = math_type::lerp(b, to.b, t);
		result.a = math_type::lerp(a, to.a, t);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	color& lerp(const color& from, const color& to, T t)
	{
		return from.lerp(to, t, *this);
	}

	//---------------------------------------------------------------------
	// html形式のバイトへ変換
	//---------------------------------------------------------------------
	int_type to_bytes() const
	{
		return ((color::float_to_byte(r) << 24) |
			(color::float_to_byte(g) << 16) |
			(color::float_to_byte(b) << 8) |
			color::float_to_byte(a));
	}

	//---------------------------------------------------------------------
	// 透過値の生成
	//---------------------------------------------------------------------
	static inline color alpha(T a)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::set(math_type::one, math_type::one, math_type::one, a));
#else
		return color(math_type::one, math_type::one, math_type::one, a);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 浮動小数からバイトへ変換（0～255）
	//---------------------------------------------------------------------
	static inline uint8_t float_to_byte(T f)
	{
		return f >= math_type::one ? static_cast<uint8_t>(0xFFU) :
				f <= math_type::zero ? static_cast<uint8_t>(0x00U) : static_cast<uint8_t>(f * color::float2byte);
	}
	//---------------------------------------------------------------------
	// html形式配色値から値の変換
	//---------------------------------------------------------------------
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

	//------------------------------------------------------------------------------------------
	// Operators
	//------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// アクセス演算子
	//---------------------------------------------------------------------
	T& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&r)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&r)[i];
#endif // POCKET_USE_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 変換演算子
	//---------------------------------------------------------------------
	template <typename U>
	POCKET_CXX11_EXPLICIT operator color<U>() const
	{
		return color<U>(static_cast<U>(r), static_cast<U>(g), static_cast<U>(b), static_cast<U>(a));
	}
	POCKET_CXX11_EXPLICIT operator T* ()
	{
#ifdef POCKET_USE_ANONYMOUS
		return &data[0];
#else
		return &r;
#endif // POCKET_USE_ANONYMOUS
	}
	POCKET_CXX11_EXPLICIT operator const T* () const
	{
#ifdef POCKET_USE_ANONYMOUS
		return &data[0];
#else
		return &r;
#endif // POCKET_USE_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const color& c) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::equal(mm, c.mm);
#else
		return r == c.r && g == c.g && b == c.b && a == c.a;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool operator != (const color& c) const
	{
		return !(*this == c);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	color operator + () const
	{
		return *this;
	}
	color operator - () const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::sub(simd::one(), mm));
#else
		return color(math_type::one - r, math_type::one - g, math_type::one - b, math_type::one - a);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	color operator + (const color& c) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::add(mm, c.mm));
#else
		color result(call::noinitialize);
		return add(c, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	color operator - (const color& c) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::sub(mm, c.mm));
#else
		color result(call::noinitialize);
		return subtract(c, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	color operator * (T f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::mul(mm, f));
#else
		color result(call::noinitialize);
		return multiply(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	color operator * (const color& c) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return color(simd::mul(mm, c.mm));
#else
		color result(call::noinitialize);
		return multiply(c, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	color operator / (T f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return color(simd::div(mm, f));
#else
		color result(call::noinitialize);
		return divide(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	color& operator += (const color& c)
	{
		return add(c, *this);
	}
	color& operator -= (const color& c)
	{
		return subtract(c, *this);
	}
	color& operator *= (T f)
	{
		return multiply(f, *this);
	}
	color& operator *= (const color& c)
	{
		return modulate(c, *this);
	}
	color& operator /= (T f)
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return operator*=(math_type::reciprocal(f));
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
#ifndef POCKET_NO_USING_MATH_INT_FLOAT
template <> const float color<float>::float2byte = 255.0f;
template <> const float color<float>::byte2float = 1.0f / 255.0f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
template <> const double color<double>::float2byte = 255.0;
template <> const double color<double>::byte2float = 1.0 / 255.0;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
template <> const long double color<long double>::float2byte = 255.0L;
template <> const long double color<long double>::byte2float = 1.0L / 255.0L;
#endif // POCKET_USING_MATH_LONG_DOUBLE

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

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_COLOR_H__
