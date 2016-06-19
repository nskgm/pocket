﻿#ifndef __POCKET_MATH_RECTANGLE_H__
#define __POCKET_MATH_RECTANGLE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../debug.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector2.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct range;
template <typename> struct rectangle;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef range<int> rangei;
typedef range<float> rangef;
typedef rectangle<int> rectanglei;
typedef rectangle<float> rectanglef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef range<double> ranged;
typedef rectangle<double> rectangled;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef range<long double> rangeld;
typedef rectangle<long double> rectangleld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct range
{
	_MATH_STATICAL_ASSERT(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;

	typedef container::array<T, 2> array_type;
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

			T minimum;
			T maximum;

#ifdef _USE_ANONYMOUS
		};

		array_type data;
	};
#endif // _USE_ANONYMOUS

	template <typename> friend struct range;

	/*-----------------------------------------------------------------------------------------
	* Constantors
	*-----------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constantors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(range);
	explicit range(const behavior::_noinitialize_t&)
	{

	}
	range(T min, T max) :
		minimum(min), maximum(max)
	{

	}
	template <typename U>
	range(const range<U>& r) :
		minimum(static_cast<T>(r.minimum)), maximum(static_cast<T>(r.maximum))
	{

	}
	template <typename U, typename U1,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1)
	>
		range(U min, U1 max) :
		minimum(static_cast<T>(min)), maximum(static_cast<T>(max))
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 範囲内か
	*---------------------------------------------------------------------*/
	bool is_in_range(T f) const
	{
		return f >= minimum && f <= maximum;
	}
	bool is_in_range(const range& r) const
	{
		return minimum <= r.minimum && maximum >= r.maximum;
	}
	/*---------------------------------------------------------------------
	* 最大と最小の差を求める
	*---------------------------------------------------------------------*/
	T difference() const
	{
		return maximum - minimum;
	}

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
#ifdef _USE_ANONYMOUS
		return data[i];
#else
		return (&minimum)[i];
#endif // _USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
#ifdef _USE_ANONYMOUS
		return data[i];
#else
		return (&minimum)[i];
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator range<U>() const
	{
		return range<U>(static_cast<U>(minimum), static_cast<U>(maximum));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &data[0];
#else
		return &minimum;
#endif // _USE_ANONYMOUS
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &data[0];
#else
		return &minimum;
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const range& r) const
	{
		return minimum == r.minimum && maximum == r.maximum;
	}
	bool operator != (const range& r) const
	{
		return !(*this == r);
	}
};

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const range<T>& r)
{
	// (minimum, maximum)
	os << io::parentheses_left << r.minimum << io::comma_space
		<< r.maximum << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, range<T>& r)
{
	is.ignore();
	is >> r.minimum;
	is.ignore();
	is >> r.maximum;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const range<T>& r)
{
	os << io::parentheses_left << r.minimum << io::comma_space
		<< r.maximum << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, range<T>& r)
{
	is.ignore();
	is >> r.minimum;
	is.ignore();
	is >> r.maximum;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO


template <typename T>
struct rectangle
{
	_MATH_STATICAL_ASSERT(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef range<T> range_type;

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

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS_NON_POD

			range_type x;
			range_type y;

#ifdef _USE_ANONYMOUS_NON_POD
		};

		array_type data;
	};
#endif // _USE_ANONYMOUS_NON_POD

	template <typename> friend struct rectangle;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constantors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(rectangle);
	explicit rectangle(const behavior::_noinitialize_t&)
	{

	}
	rectangle(T l, T r, T t, T b) :
		x(l, r), y(t, b)
	{

	}
	rectangle(T r, T b) :
		x(math_type::zero, r), y(math_type::zero, b)
	{

	}
	rectangle(const range_type& x, const range_type& y) :
		x(x), y(y)
	{

	}
	template <typename U>
	rectangle(const rectangle<U>& r) :
		x(static_cast<range_type>(r.x)),
		y(static_cast<range_type>(r.y))
	{

	}
	template <typename U>
	rectangle(const range<U>& x, const range<U>& y) :
		x(static_cast<range<T> >(x)),
		y(static_cast<range<T> >(y))
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
		rectangle(U l, U1 r, U2 t, U3 b) :
		x(static_cast<T>(l), static_cast<T>(r)),
		y(static_cast<T>(t), static_cast<T>(b))
	{

	}
	rectangle(const vector2<T>& x, const vector2<T>& y) :
		x(x.x, y.x), y(x.y, y.y)
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 範囲に入っているか
	*---------------------------------------------------------------------*/
	bool is_in_inside(const vector2<T>& v) const
	{
		return x.is_in_range(v.x) && y.is_in_range(v.y);
	}
	bool is_in_inside(const rectangle& r) const
	{
		return x.is_in_range(r.x) && y.is_in_range(r.y);
	}

	/*---------------------------------------------------------------------
	* アスペクト比を求める
	*---------------------------------------------------------------------*/
	T aspect() const
	{
		return x.difference() / y.difference();
	}
	/*---------------------------------------------------------------------
	* x方向の差を求める
	*---------------------------------------------------------------------*/
	T difference_x() const
	{
		return x.difference();
	}
	/*---------------------------------------------------------------------
	* y方向の差を求める
	*---------------------------------------------------------------------*/
	T difference_y() const
	{
		return y.difference();
	}

	const range<T>& range_x() const
	{
		return x;
	}
	const range<T>& range_y() const
	{
		return y;
	}

	// 左上の座標取得
	vector2<T> left_top() const
	{
		return vector2<T>(x.minimum, y.minimum);
	}
	// 左下の座標取得
	vector2<T> left_bottom() const
	{
		return vector2<T>(x.minimum, y.maximum);
	}
	// 右上の座標取得
	vector2<T> right_top() const
	{
		return vector2<T>(x.maximum, y.minimum);
	}
	// 右下の座標取得
	vector2<T> right_bottom() const
	{
		return vector2<T>(x.maximum, x.maximum);
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
#ifdef _USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&x.minimum)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&x.minimum)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator rectangle<U>() const
	{
		typedef range<U> other_range_type;
		return rectangle<U>(static_cast<other_range_type>(x), static_cast<other_range_type>(y));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &x.minimum;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &x.minimum;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const rectangle& r) const
	{
		return x == r.x && y == r.y;
	}
	bool operator != (const rectangle& r) const
	{
		return !(*this == r);
	}
};

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const rectangle<T>& r)
{
	os << io::parentheses_left << r.x << io::comma_space
		<< r.y << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, rectangle<T>& r)
{
	is.ignore();
	is >> r.x;
	is.ignore();
	is >> r.y;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const rectangle<T>& r)
{
	os << io::parentheses_left << r.x << io::comma_space
		<< r.y << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, rectangle<T>& r)
{
	is.ignore();
	is >> r.x;
	is.ignore();
	is >> r.y;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_RECTANGLE_H__