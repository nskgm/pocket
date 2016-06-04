#ifndef __MATH_MATRIX4X4_H__
#define __MATH_MATRIX4X4_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix3x3.h"
#ifdef _USE_SIMD_ANONYMOUS
#include "SIMD.h"
#endif // _USE_SIMD_ANONYMOUS
#ifdef _USE_CXX11
#include <initializer_list>
#endif // _USE_CXX11
#ifdef _USING_MATH_IO
#include <iostream>
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct Matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Matrix4x4<float> Matrix4x4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Matrix4x4<double> Matrix4x4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Matrix4x4<long double> Matrix4x4ld;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USE_CXX11
template <typename T>
using mat4x4 = Matrix4x4<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using mat4x4f = mat4x4<float>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using mat4x4d = mat4x4<double>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using mat4x4ld = mat4x4<long double>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T>
struct Matrix4x4
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*-----------------------------------------------------------------------------------------
	* Types
	*-----------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef Vector4<T> vector4_type;
	typedef Vector4<T> row_type;
	typedef Vector4<T> column_type;

#ifdef _USE_SIMD_ANONYMOUS
	typedef simd_t<T> simd_type;
	typedef typename simd_type::type simd_value_type;
#endif // _USE_SIMD_ANONYMOUS

	typedef container::array<Vector4<T>, 4> array4x4_type;
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

	typedef Vector4<T>(*CalcFunc)(const Vector4<T>&, const Vector4<T>&);

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
		// 1行ごと
		struct
		{
			row_type MV0;
			row_type MV1;
			row_type MV2;
			row_type MV3;
		};
		// 行列内ベクトル
		struct
		{
			Vector3<T> Right; // X方向ベクトル
			T _RightW; // X方向ベクトルW
			Vector3<T> Up; // Y方向ベクトル
			T _UpW; // Y方向ベクトルW
			Vector3<T> Forward; // Z方向ベクトル
			T _ForwardW; // Z方向ベクトルW
			Vector3<T> Position; // 座標ベクトル
			T _PositionW; // 座標ベクトルW
		};
		// それぞれの要素へアクセス
		struct
		{
			T M11; T M12; T M13; T M14;
			T M21; T M22; T M23; T M24;
			T M31; T M32; T M33; T M34;
			T M41; T M42; T M43; T M44;
		};
		array16_type Data;
	};
#endif // _USE_ANONYMOUS_NON_POD

	template <typename> friend struct Matrix4x4;

	/*-----------------------------------------------------------------------------------------
	* Constants
	*-----------------------------------------------------------------------------------------*/

	static const Matrix4x4 Zero; // 0.0 x 16
	static const Matrix4x4 Identity; // 単位行列

	/*-----------------------------------------------------------------------------------------
	* Constructors
	*-----------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Matrix4x4);
	explicit Matrix4x4(const behavior::_noinitialize_t&)
	{

	}
	explicit Matrix4x4(const behavior::_zero_t&)
	{
		M[0] = Vector4<T>::Zero;
		M[1] = Vector4<T>::Zero;
		M[2] = Vector4<T>::Zero;
		M[3] = Vector4<T>::Zero;
	}
	explicit Matrix4x4(const behavior::_identity_t&)
	{
		M[0] = Vector4<T>::UnitX;
		M[1] = Vector4<T>::UnitY;
		M[2] = Vector4<T>::UnitZ;
		M[3] = Vector4<T>::UnitW;
	}
	explicit Matrix4x4(T t)
	{
		M[0] = Vector4<T>(t, math_type::Zero, math_type::Zero, math_type::Zero);
		M[1] = Vector4<T>(math_type::Zero, t, math_type::Zero, math_type::Zero);
		M[2] = Vector4<T>(math_type::Zero, math_type::Zero, t, math_type::Zero);
		M[3] = Vector4<T>(math_type::Zero, math_type::Zero, math_type::Zero, t);
	}
	Matrix4x4(T M11, T M12, T M13, T M14,
		T M21, T M22, T M23, T M24,
		T M31, T M32, T M33, T M34,
		T M41, T M42, T M43, T M44)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		MV0.X = M11;
		MV0.Y = M12;
		MV0.Z = M13;
		MV0.W = M14;
		MV1.X = M21;
		MV1.Y = M22;
		MV1.Z = M23;
		MV1.W = M24;
		MV2.X = M31;
		MV2.Y = M32;
		MV2.Z = M33;
		MV2.W = M34;
		MV3.X = M41;
		MV3.Y = M42;
		MV3.Z = M43;
		MV3.W = M44;
#else
		M[0] = Vector4<T>(M11, M12, M13, M14);
		M[1] = Vector4<T>(M21, M22, M23, M24);
		M[2] = Vector4<T>(M31, M32, M33, M34);
		M[3] = Vector4<T>(M41, M42, M43, M44);
#endif // _USE_ANONYMOUS_NON_POD
	}
	Matrix4x4(const Vector4<T>& M1, const Vector4<T>& M2, const Vector4<T>& M3, const Vector4<T>& M4)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		MV0 = M1;
		MV1 = M2;
		MV2 = M3;
		MV3 = M4;
#else
		M[0] = M1;
		M[1] = M2;
		M[2] = M3;
		M[3] = M4;
#endif // _USE_ANONYMOUS_NON_POD
	}
	explicit Matrix4x4(const Vector3<T>& M1, T M1W,
		const Vector3<T>& M2, T M2W,
		const Vector3<T>& M3, T M3W,
		const Vector3<T>& M4, T M4W = math_type::One)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Right = M1;
		_RightW = M1W;
		Up = M2;
		_UpW = M2W;
		Forward = M3;
		_ForwardW = M3W;
		Position = M4;
		_PositionW = M4W;
#else
		Vector4<T>* p = &M[0];
		p->X = M1.X;
		p->Y = M1.Y;
		p->Z = M1.Z;
		p->W = M1W;

		p = &M[1];
		p->X = M2.X;
		p->Y = M2.Y;
		p->Z = M2.Z;
		p->W = M2W;

		p = &M[2];
		p->X = M3.X;
		p->Y = M3.Y;
		p->Z = M3.Z;
		p->W = M3W;

		p = &M[3];
		p->X = M4.X;
		p->Y = M4.Y;
		p->Z = M4.Z;
		p->W = M4W;
#endif // _USE_ANONYMOUS_NON_POD
	}
	explicit Matrix4x4(const Matrix3x3<T>& m)
	{
		const Vector3<T>* m0 = &m[0];
		const Vector3<T>* m1 = &m[1];
		const Vector3<T>* m2 = &m[2];
		Vector4<T>* p0 = &M[0];
		Vector4<T>* p1 = &M[1];
		Vector4<T>* p2 = &M[2];

		p0->X = m0->X;
		p0->Y = m0->Y;
		p0->Z = m0->Z;
		p0->W = math_type::Zero;

		p1->X = m1->X;
		p1->Y = m1->Y;
		p1->Z = m1->Z;
		p1->W = math_type::Zero;

		p2->X = m2->X;
		p2->Y = m2->Y;
		p2->Z = m2->Z;
		p2->W = math_type::Zero;

		M[3] = Vector4<T>::UnitW;
	}

	/*-----------------------------------------------------------------------------------------
	* Functions
	*-----------------------------------------------------------------------------------------*/

	template <typename FUNC>
	Matrix4x4& each_calc_line(const Matrix4x4& m, Matrix4x4& result, FUNC func) const
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
	Matrix4x4& add(const Matrix4x4& m, Matrix4x4& result) const
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
	Matrix4x4& subtract(const Matrix4x4& m, Matrix4x4& result) const
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
	Matrix4x4& multiply(const Matrix4x4& m, Matrix4x4& result) const
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
		const_iterator mi = m.M.begin();
		const_iterator j;
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			simd_value_type mx = simd_type::template permute<0>(i->mm);
			simd_value_type my = simd_type::template permute<1>(i->mm);
			simd_value_type mz = simd_type::template permute<2>(i->mm);
			simd_value_type mw = simd_type::template permute<3>(i->mm);

			j = mi;
			mx = simd_type::mul(mx, j->mm);
			++j;
			my = simd_type::mul(my, j->mm);
			++j;
			mz = simd_type::mul(mz, j->mm);
			++j;
			mw = simd_type::mul(mw, j->mm);

			mx = simd_type::add(mx, mz);
			my = simd_type::add(my, mw);

			ri->mm = simd_type::add(mx, my);
		}
