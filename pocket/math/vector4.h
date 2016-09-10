#ifndef __POCKET_MATH_VECTOR4_H__
#define __POCKET_MATH_VECTOR4_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../behavior.h"
#include "../debug.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector2.h"
#include "vector3.h"
#ifdef POCKET_USE_SIMD_ANONYMOUS
#include "simd_traits.h"
#endif // POCKET_USE_SIMD_ANONYMOUS
#include "../io.h"

namespace pocket
{
namespace math
{

template <typename> struct vector4;
template <typename> struct matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef vector4<int> Point4;
typedef vector4<int> vector4i;
typedef vector4<float> vector4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef vector4<double> vector4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef vector4<long double> vector4ld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct vector4
{
	POCKET_MATH_STATICAL_ASSERT(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;

	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef simd_traits<T> simd;
	typedef typename simd::type simd_type;
#endif // POCKET_USE_SIMD_ANONYMOUS

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

	// 匿名は使用できないがsimdは使用できる
#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			T x;
			T y;
			T z;
			T w;

#ifdef POCKET_USE_ANONYMOUS
		};

#ifdef POCKET_USE_ANONYMOUS_NON_POD
		struct
		{
			vector2<T> xy;
		};
		struct
		{
			vector3<T> xyz;
		};
#endif // POCKET_USE_ANONYMOUS_NON_POD

#ifdef POCKET_USE_SIMD_ANONYMOUS
		// simdが使用できる場合は演算に使用する
		simd_type mm;
#endif // POCKET_USE_SIMD_ANONYMOUS

		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct vector4;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const vector4 zero; // 0.0, 0.0, 0.0, 0.0
	static const vector4 one; // 1.0, 1.0, 1.0, 1.0
	static const vector4 unit_x; // 1.0, 0.0, 0.0, 0.0
	static const vector4 unit_y; // 0.0, 1.0, 0.0, 0.0
	static const vector4 unit_z; // 0.0, 0.0, 1.0, 0.0
	static const vector4 unit_w; // 0.0, 0.0, 0.0, 1.0
	static const vector4 up; // 0.0, 1.0, 0.0, 0.0
	static const vector4 down; // 0.0, -1.0, 0.0, 0.0
	static const vector4 right; // 1.0, 0.0, 0.0, 0.0
	static const vector4 left; // -1.0, 0.0, 0.0, 0.0
	static const vector4 forward; // 0.0, 0.0, 1.0, 0.0
	static const vector4 backward; // 0.0, 0.0, -1.0, 0.0

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	POCKET_DEFAULT_CONSTRUCTOR(vector4);
	explicit vector4(const behavior::_noinitialize_t&)
	{

	}
	explicit vector4(const behavior::_zero_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::zero())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::zero), y(math_type::zero), z(math_type::zero),
#	else
		xyz(math_type::zero, math_type::zero, math_type::zero),
#	endif
		w(math_type::zero)
#endif
	{

	}
	explicit vector4(const behavior::_zero_t&, const behavior::_direction_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::zero())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::zero), y(math_type::zero), z(math_type::zero),
#	else
		xyz(math_type::zero, math_type::zero, math_type::zero),
#	endif
		w(math_type::zero)
#endif
	{

	}
	explicit vector4(const behavior::_zero_t&, const behavior::_position_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::zero())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::zero), y(math_type::zero), z(math_type::zero),
#	else
		xyz(math_type::zero, math_type::zero, math_type::zero),
#	endif
		w(math_type::zero)
#endif
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w1();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	explicit vector4(const behavior::_one_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::one())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::one), y(math_type::one), z(math_type::one),
#	else
		xyz(math_type::one, math_type::one, math_type::one),
#	endif
		w(math_type::one)
#endif
	{

	}
	explicit vector4(const behavior::_one_t&, const behavior::_direction_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::one())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::one), y(math_type::one), z(math_type::one),
#	else
		xyz(math_type::one, math_type::one, math_type::one),
#	endif
		w(math_type::zero)
#endif
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w0();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	explicit vector4(const behavior::_one_t&, const behavior::_position_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::one())
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::one), y(math_type::one), z(math_type::one),
#	else
		xyz(math_type::one, math_type::one, math_type::one),
#	endif
		w(math_type::one)
