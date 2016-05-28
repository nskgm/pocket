#ifndef __MATH_VECTOR3_H__
#define __MATH_VECTOR3_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "Math.h"
#include "Vector2.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename> struct Vector3;
template <typename> struct Vector4;
template <typename> struct Matrix4x4;
template <typename> struct Quaternion;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector3<int> Point3;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Vector3<double> Vector3d;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector3<long double> Vector3ld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T>
using vec3 = Vector3<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using vec3f = vec3<float>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using vec3d = vec3<double>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using vec3ld = vec3<long double>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

template <typename T>
struct Vector3
{
	_MATH_STATICAL_ASSERT(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;

	typedef container::array<T, 3> array_type;
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
			T Z;

#ifdef _USE_ANONYMOUS
		};
		struct
		{
			Vector2<T> XY;
		};
		array_type Data;
	};
#endif /* _USE_ANONYMOUS */

	template <typename> friend struct Vector3;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Vector3 Zero; /* 0.0, 0.0, 0.0 */
	static const Vector3 One; /* 1.0, 1.0, 1.0 */
	static const Vector3 UnitX; /* 1.0, 0.0, 0.0 */
	static const Vector3 UnitY; /* 0.0, 1.0, 0.0 */
	static const Vector3 UnitZ; /* 0.0, 0.0, 1.0 */
	static const Vector3 Up; /* 0.0, 1.0, 0.0 */
	static const Vector3 Down; /* 0.0, -1.0, 0.0 */
	static const Vector3 Right; /* 1.0, 0.0, 0.0 */
	static const Vector3 Left; /* -1.0, 0.0, 0.0 */
	static const Vector3 Front; /* 0.0, 0.0, 1.0 */
	static const Vector3 Back; /* 0.0, 0.0, -1.0 */

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Vector3);
	explicit Vector3(const behavior::_noinitialize_t&)
	{

	}
	explicit Vector3(const behavior::_zero_t&) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Zero), Y(math_type::Zero),
#else
		XY(math_type::Zero, math_type::Zero),
#endif
		Z(math_type::Zero)
	{

	}
	explicit Vector3(const behavior::_one_t&) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::One), Y(math_type::One),
#else
		XY(math_type::One, math_type::One),
#endif
		Z(math_type::One)
	{

	}
	explicit Vector3(const behavior::_half_t&) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::Half), Y(math_type::Half),
#else
		XY(math_type::Half, math_type::Half),
#endif
		Z(math_type::Half)
	{

	}
	explicit Vector3(const behavior::_half_of_half_t&) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(math_type::HalfOfHalf), Y(math_type::HalfOfHalf),
#else
		XY(math_type::HalfOfHalf, math_type::HalfOfHalf),
#endif
		Z(math_type::HalfOfHalf)
	{

	}

	Vector3(T X, T Y, T Z) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(X), Y(Y),
#else
		XY(X, Y),
#endif
		Z(Z)
	{

	}
	template <typename U, typename U1, typename U2,
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2)
	>
		Vector3(U X, U1 Y, U2 Z) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(X)), Y(static_cast<T>(Y)),
#else
		XY(static_cast<T>(X), static_cast<T>(Y)),
#endif
		Z(static_cast<T>(Z))
	{

	}
	explicit Vector3(T f) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(f), Y(f),
#else
		XY(f),
#endif
		Z(f)
	{

	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit Vector3(U f) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(f)), Y(static_cast<T>(f)),
#else
		XY(static_cast<T>(f)),
#endif
		Z(static_cast<T>(f))
	{

	}
	Vector3(const Vector2<T>& v, T Z) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(v.X), Y(v.Y),
#else
		XY(v),
#endif
		Z(Z)
	{

	}
	template <typename U>
	Vector3(const Vector2<U>& v, U Z) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)),
#else
		XY(static_cast<T>(v.X), static_cast<T>(v.Y)),
#endif
		Z(static_cast<T>(Z))
	{

	}
	template <typename U>
	Vector3(const Vector3<U>& v) :
