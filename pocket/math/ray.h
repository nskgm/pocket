#ifndef __POCKET_MATH_RAY_H__
#define __POCKET_MATH_RAY_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../debug.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename, template <typename> class> struct ray;
template <typename, template <typename> class> struct line;
template <typename> struct plane;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef ray<float, vector2> ray2f;
typedef ray<float, vector3> ray3f;
typedef ray<float, vector4> ray4f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef ray<double, vector2> ray2d;
typedef ray<double, vector3> ray3d;
typedef ray<double, vector4> ray4d;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef ray<long double, vector2> ray2ld;
typedef ray<long double, vector3> ray3ld;
typedef ray<long double, vector4> ray4ld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T, template <typename> class VectorN>
struct ray
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;
	typedef VectorN<T> vector_type;
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

	vector_type origin; // 基点
	vector_type direction; // 方向

	template <typename, template <typename> class> friend struct ray;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const ray up; // [0.0, 0.0, 0.0], [0.0, 1.0, 0.0]
	static const ray down; // [0.0, 0.0, 0.0], [0.0, -1.0, 0.0]
	static const ray right; // [0.0, 0.0, 0.0], [1.0, 0.0, 0.0]
	static const ray left; // [0.0, 0.0, 0.0], [-1.0, 0.0, 0.0]
	static const ray forward; // [0.0, 0.0, 0.0], [0.0, 0.0, 1.0]
	static const ray backward; // [0.0, 0.0, 0.0], [0.0, 0.0, -1.0]

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(ray);
	explicit ray(const behavior::_noinitialize_t&)
	{}
	explicit ray(const vector_type& dir) :
		origin(vector_type::zero),
		direction(dir)
	{
		direction.normalize();
	}
	ray(const vector_type& p, const vector_type& d) :
		origin(p), direction(d)
	{
		direction.normalize();
	}

	explicit ray(const line<T, VectorN>&); // line.h

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 座標と注視点から求める
	//---------------------------------------------------------------------
	ray& from_lookat(const vector_type& position, const vector_type& center)
	{
		origin = position;
		position.direction(center, direction);
		return *this;
	}
	//---------------------------------------------------------------------
	// 線分から求める
	//---------------------------------------------------------------------
	ray& from_line(const line<T, VectorN>&); // line.h

	//---------------------------------------------------------------------
	// 座標を求める
	//---------------------------------------------------------------------
	vector_type point(T s) const
	{
		return origin + direction * s;
	}
	vector_type& point(T s, vector_type& result) const
	{
		result = origin;
		result += direction * s;
		return result;
	}

	//---------------------------------------------------------------------
	// 正規化
	//---------------------------------------------------------------------
	ray& normalize()
	{
		direction.normalize();
		return *this;
	}
	//---------------------------------------------------------------------
	// 平面と交差しているか
	//---------------------------------------------------------------------
	bool intersect(const plane_type&) const; // plane.h

	//-----------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const ray& r) const
	{
		return origin == r.origin && direction == r.direction;
	}
	bool operator != (const ray& r) const
	{
		return !(*this == r);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	ray operator + () const
	{
		return *this;
	}
	ray operator - () const
	{
		// 反対の方向を示す
		return ray(origin, -direction);
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	ray operator + (const vector_type& p) const
	{
		// 座標のみ合わせる
		return ray(origin + p, direction);
	}
	ray operator + (const ray& r) const
	{
		// 座標は足し、方向は中間を求める
		vector3<T> d = direction + r.direction;
		d.normalize();
		return ray(origin + r.origin, d);
	}
	ray operator - (const vector_type& p) const
	{
		return ray(origin - p, direction);
	}
	ray operator - (const ray& r) const
	{
		// 座標は引き、方向は逆の中間を求める
		vector3<T> d = direction - r.direction;
		d.normalize();
		return ray(origin - r.origin, d);
	}
	ray operator * (T s) const
	{
		// 座標へかける
		return ray(origin * s, direction);
	}
	ray operator / (T s) const
	{
		return ray(origin / s, direction);
	}

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	ray& operator += (const vector_type& p)
	{
		origin += p;
		// directionはなし
		return *this;
	}
	ray& operator += (const ray& r)
	{
		origin += r.origin;
		direction += r.direction;
		direction.normalize();
		return *this;
	}
	ray& operator -= (const vector_type& p)
	{
		origin -= p;
		return *this;
	}
	ray& operator -= (const ray& r)
	{
		origin -= r.origin;
		direction -= r.direction;
		direction.normalize();
		return *this;
	}
	ray& operator *= (T s)
	{
		origin *= s;
		return *this;
	}
	ray& operator /= (T s)
	{
		origin /= s;
		return *this;
	}
};

template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::up(vector_type::up);
template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::down(vector_type::down);
template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::right(vector_type::right);
template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::left(vector_type::left);
template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::forward(vector_type::forward);
template <typename T, template <typename> class VectorN>
const ray<T, VectorN> ray<T, VectorN>::backward(vector_type::backward);

template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const ray<T, VectorN>& v)
{
	// (origin, direction)
	os << io::parentheses_left << v.origin << io::comma_space << v.direction << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, template <typename> class VectorN> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, ray<T, VectorN>& v)
{
	is.ignore();
	is >> v.origin;
	is.ignore();
	is >> v.direction;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_RAY_H__
