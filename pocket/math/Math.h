#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "fwd.h"
#include "Debug.h"
#include "array.h"
#include <cmath>
#include <cfloat>
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename> class Math;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Math<int> Mathi;
typedef Math<float> Mathf;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Math<double> Mathd;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Math<long double> Mathld;
#endif /* _USING_MATH_LONG_DOUBLE */

namespace detail
{
template <typename T>
struct math_int_t;

template <>
struct math_int_t<int>
{
	typedef int type;
	_STATICAL_CONSTANT type SignBit = 1 << 31;
};
template <>
struct math_int_t<float>
{
	typedef uint32_t type;
	_STATICAL_CONSTANT type SignBit = 1UL << 31;
};
template <>
struct math_int_t<double>
{
	typedef uint64_t type;
	_STATICAL_CONSTANT type SignBit = 1ULL << 63;
};
}

template <typename T>
class Math
{
public:
	_MATH_STATICAL_ASSERT(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	struct SinCos
	{
		T Sin; /* サイン値 */
		T Cos; /* コサイン値 */

		_DEFAULT_CONSTRUCTOR(SinCos);
		SinCos(const behavior::_noinitialize_t&)
		{

		}
		SinCos(T s, T c) :
			Sin(s), Cos(c)
		{

		}

		SinCos(T f)
		{
			//Sin = Math<T>::Sin(f);
			//Cos = Math<T>::Cos(f);
			Math::sin_cos(f, Sin, Cos);
		}
	};

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;

	typedef detail::math_int_t<T> int_type;
	typedef typename int_type::type int_value_type;

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

	/* None */

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const T Zero; /* 0.0 */
	static const T Half; /* 0.5 */
	static const T HalfOfHalf; /* 0.25 */
	static const T One; /* 1.0 */
	static const T Two; /* 2.0 */
	static const T Three; /* 3.0 */
	static const T Four; /* 4.0 */
	static const T HalfAngle; /* 180.0 */
	static const T Infinity; /* #.INF */
	static const T Epsilon; /* 1.0 + Epsilon > 1.0 となる値 */
	static const T Maximum; /* 最大値 */
	static const T Minimum; /* 最小値 */

	static const T Rad2Deg; /* Radian -> Degree */
	static const T Deg2Rad; /* Degree -> Radian */

	static const T PI; /* 3.141592654 */
	static const T PIxTwo; /* PI * 2.0 */
	static const T OneDivPI; /* 1.0 / PI */
	static const T OneDivPIxTwo; /* 1.0 / (PI * 2.0) */
	static const T PIDivTwo; /* PI / 2.0 */
	static const T PIDivFour; /* PI / 4.0 */

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	/* None */

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	static inline bool is_near_zero(T f)
	{
		return (f >= -Math::Epsilon && f <= Math::Epsilon);
	}
	/*---------------------------------------------------------------------
	* 値が１に近いか
	*---------------------------------------------------------------------*/
	static inline bool is_near_one(T f)
	{
		return Math::is_near(f, Math::One);
	}

	/*---------------------------------------------------------------------
	* 値が指定の値に近いか
	*---------------------------------------------------------------------*/
	static inline bool is_near(T f, T value)
	{
		return (f <= (value + Math::Epsilon) && f >= (value - Math::Epsilon));
	}
	/*---------------------------------------------------------------------
	* 整数部分が近いか
	*---------------------------------------------------------------------*/
	template <int N>
	static inline bool is_near(T f)
	{
		return (f <= (static_cast<T>(N) + Math::Epsilon) && f >= (static_cast<T>(N) - Math::Epsilon));
	}

	/*---------------------------------------------------------------------
	* 逆数を求める
	*---------------------------------------------------------------------*/
	static inline T reciprocal(T f)
	{
		_DEB_ASSERT(!Math::is_near_zero(f));
		return (Math::One / f);
	}

	/*---------------------------------------------------------------------
	* 角度をラジアンへ変換
	*---------------------------------------------------------------------*/
	static inline T to_radian(T f)
	{
		return (f * Math::Deg2Rad);
	}

	/*---------------------------------------------------------------------
	* ラジアンを角度へ変換
	*---------------------------------------------------------------------*/
	static inline T to_degree(T f)
	{
		return (f * Math::Rad2Deg);
	}

	/*---------------------------------------------------------------------
	* 角度からサインを求める
	*---------------------------------------------------------------------*/
	static inline T sin(T deg)
	{
		return std::sin(deg * Math::Deg2Rad);
	}

	/*---------------------------------------------------------------------
	* 角度からコサインを求める
	*---------------------------------------------------------------------*/
	static inline T cos(T deg)
	{
		return std::cos(deg * Math::Deg2Rad);
	}

	/*---------------------------------------------------------------------
	* 角度からサイン、コサインを求める
	*---------------------------------------------------------------------*/
	static inline void sin_cos(T deg, T& sin, T& cos)
	{
		cos = Math::sin(deg);
		sin = Math::cos(deg);
	}
	static inline void sin_cos(T deg, SinCos& sc)
	{
		sc.Sin = Math::sin(deg);
		sc.Cos = Math::cos(deg);
	}
	static inline typename Math::SinCos sin_cos(T deg)
	{
		return typename Math::SinCos(Math::sin(deg), Math::cos(deg));
	}

	/*---------------------------------------------------------------------
	* 角度からタンジェントを求める
	*---------------------------------------------------------------------*/
	static inline T tan(T deg)
	{
		return std::tan(deg * Math::Deg2Rad);
	}

	/*---------------------------------------------------------------------
	* 逆サイン（角度）を求める
	*---------------------------------------------------------------------*/
	static inline T asin(T x)
	{
		return (std::asin(x) * Math::Rad2Deg);
	}

	/*---------------------------------------------------------------------
	* 逆コサイン（角度）を求める
	*---------------------------------------------------------------------*/
	static inline T acos(T x)
	{
		return (std::acos(x) * Math::Rad2Deg);
	}

	/*---------------------------------------------------------------------
	* 逆タンジェント（角度）を求める
	*---------------------------------------------------------------------*/
	static inline T atan(T x)
	{
		return (std::atan(x) * Math::Rad2Deg);
	}
	static inline T atan2(T y, T x)
	{
		return (std::atan2(y, x) * Math::Rad2Deg);
	}

	/*---------------------------------------------------------------------
	* 値の四捨五入を求める
	*---------------------------------------------------------------------*/
	static inline T round(T x)
	{
#ifdef _USE_CXX11
		return std::round(x);
#else
		T y = Math::remainder(x, Math::One);
		if (y >= Math::Half)
		{
			return Math::ceil(x);
		}
		return Math::floor(x);
#endif /* _USE_CXX11 */
	}
	/*---------------------------------------------------------------------
	* 値の四捨五入した値を整数型で返す
	*---------------------------------------------------------------------*/
	static inline int round_to_int(T x)
	{
		return static_cast<int>(Math::round(x));
	}

	/*---------------------------------------------------------------------
	* 値の絶対値を求める
	*---------------------------------------------------------------------*/
	static inline T abs(T x)
	{
		return std::abs(x);
	}

	/*---------------------------------------------------------------------
	* 値の切り上げを求める
	*---------------------------------------------------------------------*/
	static inline T ceil(T x)
	{
		return std::ceil(x);
	}
	/*---------------------------------------------------------------------
	* 値の切り上げした値を整数型で返す
	*---------------------------------------------------------------------*/
	static inline int ceil_to_int(T x)
	{
		return static_cast<int>(Math::ceil(x));
	}

	/*---------------------------------------------------------------------
	* 値の切り捨てを求める
	*---------------------------------------------------------------------*/
	static inline T floor(T x)
	{
		return std::floor(x);
	}
	/*---------------------------------------------------------------------
	* 値の切り下げした値を整数型で返す
	*---------------------------------------------------------------------*/
	static inline int floor_to_int(T x)
	{
		return static_cast<int>(Math::floor(x));
	}

	/*---------------------------------------------------------------------
	* 平方根を求める
	*---------------------------------------------------------------------*/
	static inline T sqrt(T x)
	{
		return std::sqrt(x);
	}

	/*---------------------------------------------------------------------
	* 逆平方根を求める
	*---------------------------------------------------------------------*/
	static inline T rsqrt(T x)
	{
		return Math::One / Math::sqrt(x);
	}

	/*---------------------------------------------------------------------
	* 冪乗を求める
	*---------------------------------------------------------------------*/
	static inline T pow(T x, T y)
	{
		return std::pow(x, y);
	}

	/*---------------------------------------------------------------------
	* ２乗を求める
	*---------------------------------------------------------------------*/
	static inline T sqr(T x)
	{
		return (x * x);
	}

	/*---------------------------------------------------------------------
	* 値以上で２の冪乗の値を求める
	*---------------------------------------------------------------------*/
	static inline T power_of_two(T x)
	{
		/* マイナス対応 */
		T r = x >= Math::Zero ? Math::Two : -Math::Two;
		while (r < x)
		{
			r *= Math::Two;
		}
		return r;
	}

	/*---------------------------------------------------------------------
	* 値が２の冪乗の値か
	*---------------------------------------------------------------------*/
	static inline bool is_power_of_two(T x)
	{
		return (power_of_two(x) == x);
	}

	/*---------------------------------------------------------------------
	* 大きい方の値を返す
	*---------------------------------------------------------------------*/
	static inline const T& max(const T& x, const T& y)
	{
		return x > y ? x : y;
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& max(const VectorN<T>& x, const VectorN<T>& y)
	{
		return x > y ? x : y;
	}

	/*---------------------------------------------------------------------
	* 小さい方の値を返す
	*---------------------------------------------------------------------*/
	static inline const T& min(const T& x, const T& y)
	{
		return x < y ? x : y;
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& min(const VectorN<T>& x, const VectorN<T>& y)
	{
		return x < y ? x : y;
	}

	/*---------------------------------------------------------------------
	* 値をクランプ
	*---------------------------------------------------------------------*/
	static inline const T& clamp(const T& v, const T& min, const T& max)
	{
		return Math::max(min, Math::min(max, v));
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& clamp(const VectorN<T>& v, const VectorN<T>& min, const VectorN<T>& max)
	{
		return Math::max(min, Math::min(max, v));
	}
	/*---------------------------------------------------------------------
	* 値を0～1へクランプ
	*---------------------------------------------------------------------*/
	static inline const T& clamp01(const T& v)
	{
		return clamp(v, Math::Zero, Math::One);
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& clamp01(const VectorN<T>& v)
	{
		typedef VectorN<T> vector_type;
		return clamp(v, vector_type::Zero, vector_type::One);
	}
	static inline T& saturate(T& v)
	{
		v = Math::clamp01(v);
		return v;
	}
	template <template <typename> class VectorN>
	static inline VectorN<T>& saturate(VectorN<T>& v)
	{
		return v.saturate();
	}

	/*---------------------------------------------------------------------
	* 線形補間を求める
	*---------------------------------------------------------------------*/
	static inline T lerp(T from, T to, T t)
	{
		return (from * (Math::One - t)) + (to * t);
	}
	template <typename U>
	static inline T lerp(T from, T to, U t)
	{
		return (from * (Math<U>::One - t)) + (to * t);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> lerp(const VectorN<T>& from, const VectorN<T>& to, T t)
	{
		return from.lerp(to, t);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T>& lerp(const VectorN<T>& from, const VectorN<T>& to, T t, VectorN<T>& result)
	{
		return from.lerp(to, t, result);
	}

	/*---------------------------------------------------------------------
	* ベジェ曲線の線形補間を求める
	*---------------------------------------------------------------------*/
	static inline T bezier(T from, T center, T to, T t)
	{
		/* lerp(lerp(from, center, t), to, t)の式展開 */

		T t0, t1, t2;
		Math::bezier_coefficient(t, t0, t1, t2);
		return (from * t0 + center * t1 + to * t2);
	}
	template <template <typename> class MathType>
	static inline MathType<T> bezier(const MathType<T>& from, const MathType<T>& center, const MathType<T>& to, T t)
	{
		T t0, t1, t2;
		Math::bezier_coefficient(t, t0, t1, t2);
		return (from * t0 + center * t1 + to * t2);
	}
	/*---------------------------------------------------------------------
	* ベジェ曲線で使われる係数を求める
	*---------------------------------------------------------------------*/
	static inline void bezier_coefficient(T t, T& t0, T& t1, T& t2)
	{
		t0 = Math::sqr(Math::One - t);
		t1 = Math::Two * (t - t * t);
		t2 = t * t;
	}

	/*---------------------------------------------------------------------
	* CatmullRom曲線を求める
	*---------------------------------------------------------------------*/
	static inline T catmull_rom(T p1, T p2, T p3, T p4, T t)
	{
		/* http://markun.cs.shinshu-u.ac.jp/learn/cg/cg3/index4.html */
		/* 式展開はFerfuson/Coons式から求められる */
		/* Ferfuson/Coonsの速度を制御点からの位置関係から求めるので速度を渡さないですむ */
		/* 初期速度は制御点1-3の半分、最終速度は制御点2-4の半分とする(定義) */
		/* X(t) = (2p0 - 2p1 + v0 + v1)t^3 + (-3p0 + 3p1 - 2v0 - v1)t^2 + v0t + p0 */

		/* 初期速度 */
		T v0 = (p3 - p1) * Math::Half;
		/* 最終速度 */
		T v1 = (p4 - p2) * Math::Half;

		T t2 = t * t;
		T t3 = t2 * t;
		//T f1 = ((Math::Two * p1 - Math::Two * p2) + v0 + v1) * t3;
		T f1 = (((p1 - p2) * Math::Two) + v0 + v1) * t3;
		//T f2 = ((-Math::Three * p1 + Math::Three * p2) - Math::Two * v0 - v1) * t2;
		T f2 = (((p2 - p1) * Math::Three) - Math::Two * v0 - v1) * t2;
		T f3 = v0 * t;
		return (f1 + f2 + f3 + p1);
	}

	/*---------------------------------------------------------------------
	* 逆から線形補間を求める
	*---------------------------------------------------------------------*/
	static inline T lerp_invert(T from, T to, T t)
	{
		return lerp(from, to, Math::One - t);
	}
	template <typename U>
	static inline T lerp_invert(T from, T to, U t)
	{
		return lerp<U>(from, to, Math<U>::One - t);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> lerp_invert(const VectorN<T>& from, const VectorN<T>& to, T t)
	{
		return lerp(from, to, Math::One - t);
	}

	/*---------------------------------------------------------------------
	* 直線の方程式を求める
	*---------------------------------------------------------------------*/
	static inline T linear_equation(const T& p, const T& v, T t)
	{
		return (p + v * t);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> linear_equation(const VectorN<T>& p, const VectorN<T>& v, T t)
	{
		VectorN<T> result(p), d;
		v.multiply(t, d);
		result += d;
		return result;
	}

	/*---------------------------------------------------------------------
	* 型で使用されている先頭アドレスを求める
	*---------------------------------------------------------------------*/
	static inline T* to_pointer(T& t)
	{
		return &t;
	}
	static inline const T* to_pointer(const T& t)
	{
		return &t;
	}
	template <template <typename> class MathType>
	static inline T* to_pointer(MathType<T>& t)
	{
		return static_cast<T*>(t);
	}
	template <template <typename> class MathType>
	static inline const T* to_pointer(const MathType<T>& t)
	{
		return static_cast<const T*>(t);
	}

	/*---------------------------------------------------------------------
	* 指定の型へキャスト
	*---------------------------------------------------------------------*/
	template <typename U>
	static inline U cast(const T& t)
	{
		return static_cast<U>(t);
	}
	template <typename U, template <typename> class MathType>
	static inline MathType<U> cast(const MathType<T>& t)
	{
		return static_cast<MathType<U> >(t);
	}

	/*---------------------------------------------------------------------
	* ベクトルの並び替え
	*---------------------------------------------------------------------*/
	template <int X, int Y, template <typename> class VectorN>
	static inline Vector2<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y);
	}
	template <int X, int Y, int Z, template <typename> class VectorN>
	static inline Vector3<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y, Z);
	}
	template <int X, int Y, int Z, int W, template <typename> class VectorN>
	static inline Vector4<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y, Z, W);
	}

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	static inline T add(T x, T y)
	{
		return (x + y);
	}
	template <template <typename> class MathType>
	static inline MathType<T> add(const MathType<T>& x, const MathType<T>& y)
	{
		return (x + y);
	}
	static inline T& add_assign(T& x, T y)
	{
		x += y;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& add_assign(MathType<T>& x, const MathType<T>& y)
	{
		x += y;
		return x;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	static inline T subtract(T x, T y)
	{
		return (x - y);
	}
	template <template <typename> class MathType>
	static inline MathType<T> subtract(const MathType<T>& x, const MathType<T>& y)
	{
		return (x - y);
	}
	static inline T& subtract_assign(T& x, T y)
	{
		x -= y;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& subtract_assign(MathType<T>& x, const MathType<T>& y)
	{
		x -= y;
		return x;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	static inline T multiply(T x, T y)
	{
		return (x * y);
	}
	template <template <typename> class MathType>
	static inline MathType<T> multiply(const MathType<T>& x, T s)
	{
		return (x * s);
	}
	template <template <typename> class MathType>
	static inline MathType<T> multiply(T s, const MathType<T>& x)
	{
		return (x * s);
	}
	template <template <typename> class MathType>
	static inline MathType<T> multiply(const MathType<T>& x, const MathType<T>& y)
	{
		return (x * y);
	}
	template <template <typename> class MathType, template <typename> class MathType2>
	static inline MathType<T> multiply(const MathType<T>& x, const MathType2<T>& y)
	{
		return (x * y);
	}
	static inline T& multiply_assign(T& x, T y)
	{
		x *= y;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& multiply_assign(MathType<T>& x, T s)
	{
		x *= s;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& multiply_assign(T s, MathType<T>& x)
	{
		return multiply_assign(x, s);
	}
	template <template <typename> class MathType>
	static inline MathType<T>& multiply_assign(MathType<T>& x, const MathType<T>& y)
	{
		x *= y;
		return x;
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	static inline T divide(T x, T y)
	{
		_DEB_ASSERT(y != Math::Zero);
		return (x / y);
	}
	template <template <typename> class MathType>
	static inline MathType<T> divide(const MathType<T>& x, T s)
	{
		_DEB_ASSERT(s != Math::Zero);
		s = Math::One / s;
		return multiply(x, s);
	}
	static inline T divide_assign(T& x, T y)
	{
		_DEB_ASSERT(y != Math::Zero);
		x /= y;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& divide_assign(MathType<T>& x, T s)
	{
		_DEB_ASSERT(s != Math::Zero);
		s = Math::One / s;
		return multiply_assign(x, s);
	}
	/*---------------------------------------------------------------------
	* 余りを求める
	*---------------------------------------------------------------------*/
	static inline T remainder(T x, T y)
	{
		_DEB_ASSERT(!Math::is_near_zero(y));
		return (x >= y ? std::fmod(x, y) : x);
		//return std::fmod(x, y);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> remainder(const VectorN<T>& x, T s)
	{
		return (x % s);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> remainder(const VectorN<T>& x, const VectorN<T>& y)
	{
		return (x % y);
	}
	static inline T& remainder_assign(T& x, T y)
	{
		_DEB_ASSERT(!Math::is_near_zero(y));
		if (x >= y)
		{
			x = std::fmod(x, y);
		}
		return x;
	}
	template <template <typename> class VectorN>
	static inline VectorN<T>& remainder_assign(VectorN<T>& x, T s)
	{
		x %= s;
		return x;
	}
	template <template <typename> class VectorN>
	static inline VectorN<T>& remainder_assign(VectorN<T>& x, const VectorN<T>& y)
	{
		x %= y;
		return x;
	}

	/*-----------------------------------------------------------------------------------------
	* Operatots
	*-----------------------------------------------------------------------------------------*/

	/* None */
};

#ifndef _UNUSING_MATH_INT_FLOAT
/*---------------------------------------------------------------------
* int特有の挙動
*---------------------------------------------------------------------*/
template <>
inline bool Math<int>::is_near_zero(int f)
{
	return (f == 0);
}
template <>
inline bool Math<int>::is_near_one(int f)
{
	return (f == 1);
}
template <>
inline bool Math<int>::is_near(int f, int value)
{
	return (f == value);
}
template <>
template <int N>
inline bool Math<int>::is_near(int f)
{
	return (f == N);
}
template <>
inline int Math<int>::reciprocal(int f)
{
	return 0;
}
template <>
inline int Math<int>::round(int x)
{
	return x;
}
template <>
inline int Math<int>::round_to_int(int x)
{
	return x;
}
template <>
inline int Math<int>::ceil(int x)
{
	return x;
}
template <>
inline int Math<int>::ceil_to_int(int x)
{
	return x;
}
template <>
inline int Math<int>::floor(int x)
{
	return x;
}
template <>
inline int Math<int>::floor_to_int(int x)
{
	return x;
}
template <>
inline int Math<int>::remainder(int x, int y)
{
	_DEB_ASSERT(y > 0);
	return (x % y);
}
template <>
inline int& Math<int>::remainder_assign(int& x, int y)
{
	_DEB_ASSERT(y > 0);
	x %= y;
	return x;
}
#endif /* _UNUSING_MATH_INT_FLOAT */

#ifndef _UNUSING_MATH_INT_FLOAT
template <> const int Math<int>::Zero = 0;
template <> const int Math<int>::Half = 0;
template <> const int Math<int>::HalfOfHalf = 0;
template <> const int Math<int>::One = 1;
template <> const int Math<int>::Two = 2;
template <> const int Math<int>::Three = 3;
template <> const int Math<int>::Four = 4;
template <> const int Math<int>::HalfAngle = 180;
template <> const int Math<int>::PI = 3;
template <> const int Math<int>::Infinity = 0;
template <> const int Math<int>::Epsilon = 0;
template <> const int Math<int>::Maximum = INT_MAX;
template <> const int Math<int>::Minimum = INT_MIN;
template <> const float Math<float>::Zero = 0.0f;
template <> const float Math<float>::Half = 0.5f;
template <> const float Math<float>::HalfOfHalf = 0.25f;
template <> const float Math<float>::One = 1.0f;
template <> const float Math<float>::Two = 2.0f;
template <> const float Math<float>::Three = 3.0f;
template <> const float Math<float>::Four = 4.0f;
template <> const float Math<float>::HalfAngle = 180.0f;
template <> const float Math<float>::PI = 3.141592654f;
template <> const float Math<float>::Infinity = HUGE_VALF;
template <> const float Math<float>::Epsilon = FLT_EPSILON;
template <> const float Math<float>::Maximum = FLT_MAX;
template <> const float Math<float>::Minimum = FLT_MIN;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
template <> const double Math<double>::Zero = 0.0;
template <> const double Math<double>::Half = 0.5;
template <> const double Math<double>::HalfOfHalf = 0.25;
template <> const double Math<double>::One = 1.0;
template <> const double Math<double>::Two = 2.0;
template <> const double Math<double>::Three = 3.0;
template <> const double Math<double>::Four = 4.0;
template <> const double Math<double>::HalfAngle = 180.0;
template <> const double Math<double>::PI = 3.141592654;
template <> const double Math<double>::Infinity = HUGE_VAL;
template <> const double Math<double>::Epsilon = DBL_EPSILON;
template <> const double Math<double>::Maximum = DBL_MAX;
template <> const double Math<double>::Minimum = DBL_MIN;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
template <> const long double Math<long double>::Zero = 0.0L;
template <> const long double Math<long double>::Half = 0.5L;
template <> const long double Math<long double>::HalfOfHalf = 0.25L;
template <> const long double Math<long double>::One = 1.0L;
template <> const long double Math<long double>::Two = 2.0L;
template <> const long double Math<long double>::Three = 3.0L;
template <> const long double Math<long double>::Four = 4.0L;
template <> const long double Math<long double>::HalfAngle = 180.0L;
template <> const long double Math<long double>::PI = 3.141592654L;
template <> const long double Math<long double>::Infinity = HUGE_VALL;
template <> const long double Math<long double>::Epsilon = LDBL_EPSILON;
template <> const long double Math<long double>::Maximum = LDBL_MAX;
template <> const long double Math<long double>::Minimum = LDBL_MIN;
#endif /* _USING_MATH_LONG_DOUBLE */

template <typename T>
const T Math<T>::PIxTwo = Math<T>::PI * Math<T>::Two;
template <typename T>
const T Math<T>::OneDivPI = Math<T>::One / Math<T>::PI;
template <typename T>
const T Math<T>::OneDivPIxTwo = Math<T>::One / (Math<T>::PI * Math<T>::Two);
template <typename T>
const T Math<T>::PIDivTwo = Math<T>::PI / Math<T>::Two;
template <typename T>
const T Math<T>::PIDivFour = Math<T>::PI / Math<T>::Four;
template <typename T>
const T Math<T>::Rad2Deg = Math<T>::HalfAngle / Math<T>::PI;
template <typename T>
const T Math<T>::Deg2Rad = Math<T>::PI / Math<T>::HalfAngle;

/* 数学クラスの関数を＊演算子で呼び出す */

#ifdef _USE_CXX11
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_sqr_t&)
{
	return Math<T>::sqr(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_radians_t&)
{
	return Math<T>::to_radian(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_degrees_t&)
{
	return Math<T>::to_degree(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_sin_t&)
{
	return Math<T>::sin(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_cos_t&)
{
	return Math<T>::cos(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_tan_t&)
{
	return Math<T>::tan(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_asin_t&)
{
	return Math<T>::asin(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_acos_t&)
{
	return Math<T>::acos(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_atan_t&)
{
	return Math<T>::atan(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
typename Math<T>::SinCos operator * (const T& f, const behavior::_sin_cos_t&)
{
	return Math<T>::sin_cos(f);
}

template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_round_t&)
{
	return Math<T>::round(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_abs_t&)
{
	return Math<T>::abs(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_ceil_t&)
{
	return Math<T>::ceil(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_floor_t&)
{
	return Math<T>::floor(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_sqrt_t&)
{
	return Math<T>::sqrt(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_pot_t&)
{
	return Math<T>::power_of_two(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
bool operator * (const T& f, const behavior::_is_pot_t&)
{
	return Math<T>::is_power_of_two(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
T operator * (const T& f, const behavior::_clamp01_t&)
{
	return Math<T>::clamp01(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
Vector2<T> operator * (const T& f, const behavior::_vec2_t&)
{
	return Vector2<T>(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
Vector3<T> operator * (const T& f, const behavior::_vec3_t&)
{
	return Vector3<T>(f);
}
template <typename T, _TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)>
Vector4<T> operator * (const T& f, const behavior::_vec4_t&)
{
	return Vector4<T>(f, f);
}
#endif /* _USE_CXX11 */

#ifdef _USING_MATH_IO

namespace detail
{
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Math<int>::SinCos MathSinCosi;
typedef Math<float>::SinCos MathSinCosf;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Math<double>::SinCos MathSinCosd;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Math<long double>::SinCos MathSinCosld;
#endif /* _USING_MATH_LONG_DOUBLE */
}

/* 型を直接指定するとエラーになってしまうので */
#ifndef _MATH_SIN_COS_OUTPUT_OPERATOR
#define _MATH_SIN_COS_OUTPUT_OPERATOR(TYPE) template <typename CharT, typename CharTraits> inline\
	std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const TYPE& s)\
	{\
		os << out_char::parentheses_left << CharT('S') << out_char::colon << out_char::space << s.Sin \
			<< out_char::comma_space << CharT('C') << out_char::colon << out_char::space << s.Cos << out_char::parentheses_right;\
		return os;\
	}\
	template <typename CharT, typename CharTraits> inline\
	std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const TYPE& s)\
	{\
		os << out_char::parentheses_left << CharT('S') << out_char::colon << out_char::space << s.Sin \
			<< out_char::comma_space << CharT('C') << out_char::colon << out_char::space << s.Cos << out_char::parentheses_right;\
		return os;\
	}
#endif /* _MATH_SIN_COS_OUTPUT_OPERATOR */

#ifndef _UNUSING_MATH_INT_FLOAT
_MATH_SIN_COS_OUTPUT_OPERATOR(detail::MathSinCosi);
_MATH_SIN_COS_OUTPUT_OPERATOR(detail::MathSinCosf);
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
_MATH_SIN_COS_OUTPUT_OPERATOR(detail::MathSinCosd);
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
_MATH_SIN_COS_OUTPUT_OPERATOR(detail::MathSinCosld);
#endif /* _USING_MATH_LONG_DOUBLE */

#undef _MATH_SIN_COS_OUTPUT_OPERATOR
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_MATH_H__ */
