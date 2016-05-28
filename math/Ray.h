#ifndef __MATH_RAY_H__
#define __MATH_RAY_H__

#pragma once

#include "Vector2.h"
#include "Vector3.h"
#ifdef _USING_MATH_IO
#include <iostream>
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename, template <typename> class> struct Ray;
template <typename, template <typename> class> struct Line;
template <typename> struct Plane;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Ray<float, Vector2> Ray2f;
typedef Ray<float, Vector3> Ray3f;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Ray<double, Vector2> Ray2d;
typedef Ray<double, Vector3> Ray3d;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Ray<long double, Vector2> Ray2ld;
typedef Ray<long double, Vector3> Ray3ld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T, template <typename> class VN>
using ray = Ray<T, VN>;
#ifndef _UNUSING_MATH_INT_FLOAT
using ray2f = ray<float, Vector2>;
using ray3f = ray<float, Vector3>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using ray2d = ray<double, Vector2>;
using ray3d = ray<double, Vector3>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using ray2ld = ray<long double, Vector2>;
using ray3ld = ray<long double, Vector3>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

template <typename T, template <typename> class VectorN>
struct Ray
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

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

	vector_type Position; /* 基点 */
	vector_type Direction; /* 方向 */

	template <typename, template <typename> class> friend struct Ray;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Ray Up; /* [0.0, 0.0, 0.0], [0.0, 1.0, 0.0] */
	static const Ray Down; /* [0.0, 0.0, 0.0], [0.0, -1.0, 0.0] */
	static const Ray Right; /* [0.0, 0.0, 0.0], [1.0, 0.0, 0.0] */
	static const Ray Left; /* [0.0, 0.0, 0.0], [-1.0, 0.0, 0.0] */
	static const Ray Front; /* [0.0, 0.0, 0.0], [0.0, 0.0, 1.0] */
	static const Ray Back; /* [0.0, 0.0, 0.0], [0.0, 0.0, -1.0] */

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Ray);
	explicit Ray(const behavior::_noinitialize_t&)
	{

	}
	explicit Ray(const vector_type& dir) :
		Position(vector_type::Zero),
		Direction(dir)
	{

	}
	Ray(const vector_type& p, const vector_type& d) :
		Position(p), Direction(d)
	{

	}

	explicit Ray(const Line<T, VectorN>&); /* Line.h */

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 座標と注視点から求める
	*---------------------------------------------------------------------*/
	Ray& from_lookat(const vector_type& position, const vector_type& center)
	{
		Position = position;
		center.direction(position, Direction);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 線分から求める
	*---------------------------------------------------------------------*/
	Ray& from_line(const Line<T, VectorN>&); /* Line.h */

	/*---------------------------------------------------------------------
	* 座標を求める
	*---------------------------------------------------------------------*/
	vector_type point(T s) const
	{
		return Position + Direction * s;
	}
	vector_type& point(T s, vector_type& result) const
	{
		result = Position;
		result += Direction * s;
		return result;
	}

	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Ray& normalize()
	{
		Direction.normalize();
		return *this;
	}
	/*---------------------------------------------------------------------
	* 平面と交差しているか
	*---------------------------------------------------------------------*/
	bool is_intersect(const Plane<T>&) const; /* Plane.h */

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Ray& r) const
	{
		return Position == r.Position && Direction == r.Direction;
	}
	bool operator != (const Ray& r) const
	{
		return !(*this == r);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Ray operator + () const
	{
		return *this;
	}
	Ray operator - () const
	{
		/* 反対の方向を示す */
		return Ray(Position, -Direction);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Ray operator + (const vector_type& p) const
	{
		/* 座標のみ合わせる */
		return Ray(Position + p, Direction);
	}
	Ray operator + (const Ray& r) const
	{
		/* 座標は足し、方向は中間を求める */
		Vector3<T> d = Direction + r.Direction;
		d.normalize();
		return Ray(Position + r.Position, d);
	}
	Ray operator - (const vector_type& p) const
	{
		return Ray(Position - p, Direction);
	}
	Ray operator - (const Ray& r) const
	{
		/* 座標は引き、方向は逆の中間を求める */
		Vector3<T> d = Direction - r.Direction;
		d.normalize();
		return Ray(Position - r.Position, d);
	}
	Ray operator * (T s) const
	{
		/* 座標へかける */
		return Ray(Position * s, Direction);
	}
	Ray operator / (T s) const
	{
		return Ray(Position / s, Direction);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Ray& operator += (const vector_type& p)
	{
		Position += p;
		/* Directionはなし */
		return *this;
	}
	Ray& operator += (const Ray& r)
	{
		Position += r.Position;
		Direction += r.Direction;
		Direction.normalize();
		return *this;
	}
	Ray& operator -= (const vector_type& p)
	{
		Position -= p;
		return *this;
	}
	Ray& operator -= (const Ray& r)
	{
		Position -= r.Position;
		Direction -= r.Direction;
		Direction.normalize();
		return *this;
	}
	Ray& operator *= (T s)
	{
		Position *= s;
		return *this;
	}
	Ray& operator /= (T s)
	{
		Position /= s;
		return *this;
	}
};

template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Up(vector_type::Up);
template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Down(vector_type::Down);
template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Right(vector_type::Right);
template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Left(vector_type::Left);
template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Front(vector_type::Front);
template <typename T, template <typename> class VectorN>
const Ray<T, VectorN> Ray<T, VectorN>::Back(vector_type::Back);

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Ray<T, VectorN>& v)
{
	/* (Position, Direction) */
	os << out_char::parentheses_left << v.Position << out_char::comma_space << v.Direction << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Ray<T, VectorN>& v)
{
	is.ignore();
	is >> v.Position;
	is.ignore();
	is >> v.Direction;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Ray<T, VectorN>& v)
{
	os << out_char::parentheses_left << v.Position << out_char::comma_space << v.Direction << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Ray<T, VectorN>& v)
{
	is.ignore();
	is >> v.Position;
	is.ignore();
	is >> v.Direction;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */


#endif /* __MATH_RAY_H__ */