#ifdef _ANONYMOUS_NORMAL_CONSTRUCT
		X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)),
#else
		XY(static_cast<T>(v.X), static_cast<T>(v.Y)),
#endif
		Z(static_cast<T>(v.Z))
	{

	}

	explicit Vector3(const Vector4<T>&);
	template <typename U> explicit Vector3(const Vector4<U>&); /* Vector4.h */

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* X軸回転角度とY軸回転角度から求める
	*---------------------------------------------------------------------*/
	Vector3& from_pitch_yaw(T pitch, T yaw)
	{
		X = math_type::cos(pitch) * math_type::sin(yaw);
		Y = math_type::sin(pitch);
		Z = math_type::cos(pitch) * math_type::cos(yaw);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Vector3& add(const Vector3& v, Vector3& result) const
	{
		result.X = X + v.X;
		result.Y = Y + v.Y;
		result.Z = Z + v.Z;
		return result;
	}
	template <typename U>
	Vector3& add(const Vector3<U>& v, Vector3& result) const
	{
		result.X = X + static_cast<T>(v.X);
		result.Y = Y + static_cast<T>(v.Y);
		result.Z = Z + static_cast<T>(v.Z);
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	Vector3& subtract(const Vector3& v, Vector3& result) const
	{
		result.X = X - v.X;
		result.Y = Y - v.Y;
		result.Z = Z - v.Z;
		return result;
	}
	template <typename U>
	Vector3& subtract(const Vector3<U>& v, Vector3& result) const
	{
		result.X = X - static_cast<T>(v.X);
		result.Y = Y - static_cast<T>(v.Y);
		result.Z = Z - static_cast<T>(v.Z);
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	Vector3& multiply(T f, Vector3& result) const
	{
		result.X = X * f;
		result.Y = Y * f;
		result.Z = Z * f;
		return result;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3& multiply(U f, Vector3& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Vector3& divide(T f, Vector3& result) const
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return multiply(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3& divide(U f, Vector3& result) const
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		return divide(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 平行か
	*---------------------------------------------------------------------*/
	bool is_parallel(const Vector3& v) const
	{
		/* 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向) */
		return math_type::is_near_zero(dot(v) - (length() * v.length()));
	}
	/*---------------------------------------------------------------------
	* 垂直か
	*---------------------------------------------------------------------*/
	bool is_vertical(const Vector3& v) const
	{
		return math_type::is_near_zero(dot(v));
	}
	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Vector3& v) const
	{
		return (math_type::is_near(X, v.X) && math_type::is_near(Y, v.Y) && math_type::is_near(Z, v.Z));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (math_type::is_near_zero(X) && math_type::is_near_zero(Y) && math_type::is_near_zero(Z));
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (X == math_type::Zero && Y == math_type::Zero && Z == math_type::Zero);
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
		return X + Y + Z;
	}
	/*---------------------------------------------------------------------
	* 距離を求める
	*---------------------------------------------------------------------*/
	T distance(const Vector3& v) const
	{
		/* 差を求めたあとの長さ */
		const Vector3 t(X - v.X, Y - v.Y, Z - v.Z);
		return t.length();
	}
	/*---------------------------------------------------------------------
	* 方向を求める
	*---------------------------------------------------------------------*/
	Vector3 direction(const Vector3& v) const
	{
		Vector3 t(X - v.X, Y - v.Y, Z - v.Z);
		return t.normalize();
	}
	Vector3& direction(const Vector3& v, Vector3& result) const
	{
		result.X = X - v.X;
		result.Y = Y - v.Y;
		result.Z = Z - v.Z;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const Vector3& v) const
	{
		/* |v1||v2|cos(θ)と同じになる */
		/* 値が０のときは垂直 */
		return X * v.X + Y * v.Y + Z * v.Z;
	}
	/*---------------------------------------------------------------------
	* 外積を求める
	*---------------------------------------------------------------------*/
	Vector3 cross(const Vector3& v) const
	{
		/* 二つのベクトルに垂直なベクトルを求める */
		/* ２Ｄでは高さを求めていたけどそれの応用でぞれぞれの軸の高さを求める */
		return Vector3(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
	}
	Vector3& cross(const Vector3& v, Vector3& result) const
	{
		result.X = Y * v.Z - Z * v.Y;
		result.Y = Z * v.X - X * v.Z;
		result.Z = X * v.Y - Y * v.X;
		return result;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Vector3& normalize()
	{
		T len = length_sq();
		if (len != math_type::Zero)
		{
			/* 長さの逆数 */
			len = math_type::rsqrt(len);
			X *= len;
			Y *= len;
			Z *= len;
		}
		return *this;
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	Vector3 normalized() const
	{
		Vector3 v = *this;
		return v.normalize();
	}
	Vector3& normalized(Vector3& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 射影を求める
	*---------------------------------------------------------------------*/
	Vector3 projection(const Vector3& v) const
	{
		return *this * v.dot(*this);
	}
	Vector3& projection(const Vector3& v, Vector3& result) const
	{
		result = *this;
		return result *= v.dot(*this);
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Vector3 lerp(const Vector3& to, T t) const
	{
		return Vector3(math_type::lerp(X, to.X, t), math_type::lerp(Y, to.Y, t), math_type::lerp(Z, to.Z, t));
	}
	Vector3& lerp(const Vector3& to, T t, Vector3& result) const
	{
		result.X = math_type::lerp(X, to.X, t);
		result.Y = math_type::lerp(Y, to.Y, t);
		result.Z = math_type::lerp(Z, to.Z, t);
		return result;
	}
	Vector3& lerp(const Vector3& from, const Vector3& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 値を0～1にクランプ
	*---------------------------------------------------------------------*/
	Vector3& saturate()
	{
		X = math_type::Clamp01(X);
		Y = math_type::Clamp01(Y);
		Z = math_type::Clamp01(Z);
		return *this;
	}
	Vector3 saturated() const
	{
		return Vector3(math_type::Clamp01(X), math_type::Clamp01(Y), math_type::Clamp01(Z));
	}
	Vector3& saturated(Vector3& result) const
	{
		result.X = math_type::Clamp01(X);
		result.Y = math_type::Clamp01(Y);
		result.Z = math_type::Clamp01(Z);
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
			return math_type::asin(-Y / math_type::sqrt(len));
		}
		return math_type::Zero;
	}
	/*---------------------------------------------------------------------
	* Y軸回転角度を求める
	*---------------------------------------------------------------------*/
	T Yaw() const
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
	Vector3& transform(const Matrix4x4<T>&); /* Matrix4x4.h */
	Vector3 transformed(const Matrix4x4<T>&) const;
	Vector3& transformed(const Matrix4x4<T>&, Vector3& result) const;
	Vector3& transform_coord(const Matrix4x4<T>&);
	Vector3 transformed_coord(const Matrix4x4<T>&) const;
	Vector3& transformed_coord(const Matrix4x4<T>&, Vector3& result) const;
	Vector3& transform_normal(const Matrix4x4<T>&);
	Vector3 transformed_normal(const Matrix4x4<T>&) const;
	Vector3& transformed_normal(const Matrix4x4<T>&, Vector3& result) const;
	Vector3& transform(const Matrix3x3<T>&); /* Matrix3x3.h */
	Vector3 transformed(const Matrix3x3<T>&) const;
	Vector3& transformed(const Matrix3x3<T>&, Vector3& result) const;
	Vector3& transform_normal(const Matrix3x3<T>&);
	Vector3 transformed_normal(const Matrix3x3<T>&) const;
	Vector3& transformed_normal(const Matrix3x3<T>&, Vector3& result) const;
	/*---------------------------------------------------------------------
	* 回転
	*---------------------------------------------------------------------*/
	Vector3& rotate(const Quaternion<T>&);
	Vector3 rotated(const Quaternion<T>&) const;
	Vector3& rotated(const Quaternion<T>&, Vector3& result) const;
	Vector3 rotated(const Vector3& axis, T angle) const
	{
		Vector3 result(behavior::noinitialize);
		return rotated(axis, angle, result);
	}
	Vector3& rotated(const Vector3& axis, T angle, Vector3& result) const
	{
		// N(N,V) + [V-N(N,V)]cosθ - (V×N)sinθ

		Vector3 calc(behavior::noinitialize);

		T s = math_type::sin(angle);
		T c = math_type::cos(angle);

		// N(N, V)
		// 射影した長さ
		// 軸方向に水平なベクトル
		axis.multiply(axis.dot(*this), result);

		// [V-N(N,V)]cosθ
		// 軸方向からのベクトルを求める
		subtract(result, calc);
		calc *= c;
		result += calc;

		// 軸と現在のベクトルの外積を引く
		axis.cross(*this, calc);
		calc *= s;
		result -= calc;

		return result;
	}
	Vector3& rotate(const Vector3& axis, T angle)
	{
		const Vector3 c(*this);
		return c.rotated(axis, angle, *this);
	}

	/*---------------------------------------------------------------------
	* 並べ替え
	*---------------------------------------------------------------------*/
	Vector2<T> swizzle(int x, int y) const
	{
		_DEB_RANGE_ASSERT(x, 0, 2);
		_DEB_RANGE_ASSERT(y, 0, 2);
		return Vector2<T>((*this)[x], (*this)[y]);
	}
	Vector3 swizzle(int x, int y, int z) const
	{
		_DEB_RANGE_ASSERT(x, 0, 2);
		_DEB_RANGE_ASSERT(y, 0, 2);
		_DEB_RANGE_ASSERT(z, 0, 2);
		return Vector3((*this)[x], (*this)[y], (*this)[z]);
	}
	Vector4<T> swizzle(int x, int y, int z, int w) const; /* Vector4.h */

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif /* _USE_ANONYMOUS */
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
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
	bool operator == (const Vector3& v) const
	{
		return (X == v.X) && (Y == v.Y) && (Z == v.Z);
	}
	bool operator != (const Vector3& v) const
	{
		return !(*this == v);
	}
	bool operator < (const Vector3& v) const
	{
		return length_sq() < v.length_sq();
	}
	bool operator <= (const Vector3& v) const
	{
		return length_sq() <= v.length_sq();
	}
	bool operator > (const Vector3& v) const
	{
		return length_sq() > v.length_sq();
	}
	bool operator >= (const Vector3& v) const
	{
		return length_sq() >= v.length_sq();
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Vector3 operator + () const
	{
		return *this;
	}
	Vector3 operator - () const
	{
		return Vector3(-X, -Y, -Z);
	}
	Vector3& operator ++ ()
	{
		++X;
		++Y;
		++Z;
		return *this;
	}
	Vector3 operator ++ (int)
	{
		Vector3 r(*this);
		++*this;
		return r;
	}
	Vector3& operator -- ()
	{
		--X;
		--Y;
		--Z;
		return *this;
	}
	Vector3 operator -- (int)
	{
		Vector3 r(*this);
		--*this;
		return r;
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Vector3 operator + (const Vector3& v) const
	{
		Vector3 result(behavior::noinitialize);
		return add(v, result);
	}
	template <typename U>
	Vector3 operator + (const Vector3<U>& v) const
	{
		Vector3 result(behavior::noinitialize);
		return add(v, result);
	}
	Vector3 operator - (const Vector3& v) const
	{
		Vector3 result(behavior::noinitialize);
		return subtract(v, result);
	}
	template <typename U>
	Vector3 operator - (const Vector3<U>& v) const
	{
		Vector3 result(behavior::noinitialize);
		return subtract(v, result);
	}
	Vector3 operator * (const Matrix3x3<T>&) const; /* Matrix3x3.h */
	Vector3 operator * (const Matrix4x4<T>&) const; /* Matrix4x4.h */
	Vector3 operator * (const Quaternion<T>&) const; /* Quaternion.h */
	Vector3 operator * (T f) const
	{
		Vector3 result(behavior::noinitialize);
		return multiply(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3 operator * (U f) const
	{
		Vector3 result(behavior::noinitialize);
		return multiply<U>(f, result);
	}
	Vector3 operator / (T f) const
	{
		Vector3 result(behavior::noinitialize);
		return divide(f, result);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3 operator / (U f) const
	{
		Vector3 result(behavior::noinitialize);
		return divide(f, result);
	}
	Vector3 operator % (T f) const
	{
		return Vector3(math_type::remainder(X, f), math_type::remainder(Y, f), math_type::remainder(Z, f));
	}
	Vector3 operator % (const Vector3& v) const
	{
		return Vector3(math_type::remainder(X, v.X), math_type::remainder(Y, v.Y), math_type::remainder(Z, v.Z));
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Vector3& operator = (T f)
	{
		X = Y = Z = f;
		return *this;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3& operator = (U f)
	{
		*this = static_cast<T>(f);
		return *this;
	}
	Vector3& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector3& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector3& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector3& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Vector3& operator += (const Vector3& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}
	template <typename U>
	Vector3& operator += (const Vector3<U>& v)
	{
		X += static_cast<T>(v.X);
		Y += static_cast<T>(v.Y);
		Z += static_cast<T>(v.Z);
		return *this;
	}
	Vector3& operator -= (const Vector3& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}
	template <typename U>
	Vector3& operator -= (const Vector3<U>& v)
	{
		X -= static_cast<T>(v.X);
		Y -= static_cast<T>(v.Y);
		Z -= static_cast<T>(v.Z);
		return *this;
	}
	Vector3& operator *= (T f)
	{
		X *= f;
		Y *= f;
		Z *= f;
		return *this;
	}
	Vector3& operator *= (const Matrix4x4<T>& m)
	{
		return transform(m);
	}
	Vector3& operator *= (const Quaternion<T>& q)
	{
		return rotate(q);
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3& operator *= (U f)
	{
		X *= static_cast<T>(f);
		Y *= static_cast<T>(f);
		Z *= static_cast<T>(f);
		return *this;
	}
	Vector3& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::Zero);
		T inv = math_type::One / f;
		return *this *= inv;
	}
	template <typename U, _TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	Vector3& operator /= (U f)
	{
		_DEB_ASSERT(f != Math<U>::Zero);
		T inv = math_type::One / static_cast<T>(f);
		return *this *= inv;
	}
	Vector3& operator %= (T f)
	{
		math_type::remainder_assign(X, f);
		math_type::remainder_assign(Y, f);
		math_type::remainder_assign(Z, f);
		return *this;
	}
	Vector3& operator %= (const Vector3& v)
	{
		math_type::remainder_assign(X, v.X);
		math_type::remainder_assign(Y, v.Y);
		math_type::remainder_assign(Z, v.Z);
		return *this;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Vector3& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::Zero);
	}
	Vector3& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::One);
	}
	Vector3& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::Half);
	}
	Vector3& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::HalfOfHalf);
	}

	Vector3 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	Vector3 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	Vector3 operator () (const behavior::_add_t&, const Vector3& v) const
	{
		return operator+(v);
	}
	Vector3 operator () (const behavior::_sub_t&, const Vector3& v) const
	{
		return operator-(v);
	}
	Vector3 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Vector3 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	Vector3 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	Vector3 operator () (const behavior::_rem_t&, const Vector3& v) const
	{
		return operator%(v);
	}

	Vector3& operator () (const behavior::_add_assign_t&, const Vector3& v)
	{
		return operator+=(v);
	}
	Vector3& operator () (const behavior::_sub_assign_t&, const Vector3& v)
	{
		return operator-=(v);
	}
	Vector3& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Vector3& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	Vector3& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	Vector3& operator () (const behavior::_rem_assign_t&, const Vector3& v)
	{
		return operator%=(v);
	}
	Vector3& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	Vector3 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	Vector3& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	Vector3 operator () (const behavior::_decrement_back_t&)
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

	bool operator () (const behavior::_equal_t&, const Vector3& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const Vector3& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const Vector3& v) const
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
	T operator () (const behavior::_dot_t&, const Vector3& v) const
	{
		return dot(v);
	}
	Vector3& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	Vector3 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	Vector3 operator () (const behavior::_cross_t&, const Vector3& v) const
	{
		return cross(v);
	}
	Vector3 operator () (const behavior::_lerp_t&, const Vector3& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const Vector3& v) const
	{
		return distance(v);
	}
	Vector3 operator () (const behavior::_direction_t&, const Vector3& v) const
	{
		return direction(v);
	}
	Vector3 operator () (const behavior::_direction_t&, const Vector3& v, T force) const
	{
		return direction(v) * force;
	}
	Vector3& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	Vector3 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	Vector2<T> operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	Vector3 operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	Vector4<T> operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
	{
		return swizzle(x, y, z, w);
	}

	T operator () (const behavior::_yaw_t&) const
	{
		return Yaw();
	}
	T operator () (const behavior::_pitch_t&) const
	{
		return pitch();
	}
	Vector3& operator () (const behavior::_from_pitch_yaw_t&, T pitch, T yaw)
	{
		return from_pitch_yaw(pitch, yaw);
	}
	Vector3 operator () (const behavior::_transform_t&, const Matrix4x4<T>& m) const
	{
		return transformed(m);
	}
	Vector3 operator () (const behavior::_transform_coord_t&, const Matrix4x4<T>& m) const
	{
		return transformed_coord(m);
	}
	Vector3 operator () (const behavior::_transform_normal_t&, const Matrix4x4<T>& m) const
	{
		return transformed_normal(m);
	}
	Vector3 operator () (const behavior::_rotate_t&, const Quaternion<T>& q) const
	{
		return rotated(q);
	}
	Vector3 operator () (const behavior::_rotate_t&, const Vector3& axis, T angle) const
	{
		return rotated(axis, angle);
	}
};

template <typename T>
const Vector3<T> Vector3<T>::Zero(math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::One(math_type::One, math_type::One, math_type::One);
template <typename T>
const Vector3<T> Vector3<T>::UnitX(math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::UnitY(math_type::Zero, math_type::One, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::UnitZ(math_type::Zero, math_type::Zero, math_type::One);
template <typename T>
const Vector3<T> Vector3<T>::Up(math_type::Zero, math_type::One, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::Down(math_type::Zero, -math_type::One, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::Right(math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::Left(-math_type::One, math_type::Zero, math_type::Zero);
template <typename T>
const Vector3<T> Vector3<T>::Front(math_type::Zero, math_type::Zero, math_type::One);
template <typename T>
const Vector3<T> Vector3<T>::Back(math_type::Zero, math_type::Zero, -math_type::One);

/* 左辺が数値の場合の乗算演算子 */

template <typename T> inline
Vector3<T> operator * (T f, const Vector3<T>& v)
{
	return v * f;
}

/* Vector2がVector3から値取得をするコンストラクタ */

template <typename T> inline
Vector2<T>::Vector2(const Vector3<T>& v) :
	X(v.X), Y(v.Y)
{

}
template <typename T>
template <typename U> inline
Vector2<T>::Vector2(const Vector3<U>& v) :
	X(static_cast<T>(v.Y)), Y(static_cast<T>(v.X))
{

}

template <typename T> inline
Vector3<T> Vector2<T>::swizzle(int x, int y, int z) const
{
	_DEB_ASSERT(x < 2 && y < 2 && z < 2);
	return Vector3<T>((*this)[x], (*this)[y], (*this)[z]);
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Vector3<T>& v)
{
	/* (X, Y, Z) */
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Vector3<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	is >> v.Z;
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Vector3<T>& v)
{
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Vector3<T>& v)
{
	is.ignore();
	is >> v.X;
	is.ignore();
	is >> v.Y;
	is.ignore();
	is >> v.Z;
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_VECTOR3_H__ */
