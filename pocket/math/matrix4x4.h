#ifndef __MATH_MATRIX4X4_H__
#define __MATH_MATRIX4X4_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "math_traits.h"
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include "matrix3x3.h"
#ifdef _USE_SIMD_ANONYMOUS
#include "simd_traits.h"
#endif // _USE_SIMD_ANONYMOUS
#ifdef _USE_CXX11
#include <initializer_list>
#endif // _USE_CXX11
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef matrix4x4<float> matrix4x4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef matrix4x4<double> matrix4x4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef matrix4x4<long double> matrix4x4ld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct matrix4x4
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef vector4<T> vector4_type;
	typedef vector4<T> row_type;
	typedef vector4<T> column_type;
	typedef typename math_type::sin_cos_t sin_cos_type;

#ifdef _USE_SIMD_ANONYMOUS
	typedef typename vector4<T>::simd simd;
	typedef typename vector4<T>::simd_type simd_type;
#endif // _USE_SIMD_ANONYMOUS

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

	typedef vector4<T>(*CalcFunc)(const vector4<T>&, const vector4<T>&);

	/*-----------------------------------------------------------------------------------------
	* Members
	*-----------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS_NON_POD

			array4x4_type M;

#ifdef _USE_ANONYMOUS_NON_POD
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
#endif // _USE_ANONYMOUS_NON_POD

	template <typename> friend struct matrix4x4;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const matrix4x4 zero; // 0.0 x 16
	static const matrix4x4 identity; // 単位行列

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(matrix4x4);
	explicit matrix4x4(const behavior::_noinitialize_t&)
	{

	}
	explicit matrix4x4(const behavior::_zero_t&)
	{
		M[0] = row_type::zero;
		M[1] = row_type::zero;
		M[2] = row_type::zero;
		M[3] = row_type::zero;
	}
	explicit matrix4x4(const behavior::_identity_t&)
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
#ifdef _USE_ANONYMOUS_NON_POD
		: mv0(M1), mv1(M2), mv2(M3), mv3(M4)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		M[0] = M1;
		M[1] = M2;
		M[2] = M3;
		M[3] = M4;
#endif // _USE_ANONYMOUS_NON_POD
	}
	explicit matrix4x4(const vector3<T>& M1, T M1W,
		const vector3<T>& M2, T M2W,
		const vector3<T>& M3, T M3W,
		const vector3<T>& M4, T M4W = math_type::one)
#ifdef _USE_ANONYMOUS_NON_POD
		: mv0(M1, M1W), mv1(M2, M2W), mv2(M3, M3W), mv3(M4, M4W)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		M[0] = row_type(M1, M1W);
		M[1] = row_type(M2, M2W);
		M[2] = row_type(M3, M3W);
		M[3] = row_type(M4, M4W);
#endif // _USE_ANONYMOUS_NON_POD
	}
	explicit matrix4x4(const matrix3x3<T>& m)
#ifdef _USE_ANONYMOUS_NON_POD
		: mv0(m[0], math_type::zero),
		mv1(m[1], math_type::zero),
		mv2(m[2], math_type::zero),
		mv3(row_type::unit_w)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		M[0] = row_type(m[0], math_type::zero);
		M[1] = row_type(m[1], math_type::zero);
		M[2] = row_type(m[2], math_type::zero);
		M[3] = row_type::unit_w;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

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
	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* 引き算
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* 掛け算
	*---------------------------------------------------------------------*/
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

#ifdef _USE_SIMD_ANONYMOUS
		/*------------------------------
		*    1  2  3  4   ->
		*   ____________   ____________   ____________
		* 1 |0, 0, 0, 0|   |0, 0, 0, 0|   |0, 1, 2, 3|
		* 2 |1, 1, 1, 1| = |1, 1, 1, 1| * |0, 1, 2, 3|
		* 3 |2, 2, 2, 2|   |2, 2, 2, 2|   |0, 1, 2, 3|
		* 4 |3, 3, 3, 3|   |3, 3, 3, 3|   |0, 1, 2, 3|
		*------------------------------*/

		iterator ri = result.M.begin();
		const_pointer mi = &m.M[0];
		const_pointer j;
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			simd_type mx = simd::template permute<0>(i->mm);
			simd_type my = simd::template permute<1>(i->mm);
			simd_type mz = simd::template permute<2>(i->mm);
			simd_type mw = simd::template permute<3>(i->mm);

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
#else // _USE_SIMD_ANONYMOUS

