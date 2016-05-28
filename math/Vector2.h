#ifndef __MATH_VECTOR2_H__
#define __MATH_VECTOR2_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "Math.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename> struct Vector2;
template <typename> struct Vector3;
template <typename> struct Vector4;
template <typename> struct Matrix3x3;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector2<int> Point2;
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Vector2<double> Vector2d;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector2<long double> Vector2ld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T>
using vec2 = Vector2<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using vec2f = vec2<float>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using vec2d = vec2<double>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using vec2ld = vec2<long double>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

template <typename T>
struct Vector2
{
	_MATH_STATICAL_ASSERT(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;

	typedef container::array<T, 2> array_type;
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

#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif /* _USE_ANONYMOUS */

			T X;
			T Y;

#ifdef _USE_ANONYMOUS
		};
		array_type Data;
	};
#endif /* _USE_ANONYMOUS */

	template <typename> friend struct Vector2;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Vector2 Zero; /* 0.0, 0.0 */
	static const Vector2 One; /* 1.0, 1.0 */
	static const Vector2 UnitX; /* 1.0, 0.0 */
	static const Vector2 UnitY; /* 0.0, 1.0 */
	static const Vector2 Up; /* 0.0, 1.0 */
	static const Vector2 Down; /* 0.0, -1.0 */
	static const Vector2 Right; /* 1.0, 0.0 */
	static const Vector2 Left; /* -1.0, 0.0 */
#if 0
	static const Vector2 Front; /* 0.0, 0.0 [Dummy] */
	static const Vector2 Back; /* 0.0, 0.0 [Dummy] */