#endif
	{

	}
	explicit vector4(const behavior::_half_t&, const behavior::_direction_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::half))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half), y(math_type::half), z(math_type::half),
#	else
		xyz(math_type::half, math_type::half, math_type::half),
#	endif
		w(math_type::zero)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w0();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	explicit vector4(const behavior::_half_t&, const behavior::_position_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::half))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half), y(math_type::half), z(math_type::half),
#	else
		xyz(math_type::half, math_type::half, math_type::half),
#	endif
		w(math_type::one)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w1();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	explicit vector4(const behavior::_half_of_half_t&, const behavior::_direction_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::half_of_half))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half_of_half), y(math_type::half_of_half), z(math_type::half_of_half),
#	else
		xyz(math_type::half_of_half, math_type::half_of_half, math_type::half_of_half),
#	endif
		w(math_type::zero)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w0();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	explicit vector4(const behavior::_half_of_half_t&, const behavior::_position_t&) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(math_type::half_of_half))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(math_type::half_of_half), y(math_type::half_of_half), z(math_type::half_of_half),
#	else
		xyz(math_type::half_of_half, math_type::half_of_half, math_type::half_of_half),
#	endif
		w(math_type::one)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		w1();
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	vector4(T x, T y, T z, T w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(x, y, z, w))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(x), y(y), z(z),
#	else
		xyz(x, y, z),
#	endif
		w(w)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	template <typename U, typename U1, typename U2, typename U3,
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U1),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U2),
		POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U3)>
	vector4(U x, U1 y, U2 z, U3 w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w)))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)),
#	else
		xyz(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)),
#	endif
		w(static_cast<T>(w))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	explicit vector4(T f, T w = math_type::one) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(f, f, f, w))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(f), y(f), z(f),
#	else
		xyz(f),
#	endif
		w(w)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	explicit vector4(U f, U w = math_traits<U>::one) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(f), static_cast<T>(f), static_cast<T>(f), w))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(f)), y(static_cast<T>(f)), z(static_cast<T>(f)),
#	else
		xyz(static_cast<T>(f)),
#	endif
		w(static_cast<T>(w))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	vector4(const vector2<T>& v, T z, T w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(v.x, v.y, z, w))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(v.x), y(v.y), z(z),
#	else
		xyz(v, z),
#	endif
		w(w)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	template <typename U>
	vector4(const vector2<U>& v, U z, U w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(z), static_cast<T>(w)))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(z)),
#	else
		xyz(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(z)),
#	endif
		w(static_cast<T>(w))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	explicit vector4(const vector3<T>& v, T w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(v.x, v.y, v.z, w))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(v.x), y(v.y), z(v.z),
#	else
		xyz(v),
#	endif
		w(w)
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	template <typename U>
	vector4(const vector3<U>& v, U w) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), static_cast<T>(w)))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)),
#	else
		xyz(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z)),
#	endif
		w(static_cast<T>(w))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	template <typename U>
	vector4(const vector4<U>& v) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::set(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), static_cast<T>(v.w)))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)),
#	else
		xyz(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z)),
#	endif
		w(static_cast<T>(v.w))
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
	explicit vector4(const T* p) :
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm(simd::load(p))
#else
#	ifdef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
		x(p[0]), y(p[1]), z(p[2]),
#	else
		xyz(p[0], p[1], p[2]),
#	endif
		w(p[3])
#endif // POCKET_USE_SIMD_ANONYMOUS
	{

	}
#ifdef POCKET_USE_SIMD_ANONYMOUS
	vector4(simd_type mm) :
		mm(mm)
	{

	}
