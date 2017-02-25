#ifndef __POCKET_MATH_MATH_TRAITS_H__
#define __POCKET_MATH_MATH_TRAITS_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../debug.h"
#include "../call.h"
#include "../fwd.h"
#include "fwd.h"
#include "../container/array.h"
#include "../io.h"
#include <cmath>
#include <cfloat>
#include <climits>

namespace pocket
{
namespace math
{

template <typename> struct math_traits;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef math_traits<int> math_traitsi;
typedef math_traits<float> math_traitsf;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef math_traits<double> math_traitsd;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef math_traits<long double> math_traitsld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

namespace detail
{
template <typename T>
struct math_int_t;

template <>
struct math_int_t<int>
{
	typedef int type;
	POCKET_STATICAL_CONSTANT type sign_bit = 1 << 31;
};
template <>
struct math_int_t<float>
{
	typedef uint32_t type;
	POCKET_STATICAL_CONSTANT type sign_bit = 1UL << 31;
};
template <>
struct math_int_t<double>
{
	typedef uint64_t type;
	POCKET_STATICAL_CONSTANT type sign_bit = 1ULL << 63;
};
}

template <typename T>
struct math_traits
{
	POCKET_MATH_STATICAL_ASSERT(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	struct sin_cos_type
	{
		T sin; // サイン値
		T cos; // コサイン値

		POCKET_DEFAULT_CONSTRUCTOR(sin_cos_type);
		explicit sin_cos_type(const call::noinitialize_t&)
		{}
		explicit sin_cos_type(T s, T c) :
			sin(s), cos(c)
		{}
		sin_cos_type(T f)
		{
			//sin = math_traits<T>::sin(f);
			//cos = math_traits<T>::cos(f);
			math_traits::sin_cos(f, sin, cos);
		}

		sin_cos_type operator + (const sin_cos_type& sc) const
		{
			return sin_cos_type(sin+sc.sin, cos+sc.cos);
		}
		sin_cos_type operator - (const sin_cos_type& sc) const
		{
			return sin_cos_type(sin-sc.sin, cos-sc.cos);
		}
		sin_cos_type operator * (T f) const
		{
			return sin_cos_type(sin*f, cos*f);
		}
		sin_cos_type operator / (T f) const
		{
			f = math_traits::reciprocal(f);
			return sin_cos_type(sin*f, cos*f);
		}
	};

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;

	typedef detail::math_int_t<T> int_type;
	typedef typename int_type::type value_int_type;

	//-----------------------------------------------------------------------------------------
	// Members
	//-----------------------------------------------------------------------------------------

	// none

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const T zero; // 0.0
	static const T half; // 0.5
	static const T half_of_half; // 0.25
	static const T one; // 1.0
	static const T two; // 2.0
	static const T three; // 3.0
	static const T four; // 4.0
	static const T half_angle; // 180.0
	static const T infinity; // #.INF
	static const T epsilon; // 1.0 + epsilon > 1.0 となる値
	static const T maximum; // 最大値
	static const T minimum; // 最小値

	static const T rad2deg; // Radian -> Degree
	static const T deg2rad; // Degree -> Radian

	static const T pi; // 3.141592654
	static const T pi_x_two; // pi * 2.0
	static const T one_div_pi; // 1.0 / pi
	static const T one_div_pi_x_two; // 1.0 / (pi * 2.0)
	static const T pi_div_two; // pi / 2.0
	static const T pi_div_four; // pi / 4.0

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	static inline bool near_equal_zero(T f)
	{
		return (f >= -math_traits::epsilon && f <= math_traits::epsilon);
	}
	//---------------------------------------------------------------------
	// 値が１に近いか
	//---------------------------------------------------------------------
	static inline bool near_one(T f)
	{
		return math_traits::near_equal(f, math_traits::one);
	}

	//---------------------------------------------------------------------
	// 値が指定の値に近いか
	//---------------------------------------------------------------------
	static inline bool near_equal(T f, T value)
	{
		return (f <= (value + math_traits::epsilon) && f >= (value - math_traits::epsilon));
	}
	//---------------------------------------------------------------------
	// 整数部分が近いか
	//---------------------------------------------------------------------
	template <int N>
	static inline bool near_equal(T f)
	{
		return (f <= (static_cast<T>(N) + math_traits::epsilon) && f >= (static_cast<T>(N) - math_traits::epsilon));
	}

	//---------------------------------------------------------------------
	// 逆数を求める
	//---------------------------------------------------------------------
	static inline T reciprocal(T f)
	{
		POCKET_DEBUG_ASSERT(!math_traits::near_equal_zero(f));
		return (math_traits::one / f);
	}

	//---------------------------------------------------------------------
	// 角度をラジアンへ変換
	//---------------------------------------------------------------------
	static inline T to_radian(T f)
	{
		return (f * math_traits::deg2rad);
	}

	//---------------------------------------------------------------------
	// ラジアンを角度へ変換
	//---------------------------------------------------------------------
	static inline T to_degree(T f)
	{
		return (f * math_traits::rad2deg);
	}

	//---------------------------------------------------------------------
	// 角度からサインを求める
	//---------------------------------------------------------------------
	static inline T sin(T deg)
	{
		return std::sin(deg * math_traits::deg2rad);
	}

	//---------------------------------------------------------------------
	// 角度からコサインを求める
	//---------------------------------------------------------------------
	static inline T cos(T deg)
	{
		return std::cos(deg * math_traits::deg2rad);
	}

	//---------------------------------------------------------------------
	// 角度からサイン、コサインを求める
	//---------------------------------------------------------------------
	static inline void sin_cos(T deg, T& s, T& c)
	{
		s = math_traits::sin(deg);
		c = math_traits::cos(deg);
	}
	static inline void sin_cos(T deg, sin_cos_type& sc)
	{
		sc.sin = math_traits::sin(deg);
		sc.cos = math_traits::cos(deg);
	}
	static inline sin_cos_type sin_cos(T deg)
	{
		return sin_cos_type(math_traits::sin(deg), math_traits::cos(deg));
	}

	//---------------------------------------------------------------------
	// 角度からタンジェントを求める
	//---------------------------------------------------------------------
	static inline T tan(T deg)
	{
		return std::tan(deg * math_traits::deg2rad);
	}

	//---------------------------------------------------------------------
	// 逆サイン（角度）を求める
	//---------------------------------------------------------------------
	static inline T asin(T x)
	{
		return (std::asin(x) * math_traits::rad2deg);
	}

	//---------------------------------------------------------------------
	// 逆コサイン（角度）を求める
	//---------------------------------------------------------------------
	static inline T acos(T x)
	{
		return (std::acos(x) * math_traits::rad2deg);
	}

	//---------------------------------------------------------------------
	// 逆タンジェント（角度）を求める
	//---------------------------------------------------------------------
	static inline T atan(T x)
	{
		return (std::atan(x) * math_traits::rad2deg);
	}
	static inline T atan2(T y, T x)
	{
		return (std::atan2(y, x) * math_traits::rad2deg);
	}

	//---------------------------------------------------------------------
	// 値の四捨五入を求める
	//---------------------------------------------------------------------
	static inline T round(T x)
	{
#ifdef POCKET_USE_CXX11
		return std::round(x);
#else
		T y = math_traits::remainder(x, math_traits::one);
		if (y >= math_traits::half)
		{
			return math_traits::ceil(x);
		}
		return math_traits::floor(x);
#endif // POCKET_USE_CXX11
	}
	//---------------------------------------------------------------------
	// 値の四捨五入した値を整数型で返す
	//---------------------------------------------------------------------
	static inline int round_to_int(T x)
	{
		return static_cast<int>(math_traits::round(x));
	}

	//---------------------------------------------------------------------
	// 値の絶対値を求める
	//---------------------------------------------------------------------
	static inline T abs(T x)
	{
		return std::abs(x);
	}

	//---------------------------------------------------------------------
	// 値の切り上げを求める
	//---------------------------------------------------------------------
	static inline T ceil(T x)
	{
		return std::ceil(x);
	}
	//---------------------------------------------------------------------
	// 値の切り上げした値を整数型で返す
	//---------------------------------------------------------------------
	static inline int ceil_to_int(T x)
	{
		return static_cast<int>(math_traits::ceil(x));
	}

	//---------------------------------------------------------------------
	// 値の切り捨てを求める
	//---------------------------------------------------------------------
	static inline T floor(T x)
	{
		return std::floor(x);
	}
	//---------------------------------------------------------------------
	// 値の切り下げした値を整数型で返す
	//---------------------------------------------------------------------
	static inline int floor_to_int(T x)
	{
		return static_cast<int>(math_traits::floor(x));
	}

	//---------------------------------------------------------------------
	// 平方根を求める
	//---------------------------------------------------------------------
	static inline T sqrt(T x)
	{
		return std::sqrt(x);
	}

	//---------------------------------------------------------------------
	// 逆平方根を求める
	//---------------------------------------------------------------------
	static inline T rsqrt(T x)
	{
		return math_traits::one / math_traits::sqrt(x);
	}

	//---------------------------------------------------------------------
	// 冪乗を求める
	//---------------------------------------------------------------------
	static inline T pow(T x, T y)
	{
		return std::pow(x, y);
	}

	//---------------------------------------------------------------------
	// ２乗を求める
	//---------------------------------------------------------------------
	static inline T sqr(T x)
	{
		return (x * x);
	}

	//---------------------------------------------------------------------
	// 値以上で２の冪乗の値を求める
	//---------------------------------------------------------------------
	static inline T power_of_two(T x)
	{
		T r = x >= math_traits::zero ? math_traits::two : -math_traits::two;
		while (r < x)
		{
			r *= math_traits::two;
		}
		return r;
	}

	//---------------------------------------------------------------------
	// 値が２の冪乗の値か
	//---------------------------------------------------------------------
	static inline bool is_power_of_two(T x)
	{
		return (power_of_two(x) == x);
	}

	//---------------------------------------------------------------------
	// 大きい方の値を返す
	//---------------------------------------------------------------------
	static inline const T& max(const T& x, const T& y)
	{
		return x > y ? x : y;
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& max(const VectorN<T>& x, const VectorN<T>& y)
	{
		return x > y ? x : y;
	}

	//---------------------------------------------------------------------
	// 小さい方の値を返す
	//---------------------------------------------------------------------
	static inline const T& min(const T& x, const T& y)
	{
		return x < y ? x : y;
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& min(const VectorN<T>& x, const VectorN<T>& y)
	{
		return x < y ? x : y;
	}

	//---------------------------------------------------------------------
	// 値をクランプ
	//---------------------------------------------------------------------
	static inline const T& clamp(const T& v, const T& min, const T& max)
	{
		return math_traits::max(min, math_traits::min(max, v));
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& clamp(const VectorN<T>& v, const VectorN<T>& min, const VectorN<T>& max)
	{
		return math_traits::max(min, math_traits::min(max, v));
	}
	//---------------------------------------------------------------------
	// 値を0～1へクランプ
	//---------------------------------------------------------------------
	static inline const T& clamp01(const T& v)
	{
		return clamp(v, math_traits::zero, math_traits::one);
	}
	template <template <typename> class VectorN>
	static inline const VectorN<T>& clamp01(const VectorN<T>& v)
	{
		typedef VectorN<T> vector_type;
		return clamp(v, vector_type::zero, vector_type::one);
	}
	static inline T& saturate(T& v)
	{
		v = math_traits::clamp01(v);
		return v;
	}
	template <template <typename> class VectorN>
	static inline VectorN<T>& saturate(VectorN<T>& v)
	{
		return v.saturate();
	}

	//---------------------------------------------------------------------
	// 線形補間を求める
	//---------------------------------------------------------------------
	static inline T lerp(T from, T to, T t)
	{
		return (from * (math_traits::one - t)) + (to * t);
	}
	template <typename U>
	static inline T lerp(T from, T to, U t)
	{
		return (from * (math_traits<U>::one - t)) + (to * t);
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

	//---------------------------------------------------------------------
	// ベジェ曲線の線形補間を求める
	//---------------------------------------------------------------------
	static inline T bezier(T from, T center, T to, T t)
	{
		// lerp(lerp(from, center, t), to, t)の式展開

		T t0, t1, t2;
		math_traits::bezier_coefficient(t, t0, t1, t2);
		return (from * t0 + center * t1 + to * t2);
	}
	template <template <typename> class MathType>
	static inline MathType<T> bezier(const MathType<T>& from, const MathType<T>& center, const MathType<T>& to, T t)
	{
		T t0, t1, t2;
		math_traits::bezier_coefficient(t, t0, t1, t2);
		return (from * t0 + center * t1 + to * t2);
	}
	//---------------------------------------------------------------------
	// ベジェ曲線で使われる係数を求める
	//---------------------------------------------------------------------
	static inline void bezier_coefficient(T t, T& t0, T& t1, T& t2)
	{
		t0 = math_traits::sqr(math_traits::one - t);
		t1 = math_traits::two * (t - t * t);
		t2 = t * t;
	}

	//---------------------------------------------------------------------
	// CatmullRom曲線を求める
	//---------------------------------------------------------------------
	static inline T catmull_rom(T p1, T p2, T p3, T p4, T t)
	{
		// http://markun.cs.shinshu-u.ac.jp/learn/cg/cg3/index4.html
		// 式展開はFerfuson/Coons式から求められる
		// Ferfuson/Coonsの速度を制御点からの位置関係から求めるので速度を渡さないですむ
		// 初期速度は制御点1-3の半分、最終速度は制御点2-4の半分とする(定義)
		// X(t) = (2p0 - 2p1 + v0 + v1)t^3 + (-3p0 + 3p1 - 2v0 - v1)t^2 + v0t + p0

		// 初期速度
		T v0 = (p3 - p1) * math_traits::half;
		// 最終速度
		T v1 = (p4 - p2) * math_traits::half;

		T t2 = t * t;
		T t3 = t2 * t;
		//T f1 = ((math_traits::two * p1 - math_traits::two * p2) + v0 + v1) * t3;
		T f1 = (((p1 - p2) * math_traits::two) + v0 + v1) * t3;
		//T f2 = ((-math_traits::three * p1 + math_traits::three * p2) - math_traits::two * v0 - v1) * t2;
		T f2 = (((p2 - p1) * math_traits::three) - math_traits::two * v0 - v1) * t2;
		T f3 = v0 * t;
		return (f1 + f2 + f3 + p1);
	}

	//---------------------------------------------------------------------
	// 逆から線形補間を求める
	//---------------------------------------------------------------------
	static inline T lerp_invert(T from, T to, T t)
	{
		return lerp(from, to, math_traits::one - t);
	}
	template <typename U>
	static inline T lerp_invert(T from, T to, U t)
	{
		return lerp<U>(from, to, math_traits<U>::one - t);
	}
	template <template <typename> class VectorN>
	static inline VectorN<T> lerp_invert(const VectorN<T>& from, const VectorN<T>& to, T t)
	{
		return lerp(from, to, math_traits::one - t);
	}

	//---------------------------------------------------------------------
	// 直線の方程式を求める
	//---------------------------------------------------------------------
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

	//---------------------------------------------------------------------
	// 型で使用されている先頭アドレスを求める
	//---------------------------------------------------------------------
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

	//---------------------------------------------------------------------
	// 指定の型へキャスト
	//---------------------------------------------------------------------
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

	//---------------------------------------------------------------------
	// ベクトルの並び替え
	//---------------------------------------------------------------------
	template <int X, int Y, template <typename> class VectorN>
	static inline vector2<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y);
	}
	template <int X, int Y, int Z, template <typename> class VectorN>
	static inline vector3<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y, Z);
	}
	template <int X, int Y, int Z, int W, template <typename> class VectorN>
	static inline vector4<T> swizzle(const VectorN<T>& t)
	{
		return t.swizzle(X, Y, Z, W);
	}

	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
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
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
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
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
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
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	static inline T divide(T x, T y)
	{
		POCKET_DEBUG_ASSERT(y != math_traits::zero);
		return (x / y);
	}
	template <template <typename> class MathType>
	static inline MathType<T> divide(const MathType<T>& x, T s)
	{
		POCKET_DEBUG_ASSERT(s != math_traits::zero);
		s = math_traits::one / s;
		return multiply(x, s);
	}
	static inline T divide_assign(T& x, T y)
	{
		POCKET_DEBUG_ASSERT(y != math_traits::zero);
		x /= y;
		return x;
	}
	template <template <typename> class MathType>
	static inline MathType<T>& divide_assign(MathType<T>& x, T s)
	{
		POCKET_DEBUG_ASSERT(s != math_traits::zero);
		s = math_traits::one / s;
		return multiply_assign(x, s);
	}
	//---------------------------------------------------------------------
	// 余りを求める
	//---------------------------------------------------------------------
	static inline T remainder(T x, T y)
	{
		POCKET_DEBUG_ASSERT(!math_traits::near_equal_zero(y));
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
		POCKET_DEBUG_ASSERT(!math_traits::near_equal_zero(y));
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

	//-----------------------------------------------------------------------------------------
	// Operatots
	//-----------------------------------------------------------------------------------------

	// none
};

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
//---------------------------------------------------------------------
// int特有の挙動
//---------------------------------------------------------------------
template <> inline
bool math_traits<int>::near_equal_zero(int f)
{
	return (f == 0);
}
template <> inline
bool math_traits<int>::near_one(int f)
{
	return (f == 1);
}
template <> inline
bool math_traits<int>::near_equal(int f, int value)
{
	return (f == value);
}
template <>
template <int N> inline
bool math_traits<int>::near_equal(int f)
{
	return (f == N);
}
template <> inline
int math_traits<int>::reciprocal(int f)
{
	static_cast<void>(f);
	return 0;
}
template <> inline
int math_traits<int>::round(int x)
{
	return x;
}
template <> inline
int math_traits<int>::round_to_int(int x)
{
	return x;
}
template <> inline
int math_traits<int>::ceil(int x)
{
	return x;
}
template <> inline
int math_traits<int>::ceil_to_int(int x)
{
	return x;
}
template <> inline
int math_traits<int>::floor(int x)
{
	return x;
}
template <> inline
int math_traits<int>::floor_to_int(int x)
{
	return x;
}
template <> inline
int math_traits<int>::remainder(int x, int y)
{
	POCKET_DEBUG_ASSERT(y > 0);
	return (x % y);
}
template <> inline
int& math_traits<int>::remainder_assign(int& x, int y)
{
	POCKET_DEBUG_ASSERT(y > 0);
	x %= y;
	return x;
}
#endif // POCKET_NO_USING_MATH_INT_FLOAT

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
template <> const int math_traits<int>::zero = 0;
template <> const int math_traits<int>::half = 0;
template <> const int math_traits<int>::half_of_half = 0;
template <> const int math_traits<int>::one = 1;
template <> const int math_traits<int>::two = 2;
template <> const int math_traits<int>::three = 3;
template <> const int math_traits<int>::four = 4;
template <> const int math_traits<int>::half_angle = 180;
template <> const int math_traits<int>::pi = 3;
template <> const int math_traits<int>::infinity = 0;
template <> const int math_traits<int>::epsilon = 0;
template <> const int math_traits<int>::maximum = INT_MAX;
template <> const int math_traits<int>::minimum = INT_MIN;
template <> const float math_traits<float>::zero = 0.0f;
template <> const float math_traits<float>::half = 0.5f;
template <> const float math_traits<float>::half_of_half = 0.25f;
template <> const float math_traits<float>::one = 1.0f;
template <> const float math_traits<float>::two = 2.0f;
template <> const float math_traits<float>::three = 3.0f;
template <> const float math_traits<float>::four = 4.0f;
template <> const float math_traits<float>::half_angle = 180.0f;
template <> const float math_traits<float>::pi = 3.141592654f;
template <> const float math_traits<float>::infinity = HUGE_VALF;
template <> const float math_traits<float>::epsilon = FLT_EPSILON;
template <> const float math_traits<float>::maximum = FLT_MAX;
template <> const float math_traits<float>::minimum = FLT_MIN;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
template <> const double math_traits<double>::zero = 0.0;
template <> const double math_traits<double>::half = 0.5;
template <> const double math_traits<double>::half_of_half = 0.25;
template <> const double math_traits<double>::one = 1.0;
template <> const double math_traits<double>::two = 2.0;
template <> const double math_traits<double>::three = 3.0;
template <> const double math_traits<double>::four = 4.0;
template <> const double math_traits<double>::half_angle = 180.0;
template <> const double math_traits<double>::pi = 3.141592654;
template <> const double math_traits<double>::infinity = HUGE_VAL;
template <> const double math_traits<double>::epsilon = DBL_EPSILON;
template <> const double math_traits<double>::maximum = DBL_MAX;
template <> const double math_traits<double>::minimum = DBL_MIN;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
template <> const long double math_traits<long double>::zero = 0.0L;
template <> const long double math_traits<long double>::half = 0.5L;
template <> const long double math_traits<long double>::half_of_half = 0.25L;
template <> const long double math_traits<long double>::one = 1.0L;
template <> const long double math_traits<long double>::two = 2.0L;
template <> const long double math_traits<long double>::three = 3.0L;
template <> const long double math_traits<long double>::four = 4.0L;
template <> const long double math_traits<long double>::half_angle = 180.0L;
template <> const long double math_traits<long double>::pi = 3.141592654L;
template <> const long double math_traits<long double>::infinity = HUGE_VALL;
template <> const long double math_traits<long double>::epsilon = LDBL_EPSILON;
template <> const long double math_traits<long double>::maximum = LDBL_MAX;
template <> const long double math_traits<long double>::minimum = LDBL_MIN;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
const T math_traits<T>::pi_x_two = math_traits<T>::pi * math_traits<T>::two;
template <typename T>
const T math_traits<T>::one_div_pi = math_traits<T>::one / math_traits<T>::pi;
template <typename T>
const T math_traits<T>::one_div_pi_x_two = math_traits<T>::one / (math_traits<T>::pi * math_traits<T>::two);
template <typename T>
const T math_traits<T>::pi_div_two = math_traits<T>::pi / math_traits<T>::two;
template <typename T>
const T math_traits<T>::pi_div_four = math_traits<T>::pi / math_traits<T>::four;
template <typename T>
const T math_traits<T>::rad2deg = math_traits<T>::half_angle / math_traits<T>::pi;
template <typename T>
const T math_traits<T>::deg2rad = math_traits<T>::pi / math_traits<T>::half_angle;

namespace detail
{
#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef math_traits<int>::sin_cos_type math_traits_sin_cos_ti;
typedef math_traits<float>::sin_cos_type math_traits_sin_cos_tf;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef math_traits<double>::sin_cos_type math_traits_sin_cos_td;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef math_traits<long double>::sin_cos_type math_traits_sin_cos_tld;
#endif // POCKET_USING_MATH_LONG_DOUBLE
}

// 型を直接指定するとエラーになってしまうたので
#define __POCKET_MATH_SIN_COS_OUTPUT_OPERATOR(TYPE) template <typename CharT, typename CharTraits> inline\
	std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const TYPE& s)\
	{\
		os << io::parentheses_left << io::widen("sin: ") << s.sin \
			<< io::comma_space << io::widen("cos: ") << s.cos << io::parentheses_right;\
		return os;\
	}

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
__POCKET_MATH_SIN_COS_OUTPUT_OPERATOR(math::detail::math_traits_sin_cos_ti);
__POCKET_MATH_SIN_COS_OUTPUT_OPERATOR(math::detail::math_traits_sin_cos_tf);
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
__POCKET_MATH_SIN_COS_OUTPUT_OPERATOR(math::detail::math_traits_sin_cos_td);
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
__POCKET_MATH_SIN_COS_OUTPUT_OPERATOR(math::detail::math_traits_sin_cos_tld);
#endif // POCKET_USING_MATH_LONG_DOUBLE

#undef __POCKET_MATH_SIN_COS_OUTPUT_OPERATOR

} // namespace math
} // namespace pocket

