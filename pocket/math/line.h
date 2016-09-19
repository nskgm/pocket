#ifndef __POCKET_MATH_LINE_H__
#define __POCKET_MATH_LINE_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../behavior.h"
#include "math_traits.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "ray.h"
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename, template <typename> class> struct line;
template <typename> struct plane;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef line<float, vector2> line2f;
typedef line<float, vector3> line3f;
typedef line<float, vector4> line4f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef line<double, vector2> line2d;
typedef line<double, vector3> line3d;
typedef line<double, vector4> line4d;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef line<long double, vector2> line2ld;
typedef line<long double, vector3> line3ld;
typedef line<long double, vector4> line4ld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T, template <typename> class VectorN>
struct line
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;
	typedef VectorN<T> vector_type;
	typedef ray<T, VectorN> ray_type;
	typedef plane<T> plane_type;

	typedef container::array<vector_type, 2> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	//-----------------------------------------------------------------------------------------
	// Members
	//-----------------------------------------------------------------------------------------

	vector_type begin; // 開始位置
	vector_type end; // 終了位置

	template <typename, template <typename> class> friend struct line;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const line up; // [0.0, 0.0, 0.0], [0.0, 1.0, 0.0]
	static const line down; // [0.0, 0.0, 0.0], [0.0, -1.0, 0.0]
	static const line right; // [0.0, 0.0, 0.0], [1.0, 0.0, 0.0]
	static const line left; // [0.0, 0.0, 0.0], [-1.0, 0.0, 0.0]
	static const line forward; // [0.0, 0.0, 0.0], [0.0, 0.0, 1.0]
	static const line backward; // [0.0, 0.0, 0.0], [0.0, 0.0, -1.0]

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(line);
	explicit line(const behavior::_noinitialize_t&)
	{}
	explicit line(const vector_type& e) :
		begin(vector_type::zero), end(e)
	{}
	line(const vector_type& b, const vector_type& e) :
		begin(b), end(e)
	{}
	explicit line(const ray_type& r, T length)
	{
		from_ray(r, length);
	}

	//-----------------------------------------------------------------------------------------
	// Funstions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// レイから作成
	//---------------------------------------------------------------------
	line& from_ray(const ray_type& r, T length)
	{
		begin = r.origin;
		r.point(length, end);
		return *this;
	}

	//---------------------------------------------------------------------
	// 線分開始地点から終了地点の方向を求める
	//---------------------------------------------------------------------
	vector_type direction() const
	{
		return begin.direction(end);
	}
	vector_type& direction(vector_type& result) const
	{
		return begin.direction(end, result);
	}

	//---------------------------------------------------------------------
	// 線分の距離を求める
	//---------------------------------------------------------------------
	T distance() const
	{
		return begin.distance(end);
	}

	//---------------------------------------------------------------------
	// 線分が平面と交差しているか
	//---------------------------------------------------------------------
	bool intersect(const plane_type&) const; // plane.h

	//-----------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const line& l) const
	{
		return begin == l.begin && end == l.end;
	}
	bool operator != (const line& l) const
	{
		return !(*this == l);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	line operator + () const
	{
		return *this;
	}
	line operator - () const
	{
		return line(-begin, -end);
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	line operator + (const vector_type& p) const
	{
		// 渡されてきた座標分どちらもずらす
		return line(begin + p, end + p);
	}
	line operator - (const vector_type& p) const
	{
		return line(begin - p, end - p);
	}
	line operator * (T s) const
	{
		return line(begin * s, end * s);
	}
	line operator / (T s) const
	{
		return line(begin / s, end / s);
	}

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	line& operator += (const vector_type& p)
	{
		begin += p;
		end += p;
		return *this;
	}
	line& operator -= (const vector_type& p)
	{
		begin -= p;
		end -= p;
		return *this;
	}
	line& operator *= (T s)
	{
		begin *= s;
		end *= s;
		return *this;
	}
	line& operator /= (T s)
	{
		begin /= s;
		end /= s;
		return *this;
	}
};

template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::up(vector_type::up);
template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::down(vector_type::down);
template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::right(vector_type::right);
template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::left(vector_type::left);
template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::forward(vector_type::forward);
template <typename T, template <typename> class VectorN>
const line<T, VectorN> line<T, VectorN>::backward(vector_type::backward);

template <typename T, template <typename> class VectorN> inline
ray<T, VectorN>::ray(const line<T, VectorN>& l) :
	origin(l.begin), direction(l.direction())
{}

template <typename T, template <typename> class VectorN> inline
ray<T, VectorN>& ray<T, VectorN>::from_line(const line<T, VectorN>& l)
{
	origin = l.begin;
	l.direction(direction);
	return *this;
}

template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const line<T, VectorN>& v)
{
	// (begin, end)
	os << io::parentheses_left << v.begin << io::comma_space << v.end << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, line<T, VectorN>& v)
{
	is.ignore();
	is >> v.begin;
	is.ignore();
	is >> v.end;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_LINE_H__