#else // _USE_SIMD_ANONYMOUS

#	if defined(__DEBUG)
		// コピー&転置をして計算しやすいようにする
		Matrix4x4 t = m.transposed();
		iterator ri = result.M.begin();
		const_iterator ti = t.M.begin(), j;
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			j = ti;
			ri->X = i->dot(*j);
			++j;
			ri->Y = i->dot(*j);
			++j;
			ri->Z = i->dot(*j);
			++j;
			ri->W = i->dot(*j);
		}
#	else
		const_pointer oi0 = &m.M[0];
		const_pointer oi1 = &m.M[1];
		const_pointer oi2 = &m.M[2];
		const_pointer oi3 = &m.M[3];
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->X = i->X * oi0->X + i->Y * oi1->X + i->Z * oi2->X + i->W * oi3->X;
			ri->Y = i->X * oi0->Y + i->Y * oi1->Y + i->Z * oi2->Y + i->W * oi3->Y;
			ri->Z = i->X * oi0->Z + i->Y * oi1->Z + i->Z * oi2->Z + i->W * oi3->Z;
			ri->W = i->X * oi0->W + i->Y * oi1->W + i->Z * oi2->W + i->W * oi3->W;
		}
#	endif
#endif // _USE_SIMD_ANONYMOUS
		return result;
	}
	Matrix4x4& multiply(T s, Matrix4x4& result) const
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
	Matrix4x4& divide(T s, Matrix4x4& result) const
	{
#ifdef _USE_SIMD_ANONYMOUS
		simd_value_type rcp = simd_type::set(math_type::reciprocal(s));
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->mm = simd_type::mul(i->mm, rcp);
		}
#else
		_DEB_ASSERT(s != math_type::Zero);
		s = math_type::One / s;
		return multiply(s, result);