#	if defined(__DEBUG)
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
#endif // _USE_SIMD_ANONYMOUS
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
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	matrix4x4& divide(T s, matrix4x4& result) const
	{
		_DEB_ASSERT(s != math_type::zero);
#ifdef _USE_SIMD_ANONYMOUS
		simd_type rcp = simd::set(math_type::reciprocal(s));
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->mm = simd::mul(i->mm, rcp);
		}
		return result;
#else
		return multiply(math_type::reciprocal(s), result);
#endif // _USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 値か近いか
	*---------------------------------------------------------------------*/
	bool is_near(const matrix4x4& m) const
	{
		return M[0].is_near(m.M[0]) &&
			M[1].is_near(m.M[1]) &&
			M[2].is_near(m.M[2]) &&
			M[3].is_near(m.M[3]);
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return M[0].is_near_zero() &&
			M[1].is_near_zero() &&
			M[2].is_near_zero() &&
			M[3].is_near_zero();
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return M[0].is_zero() &&
			M[1].is_zero() &&
			M[2].is_zero() &&
			M[3].is_zero();
	}

	/*---------------------------------------------------------------------
	* 値が単位行列と近いか
	*---------------------------------------------------------------------*/
	bool is_near_identity() const
	{
		return M[0].is_near(row_type::unit_x) &&
			M[1].is_near(row_type::unit_y) &&
			M[2].is_near(row_type::unit_z) &&
			M[3].is_near(row_type::unit_w);
	}

	/*---------------------------------------------------------------------
	* 右ベクトル取得・設定
	*---------------------------------------------------------------------*/
	vector3<T>& right()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef _USE_ANONYMOUS_NON_POD
		return _right;
#else
		return reinterpret_cast<vector3<T>&>(M[0]);
#endif // _USE_ANONYMOUS_NON_POD
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
	/*---------------------------------------------------------------------
	* 上ベクトル取得・設定
	*---------------------------------------------------------------------*/
	vector3<T>& up()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef _USE_ANONYMOUS_NON_POD
		return _up;
#else
		return reinterpret_cast<vector3<T>&>(M[1]);
