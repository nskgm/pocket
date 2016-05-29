#ifndef __MATH_RECTANGLE_H__
#define __MATH_RECTANGLE_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "array.h"
#include "Math.h"
#include "Vector2.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename> struct Range;
template <typename> struct Rectangle;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Range<int> Rangei;
typedef Range<float> Rangef;
typedef Rectangle<int> Rectanglei;
typedef Rectangle<float> Rectanglef;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Range<double> Ranged;
typedef Rectangle<double> Rectangled;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Range<long double> Rangeld;
typedef Rectangle<long double> Rectangleld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T>
using rect = Rectangle<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using rectf = rect<float>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using rectd = rect<double>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using rectld = rect<long double>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

template <typename T>
struct Range
{
	_MATH_STATICAL_ASSERT(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;

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
#endif /* _USE_ANONYMOUS */

			T Min;
			T Max;

#ifdef _USE_ANONYMOUS
		};

		array_type Data;
	};
#endif /* _USE_ANONYMOUS */

	template <typename> friend struct Range;

	/*-----------------------------------------------------------------------------------------
	* Constantors
	*-----------------------------------------------------------------------------------------*/

	/* None */

	/*------------------------------------------------------------------------------------------
	* Constantors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Range);
	explicit Range(const behavior::_noinitialize_t&)
	{

	}
	Range(T min, T max) :
		Min(min), Max(max)
	{

	}
	template <typename U>
	Range(const Range<U>& r) :
		Min(static_cast<T>(r.Min)), Max(static_cast<T>(r.Max))
	{

	}
	template <typename U, typename U1,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1)
	>
		Range(U min, U1 max) :
		Min(static_cast<T>(min)), Max(static_cast<T>(max))
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
		return f >= Min && f <= Max;
	}
	bool is_in_range(const Range& r) const
	{
		return Min <= r.Min && Max >= r.Max;
	}
	/*---------------------------------------------------------------------
	* 最大と最小の差を求める
	*---------------------------------------------------------------------*/
	T difference() const
	{
		return Max - Min;
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
		return Data[i];
#else
		return (&Min)[i];
#endif /* _USE_ANONYMOUS */
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&Min)[i];
#endif /* _USE_ANONYMOUS */
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Range<U>() const
	{
		return Range<U>(static_cast<U>(Min), static_cast<U>(Max));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &Min;
#endif /* _USE_ANONYMOUS */
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &Min;
#endif /* _USE_ANONYMOUS */
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Range& r) const
	{
		return Min == r.Min && Max == r.Max;
	}
	bool operator != (const Range& r) const
	{
		return !(*this == r);
	}
};

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Range<T>& r)
{
	/* (Min, Max) */
	os << out_char::parentheses_left << r.Min << out_char::comma_space
		<< r.Max << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Range<T>& r)
{
	is.ignore();
	is >> r.Min;
	is.ignore();
	is >> r.Max;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Range<T>& r)
{
	os << out_char::parentheses_left << r.Min << out_char::comma_space
		<< r.Max << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Range<T>& r)
{
	is.ignore();
	is >> r.Min;
	is.ignore();
	is >> r.Max;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */


template <typename T>
struct Rectangle
{
	_MATH_STATICAL_ASSERT(T);

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

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif /* _USE_ANONYMOUS_NON_POD */

			Range<T> X;
			Range<T> Y;

#ifdef _USE_ANONYMOUS_NON_POD
		};

		array_type Data;
	};
#endif /* _USE_ANONYMOUS_NON_POD */

	template <typename> friend struct Rectangle;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	/* None */

	/*------------------------------------------------------------------------------------------
	* Constantors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Rectangle);
	explicit Rectangle(const behavior::_noinitialize_t&)
	{

	}
	Rectangle(T l, T r, T t, T b) :
		X(l, r), Y(t, b)
	{

	}
	Rectangle(T r, T b) :
		X(Math<T>::Zero, r), Y(Math<T>::Zero, b)
	{

	}
	Rectangle(const Range<T>& x, const Range<T>& y) :
		X(x), Y(y)
	{

	}
	template <typename U>
	Rectangle(const Rectangle<U>& r) :
		X(static_cast<Range<T> >(r.X)),
		Y(static_cast<Range<T> >(r.Y))
	{

	}
	template <typename U>
	Rectangle(const Range<U>& x, const Range<U>& y) :
		X(static_cast<Range<T> >(x)),
		Y(static_cast<Range<T> >(y))
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
		Rectangle(U l, U1 r, U2 t, U3 b) :
		X(static_cast<T>(l), static_cast<T>(r)),
		Y(static_cast<T>(t), static_cast<T>(b))
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 範囲に入っているか
	*---------------------------------------------------------------------*/
	bool is_in_inside(const Vector2<T>& v) const
	{
		return X.is_in_range(v.X) && Y.is_in_range(v.Y);
	}
	bool is_in_inside(const Rectangle& r) const
	{
		return X.is_in_range(r.X) && Y.is_in_range(r.Y);
	}

	/*---------------------------------------------------------------------
	* アスペクト比を求める
	*---------------------------------------------------------------------*/
	T aspect() const
	{
		return X.difference() / Y.difference();
	}
	/*---------------------------------------------------------------------
	* X方向の差を求める
	*---------------------------------------------------------------------*/
	T difference_x() const
	{
		return X.difference();
	}
	/*---------------------------------------------------------------------
	* Y方向の差を求める
	*---------------------------------------------------------------------*/
	T difference_y() const
	{
		return Y.difference();
	}

	const Range<T>& range_x() const
	{
		return X;
	}
	const Range<T>& range_y() const
	{
		return Y;
	}

	/* 左上の座標取得 */
	Vector2<T> left_top() const
	{
		return Vector2<T>(X.Min, Y.Min);
	}
	/* 左下の座標取得 */
	Vector2<T> left_bottom() const
	{
		return Vector2<T>(X.Min, Y.Max);
	}
	/* 右上の座標取得 */
	Vector2<T> right_top() const
	{
		return Vector2<T>(X.Max, Y.Min);
	}
	/* 右下の座標取得 */
	Vector2<T> right_bottom() const
	{
		return Vector2<T>(X.Max, X.Max);
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
		return Data[i];
#else
		return (&X.Min)[i];
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS_NON_POD
		return Data[i];
#else
		return (&X.Min)[i];
#endif /* _USE_ANONYMOUS_NON_POD */
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Rectangle<U>() const
	{
		return Rectangle<U>(static_cast<Range<U> >(X), static_cast<Range<U> >(Y));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &X.Min;
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &X.Min;
#endif /* _USE_ANONYMOUS_NON_POD */
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Rectangle& r) const
	{
		return X == r.X && Y == r.Y;
	}
	bool operator != (const Rectangle& r) const
	{
		return !(*this == r);
	}
};

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Rectangle<T>& r)
{
	os << out_char::parentheses_left << r.X << out_char::comma_space
		<< r.Y << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Rectangle<T>& r)
{
	is.ignore();
	is >> r.X;
	is.ignore();
	is >> r.Y;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Rectangle<T>& r)
{
	os << out_char::parentheses_left << r.X << out_char::comma_space
		<< r.Y << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Rectangle<T>& r)
{
	is.ignore();
	is >> r.X;
	is.ignore();
	is >> r.Y;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_RECTANGLE_H__ */