#endif // _USE_SIMD_ANONYMOUS
	}

	/*---------------------------------------------------------------------
	* 値か近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Matrix4x4& m) const
	{
		return (M[0].is_near(m.M[0]) && M[1].is_near(m.M[1]) && M[2].is_near(m.M[2]) && M[3].is_near(m.M[3]));
	}
	/*---------------------------------------------------------------------
	* 値がゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (M[0].is_near_zero() && M[1].is_near_zero() && M[2].is_near_zero() && M[3].is_near_zero());
	}
	/*---------------------------------------------------------------------
	* 値がすべてゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (M[0].is_zero() && M[1].is_zero() && M[2].is_zero() && M[3].is_zero());
	}

	/*---------------------------------------------------------------------
	* 値が単位行列と近いか
	*---------------------------------------------------------------------*/
	bool is_near_identity() const
	{
		return (M[0].is_near(Vector4<T>::UnitX) &&
			M[1].is_near(Vector4<T>::UnitY) &&
			M[2].is_near(Vector4<T>::UnitZ) &&
			M[3].is_near(Vector4<T>::UnitW));
	}

	/*---------------------------------------------------------------------
	* 右ベクトル取得・設定
	*---------------------------------------------------------------------*/
	const Vector3<T>& right() const
	{
		//return Vector3<T>(m.X, m.Y, m.Z);
#ifdef _USE_ANONYMOUS_NON_POD
		return Right;
#else
		const Vector4<T>& m = M[0];
		return reinterpret_cast<const Vector3<T>&>(m);
#endif // _USE_ANONYMOUS_NON_POD
	}
	Matrix4x4& right(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Right = v;
#else
		Vector4<T>& m = M[0];
		m.X = v.X;
		m.Y = v.Y;
		m.Z = v.Z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	/*---------------------------------------------------------------------
	* 上ベクトル取得・設定
	*---------------------------------------------------------------------*/
	const Vector3<T>& up() const
	{
		//return Vector3<T>(m.X, m.Y, m.Z);
#ifdef _USE_ANONYMOUS_NON_POD
		return Up;
#else
		const Vector4<T>& m = M[1];
		return reinterpret_cast<const Vector3<T>&>(m);
#endif // _USE_ANONYMOUS_NON_POD
	}
	Matrix4x4& up(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Up = v;
#else
		Vector4<T>& m = M[1];
		m.X = v.X;
		m.Y = v.Y;
		m.Z = v.Z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	/*---------------------------------------------------------------------
	* 前ベクトル取得・設定
	*---------------------------------------------------------------------*/
	const Vector3<T>& forward() const
	{
		//return Vector3<T>(m.X, m.Y, m.Z);
#ifdef _USE_ANONYMOUS_NON_POD
		return Forward;
#else
		const Vector4<T>& m = M[2];
		return reinterpret_cast<const Vector3<T>&>(m);
#endif // _USE_ANONYMOUS_NON_POD
	}
	Matrix4x4& forward(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Forward = v;
#else
		Vector4<T>& m = M[2];
		m.X = v.X;
		m.Y = v.Y;
		m.Z = v.Z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	/*---------------------------------------------------------------------
	* 座標取得・設定
	*---------------------------------------------------------------------*/
	const Vector3<T>& position() const
	{
		// Vector3<T>(m.X, m.Y, m.Z);
#ifdef _USE_ANONYMOUS_NON_POD
		return Position;
#else
		const Vector4<T>& m = M[3];
		return reinterpret_cast<const Vector3<T>&>(m);
#endif // _USE_ANONYMOUS_NON_POD
	}
	Matrix4x4& position(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Position = v;
#else
		Vector4<T>& m = M[3];
		m.X = v.X;
		m.Y = v.Y;
		m.Z = v.Z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}

	/*---------------------------------------------------------------------
	* 拡大縮小取得
	*---------------------------------------------------------------------*/
	Vector3<T> scale() const
	{
		const Vector3<T>& r = right();
		const Vector3<T>& u = up();
		const Vector3<T>& f = forward();
		return Vector3<T>(r.length(), u.length(), f.length());
	}
	Vector3<T>& scale(Vector3<T>& result) const
	{
		const Vector3<T>& r = right();
		const Vector3<T>& u = up();
		const Vector3<T>& f = forward();
		result.X = r.length();
		result.Y = u.length();
		result.Z = f.length();
		return result;
	}

	/*---------------------------------------------------------------------
	* X軸回転量を求める
	*---------------------------------------------------------------------*/
	T pitch() const
	{
		// 前ベクトルのY軸の状態から計算する
		return forward().pitch();
	}
	/*---------------------------------------------------------------------
	* Y軸回転量を求める
	*---------------------------------------------------------------------*/
	T yaw() const
	{
		return forward().yaw();
	}
	/*---------------------------------------------------------------------
	* Z軸回転量を求める
	*---------------------------------------------------------------------*/
	T roll() const
	{
		return math_type::atan2(M[0].Y, M[1].Y);
	}

	/*---------------------------------------------------------------------
	* すべての要素をゼロにする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_zero()
	{
#if 0
		M[0] = Vector4<T>::Zero;
		M[1] = Vector4<T>::Zero;
		M[2] = Vector4<T>::Zero;
		M[3] = Vector4<T>::Zero;
		//M.fill(Vector4<T>::Zero);
#endif

		*this = Matrix4x4::Zero;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 単位行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_identity()
	{
#if 0
		M[0] = Vector4<T>::UnitX;
		M[1] = Vector4<T>::UnitY;
		M[2] = Vector4<T>::UnitZ;
		M[3] = Vector4<T>::UnitW;
		//std::memcpy(this, &Matrix4x4::Identity, sizeof(*this));
#endif

		*this = Matrix4x4::Identity;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_scale(T x, T y, T z)
	{
		load_identity();
		// 11から斜めの方向へ三つの要素が拡大縮小を扱う要素
		M[0].X = x;
		M[1].Y = y;
		M[2].Z = z;
		return *this;
	}
	Matrix4x4& load_scale(const Vector3<T>& v)
	{
		return load_scale(v.X, v.Y, v.Z);
	}
	Matrix4x4& load_scale(T s)
	{
		return load_scale(s, s, s);
	}

	/*---------------------------------------------------------------------
	* X軸回転行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_x(T r)
	{
		load_identity();
		// サインとコサインを角度から求める
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector4<T>* p = &M[1];
		p->Y = C;
		p->Z = S;
		p = &M[2];
		p->Y = -S;
		p->Z = C;
		return *this;
	}
	/*---------------------------------------------------------------------
	* Y軸回転行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_y(T r)
	{
		load_identity();
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector4<T>* p = &M[0];
		p->X = C;
		p->Z = -S;
		p = &M[2];
		p->X = S;
		p->Z = C;
		return *this;
	}
	/*---------------------------------------------------------------------
	* Z軸回転行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_z(T r)
	{
		load_identity();
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector4<T>* p = &M[0];
		p->X = C;
		p->Y = S;
		p = &M[1];
		p->X = -S;
		p->Y = C;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（ZXY）
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_roll_pitch_yaw(T roll, T pitch, T yaw)
	{
		T rs = math_type::sin(roll);
		T rc = math_type::cos(roll);
		T ps = math_type::sin(pitch);
		T pc = math_type::cos(pitch);
		T ys = math_type::sin(yaw);
		T yc = math_type::cos(yaw);

		Vector4<T>* p = &M[0];
		p->X = rc * yc + rs * ps * ys;
		p->Y = rs * pc;
		p->Z = rc * -ys + rs * ps * yc;
		p->W = math_type::Zero;

		p = &M[1];
		p->X = -rs * yc + rc * ps * ys;
		p->Y = rc * pc;
		p->Z = rs * ys + rc * ps * yc;
		p->W = math_type::Zero;

		p = &M[2];
		p->X = pc * ys;
		p->Y = -ps;
		p->Z = pc * yc;
		p->W = math_type::Zero;

		M[3] = Vector4<T>::UnitW;

		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（クォータニオン）
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_quaternion(const Quaternion<T>& q)
	{
		// 掛け合わせたもの, 複数使うため
		T xx = math_type::Two * math_type::sqr(q.X);
		T yy = math_type::Two * math_type::sqr(q.Y);
		T zz = math_type::Two * math_type::sqr(q.Z);
		T xy = math_type::Two * q.X * q.Y;
		T xz = math_type::Two * q.X * q.Z;
		T yz = math_type::Two * q.Y * q.Z;
		T wx = math_type::Two * q.W * q.X;
		T wy = math_type::Two * q.W * q.Y;
		T wz = math_type::Two * q.W * q.Z;

		Vector4<T>* p = &M[0];
		p->X = math_type::One - (yy + zz);
		p->Y = xy + wz;
		p->Z = xz - wy;
		p->W = math_type::Zero;

		p = &M[1];
		p->X = xy - wz;
		p->Y = math_type::One - (xx + zz);
		p->Z = yz + wz;
		p->W = math_type::Zero;

		p = &M[2];
		p->X = xz + wy;
		p->Y = yz + wx;
		p->Z = math_type::One - (xx + yy);
		p->W = math_type::Zero;

		M[3] = Vector4<T>::UnitW;

		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（任意軸＋角度）
	*---------------------------------------------------------------------*/
	Matrix4x4& load_rotate_axis_angle(const Vector3<T>& axis, T angle)
	{
		// 四元数での計算を行なう
		return load_rotate_quaternion(Quaternion<T>(axis, angle));
	}
	/*---------------------------------------------------------------------
	* 座標変換行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_translate(T x, T y, T z)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		Position.X = x;
		Position.Y = y;
		Position.Z = z;
#else
		// ４の位置が座標を扱う要素
		Vector4<T>* p = &M[3];
		p->X = x;
		p->Y = y;
		p->Z = z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	Matrix4x4& load_translate(const Vector3<T>& v)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		Position = v;
#else
		Vector4<T>* p = &M[3];
		p->X = v.X;
		p->Y = v.Y;
		p->Z = v.Z;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	/*---------------------------------------------------------------------
	* ワールド変換行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_world(const Vector3<T>& scale, T roll, T pitch, T yaw, const Vector3<T>& position)
	{
		// [Scale * RotateZ * RotateX * RotateY * Translate]を展開

		T rs = math_type::sin(roll);
		T rc = math_type::cos(roll);
		T ps = math_type::sin(pitch);
		T pc = math_type::cos(pitch);
		T ys = math_type::sin(yaw);
		T yc = math_type::cos(yaw);

		Vector4<T>* p = &M[0];
		p->X = scale.X * rc * yc + scale.X * rs * ps * ys;
		p->Y = scale.X * rs * pc;
		p->Z = scale.X * rc * -ys + scale.X * rs * ps * yc;
		p->W = math_type::Zero;

		p = &M[1];
		p->X = scale.Y * -rs * yc + scale.Y * rc * ps * ys;
		p->Y = scale.Y * rc * pc;
		p->Z = scale.Y * rs * ys + scale.Y * rc * ps * yc;
		p->W = math_type::Zero;

		p = &M[2];
		p->X = scale.Z * pc * ys;
		p->Y = scale.Z * -ps;
		p->Z = scale.Z * pc * yc;
		p->W = math_type::Zero;

		p = &M[3];
		p->X = position.X;
		p->Y = position.Y;
		p->Z = position.Z;
		p->W = math_type::One;

		return *this;
	}
	Matrix4x4& load_world(T scale, T roll, T pitch, T yaw, const Vector3<T>& position)
	{
		return load_world(Vector3<T>(scale), roll, pitch, yaw, position);
	}
	Matrix4x4& load_world(const Vector3<T>& scale, const Quaternion<T>& rotate, const Vector3<T>& position)
	{
		// [Scale * Quaternion * Translate]を展開

		T xx = math_type::Two * math_type::sqr(rotate.X);
		T yy = math_type::Two * math_type::sqr(rotate.Y);
		T zz = math_type::Two * math_type::sqr(rotate.Z);
		T xy = math_type::Two * rotate.X * rotate.Y;
		T xz = math_type::Two * rotate.X * rotate.Z;
		T yz = math_type::Two * rotate.Y * rotate.Z;
		T wx = math_type::Two * rotate.W * rotate.X;
		T wy = math_type::Two * rotate.W * rotate.Y;
		T wz = math_type::Two * rotate.W * rotate.Z;

		Vector4<T>* p = &M[0];
		p->X = scale.X * (math_type::One - (yy + zz));
		p->Y = scale.X * (xy + wz);
		p->Z = scale.X * (xz - wy);
		p->W = math_type::Zero;

		p = &M[1];
		p->X = scale.Y * (xy - wz);
		p->Y = scale.Y * (math_type::One - (xx + zz));
		p->Z = scale.Y * (yz + wz);
		p->W = math_type::Zero;

		p = &M[2];
		p->X = scale.Z * (xz + wy);
		p->Y = scale.Z * (yz + wx);
		p->Z = scale.Z * (math_type::One - (xx + yy));
		p->W = math_type::Zero;

		p = &M[3];
		p->X = position.X;
		p->Y = position.Y;
		p->Z = position.Z;
		p->W = math_type::One;

		return *this;
	}
	Matrix4x4& load_world(T scale, const Quaternion<T>& rotate, const Vector3<T>& position)
	{
		return load_world(Vector3<T>(scale), rotate, position);
	}
	/*---------------------------------------------------------------------
	* 射影変換行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_perspective_field_of_view(T fovy, T aspect, T near, T far)
	{
		load_identity();

		T t = math_type::One / math_type::tan(fovy * math_type::Half);

		Vector4<T>* p = &M[0];
		p->X = t / aspect;

		p = &M[1];
		p->Y = t;

		t = math_type::One / (far - near);

		p = &M[2];
		p->Z = (far + near) * t;
		p->W = -math_type::One;

		// 座標
		p = &M[3];
		p->Z = (math_type::Two * far * near) * t;
		p->W = math_type::Zero;

		return *this;
	}
	Matrix4x4& load_perspective_field_of_view(T fovy, T width, T height, T near, T far)
	{
		return load_perspective_field_of_view(fovy, width / height, near, far);
	}

	/*---------------------------------------------------------------------
	* 正射影変換行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_orthographics(T left, T right, T top, T bottom, T near, T far)
	{
		load_identity();

		T dx = math_type::One / (right - left);
		T dy = math_type::One / (top - bottom);
		T dz = math_type::One / (far - near);

		Vector4<T>* p = &M[0];
		p->X = math_type::Two * dx;

		p = &M[1];
		p->Y = math_type::Two * dy;

		p = &M[2];
		p->Z = -math_type::Two * dz; // 右手特有

		p = &M[3];
		p->X = -(right + left) * dx;
		p->Y = -(bottom + top) * dy;
		p->Z = -(far + near) * dz;

		return *this;
	}
	Matrix4x4& load_orthographics(T width, T height, T near, T far)
	{
		return load_orthographics(math_type::Zero, width, math_type::Zero, height, near, far);
	}
	/*---------------------------------------------------------------------
	* 2D用正射影変換行列にする
	*---------------------------------------------------------------------*/
	Matrix4x4& load_orthographics2d(T left, T right, T top, T bottom)
	{
		load_identity();

		T dx = math_type::One / (right - left);
		T dy = math_type::One / (top - bottom);

		Vector4<T>* p = &M[0];
		p->X = math_type::Two * dx;

		p = &M[1];
		p->Y = math_type::Two * dy;

		p = &M[2];
		p->Z = -math_type::One;

		p = &M[3];
		p->X = -(right + left) * dx;
		p->Y = -(bottom + top) * dy;

		return *this;
	}
	Matrix4x4& load_orthographics2d(T width, T height)
	{
		return load_orthographics2d(math_type::Zero, width, math_type::Zero, height);
	}
	/*---------------------------------------------------------------------
	* 視野変換行列にする（座標＋向き）
	*---------------------------------------------------------------------*/
	Matrix4x4& load_lookto(const Vector3<T>& eye, const Vector3<T>& direction, const Vector3<T>& up = Vector3<T>::Up)
	{
		// カメラを原点としてカメラから見た変換を行う

		const Vector3<T>& z = direction;
		// 前方向と上方向の外積で右ベクトルを求める
		Vector3<T> x(behavior::noinitialize);
		up.cross(z, x).normalize();
		// 前と右から上方向のベクトルを求める
		Vector3<T> y(behavior::noinitialize);
		z.cross(x, y);

		// X
		Vector4<T>* p = &M[0];
		p->X = x.X;
		p->Y = y.X;
		p->Z = z.X;
		p->W = math_type::Zero;
		// Y
		p = &M[1];
		p->X = x.Y;
		p->Y = y.Y;
		p->Z = z.Y;
		p->W = math_type::Zero;
		// Z
		p = &M[2];
		p->X = x.Z;
		p->Y = y.Z;
		p->Z = z.Z;
		p->W = math_type::Zero;

		// 座標
		p = &M[3];
		p->X = -eye.dot(x);
		p->Y = -eye.dot(y);
		p->Z = -eye.dot(z);
		p->W = math_type::One;

		return *this;
	}
	Matrix4x4& load_lookto(T ex, T ey, T ez, T dx, T dy, T dz, T ux = math_type::Zero, T uy = math_type::One, T uz = math_type::Zero)
	{
		return load_lookto(Vector3<T>(ex, ey, ez), Vector3<T>(dx, dy, dz), Vector3<T>(ux, uy, uz));
	}

	/*---------------------------------------------------------------------
	* 視野変換行列にする（座標＋注視点）
	*---------------------------------------------------------------------*/
	Matrix4x4& load_lookat(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up = Vector3<T>::Up)
	{
		// 向きを求める
		return load_lookto(eye, eye.direction(center), up);
	}
	Matrix4x4& load_lookat(T ex, T ey, T ez, T cx, T cy, T cz, T ux = math_type::Zero, T uy = math_type::One, T uz = math_type::Zero)
	{
		return load_lookat(Vector3<T>(ex, ey, ez), Vector3<T>(cx, cy, cz), Vector3<T>(ux, uy, uz));
	}

	/*---------------------------------------------------------------------
	* 転置行列
	*---------------------------------------------------------------------*/
	Matrix4x4& transpose()
	{
		// 回転を扱う場合の転置は逆マトリクスと同一になる

		Vector4<T>* v0 = &M[0];
		Vector4<T>* v1 = &M[1];
		Vector4<T>* v2 = &M[2];
		Vector4<T>* v3 = &M[3];

#ifdef __DEBUG
		// [0][1] = [1][0]
		T tmp = v0->Y;
		v0->Y = v1->X;
		v1->X = tmp;
		// [0][2] = [2][0]
		tmp = v0->Z;
		v0->Z = v2->X;
		v2->X = tmp;
		// [0][3] = [3][0]
		tmp = v0->W;
		v0->W = v3->X;
		v3->X = tmp;
		// [1][2] = [2][1]
		tmp = v1->Z;
		v1->Z = v2->Y;
		v2->Y = tmp;
		// [1][3] = [3][1]
		tmp = v1->W;
		v1->W = v3->Y;
		v3->Y = tmp;
		// [2][3] = [3][2]
		tmp = v2->W;
		v2->W = v3->Z;
		v3->Z = tmp;
#else
		std::swap(v0->Y, v1->X);
		std::swap(v0->Z, v2->X);
		std::swap(v0->W, v3->X);
		std::swap(v1->Z, v2->Y);
		std::swap(v1->W, v3->Y);
		std::swap(v2->W, v3->Z);
#endif

		return *this;
	}
	/*---------------------------------------------------------------------
	* 転置行列を求める
	*---------------------------------------------------------------------*/
	Matrix4x4 transposed() const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];
		return Matrix4x4(v0->X, v1->X, v2->X, v3->X,
			v0->Y, v1->Y, v2->Y, v3->Y,
			v0->Z, v1->Z, v2->Z, v3->Z,
			v0->W, v1->W, v2->W, v3->W);
	}
	Matrix4x4& transposed(Matrix4x4& result) const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];
		Vector4<T>* r0 = &result.M[0];
		Vector4<T>* r1 = &result.M[1];
		Vector4<T>* r2 = &result.M[2];
		Vector4<T>* r3 = &result.M[3];

		// [0][1] = [1][0]
		r0->Y = v1->X;
		r1->X = v0->Y;
		// [0][2] = [2][0]
		r0->Z = v2->X;
		r2->X = v0->Z;
		// [0][3] = [3][0]
		r0->W = v3->X;
		r3->X = v0->W;
		// [1][2] = [2][1]
		r1->Z = v2->Y;
		r2->Y = v1->Z;
		// [1][3] = [3][1]
		r1->W = v3->Y;
		r3->Y = v1->W;
		// [2][3] = [3][2]
		r2->W = v3->Z;
		r3->Z = v2->W;

		r0->X = v0->X;
		r1->Y = v1->Y;
		r2->Z = v2->Z;
		r3->W = v3->W;

		return result;
	}
	/*---------------------------------------------------------------------
	* 行列式を求める
	*---------------------------------------------------------------------*/
	T determinant() const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];