  // 数学クラスの関数を＊演算子で呼び出す

#ifdef POCKET_USE_CXX11
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::sqr_t &)
{
	return pocket::math::math_traits<T>::sqr(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::radians_t &)
{
	return pocket::math::math_traits<T>::to_radian(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::degrees_t &)
{
	return pocket::math::math_traits<T>::to_degree(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::sin_t &)
{
	return pocket::math::math_traits<T>::sin(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::cos_t &)
{
	return pocket::math::math_traits<T>::cos(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::tan_t &)
{
	return pocket::math::math_traits<T>::tan(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::asin_t &)
{
	return pocket::math::math_traits<T>::asin(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::acos_t &)
{
	return pocket::math::math_traits<T>::acos(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::atan_t &)
{
	return pocket::math::math_traits<T>::atan(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
typename pocket::math::math_traits<T>::sin_cos_type operator * (const T& f, const pocket::call::sin_cos_t &)
{
	return pocket::math::math_traits<T>::sin_cos(f);
}

template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::round_t &)
{
	return pocket::math::math_traits<T>::round(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::abs_t &)
{
	return pocket::math::math_traits<T>::abs(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::ceil_t &)
{
	return pocket::math::math_traits<T>::ceil(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::floor_t &)
{
	return pocket::math::math_traits<T>::floor(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::sqrt_t &)
{
	return pocket::math::math_traits<T>::sqrt(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::pot_t &)
{
	return pocket::math::math_traits<T>::power_of_two(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
bool operator * (const T& f, const pocket::call::is_pot_t &)
{
	return pocket::math::math_traits<T>::is_power_of_two(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
T operator * (const T& f, const pocket::call::clamp01_t &)
{
	return pocket::math::math_traits<T>::clamp01(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
pocket::math::vector2<T> operator * (const T& f, const pocket::call::vec2_t &)
{
	return pocket::math::vector2<T>(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
pocket::math::vector3<T> operator * (const T& f, const pocket::call::vec3_t &)
{
	return pocket::math::vector3<T>(f);
}
template <typename T, POCKET_TEMPLATE_TYPE_VALIDATE_MATH_TYPE(T)> inline
pocket::math::vector4<T> operator * (const T& f, const pocket::call::vec4_t &)
{
	return pocket::math::vector4<T>(f, f);
}
#endif // POCKET_USE_CXX11

#endif // __POCKET_MATH_MATH_TRAITS_H__