#endif // _USE_ANONYMOUS_NON_POD
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
	/*---------------------------------------------------------------------
	* 前ベクトル取得・設定
	*---------------------------------------------------------------------*/
	vector3<T>& forward()
	{
		//return vector3<T>(m.x, m.y, m.z);
#ifdef _USE_ANONYMOUS_NON_POD
		return _forward;
#else
		return reinterpret_cast<vector3<T>&>(M[2]);
#endif // _USE_ANONYMOUS_NON_POD
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
	/*---------------------------------------------------------------------
	* 座標取得・設定
	*---------------------------------------------------------------------*/
	vector3<T>& position()
	{
		// vector3<T>(m.x, m.y, m.z);
#ifdef _USE_ANONYMOUS_NON_POD
		return _position;
#else
		return reinterpret_cast<vector3<T>&>(M[3]);
#endif // _USE_ANONYMOUS_NON_POD
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

	/*---------------------------------------------------------------------
	* 拡大縮小取得
	*---------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------
	* x軸回転量を求める
	*---------------------------------------------------------------------*/
	T pitch() const
	{
		// 前ベクトルのy軸の状態から計算する
		return forward().pitch();
	}
	/*---------------------------------------------------------------------
	* y軸回転量を求める
	*---------------------------------------------------------------------*/
	T yaw() const
	{
		return forward().yaw();
	}
	/*---------------------------------------------------------------------
	* z軸回転量を求める
	*---------------------------------------------------------------------*/
	T roll() const
	{
		return math_type::atan2(M[0].y, M[1].y);
	}

	/*---------------------------------------------------------------------
	* すべての要素をゼロにする
	*---------------------------------------------------------------------*/
	matrix4x4& load_zero()
	{
#if 0
		M[0] = vector4<T>::zero;
		M[1] = vector4<T>::zero;
		M[2] = vector4<T>::zero;
		M[3] = vector4<T>::zero;
		//M.fill(vector4<T>::zero);
#endif

		*this = matrix4x4::zero;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 単位行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_identity()
	{
#if 0
		M[0] = vector4<T>::unit_x;
		M[1] = vector4<T>::unit_y;
		M[2] = vector4<T>::unit_z;
		M[3] = vector4<T>::unit_w;
		//std::memcpy(this, &matrix4x4::identity, sizeof(*this));
#endif

		*this = matrix4x4::identity;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする
	*---------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------
	* x軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_x(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type::unit_x;
		M[1] = row_type(math_type::zero, a.cos, a.sin, math_type::zero);
		M[2] = row_type(math_type::zero, -a.sin, a.cos, math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	/*---------------------------------------------------------------------
	* y軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_y(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, math_type::zero, -a.sin, math_type::zero);
		M[1] = row_type::unit_y;
		M[2] = row_type(a.sin, math_type::zero, a.cos, math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	/*---------------------------------------------------------------------
	* z軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_z(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, a.sin, math_type::zero, math_type::zero);
		M[1] = row_type(-a.sin, a.cos, math_type::zero, math_type::zero);
		M[2] = row_type::unit_z;
		M[3] = row_type::unit_w;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（ZXY）
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_roll_pitch_yaw(T roll, T pitch, T yaw)
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
	/*---------------------------------------------------------------------
	* 回転行列にする（クォータニオン）
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_quaternion(const quaternion<T>& q)
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
		M[1] = row_type(xy - wz, math_type::one - (xx + zz), yz + wz, math_type::zero);
		M[2] = row_type(xz + wy, yz + wx, math_type::one - (xx + yy), math_type::zero);
		M[3] = row_type::unit_w;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（任意軸＋角度）
	*---------------------------------------------------------------------*/
	matrix4x4& load_rotate_axis_angle(const vector3<T>& axis, T angle)
	{
		// 四元数での計算を行なう
		return load_rotate_quaternion(quaternion<T>(axis, angle));
	}
	/*---------------------------------------------------------------------
	* 座標変換行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_translate(T x, T y, T z)
	{
		load_identity();
#ifdef _USE_SIMD_ANONYMOUS
		M[3].mm = simd::set(x, y, z, math_type::one);
#else
		M[3] = row_type(x, y, z, math_type::one);
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	matrix4x4& load_translate(const vector3<T>& v)
	{
		load_identity();
		M[3] = v;
		return *this;
	}
	/*---------------------------------------------------------------------
	* ワールド変換行列にする
	*---------------------------------------------------------------------*/
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
		M[2] = row_type(s.z * p.cos * y.sin, s.z * -p.sin, s.z * p.cos * y.cos, math_type::zero);
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

		M[0] = row_type(s.x * (math_type::one - yy + zz), s.x * (xy + wz), s.x * (xz - wy), math_type::zero);
		M[1] = row_type(s.y * (xy - wz), s.y * (math_type::one - xx + zz), s.y * (yz + wz), math_type::zero);
		M[2] = row_type(s.z * (xz + wy), s.z * (yz + wx), s.z * (math_type::one - xx + yy), math_type::zero);
		M[3] = row_type(t, math_type::one);
		return *this;
	}
	matrix4x4& load_world(T s, const quaternion<T>& q, const vector3<T>& t)
	{
		return load_world(vector3<T>(s), q, t);
	}
	/*---------------------------------------------------------------------
	* 射影変換行列にする
	*---------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------
	* 正射影変換行列にする
	*---------------------------------------------------------------------*/
	matrix4x4& load_orthographics(T left, T right, T top, T bottom, T near, T far)
	{
		load_identity();

		T dx = math_type::reciprocal(right - left);
		T dy = math_type::reciprocal(top - bottom);
		T dz = math_type::reciprocal(far - near);

		M[0].x = math_type::two * dx;
		M[1].y = math_type::two * dy;
		M[2].z = -math_type::two * dz; // 右手特有

		row_type& r3 = M[3];
		r3.x = -(right + left) * dx;
		r3.y = -(bottom + top) * dy;
		r3.z = -(far + near) * dz;

		return *this;
	}
	matrix4x4& load_orthographics(T width, T height, T near, T far)
	{
		return load_orthographics(math_type::zero, width, math_type::zero, height, near, far);
	}
	/*---------------------------------------------------------------------
	* 2D用正射影変換行列にする
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* 視野変換行列にする（座標＋向き）
	*---------------------------------------------------------------------*/
	matrix4x4& load_lookto(const vector3<T>& eye, const vector3<T>& direction, const vector3<T>& up = vector3<T>::up)
	{
		// カメラを原点としてカメラから見た変換を行う

		// 前方向と上方向の外積で右ベクトルを求める
		vector3<T> x(behavior::noinitialize);
		up.cross(direction, x).normalize();
		// 前と右から上方向のベクトルを求める
		vector3<T> y(behavior::noinitialize);
		direction.cross(x, y);

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

	/*---------------------------------------------------------------------
	* 視野変換行列にする（座標＋注視点）
	*---------------------------------------------------------------------*/
	matrix4x4& load_lookat(const vector3<T>& eye, const vector3<T>& center, const vector3<T>& up = vector3<T>::up)
	{
		// 向きを求める
		vector3<T> dir(behavior::noinitialize);
		return load_lookto(eye, center.direction(eye, dir), up);
	}
	matrix4x4& load_lookat(T ex, T ey, T ez, T cx, T cy, T cz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookat(vector3<T>(ex, ey, ez), vector3<T>(cx, cy, cz), vector3<T>(ux, uy, uz));
	}

	/*---------------------------------------------------------------------
	* 転置行列
	*---------------------------------------------------------------------*/
	matrix4x4& transpose()
	{
		// 回転を扱う場合の転置は逆マトリクスと同一になる

		row_type& v0 = M[0];
		row_type& v1 = M[1];
		row_type& v2 = M[2];
		row_type& v3 = M[3];

#ifdef __DEBUG
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
#endif // __DEBUG

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
	/*---------------------------------------------------------------------
	* 転置行列を求める
	*---------------------------------------------------------------------*/
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
	/*---------------------------------------------------------------------
	* 行列式を求める
	*---------------------------------------------------------------------*/
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
#endif
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
	}

	/*---------------------------------------------------------------------
	* 逆行列にする（逆行列が存在しない場合は偽）
	*---------------------------------------------------------------------*/
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
		if (math_type::is_near_zero(det))
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
	/*---------------------------------------------------------------------
	* 逆行列を求める
	*---------------------------------------------------------------------*/
	matrix4x4 inversed() const
	{
		matrix4x4 result(behavior::noinitialize);
		return inverse(result);;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（w=1）
	*---------------------------------------------------------------------*/
	vector3<T> transform(const vector3<T>& v) const
	{
		vector3<T> r(behavior::noinitialize);
		return transform(v, r);
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
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）（w=1）
	*---------------------------------------------------------------------*/
	vector3<T> transform_coord(const vector3<T>& v) const
	{
		vector3<T> r(behavior::noinitialize);
		return transform_coord(v, r);
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
	/*---------------------------------------------------------------------
	* ベクトル座標変換
	*---------------------------------------------------------------------*/
	vector4<T> transform(const vector4<T>& v) const
	{
		vector4<T> r(behavior::noinitialize);
		return transform(v, r);
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
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）
	*---------------------------------------------------------------------*/
	vector4<T> transform_coord(const vector4<T>& v) const
	{
		vector4<T> r(behavior::noinitialize);
		return transform_coord(v, r);
	}
	vector4<T>& transform_coord(const vector4<T>& v, vector4<T>& result) const
	{
		transform(v, result);

		T w = math_type::reciprocal(result.w);
		result *= w;
		return result;
	}

	/*---------------------------------------------------------------------
	* ベクトル座標変換（w=0）
	*---------------------------------------------------------------------*/
	vector3<T> transform_normal(const vector3<T>& v) const
	{
		vector3<T> r(behavior::noinitialize);
		return transform_normal(v, r);
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
	/*---------------------------------------------------------------------
	* 球面線形補間を求める
	*---------------------------------------------------------------------*/
	matrix4x4 slerp(const matrix4x4& to, T t) const
	{
		matrix4x4 result(behavior::noinitialize);
		return slerp(to, t, result);
	}
	matrix4x4& slerp(const matrix4x4& to, T t, matrix4x4& result) const
	{
		vector3<T> s(behavior::noinitialize);
		vector3<T> p(behavior::noinitialize);
		quaternion<T> r(behavior::noinitialize);

		// 補間用の値取得
		vector3<T> s0 = scale();
		vector3<T> s1 = to.scale();
		// 回転取得
		quaternion<T> q0(*this);
		quaternion<T> q1(to);

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

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	vector4<T>& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
		return M[i];
	}
	const vector4<T>& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
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
		_DEB_RANGE_ASSERT(x, 0, 3);
		_DEB_RANGE_ASSERT(y, 0, 3);
		return M[y][x];
	}
	const T& operator () (int y, int x) const
	{
		_DEB_RANGE_ASSERT(x, 0, 3);
		_DEB_RANGE_ASSERT(y, 0, 3);
		return M[y][x];
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator matrix4x4<U>() const
	{
		typedef vector4<U> other_vector4_type;
		return matrix4x4<U>(static_cast<other_vector4_type>(M[0]),
			static_cast<other_vector4_type>(M[1]),
			static_cast<other_vector4_type>(M[2]),
			static_cast<other_vector4_type>(M[3]));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &M[0].x;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &data[0];
#else
		return &M[0].x;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
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

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	matrix4x4 operator + () const
	{
		return *this;
	}
	matrix4x4 operator - () const
	{
		return matrix4x4(-M[0], -M[1], -M[2], -M[3]);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
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
		matrix4x4 result(behavior::noinitialize);
		return multiply(m, result);
	}
	matrix4x4 operator * (T f) const
	{
		return matrix4x4(M[0] * f, M[1] * f, M[2] * f, M[3] * f);
	}
	matrix4x4 operator / (T f) const
	{
		_DEB_ASSERT(f != math_type::zero);
		T r = math_type::reciprocal(f);
		return matrix4x4(M[0] * r, M[1] * r, M[2] * r, M[3] * r);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	matrix4x4& operator = (const behavior::_zero_t&)
	{
		return load_zero();
	}
	matrix4x4& operator = (const behavior::_identity_t&)
	{
		return load_identity();
	}
#ifdef _USE_CXX11
	matrix4x4& operator = (const std::initializer_list<T>& t)
	{
		_DEB_ASSERT(t.size() <= 16);

		typename std::initializer_list<T>::const_iterator o = t.begin();
		T* p = static_cast<T*>(M[0]);
		for (typename std::initializer_list<T>::size_type i = 0, size = t.size(); i < size; ++i, ++p, ++o)
		{
			*p = *o;
		}
		return *this;
	}
#endif // _USE_CXX11

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
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
#ifdef _USE_SIMD_ANONYMOUS
		simd_type factor = simd::set(s);
#endif // _USE_SIMD_ANONYMOUS
		for (iterator i = M.begin(), end = M.end(); i != end; ++i)
		{
#ifdef _USE_SIMD_ANONYMOUS
			i->mm = simd::mul(i->mm, factor);
#else
			i->x *= s;
			i->y *= s;
			i->z *= s;
			i->w *= s;
#endif // _USE_SIMD_ANONYMOUS
		}
		return *this;
	}
	matrix4x4& operator /= (T s)
	{
		_DEB_ASSERT(s != math_type::zero);
		return operator*=(math_type::reciprocal(s));
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	matrix4x4& operator () (const behavior::_zero_t&)
	{
		return load_zero();
	}
	matrix4x4& operator () (const behavior::_identity_t&)
	{
		return load_identity();
	}

	matrix4x4 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	matrix4x4 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	matrix4x4 operator () (const behavior::_add_t&, const matrix4x4& m) const
	{
		return operator+(m);
	}
	matrix4x4 operator () (const behavior::_sub_t&, const matrix4x4& m) const
	{
		return operator-(m);
	}
	matrix4x4 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	matrix4x4 operator () (const behavior::_mul_t&, const matrix4x4& m) const
	{
		return operator*(m);
	}
	matrix4x4 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}

	matrix4x4& operator () (const behavior::_add_assign_t&, const matrix4x4& m)
	{
		return operator+=(m);
	}
	matrix4x4& operator () (const behavior::_sub_assign_t&, const matrix4x4& m)
	{
		return operator-=(m);
	}
	matrix4x4& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	matrix4x4& operator () (const behavior::_mul_assign_t&, const matrix4x4& m)
	{
		return operator*=(m);
	}
	matrix4x4& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	vector4<T>& operator () (const behavior::_at_t&, int i)
	{
		return operator[](i);
	}
	const vector4<T>& operator () (const behavior::_at_t&, int i) const
	{
		return operator[](i);
	}
	T& operator () (const behavior::_at_t&, int y, int x)
	{
		return operator()(y, x);
	}
	const T& operator () (const behavior::_at_t&, int y, int x) const
	{
		return operator()(y, x);
	}
	T& operator () (const behavior::_at_t&, const matrix_point& p)
	{
		return operator[](p);
	}
	const T& operator () (const behavior::_at_t&, const matrix_point& p) const
	{
		return operator[](p);
	}
	T* operator () (const behavior::_pointer_t&)
	{
		return operator T*();
	}
	const T* operator () (const behavior::_pointer_t&) const
	{
		return operator const T*();
	}

	bool operator () (const behavior::_equal_t&, const matrix4x4& m) const
	{
		return operator==(m);
	}
	bool operator () (const behavior::_not_equal_t&, const matrix4x4& m) const
	{
		return operator!=(m);
	}
	bool operator () (const behavior::_near_t&, const matrix4x4& m) const
	{
		return is_near(m);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return is_near_zero();
	}

	vector3<T> operator () (const behavior::_scale_t&) const
	{
		return scale();
	}
	vector3<T>& operator () (const behavior::_right_t&)
	{
		return right();
	}
	const vector3<T>& operator () (const behavior::_right_t&) const
	{
		return right();
	}
	vector3<T>& operator () (const behavior::_up_t&)
	{
		return up();
	}
	const vector3<T>& operator () (const behavior::_up_t&) const
	{
		return up();
	}
	vector3<T>& operator () (const behavior::_front_t&)
	{
		return forward();
	}
	const vector3<T>& operator () (const behavior::_front_t&) const
	{
		return forward();
	}
	vector3<T>& operator () (const behavior::_axis_t&, const behavior::_x_t&)
	{
		return right();
	}
	const vector3<T>& operator () (const behavior::_axis_t&, const behavior::_x_t&) const
	{
		return right();
	}
	vector3<T>& operator () (const behavior::_axis_t&, const behavior::_y_t&)
	{
		return up();
	}
	const vector3<T>& operator () (const behavior::_axis_t&, const behavior::_y_t&) const
	{
		return up();
	}
	vector3<T>& operator () (const behavior::_axis_t&, const behavior::_z_t&)
	{
		return forward();
	}
	const vector3<T>& operator () (const behavior::_axis_t&, const behavior::_z_t&) const
	{
		return forward();
	}
	vector3<T>& operator () (const behavior::_position_t&)
	{
		return position();
	}
	const vector3<T>& operator () (const behavior::_position_t&) const
	{
		return position();
	}

	matrix4x4& operator () (const behavior::_scale_t&, const vector3<T>& v)
	{
		return load_scale(v);
	}
	matrix4x4& operator () (const behavior::_scale_t&, T x, T y, T z)
	{
		return load_scale(x, y, z);
	}
	matrix4x4& operator () (const behavior::_scale_t&, T s)
	{
		return load_scale(s);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_x_t&, T angle)
	{
		return load_rotate_x(angle);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_y_t&, T angle)
	{
		return load_rotate_y(angle);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_z_t&, T angle)
	{
		return load_rotate_z(angle);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_roll_pitch_yaw_t&, T roll, T pitch, T yaw)
	{
		return load_rotate_roll_pitch_yaw(roll, pitch, yaw);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const quaternion<T>& q)
	{
		return load_rotate_quaternion(q);
	}
	matrix4x4& operator () (const behavior::_rotate_t&, const vector3<T>& axis, T angle)
	{
		return load_rotate_axis_angle(axis, angle);
	}
	matrix4x4& operator () (const behavior::_translate_t&, const vector3<T>& t)
	{
		return load_translate(t);
	}
	matrix4x4& operator () (const behavior::_translate_t&, T x, T y, T z)
	{
		return load_translate(x, y, z);
	}
	matrix4x4& operator () (const behavior::_world_t&, const vector3<T>& s, T roll, T pitch, T yaw, const vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	matrix4x4& operator () (const behavior::_world_t&, T s, T roll, T pitch, T yaw, const vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	matrix4x4& operator () (const behavior::_world_t&, const vector3<T>& s, const quaternion<T>& q, const vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	matrix4x4& operator () (const behavior::_world_t&, T s, const quaternion<T>& q, const vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	matrix4x4& operator () (const behavior::_perspective_field_of_view_t&, T fovy, T aspect, T n, T f)
	{
		return load_perspective_field_of_view(fovy, aspect, n, f);
	}
	matrix4x4& operator () (const behavior::_orthographics_t&, T left, T right, T up, T bottom, T n, T f)
	{
		return load_orthographics(left, right, up, bottom, n, f);
	}
	matrix4x4& operator () (const behavior::_orthographics_t&, T width, T height, T n, T f)
	{
		return load_orthographics(width, height, n, f);
	}
	matrix4x4& operator () (const behavior::_orthographics2d_t&, T left, T right, T up, T bottom)
	{
		return load_orthographics2d(left, right, up, bottom);
	}
	matrix4x4& operator () (const behavior::_orthographics2d_t&, T width, T height)
	{
		return load_orthographics2d(width, height);
	}
	matrix4x4& operator () (const behavior::_look_to_t&, const vector3<T>& eye, const vector3<T>& to, const vector3<T>& up = vector3<T>::up)
	{
		return load_lookto(eye, to, up);
	}
	matrix4x4& operator () (const behavior::_look_to_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookto(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	matrix4x4& operator () (const behavior::_look_at_t&, const vector3<T>& eye, const vector3<T>& center, const vector3<T>& up = vector3<T>::up)
	{
		return load_lookat(eye, center, up);
	}
	matrix4x4& operator () (const behavior::_look_at_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::zero, T uy = math_type::one, T uz = math_type::zero)
	{
		return load_lookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	matrix4x4& operator () (const behavior::_transpose_t&)
	{
		return transpose();
	}
	matrix4x4 operator () (const behavior::_transposed_t&) const
	{
		return transposed();
	}
	T operator () (const behavior::_determinant_t&) const
	{
		return determinant();
	}
	matrix4x4 operator () (const behavior::_inverse_t&) const
	{
		return inverse();
	}
	matrix4x4 operator () (const behavior::_slerp_t&, const matrix4x4& m, T t) const
	{
		return slerp(m, t);
	}
	vector3<T> operator () (const behavior::_transform_t&, const vector3<T>& v) const
	{
		return transform(v);
	}
	vector3<T> operator () (const behavior::_transform_coord_t&, const vector3<T>& v) const
	{
		return transform_coord(v);
	}
	vector3<T> operator () (const behavior::_transform_normal_t&, const vector3<T>& v) const
	{
		return transform_normal(v);
	}
	vector4<T> operator () (const behavior::_transform_t&, const vector4<T>& v) const
	{
		return transform(v);
	}
	T operator () (const behavior::_roll_t&) const
	{
		return roll();
	}
	T operator () (const behavior::_yaw_t&) const
	{
		return yaw();
	}
	T operator () (const behavior::_pitch_t&) const
	{
		return pitch();
	}
};

template <typename T>
const matrix4x4<T> matrix4x4<T>::zero(math_type::zero);
template <typename T>
const matrix4x4<T> matrix4x4<T>::identity(math_type::one);

/*---------------------------------------------------------------------
* vector3.transform
*---------------------------------------------------------------------*/
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
	vector3<T> v(behavior::noinitialize);
	return m.transform(*this, v);
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
	vector3<T> v(behavior::noinitialize);
	return m.transform_coord(*this, v);
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
	vector3<T> v(behavior::noinitialize);
	return m.transform_normal(*this, v);
}

/*---------------------------------------------------------------------
* vector4.transform
*---------------------------------------------------------------------*/
template <typename T> inline
vector4<T>& vector4<T>::transform(const matrix4x4<T>& m)
{
	const vector4<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector4<T> vector4<T>::transformed(const matrix4x4<T>& m) const
{
	vector4<T> v(behavior::noinitialize);
	return m.transform(*this, v);
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
	vector4<T> v(behavior::noinitialize);
	return m.transform_coord(*this, v);
}

template <typename T> inline
vector3<T> vector3<T>::operator * (const matrix4x4<T>& m) const
{
	vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
vector4<T> vector4<T>::operator * (const matrix4x4<T>& m) const
{
	vector4<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* 行列からクォータニオンを求める
*---------------------------------------------------------------------*/
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

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const matrix4x4<T>& v)
{
	//{
	//	V0
	//	V1
	//	V2
	//	V3
	//}
	os << io::braces_left << io::line;
	for (typename matrix4x4<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line;
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
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const matrix4x4<T>& v)
{
	os << io::braces_left << io::line;
	for (typename matrix4x4<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, matrix4x4<T>& v)
{
	is.ignore();
	for (typename matrix4x4<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __MATH_MATRIX4X4_H__
