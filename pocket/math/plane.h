#ifndef __POCKET_MATH_PLANE_H__
#define __POCKET_MATH_PLANE_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector3.h"
#include "vector4.h"
#include "line.h"
#include "ray.h"
#ifdef POCKET_USE_SIMD_ANONYMOUS
#include "simd_traits.h"
#endif // POCKET_USE_SIMD_ANONYMOUS
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct plane;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef plane<float> planef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef plane<double> planed;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef plane<long double> planeld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct plane
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;
	typedef vector3<T> notmal_type;
	typedef vector3<T> point_type;
	typedef line<T, vector3> line3_type;
	typedef line<T, vector4> line4_type;
	typedef ray<T, vector3> ray3_type;
	typedef ray<T, vector4> ray4_type;

	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef simd_traits<T> simd;
	typedef typename simd::type simd_type;
#endif // POCKET_USE_SIMD_ANONYMOUS

	enum intersect_result
	{
		on_forward, // 前面
		on_backward, // 背面
		on_plane // 平面上
	};

	//-----------------------------------------------------------------------------------------
	// Members
	//-----------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			T a; // 法線x成分
			T b; // 法線y成分
			T c; // 法線z成分
			T d; // 原点からの距離

#ifdef POCKET_USE_ANONYMOUS
		};
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		struct
		{
			vector3<T> n; // 法線
			T distance;
		};
#endif // POCKET_USE_ANONYMOUS_NON_POD

