#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Line.h"
#include "Ray.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct Plane;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Plane<float> Planef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Plane<double> Planed;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Plane<long double> Planeld;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USE_CXX11
template <typename T>
using plane = Plane<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using planef = plane<float>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using planed = plane<double>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using planeld = plane<long double>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T>
struct Plane
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef Vector3<T> notmal_type;
	typedef Line<T, Vector3> line_type;
	typedef Ray<T, Vector3> ray_type;

	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	enum IntersectType
	{
		eOnForward, // 前面
		eOnBackward, // 背面
		eOnPlane // 平面上
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

			Vector3<T> Normal; // 法線
			T D; // 原点からの距離

#ifdef _USE_ANONYMOUS_NON_POD
		};
		struct
		{
			T A; // 法線X成分
			T B; // 法線Y成分
			T C; // 法線Z成分
			T Distance;
		};
		array_type Data;
	};
#endif // _USE_ANONYMOUS_NON_POD

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Plane Up; // 0.0, 1.0, 0.0, 0.0
	static const Plane Down; // 0.0, -1.0, 0.0, 0.0
	static const Plane Right; // 1.0, 0.0, 0.0, 0.0
	static const Plane Left; // -1.0, 0.0, 0.0, 0.0
	static const Plane Front; // 0.0, 0.0, 1.0, 0.0
	static const Plane Back; // 0.0, 0.0, -1.0, 0.0

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Plane);
	explicit Plane(const behavior::_noinitialize_t&)
	{

	}
	explicit Plane(const behavior::_right_t&, T d) :
		Normal(math_type::One, math_type::Zero, math_type::Zero),
		D(-d)
	{

	}
	explicit Plane(const behavior::_left_t&, T d) :
		Normal(-math_type::One, math_type::Zero, math_type::Zero),
		D(-d)
	{

	}
	explicit Plane(const behavior::_up_t&, T d) :
		Normal(math_type::Zero, math_type::One, math_type::Zero),
		D(-d)
	{

	}
	explicit Plane(const behavior::_down_t&, T d) :
		Normal(math_type::Zero, -math_type::One, math_type::Zero),
		D(-d)
	{

	}
	explicit Plane(const behavior::_front_t&, T d) :
		Normal(math_type::Zero, math_type::Zero, math_type::One),
		D(-d)
	{

	}
	explicit Plane(const behavior::_back_t&, T d) :
		Normal(math_type::Zero, math_type::Zero, -math_type::One),
		D(-d)
	{

	}
	Plane(T a, T b, T c, T d) :
		Normal(a, b, c), D(d)
	{

	}
	Plane(const Vector3<T>& n, T d) :
		Normal(n), D(d)
	{

	}
	explicit Plane(T d) : // 上方向での距離を渡すのみ
		Normal(math_type::Zero, math_type::One, math_type::Zero),
		D(-d)
	{

	}
	Plane(const Vector3<T>& v0, const Vector3<T>& v1, const Vector3<T>& v2)
	{
		from_points(v0, v1, v2);
	}
	Plane(const Vector3<T>& normal, const Vector3<T>& p)
	{
		from_normal_point(normal, p);
	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Plane& p) const
	{
		return (Normal.is_near(p.Normal) && math_type::is_near(D, p.D));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (Normal.is_near_zero() && math_type::is_near_zero(D));
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (Normal.is_zero() && D == math_type::Zero);
	}

	/*---------------------------------------------------------------------
	* 3つの点から求める（0～2：反時計回り）
	*---------------------------------------------------------------------*/
	Plane& from_points(const Vector3<T>& v0, const Vector3<T>& v1, const Vector3<T>& v2)
	{
		// v0からv1へのベクトル
		Vector3<T> d1(behavior::noinitialize);
		v1.subtract(v0, d1);
		// v1からv2へのベクトル
		Vector3<T> d2(behavior::noinitialize);
		v2.subtract(v0, d2);
		// 二つのベクトルから外積と基点となるv0を渡す
		return from_normal_point(d1.Cross(d2), v0);
	}
	/*---------------------------------------------------------------------
	* 法線と座標から求める
	*---------------------------------------------------------------------*/
	Plane& from_normal_point(const Vector3<T>& normal, const Vector3<T>& p)
	{
		// 正規化されていないことを考慮して渡されてきた法線ベクトルを正規化
		normal.normalized(Normal);
		// dotという名の平面の方程式から求められる値, [ax + by + cz + d] -> [d = -(ax + by + cz)]
		// [a b c] = n
		D = -Normal.dot(p);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const Vector4<T>& v) const
	{
		return Normal.X * v.X + Normal.Y * v.Y + Normal.Z * v.Z + D * v.W;
	}
	/*---------------------------------------------------------------------
	* 法線との内積を求める
	*---------------------------------------------------------------------*/
	T dot_normal(const Vector3<T>& v) const
	{
		return Normal.dot(v);
	}
	/*---------------------------------------------------------------------
	* 内積を求める（W=1）
	*---------------------------------------------------------------------*/
	T dot_coord(const Vector3<T>& v) const
	{
		return Normal.dot(v) + D;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Plane& normalize()
	{
		// 法線の長さでDも正規化を行う
		T length = Normal.length_sq();
		if (length != math_type::Zero)
		{
			length = math_type::rsqrt(length);
			Normal *= length;
			D *= length;
		}
		return *this;
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	Plane normalized() const
	{
		Plane r = *this;
		return r.normalize();
	}
	Plane& normalized(Plane& result) const
	{
		result = *this;
		return result.normalize();
	}

	/*---------------------------------------------------------------------
	* 平面が置かれている座標を求める
	*---------------------------------------------------------------------*/
	Vector3<T> point() const
	{
		// Dは負の値になっているので反転
		return Normal * -D;
	}
	Vector3<T>& point(Vector3<T>& result) const
	{
		result = Normal;
		result *= -D;
		return result;
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Plane lerp(const Plane& to, T t) const
	{
		return Plane(Normal.lerp(to.Normal, t), math_type::lerp(D, to.D, t));
	}
	Plane& lerp(const Plane& to, T t, Plane& result) const
	{
		Normal.lerp(to.Normal, t, result.Normal);
		result.D = math_type::lerp(D, to.D, t);
		return result;
	}
	Plane& lerp(const Plane& from, const Plane& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 交差判定（型で判定）
	*---------------------------------------------------------------------*/
	bool is_intersect(const line_type& line) const
	{
		return is_intersect_line(line.Begin, line.End);
	}
	bool is_intersect(const ray_type& ray) const
	{
		return is_intersect_ray(ray.Position, ray.Direction);
	}
	/*---------------------------------------------------------------------
	* 線分との交差状態か
	*---------------------------------------------------------------------*/
	bool is_intersect_line(const Vector3<T>& begin, const Vector3<T>& end) const
	{
#if 0
		// 始点から終点へのベクトルを求める
		Vector3<T> dir(behavior::noinitialize);
		end.Direction(begin, dir);

		// 法線ベクトルとの内積が0の場合は垂直になっているので交差していない
		if (math_type::is_near_zero(Normal.dot(dir)))
		{
			// 交差なし
			return false;
		}

		// 法線と線分の始点終点の射影を求める（内積）
		// これで単位ベクトルからの長さが求まる
		T b = Normal.dot(begin);
		T e = Normal.dot(end);

		// 射影で求めた間に存在しているか, Dを基準点として考える
		//return ((b - D) >= math_type::Zero) && ((e - D) <= math_type::Zero);
		return (b * e) <= math_type::Zero; // 同符号の場合は衝突していない, 表裏関係なし
#else
		// 同符号の場合は交差していない
		return dot_coord(begin) * dot_coord(end) <= math_type::Zero;
#endif
	}
	bool is_intersect_line(const line_type& line) const
	{
		return is_intersect_line(line.Begin, line.End);
	}
	/*---------------------------------------------------------------------
	* 無限線分と交差状態か
	*---------------------------------------------------------------------*/
	bool is_intersect_ray(const Vector3<T>& position, const Vector3<T>& direction) const
	{
		// 法線との内積を行って0じゃなければいずれ交差する
		return Normal.dot(direction) != math_type::Zero;
	}
	bool is_intersect_ray(const ray_type& ray) const
	{
		return is_intersect_ray(ray.Position, ray.Direction);
	}
	/*---------------------------------------------------------------------
	* 座標との交差状態を求める
	*---------------------------------------------------------------------*/
	IntersectType intersect_point(const Vector3<T>& point) const
	{
		// 法線上の点との距離を計算
		T distance = dot_coord(point);

		if (distance >= math_type::Epsilon)
		{
			return eOnForward;
		}
		if (distance <= -math_type::Epsilon)
		{
			return eOnBackward;
		}
		return eOnPlane;
	}
	bool is_intersect_point(const Vector3<T>& point) const
	{
		return intersect_point(point) == eOnPlane;
	}
	/*---------------------------------------------------------------------
	* 球との交差状態を求める
	*---------------------------------------------------------------------*/
	IntersectType intersect_sphere(const Vector3<T>& center, T radius) const
	{
		T distance = dot_coord(center);

		// 半径より小さかったら平面上に存在している
		if (math_type::abs(distance) <= radius)
		{
			return eOnPlane;
		}
		// 半径より大きい場合は前面
		if (distance > radius)
		{
			return eOnForward;
		}
		return eOnBackward;
	}
	bool is_intersect_sphere(const Vector3<T>& center, T radius) const
	{
		return intersect_sphere(center, radius) == eOnPlane;
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
		return Data[i];
#else
		return (&Normal.X)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS_NON_POD
		return Data[i];
#else
		return (&Normal.X)[i];
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Plane<U>() const
	{
		return Plane<U>(static_cast<U>(Normal.X), static_cast<U>(Normal.Y), static_cast<U>(Normal.Z), static_cast<U>(D));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &Normal.X;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &Normal.X;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Plane& p) const
	{
		return Normal == p.Normal && D == p.D;
	}
	bool operator != (const Plane& p) const
	{
		return !(*this == p);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Plane operator + () const
	{
		return *this;
	}
	Plane operator - () const
	{
		return Plane(-Normal, -D);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Plane operator + (T d)
	{
		return Plane(Normal.X, Normal.Y, Normal.Z, D - d);
	}
	Plane operator - (T d)
	{
		return Plane(Normal.X, Normal.Y, Normal.Z, D + d);
	}
	Plane operator * (T s)
	{
		return Plane(Normal.X, Normal.Y, Normal.Z, D * s);
	}
	Plane operator / (T s)
	{
		return Plane(Normal.X, Normal.Y, Normal.Z, D / s);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Plane& operator = (const behavior::_right_t&)
	{
		Normal = Vector3<T>::Right;
		return *this;
	}
	Plane& operator = (const behavior::_left_t&)
	{
		Normal = Vector3<T>::Left;
		return *this;
	}
	Plane& operator = (const behavior::_up_t&)
	{
		Normal = Vector3<T>::Up;
		return *this;
	}
	Plane& operator = (const behavior::_down_t&)
	{
		Normal = Vector3<T>::Down;
		return *this;
	}
	Plane& operator = (const behavior::_front_t&)
	{
		Normal = Vector3<T>::Front;
		return *this;
	}
	Plane& operator = (const behavior::_back_t&)
	{
		Normal = Vector3<T>::Back;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Plane& operator += (T d)
	{
		// 距離のみを変化させる
		D -= d;
		return *this;
	}
	Plane& operator -= (T d)
	{
		D += d;
		return *this;
	}
	Plane& operator *= (T s)
	{
		D *= s;
		return *this;
	}
	Plane& operator /= (T s)
	{
		D /= s;
		return *this;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Plane& operator () (const behavior::_right_t&)
	{
		Normal = Vector3<T>::Right;
		return *this;
	}
	Plane& operator () (const behavior::_left_t&)
	{
		Normal = Vector3<T>::Left;
		return *this;
	}
	Plane& operator () (const behavior::_up_t&)
	{
		Normal = Vector3<T>::Up;
		return *this;
	}
	Plane& operator () (const behavior::_down_t&)
	{
		Normal = Vector3<T>::Down;
		return *this;
	}
	Plane& operator () (const behavior::_front_t&)
	{
		Normal = Vector3<T>::Front;
		return *this;
	}
	Plane& operator () (const behavior::_back_t&)
	{
		Normal = Vector3<T>::Back;
		return *this;
	}
	Plane operator () (const behavior::_add_t&, T f) const
	{
		return operator+(f);
	}
	Plane operator () (const behavior::_sub_t&, T f) const
	{
		return operator-(f);
	}
	Plane operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Plane operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}

	Plane& operator () (const behavior::_add_assign_t&, T f)
	{
		return operator+=(f);
	}
	Plane& operator () (const behavior::_sub_assign_t&, T f)
	{
		return operator-=(f);
	}
	Plane& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Plane& operator () (const behavior::_div_assign_t&, T f)
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

	bool operator () (const behavior::_equal_t&, const Plane& q) const
	{
		return operator==(q);
	}
	bool operator () (const behavior::_not_equal_t&, const Plane& q) const
	{
		return operator!=(q);
	}
	bool operator () (const behavior::_near_t&, const Plane& q) const
	{
		return is_near(q);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return is_near_zero();
	}

	Plane& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	Plane operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	Plane operator () (const behavior::_lerp_t&, const Plane& p, T t) const
	{
		return lerp(p, t);
	}

	T operator () (const behavior::_dot_t&, const Vector4<T>& v) const
	{
		return dot(v);
	}
	T operator () (const behavior::_dot_normal_t&, const Vector3<T>& v) const
	{
		return dot_normal(v);
	}
	T operator () (const behavior::_dot_coord_t&, const Vector3<T>& v) const
	{
		return dot_coord(v);
	}
	Vector3<T> operator () (const behavior::_point_t&) const
	{
		return point();
	}
	bool operator () (const behavior::_intersect_t&, const Ray<T, Vector3>& r) const
	{
		return is_intersect_ray(r);
	}
	bool operator () (const behavior::_intersect_t&, const Line<T, Vector3>& l) const
	{
		return is_intersect_line(l);
	}
	bool operator () (const behavior::_intersect_ray_t&, const Ray<T, Vector3>& r) const
	{
		return is_intersect_ray(r);
	}
	bool operator () (const behavior::_intersect_ray_t&, const Vector3<T>& position, const Vector3<T>& direction) const
	{
		return is_intersect_ray(position, direction);
	}
	bool operator () (const behavior::_intersect_line_t&, const Line<T, Vector3>& l) const
	{
		return is_intersect_line(l);
	}
	bool operator () (const behavior::_intersect_line_t&, const Vector3<T>& begin, const Vector3<T>& end) const
	{
		return is_intersect_line(begin, end);
	}
};

template <typename T>
const Plane<T> Plane<T>::Up(math_type::Zero, math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Plane<T> Plane<T>::Down(math_type::Zero, -math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Plane<T> Plane<T>::Right(math_type::One, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Plane<T> Plane<T>::Left(-math_type::One, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Plane<T> Plane<T>::Front(math_type::Zero, math_type::Zero, math_type::One, math_type::Zero);
template <typename T>
const Plane<T> Plane<T>::Back(math_type::Zero, math_type::Zero, -math_type::One, math_type::Zero);

template <typename T, template <typename> class VectorN> inline
bool Line<T, VectorN>::is_intersect(const Plane<T>& p) const
{
	return p.is_intersect_line(Begin, End);
}

template <typename T, template <typename> class VectorN> inline
bool Ray<T, VectorN>::is_intersect(const Plane<T>& p) const
{
	return p.is_intersect_ray(Position, Direction);
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Plane<T>& p)
{
	// (Normal, D)
	os << out_char::parentheses_left << p.Normal << out_char::comma_space << p.D << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Plane<T>& p)
{
	is.ignore();
	is >> p.Normal;
	is.ignore();
	is >> p.D;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Plane<T>& p)
{
	os << out_char::parentheses_left << p.Normal << out_char::comma_space << p.D << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Plane<T>& p)
{
	is.ignore();
	is >> p.Normal;
	is.ignore();
	is >> p.D;
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __MATH_PLANE_H__
