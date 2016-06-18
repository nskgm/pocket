#ifndef __POCKET_MATH_PLANE_H__
#define __POCKET_MATH_PLANE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector3.h"
#include "vector4.h"
#include "line.h"
#include "ray.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
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
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef vector3<T> notmal_type;
	typedef vector3<T> point_type;
	typedef line<T, vector3> line_type;
	typedef ray<T, vector3> ray_type;

	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	enum intersect_result
	{
		on_forward, // 前面
		on_backward, // 背面
		on_plane // 平面上
	};

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS_NON_POD

			vector3<T> normal; // 法線
			T d; // 原点からの距離

#ifdef _USE_ANONYMOUS_NON_POD
		};
		struct
		{
			T a; // 法線x成分
			T b; // 法線y成分
			T c; // 法線z成分
			T distance;
		};
		array_type data;
	};
#endif // _USE_ANONYMOUS_NON_POD

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const plane up; // 0.0, 1.0, 0.0, 0.0
	static const plane down; // 0.0, -1.0, 0.0, 0.0
	static const plane right; // 1.0, 0.0, 0.0, 0.0
	static const plane left; // -1.0, 0.0, 0.0, 0.0
	static const plane forward; // 0.0, 0.0, 1.0, 0.0
	static const plane backward; // 0.0, 0.0, -1.0, 0.0

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(plane);
	explicit plane(const behavior::_noinitialize_t&)
	{

	}
	explicit plane(const behavior::_right_t&, T d) :
		normal(math_type::one, math_type::zero, math_type::zero),
		d(-d)
	{

	}
	explicit plane(const behavior::_left_t&, T d) :
		normal(-math_type::one, math_type::zero, math_type::zero),
		d(-d)
	{

	}
	explicit plane(const behavior::_up_t&, T d) :
		normal(math_type::zero, math_type::one, math_type::zero),
		d(-d)
	{

	}
	explicit plane(const behavior::_down_t&, T d) :
		normal(math_type::zero, -math_type::one, math_type::zero),
		d(-d)
	{

	}
	explicit plane(const behavior::_front_t&, T d) :
		normal(math_type::zero, math_type::zero, math_type::one),
		d(-d)
	{

	}
	explicit plane(const behavior::_back_t&, T d) :
		normal(math_type::zero, math_type::zero, -math_type::one),
		d(-d)
	{

	}
	plane(T a, T b, T c, T d) :
		normal(a, b, c), d(d)
	{

	}
	plane(const vector3<T>& n, T d) :
		normal(n), d(d)
	{

	}
	explicit plane(T d) : // 上方向での距離を渡すのみ
		normal(math_type::zero, math_type::one, math_type::zero),
		d(-d)
	{

	}
	plane(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
	{
		from_points(v0, v1, v2);
	}
	plane(const vector3<T>& normal, const vector3<T>& p)
	{
		from_normal_point(normal, p);
	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const plane& p) const
	{
		return (normal.is_near(p.normal) && math_type::is_near(d, p.d));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (normal.is_near_zero() && math_type::is_near_zero(d));
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (normal.is_zero() && d == math_type::zero);
	}

	/*---------------------------------------------------------------------
	* 3つの点から求める（0～2：反時計回り）
	*---------------------------------------------------------------------*/
	plane& from_points(const vector3<T>& v0, const vector3<T>& v1, const vector3<T>& v2)
	{
		// v0からv1へのベクトル
		vector3<T> d1(behavior::noinitialize);
		v1.subtract(v0, d1);
		// v1からv2へのベクトル
		vector3<T> d2(behavior::noinitialize);
		v2.subtract(v0, d2);
		// 二つのベクトルから外積と基点となるv0を渡す
		return from_normal_point(d1.cross(d2), v0);
	}
	/*---------------------------------------------------------------------
	* 法線と座標から求める
	*---------------------------------------------------------------------*/
	plane& from_normal_point(const vector3<T>& normal, const vector3<T>& p)
	{
		// 正規化されていないことを考慮して渡されてきた法線ベクトルを正規化
		normal.normalize(this->normal);
		// dotという名の平面の方程式から求められる値, [ax + by + cz + d] -> [d = -(ax + by + cz)]
		// [a b c] = n
		d = -this->normal.dot(p);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const vector4<T>& v) const
	{
		return normal.x * v.x + normal.y * v.y + normal.z * v.z + d * v.W;
	}
	/*---------------------------------------------------------------------
	* 法線との内積を求める
	*---------------------------------------------------------------------*/
	T dot_normal(const vector3<T>& v) const
	{
		return normal.dot(v);
	}
	/*---------------------------------------------------------------------
	* 内積を求める（W=1）
	*---------------------------------------------------------------------*/
	T dot_coord(const vector3<T>& v) const
	{
		return normal.dot(v) + d;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	plane& normalize()
	{
		// 法線の長さでdも正規化を行う
		T length = normal.length_sq();
		if (length > math_type::zero)
		{
			length = math_type::rsqrt(length);
			normal *= length;
			d *= length;
		}
		return *this;
	}
	plane& normalize(plane& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	plane normalized() const
	{
		plane r = *this;
		return r.normalize();
	}

	/*---------------------------------------------------------------------
	* 平面が置かれている座標を求める
	*---------------------------------------------------------------------*/
	vector3<T> point() const
	{
		// dは負の値になっているので反転
		return normal * -d;
	}
	vector3<T>& point(vector3<T>& result) const
	{
		result = normal;
		result *= -d;
		return result;
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	plane lerp(const plane& to, T t) const
	{
		return plane(normal.lerp(to.normal, t), math_type::lerp(d, to.d, t));
	}
	plane& lerp(const plane& to, T t, plane& result) const
	{
		normal.lerp(to.normal, t, result.normal);
		result.d = math_type::lerp(d, to.d, t);
		return result;
	}
	plane& lerp(const plane& from, const plane& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 交差判定（型で判定）
	*---------------------------------------------------------------------*/
	bool is_intersect(const line_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	bool is_intersect(const ray_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	/*---------------------------------------------------------------------
	* 線分との交差状態か
	*---------------------------------------------------------------------*/
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
	bool is_intersect_line(const line_type& line) const
	{
		return is_intersect_line(line.begin, line.end);
	}
	/*---------------------------------------------------------------------
	* 無限線分と交差状態か
	*---------------------------------------------------------------------*/
	bool is_intersect_ray(const vector3<T>& position, const vector3<T>& direction) const
	{
		// 法線との内積を行って0じゃなければいずれ交差する
		static_cast<void>(position);
		return normal.dot(direction) != math_type::zero;
	}
	bool is_intersect_ray(const ray_type& ray) const
	{
		return is_intersect_ray(ray.origin, ray.direction);
	}
	/*---------------------------------------------------------------------
	* 座標との交差状態を求める
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* 球との交差状態を求める
	*---------------------------------------------------------------------*/
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

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&normal.x)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS_NON_POD
		return data[i];
#else
		return (&normal.x)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator plane<U>() const
	{
		return plane<U>(static_cast<U>(normal.x), static_cast<U>(normal.y), static_cast<U>(normal.z), static_cast<U>(d));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &normal.x;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &normal.x;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const plane& p) const
	{
		return normal == p.normal && d == p.d;
	}
	bool operator != (const plane& p) const
	{
		return !(*this == p);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	plane operator + () const
	{
		return *this;
	}
	plane operator - () const
	{
		return plane(-normal, -d);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	plane operator + (T d)
	{
		return plane(normal.x, normal.y, normal.z, d - d);
	}
	plane operator - (T d)
	{
		return plane(normal.x, normal.y, normal.z, d + d);
	}
	plane operator * (T s)
	{
		return plane(normal.x, normal.y, normal.z, d * s);
	}
	plane operator / (T s)
	{
		return plane(normal.x, normal.y, normal.z, d / s);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	plane& operator = (const behavior::_right_t&)
	{
		normal = vector3<T>::right;
		return *this;
	}
	plane& operator = (const behavior::_left_t&)
	{
		normal = vector3<T>::left;
		return *this;
	}
	plane& operator = (const behavior::_up_t&)
	{
		normal = vector3<T>::up;
		return *this;
	}
	plane& operator = (const behavior::_down_t&)
	{
		normal = vector3<T>::down;
		return *this;
	}
	plane& operator = (const behavior::_front_t&)
	{
		normal = vector3<T>::forward;
		return *this;
	}
	plane& operator = (const behavior::_back_t&)
	{
		normal = vector3<T>::backward;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
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

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	plane& operator () (const behavior::_right_t&)
	{
		normal = vector3<T>::right;
		return *this;
	}
	plane& operator () (const behavior::_left_t&)
	{
		normal = vector3<T>::left;
		return *this;
	}
	plane& operator () (const behavior::_up_t&)
	{
		normal = vector3<T>::up;
		return *this;
	}
	plane& operator () (const behavior::_down_t&)
	{
		normal = vector3<T>::down;
		return *this;
	}
	plane& operator () (const behavior::_front_t&)
	{
		normal = vector3<T>::forward;
		return *this;
	}
	plane& operator () (const behavior::_back_t&)
	{
		normal = vector3<T>::backward;
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

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const plane<T>& p)
{
	// (normal, d)
	os << io::parentheses_left << p.normal << io::comma_space << p.d << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, plane<T>& p)
{
	is.ignore();
	is >> p.normal;
	is.ignore();
	is >> p.d;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const plane<T>& p)
{
	os << io::parentheses_left << p.normal << io::comma_space << p.d << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, plane<T>& p)
{
	is.ignore();
	is >> p.normal;
	is.ignore();
	is >> p.d;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_PLANE_H__
