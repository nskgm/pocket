#ifndef __MATH_LINE_H__
#define __MATH_LINE_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "Vector2.h"
#include "Vector3.h"
#include "Ray.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename, template <typename> class> struct Line;
template <typename> struct Plane;

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

#ifdef _USE_CXX11
template <typename T, template <typename> class VN>
using line = Line<T, VN>;
#ifndef _UNUSING_MATH_INT_FLOAT
using line2f = line<float, Vector2>;
using line3f = line<float, Vector3>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using line2d = line<double, Vector2>;
using line3d = line<double, Vector3>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using line2ld = line<long double, Vector2>;
using line3ld = line<long double, Vector3>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T, template <typename> class VectorN>
struct Line
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef VectorN<T> vector_type;
	typedef container::array<vector_type, 2> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	typedef Ray<T, VectorN> ray_type;
	typedef Plane<T> plane_type;

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

	vector_type Begin; // 開始位置
	vector_type End; // 終了位置

	template <typename, template <typename> class> friend struct Line;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Line Up; // [0.0, 0.0, 0.0], [0.0, 1.0, 0.0]
	static const Line Down; // [0.0, 0.0, 0.0], [0.0, -1.0, 0.0]
	static const Line Right; // [0.0, 0.0, 0.0], [1.0, 0.0, 0.0]
	static const Line Left; // [0.0, 0.0, 0.0], [-1.0, 0.0, 0.0]
	static const Line Front; // [0.0, 0.0, 0.0], [0.0, 0.0, 1.0]
	static const Line Back; // [0.0, 0.0, 0.0], [0.0, 0.0, -1.0]

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Line);
	explicit Line(const behavior::_noinitialize_t&) :
		Begin(behavior::noinitialize), End(behavior::noinitialize)
	{

	}
	explicit Line(const vector_type& e) :
		Begin(vector_type::Zero), End(e)
	{

	}
	Line(const vector_type& b, const vector_type& e) :
		Begin(b), End(e)
	{

	}

	explicit Line(const ray_type& r, T length) :
		Begin(r.Position), End(r.point(length))
	{

	}

	/*-----------------------------------------------------------------------------------------
	* Funstions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* レイから作成
	*---------------------------------------------------------------------*/
	Line& from_ray(const ray_type& r, T length)
	{
		Begin = r.Position;
		r.point(length, End);
		return *this;
	}

	/*---------------------------------------------------------------------
	* 線分開始地点から終了地点の方向を求める
	*---------------------------------------------------------------------*/
	vector_type direction() const
	{
		return End.direction(Begin);
	}
	vector_type& direction(vector_type& result) const
	{
		End.direction(Begin, result);
		return result;
	}

	/*---------------------------------------------------------------------
	* 線分の距離を求める
	*---------------------------------------------------------------------*/
	T distance() const
	{
		return Begin.distance(End);
	}

	/*---------------------------------------------------------------------
	* 線分が平面と交差しているか
	*---------------------------------------------------------------------*/
	bool is_intersect(const plane_type&) const; // Plane.h

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Line& l) const
	{
		return Begin == l.Begin && End == l.End;
	}
	bool operator != (const Line& l) const
	{
		return !(*this == l);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Line operator + () const
	{
		return *this;
	}
	Line operator - () const
	{
		return Line(-Begin, -End);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Line operator + (const vector_type& p) const
	{
		// 渡されてきた座標分どちらもずらす
		return Line(Begin + p, End + p);
	}
	Line operator - (const vector_type& p) const
	{
		return Line(Begin - p, End - p);
	}
	Line operator * (T s) const
	{
		return Line(Begin * s, End * s);
	}
	Line operator / (T s) const
	{
		return Line(Begin / s, End / s);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Line& operator += (const vector_type& p)
	{
		Begin += p;
		End += p;
		return *this;
	}
	Line& operator -= (const vector_type& p)
	{
		Begin -= p;
		End -= p;
		return *this;
	}
	Line& operator *= (T s)
	{
		Begin *= s;
		End *= s;
		return *this;
	}
	Line& operator /= (T s)
	{
		Begin /= s;
		End /= s;
		return *this;
	}
};

template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Up(vector_type::Up);
template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Down(vector_type::Down);
template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Right(vector_type::Right);
template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Left(vector_type::Left);
template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Front(vector_type::Front);
template <typename T, template <typename> class VectorN>
const Line<T, VectorN> Line<T, VectorN>::Back(vector_type::Back);

template <typename T, template <typename> class VectorN> inline
Ray<T, VectorN>::Ray(const Line<T, VectorN>& l) :
	Position(l.Begin), Direction(l.direction())
{

}

template <typename T, template <typename> class VectorN> inline
Ray<T, VectorN>& Ray<T, VectorN>::from_line(const Line<T, VectorN>& l)
{
	Position = l.Begin;
	l.direction(Direction);
	return *this;
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Line<T, VectorN>& v)
{
	// (Begin, End)
	os << out_char::parentheses_left << v.Begin << out_char::comma_space << v.End << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Line<T, VectorN>& v)
{
	is.ignore();
	is >> v.Begin;
	is.ignore();
	is >> v.End;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Line<T, VectorN>& v)
{
	os << out_char::parentheses_left << v.Begin << out_char::comma_space << v.End << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Line<T, VectorN>& v)
{
	is.ignore();
	is >> v.Begin;
	is.ignore();
	is >> v.End;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __MATH_LINE_H__
