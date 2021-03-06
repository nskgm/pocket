﻿#ifndef __POCKET_MATH_MATRIX4X4_H__
#define __POCKET_MATH_MATRIX4X4_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../debug.h"
#include "../call.h"
#include "math_traits.h"
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include "matrix3x3.h"
#ifdef POCKET_USE_SIMD_ANONYMOUS
#include "simd_traits.h"
#endif // POCKET_USE_SIMD_ANONYMOUS
#include "../io.h"
#ifdef POCKET_USE_CXX11
#include <initializer_list>
#endif // POCKET_USE_CXX11

namespace pocket
{
namespace math
{

template <typename> struct matrix4x4;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef matrix4x4<float> matrix4x4f;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef matrix4x4<double> matrix4x4d;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef matrix4x4<long double> matrix4x4ld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
struct matrix4x4
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//-----------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------

	typedef math_traits<T> math_type;
	typedef vector4<T> vector4_type;
	typedef vector4<T> row_type;
	typedef vector4<T> column_type;
	typedef typename math_type::sin_cos_type sin_cos_type;

#ifdef POCKET_USE_SIMD_ANONYMOUS
	typedef typename vector4<T>::simd simd;
	typedef typename vector4<T>::simd_type simd_type;
#endif // POCKET_USE_SIMD_ANONYMOUS

	typedef container::array<vector4<T>, 4> array4x4_type;
	typedef typename array4x4_type::value_type value_type;
	typedef typename array4x4_type::iterator iterator;
	typedef typename array4x4_type::const_iterator const_iterator;
	typedef typename array4x4_type::pointer pointer;
	typedef typename array4x4_type::const_pointer const_pointer;
	typedef typename array4x4_type::reference reference;
	typedef typename array4x4_type::const_reference const_reference;

	typedef container::array<T, 16> array16_type;
	typedef typename array16_type::value_type value_type_16;
	typedef typename array16_type::iterator iterator_16;
	typedef typename array16_type::const_iterator const_iterator_16;
	typedef typename array16_type::pointer pointer_16;
	typedef typename array16_type::const_pointer const_pointer_16;
	typedef typename array16_type::reference reference_16;
	typedef typename array16_type::const_reference const_reference_16;

	typedef vector4<T>(*each_calc_function)(const vector4<T>&, const vector4<T>&);

	//-----------------------------------------------------------------------------------------
	// Members
	//-----------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS_NON_POD

			array4x4_type M;

#ifdef POCKET_USE_ANONYMOUS_NON_POD
		};
		struct
		{
			row_type mv0;
			row_type mv1;
			row_type mv2;
			row_type mv3;
		};
		struct
		{
			vector3<T> _right; // x方向ベクトル
			T _right_w; // x方向ベクトルw
			vector3<T> _up; // y方向ベクトル
			T _up_w; // y方向ベクトルw
			vector3<T> _forward; // z方向ベクトル
			T _forward_w; // z方向ベクトルw
			vector3<T> _position; // 座標ベクトル
			T _position_w; // 座標ベクトルw
		};
		struct
		{
			T m11; T m12; T m13; T m14;
			T m21; T m22; T m23; T m24;
			T m31; T m32; T m33; T m34;
			T m41; T m42; T m43; T m44;
		};
		array16_type data;
	};
#endif // POCKET_USE_ANONYMOUS_NON_POD

	template <typename> friend struct matrix4x4;

	//-----------------------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------------------

	static const matrix4x4 zero; // 0.0 x 16
	static const matrix4x4 identity; // 単位行列

	//-----------------------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(matrix4x4);
	explicit matrix4x4(const call::noinitialize_t&)
	{}
	explicit matrix4x4(const call::zero_t&)
	{
		M[0] = row_type::zero;
		M[1] = row_type::zero;
		M[2] = row_type::zero;
		M[3] = row_type::zero;
	}
	explicit matrix4x4(const call::identity_t&)
	{
		M[0] = row_type::unit_x;
		M[1] = row_type::unit_y;
		M[2] = row_type::unit_z;
		M[3] = row_type::unit_w;
	}
	explicit matrix4x4(T t)
	{
		M[0] = row_type(t, math_type::zero, math_type::zero, math_type::zero);
		M[1] = row_type(math_type::zero, t, math_type::zero, math_type::zero);
		M[2] = row_type(math_type::zero, math_type::zero, t, math_type::zero);
		M[3] = row_type(math_type::zero, math_type::zero, math_type::zero, t);
	}
	matrix4x4(T M11, T M12, T M13, T M14,
		T M21, T M22, T M23, T M24,
		T M31, T M32, T M33, T M34,
		T M41, T M42, T M43, T M44)
	{
		M[0] = row_type(M11, M12, M13, M14);
		M[1] = row_type(M21, M22, M23, M24);
		M[2] = row_type(M31, M32, M33, M34);
		M[3] = row_type(M41, M42, M43, M44);
	}
	matrix4x4(const vector4<T>& M1, const vector4<T>& M2, const vector4<T>& M3, const vector4<T>& M4)
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		: mv0(M1), mv1(M2), mv2(M3), mv3(M4)
#endif // POCKET_USE_ANONYMOUS_NON_POD
	{
#ifndef POCKET_USE_ANONYMOUS_NON_POD
		M[0] = M1;
		M[1] = M2;
		M[2] = M3;
		M[3] = M4;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	explicit matrix4x4(const vector3<T>& M1, T M1W,
		const vector3<T>& M2, T M2W,
		const vector3<T>& M3, T M3W,
		const vector3<T>& M4, T M4W = math_type::one)
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		: mv0(M1, M1W), mv1(M2, M2W), mv2(M3, M3W), mv3(M4, M4W)
#endif // POCKET_USE_ANONYMOUS_NON_POD
	{
#ifndef POCKET_USE_ANONYMOUS_NON_POD
		M[0] = row_type(M1, M1W);
		M[1] = row_type(M2, M2W);
		M[2] = row_type(M3, M3W);
		M[3] = row_type(M4, M4W);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	explicit matrix4x4(const matrix3x3<T>& m)
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		: mv0(m[0], math_type::zero),
		mv1(m[1], math_type::zero),
		mv2(m[2], math_type::zero),
		mv3(row_type::unit_w)
#endif // POCKET_USE_ANONYMOUS_NON_POD
	{
#ifndef POCKET_USE_ANONYMOUS_NON_POD
		M[0] = row_type(m[0], math_type::zero);
		M[1] = row_type(m[1], math_type::zero);
		M[2] = row_type(m[2], math_type::zero);
		M[3] = row_type::unit_w;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}

	//-----------------------------------------------------------------------------------------
	// Functions
	//-----------------------------------------------------------------------------------------

	template <typename FUNC>
	matrix4x4& each_calc_line(const matrix4x4& m, matrix4x4& result, FUNC func) const
	{
		iterator ri = result.M.begin();
		const_iterator oi = m.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi, ++ri)
		{
			*ri = func(*i, *oi);
		}
		return result;
	}
	//---------------------------------------------------------------------
	// 足し算
	//---------------------------------------------------------------------
	matrix4x4& add(const matrix4x4& m, matrix4x4& result) const
	{
		iterator ri = result.M.begin();
		const_iterator oi = m.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi, ++ri)
		{
			i->add(*oi, *ri);
		}
		return result;
	}
	//---------------------------------------------------------------------
	// 引き算
	//---------------------------------------------------------------------
	matrix4x4& subtract(const matrix4x4& m, matrix4x4& result) const
	{
		iterator ri = result.M.begin();
		const_iterator oi = m.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi, ++ri)
		{
			i->subtract(*oi, *ri);
		}
		return result;
	}
	//---------------------------------------------------------------------
	// 掛け算
	//---------------------------------------------------------------------
	matrix4x4& multiply(const matrix4x4& m, matrix4x4& result) const
	{
#if 0
		// ゼロ埋め
		result.load_zero();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				// ４つの要素を掛け合わせるのでもう一度４回ループ
				for (int k = 0; k < 4; ++k)
				{
					result.M[i][j] += M[i][k] * m.M[k][j];
				}
			}
		}
