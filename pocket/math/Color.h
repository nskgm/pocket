#ifndef __MATH_COLOR_H__
#define __MATH_COLOR_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "array.h"
#include "Math.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif // _USING_MATH_IO

namespace pocket
{

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

#ifdef _USE_CXX11
template <typename T>
using color = Color<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using colorf = color<float>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using colord = color<double>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using colorld = color<long double>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T>
struct Color
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS

			T R;
			T G;
			T B;
			T A;

#ifdef _USE_ANONYMOUS
		};

		array_type Data;
	};
#endif // _USE_ANONYMOUS

	template <typename> friend struct Color;

	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	static const Color Black; // [0.0, 0.0, 0.0, 1.0]
	static const Color White; // [1.0, 1.0, 1.0, 1.0]
	static const Color Red; // [1.0, 0.0, 0.0, 1.0]
	static const Color Green; // [0.0, 1.0, 0.0, 1.0]
	static const Color Blue; // [0.0, 0.0, 1.0, 1.0]
	static const Color Yellow; // [1.0, 1.0, 0.0, 1.0]
	static const Color Cyan; // [0.0, 1.0, 1.0, 1.0]
	static const Color Magenta; // [1.0, 0.0, 1.0, 1.0]
	static const Color Gray; // [0.25, 0.25, 0.25, 1.0]
	static const Color Orange; // [1.0, 0.5, 0.0, 1.0]
	static const Color Clear; // [1.0, 1.0, 1.0, 0.25]
	static const Color None; // [0.0, 0.0, 0.0, 0.0]

	static const T Byte2Float; // 1.0 / 255.0
	static const T Float2Byte; // 255.0

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Color);
	explicit Color(const behavior::_noinitialize_t&)
	{

	}
	Color(T r, T g, T b, T a) :
		R(r), G(g), B(b), A(a)
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
	Color(U r, U1 g, U2 b, U3 a) :
		R(static_cast<T>(r)), G(static_cast<T>(g)), B(static_cast<T>(b)), A(static_cast<T>(a))
	{

	}
	template <typename U>
	Color(const Color<U>& c) :
		R(static_cast<T>(c.R)), G(static_cast<T>(c.G)), B(static_cast<U>(c.B)), A(static_cast<U>(c.A))
	{

	}
	Color(uint32_t bytes) :
		R(Color::byte_to_float_r(bytes)),
		G(Color::byte_to_float_g(bytes)),
		B(Color::byte_to_float_b(bytes)),
		A(Color::byte_to_float_a(bytes))
	{

	}
	explicit Color(T a) :
		R(math_type::One),
		G(math_type::One),
		B(math_type::One),
		A(a)
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Color& add(const Color& c, Color& result) const
	{
		result.R = R + c.R;
		result.G = G + c.G;
		result.B = B + c.B;
		result.A = A + c.A;
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	Color& subtract(const Color& c, Color& result) const
	{
		result.R = R - c.R;
		result.G = G - c.G;
		result.B = B - c.B;
		result.A = A - c.A;
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	Color& multiply(T f, Color& result) const
	{
		result.R = R * f;
		result.G = G * f;
		result.B = B * f;
		result.A = A * f;
		return result;
	}
	Color& multiply(const Color& c, Color& result) const
	{
		result.R = R * c.R;
		result.G = G * c.G;
		result.B = B * c.B;
		result.A = A * c.A;
		return result;
	}
	Color& modulate(const Color& c, Color& result) const
	{
		return multiply(c, result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Color& divide(T f, Color& result) const
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return multiply(f, result);
	}

	/*---------------------------------------------------------------------
	* 値が等しいか
	*---------------------------------------------------------------------*/
	bool is_near(const Color& q) const
	{
		return (math_type::is_near(R, q.R) && math_type::is_near(G, q.G) && math_type::is_near(B, q.B) && math_type::is_near(A, q.A));
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロに等しいか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(R) && math_type::is_near_zero(G) && math_type::is_near_zero(B) && math_type::is_near_zero(A));
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (R == math_type::Zero && G == math_type::Zero && B == math_type::Zero && A == math_type::Zero);
	}

	/*---------------------------------------------------------------------
	* すべての値を０～１へクランプ
	*---------------------------------------------------------------------*/
	Color& saturate()
	{
		R = math_type::clamp01(R);
		G = math_type::clamp01(G);
		B = math_type::clamp01(B);
		A = math_type::clamp01(A);
		return *this;
	}
	Color& saturated(Color& result) const
	{
		result.R = math_type::clamp01(R);
		result.G = math_type::clamp01(G);
		result.B = math_type::clamp01(B);
		result.A = math_type::clamp01(A);
		return result;
	}
	Color saturated() const
	{
		return Color(math_type::clamp01(R),
			math_type::clamp01(G),
			math_type::clamp01(B),
			math_type::clamp01(A));
	}

	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Color lerp(const Color& to, T t) const
	{
		return Color(math_type::lerp(R, to.R, t),
			math_type::lerp(G, to.G, t),
			math_type::lerp(B, to.B, t),
			math_type::lerp(A, to.A, t));
	}
	Color& lerp(const Color& to, T t, Color& result) const
	{
		result.R = math_type::lerp(R, to.R, t);
		result.G = math_type::lerp(G, to.G, t);
		result.B = math_type::lerp(B, to.B, t);
		result.A = math_type::lerp(A, to.A, t);
		return result;
	}
	Color& lerp(const Color& from, const Color& to, T t)
	{
		return from.lerp(to, t, *this);
	}

	/*---------------------------------------------------------------------
	* html形式のバイトへ変換
	*---------------------------------------------------------------------*/
	uint32_t to_bytes() const
	{
		return ((Color::float_to_byte(R) << 24) | (Color::float_to_byte(G) << 16) | (Color::float_to_byte(B) << 8) | Color::float_to_byte(A));
	}

	/*---------------------------------------------------------------------
	* 浮動小数からバイトへ変換（0～255）
	*---------------------------------------------------------------------*/
	static inline uint8_t float_to_byte(T f)
	{
		return f >= math_type::One ? static_cast<uint8_t>(0xFFU) :
				f <= math_type::Zero ? static_cast<uint8_t>(0x00U) : static_cast<uint8_t>(f * Color::Float2Byte);
	}
	/*---------------------------------------------------------------------
	* html形式配色値から値の変換
	*---------------------------------------------------------------------*/
	static inline T byte_to_float_r(uint32_t bytes)
	{
		return static_cast<T>((bytes >> 24) & 0xFF) * Color::Byte2Float;
	}
	static inline T byte_to_float_g(uint32_t bytes)
	{
		return static_cast<T>((bytes >> 16) & 0xFF) * Color::Byte2Float;
	}
	static inline T byte_to_float_b(uint32_t bytes)
	{
		return static_cast<T>((bytes >> 8) & 0xFF) * Color::Byte2Float;
	}
	static inline T byte_to_float_a(uint32_t bytes)
	{
		return static_cast<T>(bytes & 0xFF) * Color::Byte2Float;
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
		return Data[i];
#else
		return (&R)[i];
#endif // _USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&R)[i];
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Color<U>() const
	{
		return Color<U>(static_cast<U>(R), static_cast<U>(G), static_cast<U>(B), static_cast<U>(A));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &R;
#endif // _USE_ANONYMOUS
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &R;
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Color& c) const
	{
		return R == c.R && G == c.G && B == c.B && A == c.A;
	}
	bool operator != (const Color& c) const
	{
		return !(*this == c);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Color operator + () const
	{
		return *this;
	}
	Color operator - () const
	{
		return Color(math_type::One - R, math_type::One - G, math_type::One - B, math_type::One - A);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Color operator + (const Color& q) const
	{
		Color result(behavior::noinitialize);
		return add(q, result);
	}
	Color operator - (const Color& q) const
	{
		Color result(behavior::noinitialize);
		return subtract(q, result);
	}
	Color operator * (T f) const
	{
		Color result(behavior::noinitialize);
		return multiply(f, result);
	}
	Color operator * (const Color& q) const
	{
		Color result(behavior::noinitialize);
		return multiply(q, result);
	}
	Color operator / (T f) const
	{
		Color result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Color& operator += (const Color& c)
	{
		R += c.R;
		G += c.G;
		B += c.B;
		A += c.A;
		return *this;
	}
	Color& operator -= (const Color& c)
	{
		R -= c.R;
		G -= c.G;
		B -= c.B;
		A -= c.A;
		return *this;
	}
	Color& operator *= (T f)
	{
		R *= f;
		G *= f;
		B *= f;
		A *= f;
		return *this;
	}
	Color& operator *= (const Color& c)
	{
		R *= c.R;
		G *= c.G;
		B *= c.B;
		A *= c.A;
		return *this;
	}
	Color& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return *this *= f;
	}
};

template <typename T>
const Color<T> Color<T>::Black(math_type::Zero, math_type::Zero, math_type::Zero, math_type::One);
template <typename T>
const Color<T> Color<T>::White(math_type::One, math_type::One, math_type::One, math_type::One);
template <typename T>
const Color<T> Color<T>::Red(math_type::One, math_type::Zero, math_type::Zero, math_type::One);
template <typename T>
const Color<T> Color<T>::Green(math_type::Zero, math_type::One, math_type::Zero, math_type::One);
template <typename T>
const Color<T> Color<T>::Blue(math_type::Zero, math_type::Zero, math_type::One, math_type::One);
template <typename T>
const Color<T> Color<T>::Yellow(math_type::One, math_type::One, math_type::Zero, math_type::One);
template <typename T>
const Color<T> Color<T>::Cyan(math_type::Zero, math_type::One, math_type::One, math_type::One);
template <typename T>
const Color<T> Color<T>::Magenta(math_type::One, math_type::Zero, math_type::One, math_type::One);
template <typename T>
const Color<T> Color<T>::Gray(math_type::HalfOfHalf, math_type::HalfOfHalf, math_type::HalfOfHalf, math_type::One);
template <typename T>
const Color<T> Color<T>::Orange(math_type::One, math_type::Half, math_type::Zero, math_type::One);
template <typename T>
const Color<T> Color<T>::Clear(math_type::One, math_type::One, math_type::One, math_type::HalfOfHalf);
template <typename T>
const Color<T> Color<T>::None(math_type::Zero, math_type::Zero, math_type::Zero, math_type::Zero);
#ifndef _UNUSING_MATH_INT_FLOAT
template <> const float Color<float>::Float2Byte = 255.0f;
template <> const float Color<float>::Byte2Float = 1.0f / 255.0f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
template <> const double Color<double>::Float2Byte = 255.0;
template <> const double Color<double>::Byte2Float = 1.0 / 255.0;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
template <> const long double Color<long double>::Float2Byte = 255.0L;
template <> const long double Color<long double>::Byte2Float = 1.0L / 255.0L;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Color<T>& c)
{
	os << out_char::parentheses_left << c.R << out_char::comma_space
		<< c.G << out_char::comma_space
		<< c.B << out_char::comma_space
		<< c.A << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Color<T>& c)
{
	is.ignore();
	is >> c.R;
	is.ignore();
	is >> c.G;
	is.ignore();
	is >> c.B;
	is.ignore();
	is >> c.A;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Color<T>& c)
{
	os << out_char::parentheses_left << c.R << out_char::comma_space
		<< c.G << out_char::comma_space
		<< c.B << out_char::comma_space
		<< c.A << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Color<T>& c)
{
	is.ignore();
	is >> c.R;
	is.ignore();
	is >> c.G;
	is.ignore();
	is >> c.B;
	is.ignore();
	is >> c.A;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __MATH_COLOR_H__