#if 0
		T a = v0->X * v1->Y * v2->Z * v3->W + v0->X * v1->Z * v2->W * v3->Y + v0->X * v1->W * v2->Y * v3->Z + v0->Y * v1->X * v2->W * v3->Z +
			v0->Y * v1->Z * v2->X * v3->W + v0->Y * v1->W * v2->Z * v3->X + v0->Z * v1->X * v2->Y * v3->W + v0->Z * v1->Y * v2->W * v3->X +
			v0->Z * v1->W * v2->X * v3->Y + v0->W * v1->X * v2->Z * v3->Y + v0->W * v1->Y * v2->X * v3->Z + v0->W * v1->Z * v2->Y * v3->X;
		T b = -(v0->X * v1->Y * v2->W * v3->Z) - v0->X * v1->Z * v2->Y * v3->W - v0->X * v1->W * v2->Z * v3->Y - v0->Y * v1->X * v2->Z * v3->W -
			v0->Y * v1->Z * v2->W * v3->X - v0->Y * v1->W * v2->X * v3->Z - v0->Z * v1->X * v2->W * v3->Y - v0->Z * v1->Y * v2->X * v3->W -
			v0->Z * v1->W * v2->Y * v3->X - v0->W * v1->X * v2->Y * v3->Z - v0->W * v1->Y * v2->Z * v3->X - v0->W * v1->Z * v2->X * v3->Y;
		return (a + b);