#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type mm;
#endif // POCKET_USE_SIMD_ANONYMOUS

		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const plane up; // 0.0, 1.0, 0.0, 0.0
	static const plane down; // 0.0, -1.0, 0.0, 0.0
	static const plane right; // 1.0, 0.0, 0.0, 0.0
	static const plane left; // -1.0, 0.0, 0.0, 0.0
	static const plane forward; // 0.0, 0.0, 1.0, 0.0
	static const plane backward; // 0.0, 0.0, -1.0, 0.0

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(plane);
	explicit plane(const behavior::_noinitialize_t&)
	{

	}
	explicit plane(const behavior::_right_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::one, math_type::zero, math_type::zero, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(math_type::one), b(math_type::zero), c(math_type::zero),
#	else
		n(math_type::one, math_type::zero, math_type::zero),
#	endif
		d(-d)
#endif
	{

	}
	explicit plane(const behavior::_left_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(-math_type::one, math_type::zero, math_type::zero, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(-math_type::one), b(math_type::zero), c(math_type::zero),
#	else
		n(-math_type::one, math_type::zero, math_type::zero),
#	endif
		d(-d)
#endif
	{

	}
	explicit plane(const behavior::_up_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::zero, math_type::one, math_type::zero, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(math_type::zero), b(math_type::one), c(math_type::zero),
#	else
		n(math_type::zero, math_type::one, math_type::zero),
#	endif
		d(-d)
#endif
	{

	}
	explicit plane(const behavior::_down_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::zero, -math_type::one, math_type::zero, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(math_type::zero), b(-math_type::one), c(math_type::zero),
#	else
		n(math_type::zero, -math_type::one, math_type::zero),
#	endif
		d(-d)
#endif
	{

	}
	explicit plane(const behavior::_front_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::zero, math_type::zero, math_type::one, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(math_type::zero), b(math_type::zero), c(math_type::one),
#	else
		n(math_type::zero, math_type::zero, math_type::one),
#	endif
		d(-d)
#endif
	{

	}
	explicit plane(const behavior::_back_t&, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::zero, math_type::zero, -math_type::one, -d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(math_type::zero), b(math_type::zero), c(-math_type::one),
#	else
		n(math_type::zero, math_type::zero, -math_type::one),
#	endif
		d(-d)
#endif
	{

	}
	plane(T a, T b, T c, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(a, b, c, d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(a), b(b), c(c),
#	else
		n(a, b, c),
#	endif
		d(d)
#endif
	{

	}
	plane(const vector3<T>& n, T d) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(n.x, n.y, n.z, d))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		a(n.x), b(n.y), c(n.z),
#	else
		n(n),
#	endif
		d(d)
#endif
	{

	}
	plane(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
	{
		from_points(v0, v1, v2);
	}
	plane(const vector4<T>& v0, const vector4<T>& v1, const vector4<T>& v2)
	{
		from_points(v0, v1, v2);
	}
	plane(const vector3<T>& nrm, const vector3<T>& p)
	{
		from_normal_point(nrm, p);
	}
	plane(const vector4<T>& nrm, const vector4<T>& p)
	{
		from_normal_point(nrm, p);
	}
#ifdef POCKET_USE_SIMD_ANONYMOUS
	plane(simd_type mm) :
		mm(mm)
	{

	}
#endif // POCKET_USE_SIMD_ANONYMOUS

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 値が近いか
	//---------------------------------------------------------------------
	bool is_near(const plane& p) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal(mm, p.mm);
#else
		return (math_type::is_near(a, p.a) && math_type::is_near(b, p.b) && math_type::is_near(c, p.c) && math_type::is_near(d, p.d));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	bool is_near_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal_zero(mm);
#else
		return (math_type::is_near_zero(a) && math_type::is_near_zero(b) && math_type::is_near_zero(c) && math_type::is_near_zero(d));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	//---------------------------------------------------------------------
	// 値がゼロか
	//---------------------------------------------------------------------
	bool is_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::equal(mm, simd::zero());
#else
		return (a == math_type::zero && b == math_type::zero && c == math_type::zero && d == math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 3つの点から求める（0～2：反時計回り）
	//---------------------------------------------------------------------
	plane& from_points(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
	{
		// v0からv1へのベクトル
		vector3<T> d1 = v1 - v0;
		// v1からv2へのベクトル
		vector3<T> d2 = v2 - v0;
		// 二つのベクトルから外積と基点となるv0を渡す
		return from_normal_point(d1.cross(d2), v0);
	}
	plane& from_points(const vector4<T>& v0, const vector4<T>& v1, const vector4<T>& v2)
	{
		vector4<T> d1 = v1 - v0;
		vector4<T> d2 = v2 - v0;
		return from_normal_point(d1.cross(d2), v0);
	}
	//---------------------------------------------------------------------
	// 法線と座標から求める
	//---------------------------------------------------------------------
	plane& from_normal_point(const vector3<T>& normal, const vector3<T>& p)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::normalize(simd::set(normal.x, normal.y, normal.z, math_type::zero));
		simd_type md = simd::negate(simd::dot(m, simd::set(p.x, p.y, p.z, math_type::zero)));
		mm = simd::select1110(m, md);
#else
		a = normal.x;
		b = normal.y;
		c = normal.z;
		// 正規化されていないことを考慮して渡されてきた法線ベクトルを正規化
		T l = normal_length_sq();
		if (l > math_type::zero)
		{
			l = math_type::rsqrt(l);
			a *= l;
			b *= l;
			c *= l;
		}
		// dotという名の平面の方程式から求められる値, [ax + by + cz + d] -> [d = -(ax + by + cz)]
		// [a b c] = n
		d = -dot_normal(p);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	plane& from_normal_point(const vector4<T>& normal, const vector4<T>& p)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::normalize(normal.mm);
		simd_type md = simd::negate(simd::dot(m, p.mm));
		mm = simd::select1110(m, md);
#else
		a = normal.x;
		b = normal.y;
		c = normal.z;
		T l = normal_length_sq();
		if (l > math_type::zero)
		{
			l = math_type::rsqrt(l);
			a *= l;
			b *= l;
			c *= l;
		}
		d = -dot_normal(p);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	//---------------------------------------------------------------------
	// 内積を求める
	//---------------------------------------------------------------------
	T dot(const vector4<T>& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::first(simd::dot(mm, v.mm));
#else
		return a * v.x + b * v.y + c * v.z + d * v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	//---------------------------------------------------------------------
	// 法線との内積を求める
	//---------------------------------------------------------------------
	T dot_normal(const vector3<T>& v) const
	{
		return normal_dot(v);
	}
	//---------------------------------------------------------------------
	// 内積を求める（W=1）
	//---------------------------------------------------------------------
	T dot_coord(const vector3<T>& v) const
	{
		return dot_normal(v) + d;
	}
	//---------------------------------------------------------------------
	// 正規化
	//---------------------------------------------------------------------
	plane& normalize()
	{
		// 法線の長さでdも正規化を行う
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type r = simd::dot3(mm, mm);
		r = simd::rsqrt(r);
		mm = simd::mul(mm, r);
#else
		T length = normal_length_sq();
		if (length > math_type::zero)
		{
			length = math_type::rsqrt(length);
			a *= length;
			b *= length;
			c *= length;
			d *= length;
		}
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	plane& normalize(plane& result) const
	{
		result = *this;
		return result.normalize();
	}
	//---------------------------------------------------------------------
	// 正規化を求める
	//---------------------------------------------------------------------
	plane normalized() const
	{
		plane r = *this;
		return r.normalize();
	}

	//---------------------------------------------------------------------
	// 平面が置かれている座標を求める
	//---------------------------------------------------------------------
	vector3<T> point3() const
	{
		// dは負の値になっているので反転
		T dd = -d;
		return vector3<T>(a*dd, b*dd, c*dd);
	}
	vector4<T> point4() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::mul(mm, -d);
		return vector4<T>(simd::select1110(m, simd::one()));
#else
		T dd = -d;
		return vector4<T>(a*dd, b*dd, c*dd, math_type::one);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector3<T>& point(vector3<T>& result) const
	{
		result = point3();
		return result;
	}
	vector4<T>& point(vector4<T>& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::mul(mm, -d);
		result.mm = simd::select1110(m, simd::one());
#else
		result = point4();
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	//---------------------------------------------------------------------
	// 線形補間
	//---------------------------------------------------------------------
	plane lerp(const plane& to, T t) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return plane(simd::lerp(mm, to.mm, t));
#else
		return plane(math_type::lerp(a, to.a, t),
			math_type::lerp(b, to.b, t),
			math_type::lerp(c, to.c, t),
			math_type::lerp(d, to.d, t)
		);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	plane& lerp(const plane& to, T t, plane& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::lerp(mm, to.mm, t);
#else
		result.a = math_type::lerp(a, to.a, t);
		result.b = math_type::lerp(b, to.b, t);
		result.c = math_type::lerp(c, to.c, t);
		result.d = math_type::lerp(d, to.d, t);
#endif
		return result;
	}
	plane& lerp(const plane& from, const plane& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	//---------------------------------------------------------------------
	// 交差判定（型で判定）
	//---------------------------------------------------------------------
	bool is_intersect(const line3_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	bool is_intersect(const line4_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	bool is_intersect(const ray3_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	bool is_intersect(const ray4_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	//---------------------------------------------------------------------
	// 線分との交差状態か
	//---------------------------------------------------------------------
	bool is_intersect_line(const vector3<T>& begin, const vector3<T>& end) const
	{
#if 0
		// 始点から終点へのベクトルを求める
		vector3<T> dir(behavior::noinitialize);
		end.Direction(begin, dir);

		// 法線ベクトルとの内積が0の場合は垂直になっているので交差していない
		if (math_type::is_near_zero(normal.dot(dir)))
		{
			// 交差なし
			return false;
		}

		// 法線と線分の始点終点の射影を求める（内積）
		// これで単位ベクトルからの長さが求まる
		T b = normal.dot(begin);
		T e = normal.dot(end);

		// 射影で求めた間に存在しているか, dを基準点として考える
		//return ((b - d) >= math_type::zero) && ((e - d) <= math_type::zero);
		return (b * e) <= math_type::zero; // 同符号の場合は衝突していない, 表裏関係なし
#else
		// 同符号の場合は交差していない
		return dot_coord(begin) * dot_coord(end) <= math_type::zero;
#endif
	}
	bool is_intersect_line(const line3_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	bool is_intersect_line(const vector4<T>& begin, const vector4<T>& end) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m1 = simd::dot(mm, begin.mm);
		simd_type m2 = simd::dot(mm, end.mm);
		return !simd::greater(simd::zero(), simd::mul(m1, m2));
#else
		return dot(begin) * dot(end) <= math_type::zero;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool is_intersect_line(const line4_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	//---------------------------------------------------------------------
	// 無限線分と交差状態か
	//---------------------------------------------------------------------
	bool is_intersect_ray(const vector3<T>& position, const vector3<T>& direction) const
	{
		// 法線との内積を行って0じゃなければいずれ交差する
		static_cast<void>(position);
		return normal_dot(direction) != math_type::zero;
	}
	bool is_intersect_ray(const ray3_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	bool is_intersect_ray(const vector4<T>& position, const vector4<T>& direction) const
	{
		// 法線との内積を行って0じゃなければいずれ交差する
		static_cast<void>(position);
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::not_equal(simd::dot(mm, direction.mm), simd::zero());
#else
		return dot(direction) != math_type::zero;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool is_intersect_ray(const ray4_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	//---------------------------------------------------------------------
	// 座標との交差状態を求める
	//---------------------------------------------------------------------
	intersect_result intersect_point(const vector3<T>& point) const
	{
		// 法線上の点との距離を計算
		T distance = dot_coord(point);

		if (distance >= math_type::epsilon)
		{
			return on_forward;
		}
		if (distance <= -math_type::epsilon)
		{
			return on_backward;
		}
		return on_plane;
	}
	bool is_intersect_point(const vector3<T>& point) const
	{
		return intersect_point(point) == on_plane;
	}
	intersect_result intersect_point(const vector4<T>& point) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::dot(mm, point.mm);
		simd_type ep = simd::set(math_type::epsilon);
		if (simd::greater_equal(m, ep))
		{
			return on_forward;
		}
		if (simd::less_equal(m, simd::negate(ep)))
		{
			return on_backward;
		}
		return on_plane;
#else
		T distance = dot(point);
		if (distance >= math_type::epsilon)
		{
			return on_forward;
		}
		if (distance <= -math_type::epsilon)
		{
			return on_backward;
		}
		return on_plane;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool is_intersect_point(const vector4<T>& point) const
	{
		return intersect_point(point) == on_plane;
	}
	//---------------------------------------------------------------------
	// 球との交差状態を求める
	//---------------------------------------------------------------------
	intersect_result intersect_sphere(const vector3<T>& center, T radius) const
	{
		T distance = dot_coord(center);

		// 半径より小さかったら平面上に存在している
		if (math_type::abs(distance) <= radius)
		{
			return on_plane;
		}
		// 半径より大きい場合は前面
		if (distance > radius)
		{
			return on_forward;
		}
		return on_backward;
	}
	bool is_intersect_sphere(const vector3<T>& center, T radius) const
	{
		return intersect_sphere(center, radius) == on_plane;
	}
	intersect_result intersect_sphere(const vector4<T>& center, T radius) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type m = simd::dot(mm, center.mm);
		simd_type r = simd::set(radius);
		if (simd::less_equal(simd::abs(m), r))
		{
			return on_plane;
		}
		if (simd::greater(m, r))
		{
			return on_forward;
		}
		return on_backward;
#else
		T distance = dot(center);
		if (math_type::abs(distance) <= radius)
		{
			return on_plane;
		}
		if (distance > radius)
		{
			return on_forward;
		}
		return on_backward;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool is_intersect_sphere(const vector4<T>& center, T radius) const
	{
		return intersect_sphere(center, radius) == on_plane;
	}


	//---------------------------------------------------------------------
	// 法線補助
	//---------------------------------------------------------------------
	T normal_dot(T pa, T pb, T pc) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::first(simd::dot3(mm, simd::set(pa, pb, pc, math_type::zero)));
#else
		return a * pa + b * pb + c * pc;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	T normal_dot(const vector3<T>& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::first(simd::dot3(mm, simd::set(v.x, v.y, v.z, math_type::zero)));
#else
		return a * v.x + b * v.y + c * v.z;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	T normal_length_sq() const
	{
		return normal_dot(a, b, c);
	}

	T normal_length() const
	{
		return math_type::sqrt(normal_length_sq());
	}

	vector3<T>& normal()
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return n;
#else
		return reinterpret_cast<vector3<T>&>(a);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& normal() const
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return n;
#else
		return reinterpret_cast<const vector3<T>&>(a);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	plane& normal(const vector3<T>& v)
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		n = v;
#else
		a = v.x;
		b = v.y;
		c = v.z;
#endif // POCKET_USE_ANONYMOUS_NON_POD
		return *this;
	}

	//-----------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// アクセス演算子
	//---------------------------------------------------------------------
	T& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&a)[i];
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&a)[i];
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}

	//---------------------------------------------------------------------
	// 型変換演算子
	//---------------------------------------------------------------------
	template <typename U>
	POCKET_CXX11_EXPLICIT operator plane<U>() const
	{
		return plane<U>(static_cast<U>(a), static_cast<U>(b), static_cast<U>(c), static_cast<U>(d));
	}
	POCKET_CXX11_EXPLICIT operator T* ()
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &a;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	POCKET_CXX11_EXPLICIT operator const T* () const
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &a;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const plane& p) const
	{
		return a == p.a && b == p.b && c == p.c && d == p.d;
	}
	bool operator != (const plane& p) const
	{
		return !(*this == p);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	plane operator + () const
	{
		return *this;
	}
	plane operator - () const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return plane(simd::negate(mm));
#else
		return plane(-a, -b, -c, -d);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	plane operator + (T d)
	{
		return plane(a, b, c, d - d);
	}
	plane operator - (T d)
	{
		return plane(a, b, c, d + d);
	}
	plane operator * (T s)
	{
		return plane(a, b, c, d * s);
	}
	plane operator / (T s)
	{
		return plane(a, b, c, d / s);
	}

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
	plane& operator = (const behavior::_right_t&)
	{
		normal() = vector3<T>::right;
		return *this;
	}
	plane& operator = (const behavior::_left_t&)
	{
		normal() = vector3<T>::left;
		return *this;
	}
	plane& operator = (const behavior::_up_t&)
	{
		normal() = vector3<T>::up;
		return *this;
	}
	plane& operator = (const behavior::_down_t&)
	{
		normal() = vector3<T>::down;
		return *this;
	}
	plane& operator = (const behavior::_front_t&)
	{
		normal() = vector3<T>::forward;
		return *this;
	}
	plane& operator = (const behavior::_back_t&)
	{
		normal() = vector3<T>::backward;
		return *this;
	}

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	plane& operator += (T d)
	{
		// 距離のみを変化させる
		d -= d;
		return *this;
	}
	plane& operator -= (T d)
	{
		d += d;
		return *this;
	}
	plane& operator *= (T s)
	{
		d *= s;
		return *this;
	}
	plane& operator /= (T s)
	{
		d /= s;
		return *this;
	}

	//------------------------------------------------------------------------------------------
	// タグでの関数呼び出し
	//------------------------------------------------------------------------------------------

	plane& operator () (const behavior::_right_t&)
	{
		normal() = vector3<T>::right;
		return *this;
	}
	plane& operator () (const behavior::_left_t&)
	{
		normal() = vector3<T>::left;
		return *this;
	}
	plane& operator () (const behavior::_up_t&)
	{
		normal() = vector3<T>::up;
		return *this;
	}
	plane& operator () (const behavior::_down_t&)
	{
		normal() = vector3<T>::down;
		return *this;
	}
	plane& operator () (const behavior::_front_t&)
	{
		normal() = vector3<T>::forward;
		return *this;
	}
	plane& operator () (const behavior::_back_t&)
	{
		normal() = vector3<T>::backward;
		return *this;
	}
	plane operator () (const behavior::_add_t&, T f) const
	{
		return operator+(f);
	}
	plane operator () (const behavior::_sub_t&, T f) const
	{
		return operator-(f);
	}
	plane operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	plane operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}

	plane& operator () (const behavior::_add_assign_t&, T f)
	{
		return operator+=(f);
	}
	plane& operator () (const behavior::_sub_assign_t&, T f)
	{
		return operator-=(f);
	}
	plane& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	plane& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	T& operator () (const behavior::_at_t&, int i)
	{
		return operator[](i);
	}
	const T& operator () (const behavior::_at_t&, int i) const
	{
		return operator[](i);
	}
	T* operator () (const behavior::_pointer_t&)
	{
		return operator T*();
	}
	const T* operator () (const behavior::_pointer_t&) const
	{
		return operator const T*();
	}

	bool operator () (const behavior::_equal_t&, const plane& q) const
	{
		return operator==(q);
	}
	bool operator () (const behavior::_not_equal_t&, const plane& q) const
	{
		return operator!=(q);
	}
	bool operator () (const behavior::_near_t&, const plane& q) const
	{
		return is_near(q);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return is_near_zero();
	}

	plane& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	plane operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	plane operator () (const behavior::_lerp_t&, const plane& p, T t) const
	{
		return lerp(p, t);
	}

	T operator () (const behavior::_dot_t&, const vector4<T>& v) const
	{
		return dot(v);
	}
	T operator () (const behavior::_dot_normal_t&, const vector3<T>& v) const
	{
		return dot_normal(v);
	}
	T operator () (const behavior::_dot_coord_t&, const vector3<T>& v) const
	{
		return dot_coord(v);
	}
	vector3<T> operator () (const behavior::_point_t&) const
	{
		return point();
	}
	bool operator () (const behavior::_intersect_t&, const ray<T, vector3>& r) const
	{
		return is_intersect_ray(r);
	}
	bool operator () (const behavior::_intersect_t&, const line<T, vector3>& l) const
	{
		return is_intersect_line(l);
	}
	bool operator () (const behavior::_intersect_ray_t&, const ray<T, vector3>& r) const
	{
		return is_intersect_ray(r);
	}
	bool operator () (const behavior::_intersect_ray_t&, const vector3<T>& position, const vector3<T>& direction) const
	{
		return is_intersect_ray(position, direction);
	}
	bool operator () (const behavior::_intersect_line_t&, const line<T, vector3>& l) const
	{
		return is_intersect_line(l);
	}
	bool operator () (const behavior::_intersect_line_t&, const vector3<T>& begin, const vector3<T>& end) const
	{
		return is_intersect_line(begin, end);
	}
};

template <typename T>
const plane<T> plane<T>::up(math_type::zero, math_type::one, math_type::zero, math_type::zero);
template <typename T>
const plane<T> plane<T>::down(math_type::zero, -math_type::one, math_type::zero, math_type::zero);
template <typename T>
const plane<T> plane<T>::right(math_type::one, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const plane<T> plane<T>::left(-math_type::one, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const plane<T> plane<T>::forward(math_type::zero, math_type::zero, math_type::one, math_type::zero);
template <typename T>
const plane<T> plane<T>::backward(math_type::zero, math_type::zero, -math_type::one, math_type::zero);

template <typename T, template <typename> class VectorN> inline
bool line<T, VectorN>::is_intersect(const plane<T>& p) const
{
	return p.is_intersect_line(begin, end);
}

template <typename T, template <typename> class VectorN> inline
bool ray<T, VectorN>::is_intersect(const plane<T>& p) const
{
	return p.is_intersect_ray(origin, direction);
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const plane<T>& v)
{
	// (x, y, z, w)
	os << io::parentheses_left << v.a << io::comma_space
		<< v.b << io::comma_space
		<< v.c << io::comma_space
		<< v.d << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, plane<T>& v)
{
	is.ignore();
	is >> v.a;
	is.ignore();
	is >> v.b;
	is.ignore();
	is >> v.c;
	is.ignore();
	is >> v.d;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_PLANE_H__
