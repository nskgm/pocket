#ifndef __POCKET_MATH_VECTOR3_H__
#define __POCKET_MATH_VECTOR3_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector2.h"
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct vector3;
template <typename> struct vector4;
template <typename> struct matrix4x4;
template <typename> struct quaternion;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef vector3<int> Point3;
typedef vector3<int> vector3i;
typedef vector3<float> vector3f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef vector3<double> vector3d;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef vector3<long double> vector3ld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
struct vector3
{
	POCKET_MATH_STATICAL_ASSERT(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;

	typedef container::array<T, 3> array_type;
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
			T z;

#ifdef POCKET_USE_ANONYMOUS
		};

#ifdef POCKET_USE_ANONYMOUS_NON_POD
		struct
		{
			vector2<T> xy;
		};
#endif // POCKET_USE_ANONYMOUS_NON_POD

		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct vector3;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const vector3 zero; // 0.0, 0.0, 0.0
	static const vector3 one; // 1.0, 1.0, 1.0
	static const vector3 unit_x; // 1.0, 0.0, 0.0
	static const vector3 unit_y; // 0.0, 1.0, 0.0
	static const vector3 unit_z; // 0.0, 0.0, 1.0
	static const vector3 up; // 0.0, 1.0, 0.0
	static const vector3 down; // 0.0, -1.0, 0.0
	static const vector3 right; // 1.0, 0.0, 0.0
	static const vector3 left; // -1.0, 0.0, 0.0
	static const vector3 forward; // 0.0, 0.0, 1.0
	static const vector3 backward; // 0.0, 0.0, -1.0

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(vector3);
	explicit vector3(const behavior::_noinitialize_t&)
	{}
	explicit vector3(const behavior::_zero_t&) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::zero), y(math_type::zero),
#else
		xy(math_type::zero, math_type::zero),
#endif
		z(math_type::zero)
	{}
	explicit vector3(const behavior::_one_t&) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::one), y(math_type::one),
#else
		xy(math_type::one, math_type::one),
#endif
		z(math_type::one)
	{}
	explicit vector3(const behavior::_half_t&) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half), y(math_type::half),
#else
		xy(math_type::half, math_type::half),
#endif
		z(math_type::half)
	{}
	explicit vector3(const behavior::_half_of_half_t&) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half_of_half), y(math_type::half_of_half),
#else
		xy(math_type::half_of_half, math_type::half_of_half),
#endif
		z(math_type::half_of_half)
	{}

	vector3(T x, T y, T z) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(x), y(y),
#else
		xy(x, y),
#endif
		z(z)
	{}
	template <typename U, typename U1, typename U2,
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2)
	>
	vector3(U x, U1 y, U2 z) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(x)), y(static_cast<T>(y)),
#else
		xy(static_cast<T>(x), static_cast<T>(y)),
#endif
		z(static_cast<T>(z))
	{}
	explicit vector3(T f) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(f), y(f),
#else
		xy(f),
#endif
		z(f)
	{}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit vector3(U f) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(f)), y(static_cast<T>(f)),
#else
		xy(static_cast<T>(f)),
#endif
		z(static_cast<T>(f))
	{}
	vector3(const vector2<T>& v, T z) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(v.x), y(v.y),
#else
		xy(v),
#endif
		z(z)
	{}
	template <typename U>
	vector3(const vector2<U>& v, U z) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
#else
		xy(static_cast<T>(v.x), static_cast<T>(v.y)),
#endif
		z(static_cast<T>(z))
	{}
	template <typename U>
	vector3(const vector3<U>& v) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
#else
		xy(static_cast<T>(v.x), static_cast<T>(v.y)),
#endif
		z(static_cast<T>(v.z))
	{}
	vector3(const T* p) :
#ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(p[0]), y(p[1]),
#else
		xy(p[0], p[1]),