#endif
		// 余因子展開で求める
		T a1 = ((v0->X * v1->Y * v2->Z * v3->W) + (v0->X * v1->Z * v2->W * v3->Y) + (v0->X * v1->W * v2->Y * v3->Z) -
			(v0->X * v1->W * v2->Z * v3->Y) - (v0->X * v1->Z * v2->Y * v3->W) - (v0->X * v1->Y * v2->W * v3->Z));
		T a2 = ((v1->X * v0->Y * v2->Z * v3->W) + (v1->X * v0->Z * v2->W * v3->Y) + (v1->X * v0->W * v2->Y * v3->Z) -
			(v1->X * v0->W * v2->Z * v3->Y) - (v1->X * v0->Z * v2->Y * v3->W) - (v1->X * v0->Y * v2->W * v3->Z));
		T a3 = ((v2->X * v0->Y * v1->Z * v3->W) + (v2->X * v0->Z * v1->W * v3->Y) + (v2->X * v0->W * v1->Y * v3->Z) -
			(v2->X * v0->W * v1->Z * v3->Y) - (v2->X * v0->Z * v1->Y * v3->W) - (v2->X * v0->Y * v1->W * v3->Z));
		T a4 = ((v3->X * v0->Y * v1->Z * v2->W) + (v3->X * v0->Z * v1->W * v2->Y) + (v3->X * v0->W * v1->Y * v2->Z) -
			(v3->X * v0->W * v1->Z * v2->Y) - (v3->X * v0->Z * v1->Y * v2->W) - (v3->X * v0->Y * v1->W * v2->Z));
		return (a1 - a2 + a3 - a4);
	}

	/*---------------------------------------------------------------------
	* 逆行列にする（逆行列が存在しない場合は偽）
	*---------------------------------------------------------------------*/
	Matrix4x4& inverse()
	{
		// 自身を構築するためのコピー(自身が変わるため)
		const Matrix4x4 c = *this;
		return c.inversed(*this);
	}
	/*---------------------------------------------------------------------
	* 逆行列を求める
	*---------------------------------------------------------------------*/
	Matrix4x4 inversed() const
	{
		Matrix4x4 result(behavior::noinitialize);
		return inversed(result);;
	}
	Matrix4x4& inversed(Matrix4x4& result) const
	{
		T det = determinant();
		// 逆行列が存在しない
		//if (det == math_type::Zero)
		if (math_type::is_near_zero(det))
		{
			// 単位行列
			return result.load_identity();
		}

		// 逆数にする
		det = math_type::reciprocal(det);

		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];
		Vector4<T>* r0 = &result.M[0];
		Vector4<T>* r1 = &result.M[1];
		Vector4<T>* r2 = &result.M[2];
		Vector4<T>* r3 = &result.M[3];

		// iが偶数の時は-から, iが奇数の時には+から

		// (-1)i+j |M ji| (M ji)のjiが逆なのに注意

		// +M-M+M-M
		r0->X = ((v1->Y * v2->Z * v3->W) + (v1->Z * v2->W * v3->Y) + (v1->W * v2->Y * v3->Z) -
			(v1->W * v2->Z * v3->Y) - (v1->Z * v2->Y * v3->W) - (v1->Y * v2->W * v3->Z)) * det;
		r0->Y = ((v0->Y * v2->W * v3->Z) + (v0->Z * v2->Y * v3->W) + (v0->W * v2->Z * v3->Y) -
			(v0->Y * v2->Z * v3->W) - (v0->Z * v2->W * v3->Y) - (v0->W * v2->Y * v3->Z)) * det;
		r0->Z = ((v0->Y * v1->Z * v3->W) + (v0->Z * v1->W * v3->Y) + (v0->W * v1->Y * v3->Z) -
			(v0->W * v1->Z * v3->Y) - (v0->Z * v1->Y * v3->W) - (v0->Y * v1->W * v3->Z)) * det;
		r0->W = ((v0->Y * v1->W * v2->Z) + (v0->Z * v1->Y * v2->W) + (v0->W * v1->Z * v2->Y) -
			(v0->Y * v1->Z * v2->W) - (v0->Z * v1->W * v2->Y) - (v0->W * v1->Y * v2->Z)) * det;
		// -M+M-M+M
		r1->X = ((v1->X * v2->W * v3->Z) + (v1->Z * v2->X * v3->W) + (v1->W * v2->Z * v3->X) -
			(v1->X * v2->Z * v3->W) - (v1->Z * v2->W * v3->X) - (v1->W * v2->X * v3->Z)) * det;
		r1->Y = ((v0->X * v2->Z * v3->W) + (v0->Z * v2->W * v3->X) + (v0->W * v2->X * v3->Z) -
			(v0->W * v2->Z * v3->X) - (v0->Z * v2->X * v3->W) - (v0->X * v2->W * v3->Z)) * det;
		r1->Z = ((v0->X * v1->W * v3->Z) + (v0->Z * v1->X * v3->W) + (v0->W * v1->Z * v3->X) -
			(v0->X * v1->Z * v3->W) - (v0->Z * v1->W * v3->X) - (v0->W * v1->X * v3->Z)) * det;
		r1->W = ((v0->X * v1->Z * v2->W) + (v0->Z * v1->W * v2->X) + (v0->W * v1->X * v2->Z) -
			(v0->W * v1->Z * v2->X) - (v0->Z * v1->X * v2->W) - (v0->X * v1->W * v2->Z)) * det;
		// +M-M+M-M
		r2->X = ((v1->X * v2->Y * v3->W) + (v1->Y * v2->W * v3->X) + (v1->W * v2->X * v3->Y) -
			(v1->W * v2->Y * v3->X) - (v1->Y * v2->X * v3->W) - (v1->X * v2->W * v3->Y)) * det;
		r2->Y = ((v0->X * v2->W * v3->Y) + (v0->Y * v2->X * v3->W) + (v0->W * v2->Y * v3->X) -
			(v0->W * v2->X * v3->Y) - (v0->Y * v2->W * v3->X) - (v0->X * v2->Y * v3->W)) * det;
		r2->Z = ((v0->X * v1->Y * v3->W) + (v0->Y * v1->W * v3->X) + (v0->W * v1->X * v3->Y) -
			(v0->W * v1->Y * v3->X) - (v0->Y * v1->X * v3->W) - (v0->X * v1->W * v3->Y)) * det;
		r2->W = ((v0->X * v1->W * v2->Y) + (v0->Y * v1->X * v2->W) + (v0->W * v1->Y * v2->X) -
			(v0->W * v1->X * v2->Y) - (v0->Y * v1->W * v2->X) - (v0->X * v1->Y * v2->W)) * det;
		// -M+M-M+M
		r3->X = ((v1->X * v2->Z * v3->Y) + (v1->Y * v2->X * v3->Z) + (v1->Z * v2->Y * v3->X) -
			(v1->Z * v2->X * v3->Y) - (v1->Y * v2->Z * v3->X) - (v1->X * v2->Y * v3->Z)) * det;
		r3->Y = ((v0->X * v2->Y * v3->Z) + (v0->Y * v2->Z * v3->X) + (v0->Z * v2->X * v3->Y) -
			(v0->Z * v2->Y * v3->X) - (v0->Y * v2->X * v3->Z) - (v0->X * v2->Z * v3->Y)) * det;
		r3->Z = ((v0->X * v1->Z * v3->Y) + (v0->Y * v1->X * v3->Z) + (v0->Z * v1->Y * v3->X) -
			(v0->Z * v1->X * v3->Y) - (v0->Y * v1->Z * v3->X) - (v0->X * v1->Y * v3->Z)) * det;
		r3->W = ((v0->X * v1->Y * v2->Z) + (v0->Y * v1->Z * v2->X) + (v0->Z * v1->X * v2->Y) -
			(v0->Z * v1->Y * v2->X) - (v0->Y * v1->X * v2->Z) - (v0->X * v1->Z * v2->Y)) * det;

		return true;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（W=1）
	*---------------------------------------------------------------------*/
	Vector3<T> transform(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	Vector3<T>& transform(const Vector3<T>& v, Vector3<T>& result) const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];

		result.X = (v0->X * v.X + v1->X * v.Y + v2->X * v.Z + v3->X);
		result.Y = (v0->Y * v.X + v1->Y * v.Y + v2->Y * v.Z + v3->Y);
		result.Z = (v0->Z * v.X + v1->Z * v.Y + v2->Z * v.Z + v3->Z);

		return result;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）（W=1）
	*---------------------------------------------------------------------*/
	Vector3<T> transform_coord(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return transform_coord(v, r);
	}
	Vector3<T>& transform_coord(const Vector3<T>& v, Vector3<T>& result) const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];

		T w = math_type::One / (v0->W * v.X + v1->W * v.Y + v2->W * v.Z + v3->W);
		result.X = (v0->X * v.X + v1->X * v.Y + v2->X * v.Z + v3->X) * w;
		result.Y = (v0->Y * v.X + v1->Y * v.Y + v2->Y * v.Z + v3->Y) * w;
		result.Z = (v0->Z * v.X + v1->Z * v.Y + v2->Z * v.Z + v3->Z) * w;

		return result;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換
	*---------------------------------------------------------------------*/
	Vector4<T> transform(const Vector4<T>& v) const
	{
		Vector4<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	Vector4<T>& transform(const Vector4<T>& v, Vector4<T>& result) const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];
		const Vector4<T>* v3 = &M[3];

		result.X = v0->X * v.X + v1->X * v.Y + v2->X * v.Z + v3->X * v.W;
		result.Y = v0->Y * v.X + v1->Y * v.Y + v2->Y * v.Z + v3->Y * v.W;
		result.Z = v0->Z * v.X + v1->Z * v.Y + v2->Z * v.Z + v3->Z * v.W;
		result.W = v0->W * v.X + v1->W * v.Y + v2->W * v.Z + v3->W * v.W;

		return result;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）
	*---------------------------------------------------------------------*/
	Vector4<T> transform_coord(const Vector4<T>& v) const
	{
		Vector4<T> r(behavior::noinitialize);
		return transform_coord(v, r);
	}
	Vector4<T>& transform_coord(const Vector4<T>& v, Vector4<T>& result) const
	{
		transform(v, result);

		T w = math_type::One / result.W;
		result.X *= w;
		result.Y *= w;
		result.Z *= w;

		return result;
	}

	/*---------------------------------------------------------------------
	* ベクトル座標変換（W=0）
	*---------------------------------------------------------------------*/
	Vector3<T> transform_normal(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return transform_normal(v, r);
	}
	Vector3<T>& transform_normal(const Vector3<T>& v, Vector3<T>& result) const
	{
		const Vector4<T>* v0 = &M[0];
		const Vector4<T>* v1 = &M[1];
		const Vector4<T>* v2 = &M[2];

		result.X = v0->X * v.X + v1->X * v.Y + v2->X * v.Z;
		result.Y = v0->Y * v.X + v1->Y * v.Y + v2->Y * v.Z;
		result.Z = v0->Z * v.X + v1->Z * v.Y + v2->Z * v.Z;

		return result;
	}
	/*---------------------------------------------------------------------
	* 球面線形補間を求める
	*---------------------------------------------------------------------*/
	Matrix4x4 slerp(const Matrix4x4& to, T t) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return slerp(to, t, result);
	}
	Matrix4x4& slerp(const Matrix4x4& to, T t, Matrix4x4& result) const
	{
		Vector3<T> s(behavior::noinitialize);
		Vector3<T> p(behavior::noinitialize);
		Quaternion<T> r(behavior::noinitialize);

		// 補間用の値取得
		Vector3<T> s0(behavior::noinitialize), s1(behavior::noinitialize);
		scale(s0);
		to.scale(s1);
		// 回転取得
		Quaternion<T> q0(*this), q1(to);

		// 補間を行なう
		s0.lerp(s1, t, s);
		q0.slerp(q1, t, r);
		position().lerp(to.position(), t, p);

		// ワールド変換行列
		return result.load_world(s, r, p);
	}
	Matrix4x4& slerp(const Matrix4x4& from, const Matrix4x4& to, T t)
	{
		return from.slerp(to, t, *this);
	}

	/*-----------------------------------------------------------------------------------------
	* Operators
	*-----------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	Vector4<T>& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
		return M[i];
	}
	const Vector4<T>& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
		return M[i];
	}
	T& operator [] (const MatrixPoint& p)
	{
		_DEB_RANGE_ASSERT(p.X, 0, 3);
		_DEB_RANGE_ASSERT(p.Y, 0, 3);
		return (*this)(p.Y, p.X);
	}
	const T& operator [] (const MatrixPoint& p) const
	{
		_DEB_RANGE_ASSERT(p.X, 0, 3);
		_DEB_RANGE_ASSERT(p.Y, 0, 3);
		return (*this)(p.Y, p.X);
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
	_CXX11_EXPLICIT operator Matrix4x4<U>() const
	{
		return Matrix4x4<U>(
			static_cast<Vector4<U> >(M[0]),
			static_cast<Vector4<U> >(M[1]),
			static_cast<Vector4<U> >(M[2]),
			static_cast<Vector4<U> >(M[3]));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &M[0].X;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &M[0].X;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Matrix4x4& v) const
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
	bool operator != (const Matrix4x4& v) const
	{
		return !(*this == v);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Matrix4x4 operator + () const
	{
		return *this;
	}
	Matrix4x4 operator - () const
	{
		return Matrix4x4(-M[0], -M[1], -M[2], -M[3]);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Matrix4x4 operator + (const Matrix4x4& m) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return add(m, result);
	}
	Matrix4x4 operator - (const Matrix4x4& m) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return subtract(m, result);
	}
	Matrix4x4 operator * (const Matrix4x4& m) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return multiply(m, result);
	}
	Matrix4x4 operator * (T f) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return multiply(f, result);
	}
	Matrix4x4 operator / (T f) const
	{
		Matrix4x4 result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Matrix4x4& operator = (const behavior::_zero_t&)
	{
		return load_zero();
	}
	Matrix4x4& operator = (const behavior::_identity_t&)
	{
		return load_identity();
	}
#ifdef _USE_CXX11
	Matrix4x4& operator = (const std::initializer_list<T>& t)
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
	Matrix4x4& operator += (const Matrix4x4& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i += *oi;
		}
		return *this;
	}
	Matrix4x4& operator -= (const Matrix4x4& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i -= *oi;
		}
		return *this;
	}
	Matrix4x4& operator *= (const Matrix4x4& m)
	{
		const Matrix4x4 c(*this);
		c.multiply(m, *this);
		return *this;
	}
	Matrix4x4& operator *= (T s)
	{
		for (iterator i = M.begin(), end = M.end(); i != end; ++i)
		{
			i->X *= s;
			i->Y *= s;
			i->Z *= s;
			i->W *= s;
		}
		return *this;
	}
	Matrix4x4& operator /= (T s)
	{
		_DEB_ASSERT(s != math_type::Zero);
		s = math_type::One / s;
		return operator*=(s);
	}

	/*------------------------------------------------------------------------------------------
	* タグでの関数呼び出し
	*------------------------------------------------------------------------------------------*/

	Matrix4x4& operator () (const behavior::_zero_t&)
	{
		return load_zero();
	}
	Matrix4x4& operator () (const behavior::_identity_t&)
	{
		return load_identity();
	}

	Matrix4x4 operator () (const behavior::_plus_t&) const
	{
		return operator+();
	}
	Matrix4x4 operator () (const behavior::_negate_t&) const
	{
		return operator-();
	}
	Matrix4x4 operator () (const behavior::_add_t&, const Matrix4x4& m) const
	{
		return operator+(m);
	}
	Matrix4x4 operator () (const behavior::_sub_t&, const Matrix4x4& m) const
	{
		return operator-(m);
	}
	Matrix4x4 operator () (const behavior::_mul_t&, T f) const
	{
		return operator*(f);
	}
	Matrix4x4 operator () (const behavior::_mul_t&, const Matrix4x4& m) const
	{
		return operator*(m);
	}
	Matrix4x4 operator () (const behavior::_div_t&, T f) const
	{
		return operator/(f);
	}

	Matrix4x4& operator () (const behavior::_add_assign_t&, const Matrix4x4& m)
	{
		return operator+=(m);
	}
	Matrix4x4& operator () (const behavior::_sub_assign_t&, const Matrix4x4& m)
	{
		return operator-=(m);
	}
	Matrix4x4& operator () (const behavior::_mul_assign_t&, T f)
	{
		return operator*=(f);
	}
	Matrix4x4& operator () (const behavior::_mul_assign_t&, const Matrix4x4& m)
	{
		return operator*=(m);
	}
	Matrix4x4& operator () (const behavior::_div_assign_t&, T f)
	{
		return operator/=(f);
	}
	Vector4<T>& operator () (const behavior::_at_t&, int i)
	{
		return operator[](i);
	}
	const Vector4<T>& operator () (const behavior::_at_t&, int i) const
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
	T& operator () (const behavior::_at_t&, const MatrixPoint& p)
	{
		return operator[](p);
	}
	const T& operator () (const behavior::_at_t&, const MatrixPoint& p) const
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

	bool operator () (const behavior::_equal_t&, const Matrix4x4& m) const
	{
		return operator==(m);
	}
	bool operator () (const behavior::_not_equal_t&, const Matrix4x4& m) const
	{
		return operator!=(m);
	}
	bool operator () (const behavior::_near_t&, const Matrix4x4& m) const
	{
		return is_near(m);
	}
	bool operator () (const behavior::_near_zero_t&) const
	{
		return is_near_zero();
	}

	Vector3<T> operator () (const behavior::_scale_t&) const
	{
		return scale();
	}
	Vector3<T>& operator () (const behavior::_right_t&)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return Right;