#endif

#ifdef POCKET_USE_SIMD_ANONYMOUS
		// ------------------------------
		//    1  2  3  4   ->
		//   ____________   ____________   ____________
		// 1 |0, 0, 0, 0|   |0, 0, 0, 0|   |0, 1, 2, 3|
		// 2 |1, 1, 1, 1| = |1, 1, 1, 1| * |0, 1, 2, 3|
		// 3 |2, 2, 2, 2|   |2, 2, 2, 2|   |0, 1, 2, 3|
		// 4 |3, 3, 3, 3|   |3, 3, 3, 3|   |0, 1, 2, 3|
		// ------------------------------

		iterator ri = result.M.begin();
		const_pointer mi = &m.M[0];
		const_pointer j;
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			simd_type mx = simd::permute_x(i->mm);
			simd_type my = simd::permute_y(i->mm);
			simd_type mz = simd::permute_z(i->mm);
			simd_type mw = simd::permute_w(i->mm);

			j = mi;
			mx = simd::mul(mx, j->mm);
			++j;
			my = simd::mul(my, j->mm);
			++j;
			mz = simd::mul(mz, j->mm);
			++j;
			mw = simd::mul(mw, j->mm);

			mx = simd::add(mx, mz);
			my = simd::add(my, mw);

			ri->mm = simd::add(mx, my);
		}
#else // POCKET_USE_SIMD_ANONYMOUS

#	if defined(POCKET_DEBUG)
		// コピー&転置をして計算しやすいようにする
		matrix4x4 t = m.transposed();
		iterator ri = result.M.begin();
		const_iterator ti = t.M.begin(), j;
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			j = ti;
			ri->x = i->dot(*j);
			++j;
			ri->y = i->dot(*j);
			++j;
			ri->z = i->dot(*j);
			++j;
			ri->w = i->dot(*j);
		}
#	else
		const_pointer oi0 = &m.M[0];
		const_pointer oi1 = &m.M[1];
		const_pointer oi2 = &m.M[2];
		const_pointer oi3 = &m.M[3];
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->x = i->x * oi0->x + i->y * oi1->x + i->z * oi2->x + i->w * oi3->x;
			ri->y = i->x * oi0->y + i->y * oi1->y + i->z * oi2->y + i->w * oi3->y;
			ri->z = i->x * oi0->z + i->y * oi1->z + i->z * oi2->z + i->w * oi3->z;
			ri->w = i->x * oi0->w + i->y * oi1->w + i->z * oi2->w + i->w * oi3->w;
		}
#	endif
#endif // POCKET_USE_SIMD_ANONYMOUS
		return result;
	}
	matrix4x4& multiply(T s, matrix4x4& result) const
	{
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			i->multiply(s, *ri);
		}
		return result;
	}
	//---------------------------------------------------------------------
	// 割り算
	//---------------------------------------------------------------------
	matrix4x4& divide(T s, matrix4x4& result) const
	{
		POCKET_DEBUG_ASSERT(s != math_type::zero);
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type rcp = simd::set(math_type::reciprocal(s));
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->mm = simd::mul(i->mm, rcp);
		}
		return result;
#else
		return multiply(math_type::reciprocal(s), result);
#endif // POCKET_USE_SIMD_ANONYMOUS
	}

	//---------------------------------------------------------------------
	// 値か近いか
	//---------------------------------------------------------------------
	bool near_equal(const matrix4x4& m) const
	{
		return M[0].near_equal(m.M[0]) &&
			M[1].near_equal(m.M[1]) &&
			M[2].near_equal(m.M[2]) &&
			M[3].near_equal(m.M[3]);
	}
	//---------------------------------------------------------------------
	// 値がゼロに近いか
	//---------------------------------------------------------------------
	bool near_equal_zero() const
	{
		return M[0].near_equal_zero() &&
			M[1].near_equal_zero() &&
			M[2].near_equal_zero() &&
			M[3].near_equal_zero();
	}
	//---------------------------------------------------------------------
	// 値がすべてゼロか
	//---------------------------------------------------------------------
	bool equal_zero() const
	{
		return M[0].equal_zero() &&
			M[1].equal_zero() &&
			M[2].equal_zero() &&
			M[3].equal_zero();
	}

	//---------------------------------------------------------------------
	// 値が単位行列と近いか
	//---------------------------------------------------------------------
	bool near_identity() const
	{
		return M[0].near_equal(row_type::unit_x) &&
			M[1].near_equal(row_type::unit_y) &&
			M[2].near_equal(row_type::unit_z) &&
			M[3].near_equal(row_type::unit_w);
	}

	//---------------------------------------------------------------------
	// 右ベクトル取得・設定
	//---------------------------------------------------------------------
	vector3<T>& right()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return _right;
#else
		return reinterpret_cast<vector3<T>&>(M[0]);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& right() const
	{
		return right();
	}
	matrix4x4& right(const vector3<T>& v)
	{
		M[0] = v;
		return *this;
	}
	//---------------------------------------------------------------------
	// 上ベクトル取得・設定
	//---------------------------------------------------------------------
	vector3<T>& up()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return _up;
#else
		return reinterpret_cast<vector3<T>&>(M[1]);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& up() const
	{
		return up();
	}
	matrix4x4& up(const vector3<T>& v)
	{
		M[1] = v;
		return *this;
	}
	//---------------------------------------------------------------------
	// 前ベクトル取得・設定
	//---------------------------------------------------------------------
	vector3<T>& forward()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return _forward;