#endif
		z(p[2])
	{}

	explicit vector3(const vector4<T>&);
	template <typename U> explicit vector3(const vector4<U>&); // vector4.h

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// x軸回転角度とy軸回転角度から求める
	//---------------------------------------------------------------------
	vector3& from_pitch_yaw(T pitch, T yaw)
	{
		x = math_type::cos(pitch) * math_type::sin(yaw);
		y = math_type::sin(pitch);
		z = math_type::cos(pitch) * math_type::cos(yaw);
		return *this;
	}
	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
	vector3& add(const vector3& v, vector3& result) const
	{
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;
		return result;
	}
	template <typename U>
	vector3& add(const vector3<U>& v, vector3& result) const
	{
		result.x = x + static_cast<T>(v.x);
		result.y = y + static_cast<T>(v.y);
		result.z = z + static_cast<T>(v.z);
		return result;
	}
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
	vector3& subtract(const vector3& v, vector3& result) const
	{
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
		return result;
	}
	template <typename U>
	vector3& subtract(const vector3<U>& v, vector3& result) const
	{
		result.x = x - static_cast<T>(v.x);
		result.y = y - static_cast<T>(v.y);
		result.z = z - static_cast<T>(v.z);
		return result;
	}
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
	vector3& multiply(T f, vector3& result) const
	{
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		return result;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3& multiply(U f, vector3& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	vector3& divide(T f, vector3& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return multiply(math_type::reciprocal(f), result);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3& divide(U f, vector3& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_traits<U>::zero);
		return divide(static_cast<T>(f), result);
	}
	//---------------------------------------------------------------------
	// 平行か
	//---------------------------------------------------------------------
	bool parallel(const vector3& v) const
	{
		// 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向)
		return math_type::near_equal_zero(dot(v) - (length() * v.length()));
	}
	//---------------------------------------------------------------------
	// 垂直か
	//---------------------------------------------------------------------
	bool vertical(const vector3& v) const
	{
		return math_type::near_equal_zero(dot(v));
	}
	//---------------------------------------------------------------------
	// 値が近いか
	//---------------------------------------------------------------------
	bool near_equal(const vector3& v) const
	{
		return (math_type::near_equal(x, v.x) && math_type::near_equal(y, v.y) && math_type::near_equal(z, v.z));
	}
	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	bool near_equal_zero() const
	{
		return (math_type::near_equal_zero(x) && math_type::near_equal_zero(y) && math_type::near_equal_zero(z));
	}
	//---------------------------------------------------------------------
	// 値がゼロか
	//---------------------------------------------------------------------
	bool equal_zero() const
	{
		return (x == math_type::zero && y == math_type::zero && z == math_type::zero);
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
		return x + y + z;
	}
	//---------------------------------------------------------------------
	// 距離を求める
	//---------------------------------------------------------------------
	T distance(const vector3& v) const
	{
		// 差を求めたあとの長さ
		const vector3 t(x - v.x, y - v.y, z - v.z);
		return t.length();
	}
	//---------------------------------------------------------------------
	// 方向を求める
	//---------------------------------------------------------------------
	vector3 direction(const vector3& to) const
	{
		vector3 t(to.x - x, to.y - y, to.z - z);
		return t.normalize();
	}
	vector3& direction(const vector3& to, vector3& result) const
	{
		result.x = to.x - x;
		result.y = to.y - y;
		result.z = to.z - z;
		return result.normalize();
	}
	//---------------------------------------------------------------------
	// 内積を求める
	//---------------------------------------------------------------------
	T dot(const vector3& v) const
	{
		// |v1||v2|cos(θ)と同じになる
		// 値が０のときは垂直
		return x * v.x + y * v.y + z * v.z;
	}
	//---------------------------------------------------------------------
	// 外積を求める
	//---------------------------------------------------------------------
	vector3 cross(const vector3& v) const
	{
		// 二つのベクトルに垂直なベクトルを求める
		// ２Ｄでは高さを求めていたけどそれの応用でぞれぞれの軸の高さを求める
		return vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	vector3& cross(const vector3& v, vector3& result) const
	{
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		return result;
	}
	//---------------------------------------------------------------------
	// 正規化
	//---------------------------------------------------------------------
	vector3& normalize()
	{
		T len = length_sq();
		if (len > math_type::zero)
		{
			// 長さの逆数
			len = math_type::rsqrt(len);
			x *= len;
			y *= len;
			z *= len;
		}
		return *this;
	}
	vector3& normalize(vector3& result) const
	{
		result = *this;
		return result.normalize();
	}

	//---------------------------------------------------------------------
	// 正規化を求める
	//---------------------------------------------------------------------
	vector3 normalized() const
	{
		vector3 v = *this;
		return v.normalize();
	}
	//---------------------------------------------------------------------
	// 射影を求める
	//---------------------------------------------------------------------
	vector3 projection(const vector3& v) const
	{
		return *this * v.dot(*this);
	}
	vector3& projection(const vector3& v, vector3& result) const
	{
		result = *this;
		return result *= v.dot(*this);
	}
	//---------------------------------------------------------------------
	// 線形補間
	//---------------------------------------------------------------------
	vector3 lerp(const vector3& to, T t) const
	{
		return vector3(math_type::lerp(x, to.x, t), math_type::lerp(y, to.y, t), math_type::lerp(z, to.z, t));
	}
	vector3& lerp(const vector3& to, T t, vector3& result) const
	{
		result.x = math_type::lerp(x, to.x, t);
		result.y = math_type::lerp(y, to.y, t);
		result.z = math_type::lerp(z, to.z, t);
		return result;
	}
	vector3& lerp(const vector3& from, const vector3& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	//---------------------------------------------------------------------
	// 値を0～1にクランプ
	//---------------------------------------------------------------------
	vector3& saturate()
	{
		x = math_type::clamp01(x);
		y = math_type::clamp01(y);
		z = math_type::clamp01(z);
		return *this;
	}
	vector3& saturate(vector3& result) const
	{
		result.x = math_type::clamp01(x);
		result.y = math_type::clamp01(y);
		result.z = math_type::clamp01(z);
		return result;
	}
	vector3 saturated() const
	{
		return vector3(math_type::clamp01(x), math_type::clamp01(y), math_type::clamp01(z));
	}

	//---------------------------------------------------------------------
	// 最大値にする
	//---------------------------------------------------------------------
	vector3& maximize(const vector3& v)
	{
		x = math_type::max(x, v.x);
		y = math_type::max(y, v.y);
		z = math_type::max(z, v.z);
		return *this;
	}
	vector3& maximize(const vector3& v, vector3& result) const
	{
		result.x = math_type::max(x, v.x);
		result.y = math_type::max(y, v.y);
		result.z = math_type::max(z, v.z);
		return result;
	}
	vector3 maximized(const vector3& v) const
	{
		return vector3(math_type::max(x, v.x), math_type::max(y, v.y), math_type::max(z, v.z));
	}

	//---------------------------------------------------------------------
	// 最小値にする
	//---------------------------------------------------------------------
	vector3& minimize(const vector3& v)
	{
		x = math_type::min(x, v.x);
		y = math_type::min(y, v.y);
		z = math_type::min(z, v.z);
		return *this;
	}
	vector3& minimize(const vector3& v, vector3& result) const
	{
		result.x = math_type::min(x, v.x);
		result.y = math_type::min(y, v.y);
		result.z = math_type::min(z, v.z);
		return result;
	}
	vector3 minimized(const vector3& v) const
	{
		return vector3(math_type::min(x, v.x), math_type::min(y, v.y), math_type::min(z, v.z));
	}

	//---------------------------------------------------------------------
	// x軸回転角度を求める
	//---------------------------------------------------------------------
	T pitch() const
	{
		// Asin(y / |v|)

		T len = length_sq();
		if (len > math_type::zero)
		{
			return math_type::asin(-y / math_type::sqrt(len));
		}
		return math_type::zero;
	}
	//---------------------------------------------------------------------
	// y軸回転角度を求める
	//---------------------------------------------------------------------
	T yaw() const
	{
		if (length_sq() > math_type::zero)
		{
			// atan2は正規化されていなくてもいい
			return math_type::atan2(x, z);
		}
		return math_type::zero;
	}
	//---------------------------------------------------------------------
	// 座標変換
	//---------------------------------------------------------------------
	vector3& transform(const matrix4x4<T>&); // matrix4x4.h
	vector3& transform(const matrix4x4<T>&, vector3& result) const;
	vector3 transformed(const matrix4x4<T>&) const;
	vector3& transform_coord(const matrix4x4<T>&);
	vector3& transform_coord(const matrix4x4<T>&, vector3& result) const;
	vector3 transformed_coord(const matrix4x4<T>&) const;
	vector3& transform_normal(const matrix4x4<T>&);
	vector3& transform_normal(const matrix4x4<T>&, vector3& result) const;
	vector3 transformed_normal(const matrix4x4<T>&) const;
	vector3& transform(const matrix3x3<T>&); // matrix3x3.h
	vector3& transform(const matrix3x3<T>&, vector3& result) const;
	vector3 transformed(const matrix3x3<T>&) const;
	vector3& transform_normal(const matrix3x3<T>&);
	vector3& transform_normal(const matrix3x3<T>&, vector3& result) const;
	vector3 transformed_normal(const matrix3x3<T>&) const;
	//---------------------------------------------------------------------
	// 回転
	//---------------------------------------------------------------------
	vector3& rotate(const quaternion<T>&);
	vector3 rotated(const quaternion<T>&) const;
	vector3& rotate(const quaternion<T>&, vector3& result) const;
	vector3 rotated(const vector3& axis, T angle) const
	{
		vector3 result(behavior::noinitialize);
		return rotate(axis, angle, result);
	}
	vector3& rotate(const vector3& axis, T angle, vector3& result) const
	{
		// N(N,V) + [V-N(N,V)]cosθ - (V×N)sinθ

		vector3 calc(behavior::noinitialize);

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
	vector3& rotate(const vector3& axis, T angle)
	{
		const vector3 c(*this);
		return c.rotated(axis, angle, *this);
	}

	//---------------------------------------------------------------------
	// 並べ替え
	//---------------------------------------------------------------------
	vector2<T> swizzle(int x, int y) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 2);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 2);
		return vector2<T>((*this)[x], (*this)[y]);
	}
	vector3 swizzle(int x, int y, int z) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 2);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 2);
		POCKET_DEBUG_RANGE_ASSERT(z, 0, 2);
		return vector3((*this)[x], (*this)[y], (*this)[z]);
	}
	vector4<T> swizzle(int, int, int, int) const; // vector4.h

	//-----------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// アクセス演算子
	//---------------------------------------------------------------------
	T& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 2);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 2);
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
	POCKET_CXX11_EXPLICIT operator vector3<U>() const
	{
		return vector3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
	}
	POCKET_CXX11_EXPLICIT operator vector2<T>() const
	{
		return vector2<T>(x, y);
	}
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
	bool operator == (const vector3& v) const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}
	bool operator != (const vector3& v) const
	{
		return !(*this == v);
	}
	bool operator < (const vector3& v) const
	{
		return x < v.x && y < v.y && z < v.z;
	}
	bool operator <= (const vector3& v) const
	{
		return x <= v.x && y <= v.y && z <= v.z;
	}
	bool operator > (const vector3& v) const
	{
		return x > v.x && y > v.y && z > v.z;
	}
	bool operator >= (const vector3& v) const
	{
		return x >= v.x && y >= v.y && z >= v.z;
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	vector3 operator + () const
	{
		return *this;
	}
	vector3 operator - () const
	{
		return vector3(-x, -y, -z);
	}
	vector3& operator ++ ()
	{
		++x;
		++y;
		++z;
		return *this;
	}
	vector3 operator ++ (int)
	{
		const vector3 r = *this;
		++(*this);
		return r;
	}
	vector3& operator -- ()
	{
		--x;
		--y;
		--z;
		return *this;
	}
	vector3 operator -- (int)
	{
		const vector3 r = *this;
		--(*this);
		return r;
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	vector3 operator + (const vector3& v) const
	{
		vector3 result(behavior::noinitialize);
		return add(v, result);
	}
	template <typename U>
	vector3 operator + (const vector3<U>& v) const
	{
		vector3 result(behavior::noinitialize);
		return add(v, result);
	}
	vector3 operator - (const vector3& v) const
	{
		vector3 result(behavior::noinitialize);
		return subtract(v, result);
	}
	template <typename U>
	vector3 operator - (const vector3<U>& v) const
	{
		vector3 result(behavior::noinitialize);
		return subtract(v, result);
	}
	vector3 operator * (const matrix3x3<T>&) const; // matrix3x3.h
	vector3 operator * (const matrix4x4<T>&) const; // matrix4x4.h
	vector3 operator * (const quaternion<T>&) const; // quaternion.h
	vector3 operator * (T f) const
	{
		vector3 result(behavior::noinitialize);
		return multiply(f, result);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3 operator * (U f) const
	{
		vector3 result(behavior::noinitialize);
		return multiply<U>(f, result);
	}
	vector3 operator / (T f) const
	{
		vector3 result(behavior::noinitialize);
		return divide(f, result);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3 operator / (U f) const
	{
		vector3 result(behavior::noinitialize);
		return divide(f, result);
	}
	vector3 operator % (T f) const
	{
		return vector3(math_type::remainder(x, f), math_type::remainder(y, f), math_type::remainder(z, f));
	}
	vector3 operator % (const vector3& v) const
	{
		return vector3(math_type::remainder(x, v.x), math_type::remainder(y, v.y), math_type::remainder(z, v.z));
	}

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
	vector3& operator = (T f)
	{
		x = y = z = f;
		return *this;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3& operator = (U f)
	{
		*this = static_cast<T>(f);
		return *this;
	}
	vector3& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector3& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector3& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector3& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}
	vector3& operator = (const vector2<T>& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	template <typename U>
	vector3& operator = (const vector2<U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		return *this;
	}
	template <typename U>
	vector3& operator = (const vector3<U>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		return *this;
	}
	vector3& operator = (const vector4<T>&);
	template <typename U> vector3& operator = (const vector4<U>&); // vector4.h

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	vector3& operator += (const vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	template <typename U>
	vector3& operator += (const vector3<U>& v)
	{
		x += static_cast<T>(v.x);
		y += static_cast<T>(v.y);
		z += static_cast<T>(v.z);
		return *this;
	}
	vector3& operator -= (const vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	template <typename U>
	vector3& operator -= (const vector3<U>& v)
	{
		x -= static_cast<T>(v.x);
		y -= static_cast<T>(v.y);
		z -= static_cast<T>(v.z);
		return *this;
	}
	vector3& operator *= (T f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	vector3& operator *= (const matrix4x4<T>& m)
	{
		return transform(m);
	}
	vector3& operator *= (const quaternion<T>& q)
	{
		return rotate(q);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3& operator *= (U f)
	{
		x *= static_cast<T>(f);
		y *= static_cast<T>(f);
		z *= static_cast<T>(f);
		return *this;
	}
	vector3& operator /= (T f)
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		return *this *= math_type::reciprocal(f);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector3& operator /= (U f)
	{
		POCKET_DEBUG_ASSERT(f != math_traits<U>::zero);
		return *this *= math_type::reciprocal(static_cast<T>(f));
	}
	vector3& operator %= (T f)
	{
		math_type::remainder_assign(x, f);
		math_type::remainder_assign(y, f);
		math_type::remainder_assign(z, f);
		return *this;
	}
	vector3& operator %= (const vector3& v)
	{
		math_type::remainder_assign(x, v.x);
		math_type::remainder_assign(y, v.y);
		math_type::remainder_assign(z, v.z);
		return *this;
	}

	//------------------------------------------------------------------------------------------
	// タグでの関数呼び出し
	//------------------------------------------------------------------------------------------

	vector3& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector3& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector3& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector3& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}

	vector3 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	vector3 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	vector3 operator () (const behavior::_add_t&, const vector3& v) const
	{
		return operator+(v);
	}
	vector3 operator () (const behavior::_sub_t&, const vector3& v) const
	{
		return operator-(v);
	}
	vector3 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	vector3 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	vector3 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	vector3 operator () (const behavior::_rem_t&, const vector3& v) const
	{
		return operator%(v);
	}

	vector3& operator () (const behavior::_add_assign_t&, const vector3& v)
	{
		return operator+=(v);
	}
	vector3& operator () (const behavior::_sub_assign_t&, const vector3& v)
	{
		return operator-=(v);
	}
	vector3& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	vector3& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	vector3& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	vector3& operator () (const behavior::_rem_assign_t&, const vector3& v)
	{
		return operator%=(v);
	}
	vector3& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	vector3 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	vector3& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	vector3 operator () (const behavior::_decrement_back_t&)
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

	bool operator () (const behavior::_equal_t&, const vector3& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const vector3& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const vector3& v) const
	{
		return near_equal(v);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return near_equal_zero();
	}

	T operator () (const behavior::_length_t&) const
	{
		return length();
	}
	T operator () (const behavior::_length_square_t&) const
	{
		return length_sq();
	}
	T operator () (const behavior::_dot_t&, const vector3& v) const
	{
		return dot(v);
	}
	vector3& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	vector3 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	vector3 operator () (const behavior::_cross_t&, const vector3& v) const
	{
		return cross(v);
	}
	vector3 operator () (const behavior::_lerp_t&, const vector3& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const vector3& v) const
	{
		return distance(v);
	}
	vector3 operator () (const behavior::_direction_t&, const vector3& v) const
	{
		return direction(v);
	}
	vector3 operator () (const behavior::_direction_t&, const vector3& v, T force) const
	{
		return direction(v) * force;
	}
	vector3& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	vector3 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	vector2<T> operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	vector3 operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	vector4<T> operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
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
	vector3& operator () (const behavior::_from_pitch_yaw_t&, T pitch, T yaw)
	{
		return from_pitch_yaw(pitch, yaw);
	}
	vector3 operator () (const behavior::_transform_t&, const matrix4x4<T>& m) const
	{
		return transformed(m);
	}
	vector3 operator () (const behavior::_transform_coord_t&, const matrix4x4<T>& m) const
	{
		return transformed_coord(m);
	}
	vector3 operator () (const behavior::_transform_normal_t&, const matrix4x4<T>& m) const
	{
		return transformed_normal(m);
	}
	vector3 operator () (const behavior::_rotate_t&, const quaternion<T>& q) const
	{
		return rotated(q);
	}
	vector3 operator () (const behavior::_rotate_t&, const vector3& axis, T angle) const
	{
		return rotated(axis, angle);
	}
};

template <typename T>
const vector3<T> vector3<T>::zero(math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::one(math_type::one, math_type::one, math_type::one);
template <typename T>
const vector3<T> vector3<T>::unit_x(math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::unit_y(math_type::zero, math_type::one, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::unit_z(math_type::zero, math_type::zero, math_type::one);
template <typename T>
const vector3<T> vector3<T>::up(math_type::zero, math_type::one, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::down(math_type::zero, -math_type::one, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::right(math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::left(-math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector3<T> vector3<T>::forward(math_type::zero, math_type::zero, math_type::one);
template <typename T>
const vector3<T> vector3<T>::backward(math_type::zero, math_type::zero, -math_type::one);

// 左辺が数値の場合の乗算演算子

template <typename T> inline
vector3<T> operator * (T f, const vector3<T>& v)
{
	return v * f;
}


//---------------------------------------------------------------------
// vector2
//---------------------------------------------------------------------
template <typename T> inline
vector2<T>::vector2(const vector3<T>& v) :
	x(v.x), y(v.y)
{}
template <typename T>
template <typename U> inline
vector2<T>::vector2(const vector3<U>& v) :
	x(static_cast<T>(v.y)), y(static_cast<T>(v.x))
{}

template <typename T> inline
vector3<T> vector2<T>::swizzle(int x, int y, int z) const
{
	POCKET_DEBUG_RANGE_ASSERT(x, 0, 2);
	POCKET_DEBUG_RANGE_ASSERT(y, 0, 2);
	POCKET_DEBUG_RANGE_ASSERT(z, 0, 2);
	return vector3<T>((*this)[x], (*this)[y], (*this)[z]);
}

template <typename T> inline
vector2<T>& vector2<T>::operator = (const vector3<T>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}
template <typename T>
template <typename U> inline
vector2<T>& vector2<T>::operator = (const vector3<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	return *this;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vector3<T>& v)
{
	// (x, y, z)
	os << io::parentheses_left << v.x << io::comma_space
		<< v.y << io::comma_space
		<< v.z << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, vector3<T>& v)
{
	is.ignore();
	is >> v.x;
	is.ignore();
	is >> v.y;
	is.ignore();
	is >> v.z;
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_VECTOR3_H__
