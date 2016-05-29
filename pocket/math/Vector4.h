#ifndef __MATH_VECTOR4_H__
#define __MATH_VECTOR4_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "SIMD.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

#if defined(_USE_SIMD) && defined(_USE_ANONYMOUS)
#	define _USE_SIMD_ANONYMOUS
#endif

namespace pocket
{

template <typename> struct Vector4;
template <typename> struct Matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector4<int> Point4;
typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Vector4<double> Vector4d;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector4<long double> Vector4ld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T>
using vec4 = Vector4<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using vec4f = vec4<float>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using vec4d = vec4<double>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using vec4ld = vec4<long double>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

template <typename T>
struct Vector4
{
	_MATH_STATICAL_ASSERT(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;

	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	typedef simd_t<T> simd_type;
	typedef typename simd_type::type simd_value_type;

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

	// 匿名は使用できないがsimd_typeは使用できる
#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif /* _USE_ANONYMOUS */

			T X;
			T Y;
			T Z;
			T W;

#ifdef _USE_ANONYMOUS
		};

#ifdef _USE_ANONYMOUS_NON_POD
		struct
		{
			Vector2<T> XY;
		};
		struct
		{
			Vector3<T> XYZ;
		};
#endif /* _USE_ANONYMOUS_NON_POD */

#ifdef _USE_SIMD
		// simd_typeが使用できる場合は演算に使用する
		simd_type simd;
#endif /* _USE_SIMD */

		array_type Data;
	};
#endif /* _USE_ANONYMOUS */

	template <typename> friend struct Vector4;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Vector4 Zero; /* 0.0, 0.0, 0.0, 0.0 */
	static const Vector4 One; /* 1.0, 1.0, 1.0, 1.0 */
	static const Vector4 UnitX; /* 1.0, 0.0, 0.0, 0.0 */
	static const Vector4 UnitY; /* 0.0, 1.0, 0.0, 0.0 */
	static const Vector4 UnitZ; /* 0.0, 0.0, 1.0, 0.0 */
	static const Vector4 UnitW; /* 0.0, 0.0, 0.0, 1.0 */
	static const Vector4 Up; /* 0.0, 1.0, 0.0, 0.0 */
	static const Vector4 Down; /* 0.0, -1.0, 0.0, 0.0 */
	static const Vector4 Right; /* 1.0, 0.0, 0.0, 0.0 */
	static const Vector4 Left; /* -1.0, 0.0, 0.0, 0.0 */
	static const Vector4 Front; /* 0.0, 0.0, 1.0, 0.0 */
	static const Vector4 Back; /* 0.0, 0.0, -1.0, 0.0 */

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Vector4);
	explicit Vector4(const behavior::_noinitialize_t&)
	{

	}
	explicit Vector4(const behavior::_zero_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Zero), Y(math_type::Zero), Z(math_type::Zero),
#else
		XYZ(math_type::Zero, math_type::Zero, math_type::Zero),
#endif
		W(math_type::Zero)
	{

	}
	explicit Vector4(const behavior::_zero_t&, const behavior::_direction_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Zero), Y(math_type::Zero), Z(math_type::Zero),
#else
		XYZ(math_type::Zero, math_type::Zero, math_type::Zero),
#endif
		W(math_type::Zero)
	{

	}
	explicit Vector4(const behavior::_zero_t&, const behavior::_position_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Zero), Y(math_type::Zero), Z(math_type::Zero),
#else
		XYZ(math_type::Zero, math_type::Zero, math_type::Zero),
#endif
		W(math_type::One)
	{

	}
	explicit Vector4(const behavior::_one_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::One), Y(math_type::One), Z(math_type::One),
#else
		XYZ(math_type::One, math_type::One, math_type::One),
#endif
		W(math_type::One)
	{

	}
	explicit Vector4(const behavior::_one_t&, const behavior::_direction_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::One), Y(math_type::One), Z(math_type::One),
#else
		XYZ(math_type::One, math_type::One, math_type::One),
#endif
		W(math_type::Zero)
	{

	}
	explicit Vector4(const behavior::_one_t&, const behavior::_position_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::One), Y(math_type::One), Z(math_type::One),