#else
		return reinterpret_cast<vector3<T>&>(M[2]);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& forward() const
	{
		return forward();
	}
	matrix4x4& forward(const vector3<T>& v)
	{
		M[2] = v;
		return *this;
	}
	//---------------------------------------------------------------------
	// 座標取得・設定
	//---------------------------------------------------------------------
	vector3<T>& position()
	{
		// vector3<T>(m.x, m.y, m.z);
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return _position;
#else
		return reinterpret_cast<vector3<T>&>(M[3]);
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& position() const
	{
		return position();
	}
	matrix4x4& position(const vector3<T>& v)
	{
		M[3] = v;
		return *this;
	}

	//---------------------------------------------------------------------
	// 拡大縮小取得
	//---------------------------------------------------------------------
	vector3<T> scale() const
	{
		return vector3<T>(right().length(), up().length(), forward().length());
	}
	vector3<T>& scale(vector3<T>& result) const
	{
		result.x = right().length();
		result.y = up().length();
		result.z = forward().length();
		return result;
	}

	//---------------------------------------------------------------------
	// x軸回転量を求める
	//---------------------------------------------------------------------
	T pitch() const
	{
		// 前ベクトルのy軸の状態から計算する
		return forward().pitch();
	}
	//---------------------------------------------------------------------
	// y軸回転量を求める
	//---------------------------------------------------------------------
	T yaw() const
	{
		return forward().yaw();
	}
	//---------------------------------------------------------------------
	// z軸回転量を求める
	//---------------------------------------------------------------------
	T roll() const
	{
		return math_type::atan2(M[0].y, M[1].y);
	}

	//---------------------------------------------------------------------
	// すべての要素をゼロにする
	//---------------------------------------------------------------------
	matrix4x4& load_zero()
	{
		*this = matrix4x4::zero;
		return *this;
	}

	//---------------------------------------------------------------------
	// 単位行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_identity()
	{
		*this = matrix4x4::identity;
		return *this;
	}
	//---------------------------------------------------------------------
	// 拡大縮小行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_scale(T x, T y, T z)
	{
		load_identity();
		// 11から斜めの方向へ三つの要素が拡大縮小を扱う要素
		M[0].x = x;
		M[1].y = y;
		M[2].z = z;
		return *this;
	}
	matrix4x4& load_scale(const vector3<T>& v)
	{
		return load_scale(v.x, v.y, v.z);
	}
	matrix4x4& load_scale(T s)
	{
		return load_scale(s, s, s);
	}

	//---------------------------------------------------------------------
	// x軸回転行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_rotate_x(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type::unit_x;
		M[1] = row_type(math_type::zero, a.cos, a.sin, math_type::zero);
		M[2] = row_type(math_type::zero, -a.sin, a.cos, math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	//---------------------------------------------------------------------
	// y軸回転行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_rotate_y(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, math_type::zero, -a.sin, math_type::zero);
		M[1] = row_type::unit_y;
		M[2] = row_type(a.sin, math_type::zero, a.cos, math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	//---------------------------------------------------------------------
	// z軸回転行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_rotate_z(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, a.sin, math_type::zero, math_type::zero);
		M[1] = row_type(-a.sin, a.cos, math_type::zero, math_type::zero);
		M[2] = row_type::unit_z;
		M[3] = row_type::unit_w;
		return *this;
	}
	//---------------------------------------------------------------------
	// 回転行列にする（ZXY）
	//---------------------------------------------------------------------
	matrix4x4& load_rotate(T roll, T pitch, T yaw)
	{
		sin_cos_type r = roll;
		sin_cos_type p = pitch;
		sin_cos_type y = yaw;

		M[0] = row_type(r.cos * y.cos + r.sin * p.sin * y.sin,
			r.sin * p.cos,
			r.cos * -y.sin + r.sin * p.sin * y.cos,
			math_type::zero);
		M[1] = row_type(-r.sin * y.cos + r.cos * p.sin * y.sin,
			r.cos * p.cos,
			r.sin * y.sin + r.cos * p.sin * y.cos,
			math_type::zero);
		M[2] = row_type(p.cos * y.sin, -p.sin, p.cos * y.cos, math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	//---------------------------------------------------------------------
	// 回転行列にする（クォータニオン）
	//---------------------------------------------------------------------
	matrix4x4& load_rotate(const quaternion<T>& q)
	{
		T xx = math_type::two * math_type::sqr(q.x);
		T yy = math_type::two * math_type::sqr(q.y);
		T zz = math_type::two * math_type::sqr(q.z);
		T xy = math_type::two * q.x * q.y;
		T xz = math_type::two * q.x * q.z;
		T yz = math_type::two * q.y * q.z;
		T wx = math_type::two * q.w * q.x;
		T wy = math_type::two * q.w * q.y;
		T wz = math_type::two * q.w * q.z;

		M[0] = row_type(math_type::one - (yy + zz), xy + wz, xz - wy, math_type::zero);
		M[1] = row_type(xy - wz, math_type::one - (xx + zz), yz + wx, math_type::zero);
		M[2] = row_type(xz + wy, yz - wx, math_type::one - (xx + yy), math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	//---------------------------------------------------------------------
	// 回転行列にする（任意軸＋角度）
	//---------------------------------------------------------------------
	matrix4x4& load_rotate(const vector3<T>& axis, T angle)
	{
		// 四元数での計算を行なう
		return load_rotate(quaternion<T>(axis, angle));
	}
	//---------------------------------------------------------------------
	// 座標変換行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_translate(T x, T y, T z)
	{
		load_identity();
#ifdef POCKET_USE_SIMD_ANONYMOUS
		M[3].mm = simd::set(x, y, z, math_type::one);
#else
		M[3] = row_type(x, y, z, math_type::one);
#endif // POCKET_USE_SIMD_ANONYMOUS
		return *this;
	}
	matrix4x4& load_translate(const vector3<T>& v)
	{
		load_identity();
		M[3] = v;
		return *this;
	}
	//---------------------------------------------------------------------
	// ワールド変換行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_world(const vector3<T>& s, T roll, T pitch, T yaw, const vector3<T>& t)
	{
		// [Scale * RotateZ * RotateX * RotateY * Translate]を展開

		sin_cos_type r = roll;
		sin_cos_type p = pitch;
		sin_cos_type y = yaw;

		M[0] = row_type(s.x * r.cos * y.cos + s.x * r.sin * p.sin * y.sin,
			s.x * r.sin * p.cos,
			s.x * r.cos * -y.sin + s.x * r.sin * p.sin * y.cos,
			math_type::zero);
		M[1] = row_type(s.y * -r.sin * y.cos + s.y * r.cos * p.sin * y.sin,
			s.y * r.cos * p.cos,
			s.y * r.sin * y.sin + s.y * r.cos * p.sin * y.cos,
			math_type::zero);
		M[2] = row_type(s.z * p.cos * y.sin,
			s.z * -p.sin,
			s.z * p.cos * y.cos,
			math_type::zero);
		M[3] = row_type(t, math_type::one);
		return *this;
	}
	matrix4x4& load_world(T s, T roll, T pitch, T yaw, const vector3<T>& t)
	{
		return load_world(vector3<T>(s), roll, pitch, yaw, t);
	}
	matrix4x4& load_world(const vector3<T>& s, const quaternion<T>& q, const vector3<T>& t)
	{
		// [scale * quaternion * translate]を展開

		T xx = math_type::two * math_type::sqr(q.x);
		T yy = math_type::two * math_type::sqr(q.y);
		T zz = math_type::two * math_type::sqr(q.z);
		T xy = math_type::two * q.x * q.y;
		T xz = math_type::two * q.x * q.z;
		T yz = math_type::two * q.y * q.z;
		T wx = math_type::two * q.w * q.x;
		T wy = math_type::two * q.w * q.y;
		T wz = math_type::two * q.w * q.z;

		M[0] = row_type(s.x * (math_type::one - (yy + zz)), s.x * (xy + wz), s.x * (xz - wy), math_type::zero);
		M[1] = row_type(s.y * (xy - wz), s.y * (math_type::one - (xx + zz)), s.y * (yz + wx), math_type::zero);
		M[2] = row_type(s.z * (xz + wy), s.z * (yz - wx), s.z * (math_type::one - (xx + yy)), math_type::zero);
		M[3] = row_type(t, math_type::one);
		return *this;
	}
	matrix4x4& load_world(T s, const quaternion<T>& q, const vector3<T>& t)
	{
		return load_world(vector3<T>(s), q, t);
	}
	//---------------------------------------------------------------------
	// 射影変換行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_perspective_field_of_view(T fovy, T aspect, T n, T f)
	{
		load_identity();

		T t = math_type::reciprocal(math_type::tan(fovy * math_type::half));

		M[0].x = t / aspect;
		M[1].y = t;

		t = math_type::reciprocal(f - n);

		row_type& r2 = M[2];
		r2.z = (f + n) * t;
		r2.w = -math_type::one;

		row_type& r3 = M[3];
		r3.z = (math_type::two * f * n) * t;
		r3.w = math_type::zero;

		return *this;
	}
	matrix4x4& load_perspective_field_of_view(T fovy, T width, T height, T n, T f)
	{
		return load_perspective_field_of_view(fovy, width / height, n, f);
	}
	template <int W, int H>
	matrix4x4& load_perspective_field_of_view(T fovy, T n, T f)
	{
		POCKET_CONST_OR_CONSTEXPR T asp = static_cast<T>(W) / static_cast<T>(H);
		return load_perspective_field_of_view(fovy, asp, n, f);
	}
	matrix4x4& load_perspective_field_of_view_4_3(T fovy, T n, T f)
	{
		return load_perspective_field_of_view(fovy, static_cast<T>(1.333333333), n, f);
	}
	matrix4x4& load_perspective_field_of_view_16_9(T fovy, T n, T f)
	{
		return load_perspective_field_of_view(fovy, static_cast<T>(1.777777777), n, f);
	}

	//---------------------------------------------------------------------
	// 正射影変換行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_orthographics(T left, T right, T top, T bottom, T n, T f)
	{
		load_identity();

		T dx = math_type::reciprocal(right - left);
		T dy = math_type::reciprocal(top - bottom);
		T dz = math_type::reciprocal(f - n);

		M[0].x = math_type::two * dx;
		M[1].y = math_type::two * dy;
		M[2].z = -math_type::two * dz; // 右手特有

		row_type& r3 = M[3];
		r3.x = -(right + left) * dx;
		r3.y = -(bottom + top) * dy;
		r3.z = -(f + n) * dz;

		return *this;
	}
	matrix4x4& load_orthographics(T width, T height, T n, T f)
	{
		return load_orthographics(math_type::zero, width, math_type::zero, height, n, f);
	}
	//---------------------------------------------------------------------
	// 2D用正射影変換行列にする
	//---------------------------------------------------------------------
	matrix4x4& load_orthographics2d(T left, T right, T top, T bottom)
	{
		load_identity();

		T dx = math_type::reciprocal(right - left);
		T dy = math_type::reciprocal(top - bottom);

		M[0].x = math_type::two * dx;
		M[1].y = math_type::two * dy;
		M[2].z = -math_type::one;

		row_type& r3 = M[3];
		r3.x = -(right + left) * dx;
		r3.y = -(bottom + top) * dy;

		return *this;
	}
	matrix4x4& load_orthographics2d(T width, T height)
	{
		return load_orthographics2d(math_type::zero, width, math_type::zero, height);
	}
	//---------------------------------------------------------------------
	// 視野変換行列にする（座標＋向き）
	//---------------------------------------------------------------------
	matrix4x4& load_lookto(const vector3<T>& eye, const vector3<T>& direction, const vector3<T>& up = vector3<T>::up)
	{
		// カメラを原点としてカメラから見た変換を行う

		// 前方向と上方向の外積で右ベクトルを求める
		vector3<T> x(call::noinitialize);
		up.cross(direction, x).normalize();
		// 前と右から上方向のベクトルを求める
		vector3<T> y = direction.cross(x);

		M[0] = row_type(x.x, y.x, direction.x, math_type::zero);
		M[1] = row_type(x.y, y.y, direction.y, math_type::zero);
		M[2] = row_type(x.z, y.z, direction.z, math_type::zero);
		M[3] = row_type(-eye.dot(x), -eye.dot(y), -eye.dot(direction), math_type::one);
		return *this;
	}
	matrix4x4& load_lookto(T ex, T ey, T ez, T dx, T dy, T dz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookto(vector3<T>(ex, ey, ez), vector3<T>(dx, dy, dz), vector3<T>(ux, uy, uz));
	}

	//---------------------------------------------------------------------
	// 視野変換行列にする（座標＋注視点）
	//---------------------------------------------------------------------
	matrix4x4& load_lookat(const vector3<T>& eye, const vector3<T>& center, const vector3<T>& up = vector3<T>::up)
	{
		// 向きを求める
		vector3<T> dir(call::noinitialize);
		return load_lookto(eye, center.direction(eye, dir), up);
	}
	matrix4x4& load_lookat(T ex, T ey, T ez, T cx, T cy, T cz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookat(vector3<T>(ex, ey, ez), vector3<T>(cx, cy, cz), vector3<T>(ux, uy, uz));
	}

	//---------------------------------------------------------------------
	// 転置行列
	//---------------------------------------------------------------------
	matrix4x4& transpose()
	{
		// 回転を扱う場合の転置は逆マトリクスと同一になる

		row_type& v0 = M[0];
		row_type& v1 = M[1];
		row_type& v2 = M[2];
		row_type& v3 = M[3];

#ifdef POCKET_DEBUG
		// [0][1] = [1][0]
		T tmp = v0.y;
		v0.y = v1.x;
		v1.x = tmp;
		// [0][2] = [2][0]
		tmp = v0.z;
		v0.z = v2.x;
		v2.x = tmp;
		// [0][3] = [3][0]
		tmp = v0.w;
		v0.w = v3.x;
		v3.x = tmp;
		// [1][2] = [2][1]
		tmp = v1.z;
		v1.z = v2.y;
		v2.y = tmp;
		// [1][3] = [3][1]
		tmp = v1.w;
		v1.w = v3.y;
		v3.y = tmp;
		// [2][3] = [3][2]
		tmp = v2.w;
		v2.w = v3.z;
		v3.z = tmp;
#else
		std::swap(v0.y, v1.x);
		std::swap(v0.z, v2.x);
		std::swap(v0.w, v3.x);
		std::swap(v1.z, v2.y);
		std::swap(v1.w, v3.y);
		std::swap(v2.w, v3.z);
#endif // POCKET_DEBUG

		return *this;
	}
	matrix4x4& transpose(matrix4x4& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];
		row_type& r0 = result.M[0];
		row_type& r1 = result.M[1];
		row_type& r2 = result.M[2];
		row_type& r3 = result.M[3];

		// [0][1] = [1][0]
		r0.y = v1.x;
		r1.x = v0.y;
		// [0][2] = [2][0]
		r0.z = v2.x;
		r2.x = v0.z;
		// [0][3] = [3][0]
		r0.w = v3.x;
		r3.x = v0.w;
		// [1][2] = [2][1]
		r1.z = v2.y;
		r2.y = v1.z;
		// [1][3] = [3][1]
		r1.w = v3.y;
		r3.y = v1.w;
		// [2][3] = [3][2]
		r2.w = v3.z;
		r3.z = v2.w;

		r0.x = v0.x;
		r1.y = v1.y;
		r2.z = v2.z;
		r3.w = v3.w;

		return result;
	}
	//---------------------------------------------------------------------
	// 転置行列を求める
	//---------------------------------------------------------------------
	matrix4x4 transposed() const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];
		return matrix4x4(v0.x, v1.x, v2.x, v3.x,
			v0.y, v1.y, v2.y, v3.y,
			v0.z, v1.z, v2.z, v3.z,
			v0.w, v1.w, v2.w, v3.w);
	}
	//---------------------------------------------------------------------
	// 行列式を求める
	//---------------------------------------------------------------------
	T determinant() const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];
#if 0
		T a = v0.x * v1.y * v2.z * v3.w + v0.x * v1.z * v2.w * v3.y + v0.x * v1.w * v2.y * v3.z + v0.y * v1.x * v2.w * v3.z +
			v0.y * v1.z * v2.x * v3.w + v0.y * v1.w * v2.z * v3.x + v0.z * v1.x * v2.y * v3.w + v0.z * v1.y * v2.w * v3.x +
			v0.z * v1.w * v2.x * v3.y + v0.w * v1.x * v2.z * v3.y + v0.w * v1.y * v2.x * v3.z + v0.w * v1.z * v2.y * v3.x;
		T b = -(v0.x * v1.y * v2.w * v3.z) - v0.x * v1.z * v2.y * v3.w - v0.x * v1.w * v2.z * v3.y - v0.y * v1.x * v2.z * v3.w -
			v0.y * v1.z * v2.w * v3.x - v0.y * v1.w * v2.x * v3.z - v0.z * v1.x * v2.w * v3.y - v0.z * v1.y * v2.x * v3.w -
			v0.z * v1.w * v2.y * v3.x - v0.w * v1.x * v2.y * v3.z - v0.w * v1.y * v2.z * v3.x - v0.w * v1.z * v2.x * v3.y;
		return (a + b);
#else
		// 余因子展開で求める
		T a1 = ((v0.x * v1.y * v2.z * v3.w) + (v0.x * v1.z * v2.w * v3.y) + (v0.x * v1.w * v2.y * v3.z) -
			(v0.x * v1.w * v2.z * v3.y) - (v0.x * v1.z * v2.y * v3.w) - (v0.x * v1.y * v2.w * v3.z));
		T a2 = ((v1.x * v0.y * v2.z * v3.w) + (v1.x * v0.z * v2.w * v3.y) + (v1.x * v0.w * v2.y * v3.z) -
			(v1.x * v0.w * v2.z * v3.y) - (v1.x * v0.z * v2.y * v3.w) - (v1.x * v0.y * v2.w * v3.z));
		T a3 = ((v2.x * v0.y * v1.z * v3.w) + (v2.x * v0.z * v1.w * v3.y) + (v2.x * v0.w * v1.y * v3.z) -
			(v2.x * v0.w * v1.z * v3.y) - (v2.x * v0.z * v1.y * v3.w) - (v2.x * v0.y * v1.w * v3.z));
		T a4 = ((v3.x * v0.y * v1.z * v2.w) + (v3.x * v0.z * v1.w * v2.y) + (v3.x * v0.w * v1.y * v2.z) -
			(v3.x * v0.w * v1.z * v2.y) - (v3.x * v0.z * v1.y * v2.w) - (v3.x * v0.y * v1.w * v2.z));
		return (a1 - a2 + a3 - a4);
#endif
	}

	//---------------------------------------------------------------------
	// 逆行列にする（逆行列が存在しない場合は偽）
	//---------------------------------------------------------------------
	matrix4x4& inverse()
	{
		// 自身を構築するためのコピー(自身が変わるため)
		const matrix4x4 c = *this;
		return c.inverse(*this);
	}
	matrix4x4& inverse(matrix4x4& result) const
	{
		T det = determinant();
		// 逆行列が存在しない
		//if (det == math_type::zero)
		if (math_type::near_equal_zero(det))
		{
			// 単位行列
			return result.load_identity();
		}

		// 逆数にする
		det = math_type::reciprocal(det);

		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];
		row_type& r0 = result.M[0];
		row_type& r1 = result.M[1];
		row_type& r2 = result.M[2];
		row_type& r3 = result.M[3];

		// iが偶数の時は-から, iが奇数の時には+から

		// (-1)i+j |M ji| (M ji)のjiが逆なのに注意

		// +M-M+M-M
		r0.x = ((v1.y * v2.z * v3.w) + (v1.z * v2.w * v3.y) + (v1.w * v2.y * v3.z) -
			(v1.w * v2.z * v3.y) - (v1.z * v2.y * v3.w) - (v1.y * v2.w * v3.z)) * det;
		r0.y = ((v0.y * v2.w * v3.z) + (v0.z * v2.y * v3.w) + (v0.w * v2.z * v3.y) -
			(v0.y * v2.z * v3.w) - (v0.z * v2.w * v3.y) - (v0.w * v2.y * v3.z)) * det;
		r0.z = ((v0.y * v1.z * v3.w) + (v0.z * v1.w * v3.y) + (v0.w * v1.y * v3.z) -
			(v0.w * v1.z * v3.y) - (v0.z * v1.y * v3.w) - (v0.y * v1.w * v3.z)) * det;
		r0.w = ((v0.y * v1.w * v2.z) + (v0.z * v1.y * v2.w) + (v0.w * v1.z * v2.y) -
			(v0.y * v1.z * v2.w) - (v0.z * v1.w * v2.y) - (v0.w * v1.y * v2.z)) * det;
		// -M+M-M+M
		r1.x = ((v1.x * v2.w * v3.z) + (v1.z * v2.x * v3.w) + (v1.w * v2.z * v3.x) -
			(v1.x * v2.z * v3.w) - (v1.z * v2.w * v3.x) - (v1.w * v2.x * v3.z)) * det;
		r1.y = ((v0.x * v2.z * v3.w) + (v0.z * v2.w * v3.x) + (v0.w * v2.x * v3.z) -
			(v0.w * v2.z * v3.x) - (v0.z * v2.x * v3.w) - (v0.x * v2.w * v3.z)) * det;
		r1.z = ((v0.x * v1.w * v3.z) + (v0.z * v1.x * v3.w) + (v0.w * v1.z * v3.x) -
			(v0.x * v1.z * v3.w) - (v0.z * v1.w * v3.x) - (v0.w * v1.x * v3.z)) * det;
		r1.w = ((v0.x * v1.z * v2.w) + (v0.z * v1.w * v2.x) + (v0.w * v1.x * v2.z) -
			(v0.w * v1.z * v2.x) - (v0.z * v1.x * v2.w) - (v0.x * v1.w * v2.z)) * det;
		// +M-M+M-M
		r2.x = ((v1.x * v2.y * v3.w) + (v1.y * v2.w * v3.x) + (v1.w * v2.x * v3.y) -
			(v1.w * v2.y * v3.x) - (v1.y * v2.x * v3.w) - (v1.x * v2.w * v3.y)) * det;
		r2.y = ((v0.x * v2.w * v3.y) + (v0.y * v2.x * v3.w) + (v0.w * v2.y * v3.x) -
			(v0.w * v2.x * v3.y) - (v0.y * v2.w * v3.x) - (v0.x * v2.y * v3.w)) * det;
		r2.z = ((v0.x * v1.y * v3.w) + (v0.y * v1.w * v3.x) + (v0.w * v1.x * v3.y) -
			(v0.w * v1.y * v3.x) - (v0.y * v1.x * v3.w) - (v0.x * v1.w * v3.y)) * det;
		r2.w = ((v0.x * v1.w * v2.y) + (v0.y * v1.x * v2.w) + (v0.w * v1.y * v2.x) -
			(v0.w * v1.x * v2.y) - (v0.y * v1.w * v2.x) - (v0.x * v1.y * v2.w)) * det;
		// -M+M-M+M
		r3.x = ((v1.x * v2.z * v3.y) + (v1.y * v2.x * v3.z) + (v1.z * v2.y * v3.x) -
			(v1.z * v2.x * v3.y) - (v1.y * v2.z * v3.x) - (v1.x * v2.y * v3.z)) * det;
		r3.y = ((v0.x * v2.y * v3.z) + (v0.y * v2.z * v3.x) + (v0.z * v2.x * v3.y) -
			(v0.z * v2.y * v3.x) - (v0.y * v2.x * v3.z) - (v0.x * v2.z * v3.y)) * det;
		r3.z = ((v0.x * v1.z * v3.y) + (v0.y * v1.x * v3.z) + (v0.z * v1.y * v3.x) -
			(v0.z * v1.x * v3.y) - (v0.y * v1.z * v3.x) - (v0.x * v1.y * v3.z)) * det;
		r3.w = ((v0.x * v1.y * v2.z) + (v0.y * v1.z * v2.x) + (v0.z * v1.x * v2.y) -
			(v0.z * v1.y * v2.x) - (v0.y * v1.x * v2.z) - (v0.x * v1.z * v2.y)) * det;

		return result;
	}
	//---------------------------------------------------------------------
	// 逆行列を求める
	//---------------------------------------------------------------------
	matrix4x4 inversed() const
	{
		matrix4x4 r(call::noinitialize);
		return POCKET_CXX11_MOVE(inverse(r));
	}
	//---------------------------------------------------------------------
	// ベクトル座標変換（w=1）
	//---------------------------------------------------------------------
	vector3<T> transform(const vector3<T>& v) const
	{
		vector3<T> r(call::noinitialize);
		return POCKET_CXX11_MOVE(transform(v, r));
	}
	vector3<T>& transform(const vector3<T>& v, vector3<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];

		result.x = (v0.x * v.x + v1.x * v.y + v2.x * v.z + v3.x);
		result.y = (v0.y * v.x + v1.y * v.y + v2.y * v.z + v3.y);
		result.z = (v0.z * v.x + v1.z * v.y + v2.z * v.z + v3.z);

		return result;
	}
	//---------------------------------------------------------------------
	// ベクトル座標変換（0～1）（w=1）
	//---------------------------------------------------------------------
	vector3<T> transform_coord(const vector3<T>& v) const
	{
		vector3<T> r(call::noinitialize);
		return POCKET_CXX11_MOVE(transform_coord(v, r));
	}
	vector3<T>& transform_coord(const vector3<T>& v, vector3<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];

		T w = math_type::reciprocal(v0.w * v.x + v1.w * v.y + v2.w * v.z + v3.w);
		result.x = (v0.x * v.x + v1.x * v.y + v2.x * v.z + v3.x) * w;
		result.y = (v0.y * v.x + v1.y * v.y + v2.y * v.z + v3.y) * w;
		result.z = (v0.z * v.x + v1.z * v.y + v2.z * v.z + v3.z) * w;

		return result;
	}
	//---------------------------------------------------------------------
	// ベクトル座標変換
	//---------------------------------------------------------------------
	vector4<T> transform(const vector4<T>& v) const
	{
		vector4<T> r(call::noinitialize);
		return POCKET_CXX11_MOVE(transform(v, r));
	}
	vector4<T>& transform(const vector4<T>& v, vector4<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		const row_type& v3 = M[3];

		result.x = v0.x * v.x + v1.x * v.y + v2.x * v.z + v3.x * v.w;
		result.y = v0.y * v.x + v1.y * v.y + v2.y * v.z + v3.y * v.w;
		result.z = v0.z * v.x + v1.z * v.y + v2.z * v.z + v3.z * v.w;
		result.w = v0.w * v.x + v1.w * v.y + v2.w * v.z + v3.w * v.w;

		return result;
	}
	//---------------------------------------------------------------------
	// ベクトル座標変換（0～1）
	//---------------------------------------------------------------------
	vector4<T> transform_coord(const vector4<T>& v) const
	{
		vector4<T> r(call::noinitialize);
		return POCKET_CXX11_MOVE(transform_coord(v, r));
	}
	vector4<T>& transform_coord(const vector4<T>& v, vector4<T>& result) const
	{
		transform(v, result);

		T w = math_type::reciprocal(result.w);
		result *= w;
		return result;
	}

	//---------------------------------------------------------------------
	// ベクトル座標変換（w=0）
	//---------------------------------------------------------------------
	vector3<T> transform_normal(const vector3<T>& v) const
	{
		vector3<T> r(call::noinitialize);
		return POCKET_CXX11_MOVE(transform_normal(v, r));
	}
	vector3<T>& transform_normal(const vector3<T>& v, vector3<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];

		result.x = v0.x * v.x + v1.x * v.y + v2.x * v.z;
		result.y = v0.y * v.x + v1.y * v.y + v2.y * v.z;
		result.z = v0.z * v.x + v1.z * v.y + v2.z * v.z;

		return result;
	}
	//---------------------------------------------------------------------
	// 球面線形補間を求める
	//---------------------------------------------------------------------
	matrix4x4 slerp(const matrix4x4& to, T t) const
	{
		matrix4x4 result(call::noinitialize);
		return POCKET_CXX11_MOVE(slerp(to, t, result));
	}
	matrix4x4& slerp(const matrix4x4& to, T t, matrix4x4& result) const
	{
		vector3<T> s(call::noinitialize);
		vector3<T> p(call::noinitialize);
		quaternion<T> r(call::noinitialize);

		// 補間用の値取得
		const vector3<T> s0 = scale();
		const vector3<T> s1 = to.scale();
		// 回転取得
		const quaternion<T> q0(*this);
		const quaternion<T> q1(to);

		// 補間を行なう
		s0.lerp(s1, t, s);
		q0.slerp(q1, t, r);
		position().lerp(to.position(), t, p);

		// ワールド変換行列
		return result.load_world(s, r, p);
	}
	matrix4x4& slerp(const matrix4x4& from, const matrix4x4& to, T t)
	{
		return from.slerp(to, t, *this);
	}

	//-----------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// アクセス演算子
	//---------------------------------------------------------------------
	vector4<T>& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
		return M[i];
	}
	const vector4<T>& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
		return M[i];
	}
	T& operator [] (const matrix_point& p)
	{
		return (*this)(p.y, p.x);
	}
	const T& operator [] (const matrix_point& p) const
	{
		return (*this)(p.y, p.x);
	}
	T& operator () (int y, int x)
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 3);
		return M[y][x];
	}
	const T& operator () (int y, int x) const
	{
		POCKET_DEBUG_RANGE_ASSERT(x, 0, 3);
		POCKET_DEBUG_RANGE_ASSERT(y, 0, 3);
		return M[y][x];
	}

	//---------------------------------------------------------------------
	// 型変換演算子
	//---------------------------------------------------------------------
	template <typename U>
	POCKET_CXX11_EXPLICIT operator matrix4x4<U>() const
	{
		typedef vector4<U> other_vector4_type;
		return matrix4x4<U>(static_cast<other_vector4_type>(M[0]),
			static_cast<other_vector4_type>(M[1]),
			static_cast<other_vector4_type>(M[2]),
			static_cast<other_vector4_type>(M[3]));
	}
	POCKET_CXX11_EXPLICIT operator T* ()
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &M[0].x;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}
	POCKET_CXX11_EXPLICIT operator const T* () const
	{
#ifdef POCKET_USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &M[0].x;
#endif // POCKET_USE_ANONYMOUS_NON_POD
	}

	//---------------------------------------------------------------------
	// 比較演算子
	//---------------------------------------------------------------------
	bool operator == (const matrix4x4& v) const
	{
		const_iterator oi = v.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			if (*i != *oi)
			{
				return false;
			}
		}
		return true;
	}
	bool operator != (const matrix4x4& v) const
	{
		return !(*this == v);
	}

	//---------------------------------------------------------------------
	// 単項演算子
	//---------------------------------------------------------------------
	matrix4x4 operator + () const
	{
		return *this;
	}
	matrix4x4 operator - () const
	{
		return matrix4x4(-M[0], -M[1], -M[2], -M[3]);
	}

	//---------------------------------------------------------------------
	// 二項演算子
	//---------------------------------------------------------------------
	matrix4x4 operator + (const matrix4x4& m) const
	{
		return matrix4x4(M[0] + m.M[0], M[1] + m.M[1], M[2] + m.M[2], M[3] + m.M[3]);
	}
	matrix4x4 operator - (const matrix4x4& m) const
	{
		return matrix4x4(M[0] - m.M[0], M[1] - m.M[1], M[2] - m.M[2], M[3] - m.M[3]);
	}
	matrix4x4 operator * (const matrix4x4& m) const
	{
		matrix4x4 r(call::noinitialize);
		return POCKET_CXX11_MOVE(multiply(m, r));
	}
	matrix4x4 operator * (T f) const
	{
		return matrix4x4(M[0] * f, M[1] * f, M[2] * f, M[3] * f);
	}
	matrix4x4 operator / (T f) const
	{
		POCKET_DEBUG_ASSERT(f != math_type::zero);
		T r = math_type::reciprocal(f);
		return matrix4x4(M[0] * r, M[1] * r, M[2] * r, M[3] * r);
	}

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
	matrix4x4& operator = (const call::zero_t&)
	{
		return load_zero();
	}
	matrix4x4& operator = (const call::identity_t&)
	{
		return load_identity();
	}
