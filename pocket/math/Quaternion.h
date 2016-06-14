#ifndef __POCKET_MATH_QUATERNION_H__
#define __POCKET_MATH_QUATERNION_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../debug.h"
#include "array.h"
#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct Quaternion;
template <typename> struct Matrix3x3;
template <typename> struct Matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Quaternion<float> Quaternionf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Quaternion<double> Quaterniond;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Quaternion<long double> Quaternionld;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USE_CXX11
template <typename T>
using quat = Quaternion<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using quatf = quat<float>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using quatd = quat<double>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using quatld = quat<long double>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T>
struct Quaternion
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

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

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS

			T X; // X軸、虚部
			T Y; // Y軸、虚部
			T Z; // Z軸、虚部
			T W; // 回転量、実部

#ifdef _USE_ANONYMOUS
		};
		array_type Data;
	};
#endif // _USE_ANONYMOUS

	template <typename> friend struct Quaternion;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const T ErrorSlerp; // 0.001, 誤差
	static const Quaternion Zero; // 0.0, 0.0, 0.0, 0.0
	static const Quaternion Identity; // 0.0, 0.0, 0.0, 1.0

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Quaternion);
	explicit Quaternion(const behavior::_noinitialize_t&)
	{

	}
	explicit Quaternion(const behavior::_zero_t&) :
		X(math_type::Zero), Y(math_type::Zero), Z(math_type::Zero),
		W(math_type::Zero)
	{

	}
	explicit Quaternion(const behavior::_identity_t&) :
		X(math_type::Zero), Y(math_type::Zero), Z(math_type::Zero),
		W(math_type::One)
	{

	}
	Quaternion(T x, T y, T z, T w) :
		X(x), Y(y), Z(z), W(w)
	{

	}
	explicit Quaternion(T f) :
		X(f), Y(f), Z(f), W(f)
	{

	}
	explicit Quaternion(const Vector3<T>& axis, T deg)
	{
		from_axis(axis, deg);
	}
	explicit Quaternion(const Matrix4x4<T>& m)
	{
		from_matrix(m);
	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Quaternion& add(const Quaternion& q, Quaternion& result) const
	{
		result.X = X + q.X;
		result.Y = Y + q.Y;
		result.Z = Z + q.Z;
		result.W = W + q.W;
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	Quaternion& subtract(const Quaternion& q, Quaternion& result) const
	{
		result.X = X - q.X;
		result.Y = Y - q.Y;
		result.Z = Z - q.Z;
		result.W = W - q.W;
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	Quaternion& multiply(T f, Quaternion& result) const
	{
		result.X = X * f;
		result.Y = Y * f;
		result.Z = Z * f;
		result.W = W * f;
		return result;
	}
	Quaternion& multiply(const Quaternion& q, Quaternion& result) const
	{
		// W -> W要素と内積、XYZ -> 外積*W
		// (W*q.W - V・q.V, (W * q.V) + (q.W * V) + (V×q.V))
		result.X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
		result.Y = W * q.Y - X * q.Z + Y * q.W + Z * q.X;
		result.Z = W * q.Z + X * q.Y - Y * q.X + Z * q.W;
		result.W = W * q.W - X * q.X - Y * q.Y - Z * q.Z;
		return result;
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Quaternion& divide(T f, Quaternion& result) const
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return multiply(f, result);
	}

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Quaternion& q) const
	{
		return (math_type::is_near(X, q.X) && math_type::is_near(Y, q.Y) && math_type::is_near(Z, q.Z) && math_type::is_near(W, q.W));
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
	* 値が単位クォータニオンに近いか
	*---------------------------------------------------------------------*/
	bool is_near_identity() const
	{
		return is_near(Quaternion::Identity);
	}

	/*---------------------------------------------------------------------
	* 任意軸と角度から求める
	*---------------------------------------------------------------------*/
	Quaternion& from_axis(const Vector3<T>& axis, T deg)
	{
		// 軸ベクトルは正規化されていなければいけない

		// 値を半分
		deg *= math_type::Half;
		T s = math_type::sin(deg);

		X = axis.X * s;
		Y = axis.Y * s;
		Z = axis.Z * s;
		W = math_type::cos(deg);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 行列から求める
	*---------------------------------------------------------------------*/
	Quaternion& from_matrix(const Matrix3x3<T>&); // Matrix3x3.h
	Quaternion& from_matrix(const Matrix4x4<T>&); // Matrix4x4.h

	/*---------------------------------------------------------------------
	* 単位クォータニオンにする
	*---------------------------------------------------------------------*/
	Quaternion& load_identity()
	{
		*this = Quaternion::Identity;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 共役クォータニオンにする
	*---------------------------------------------------------------------*/
	Quaternion& conjugate()
	{
		// それぞれをただ反転させるのみ
		X = -X;
		Y = -Y;
		Z = -Z;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 共役クォータニオンを求める
	*---------------------------------------------------------------------*/
	Quaternion conjugated() const
	{
		return Quaternion(-X, -Y, -Z, W);
	}
	Quaternion& conjugated(Quaternion& result) const
	{
		result.X = -X;
		result.Y = -Y;
		result.Z = -Z;
		result.W = W;
		return result;
	}
	/*---------------------------------------------------------------------
	* 逆クォータニオンにする
	*---------------------------------------------------------------------*/
	bool inverse()
	{
		T len = length_sq();
		if (math_type::is_near_zero(len))
		{
			return false;
		}

		len = math_type::reciprocal(len);
		X = -X * len;
		Y = -Y * len;
		Z = -Z * len;
		W = W * len;

		return true;
	}
	/*---------------------------------------------------------------------
	* 逆クォータニオンを求める
	*---------------------------------------------------------------------*/
	Quaternion inversed() const
	{
		Quaternion r(behavior::noinitialize);
		inversed(r);
		return r;
	}
	bool inversed(Quaternion& result) const
	{
		T len = length_sq();
		if (math_type::is_near_zero(len))
		{
			return false;
		}

		len = math_type::reciprocal(len);
		result.X = -X * len;
		result.Y = -Y * len;
		result.Z = -Z * len;
		result.W = W * len;

		return true;
	}
	/*---------------------------------------------------------------------
	* 保持している回転量を求める
	*---------------------------------------------------------------------*/
	T angle() const
	{
		// コサインで求めるのでその逆
		return math_type::acos(W) * math_type::Two;
	}
	/*---------------------------------------------------------------------
	* 保持している軸（ベクトル）を求める
	*---------------------------------------------------------------------*/
	Vector3<T> axis() const
	{
		Vector3<T> r(behavior::noinitialize);
		return axis(r);
	}
	Vector3<T>& axis(Vector3<T>& result) const
	{
		T as = math_type::asin(angle() * math_type::Half);
		result.X = X * as;
		result.Y = Y * as;
		result.Z = Z * as;
		return result;
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
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const Quaternion& q) const
	{
		return X * q.X + Y * q.Y + Z * q.Z + W * q.W;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	Quaternion& normalize()
	{
		T len = length_sq();
		if (len != math_type::Zero)
		{
			len = math_type::rsqrt(len);
			X *= len;
			Y *= len;
			Z *= len;
			W *= len;
		}
		return *this;
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	Quaternion normalized() const
	{
		Quaternion v(*this);
		return v.normalize();
	}
	Quaternion& normalized(Quaternion& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	Quaternion lerp(const Quaternion& to, T t) const
	{
		return Quaternion(math_type::lerp(X, to.X, t),
			math_type::lerp(Y, to.Y, t),
			math_type::lerp(Z, to.Z, t),
			math_type::lerp(W, to.W, t));
	}
	Quaternion& lerp(const Quaternion& to, T t, Quaternion& result) const
	{
		result.X = math_type::lerp(X, to.X, t);
		result.Y = math_type::lerp(Y, to.Y, t);
		result.Z = math_type::lerp(Z, to.Z, t);
		result.W = math_type::lerp(W, to.W, t);
		return result;
	}
	Quaternion& lerp(const Quaternion& from, const Quaternion& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 球面線形補間
	*---------------------------------------------------------------------*/
	Quaternion slerp(const Quaternion& to, T t) const
	{
		Quaternion result(behavior::noinitialize);
		return slerp(to, t, result);
	}
	Quaternion& slerp(const Quaternion& to, T t, Quaternion& result) const
	{
		T c = dot(to);
		Quaternion tmp(behavior::noinitialize);
		if (c < math_type::Zero)
		{
			c = -c;
			tmp = -to;
		}
		else
		{
			tmp = to;
		}
		T k0 = math_type::One - t;
		T k1 = t;
		if ((math_type::One - c) > Quaternion::ErrorSlerp)
		{
			T theta = math_type::acos(c);
			T s = math_type::One / math_type::sin(theta);
			k0 = math_type::sin(theta * k0) * s;
			k1 = math_type::sin(theta * k1) * s;
		}

		multiply(k0, result);
		tmp *= k1;
		result += tmp;

		return result;
	}
	Quaternion& slerp(const Quaternion& from, const Quaternion& to, T t)
	{
		return from.slerp(to, t, *this);
	}

	/*---------------------------------------------------------------------
	* ベクトル回転
	*---------------------------------------------------------------------*/
	Vector3<T> rotate(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return rotate(v, r);
	}
	Vector3<T>& rotate(const Vector3<T>& v, Vector3<T>& result) const
	{
		// Q * V * Q^: 右手
		// Q^ * V * Q: 左手

		Quaternion conj(-X, -Y, -Z, W), vq(v.X, v.Y, v.Z, math_type::Zero), calc(behavior::noinitialize);

		conj.multiply(vq, calc);
		calc.multiply(*this, vq);

		// XYZに値が格納される
		result.X = vq.X;
		result.Y = vq.Y;
		result.Z = vq.Z;

		return result;
	}
	Vector4<T> rotate(const Vector4<T>& v) const
	{
		Vector4<T> r(behavior::noinitialize);
		return rotate(v, r);
	}
	Vector4<T>& rotate(const Vector4<T>& v, Vector4<T>& result) const
	{
		// Q * V * Q^: 右手
		// Q^ * V * Q: 左手

		Quaternion conj(-X, -Y, -Z, W), vq(v.X, v.Y, v.Z, math_type::Zero), calc(behavior::noinitialize);

		conj.multiply(vq, calc);
		calc.multiply(*this, vq);

		// XYZに値が格納される
		result.X = vq.X;
		result.Y = vq.Y;
		result.Z = vq.Z;
		result.W = v.W;

		return result;
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
#endif // _USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Quaternion<U>() const
	{
		return Quaternion<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z), static_cast<U>(W));
	}
	_CXX11_EXPLICIT operator Vector3<T>() const
	{
		Vector3<T> r(behavior::noinitialize);
		return axis(r);
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // _USE_ANONYMOUS
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // _USE_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Quaternion& q) const
	{
		return (X == q.X) && (Y == q.Y) && (Z == q.Z) && (W == q.W);
	}
	bool operator != (const Quaternion& q) const
	{
		return !(*this == q);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Quaternion operator + () const
	{
		return *this;
	}
	Quaternion operator - () const
	{
		return Quaternion(-X, -Y, -Z, -W);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Quaternion operator + (const Quaternion& q) const
	{
		Quaternion result(behavior::noinitialize);
		return add(q, result);
	}
	Quaternion operator - (const Quaternion& q) const
	{
		Quaternion result(behavior::noinitialize);
		return subtract(q, result);
	}
	Quaternion operator * (T f) const
	{
		Quaternion result(behavior::noinitialize);
		return multiply(f, result);
	}
	Quaternion operator * (const Quaternion& q) const
	{
		Quaternion result(behavior::noinitialize);
		return multiply(q, result);
	}
	Vector3<T> operator * (const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return rotate(v, r);
	}
	Vector4<T> operator * (const Vector4<T>& v) const
	{
		Vector4<T> r(behavior::noinitialize);
		return rotate(v, r);
	}
	Quaternion operator / (T f) const
	{
		Quaternion result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Quaternion& operator = (const behavior::_zero_t&)
	{
		X = Y = Z = W = math_type::Zero;
		return *this;
	}
	Quaternion& operator = (const behavior::_identity_t&)
	{
		return load_identity();
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Quaternion& operator += (const Quaternion& q)
	{
		X += q.X;
		Y += q.Y;
		Z += q.Z;
		W += q.W;
		return *this;
	}
	Quaternion& operator -= (const Quaternion& q)
	{
		X -= q.X;
		Y -= q.Y;
		Z -= q.Z;
		W -= q.W;
		return *this;
	}
	Quaternion& operator *= (T f)
	{
		X *= f;
		Y *= f;
		Z *= f;
		W *= f;
		return *this;
	}
	Quaternion& operator *= (const Quaternion& q)
	{
		const Quaternion c(*this);
		return c.multiply(q, *this);
	}
	Quaternion& operator /= (T f)
	{
		_DEB_ASSERT(f != math_type::Zero);
		f = math_type::One / f;
		return *this *= f;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Quaternion& operator () (const behavior::_zero_t&)
	{
		X = Y = Z = W = math_type::Zero;
		return *this;
	}
	Quaternion& operator () (const behavior::_identity_t&)
	{
		return load_identity();
	}

	Quaternion operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	Quaternion operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	Quaternion operator () (const behavior::_add_t&, const Quaternion& q) const
	{
		return operator+(q);
	}
	Quaternion operator () (const behavior::_sub_t&, const Quaternion& q) const
	{
		return operator-(q);
	}
	Quaternion operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Quaternion operator () (const behavior::_mul_t&, const Quaternion& q) const
	{
		return operator*(q);
	}
	Quaternion operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}

	Quaternion& operator () (const behavior::_add_assign_t&, const Quaternion& q)
	{
		return operator+=(q);
	}
	Quaternion& operator () (const behavior::_sub_assign_t&, const Quaternion& q)
	{
		return operator-=(q);
	}
	Quaternion& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Quaternion& operator () (const behavior::_mul_assign_t&, const Quaternion& q)
	{
		return operator*=(q);
	}
	Quaternion& operator () (const behavior::_div_assign_t&, T f)
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

	bool operator () (const behavior::_equal_t&, const Quaternion& q) const
	{
		return operator==(q);
	}
	bool operator () (const behavior::_not_equal_t&, const Quaternion& q) const
	{
		return operator!=(q);
	}
	bool operator () (const behavior::_near_t&, const Quaternion& q) const
	{
		return is_near(q);
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
	T operator () (const behavior::_dot_t&, const Quaternion& q) const
	{
		return dot(q);
	}
	Quaternion& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	Quaternion operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	Quaternion operator () (const behavior::_conjugate_t&) const
	{
		return conjugated();
	}
	Vector3<T> operator () (const behavior::_axis_t&) const
	{
		return axis();
	}
	T operator () (const behavior::_angle_t&) const
	{
		return angle();
	}
	Quaternion operator () (const behavior::_inverse_t&) const
	{
		return inversed();
	}
	Quaternion operator () (const behavior::_slerp_t&, const Quaternion& q, T t) const
	{
		return slerp(q, t);
	}
	Quaternion operator () (const behavior::_lerp_t&, const Quaternion& q, T t) const
	{
		return lerp(q, t);
	}
	Vector3<T> operator () (const behavior::_rotate_t&, const Vector3<T>& v) const
	{
		return rotate(v);
	}
	Vector4<T> operator () (const behavior::_rotate_t&, const Vector4<T>& v) const
	{
		return rotate(v);
	}
	Quaternion& operator () (const behavior::_from_matrix_t&, const Matrix4x4<T>& m)
	{
		return from_matrix(m);
	}
	Quaternion& operator () (const behavior::_from_axis_angle_t&, const Vector3<T>& axis, T angle)
	{
		return from_axis(axis, angle);
	}
};

template <typename T>
const Quaternion<T> Quaternion<T>::Zero(math_type::Zero, math_type::Zero, math_type::Zero, math_type::Zero);
template <typename T>
const Quaternion<T> Quaternion<T>::Identity(math_type::Zero, math_type::Zero, math_type::Zero, math_type::One);
#ifndef _UNUSING_MATH_INT_FLOAT
template <> const float Quaternion<float>::ErrorSlerp = 0.001f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
template <> const double Quaternion<double>::ErrorSlerp = 0.001;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
template <> const long double Quaternion<long double>::ErrorSlerp = 0.001L;
#endif // _USING_MATH_LONG_DOUBLE

/*---------------------------------------------------------------------
* Vector3.Rotate
*---------------------------------------------------------------------*/
template <typename T> inline
Vector3<T> Vector3<T>::rotated(const Quaternion<T>& q) const
{
	Vector3<T> r(behavior::noinitialize);
	return q.rotate(*this, r);
}
template <typename T> inline
Vector3<T>& Vector3<T>::rotated(const Quaternion<T>& q, Vector3<T>& result) const
{
	return q.rotate(*this, result);
}
template <typename T> inline
Vector3<T>& Vector3<T>::rotate(const Quaternion<T>& q)
{
	Vector3<T> c(*this);
	return q.rotate(c, *this);
}

template <typename T> inline
Vector3<T> Vector3<T>::operator * (const Quaternion<T>& q) const
{
	Vector3<T> r(behavior::noinitialize);
	return q.rotate(*this, r);
}

/*---------------------------------------------------------------------
* Vector4.Rotate
*---------------------------------------------------------------------*/
template <typename T> inline
Vector4<T> Vector4<T>::rotated(const Quaternion<T>& q) const
{
	Vector4<T> r(behavior::noinitialize);
	return q.rotate(*this, r);
}
template <typename T> inline
Vector4<T>& Vector4<T>::rotated(const Quaternion<T>& q, Vector4<T>& result) const
{
	return q.rotate(*this, result);
}
template <typename T> inline
Vector4<T>& Vector4<T>::rotate(const Quaternion<T>& q)
{
	Vector4<T> c(*this);
	return q.rotate(c, *this);
}

template <typename T> inline
Vector4<T> Vector4<T>::operator * (const Quaternion<T>& q) const
{
	Vector4<T> r(behavior::noinitialize);
	return q.rotate(*this, r);
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Quaternion<T>& v)
{
	// (X, Y, Z, W)
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::comma_space
		<< v.W << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Quaternion<T>& v)
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
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Quaternion<T>& v)
{
	os << out_char::parentheses_left << v.X << out_char::comma_space
		<< v.Y << out_char::comma_space
		<< v.Z << out_char::comma_space
		<< v.W << out_char::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Quaternion<T>& v)
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
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_QUATERNION_H__
