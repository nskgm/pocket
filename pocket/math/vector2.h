#ifndef __POCKET_MATH_VECTOR2_H__
#define __POCKET_MATH_VECTOR2_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../debug.h"
#include "../container/array.h"
#include "math_traits.h"
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct vector2;
template <typename> struct vector3;
template <typename> struct vector4;
template <typename> struct matrix3x3;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef vector2<int> Point2;
typedef vector2<int> vector2i;
typedef vector2<float> vector2f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef vector2<double> vector2d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef vector2<long double> vector2ld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct vector2
{
	POCKET_MATH_STATICAL_ASSERT(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;

	typedef container::array<T, 2> array_type;
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

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			T x;
			T y;

#ifdef POCKET_USE_ANONYMOUS
		};
		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct vector2;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const vector2 zero; // 0.0, 0.0
	static const vector2 one; // 1.0, 1.0
	static const vector2 unit_x; // 1.0, 0.0
	static const vector2 unit_y; // 0.0, 1.0
	static const vector2 up; // 0.0, 1.0
	static const vector2 down; // 0.0, -1.0
	static const vector2 right; // 1.0, 0.0
	static const vector2 left; // -1.0, 0.0
#if 0
	static const vector2 forward; // 0.0, 0.0 [Dummy]
	static const vector2 backward; // 0.0, 0.0 [Dummy]
#endif

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(vector2);
	explicit vector2(const behavior::_noinitialize_t&)
	{

	}
	explicit vector2(const behavior::_zero_t&) :
		x(math_type::zero), y(math_type::zero)
	{

	}
	explicit vector2(const behavior::_one_t&) :
		x(math_type::one), y(math_type::one)
	{

	}
	explicit vector2(const behavior::_half_t&) :
		x(math_type::half), y(math_type::half)
	{

	}
	explicit vector2(const behavior::_half_of_half_t&) :
		x(math_type::half_of_half), y(math_type::half_of_half)
	{

	}
	vector2(T x, T y) :
		x(x), y(y)
	{

	}
	template <typename U, typename U1,
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U), POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1)>
		vector2(U x, U1 y) :
		x(static_cast<T>(x)), y(static_cast<T>(y))
	{

	}
	explicit vector2(T f) :
		x(f), y(f)
	{

	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit vector2(U f) :
		x(static_cast<T>(f)), y(static_cast<T>(f))
	{

	}
	template <typename U>
	vector2(const vector2<U>& v) :
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
	{

	}
	explicit vector2(const T* p) :
		x(p[0]), y(p[1])
	{

	}

	explicit vector2(const vector3<T>&);
	template <typename U> explicit vector2(const vector3<U>&); // vector3.h
	explicit vector2(const vector4<T>&);
	template <typename U> explicit vector2(const vector4<U>&); // vector4.h

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 角度から求める
	//---------------------------------------------------------------------
	vector2& from_angle(T angle)
	{
		x = math_type::cos(angle);
		y = math_type::sin(angle);
		return *this;
	}
	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
	vector2& add(const vector2& v, vector2& result) const
	{
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}
	template <typename U>
	vector2& add(const vector2<U>& v, vector2& result) const
	{
		result.x = x + static_cast<T>(v.x);
		result.y = y + static_cast<T>(v.y);
		return result;
	}
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
	vector2& subtract(const vector2& v, vector2& result) const
	{
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}
	template <typename U>
	vector2& subtract(const vector2<U>& v, vector2& result) const
	{
		result.x = x - static_cast<T>(v.x);
		result.y = y - static_cast<T>(v.y);
		return result;
	}
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
	vector2& multiply(T f, vector2& result) const
	{
		result.x = x * f;
		result.y = y * f;
		return result;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2& multiply(U f, vector2& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	vector2& divide(T f, vector2& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return multiply(math_type::reciprocal(f), result);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2& divide(U f, vector2& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_traits<U>::zero);
		return divide(static_cast<T>(f), result);
	}
	//---------------------------------------------------------------------
	// 平行か
	//---------------------------------------------------------------------
	bool is_parallel(const vector2& v) const
	{
		// 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向)
		return math_type::is_near_zero(dot(v) - (length() * v.length()));
	}
	//---------------------------------------------------------------------
	// 垂直か
	//---------------------------------------------------------------------
	bool is_vertical(const vector2& v) const
	{
		return math_type::is_near_zero(dot(v));
	}

	//---------------------------------------------------------------------
	// 値が近いか
	//---------------------------------------------------------------------
	bool is_near(const vector2& v) const
	{
		return (math_type::is_near(x, v.x) && math_type::is_near(y, v.y));
	}
	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(x) && math_type::is_near_zero(y));
	}
	//---------------------------------------------------------------------
	// 値がゼロか
	//---------------------------------------------------------------------
	bool is_zero() const
	{
		return (x == math_type::zero && y == math_type::zero);
	}

	//---------------------------------------------------------------------
	// 長さを求める
	//---------------------------------------------------------------------
	T length() const
	{
		return math_type::sqrt(length_sq());
	}

	//---------------------------------------------------------------------
	// 長さを求める（二乗）
	//---------------------------------------------------------------------
	T length_sq() const
	{
		return dot(*this);
	}
	//---------------------------------------------------------------------
	// 要素の合計を求める
	//---------------------------------------------------------------------
	T accumulate() const
	{
		return x + y;
	}
	//---------------------------------------------------------------------
	// 距離を求める
	//---------------------------------------------------------------------
	T distance(const vector2& v) const
	{
		// 差を求めたあとの長さ
		vector2 t(x - v.x, y - v.y);
		return t.length();
	}
	//---------------------------------------------------------------------
	// 方向を求める
	//---------------------------------------------------------------------
	vector2 direction(const vector2& to) const
	{
		vector2 t(to.x - x, to.y - y);
		return t.normalize();
	}
	vector2& direction(const vector2& to, vector2& result) const
	{
		result.x = to.x - x;
		result.y = to.y - y;
		return result.normalize();
	}
	//---------------------------------------------------------------------
	// 内積を求める
	//---------------------------------------------------------------------
	T dot(const vector2& v) const
	{
		// |v1||v2|cos(θ)と同じになる
		// 値が０のときは垂直
		return x * v.x + y * v.y;
	}
	//---------------------------------------------------------------------
	// 外積を求める
	//---------------------------------------------------------------------
	T cross(const vector2& v) const
	{
		// 三角形をイメージしたときに高さになる
		return x * v.y - v.x * y;
	}
	//---------------------------------------------------------------------
	// 正規化
	//---------------------------------------------------------------------
	vector2& normalize()
	{
		T len = length_sq();
		// ゼロ割対策
		if (len > math_type::zero)
		{
			len = math_type::rsqrt(len);
			x *= len;
			y *= len;
		}
		return *this;
	}
	vector2& normalize(vector2& result) const
	{
		result = *this;
		return result.normalize();
	}

	//---------------------------------------------------------------------
	// 正規化を求める
	//---------------------------------------------------------------------
	vector2 normalized() const
	{
		vector2 v = *this;
		return v.normalize();
	}
	//---------------------------------------------------------------------
	// 線形補間
	//---------------------------------------------------------------------
	vector2 lerp(const vector2& to, T t) const
	{
		return vector2(math_type::lerp(x, to.x, t), math_type::lerp(y, to.y, t));
	}
	vector2& lerp(const vector2& to, T t, vector2& result) const
	{
		result.x = math_type::lerp(x, to.x, t);
		result.y = math_type::lerp(y, to.y, t);
		return result;
	}
	vector2& lerp(const vector2& from, const vector2& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	//---------------------------------------------------------------------
	// 値を0～1にクランプ
	//---------------------------------------------------------------------
	vector2& saturate()
	{
		x = math_type::clamp01(x);
		y = math_type::clamp01(y);
		return *this;
	}
	vector2& saturate(vector2& result) const
	{
		result.x = math_type::clamp01(x);
		result.y = math_type::clamp01(y);
		return result;
	}
	vector2 saturated() const
	{
		return vector2(math_type::clamp01(x), math_type::clamp01(y));
	}

	//---------------------------------------------------------------------
	// 最大値にする
	//---------------------------------------------------------------------
	vector2& maximize(const vector2& v)
	{
		x = math_type::max(x, v.x);
		y = math_type::max(y, v.y);
		return *this;
	}
	vector2& maximize(const vector2& v, vector2& result) const
	{
		result.x = math_type::max(x, v.x);
		result.y = math_type::max(y, v.y);
		return result;
	}
	vector2 maximized(const vector2& v) const
	{
		return vector2(math_type::max(x, v.x), math_type::max(y, v.y));
	}

	//---------------------------------------------------------------------
	// 最小値にする
	//---------------------------------------------------------------------
	vector2& minimize(const vector2& v)
	{
		x = math_type::min(x, v.x);
		y = math_type::min(y, v.y);
		return *this;
	}
	vector2& minimize(const vector2& v, vector2& result) const
	{
		result.x = math_type::min(x, v.x);
		result.y = math_type::min(y, v.y);
		return result;
	}
	vector2 minimized(const vector2& v) const
	{
		return vector2(math_type::min(x, v.x), math_type::min(y, v.y));
	}

	//---------------------------------------------------------------------
	// 角度を求める
	//---------------------------------------------------------------------
	T angle() const
	{
		return math_type::atan2(y, x);
	}
	//---------------------------------------------------------------------
	// 二点間の角度を求める
	//---------------------------------------------------------------------
	T angle(const vector2& v) const
	{
		const vector2 c(x - v.x, y - v.y);
		return c.angle();
	}
	//---------------------------------------------------------------------
	// 正規化済みでの二点間の角度を求める
	//---------------------------------------------------------------------
	T inner_angle(const vector2& v) const
	{
		vector2 from = *this;
		from.normalize();
		vector2 to = v;
		to.normalize();
		return math_type::acos(from.dot(to));
	}
	//---------------------------------------------------------------------
	// 座標変換
	//---------------------------------------------------------------------
	vector2& transform(const matrix3x3<T>&); // matrix3x3.h
	vector2& transform(const matrix3x3<T>&, vector2& result) const;
	vector2 transformed(const matrix3x3<T>&) const;
	vector2& transform_coord(const matrix3x3<T>&);
	vector2& transform_coord(const matrix3x3<T>&, vector2& result) const;
	vector2 transformed_coord(const matrix3x3<T>&) const;
	vector2& transform_normal(const matrix3x3<T>&);
	vector2& transform_normal(const matrix3x3<T>&, vector2& result) const;
	vector2 transformed_normal(const matrix3x3<T>&) const;

	//---------------------------------------------------------------------
	// 並べ替え
	//---------------------------------------------------------------------
	vector2 swizzle(int x, int y) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 1);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 1);
		return vector2((*this)[x], (*this)[y]);
	}
	vector3<T> swizzle(int, int, int) const; // vector3.h
	vector4<T> swizzle(int, int, int, int) const; // vector4.h

	//-----------------------------------------------------------------------------------------
	// Operatots
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// アクセス演算子
	//---------------------------------------------------------------------
	T& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 1);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 1);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 型変換演算子
	//---------------------------------------------------------------------
	template <typename U>
	POCKET_CXX11_EXPLICIT operator vector2<U>() const
	{
		return vector2<U>(static_cast<U>(x), static_cast<U>(y));
	}
	POCKET_CXX11_EXPLICIT operator T* ()
	{
#ifdef POCKET_USE_ANONYMOUS
		return &data[0];
#else
		return &x;
#endif // POCKET_USE_ANONYMOUS
	}
	POCKET_CXX11_EXPLICIT operator const T* () const
	{
#ifdef POCKET_USE_ANONYMOUS
		return &data[0];
#else
		return &x;
#endif // POCKET_USE_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const vector2& v) const
	{
		return (x == v.x) && (y == v.y);
	}
	bool operator != (const vector2& v) const
	{
		return !(*this == v);
	}
	bool operator < (const vector2& v) const
	{
		return x < v.x && y < v.y;
	}
	bool operator <= (const vector2& v) const
	{
		return x <= v.x && y <= v.y;
	}
	bool operator > (const vector2& v) const
	{
		return x > v.x && y > v.y;
	}
	bool operator >= (const vector2& v) const
	{
		return x >= v.x && y >= v.y;
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	vector2 operator + () const
	{
		return *this;
	}
	vector2 operator - () const
	{
		return vector2(-x, -y);
	}
	vector2& operator ++ ()
	{
		++x;
		++y;
		return *this;
	}
	vector2 operator ++ (int)
	{
		const vector2 r = *this;
		++(*this);
		return r;
	}
	vector2& operator -- ()
	{
		--x;
		--y;
		return *this;
	}
	vector2 operator -- (int)
	{
		const vector2 r = *this;
		--(*this);
		return r;
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	vector2 operator + (const vector2& v) const
	{
		vector2 result(behavior::noinitialize);
		return add(v, result);
	}
	template <typename U>
	vector2 operator + (const vector2<U>& v) const
	{
		vector2 result(behavior::noinitialize);
		return add<U>(v, result);
	}
	vector2 operator - (const vector2& v) const
	{
		vector2 result(behavior::noinitialize);
		return subtract(v, result);
	}
	template <typename U>
	vector2 operator - (const vector2<U>& v) const
	{
		vector2 result(behavior::noinitialize);
		return subtract<U>(v, result);
	}
	vector2 operator * (T f) const
	{
		vector2 result(behavior::noinitialize);
		return multiply(f, result);
	}
	vector2 operator * (const matrix3x3<T>&) const; // matrix3x3.h
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2 operator * (U f) const
	{
		vector2 result(behavior::noinitialize);
		return multiply<U>(f, result);
	}
	vector2 operator / (T f) const
	{
		vector2 result(behavior::noinitialize);
		return divide(f, result);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2 operator / (U f) const
	{
		vector2 result(behavior::noinitialize);
		return divide<U>(f, result);
	}
	vector2 operator % (T f) const
	{
		return vector2(math_type::remainder(x, f), math_type::remainder(y, f));
	}
	vector2 operator % (const vector2& v) const
	{
		return vector2(math_type::remainder(x, v.x), math_type::remainder(y, v.y));
	}

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
	vector2& operator = (T f)
	{
		x = y = f;
		return *this;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2& operator = (U f)
	{
		return operator=(static_cast<T>(f));
	}
	vector2& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector2& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector2& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector2& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}
	template <typename U>
	vector2& operator = (const vector2<U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		return *this;
	}
	vector2& operator = (const vector3<T>&);
	template <typename U> vector2& operator = (const vector3<U>&); // vector3.h
	vector2& operator = (const vector4<T>&);
	template <typename U> vector2& operator = (const vector4<U>&); // vector4.h

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	vector2& operator += (const vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	template <typename U>
	vector2& operator += (const vector2<U>& v)
	{
		x += static_cast<T>(v.x);
		y += static_cast<T>(v.y);
		return *this;
	}
	vector2& operator -= (const vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	template <typename U>
	vector2& operator -= (const vector2<U>& v)
	{
		x -= static_cast<T>(v.x);
		y -= static_cast<T>(v.y);
		return *this;
	}
	vector2& operator *= (T f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2& operator *= (U f)
	{
		x *= static_cast<T>(f);
		y *= static_cast<T>(f);
		return *this;
	}
	vector2& operator /= (T f)
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		f = math_type::one / f;
		return *this *= f;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector2& operator /= (U f)
	{
		POCKET_DEBUG_ASSERT(f != math_traits<U>::zero);
		T inv = math_type::one / static_cast<T>(f);
		return *this *= inv;
	}
	vector2& operator %= (T f)
	{
		math_type::remainder_assign(x, f);
		math_type::remainder_assign(y, f);
		return *this;
	}
	vector2& operator %= (const vector2& v)
	{
		math_type::remainder_assign(x, v.x);
		math_type::remainder_assign(y, v.y);
		return *this;
	}

	//------------------------------------------------------------------------------------------
	// タグでの関数呼び出し
	//------------------------------------------------------------------------------------------

	vector2& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector2& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector2& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector2& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}

	vector2 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	vector2 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	vector2 operator () (const behavior::_add_t&, const vector2& v) const
	{
		return operator+(v);
	}
	vector2 operator () (const behavior::_sub_t&, const vector2& v) const
	{
		return operator-(v);
	}
	vector2 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	vector2 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	vector2 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	vector2 operator () (const behavior::_rem_t&, const vector2& v) const
	{
		return operator%(v);
	}

	vector2& operator () (const behavior::_add_assign_t&, const vector2& v)
	{
		return operator+=(v);
	}
	vector2& operator () (const behavior::_sub_assign_t&, const vector2& v)
	{
		return operator-=(v);
	}
	vector2& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	vector2& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	vector2& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	vector2& operator () (const behavior::_rem_assign_t&, const vector2& v)
	{
		return operator%=(v);
	}
	vector2& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	vector2 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	vector2& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	vector2 operator () (const behavior::_decrement_back_t&)
	{
		return operator--(0);
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

	bool operator () (const behavior::_equal_t&, const vector2& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const vector2& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const vector2& v) const
	{
		return is_near(v);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return is_near_zero();
	}

	T operator () (const behavior::_length_t&) const
	{
		return length();
	}
	T operator () (const behavior::_length_square_t&) const
	{
		return length_sq();
	}
	T operator () (const behavior::_dot_t&, const vector2& v) const
	{
		return dot(v);
	}
	vector2& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	vector2 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	T operator () (const behavior::_cross_t&, const vector2& v) const
	{
		return cross(v);
	}
	vector2 operator () (const behavior::_lerp_t&, const vector2& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const vector2& v) const
	{
		return distance(v);
	}
	vector2 operator () (const behavior::_direction_t&, const vector2& v) const
	{
		return direction(v);
	}
	vector2 operator () (const behavior::_direction_t&, const vector2& v, T force) const
	{
		return direction(v) * force;
	}
	vector2& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	vector2 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	vector2 operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	vector3<T> operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	vector4<T> operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
	{
		return swizzle(x, y, z, w);
	}

	T operator () (const behavior::_inner_t&, const vector2& v) const
	{
		return inner_angle(v);
	}
	T operator () (const behavior::_angle_t&) const
	{
		return angle();
	}
	T operator () (const behavior::_angle_t&, const vector2& v) const
	{
		return angle(v);
	}

	vector2& operator () (const behavior::_from_angle_t&, T angle) const
	{
		return from_angle(angle);
	}
};

template <typename T>
const vector2<T> vector2<T>::zero(math_type::zero, math_type::zero);
template <typename T>
const vector2<T> vector2<T>::one(math_type::one, math_type::one);
template <typename T>
const vector2<T> vector2<T>::unit_x(math_type::one, math_type::zero);
template <typename T>
const vector2<T> vector2<T>::unit_y(math_type::zero, math_type::one);
template <typename T>
const vector2<T> vector2<T>::up(math_type::zero, math_type::one);
template <typename T>
const vector2<T> vector2<T>::down(math_type::zero, -math_type::one);
template <typename T>
const vector2<T> vector2<T>::right(math_type::one, math_type::zero);
template <typename T>
const vector2<T> vector2<T>::left(-math_type::one, math_type::zero);
#if 0
template <typename T>
const vector2<T> vector2<T>::forward(math_type::zero, math_type::zero);
template <typename T>
const vector2<T> vector2<T>::backward(math_type::zero, math_type::zero);
#endif

// 左辺が数値の場合の演算子

template <typename T> inline
vector2<T> operator * (T f, const vector2<T>& v)
{
	return v * f;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vector2<T>& v)
{
	// (x, y)
	os << io::parentheses_left << v.x << io::comma_space
		<< v.y << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, vector2<T>& v)
{
	is.ignore();
	is >> v.x;
	is.ignore();
	is >> v.y;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_VECTOR2_H__