#endif // POCKET_USE_SIMD_ANONYMOUS

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* x軸回転角度とy軸回転角度からベクトルを求める
	*---------------------------------------------------------------------*/
	vector4& from_pitch_yaw(T pitch, T yaw)
	{
		x = math_type::cos(pitch) * math_type::sin(yaw);
		y = math_type::sin(pitch);
		z = math_type::cos(pitch) * math_type::cos(yaw);
		w = math_type::zero;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	vector4& add(const vector4& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::add(mm, v.mm);
#else
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;
		result.w = w + v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	template <typename U>
	vector4& add(const vector4<U>& v, vector4& result) const
	{
		return add(vector4(v), result);
	}
	vector4& add(const vector3<T>& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::add(mm, simd::set(v.x, v.y, v.z, math_type::zero));
#else
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;
		result.w = w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
	vector4& subtract(const vector4& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::sub(mm, v.mm);
#else
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
		result.w = w - v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	template <typename U>
	vector4& subtract(const vector4<U>& v, vector4& result) const
	{
		return subtract(vector4(v), result);
	}
	vector4& subtract(const vector3<T>& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::sub(mm, simd::set(v.x, v.y, v.z, math_type::zero));
#else
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
		result.w = w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
	vector4& multiply(T f, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::mul(mm, f);
#else
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		result.w = w * f;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& multiply(U f, vector4& result) const
	{
		return multiply(static_cast<T>(f), result);
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	vector4& divide(T f, vector4& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::div(mm, f);
		return result;
#else
		return multiply(math_type::reciprocal(f), result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& divide(U f, vector4& result) const
	{
		POCKET_DEBUG_ASSERT(f != math_traits<U>::zero);
		return divide(static_cast<T>(f), result);
	}

	/*---------------------------------------------------------------------
	* 平行か
	*---------------------------------------------------------------------*/
	bool is_parallel(const vector4& v) const
	{
		// 長さの積の大きさが一致していたら平行(+: 同方向, -: 逆方向)
		return math_type::is_near_zero(dot(v) - (length() * v.length()));
	}
	/*---------------------------------------------------------------------
	* 垂直
	*---------------------------------------------------------------------*/
	bool is_vertical(const vector4& v) const
	{
		return math_type::is_near_zero(dot(v));
	}
	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal(mm, v.mm);
#else
		return (math_type::is_near(x, v.x) && math_type::is_near(y, v.y) && math_type::is_near(z, v.z) && math_type::is_near(w, v.w));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::near_equal_zero(mm);
#else
		return (math_type::is_near_zero(x) && math_type::is_near_zero(y) && math_type::is_near_zero(z) && math_type::is_near_zero(w));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	/*---------------------------------------------------------------------
	* 値がゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::equal(mm, vector4::zero.mm);
#else
		return (x == math_type::zero && y == math_type::zero && z == math_type::zero && w == math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* wを0に設定
	*---------------------------------------------------------------------*/
	vector4& w0()
	{
		w = math_type::zero;
		return *this;
	}
	/*---------------------------------------------------------------------
	* wを1に設定
	*---------------------------------------------------------------------*/
	vector4& w1()
	{
		w = math_type::one;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 長さを求める
	*---------------------------------------------------------------------*/
	T length() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::first(simd::length(mm));
#else
		return math_type::sqrt(dot(*this));
#endif // POCKET_USE_SIMD_ANONYMOUS
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
		return x + y + z + w;
	}
	/*---------------------------------------------------------------------
	* 距離を求める
	*---------------------------------------------------------------------*/
	T distance(const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		// 引き算したもののxyzのみを受け取る
		const vector4 t(simd::select1110(simd::sub(mm, v.mm)));
#else
		const vector4 t(x - v.x, y - v.y, z - v.z, math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return t.length();
	}
	/*---------------------------------------------------------------------
	* 方向を求める
	*---------------------------------------------------------------------*/
	vector4 direction(const vector4& to) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		vector4 t(simd::select1110(simd::sub(to.mm, mm)));
#else
		vector4 t(to.x - x, to.y - y, to.z - z, math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return t.normalize();
	}
	vector4& direction(const vector4& to, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::select1110(simd::sub(to.mm, mm));
#else
		result.x = to.x - x;
		result.y = to.y - y;
		result.z = to.z - z;
		result.w = math_type::zero;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 内積を求める
	*---------------------------------------------------------------------*/
	T dot(const vector4& v) const
	{
		// |v1||v2|cos(θ)と同じになる
		// 値が０のときは垂直
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::first(simd::dot(mm, v.mm));
#else
		return x * v.x + y * v.y + z * v.z + w * v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	/*---------------------------------------------------------------------
	* 外積を求める
	*---------------------------------------------------------------------*/
	vector4 cross(const vector4& v) const
	{
		// vector3分でw成分は0とする
#ifdef POCKET_USE_SIMD_ANONYMOUS

// 3, 0, 2, 1
#ifndef __Y_Z_X
#define __Y_Z_X 1, 2, 0, 3
#endif // __Y_Z_X

// 3, 1, 0, 2
#ifndef __Z_X_Y
#define __Z_X_Y 2, 0, 1, 3
#endif // __Z_X_Y

		simd_type syzx = simd::template permute<__Y_Z_X>(mm);
		simd_type szxy = simd::template permute<__Z_X_Y>(mm);
		simd_type tzxy = simd::template permute<__Z_X_Y>(v.mm);
		simd_type tyzx = simd::template permute<__Y_Z_X>(v.mm);
		simd_type m1 = simd::mul(syzx, tzxy);
		simd_type m2 = simd::mul(szxy, tyzx);

		return vector4(simd::select1110(simd::sub(m1, m2)));

#undef __Z_X_Y
#undef __Y_Z_X

#else
		return vector4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, math_type::zero);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector4& cross(const vector4& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS

#ifndef __Y_Z_X
// 3, 0, 2, 1
#define __Y_Z_X 1, 2, 0, 3
#endif // __Y_Z_X

#ifndef __Z_X_Y
// 3, 1, 0, 2
#define __Z_X_Y 2, 0, 1, 3
#endif // __Z_X_Y

		simd_type syzx = simd::template permute<__Y_Z_X>(mm);
		simd_type szxy = simd::template permute<__Z_X_Y>(mm);
		simd_type tzxy = simd::template permute<__Z_X_Y>(v.mm);
		simd_type tyzx = simd::template permute<__Y_Z_X>(v.mm);
		simd_type m1 = simd::mul(syzx, tzxy);
		simd_type m2 = simd::mul(szxy, tyzx);
		result.mm = simd::select1110(simd::sub(m1, m2));

#undef __Z_X_Y
#undef __Y_Z_X

#else
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		result.w = math_type::zero;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	/*---------------------------------------------------------------------
	* 正規化
	*---------------------------------------------------------------------*/
	vector4& normalize()
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::normalize(mm);
#else
		T len = length_sq();
		// ゼロ割対策
		if (len > math_type::zero)
		{
			// 長さの逆数
			len = math_type::rsqrt(len);
			x *= len;
			y *= len;
			z *= len;
			w *= len;
		}
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4& normalize(vector4& result) const
	{
		result = *this;
		return result.normalize();
	}
	/*---------------------------------------------------------------------
	* 正規化を求める
	*---------------------------------------------------------------------*/
	vector4 normalized() const
	{
		vector4 v = *this;
		return v.normalize();
	}
	/*---------------------------------------------------------------------
	* 射影を求める
	*---------------------------------------------------------------------*/
	vector4 projection(const vector4& v) const
	{
		return *this * v.dot(*this);
	}
	vector4& projection(const vector4& v, vector4& result) const
	{
		result = *this;
		return result *= v.dot(*this);
	}
	/*---------------------------------------------------------------------
	* 線形補間
	*---------------------------------------------------------------------*/
	vector4 lerp(const vector4& to, T t) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::lerp(mm, to.mm, t));
#else
		return vector4(
			math_type::lerp(x, to.x, t),
			math_type::lerp(y, to.y, t),
			math_type::lerp(z, to.z, t),
			math_type::lerp(w, to.w, t)
		);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector4& lerp(const vector4& to, T t, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::lerp(mm, to.mm, t);
#else
		result.x = math_type::lerp(x, to.x, t);
		result.y = math_type::lerp(y, to.y, t);
		result.z = math_type::lerp(z, to.z, t);
		result.w = math_type::lerp(w, to.w, t);
#endif
		return result;
	}
	vector4& lerp(const vector4& from, const vector4& to, T t)
	{
		return from.lerp(to, t, *this);
	}
	/*---------------------------------------------------------------------
	* 値を0～1にクランプ
	*---------------------------------------------------------------------*/
	vector4& saturate()
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::clamp01(mm);
#else
		x = math_type::clamp01(x);
		y = math_type::clamp01(y);
		z = math_type::clamp01(z);
		w = math_type::clamp01(w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4& saturate(vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::clamp01(mm);
#else
		result.x = math_type::clamp01(x);
		result.y = math_type::clamp01(y);
		result.z = math_type::clamp01(z);
		result.w = math_type::clamp01(w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	vector4 saturated() const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::clamp01(mm));
#else
		return vector4(math_type::clamp01(x),
			math_type::clamp01(y),
			math_type::clamp01(z),
			math_type::clamp01(w));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 最大値にする
	*---------------------------------------------------------------------*/
	vector4& maximize(const vector4& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::max(mm, v.mm);
#else
		x = math_type::max(x, v.x);
		y = math_type::max(y, v.y);
		z = math_type::max(z, v.z);
		w = math_type::max(w, v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4& maximize(const vector4& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::max(mm, v.mm);
#else
		result.x = math_type::max(x, v.x);
		result.y = math_type::max(y, v.y);
		result.z = math_type::max(z, v.z);
		result.w = math_type::max(w, v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	vector4 maximized(const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::max(mm, v.mm));
#else
		return vector4(math_type::max(x, v.x), math_type::max(y, v.y), math_type::max(z, v.z), math_type::max(w, v.w));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 最小値にする
	*---------------------------------------------------------------------*/
	vector4& minimize(const vector4& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::min(mm, v.mm);
#else
		x = math_type::min(x, v.x);
		y = math_type::min(y, v.y);
		z = math_type::min(z, v.z);
		w = math_type::min(w, v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4& minimize(const vector4& v, vector4& result) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		result.mm = simd::min(mm, v.mm);
#else
		result.x = math_type::min(x, v.x);
		result.y = math_type::min(y, v.y);
		result.z = math_type::min(z, v.z);
		result.w = math_type::min(w, v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	vector4 minimized(const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::min(mm, v.mm));
#else
		return vector4(math_type::min(x, v.x), math_type::min(y, v.y), math_type::min(z, v.z), math_type::min(w, v.w));
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* x軸回転角度を求める
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* y軸回転角度を求める
	*---------------------------------------------------------------------*/
	T yaw() const
	{
		if (length_sq() > math_type::zero)
		{
			// atan2は正規化されていなくてもいい
			return math_type::atan2(x, z);
		}
		return math_type::zero;
	}
	/*---------------------------------------------------------------------
	* 座標変換
	*---------------------------------------------------------------------*/
	vector4& transform(const matrix4x4<T>&); // matrix4x4.h
	vector4& transform(const matrix4x4<T>&, vector4& result) const;
	vector4 transformed(const matrix4x4<T>&) const;
	vector4& transform_coord(const matrix4x4<T>&);
	vector4& transform_coord(const matrix4x4<T>&, vector4& result) const;
	vector4 transformed_coord(const matrix4x4<T>&) const;
	/*---------------------------------------------------------------------
	* 回転
	*---------------------------------------------------------------------*/
	vector4& rotate(const quaternion<T>&);
	vector4& rotate(const quaternion<T>&, vector4<T>& result) const;
	vector4 rotated(const quaternion<T>&) const;
	vector4 rotated(const vector3<T>& axis, T angle) const
	{
		vector4 result(behavior::noinitialize);
		return rotated(axis, angle, result);
	}
	vector4& rotate(const vector3<T>& axis, T angle, vector4& result) const
	{
		reinterpret_cast<const vector3<T>&>(*this).rotate(axis, angle, reinterpret_cast<vector3<T>&>(result));
		result.w = w;

		return result;
	}
	vector4& rotate(const vector3<T>& axis, T angle)
	{
		const vector4 c = *this;
		return c.rotated(axis, angle, *this);
	}

	/*---------------------------------------------------------------------
	* 並べ替え
	*---------------------------------------------------------------------*/
	vector2<T> swizzle(int x, int y) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 3);
		return vector2<T>((*this)[x], (*this)[y]);
	}
	vector3<T> swizzle(int x, int y, int z) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(z, 0, 3);
		return vector3<T>((*this)[x], (*this)[y], (*this)[z]);
	}
	vector4 swizzle(int x, int y, int z, int w) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(z, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(w, 0, 3);
		return vector4((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
	}

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	POCKET_CXX11_EXPLICIT operator vector4<U>() const
	{
		return vector4<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
	}
	POCKET_CXX11_EXPLICIT operator vector3<T>() const
	{
		return vector3<T>(x, y, z);
	}
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

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::equal(mm, v.mm);
#else
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool operator != (const vector4& v) const
	{
		return !(*this == v);
	}
	bool operator < (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::less(mm, v.mm);
#else
		return x < v.x && y < v.y && z < v.z && w < v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool operator <= (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::less_equal(mm, v.mm);
#else
		return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool operator > (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::greater(mm, v.mm);
#else
		return x > v.x && y > v.y && z > v.z && w > v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	bool operator >= (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return simd::greater_equal(mm, v.mm);
#else
		return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	vector4 operator + () const
	{
		return *this;
	}
	vector4 operator - () const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::negate(mm));
#else
		return vector4(-x, -y, -z, -w);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector4& operator ++ ()
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::add(mm, simd::one());
#else
		++x;
		++y;
		++z;
		++w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4 operator ++ (int)
	{
		const vector4 r = *this;
		++(*this);
		return r;
	}
	vector4& operator -- ()
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::sub(mm, simd::one());
#else
		--x;
		--y;
		--z;
		--w;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4 operator -- (int)
	{
		const vector4 r = *this;
		--(*this);
		return r;
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	vector4 operator + (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::add(mm, v.mm));
#else
		vector4 result(behavior::noinitialize);
		return add(v, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4 operator + (const vector4<U>& v) const
	{
		vector4 result(behavior::noinitialize);
		return add<U>(v, result);
	}
	vector4 operator - (const vector4& v) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::sub(mm, v.mm));
#else
		vector4 result(behavior::noinitialize);
		return subtract(v, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4 operator - (const vector4<U>& v) const
	{
		vector4 result(behavior::noinitialize);
		return subtract<U>(v, result);
	}
	vector4 operator * (const matrix4x4<T>&) const; // matrix4x4.h
	vector4 operator * (const quaternion<T>&) const; // quaternion.h
	vector4 operator * (T f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::mul(mm, f));
#else
		vector4 result(behavior::noinitialize);
		return multiply(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4 operator * (U f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::mul(mm, simd::set(static_cast<T>(f))));
#else
		vector4 result(behavior::noinitialize);
		return multiply<U>(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector4 operator / (T f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::div(mm, f));
#else
		vector4 result(behavior::noinitialize);
		return divide(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4 operator / (U f) const
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		return vector4(simd::div(mm, static_cast<T>(f)));
#else
		vector4 result(behavior::noinitialize);
		return divide<U>(f, result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}
	vector4 operator % (T f) const
	{
		return vector4(math_type::remainder(x, f), math_type::remainder(y, f), math_type::remainder(z, f), math_type::remainder(w, f));
	}
	vector4 operator % (const vector4& v) const
	{
		return vector4(math_type::remainder(x, v.x), math_type::remainder(y, v.y), math_type::remainder(z, v.z), math_type::remainder(w, v.w));
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	vector4& operator = (T f)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(f);
#else
		x = y = z = w = f;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& operator = (U f)
	{
		*this = static_cast<T>(f);
		return *this;
	}
	vector4& operator = (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector4& operator = (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector4& operator = (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector4& operator = (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}
	vector4& operator = (const vector2<T>& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(v.x, v.y, z, w);
#else
		x = v.x;
		y = v.y;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	template <typename U>
	vector4& operator = (const vector2<U>& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(static_cast<T>(v.x), static_cast<T>(v.y), z, w);
#else
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	vector4& operator = (const vector3<T>& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(v.x, v.y, v.z, w);
#else
		x = v.x;
		y = v.y;
		z = v.z;
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	template <typename U>
	vector4& operator = (const vector3<U>& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), w);
#else
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	template <typename U>
	vector4& operator = (const vector4<U>& v)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		mm = simd::set(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), static_cast<T>(v.w));
#else
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		w = static_cast<T>(v.w);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	vector4& operator += (const vector4& v)
	{
		return add(v, *this);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& operator += (const vector4<U>& v)
	{
		return add<U>(v, *this);
	}
	vector4& operator += (const vector3<T>& v)
	{
		return add(v, *this);
	}
	vector4& operator -= (const vector4& v)
	{
		return subtract(v, *this);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& operator -= (const vector4<U>& v)
	{
		return subtract<U>(v, *this);
	}
	vector4& operator -= (const vector3<T>& v)
	{
		return subtract(v, *this);
	}
	vector4& operator *= (T f)
	{
		return multiply(f, *this);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& operator *= (U f)
	{
		return multiply<U>(f, *this);
	}
	vector4& operator *= (const matrix4x4<T>& m)
	{
		return transform(m);
	}
	vector4& operator *= (const quaternion<T>& q)
	{
		return rotate(q);
	}
	vector4& operator /= (T f)
	{
		return divide(f, *this);
	}
	template <typename U, POCKET_TEMPLATE_TYPE_VALIDATE_ARITHMETIC(U)>
	vector4& operator /= (U f)
	{
		return divide(f, *this);
	}
	vector4& operator %= (T f)
	{
		math_type::remainder_assign(x, f);
		math_type::remainder_assign(y, f);
		math_type::remainder_assign(z, f);
		math_type::remainder_assign(w, f);
		return *this;
	}
	vector4& operator %= (const vector4& v)
	{
		math_type::remainder_assign(x, v.x);
		math_type::remainder_assign(y, v.y);
		math_type::remainder_assign(z, v.z);
		math_type::remainder_assign(w, v.w);
		return *this;
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	vector4& operator () (const behavior::_zero_t&)
	{
		return operator=(math_type::zero);
	}
	vector4& operator () (const behavior::_one_t&)
	{
		return operator=(math_type::one);
	}
	vector4& operator () (const behavior::_half_t&)
	{
		return operator=(math_type::half);
	}
	vector4& operator () (const behavior::_half_of_half_t&)
	{
		return operator=(math_type::half_of_half);
	}

	vector4 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	vector4 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	vector4 operator () (const behavior::_add_t&, const vector4& v) const
	{
		return operator+(v);
	}
	vector4 operator () (const behavior::_sub_t&, const vector4& v) const
	{
		return operator-(v);
	}
	vector4 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	vector4 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}
	vector4 operator () (const behavior::_rem_t&, T f) const
	{
		return operator%(f);
	}
	vector4 operator () (const behavior::_rem_t&, const vector4& v) const
	{
		return operator%(v);
	}

	vector4& operator () (const behavior::_add_assign_t&, const vector4& v)
	{
		return operator+=(v);
	}
	vector4& operator () (const behavior::_sub_assign_t&, const vector4& v)
	{
		return operator-=(v);
	}
	vector4& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	vector4& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	vector4& operator () (const behavior::_rem_assign_t&, T f)
	{
		return operator%=(f);
	}
	vector4& operator () (const behavior::_rem_assign_t&, const vector4& v)
	{
		return operator%=(v);
	}
	vector4& operator () (const behavior::_increment_t&)
	{
		return operator++();
	}
	vector4 operator () (const behavior::_increment_back_t&)
	{
		return operator++(0);
	}
	vector4& operator () (const behavior::_decrement_t&)
	{
		return operator--();
	}
	vector4 operator () (const behavior::_decrement_back_t&)
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

	bool operator () (const behavior::_equal_t&, const vector4& v) const
	{
		return operator==(v);
	}
	bool operator () (const behavior::_not_equal_t&, const vector4& v) const
	{
		return operator!=(v);
	}
	bool operator () (const behavior::_near_t&, const vector4& v) const
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
	T operator () (const behavior::_dot_t&, const vector4& v) const
	{
		return dot(v);
	}
	vector4& operator () (const behavior::_normalize_t&)
	{
		return normalize();
	}
	vector4 operator () (const behavior::_normalized_t&) const
	{
		return normalized();
	}
	vector4 operator () (const behavior::_cross_t&, const vector4& v) const
	{
		return cross(v);
	}
	vector4 operator () (const behavior::_lerp_t&, const vector4& to, T t) const
	{
		return lerp(to, t);
	}
	T operator () (const behavior::_distance_t&, const vector4& v) const
	{
		return distance(v);
	}
	vector4 operator () (const behavior::_direction_t&, const vector4& v) const
	{
		return direction(v);
	}
	vector4 operator () (const behavior::_direction_t&, const vector4& v, T force) const
	{
		return direction(v) * force;
	}
	vector4& operator () (const behavior::_saturate_t&) const
	{
		return saturate();
	}
	vector4 operator () (const behavior::_saturated_t&) const
	{
		return saturated();
	}
	vector2<T> operator () (const behavior::_swizzle_t&, int x, int y) const
	{
		return swizzle(x, y);
	}
	vector3<T> operator () (const behavior::_swizzle_t&, int x, int y, int z) const
	{
		return swizzle(x, y, z);
	}
	vector4 operator () (const behavior::_swizzle_t&, int x, int y, int z, int w) const
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
	vector4& operator () (const behavior::_from_pitch_yaw_t&, T pitch, T yaw)
	{
		return from_pitch_yaw(pitch, yaw);
	}
	vector4 operator () (const behavior::_transform_t&, const matrix4x4<T>& m) const
	{
		return transformed(m);
	}
	vector4 operator () (const behavior::_transform_coord_t&, const matrix4x4<T>& m) const
	{
		return transformed_coord(m);
	}
	vector4 operator () (const behavior::_rotate_t&, const quaternion<T>& q) const
	{
		return rotated(q);
	}
	vector4 operator () (const behavior::_rotate_t&, const vector3<T>& axis, T angle) const
	{
		return rotated(axis, angle);
	}
};

template <typename T>
const vector4<T> vector4<T>::zero(math_type::zero, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::one(math_type::one, math_type::one, math_type::one, math_type::one);
template <typename T>
const vector4<T> vector4<T>::unit_x(math_type::one, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::unit_y(math_type::zero, math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::unit_z(math_type::zero, math_type::zero, math_type::one, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::unit_w(math_type::zero, math_type::zero, math_type::zero, math_type::one);
template <typename T>
const vector4<T> vector4<T>::up(math_type::zero, math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::down(math_type::zero, -math_type::one, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::right(math_type::one, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::left(-math_type::one, math_type::zero, math_type::zero, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::forward(math_type::zero, math_type::zero, math_type::one, math_type::zero);
template <typename T>
const vector4<T> vector4<T>::backward(math_type::zero, math_type::zero, -math_type::one, math_type::zero);

// 左辺が数値の場合の演算子

template <typename T> inline
vector4<T> operator * (T f, const vector4<T>& v)
{
	return v * f;
}

// vector2がvector4からの値を取得するコンストラクタ

template <typename T> inline
vector2<T>::vector2(const vector4<T>& v) :
	x(v.x), y(v.y)
{

}
template <typename T>
template <typename U> inline
vector2<T>::vector2(const vector4<U>& v) :
	x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
{

}

template <typename T> inline
vector4<T> vector2<T>::swizzle(int x, int y, int z, int w) const
{
	POCKET_DEBUG_ASSERT(x < 2 && y < 2 && z < 2 && w < 2);
	return vector4<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
}

template <typename T> inline
vector2<T>& vector2<T>::operator = (const vector4<T>& v)
{
#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef simd_traits<T> simd;
	simd::store2(&x, &y, v.mm);
#else
	x = v.x;
	y = v.y;
#endif // POCKET_USE_SIMD_ANONYMOUS
	return *this;
}
template <typename T>
template <typename U> inline
vector2<T>& vector2<T>::operator = (const vector4<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	return *this;
}

// vector3がvector4から値を取得するコンストラクタ

template <typename T> inline
vector3<T>::vector3(const vector4<T>& v) :
#ifndef POCKET_USE_ANONYMOUS_NON_POD
	x(v.x), y(v.y),
#else
	xy(v.x, v.y),
#endif
	z(v.z)
{

}
template <typename T>
template <typename U> inline
vector3<T>::vector3(const vector4<U>& v) :
#ifndef POCKET_USE_ANONYMOUS_NON_POD
	x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
#else
	xy(static_cast<T>(v.x), static_cast<T>(v.y)),
#endif
	z(static_cast<T>(v.z))
{

}

template <typename T> inline
vector4<T> vector3<T>::swizzle(int x, int y, int z, int w) const
{
	POCKET_DEBUG_ASSERT(x < 3 && y < 3 && z < 3 && w < 3);
	return vector4<T>((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
}

template <typename T> inline
vector3<T>& vector3<T>::operator = (const vector4<T>& v)
{
#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef simd_traits<T> simd;
	simd::store3(&x, &y, &z, v.mm);
#else
	x = v.x;
	y = v.y;
	z = v.z;
#endif // POCKET_USE_SIMD_ANONYMOUS
	return *this;
}
template <typename T>
template <typename U> inline
vector3<T>& vector3<T>::operator = (const vector4<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	z = static_cast<T>(v.z);
	return *this;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vector4<T>& v)
{
	// (x, y, z, w)
	os << io::parentheses_left << v.x << io::comma_space
		<< v.y << io::comma_space
		<< v.z << io::comma_space
		<< v.w << io::parentheses_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, vector4<T>& v)
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

#endif // __POCKET_MATH_VECTOR4_H__
