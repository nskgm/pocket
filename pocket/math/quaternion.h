#ifndef __POCKET_MATH_QUATERNION_H__
#define __POCKET_MATH_QUATERNION_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../call.h"
#include "../debug.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector3.h"
#include "vector4.h"
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct quaternion;
template <typename> struct matrix3x3;
template <typename> struct matrix4x4;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef quaternion<float> quaternionf;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef quaternion<double> quaterniond;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef quaternion<long double> quaternionld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
struct quaternion
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;

	typedef container::array<T, 4> array_type;
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

			T x; // x軸、虚部
			T y; // y軸、虚部
			T z; // z軸、虚部
			T w; // 回転量、実部

#ifdef POCKET_USE_ANONYMOUS
		};
		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct quaternion;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const T error_slerp_value; // 0.001, 誤差
	static const quaternion zero; // 0.0, 0.0, 0.0, 0.0
	static const quaternion identity; // 0.0, 0.0, 0.0, 1.0

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(quaternion);
	explicit quaternion(const call::noinitialize_t&)
	{}
	explicit quaternion(const call::zero_t&) :
		x(math_type::zero), y(math_type::zero), z(math_type::zero),
		w(math_type::zero)
	{}
	explicit quaternion(const call::identity_t&) :
		x(math_type::zero), y(math_type::zero), z(math_type::zero),
		w(math_type::one)
	{}
	quaternion(T x, T y, T z, T w) :
		x(x), y(y), z(z), w(w)
	{}
	explicit quaternion(T f) :
		x(f), y(f), z(f), w(f)
	{}
	explicit quaternion(const vector3<T>& axis, T deg)
	{
		from_axis_angle(axis, deg);
	}
	explicit quaternion(const matrix4x4<T>& m)
	{
		from_matrix(m);
	}

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
	quaternion& add(const quaternion& q, quaternion& result) const
	{
		result.x = x + q.x;
		result.y = y + q.y;
		result.z = z + q.z;
		result.w = w + q.w;
		return result;
	}
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
	quaternion& subtract(const quaternion& q, quaternion& result) const
	{
		result.x = x - q.x;
		result.y = y - q.y;
		result.z = z - q.z;
		result.w = w - q.w;
		return result;
	}
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
	quaternion& multiply(T f, quaternion& result) const
	{
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		result.w = w * f;
		return result;
	}
	quaternion& multiply(const quaternion& q, quaternion& result) const
	{
		// w -> w要素と内積、XYZ -> 外積*w
		// (w*q.w - V・q.V, (w * q.V) + (q.w * V) + (V×q.V))
		result.x = w * q.x + x * q.w + y * q.z - z * q.y;
		result.y = w * q.y - x * q.z + y * q.w + z * q.x;
		result.z = w * q.z + x * q.y - y * q.x + z * q.w;
		result.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return result;
	}
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	quaternion& divide(T f, quaternion& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return multiply(math_type::reciprocal(f), result);
	}

	//---------------------------------------------------------------------
	// 値が近いか
	//---------------------------------------------------------------------
	bool near_equal(const quaternion& q) const
	{
		return (math_type::near_equal(x, q.x) && math_type::near_equal(y, q.y) && math_type::near_equal(z, q.z) && math_type::near_equal(w, q.w));
	}
	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	bool near_equal_zero() const
	{
		return (math_type::near_equal_zero(x) && math_type::near_equal_zero(y) && math_type::near_equal_zero(z) && math_type::near_equal_zero(w));
	}
	//---------------------------------------------------------------------
	// 値がゼロか
	//---------------------------------------------------------------------
	bool equal_zero() const
	{
		return (x == math_type::zero && y == math_type::zero && z == math_type::zero && w == math_type::zero);
	}

	//---------------------------------------------------------------------
	// 値が単位クォータニオンに近いか
	//---------------------------------------------------------------------
	bool near_identity() const
	{
		return near_equal(quaternion::identity);
	}

	//---------------------------------------------------------------------
	// 任意軸と角度から求める
	//---------------------------------------------------------------------
	quaternion& from_axis_angle(const vector3<T>& axis, T deg)
	{
		// 軸ベクトルは正規化されていなければいけない

		// 値を半分
		deg *= math_type::half;
		T s = math_type::sin(deg);

		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = math_type::cos(deg);
		return *this;
	}
	quaternion& from_angle_x(T deg)
	{
		return from_axis_angle(vector3<T>::unit_x, deg);
	}
	quaternion& from_angle_y(T deg)
	{
		return from_axis_angle(vector3<T>::unit_y, deg);
	}
	quaternion& from_angle_z(T deg)
	{
		return from_axis_angle(vector3<T>::unit_z, deg);
	}
	//---------------------------------------------------------------------
	// 行列から求める
	//---------------------------------------------------------------------
	quaternion& from_matrix(const matrix3x3<T>&); // matrix3x3.h
	quaternion& from_matrix(const matrix4x4<T>&); // matrix4x4.h

	//---------------------------------------------------------------------
	// 単位クォータニオンにする
	//---------------------------------------------------------------------
	quaternion& load_identity()
	{
		*this = quaternion::identity;
		return *this;
	}
	//---------------------------------------------------------------------
	// 0クォータニオンにする
	//---------------------------------------------------------------------
	quaternion& load_zero()
	{
		*this = quaternion::zero;
		return *this;
	}
	//---------------------------------------------------------------------
	// 共役クォータニオンにする
	//---------------------------------------------------------------------
	quaternion& conjugate()
	{
		// それぞれをただ反転させるのみ
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	quaternion& conjugate(quaternion& result) const
	{
		result.x = -x;
		result.y = -y;
		result.z = -z;
		result.w = w;
		return result;
	}
	//---------------------------------------------------------------------
	// 共役クォータニオンを求める
	//---------------------------------------------------------------------
	quaternion conjugated() const
	{
		return quaternion(-x, -y, -z, w);
	}
	//---------------------------------------------------------------------
	// 逆クォータニオンにする
	//---------------------------------------------------------------------
	quaternion& inverse()
	{
		T len = length_sq();
		if (math_type::near_equal_zero(len))
		{
			return *this;
		}

		len = math_type::reciprocal(len);
		x = -x * len;
		y = -y * len;
		z = -z * len;
		w = w * len;

		return *this;
	}
	quaternion& inverse(quaternion& result) const
	{
		T len = length_sq();
		if (math_type::near_equal_zero(len))
		{
			return result.load_identity();
		}

		len = math_type::reciprocal(len);
		result.x = -x * len;
		result.y = -y * len;
		result.z = -z * len;
		result.w = w * len;

		return result;
	}
	//---------------------------------------------------------------------
	// 逆クォータニオンを求める
	//---------------------------------------------------------------------
	quaternion inversed() const
	{
		quaternion r(call::noinitialize);
		return inverse(r);
	}
	//---------------------------------------------------------------------
	// クォータニオン同士の差を求める
	//---------------------------------------------------------------------
	quaternion difference(const quaternion& q) const
	{
		quaternion inv;
		inverse(inv);
		return inv * q;
	}
	//---------------------------------------------------------------------
	// 保持している回転量を求める
	//---------------------------------------------------------------------
	T angle() const
	{
		// コサインで求めるのでその逆
		return math_type::acos(w) * math_type::two;
	}
	//---------------------------------------------------------------------
	// クォータニオン同士の回転量を求める
	//---------------------------------------------------------------------
	T angle(const quaternion& q) const
	{
		// const quaternion diff = difference();
		// return diff.angle();
		const T d = dot(q);
		return math_type::acos(d) * math_type::two;
	}
	//---------------------------------------------------------------------
	// 保持している軸（ベクトル）を求める
	//---------------------------------------------------------------------
	vector3<T> axis() const
	{
		vector3<T> r(call::noinitialize);
		return axis(r);
	}
	vector3<T>& axis(vector3<T>& result) const
	{
		T as = math_type::reciprocal(math_type::sin(math_type::acos(w)));
		result.x = x * as;
		result.y = y * as;
		result.z = z * as;
		return result;
	}
	//---------------------------------------------------------------------
	// 軸と角度へ変換
	//---------------------------------------------------------------------
	void axis_angle(vector3<T>& ax, T& an) const
	{
		const T c = math_type::acos(w);
		an = c * math_type::two;
		const T s = math_type::reciprocal(math_type::sin(c));
		ax.x = x * s;
		ax.y = y * s;
		ax.z = z * s;
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
	// 内積を求める
	//---------------------------------------------------------------------
	T dot(const quaternion& q) const
	{
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}
	//---------------------------------------------------------------------
	// 正規化
	//---------------------------------------------------------------------
	quaternion& normalize()
	{
		T len = length_sq();
		if (len > math_type::zero)
		{
			len = math_type::rsqrt(len);
			x *= len;
			y *= len;
			z *= len;
			w *= len;
		}
		return *this;
	}
	quaternion& normalize(quaternion& result) const
	{
		result = *this;
		return result.normalize();
	}
	//---------------------------------------------------------------------
	// 正規化を求める
	//---------------------------------------------------------------------
	quaternion normalized() const
	{
		quaternion v(*this);
		return v.normalize();
	}
	//---------------------------------------------------------------------
	// 線形補間
	//---------------------------------------------------------------------
	quaternion lerp(const quaternion& to, T t) const
	{
		return quaternion(math_type::lerp(x, to.x, t),
			math_type::lerp(y, to.y, t),
			math_type::lerp(z, to.z, t),
			math_type::lerp(w, to.w, t));
	}
	quaternion& lerp(const quaternion& to, T t, quaternion& result) const
	{
		result.x = math_type::lerp(x, to.x, t);
		result.y = math_type::lerp(y, to.y, t);
		result.z = math_type::lerp(z, to.z, t);
		result.w = math_type::lerp(w, to.w, t);
		return result;
	}
	quaternion& lerp(const quaternion& from, const quaternion& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	//---------------------------------------------------------------------
	// 球面線形補間
	//---------------------------------------------------------------------
	quaternion slerp(const quaternion& to, T t) const
	{
		quaternion result(call::noinitialize);
		return slerp(to, t, result);
	}
	quaternion& slerp(const quaternion& to, T t, quaternion& result) const
	{
		T c = dot(to);
		quaternion tmp(call::noinitialize);
		if (c < math_type::zero)
		{
			c = -c;
			tmp = -to;
		}
		else
		{
			tmp = to;
		}
		T k0 = math_type::one - t;
		T k1 = t;
		if ((math_type::one - c) > quaternion::error_slerp_value)
		{
			T theta = math_type::acos(c);
			T s = math_type::one / math_type::sin(theta);
			k0 = math_type::sin(theta * k0) * s;
			k1 = math_type::sin(theta * k1) * s;
		}

		multiply(k0, result);
		tmp *= k1;
		result += tmp;

		return result;
	}
	quaternion& slerp(const quaternion& from, const quaternion& to, T t)
	{
		return from.slerp(to, t, *this);
	}

	//---------------------------------------------------------------------
	// ベクトル回転
	//---------------------------------------------------------------------
	vector3<T> rotate(const vector3<T>& v) const
	{
		vector3<T> r(call::noinitialize);
		return rotate(v, r);
	}
	vector3<T>& rotate(const vector3<T>& v, vector3<T>& result) const
	{
		// Q * V * Q^: 右手
		// Q^ * V * Q: 左手

		quaternion conj(-x, -y, -z, w), vq(v.x, v.y, v.z, math_type::zero), calc(call::noinitialize);

		conj.multiply(vq, calc);
		calc.multiply(*this, vq);

		// XYZに値が格納される
		result.x = vq.x;
		result.y = vq.y;
		result.z = vq.z;

		return result;
	}
	vector4<T> rotate(const vector4<T>& v) const
	{
		vector4<T> r(call::noinitialize);
		return rotate(v, r);
	}
	vector4<T>& rotate(const vector4<T>& v, vector4<T>& result) const
	{
		// Q * V * Q^: 右手
		// Q^ * V * Q: 左手

		quaternion conj(-x, -y, -z, w), vq(v.x, v.y, v.z, math_type::zero), calc(call::noinitialize);

		conj.multiply(vq, calc);
		calc.multiply(*this, vq);

		// XYZに値が格納される
		result.x = vq.x;
		result.y = vq.y;
		result.z = vq.z;
		result.w = v.w;

		return result;
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
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
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
	POCKET_CXX11_EXPLICIT operator quaternion<U>() const
	{
		return quaternion<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
	}
	POCKET_CXX11_EXPLICIT operator vector3<T>() const
	{
		vector3<T> r(call::noinitialize);
		return axis(r);
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
	bool operator == (const quaternion& q) const
	{
		return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w);
	}
	bool operator != (const quaternion& q) const
	{
		return !(*this == q);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	quaternion operator + () const
	{
		return *this;
	}
	quaternion operator - () const
	{
		return quaternion(-x, -y, -z, -w);
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	quaternion operator + (const quaternion& q) const
	{
		return quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}
	quaternion operator - (const quaternion& q) const
	{
		return quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
	}
	quaternion operator * (T f) const
	{
		return quaternion(x * f, y * f, z * f, w * f);
	}
	quaternion operator * (const quaternion& q) const
	{
		return quaternion(w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z);
	}
	vector3<T> operator * (const vector3<T>& v) const
	{
		vector3<T> r(call::noinitialize);
		return rotate(v, r);
	}
	vector4<T> operator * (const vector4<T>& v) const
	{
		vector4<T> r(call::noinitialize);
		return rotate(v, r);
	}
	quaternion operator / (T f) const
	{
		quaternion result(call::noinitialize);
		return divide(f, result);
	}

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
	quaternion& operator = (const call::zero_t&)
	{
		x = y = z = w = math_type::zero;
		return *this;
	}
	quaternion& operator = (const call::identity_t&)
	{
		return load_identity();
	}

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	quaternion& operator += (const quaternion& q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return *this;
	}
	quaternion& operator -= (const quaternion& q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return *this;
	}
	quaternion& operator *= (T f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	quaternion& operator *= (const quaternion& q)
	{
		const quaternion c = *this;
		return c.multiply(q, *this);
	}
	quaternion& operator /= (T f)
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return *this *= math_type::reciprocal(f);
	}

	//------------------------------------------------------------------------------------------
	// タグでの関数呼び出し
	//------------------------------------------------------------------------------------------

	quaternion& operator () (const call::zero_t&)
	{
		x = y = z = w = math_type::zero;
		return *this;
	}
	quaternion& operator () (const call::identity_t&)
	{
		return load_identity();
	}

	quaternion operator () (const call::plus_t&) const
	{
		return operator+();
	}
	quaternion operator () (const call::negate_t&) const
	{
		return operator-();
	}
	quaternion operator () (const call::add_t&, const quaternion& q) const
	{
		return operator+(q);
	}
	quaternion operator () (const call::sub_t&, const quaternion& q) const
	{
		return operator-(q);
	}
	quaternion operator () (const call::mul_t&, T f) const
	{
		return operator*(f);
	}
	quaternion operator () (const call::mul_t&, const quaternion& q) const
	{
		return operator*(q);
	}
	quaternion operator () (const call::div_t&, T f) const
	{
		return operator/(f);
	}

	quaternion& operator () (const call::add_assign_t&, const quaternion& q)
	{
		return operator+=(q);
	}
	quaternion& operator () (const call::sub_assign_t&, const quaternion& q)
	{
		return operator-=(q);
	}
	quaternion& operator () (const call::mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	quaternion& operator () (const call::mul_assign_t&, const quaternion& q)
	{
		return operator*=(q);
	}
	quaternion& operator () (const call::div_assign_t&, T f)
	{
		return operator/=(f);
	}
	T& operator () (const call::at_t&, int i)
	{
		return operator[](i);
	}
	const T& operator () (const call::at_t&, int i) const
	{
		return operator[](i);
	}
	T* operator () (const call::pointer_t&)
	{
		return operator T*();
	}
	const T* operator () (const call::pointer_t&) const
	{
		return operator const T*();
	}

	bool operator () (const call::equal_t&, const quaternion& q) const
	{
		return operator==(q);
	}
	bool operator () (const call::not_equal_t&, const quaternion& q) const
	{
		return operator!=(q);
	}
	bool operator () (const call::near_t&, const quaternion& q) const
	{
		return near_equal(q);
	}
	bool operator () (const call::near_zero_t&) const
	{
		return near_equal_zero();
	}

	T operator () (const call::length_t&) const
	{
		return length();
	}
	T operator () (const call::length_square_t&) const
	{
		return length_sq();
	}
	T operator () (const call::dot_t&, const quaternion& q) const
	{
		return dot(q);
	}
	quaternion& operator () (const call::normalize_t&)
	{
		return normalize();
	}
	quaternion operator () (const call::normalized_t&) const
	{
		return normalized();
	}
	quaternion operator () (const call::conjugate_t&) const
	{
		return conjugated();
	}
	vector3<T> operator () (const call::axis_t&) const
	{
		return axis();
	}
	T operator () (const call::angle_t&) const
	{
		return angle();
	}
	quaternion operator () (const call::inverse_t&) const
	{
		return inversed();
	}
	quaternion operator () (const call::slerp_t&, const quaternion& q, T t) const
	{
		return slerp(q, t);
	}
	quaternion operator () (const call::lerp_t&, const quaternion& q, T t) const
	{
		return lerp(q, t);
	}
	vector3<T> operator () (const call::rotate_t&, const vector3<T>& v) const
	{
		return rotate(v);
	}
	vector4<T> operator () (const call::rotate_t&, const vector4<T>& v) const
	{
		return rotate(v);
	}
	quaternion& operator () (const call::from_matrix_t&, const matrix4x4<T>& m)
	{
		return from_matrix(m);
	}
	quaternion& operator () (const call::from_axis_angle_t&, const vector3<T>& axis, T angle)
	{
		return from_axis_angle(axis, angle);
	}
};

template <typename T>
const quaternion<T> quaternion<T>::zero(math_type::zero, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const quaternion<T> quaternion<T>::identity(math_type::zero, math_type::zero, math_type::zero, math_type::one);
#ifndef POCKET_NO_USING_MATH_INT_FLOAT
template <> const float quaternion<float>::error_slerp_value = 0.001f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
template <> const double quaternion<double>::error_slerp_value = 0.001;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
template <> const long double quaternion<long double>::error_slerp_value = 0.001L;
#endif // POCKET_USING_MATH_LONG_DOUBLE

//---------------------------------------------------------------------
// vector3.Rotate
//---------------------------------------------------------------------
template <typename T> inline
vector3<T> vector3<T>::rotated(const quaternion<T>& q) const
{
	vector3<T> r(call::noinitialize);
	return q.rotate(*this, r);
}
template <typename T> inline
vector3<T>& vector3<T>::rotate(const quaternion<T>& q, vector3<T>& result) const
{
	return q.rotate(*this, result);
}
template <typename T> inline
vector3<T>& vector3<T>::rotate(const quaternion<T>& q)
{
	const vector3<T> c = *this;
	return q.rotate(c, *this);
}

template <typename T> inline
vector3<T> vector3<T>::operator * (const quaternion<T>& q) const
{
	vector3<T> r(call::noinitialize);
	return q.rotate(*this, r);
}

//---------------------------------------------------------------------
// vector4.Rotate
//---------------------------------------------------------------------
template <typename T> inline
vector4<T> vector4<T>::rotated(const quaternion<T>& q) const
{
	vector4<T> r(call::noinitialize);
	return q.rotate(*this, r);
}
template <typename T> inline
vector4<T>& vector4<T>::rotate(const quaternion<T>& q, vector4<T>& result) const
{
	return q.rotate(*this, result);
}
template <typename T> inline
vector4<T>& vector4<T>::rotate(const quaternion<T>& q)
{
	const vector4<T> c = *this;
	return q.rotate(c, *this);
}

template <typename T> inline
vector4<T> vector4<T>::operator * (const quaternion<T>& q) const
{
	vector4<T> r(call::noinitialize);
	return q.rotate(*this, r);
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const quaternion<T>& v)
{
	// (x, y, z, w)
	os << io::parentheses_left << v.x << io::comma_space
		<< v.y << io::comma_space
		<< v.z << io::comma_space
		<< v.w << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, quaternion<T>& v)
{
	is.ignore();
	is >> v.x;
	is.ignore();
	is >> v.y;
	is.ignore();
	is >> v.z;
	is.ignore();
	is >> v.w;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_QUATERNION_H__