#else
		return reinterpret_cast<Vector3<T>&>(M[0]);
#endif // _USE_ANONYMOUS_NON_POD
	}
	const Vector3<T>& operator () (const behavior::_right_t&) const
	{
		return (*this)(behavior::right);
	}
	Vector3<T>& operator () (const behavior::_up_t&)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return Up;
#else
		return reinterpret_cast<Vector3<T>&>(M[1]);
#endif // _USE_ANONYMOUS_NON_POD
	}
	const Vector3<T>& operator () (const behavior::_up_t&) const
	{
		return (*this)(behavior::up);
	}
	Vector3<T>& operator () (const behavior::_front_t&)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return Forward;
#else
		return reinterpret_cast<Vector3<T>&>(M[2]);
#endif // _USE_ANONYMOUS_NON_POD
	}
	const Vector3<T>& operator () (const behavior::_front_t&) const
	{
		return (*this)(behavior::front);
	}
	Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_x_t&)
	{
		return (*this)(behavior::right);
	}
	const Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_x_t&) const
	{
		return (*this)(behavior::right);
	}
	Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_y_t&)
	{
		return (*this)(behavior::up);
	}
	const Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_y_t&) const
	{
		return (*this)(behavior::up);
	}
	Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_z_t&)
	{
		return (*this)(behavior::front);
	}
	const Vector3<T>& operator () (const behavior::_axis_t&, const behavior::_z_t&) const
	{
		return (*this)(behavior::front);
	}
	Vector3<T>& operator () (const behavior::_position_t&)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return Position;