#endif

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Vector2);
	explicit Vector2(const behavior::_noinitialize_t&)
	{

	}
	explicit Vector2(const behavior::_zero_t&) :
		X(math_type::Zero), Y(math_type::Zero)
	{

	}
	explicit Vector2(const behavior::_one_t&) :
		X(math_type::One), Y(math_type::One)
	{

	}
	explicit Vector2(const behavior::_half_t&) :
		X(math_type::Half), Y(math_type::Half)
	{

	}
	explicit Vector2(const behavior::_half_of_half_t&) :
		X(math_type::HalfOfHalf), Y(math_type::HalfOfHalf)
	{

	}
	Vector2(T X, T Y) :
		X(X), Y(Y)
	{

	}
	template <typename U, typename U1,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U), _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1)>
		Vector2(U X, U1 Y) :
		X(static_cast<T>(X)), Y(static_cast<T>(Y))
	{

	}
	explicit Vector2(T f) :
		X(f), Y(f)
	{

	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit Vector2(U f) :
		X(static_cast<T>(f)), Y(static_cast<T>(f))
	{

	}
	template <typename U>
	Vector2(const Vector2<U>& v) :
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y))
	{

	}

	explicit Vector2(const Vector3<T>&);
	template <typename U> explicit Vector2(const Vector3<U>&); /* Vector3.h */
	explicit Vector2(const Vector4<T>&);
	template <typename U> explicit Vector2(const Vector4<U>&); /* Vector4.h */

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 角度から求める
	*---------------------------------------------------------------------*/
	Vector2& from_angle(T angle)
	{
		X = math_type::cos(angle);
		Y = math_type::sin(angle);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Vector2& add(const Vector2& v, Vector2& result) const
	{
		result.X = X + v.X;
		result.Y = Y + v.Y;
		return result;
	}
	template <typename U>
	Vector2& add(const Vector2<U>& v, Vector2& result) const
	{
		result.X = X + static_cast<T>(v.X);
		result.Y = Y + static_cast<T>(v.Y);
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	Vector2& subtract(const Vector2& v, Vector2& result) const
	{
		result.X = X - v.X;
		result.Y = Y - v.Y;
		return result;
	}
	template <typename U>
	Vector2& subtract(const Vector2<U>& v, Vector2& result) const
	{
		result.X = X - static_cast<T>(v.X);
		result.Y = Y - static_cast<T>(v.Y);
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	Vector2& multiply(T f, Vector2& result) const
	{
		result.X = X * f;
		result.Y = Y * f;
		return result;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2& multiply(U f, Vector2& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Vector2& divide(T f, Vector2& result) const
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return multiply(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2& divide(U f, Vector2& result) const
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		return divide(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 平行か
	*---------------------------------------------------------------------*/
	bool is_parallel(const Vector2& v) const
	{
		/* 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向) */
		return math_type::is_near_zero(dot(v) - (length() * v.length()));
	}
	/*---------------------------------------------------------------------
	* 垂直か
	*---------------------------------------------------------------------*/
	bool is_vertical(const Vector2& v) const
	{
		return math_type::is_near_zero(dot(v));
	}

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Vector2& v) const
	{
		return (math_type::is_near(X, v.X) && math_type::is_near(Y, v.Y));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(X) && math_type::is_near_zero(Y));
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (X == math_type::Zero && Y == math_type::Zero);
	}

	/*---------------------------------------------------------------------
	* 長さを求める
	*---------------------------------------------------------------------*/
	T length() const
	{
		return math_type::sqrt(dot(*this));
	}

	/*---------------------------------------------------------------------
	* 長さを求める（二乗）
	*---------------------------------------------------------------------*/
	T length_sq() const
	{
		return dot(*this);
	}
	/*---------------------------------------------------------------------
	* 要素の合計を求める
	*---------------------------------------------------------------------*/
	T accumulate() const
	{
		return X + Y;
	}
	/*---------------------------------------------------------------------
	* 距離を求める
	*---------------------------------------------------------------------*/
	T distance(const Vector2& v) const
	{
		/* 差を求めたあとの長さ */
		Vector2 t(X - v.X, Y - v.Y);
		return t.length();
	}
	/*---------------------------------------------------------------------
	* 方向を求める
	*---------------------------------------------------------------------*/
	Vector2 direction(const Vector2& v) const
	{
		Vector2 t(X - v.X, Y - v.Y);
		return t.normalize();
	}
	Vector2& direction(const Vector2& v, Vector2& result) const
	{
		result.X = X - v.X;
		result.Y = Y - v.Y;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const Vector2& v) const
	{
		/* |v1||v2|cos(θ)と同じになる */
		/* 値が０のときは垂直 */
		return X * v.X + Y * v.Y;
	}
	/*---------------------------------------------------------------------
	* 外積を求める
	*---------------------------------------------------------------------*/
	T cross(const Vector2& v) const
	{
		/* 三角形をイメージしたときに高さになる */
		return X * v.Y - v.X * Y;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Vector2& normalize()
	{
		T len = length_sq();
		/* ゼロ割対策 */
		if (len != math_type::Zero)
		{
			/* 長さの逆数 */
			len = math_type::rsqrt(len);
			X *= len;
			Y *= len;
		}
		return *this;
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	Vector2 normalized() const
	{
		Vector2 v = *this;
		return v.normalize();
	}
	Vector2& normalized(Vector2& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 射影を求める
	*---------------------------------------------------------------------*/
	Vector2 projection(const Vector2& v) const
	{
		/* 正規化された基準となる線のもう片方のベクトルの比率 */
		/* 基準となるベクトルは目安となるので正規化されている方がやりやすい */
		/* ベクトル同士の角度で求めるので内積を使用する */
		/* どちらも正規化されていた場合比率が求めやすい */
		return *this * v.dot(*this);
	}
	Vector2& projection(const Vector2& v, Vector2& result) const
	{
		result = *this;
		return result *= v.dot(*this);
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Vector2 lerp(const Vector2& to, T t) const
	{
		return Vector2(math_type::lerp(X, to.X, t), math_type::lerp(Y, to.Y, t));
	}
	Vector2& lerp(const Vector2& to, T t, Vector2& result) const
	{
		result.X = math_type::lerp(X, to.X, t);
		result.Y = math_type::lerp(Y, to.Y, t);
		return result;
	}
	Vector2& lerp(const Vector2& from, const Vector2& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 値を0～1にクランプ
	*---------------------------------------------------------------------*/
	Vector2& saturate()
	{
		X = math_type::clamp01(X);
		Y = math_type::clamp01(Y);
		return *this;
	}
	Vector2 saturated() const
	{
		return Vector2(math_type::clamp01(X), math_type::clamp01(Y));
	}
	Vector2& saturated(Vector2& result) const
	{
		result.X = math_type::clamp01(X);
		result.Y = math_type::clamp01(Y);
		return result;
	}
	/*---------------------------------------------------------------------
	* 角度を求める
	*---------------------------------------------------------------------*/
	T angle() const
	{
		return math_type::atan2(Y, X);
	}
	/*---------------------------------------------------------------------
	* 二点間の角度を求める
	*---------------------------------------------------------------------*/
	T angle(const Vector2& v) const
	{
		const Vector2 c(X - v.X, Y - v.Y);
		return c.angle();
	}
	/*---------------------------------------------------------------------
	* 正規化済みでの二点間の角度を求める
	*---------------------------------------------------------------------*/
	T inner_angle(const Vector2& v) const
	{
		Vector2 from(*this);
		from.normalize();
		Vector2 to(v);
		to.normalize();
		return math_type::acos(from.dot(to));
	}
	/*---------------------------------------------------------------------
	* 座標変換
	*---------------------------------------------------------------------*/
	Vector2& transform(const Matrix3x3<T>&); /* Matrix3x3.h */
	Vector2& transformed(const Matrix3x3<T>&, Vector2&) const;
	Vector2 transformed(const Matrix3x3<T>&) const;
	Vector2& transform_coord(const Matrix3x3<T>&);
	Vector2& transformed_coord(const Matrix3x3<T>&, Vector2&) const;
	Vector2 transformed_coord(const Matrix3x3<T>&) const;
	Vector2& transform_normal(const Matrix3x3<T>&);
	Vector2& transformed_normal(const Matrix3x3<T>&, Vector2&) const;
	Vector2 transformed_normal(const Matrix3x3<T>&) const;

	/*---------------------------------------------------------------------
	* 並べ替え
	*---------------------------------------------------------------------*/
	Vector2 swizzle(int x, int y) const
	{
		_DEB_RANGE_ASSERT(x, 0, 1);
		_DEB_RANGE_ASSERT(y, 0, 1);
		return Vector2((*this)[x], (*this)[y]);
	}
	Vector3<T> swizzle(int x, int y, int z) const; /* Vector3.h */
	Vector4<T> swizzle(int x, int y, int z, int w) const; /* Vector4.h */

	/*-----------------------------------------------------------------------------------------
	* Operatots
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif /* _USE_ANONYMOUS */
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif /* _USE_ANONYMOUS */
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Vector2<U>() const
	{
		return Vector2<U>(static_cast<U>(X), static_cast<U>(Y));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif /* _USE_ANONYMOUS */
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif /* _USE_ANONYMOUS */
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Vector2& v) const
	{
		return (X == v.X) && (Y == v.Y);
	}
	bool operator != (const Vector2& v) const
	{
		return !(*this == v);
	}
	bool operator < (const Vector2& v) const
	{
		return length_sq() < v.length_sq();
	}
	bool operator <= (const Vector2& v) const
	{
		return length_sq() <= v.length_sq();
	}
	bool operator > (const Vector2& v) const
	{
		return length_sq() > v.length_sq();
	}
	bool operator >= (const Vector2& v) const
	{
		return length_sq() >= v.length_sq();
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Vector2 operator + () const
	{
		return *this;
	}
	Vector2 operator - () const
	{
		return Vector2(-X, -Y);
	}
	Vector2& operator ++ ()
	{
		++X;
		++Y;
		return *this;
	}
	Vector2 operator ++ (int)
	{
		const Vector2 r(*this);
		++(*this);
		return r;
	}
	Vector2& operator -- ()
	{
		--X;
		--Y;
		return *this;
	}
	Vector2 operator -- (int)
	{
		const Vector2 r(*this);
		--(*this);
		return r;
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Vector2 operator + (const Vector2& v) const
	{
		Vector2 result(behavior::noinitialize);
		return add(v, result);
	}
	template <typename U>
	Vector2 operator + (const Vector2<U>& v) const
	{
		Vector2 result(behavior::noinitialize);
		return add<U>(v, result);
	}
	Vector2 operator - (const Vector2& v) const
	{
		Vector2 result(behavior::noinitialize);
		return subtract(v, result);
	}
	template <typename U>
	Vector2 operator - (const Vector2<U>& v) const
	{
		Vector2 result(behavior::noinitialize);
		return subtract<U>(v, result);
	}
	Vector2 operator * (T f) const
	{
		Vector2 result(behavior::noinitialize);
		return multiply(f, result);
	}
	Vector2 operator * (const Matrix3x3<T>&) const; /* Matrix3x3.h */
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2 operator * (U f) const
	{
		Vector2 result(behavior::noinitialize);
		return multiply<U>(f, result);
	}
	Vector2 operator / (T f) const
	{
		Vector2 result(behavior::noinitialize);
		return divide(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2 operator / (U f) const
	{
		Vector2 result(behavior::noinitialize);
		return divide<U>(f, result);
	}
	Vector2 operator % (T f) const
	{
		return Vector2(math_type::remainder(X, f), math_type::remainder(Y, f));
	}
	Vector2 operator % (const Vector2& v) const
	{
		return Vector2(math_type::remainder(X, v.X), math_type::remainder(Y, v.Y));
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Vector2& operator = (T f)
	{
		X = Y = f;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2& operator = (U f)
	{
		return operator=(static_cast<T>(f));
	}
	Vector2& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector2& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector2& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector2& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Vector2& operator += (const Vector2& v)
	{
		X += v.X;
		Y += v.Y;
		return *this;
	}
	template <typename U>
	Vector2& operator += (const Vector2<U>& v)
	{
		X += static_cast<T>(v.X);
		Y += static_cast<T>(v.Y);
		return *this;
	}
	Vector2& operator -= (const Vector2& v)
	{
		X -= v.X;
		Y -= v.Y;
		return *this;
	}
	template <typename U>
	Vector2& operator -= (const Vector2<U>& v)
	{
		X -= static_cast<T>(v.X);
		Y -= static_cast<T>(v.Y);
		return *this;
	}
	Vector2& operator *= (T f)
	{
		X *= f;
		Y *= f;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2& operator *= (U f)
	{
		X *= static_cast<T>(f);
		Y *= static_cast<T>(f);
		return *this;
	}
	Vector2& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return *this *= f;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector2& operator /= (U f)
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		T inv = math_type::One / static_cast<T>(f);
		return *this *= inv;
	}
	Vector2& operator %= (T f)
	{
		math_type::remainder_assign(X, f);
		math_type::remainder_assign(Y, f);
		return *this;
	}
	Vector2& operator %= (const Vector2& v)
	{
		math_type::remainder_assign(X, v.X);
		math_type::remainder_assign(Y, v.Y);
		return *this;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Vector2& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector2& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector2& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector2& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	Vector2 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	Vector2 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	Vector2 operator () (const behavior::_add_t&, const Vector2& v) const
	{
		return operator+(v);
	}
	Vector2 operator () (const behavior::_sub_t&, const Vector2& v) const
	{
		return operator-(v);
	}
	Vector2 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Vector2 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	Vector2 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	Vector2 operator () (const behavior::_rem_t&, const Vector2& v) const
	{
		return operator%(v);
	}

	Vector2& operator () (const behavior::_add_assign_t&, const Vector2& v)
	{
		return operator+=(v);
	}
	Vector2& operator () (const behavior::_sub_assign_t&, const Vector2& v)
	{
		return operator-=(v);
	}
	Vector2& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Vector2& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	Vector2& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	Vector2& operator () (const behavior::_rem_assign_t&, const Vector2& v)
	{
		return operator%=(v);
	}
	Vector2& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	Vector2 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	Vector2& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	Vector2 operator () (const behavior::_decrement_back_t&)
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

	bool operator () (const behavior::_equal_t&, const Vector2& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const Vector2& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const Vector2& v) const
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
	T operator () (const behavior::_dot_t&, const Vector2& v) const
	{
		return dot(v);
	}
	Vector2& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	Vector2 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	T operator () (const behavior::_cross_t&, const Vector2& v) const
	{
		return cross(v);
	}
	Vector2 operator () (const behavior::_lerp_t&, const Vector2& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const Vector2& v) const
	{
		return distance(v);
	}
	Vector2 operator () (const behavior::_direction_t&, const Vector2& v) const
	{
		return direction(v);
	}
	Vector2 operator () (const behavior::_direction_t&, const Vector2& v, T force) const
	{
		return direction(v) * force;
	}
	Vector2& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	Vector2 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	Vector2 operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	Vector3<T> operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	Vector4<T> operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
	{
		return swizzle(x, y, z, w);
	}

	T operator () (const behavior::_inner_t&, const Vector2& v) const
	{
		return inner_angle(v);
	}
	T operator () (const behavior::_angle_t&) const
	{
		return angle();
	}
	T operator () (const behavior::_angle_t&, const Vector2& v) const
	{
		return angle(v);
	}

	Vector2& operator () (const behavior::_from_angle_t&, T angle) const
	{
		return from_angle(angle);
	}
};

template <typename T>
const Vector2<T> Vector2<T>::Zero(math_type::Zero, math_type::Zero);
template <typename T>
const Vector2<T> Vector2<T>::One(math_type::One, math_type::One);
template <typename T>
const Vector2<T> Vector2<T>::UnitX(math_type::One, math_type::Zero);
template <typename T>
const Vector2<T> Vector2<T>::UnitY(math_type::Zero, math_type::One);
template <typename T>
const Vector2<T> Vector2<T>::Up(math_type::Zero, math_type::One);
template <typename T>
const Vector2<T> Vector2<T>::Down(math_type::Zero, -math_type::One);
template <typename T>
const Vector2<T> Vector2<T>::Right(math_type::One, math_type::Zero);
template <typename T>
const Vector2<T> Vector2<T>::Left(-math_type::One, math_type::Zero);
#if 0
template <typename T>
const Vector2<T> Vector2<T>::Front(math_type::Zero, math_type::Zero);
template <typename T>
const Vector2<T> Vector2<T>::Back(math_type::Zero, math_type::Zero);
#endif

/* 左辺が数値の場合の演算子 */

template <typename T> inline
Vector2<T> operator * (T f, const Vector2<T>& v)
{
	return v * f;
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Vector2<T>& v)
{
	/* (X, Y) */
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Vector2<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Vector2<T>& v)
{
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Vector2<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_VECTOR2_H__ */