#ifdef POCKET_USE_CXX11
	matrix4x4& operator = (const std::initializer_list<T>& t)
	{
		if (t.size() == 0)
		{
			return load_identity();
		}
		POCKET_DEBUG_ASSERT(t.size() <= 16);
		typedef typename std::initializer_list<T>::const_iterator initializer_iterator;
		T* p = static_cast<T*>(M[0]);
		for (initializer_iterator i = t.begin(), end = t.end(); i != end; ++i, ++p)
		{
			*p = *i;
		}
		return *this;
	}
#endif // POCKET_USE_CXX11

	//---------------------------------------------------------------------
	// 複合演算子
	//---------------------------------------------------------------------
	matrix4x4& operator += (const matrix4x4& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i += *oi;
		}
		return *this;
	}
	matrix4x4& operator -= (const matrix4x4& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i -= *oi;
		}
		return *this;
	}
	matrix4x4& operator *= (const matrix4x4& m)
	{
		const matrix4x4 c = *this;
		return c.multiply(m, *this);
	}
	matrix4x4& operator *= (T s)
	{
#ifdef POCKET_USE_SIMD_ANONYMOUS
		simd_type factor = simd::set(s);
#endif // POCKET_USE_SIMD_ANONYMOUS
		for (iterator i = M.begin(), end = M.end(); i != end; ++i)
		{
#ifdef POCKET_USE_SIMD_ANONYMOUS
			i->mm = simd::mul(i->mm, factor);
#else
			i->x *= s;
			i->y *= s;
			i->z *= s;
			i->w *= s;
#endif // POCKET_USE_SIMD_ANONYMOUS
		}
		return *this;
	}
	matrix4x4& operator /= (T s)
	{
		POCKET_DEBUG_ASSERT(s != math_type::zero);
		return operator*=(math_type::reciprocal(s));
	}

	//------------------------------------------------------------------------------------------
	// タグでの関数呼び出し
	//------------------------------------------------------------------------------------------

	matrix4x4& operator () (const call::zero_t&)
	{
		return load_zero();
	}
	matrix4x4& operator () (const call::identity_t&)
	{
		return load_identity();
	}

	matrix4x4 operator () (const call::plus_t&) const
	{
		return operator+();
	}
	matrix4x4 operator () (const call::negate_t&) const
	{
		return operator-();
	}
	matrix4x4 operator () (const call::add_t&, const matrix4x4& m) const
	{
		return operator+(m);
	}
	matrix4x4 operator () (const call::sub_t&, const matrix4x4& m) const
	{
		return operator-(m);
	}
	matrix4x4 operator () (const call::mul_t&, T f) const
	{
		return operator*(f);
	}
	matrix4x4 operator () (const call::mul_t&, const matrix4x4& m) const
	{
		return operator*(m);
	}
	matrix4x4 operator () (const call::div_t&, T f) const
	{
		return operator/(f);
	}

	matrix4x4& operator () (const call::add_assign_t&, const matrix4x4& m)
	{
		return operator+=(m);
	}
	matrix4x4& operator () (const call::sub_assign_t&, const matrix4x4& m)
	{
		return operator-=(m);
	}
	matrix4x4& operator () (const call::mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	matrix4x4& operator () (const call::mul_assign_t&, const matrix4x4& m)
	{
		return operator*=(m);
	}
	matrix4x4& operator () (const call::div_assign_t&, T f)
	{
		return operator/=(f);
	}
	vector4<T>& operator () (const call::at_t&, int i)
	{
		return operator[](i);
	}
	const vector4<T>& operator () (const call::at_t&, int i) const
	{
		return operator[](i);
	}
	T& operator () (const call::at_t&, int y, int x)
	{
		return operator()(y, x);
	}
	const T& operator () (const call::at_t&, int y, int x) const
	{
		return operator()(y, x);
	}
	T& operator () (const call::at_t&, const matrix_point& p)
	{
		return operator[](p);
	}
	const T& operator () (const call::at_t&, const matrix_point& p) const
	{
		return operator[](p);
	}
	T* operator () (const call::pointer_t&)
	{
		return operator T*();
	}
	const T* operator () (const call::pointer_t&) const
	{
		return operator const T*();
	}

	bool operator () (const call::equal_t&, const matrix4x4& m) const
	{
		return operator==(m);
	}
	bool operator () (const call::not_equal_t&, const matrix4x4& m) const
	{
		return operator!=(m);
	}
	bool operator () (const call::near_t&, const matrix4x4& m) const
	{
		return near_equal(m);
	}
	bool operator () (const call::near_zero_t&) const
	{
		return near_equal_zero();
	}

	vector3<T> operator () (const call::scale_t&) const
	{
		return scale();
	}
	vector3<T>& operator () (const call::right_t&)
	{
		return right();
	}
	const vector3<T>& operator () (const call::right_t&) const
	{
		return right();
	}
	vector3<T>& operator () (const call::up_t&)
	{
		return up();
	}
	const vector3<T>& operator () (const call::up_t&) const
	{
		return up();
	}
	vector3<T>& operator () (const call::front_t&)
	{
		return forward();
	}
	const vector3<T>& operator () (const call::front_t&) const
	{
		return forward();
	}
	vector3<T>& operator () (const call::axis_t&, const call::x_t&)
	{
		return right();
	}
	const vector3<T>& operator () (const call::axis_t&, const call::x_t&) const
	{
		return right();
	}
	vector3<T>& operator () (const call::axis_t&, const call::y_t&)
	{
		return up();
	}
	const vector3<T>& operator () (const call::axis_t&, const call::y_t&) const
	{
		return up();
	}
	vector3<T>& operator () (const call::axis_t&, const call::z_t&)
	{
		return forward();
	}
	const vector3<T>& operator () (const call::axis_t&, const call::z_t&) const
	{
		return forward();
	}
	vector3<T>& operator () (const call::position_t&)
	{
		return position();
	}
	const vector3<T>& operator () (const call::position_t&) const
	{
		return position();
	}

	matrix4x4& operator () (const call::scale_t&, const vector3<T>& v)
	{
		return load_scale(v);
	}
	matrix4x4& operator () (const call::scale_t&, T x, T y, T z)
	{
		return load_scale(x, y, z);
	}
	matrix4x4& operator () (const call::scale_t&, T s)
	{
		return load_scale(s);
	}
	matrix4x4& operator () (const call::rotate_t&, const call::x_t&, T angle)
	{
		return load_rotate_x(angle);
	}
	matrix4x4& operator () (const call::rotate_t&, const call::y_t&, T angle)
	{
		return load_rotate_y(angle);
	}
	matrix4x4& operator () (const call::rotate_t&, const call::z_t&, T angle)
	{
		return load_rotate_z(angle);
	}
	matrix4x4& operator () (const call::rotate_t&, T roll, T pitch, T yaw)
	{
		return load_rotate(roll, pitch, yaw);
	}
	matrix4x4& operator () (const call::rotate_t&, const quaternion<T>& q)
	{
		return load_rotate(q);
	}
	matrix4x4& operator () (const call::rotate_t&, const vector3<T>& axis, T angle)
	{
		return load_rotate(axis, angle);
	}
	matrix4x4& operator () (const call::translate_t&, const vector3<T>& t)
	{
		return load_translate(t);
	}
	matrix4x4& operator () (const call::translate_t&, T x, T y, T z)
	{
		return load_translate(x, y, z);
	}
	matrix4x4& operator () (const call::world_t&, const vector3<T>& s, T roll, T pitch, T yaw, const vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	matrix4x4& operator () (const call::world_t&, T s, T roll, T pitch, T yaw, const vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	matrix4x4& operator () (const call::world_t&, const vector3<T>& s, const quaternion<T>& q, const vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	matrix4x4& operator () (const call::world_t&, T s, const quaternion<T>& q, const vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	matrix4x4& operator () (const call::perspective_field_of_view_t&, T fovy, T aspect, T n, T f)
	{
		return load_perspective_field_of_view(fovy, aspect, n, f);
	}
	matrix4x4& operator () (const call::orthographics_t&, T left, T right, T up, T bottom, T n, T f)
	{
		return load_orthographics(left, right, up, bottom, n, f);
	}
	matrix4x4& operator () (const call::orthographics_t&, T width, T height, T n, T f)
	{
		return load_orthographics(width, height, n, f);
	}
	matrix4x4& operator () (const call::orthographics2d_t&, T left, T right, T up, T bottom)
	{
		return load_orthographics2d(left, right, up, bottom);
	}
	matrix4x4& operator () (const call::orthographics2d_t&, T width, T height)
	{
		return load_orthographics2d(width, height);
	}
	matrix4x4& operator () (const call::look_to_t&, const vector3<T>& eye, const vector3<T>& to, const vector3<T>& up = vector3<T>::up)
	{
		return load_lookto(eye, to, up);
	}
	matrix4x4& operator () (const call::look_to_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookto(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	matrix4x4& operator () (const call::look_at_t&, const vector3<T>& eye, const vector3<T>& center, const vector3<T>& up = vector3<T>::up)
	{
		return load_lookat(eye, center, up);
	}
	matrix4x4& operator () (const call::look_at_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	matrix4x4& operator () (const call::transpose_t&)
	{
		return transpose();
	}
	matrix4x4 operator () (const call::transposed_t&) const
	{
		return transposed();
	}
	T operator () (const call::determinant_t&) const
	{
		return determinant();
	}
	matrix4x4& operator () (const call::inverse_t&) const
	{
		return inverse();
	}
	matrix4x4 operator () (const call::slerp_t&, const matrix4x4& m, T t) const
	{
		return slerp(m, t);
	}
	vector3<T> operator () (const call::transform_t&, const vector3<T>& v) const
	{
		return transform(v);
	}
	vector3<T> operator () (const call::transform_coord_t&, const vector3<T>& v) const
	{
		return transform_coord(v);
	}
	vector3<T> operator () (const call::transform_normal_t&, const vector3<T>& v) const
	{
		return transform_normal(v);
	}
	vector4<T> operator () (const call::transform_t&, const vector4<T>& v) const
	{
		return transform(v);
	}
	T operator () (const call::roll_t&) const
	{
		return roll();
	}
	T operator () (const call::yaw_t&) const
	{
		return yaw();
	}
	T operator () (const call::pitch_t&) const
	{
		return pitch();
	}
};

template <typename T>
const matrix4x4<T> matrix4x4<T>::zero(math_type::zero);
template <typename T>
const matrix4x4<T> matrix4x4<T>::identity(math_type::one);

//---------------------------------------------------------------------
// vector3.transform
//---------------------------------------------------------------------
template <typename T> inline
vector3<T>& vector3<T>::transform(const matrix4x4<T>& m)
{
	const vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector3<T>& vector3<T>::transform(const matrix4x4<T>& m, vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector3<T> vector3<T>::transformed(const matrix4x4<T>& m) const
{
	vector3<T> v(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform(*this, v));
}
template <typename T> inline
vector3<T>& vector3<T>::transform_coord(const matrix4x4<T>& m)
{
	const vector3<T> v = *this;
	return m.transform_coord(v, *this);
}
template <typename T> inline
vector3<T>& vector3<T>::transform_coord(const matrix4x4<T>& m, vector3<T>& result) const
{
	return m.transform_coord(*this, result);
}
template <typename T> inline
vector3<T> vector3<T>::transformed_coord(const matrix4x4<T>& m) const
{
	vector3<T> v(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform_coord(*this, v));
}
template <typename T> inline
vector3<T>& vector3<T>::transform_normal(const matrix4x4<T>& m)
{
	const vector3<T> v = *this;
	return m.transform_normal(v, *this);
}
template <typename T> inline
vector3<T>& vector3<T>::transform_normal(const matrix4x4<T>& m, vector3<T>& result) const
{
	return m.transform_normal(*this, result);
}
template <typename T> inline
vector3<T> vector3<T>::transformed_normal(const matrix4x4<T>& m) const
{
	vector3<T> v(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform_normal(*this, v));
}

//---------------------------------------------------------------------
// vector4.transform
//---------------------------------------------------------------------
template <typename T> inline
vector4<T>& vector4<T>::transform(const matrix4x4<T>& m)
{
	const vector4<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector4<T> vector4<T>::transformed(const matrix4x4<T>& m) const
{
	vector4<T> v(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform(*this, v));
}
template <typename T> inline
vector4<T>& vector4<T>::transform(const matrix4x4<T>& m, vector4<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector4<T>& vector4<T>::transform_coord(const matrix4x4<T>& m)
{
	const vector4<T> v = *this;
	return m.transform_coord(v, *this);
}
template <typename T> inline
vector4<T>& vector4<T>::transform_coord(const matrix4x4<T>& m, vector4<T>& result) const
{
	return m.transform_coord(*this, result);
}
template <typename T> inline
vector4<T> vector4<T>::transformed_coord(const matrix4x4<T>& m) const
{
	vector4<T> v(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform_coord(*this, v));
}

template <typename T> inline
vector3<T> vector3<T>::operator * (const matrix4x4<T>& m) const
{
	vector3<T> r(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform(*this, r));
}
template <typename T> inline
vector4<T> vector4<T>::operator * (const matrix4x4<T>& m) const
{
	vector4<T> r(call::noinitialize);
	return POCKET_CXX11_MOVE(m.transform(*this, r));
}

//---------------------------------------------------------------------
// 行列からクォータニオンを求める
//---------------------------------------------------------------------
template <typename T> inline
quaternion<T>& quaternion<T>::from_matrix(const matrix4x4<T>& m)
{
	const vector4<T>* v0 = &m.M[0];
	const vector4<T>* v1 = &m.M[1];
	const vector4<T>* v2 = &m.M[2];

	T tr = v0->x + v1->y + v2->z + m.M[3].w;
	if (tr >= math_type::one)
	{
		T fourD = math_type::two * math_type::sqrt(tr);
		T dFourD = math_type::one / fourD;
		x = (v1->z - v2->y) * dFourD;
		y = (v2->x - v0->z) * dFourD;
		z = (v0->y - v1->x) * dFourD;
		// fourD / math_type::four
		w = fourD * math_type::half_of_half;
		return *this;
	}

	int i = 0;
	if (v0->x <= v1->y)
	{
		++i;
	}
	if (v2->z > m[i][i])
	{
		++i;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;

	tr = m[i][i] - m[j][j] - m[k][k] + math_type::one;
	T fourD = math_type::two * math_type::sqrt(tr);
	T dFourD = math_type::one / fourD;

	(*this)[i] = fourD * math_type::half_of_half;
	(*this)[j] = (m[j][i] + m[i][j]) * dFourD;
	(*this)[k] = (m[k][i] + m[i][k]) * dFourD;
	w = (m[j][k] - m[k][j]) * dFourD;

	return *this;
}

// 4x4行列からの作成
template <typename T> inline
matrix3x3<T>::matrix3x3(const matrix4x4<T>& m)
{
	M[0] = m[0];
	M[1] = m[1];
	M[2] = m[2];
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const matrix4x4<T>& v)
{
	//{
	//	V0
	//	V1
	//	V2
	//	V3
	//}
	os << io::braces_left << io::line_feed;
	for (typename matrix4x4<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line_feed;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, matrix4x4<T>& v)
{
	is.ignore();
	for (typename matrix4x4<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_MATRIX4X4_H__