#else
		return reinterpret_cast<Vector3<T>&>(M[3]);
#endif // _USE_ANONYMOUS_NON_POD
	}
	const Vector3<T>& operator () (const behavior::_position_t&) const
	{
		return (*this)(behavior::position);
	}

	Matrix4x4& operator () (const behavior::_scale_t&, const Vector3<T>& v)
	{
		return load_scale(v);
	}
	Matrix4x4& operator () (const behavior::_scale_t&, T x, T y, T z)
	{
		return load_scale(x, y, z);
	}
	Matrix4x4& operator () (const behavior::_scale_t&, T s)
	{
		return load_scale(s);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_x_t&, T angle)
	{
		return load_rotate_x(angle);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_y_t&, T angle)
	{
		return load_rotate_y(angle);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_z_t&, T angle)
	{
		return load_rotate_z(angle);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const behavior::_roll_pitch_yaw_t&, T roll, T pitch, T yaw)
	{
		return load_rotate_roll_pitch_yaw(roll, pitch, yaw);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const Quaternion<T>& q)
	{
		return load_rotate_quaternion(q);
	}
	Matrix4x4& operator () (const behavior::_rotate_t&, const Vector3<T>& axis, T angle)
	{
		return load_rotate_axis_angle(axis, angle);
	}
	Matrix4x4& operator () (const behavior::_translate_t&, const Vector3<T>& t)
	{
		return load_translate(t);
	}
	Matrix4x4& operator () (const behavior::_translate_t&, T x, T y, T z)
	{
		return load_translate(x, y, z);
	}
	Matrix4x4& operator () (const behavior::_world_t&, const Vector3<T>& s, T roll, T pitch, T yaw, const Vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	Matrix4x4& operator () (const behavior::_world_t&, T s, T roll, T pitch, T yaw, const Vector3<T>& p)
	{
		return load_world(s, roll, pitch, yaw, p);
	}
	Matrix4x4& operator () (const behavior::_world_t&, const Vector3<T>& s, const Quaternion<T>& q, const Vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	Matrix4x4& operator () (const behavior::_world_t&, T s, const Quaternion<T>& q, const Vector3<T>& p)
	{
		return load_world(s, q, p);
	}
	Matrix4x4& operator () (const behavior::_perspective_field_of_view_t&, T fovy, T aspect, T n, T f)
	{
		return load_perspective_field_of_view(fovy, aspect, n, f);
	}
	Matrix4x4& operator () (const behavior::_orthographics_t&, T left, T right, T up, T bottom, T n, T f)
	{
		return load_orthographics(left, right, up, bottom, n, f);
	}
	Matrix4x4& operator () (const behavior::_orthographics_t&, T width, T height, T n, T f)
	{
		return load_orthographics(width, height, n, f);
	}
	Matrix4x4& operator () (const behavior::_orthographics2d_t&, T left, T right, T up, T bottom)
	{
		return load_orthographics2d(left, right, up, bottom);
	}
	Matrix4x4& operator () (const behavior::_orthographics2d_t&, T width, T height)
	{
		return load_orthographics2d(width, height);
	}
	Matrix4x4& operator () (const behavior::_look_to_t&, const Vector3<T>& eye, const Vector3<T>& to, const Vector3<T>& up = Vector3<T>::Up)
	{
		return load_lookto(eye, to, up);
	}
	Matrix4x4& operator () (const behavior::_look_to_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::Zero, T uy = math_type::One, T uz = math_type::Zero)
	{
		return load_lookto(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	Matrix4x4& operator () (const behavior::_look_at_t&, const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up = Vector3<T>::Up)
	{
		return load_lookat(eye, center, up);
	}
	Matrix4x4& operator () (const behavior::_look_at_t&, T ex, T ey, T ez, T tx, T ty, T tz, T ux = math_type::Zero, T uy = math_type::One, T uz = math_type::Zero)
	{
		return load_lookat(ex, ey, ez, tx, ty, tz, ux, uy, uz);
	}
	Matrix4x4& operator () (const behavior::_transpose_t&)
	{
		return transpose();
	}
	Matrix4x4 operator () (const behavior::_transposed_t&) const
	{
		return transposed();
	}
	T operator () (const behavior::_determinant_t&) const
	{
		return determinant();
	}
	Matrix4x4 operator () (const behavior::_inverse_t&) const
	{
		return inversed();
	}
	Matrix4x4 operator () (const behavior::_slerp_t&, const Matrix4x4& m, T t) const
	{
		return slerp(m, t);
	}
	Vector3<T> operator () (const behavior::_transform_t&, const Vector3<T>& v) const
	{
		return transform(v);
	}
	Vector3<T> operator () (const behavior::_transform_coord_t&, const Vector3<T>& v) const
	{
		return transform_coord(v);
	}
	Vector3<T> operator () (const behavior::_transform_normal_t&, const Vector3<T>& v) const
	{
		return transform_normal(v);
	}
	Vector4<T> operator () (const behavior::_transform_t&, const Vector4<T>& v) const
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
const Matrix4x4<T> Matrix4x4<T>::Zero(math_type::Zero);
template <typename T>
const Matrix4x4<T> Matrix4x4<T>::Identity(math_type::One);

/*---------------------------------------------------------------------
* Vector3.transform
*---------------------------------------------------------------------*/
template <typename T> inline
Vector3<T>& Vector3<T>::transform(const Matrix4x4<T>& m)
{
	Vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector3<T> Vector3<T>::transformed(const Matrix4x4<T>& m) const
{
	Vector3<T> v(behavior::noinitialize);
	return m.transform(*this, v);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transformed(const Matrix4x4<T>& m, Vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transform_coord(const Matrix4x4<T>& m)
{
	Vector3<T> v = *this;
	return m.transform_coord(v, *this);
}
template <typename T> inline
Vector3<T> Vector3<T>::transformed_coord(const Matrix4x4<T>& m) const
{
	Vector3<T> v(behavior::noinitialize);
	return m.transform_coord(*this, v);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transformed_coord(const Matrix4x4<T>& m, Vector3<T>& result) const
{
	return m.transform_coord(*this, result);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transform_normal(const Matrix4x4<T>& m)
{
	Vector3<T> v = *this;
	return m.transform_normal(v, *this);
}
template <typename T> inline
Vector3<T> Vector3<T>::transformed_normal(const Matrix4x4<T>& m) const
{
	Vector3<T> v(behavior::noinitialize);
	return m.transform_normal(*this, v);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transformed_normal(const Matrix4x4<T>& m, Vector3<T>& result) const
{
	return m.transform_normal(*this, result);
}

/*---------------------------------------------------------------------
* Vector4.transform
*---------------------------------------------------------------------*/
template <typename T> inline
Vector4<T>& Vector4<T>::transform(const Matrix4x4<T>& m)
{
	Vector4<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector4<T> Vector4<T>::transformed(const Matrix4x4<T>& m) const
{
	Vector4<T> v(behavior::noinitialize);
	return m.transform(*this, v);
}
template <typename T> inline
Vector4<T>& Vector4<T>::transformed(const Matrix4x4<T>& m, Vector4<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector4<T>& Vector4<T>::transform_coord(const Matrix4x4<T>& m)
{
	Vector4<T> v = *this;
	return m.transform_coord(v, *this);
}
template <typename T> inline
Vector4<T> Vector4<T>::transformed_coord(const Matrix4x4<T>& m) const
{
	Vector4<T> v(behavior::noinitialize);
	return m.transform_coord(*this, v);
}
template <typename T> inline
Vector4<T>& Vector4<T>::transformed_coord(const Matrix4x4<T>& m, Vector4<T>& result) const
{
	return m.transform_coord(*this, result);
}

template <typename T> inline
Vector3<T> Vector3<T>::operator * (const Matrix4x4<T>& m) const
{
	Vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
Vector4<T> Vector4<T>::operator * (const Matrix4x4<T>& m) const
{
	Vector4<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* 行列からクォータニオンを求める
*---------------------------------------------------------------------*/
template <typename T> inline
Quaternion<T>& Quaternion<T>::from_matrix(const Matrix4x4<T>& m)
{
	const Vector4<T>* v0 = &m.M[0];
	const Vector4<T>* v1 = &m.M[1];
	const Vector4<T>* v2 = &m.M[2];

	T tr = v0->X + v1->Y + v2->Z + m.M[3].W;
	if (tr >= math_type::One)
	{
		T fourD = math_type::Two * math_type::sqrt(tr);
		T dFourD = math_type::One / fourD;
		X = (v1->Z - v2->Y) * dFourD;
		Y = (v2->X - v0->Z) * dFourD;
		Z = (v0->Y - v1->X) * dFourD;
		// fourD / math_type::Four
		W = fourD * math_type::HalfOfHalf;
		return *this;
	}

	int i = 0;
	if (v0->X <= v1->Y)
	{
		++i;
	}
	if (v2->Z > m[i][i])
	{
		++i;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;

	tr = m[i][i] - m[j][j] - m[k][k] + math_type::One;
	T fourD = math_type::Two * math_type::sqrt(tr);
	T dFourD = math_type::One / fourD;

	(*this)[i] = fourD * math_type::HalfOfHalf;
	(*this)[j] = (m[j][i] + m[i][j]) * dFourD;
	(*this)[k] = (m[k][i] + m[i][k]) * dFourD;
	W = (m[j][k] - m[k][j]) * dFourD;

	return *this;
}

// 4x4行列からの作成
template <typename T> inline
Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& m)
{
	const Vector4<T>* m0 = &m[0];
	const Vector4<T>* m1 = &m[1];
	const Vector4<T>* m2 = &m[2];
	Vector3<T>* p0 = &M[0];
	Vector3<T>* p1 = &M[1];
	Vector3<T>* p2 = &M[2];

	p0->X = m0->X;
	p0->Y = m0->Y;
	p0->Z = m0->Z;

	p1->X = m1->X;
	p1->Y = m1->Y;
	p1->Z = m1->Z;

	p2->X = m2->X;
	p2->Y = m2->Y;
	p2->Z = m2->Z;
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Matrix4x4<T>& v)
{
	//{
	//	V0
	//	V1
	//	V2
	//	V3
	//}
	os << out_char::braces_left << out_char::line;
	for (typename Matrix4x4<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Matrix4x4<T>& v)
{
	is.ignore();
	for (typename Matrix4x4<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Matrix4x4<T>& v)
{
	os << out_char::braces_left << out_char::line;
	for (typename Matrix4x4<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Matrix4x4<T>& v)
{
	is.ignore();
	for (typename Matrix4x4<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __MATH_MATRIX4X4_H__