#else
		XYZ(math_type::One, math_type::One, math_type::One),
#endif
		W(math_type::One)
	{

	}
	explicit Vector4(const behavior::_half_t&, const behavior::_direction_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Half), Y(math_type::Half), Z(math_type::Half),
#else
		XYZ(math_type::Half, math_type::Half, math_type::Half),
#endif
		W(math_type::Zero)
	{

	}
	explicit Vector4(const behavior::_half_t&, const behavior::_position_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Half), Y(math_type::Half), Z(math_type::Half),
#else
		XYZ(math_type::Half, math_type::Half, math_type::Half),
#endif
		W(math_type::One)
	{

	}
	explicit Vector4(const behavior::_half_of_half_t&, const behavior::_direction_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::HalfOfHalf), Y(math_type::HalfOfHalf), Z(math_type::HalfOfHalf),
#else
		XYZ(math_type::HalfOfHalf, math_type::HalfOfHalf, math_type::HalfOfHalf),
#endif
		W(math_type::Zero)
	{

	}
	explicit Vector4(const behavior::_half_of_half_t&, const behavior::_position_t&) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::HalfOfHalf), Y(math_type::HalfOfHalf), Z(math_type::HalfOfHalf),
#else
		XYZ(math_type::HalfOfHalf, math_type::HalfOfHalf, math_type::HalfOfHalf),
#endif
		W(math_type::One)
	{

	}

	Vector4(T X, T Y, T Z, T W) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(X), Y(Y), Z(Z),
#else
		XYZ(X, Y, Z),
#endif
		W(W)
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)
	>
		Vector4(U X, U1 Y, U2 Z, U3 W) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(X)), Y(static_cast<T>(Y)), Z(static_cast<T>(Z)),
#else
		XYZ(static_cast<T>(X), static_cast<T>(Y), static_cast<T>(Z)),
#endif
		W(static_cast<T>(W))
	{

	}
	explicit Vector4(T f, T w = math_type::One) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(f), Y(f), Z(f),
#else
		XYZ(f),
#endif
		W(w)
	{

	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit Vector4(U f, U w = Math<U>::One) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(f)), Y(static_cast<T>(f)), Z(static_cast<T>(f)),
#else
		XYZ(static_cast<T>(f)),
#endif
		W(static_cast<T>(w))
	{

	}
	explicit Vector4(const Vector2<T>& v, T z, T w) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(v.X), Y(v.Y), Z(z),
#else
		XYZ(v, z),
#endif
		W(w)
	{

	}
	template <typename U>
	explicit Vector4(const Vector2<U>& v, U z, U w) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)), Z(static_cast<T>(z)),
#else
		XYZ(static_cast<T>(v.X), static_cast<T>(v.Y), static_cast<T>(z)),
#endif
		W(static_cast<T>(w))
	{

	}
	explicit Vector4(const Vector3<T>& v, T w) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(v.Z), Y(v.Y), Z(v.Z),
#else
		XYZ(v),
#endif
		W(w)
	{

	}
	template <typename U>
	explicit Vector4(const Vector3<U>& v, U w) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)), Z(static_cast<T>(v.Z)),
#else
		XYZ(static_cast<T>(v.X), static_cast<T>(v.Y), static_cast<T>(v.Z)),
#endif
		W(static_cast<T>(w))
	{

	}
	template <typename U>
	explicit Vector4(const Vector4<U>& v) :
#ifdef _USE_ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)), Z(static_cast<T>(v.Z)),
#else
		XYZ(static_cast<T>(v.X), static_cast<T>(v.Y), static_cast<T>(v.Z)),
#endif
		W(static_cast<T>(v.W))
	{

	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* X軸回転角度とY軸回転角度からベクトルを求める
	*---------------------------------------------------------------------*/
	Vector4& from_pitch_yaw(T pitch, T yaw)
	{
		X = math_type::cos(pitch) * math_type::sin(yaw);
		Y = math_type::sin(pitch);
		Z = math_type::cos(pitch) * math_type::cos(yaw);
		W = math_type::Zero;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Vector4& add(const Vector4& v, Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		result.simd = simd;
		result.simd += v.simd;
#else
		result.X = X + v.X;
		result.Y = Y + v.Y;
		result.Z = Z + v.Z;
		result.W = W + v.W;
#endif /* _USE_SIMD */
		return result;
	}
	template <typename U>
	Vector4& add(const Vector4<U>& v, Vector4& result) const
	{
		result.X = X + static_cast<T>(v.X);
		result.Y = Y + static_cast<T>(v.Y);
		result.Z = Z + static_cast<T>(v.Z);
		result.W = W + static_cast<T>(v.W);
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	Vector4& subtract(const Vector4& v, Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		result.simd = simd;
		result.simd -= v.simd;
#else
		result.X = X - v.X;
		result.Y = Y - v.Y;
		result.Z = Z - v.Z;
		result.W = W - v.W;
#endif /* _USE_SIMD */
		return result;
	}
	template <typename U>
	Vector4& subtract(const Vector4<U>& v, Vector4& result) const
	{
		result.X = X - static_cast<T>(v.X);
		result.Y = Y - static_cast<T>(v.Y);
		result.Z = Z - static_cast<T>(v.Z);
		result.W = W - static_cast<T>(v.W);
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	Vector4& multiply(T f, Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		result.simd = simd;
		result.simd *= f;
#else
		result.X = X * f;
		result.Y = Y * f;
		result.Z = Z * f;
		result.W = W * f;
#endif /* _USE_SIMD */
		return result;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& multiply(U f, Vector4& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Vector4& divide(T f, Vector4& result) const
	{
		_DEB_ASSERT(f != math_type::Zero);
#ifdef _USE_SIMD_ANONYMOUS
		result.simd = simd;
		result.simd /= f;
		return *this;
#else
		f = math_type::One / f;
		return multiply(f, result);
#endif /* _USE_SIMD */
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& divide(U f, Vector4& result) const
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		return divide(static_cast<T>(f), result);
	}

	/*---------------------------------------------------------------------
	* 平行か
	*---------------------------------------------------------------------*/
	bool is_parallel(const Vector4& v) const
	{
		/* 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向) */
		return math_type::is_near_zero(dot(v) - (length() * v.length()));
	}
	/*---------------------------------------------------------------------
	* 垂直
	*---------------------------------------------------------------------*/
	bool is_vertical(const Vector4& v) const
	{
		return math_type::is_near_zero(dot(v));
	}
	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Vector4& v) const
	{
		return (math_type::is_near(X, v.X) && math_type::is_near(Y, v.Y) && math_type::is_near(Z, v.Z) && math_type::is_near(W, v.W));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(X) && math_type::is_near_zero(Y) && math_type::is_near_zero(Z) && math_type::is_near_zero(W));
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (X == math_type::Zero && Y == math_type::Zero && Z == math_type::Zero && W == math_type::Zero);
	}
	/*---------------------------------------------------------------------
	* 長さを求める
	*---------------------------------------------------------------------*/
	T length() const
	{
#ifdef _USE_SIMD_ANONYMOUS
		simd_type result = {_mm_mul_ps(simd.mm, simd.mm)};
		simd_value_type perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(0, 1, 2, 3));
		result.mm = _mm_add_ps(result.mm, perm);
		perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(1, 0, 3, 2));
		result.mm = _mm_add_ps(result.mm, perm);

		result.mm = _mm_sqrt_ps(result);
		return result.first();
#else
		return math_type::sqrt(dot(*this));
#endif /* _USE_SIMD */
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
		return X + Y + Z + W;
	}
	/*---------------------------------------------------------------------
	* 距離を求める
	*---------------------------------------------------------------------*/
	T distance(const Vector4& v) const
	{
		/* 差を求めたあとの長さ */
		const Vector4 t(X - v.X, Y - v.Y, Z - v.Z, math_type::Zero);
		return t.length();
	}
	/*---------------------------------------------------------------------
	* 方向を求める
	*---------------------------------------------------------------------*/
	Vector4 direction(const Vector4& v) const
	{
		Vector4 t(X - v.X, Y - v.Y, Z - v.Z, math_type::Zero);
		return t.normalize();
	}
	Vector4& direction(const Vector4& v, Vector4& result) const
	{
		result.X = X - v.X;
		result.Y = Y - v.Y;
		result.Z = Z - v.Z;
		result.W = math_type::Zero;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const Vector4& v) const
	{
		/* |v1||v2|cos(θ)と同じになる */
		/* 値が０のときは垂直 */
#ifdef _USE_SIMD_ANONYMOUS
		/* X:X*v.X, Y:Y*v.Y, Z:Z*v.Z, W:W*v.W */
		simd_type result = {_mm_mul_ps(simd.mm, v.simd.mm)};
		/* W, Z, Y, X */
		//simd_type perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(3, 2, 1, 0));
		simd_value_type perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(0, 1, 2, 3));
		/* X+W, Y+Z, Z+Y, W+X */
		result.mm = _mm_add_ps(result.mm, perm);

		/* Z+Y, W+X, X+W, Y+Z */
		//perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(2, 3, 0, 1));
		perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(1, 0, 3, 2));
		/* X+W+Z+Y, Y+Z+W+X, Z+Y+X+W, W+X+Y+Z */
		result.mm = _mm_add_ps(result.mm, perm);

		return result.first();
#else
		return X * v.X + Y * v.Y + Z * v.Z + W * v.W;
#endif /* _USE_SIMD */
	}
	/*---------------------------------------------------------------------
	* 外積を求める
	*---------------------------------------------------------------------*/
	Vector4 cross(const Vector4& v) const
	{
		/* Vector3分でW成分は0とする */
#ifdef _USE_SIMD_ANONYMOUS
		Vector4 result(behavior::noinitialize);
		return cross(v, result);
#else
		return Vector4(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X, math_type::Zero);
#endif /* _USE_SIMD */		
	}
	Vector4& cross(const Vector4& v, Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		const unsigned int YZX = _MM_SHUFFLE(3, 0, 2, 1);
		const unsigned int ZXY = _MM_SHUFFLE(3, 1, 0, 2);
		simd_value_type syzx = _mm_shuffle_ps(simd.mm, simd.mm, YZX);
		simd_value_type szxy = _mm_shuffle_ps(simd.mm, simd.mm, ZXY);
		simd_value_type tzxy = _mm_shuffle_ps(v.simd.mm, v.simd.mm, ZXY);
		simd_value_type tyzx = _mm_shuffle_ps(v.simd.mm, v.simd.mm, YZX);
		result.simd.mm = _mm_sub_ps(_mm_mul_ps(syzx, tzxy), _mm_mul_ps(szxy, tyzx));
#else
		result.X = Y * v.Z - Z * v.Y;
		result.Y = Z * v.X - X * v.Z;
		result.Z = X * v.Y - Y * v.X;
		result.W = math_type::Zero;
#endif /* _USE_SIMD */
		return result;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Vector4& normalize()
	{
#ifdef _USE_SIMD_ANONYMOUS
		simd_type result = {_mm_mul_ps(simd.mm, simd.mm)};
		simd_value_type perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(0, 1, 2, 3));
		result.mm = _mm_add_ps(result.mm, perm);
		perm = _mm_shuffle_ps(result.mm, result.mm, _MM_SHUFFLE(1, 0, 3, 2));
		result.mm = _mm_add_ps(result.mm, perm);

		/* 1.0 / sqrt */
		result.mm = _mm_rsqrt_ps(result.mm);
		simd.mm = _mm_mul_ps(simd.mm, result.mm);
#else
		T len = length_sq();
		/* ゼロ割対策 */
		if (len != math_type::Zero)
		{
			/* 長さの逆数 */
			len = math_type::rsqrt(len);
			X *= len;
			Y *= len;
			Z *= len;
			W *= len;
		}
#endif /* _USE_SIMD */
		return *this;
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	Vector4 normalized() const
	{
		Vector4 v = *this;
		return v.normalize();
	}
	Vector4& normalized(Vector4& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 射影を求める
	*---------------------------------------------------------------------*/
	Vector4 projection(const Vector4& v) const
	{
		return *this * v.dot(*this);
	}
	Vector4& projection(const Vector4& v, Vector4& result) const
	{
		result = *this;
		return result *= v.dot(*this);
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Vector4 lerp(const Vector4& to, T t) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		Vector4 result(behavior::noinitialize);
		return lerp(to, t, result);
#else
		return Vector4(
			math_type::lerp(X, to.X, t),
			math_type::lerp(Y, to.Y, t),
			math_type::lerp(Z, to.Z, t),
			math_type::lerp(W, to.W, t)
		);
#endif /* _USE_SIMD */
	}
	Vector4& lerp(const Vector4& to, T t, Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		/* from*(1.0 - t) + to*t */
		simd_value_type ft = _mm_set1_ps(math_type::One - t);
		simd_value_type tt = _mm_set1_ps(t);
		result.simd.mm = _mm_mul_ps(simd.mm, ft);
		result.simd.mm = _mm_add_ps(result.simd.mm, _mm_mul_ps(to.simd.mm, tt));
#else
		result.X = math_type::lerp(X, to.X, t);
		result.Y = math_type::lerp(Y, to.Y, t);
		result.Z = math_type::lerp(Z, to.Z, t);
		result.W = math_type::lerp(W, to.W, t);
#endif
		return result;
	}
	Vector4& lerp(const Vector4& from, const Vector4& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 値を0～1にクランプ
	*---------------------------------------------------------------------*/
	Vector4& saturate()
	{
#ifdef _USE_SIMD_ANONYMOUS
		const simd_value_type zero = _mm_setzero_ps();
		const simd_value_type one = _mm_set1_ps(math_type::One);
		simd.mm = _mm_max_ps(zero, _mm_min_ps(simd.mm, one));
#else
		X = math_type::clamp01(X);
		Y = math_type::clamp01(Y);
		Z = math_type::clamp01(Z);
		W = math_type::clamp01(W);
#endif /* _USE_SIMD */
		return *this;
	}
	Vector4 saturated() const
	{
		Vector4 result(behavior::noinitialize);
		return saturated(result);
	}
	Vector4& saturated(Vector4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		result.simd = simd;
		result.saturate();
#else
		result.X = math_type::clamp01(X);
		result.Y = math_type::clamp01(Y);
		result.Z = math_type::clamp01(Z);
		result.W = math_type::clamp01(W);
#endif /* _USE_SIMD */
		return result;
	}
	/*---------------------------------------------------------------------
	* X軸回転角度を求める
	*---------------------------------------------------------------------*/
	T pitch() const
	{
		/* Asin(Y / |v|) */

		T len = length_sq();
		if (len != math_type::Zero)
		{
			return math_type::ASin(-Y / math_type::sqrt(len));
		}
		return math_type::Zero;
	}
	/*---------------------------------------------------------------------
	* Y軸回転角度を求める
	*---------------------------------------------------------------------*/
	T yaw() const
	{
		if (length_sq() != math_type::Zero)
		{
			/* atan2は正規化されていなくてもいい */
			return math_type::atan2(X, Z);
		}
		return math_type::Zero;
	}
	/*---------------------------------------------------------------------
	* 座標変換
	*---------------------------------------------------------------------*/
	Vector4& transform(const Matrix4x4<T>&); /* Matrix4x4.h */
	Vector4 transformed(const Matrix4x4<T>&) const;
	Vector4& transformed(const Matrix4x4<T>&, Vector4& result) const;
	Vector4& transform_coord(const Matrix4x4<T>&); /* Matrix4x4.h */
	Vector4 transformed_coord(const Matrix4x4<T>&) const;
	Vector4& transformed_coord(const Matrix4x4<T>&, Vector4& result) const;
	/*---------------------------------------------------------------------
	* 回転
	*---------------------------------------------------------------------*/
	Vector4& rotate(const Quaternion<T>&);
	Vector4 rotated(const Quaternion<T>&) const;
	Vector4& rotated(const Quaternion<T>&, Vector4<T>& result) const;
	Vector4 rotated(const Vector3<T>& axis, T angle) const
	{
		Vector4 result(behavior::noinitialize);
		return rotated(axis, angle, result);
	}
	Vector4& rotated(const Vector3<T>& axis, T angle, Vector4& result) const
	{
		reinterpret_cast<const Vector3<T>&>(*this).rotate(axis, angle, reinterpret_cast<Vector3<T>&>(result));
		result.W = W;

		return result;
	}
	Vector4& rotate(const Vector3<T>& axis, T angle)
	{
		const Vector4 c(*this);
		return c.rotated(axis, angle, *this);
	}

	/*---------------------------------------------------------------------
	* 並べ替え
	*---------------------------------------------------------------------*/
	Vector2<T> swizzle(int x, int y) const
	{
		_DEB_RANGE_ASSERT(x, 0, 3);
		_DEB_RANGE_ASSERT(y, 0, 3);
		return Vector2<T>((*this)[x], (*this)[y]);
	}
	Vector3<T> swizzle(int x, int y, int z) const
	{
		_DEB_RANGE_ASSERT(x, 0, 3);
		_DEB_RANGE_ASSERT(y, 0, 3);
		_DEB_RANGE_ASSERT(z, 0, 3);
		return Vector3<T>((*this)[x], (*this)[y], (*this)[z]);
	}
	Vector4 swizzle(int x, int y, int z, int w) const
	{
		_DEB_RANGE_ASSERT(x, 0, 3);
		_DEB_RANGE_ASSERT(y, 0, 3);
		_DEB_RANGE_ASSERT(z, 0, 3);
		_DEB_RANGE_ASSERT(w, 0, 3);
		return Vector4((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
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
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif /* _USE_ANONYMOUS */
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
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
	_CXX11_EXPLICIT operator Vector4<U>() const
	{
		return Vector4<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z), static_cast<U>(W));
	}
	_CXX11_EXPLICIT operator Vector3<T>() const
	{
		return Vector3<T>(X, Y, Z);
	}
	template <typename U>
	_CXX11_EXPLICIT operator Vector3<U>() const
	{
		return Vector3<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z));
	}
	_CXX11_EXPLICIT operator Vector2<T>() const
	{
		return Vector2<T>(X, Y);
	}
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
	bool operator == (const Vector4& v) const
	{
		return (X == v.X) && (Y == v.Y) && (Z == v.Z) && (W == v.W);
	}
	bool operator != (const Vector4& v) const
	{
		return !(*this == v);
	}
	bool operator < (const Vector4& v) const
	{
		return length_sq() < v.length_sq();
	}
	bool operator <= (const Vector4& v) const
	{
		return length_sq() <= v.length_sq();
	}
	bool operator > (const Vector4& v) const
	{
		return length_sq() > v.length_sq();
	}
	bool operator >= (const Vector4& v) const
	{
		return length_sq() >= v.length_sq();
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Vector4 operator + () const
	{
		return *this;
	}
	Vector4 operator - () const
	{
		return Vector4(-X, -Y, -Z, -W);
	}
	Vector4& operator ++ ()
	{
		++X;
		++Y;
		++Z;
		++W;
		return *this;
	}
	Vector4 operator ++ (int)
	{
		Vector4 r(*this);
		++*this;
		return r;
	}
	Vector4& operator -- ()
	{
		--X;
		--Y;
		--Z;
		--W;
		return *this;
	}
	Vector4 operator -- (int)
	{
		Vector4 r(*this);
		--*this;
		return r;
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Vector4 operator + (const Vector4& v) const
	{
		Vector4 result(behavior::noinitialize);
		return add(v, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4 operator + (const Vector4<U>& v) const
	{
		Vector4 result(behavior::noinitialize);
		return add<U>(v, result);
	}
	Vector4 operator - (const Vector4& v) const
	{
		Vector4 result(behavior::noinitialize);
		return subtract(v, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4 operator - (const Vector4<U>& v) const
	{
		Vector4 result(behavior::noinitialize);
		return subtract<U>(v, result);
	}
	Vector4 operator * (const Matrix4x4<T>&) const; /* Matrix4x4.h */
	Vector4 operator * (const Quaternion<T>&) const; /* Quaternion.h */
	Vector4 operator * (T f) const
	{
		Vector4 result(behavior::noinitialize);
		return multiply(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4 operator * (U f) const
	{
		Vector4 result(behavior::noinitialize);
		return multiply<U>(f, result);
	}
	Vector4 operator / (T f) const
	{
		Vector4 result(behavior::noinitialize);
		return divide(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4 operator / (U f) const
	{
		Vector4 result(behavior::noinitialize);
		return divide<U>(f, result);
	}
	Vector4 operator % (T f) const
	{
		return Vector4(math_type::remainder(X, f), math_type::remainder(Y, f), math_type::remainder(Z, f), math_type::remainder(W, f));
	}
	Vector4 operator % (const Vector4& v) const
	{
		return Vector4(math_type::remainder(X, v.X), math_type::remainder(Y, v.Y), math_type::remainder(Z, v.Z), math_type::remainder(W, v.W));
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Vector4& operator = (T f)
	{
		X = Y = Z = W = f;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& operator = (U f)
	{
		*this = static_cast<T>(f);
		return *this;
	}
	Vector4& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector4& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector4& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector4& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Vector4& operator += (const Vector4& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		W += v.W;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& operator += (const Vector4<U>& v)
	{
		X += static_cast<T>(v.X);
		Y += static_cast<T>(v.Y);
		Z += static_cast<T>(v.Z);
		W += static_cast<T>(v.W);
		return *this;
	}
	Vector4& operator -= (const Vector4& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		W -= v.W;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& operator -= (const Vector4<U>& v)
	{
		X -= static_cast<T>(v.X);
		Y -= static_cast<T>(v.Y);
		Z -= static_cast<T>(v.Z);
		W -= static_cast<T>(v.W);
		return *this;
	}
	Vector4& operator *= (T f)
	{
		X *= f;
		Y *= f;
		Z *= f;
		W *= f;
		return *this;
	}
	Vector4& operator *= (const Matrix4x4<T>& m)
	{
		return transform(m);
	}
	Vector4& operator *= (const Quaternion<T>& q)
	{
		return rotate(q);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& operator *= (U f)
	{
		X *= static_cast<T>(f);
		Y *= static_cast<T>(f);
		Z *= static_cast<T>(f);
		return *this;
	}
	Vector4& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return *this *= f;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector4& operator /= (U f)
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		return *this *= static_cast<T>(f);
	}
	Vector4& operator %= (T f)
	{
		math_type::remainder_assign(X, f);
		math_type::remainder_assign(Y, f);
		math_type::remainder_assign(Z, f);
		math_type::remainder_assign(W, f);
		return *this;
	}
	Vector4& operator %= (const Vector4& v)
	{
		math_type::remainder_assign(X, v.X);
		math_type::remainder_assign(Y, v.Y);
		math_type::remainder_assign(Z, v.Z);
		math_type::remainder_assign(W, v.W);
		return *this;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Vector4& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector4& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector4& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector4& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	Vector4 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	Vector4 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	Vector4 operator () (const behavior::_add_t&, const Vector4& v) const
	{
		return operator+(v);
	}
	Vector4 operator () (const behavior::_sub_t&, const Vector4& v) const
	{
		return operator-(v);
	}
	Vector4 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Vector4 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	Vector4 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	Vector4 operator () (const behavior::_rem_t&, const Vector4& v) const
	{
		return operator%(v);
	}

	Vector4& operator () (const behavior::_add_assign_t&, const Vector4& v)
	{
		return operator+=(v);
	}
	Vector4& operator () (const behavior::_sub_assign_t&, const Vector4& v)
	{
		return operator-=(v);
	}
	Vector4& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Vector4& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	Vector4& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	Vector4& operator () (const behavior::_rem_assign_t&, const Vector4& v)
	{
		return operator%=(v);
	}
	Vector4& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	Vector4 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	Vector4& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	Vector4 operator () (const behavior::_decrement_back_t&)
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

	bool operator () (const behavior::_equal_t&, const Vector4& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const Vector4& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const Vector4& v) const
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
	T operator () (const behavior::_dot_t&, const Vector4& v) const
	{
		return dot(v);
	}
	Vector4& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	Vector4 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	Vector4 operator () (const behavior::_cross_t&, const Vector4& v) const
	{
		return cross(v);
	}
	Vector4 operator () (const behavior::_lerp_t&, const Vector4& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const Vector4& v) const
	{
		return distance(v);
	}
	Vector4 operator () (const behavior::_direction_t&, const Vector4& v) const
	{
		return direction(v);
	}
	Vector4 operator () (const behavior::_direction_t&, const Vector4& v, T force) const
	{
		return direction(v) * force;
	}
	Vector4& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	Vector4 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	Vector2<T> operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	Vector3<T> operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	Vector4 operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
	{
		return swizzle(x, y, z, w);
	}

	T operator () (const behavior::_yaw_t&) const
	{
		return yaw();
	}
	T operator () (const behavior::_pitch_t&) const
	{
		return pitch();
	}
	Vector4& operator () (const behavior::_from_pitch_yaw_t&, T pitch, T yaw)
	{
		return from_pitch_yaw(pitch, yaw);
	}
	Vector4 operator () (const behavior::_transform_t&, const Matrix4x4<T>& m) const
	{
		return transformed(m);
	}
	Vector4 operator () (const behavior::_transform_coord_t&, const Matrix4x4<T>& m) const
	{
		return transformed_coord(m);
	}
	Vector4 operator () (const behavior::_rotate_t&, const Quaternion<T>& q) const
	{
		return rotated(q);
	}
	Vector4 operator () (const behavior::_rotate_t&, const Vector3<T>& axis, T angle) const
	{
		return rotated(axis, angle);
	}
};

template <typename T>
const Vector4<T> Vector4<T>::Zero(math_type::Zero, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::One(math_type::One, math_type::One, math_type::One, math_type::One);
template <typename T>
const Vector4<T> Vector4<T>::UnitX(math_type::One, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::UnitY(math_type::Zero, math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::UnitZ(math_type::Zero, math_type::Zero, math_type::One, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::UnitW(math_type::Zero, math_type::Zero, math_type::Zero, math_type::One);
template <typename T>
const Vector4<T> Vector4<T>::Up(math_type::Zero, math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::Down(math_type::Zero, -math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::Right(math_type::One, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::Left(-math_type::One, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::Front(math_type::Zero, math_type::Zero, math_type::One, math_type::Zero);
template <typename T>
const Vector4<T> Vector4<T>::Back(math_type::Zero, math_type::Zero, -math_type::One, math_type::Zero);

/* 左辺が数値の場合の演算子 */

template <typename T> inline
Vector4<T> operator * (T f, const Vector4<T>& v)
{
	return v * f;
}

/* Vector2がVector4からの値を取得するコンストラクタ */

template <typename T> inline
Vector2<T>::Vector2(const Vector4<T>& v) :
	X(v.X), Y(v.Y)
{

}
template <typename T>
template <typename U> inline
Vector2<T>::Vector2(const Vector4<U>& v) :
	X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y))
{

}

template <typename T> inline
Vector4<T> Vector2<T>::swizzle(int x, int y, int z, int w) const
{
	_DEB_ASSERT(x < 2 && y < 2 && z < 2 && w < 2);
	return Vector4<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
}

/* Vector3がVector4から値を取得するコンストラクタ */

template <typename T> inline
Vector3<T>::Vector3(const Vector4<T>& v) :
#ifndef _USE_ANONYMOUS_NON_POD
	X(v.X), Y(v.Y),
#else
	XY(v.X, v.Y),
#endif
	Z(v.Z)
{

}
template <typename T>
template <typename U> inline
Vector3<T>::Vector3(const Vector4<U>& v) :
#ifndef _USE_ANONYMOUS_NON_POD
	X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)),
#else
	XY(static_cast<T>(v.X), static_cast<T>(v.Y)),
#endif
	Z(static_cast<T>(v.Z))
{

}

template <typename T> inline
Vector4<T> Vector3<T>::swizzle(int x, int y, int z, int w) const
{
	_DEB_ASSERT(x < 3 && y < 3 && z < 3 && w < 3);
	return Vector4<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Vector4<T>& v)
{
	/* (X, Y, Z, W) */
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::comma_space
		<< v.W << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Vector4<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	is >> v.Z;
	is.ignore();
	is >> v.W;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Vector4<T>& v)
{
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::comma_space
		<< v.W << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Vector4<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	is >> v.Z;
	is.ignore();
	is >> v.W;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_VECTOR4_H__ */
