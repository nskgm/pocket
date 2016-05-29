#ifndef __MATH_MATRIX3X3_H__
#define __MATH_MATRIX3X3_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "array.h"
#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

template <typename> struct Matrix3x3;
template <typename> struct Matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef Matrix3x3<float> Matrix3x3f;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
typedef Matrix3x3<double> Matrix3x3d;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
typedef Matrix3x3<long double> Matrix3x3ld;
#endif /* _USING_MATH_LONG_DOUBLE */

#ifdef _USE_CXX11
template <typename T>
using mat3x3 = Matrix3x3<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using mat3x3f = mat3x3<float>;
#endif /* _UNUSING_MATH_INT_FLOAT */
#ifdef _USING_MATH_DOUBLE
using mat3x3d = mat3x3<double>;
#endif /* _USING_MATH_DOUBLE */
#ifdef _USING_MATH_LONG_DOUBLE
using mat3x3ld = mat3x3<long double>;
#endif /* _USING_MATH_LONG_DOUBLE */
#endif /* _USE_CXX11 */

/* 行列の配列位置を示す */
struct MatrixPoint
{
	enum Elem
#ifdef _USE_CXX11
		: unsigned int
#endif /* _USE_CXX11 */
	{

#ifdef _USE_CXX11
		_0, _1, _2, _3,
#else
		_0 = 0x00U, _1 = 0x01U, _2 = 0x02U, _3 = 0x03U,
#endif /* _USE_CXX11 */

		_X = _0,
		_Y = _1,
		_Z = _2,
		_W = _3
	};

	Elem X;
	Elem Y;

	_DEFAULT_CONSTRUCTOR(MatrixPoint);
	MatrixPoint(Elem x, Elem y) :
		X(x), Y(y)
	{

	}
	MatrixPoint(unsigned int x, unsigned int y) :
		X(static_cast<MatrixPoint::Elem>(x)),
		Y(static_cast<MatrixPoint::Elem>(y))
	{

	}
};

/* カンマでMatrixPointを生成する演算子 */

inline MatrixPoint operator , (MatrixPoint::Elem y, MatrixPoint::Elem x)
{
	return MatrixPoint(x, y);
}

template <typename T>
struct Matrix3x3
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef Vector3<T> vector3_type;
	typedef Vector3<T> row_type;
	typedef Vector3<T> column_type;

	typedef container::array<Vector3<T>, 3> array3x3_type;
	typedef typename array3x3_type::value_type value_type;
	typedef typename array3x3_type::iterator iterator;
	typedef typename array3x3_type::const_iterator const_iterator;
	typedef typename array3x3_type::pointer pointer;
	typedef typename array3x3_type::const_pointer const_pointer;
	typedef typename array3x3_type::reference reference;
	typedef typename array3x3_type::const_reference const_reference;

	typedef container::array<T, 16> array9_type;
	typedef typename array9_type::value_type value_type_9;
	typedef typename array9_type::iterator iterator_9;
	typedef typename array9_type::const_iterator const_iterator_9;
	typedef typename array9_type::pointer pointer_9;
	typedef typename array9_type::const_pointer const_pointer_9;
	typedef typename array9_type::reference reference_9;
	typedef typename array9_type::const_reference const_reference_9;

	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif /* _USE_ANONYMOUS_NON_POD */

			array3x3_type M; /* 行3 */

#ifdef _USE_ANONYMOUS_NON_POD
		};
		/* 1行ごと */
		struct
		{
			row_type MV0;
			row_type MV1;
			row_type MV2;
		};
		/* 行列内ベクトル */
		struct
		{
			Vector3<T> Right; /* X方向ベクトル */
			Vector3<T> Up; /* Y方向ベクトル */
			union
			{
				Vector3<T> Forward; /* Z方向ベクトル */
				struct
				{
					Vector2<T> Position; /* 2D時座標 */
					T PositionZ;
				};
			};
		};

		/* それぞれの要素へアクセス */
		struct
		{
			T M11; T M12; T M13;
			T M21; T M22; T M23;
			T M31; T M32; T M33;
		};
		array9_type Data;
	};
#endif /* _USE_ANONYMOUS_NON_POD */

	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	static const Matrix3x3 Zero; /* 0.0 x 9 */
	static const Matrix3x3 Identity; /* 単位行列 */

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Matrix3x3);
	explicit Matrix3x3(const behavior::_noinitialize_t&)
	{

	}
	explicit Matrix3x3(const behavior::_zero_t&)
	{
		M[0] = Vector3<T>::Zero;
		M[1] = Vector3<T>::Zero;
		M[2] = Vector3<T>::Zero;
	}
	explicit Matrix3x3(const behavior::_identity_t&)
	{
		M[0] = Vector3<T>::UnitX;
		M[1] = Vector3<T>::UnitY;
		M[2] = Vector3<T>::UnitZ;
	}
	explicit Matrix3x3(T t)
	{
		M[0] = Vector3<T>(t, math_type::Zero, math_type::Zero);
		M[1] = Vector3<T>(math_type::Zero, t, math_type::Zero);
		M[2] = Vector3<T>(math_type::Zero, math_type::Zero, t);
	}
	Matrix3x3(T M11, T M12, T M13,
		T M21, T M22, T M23,
		T M31, T M32, T M33)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		MV0.X = M11;
		MV0.Y = M12;
		MV0.Z = M13;
		MV1.X = M21;
		MV1.Y = M22;
		MV1.Z = M23;
		MV2.X = M31;
		MV2.Y = M32;
		MV2.Z = M33;
#else
		M[0] = Vector3<T>(M11, M12, M13);
		M[1] = Vector3<T>(M21, M22, M23);
		M[2] = Vector3<T>(M31, M32, M33);
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	Matrix3x3(const Vector3<T>& M1, const Vector3<T>& M2, const Vector3<T>& M3)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		MV0 = M1;
		MV1 = M2;
		MV2 = M3;
#else
		M[0] = M1;
		M[1] = M2;
		M[2] = M3;
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	explicit Matrix3x3(const Matrix4x4<T>&); /* Matrix4x4.h */

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	Matrix3x3& add(const Matrix3x3& m, Matrix3x3& result) const
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
	Matrix3x3& subtract(const Matrix3x3& m, Matrix3x3& result) const
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
	Matrix3x3& multiply(const Matrix3x3& m, Matrix3x3& result) const
	{
#if defined(__DEBUG)
		Matrix3x3 t = m.transposed();
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
		}
#else
		const_iterator oi0 = m.M.begin();
		const_iterator oi1 = oi0 + 1;
		const_iterator oi2 = oi0 + 2;
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->X = i->X * oi0->X + i->Y * oi1->X + i->Z * oi2->X;
			ri->Y = i->X * oi0->Y + i->Y * oi1->Y + i->Z * oi2->Y;
			ri->Z = i->X * oi0->Z + i->Y * oi1->Z + i->Z * oi2->Z;
		}
#endif
		return result;
	}
	Matrix3x3& multiply(T s, Matrix3x3& result) const
	{
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->X = i->X * s;
			ri->Y = i->Y * s;
			ri->Z = i->Z * s;
		}
		return result;
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	Matrix3x3& divide(T s, Matrix3x3& result) const
	{
		_DEB_ASSERT(s != math_type::Zero);
		s = math_type::One / s;
		return multiply(s, result);
	}

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const Matrix3x3& m) const
	{
		return (M[0].is_near(m.M[0]) &&
			M[1].is_near(m.M[1]) &&
			M[2].is_near(m.M[2]));
	}
	/*---------------------------------------------------------------------
	* すべてがゼロに近いか
	*---------------------------------------------------------------------*/
	bool is_near_zero() const
	{
		return (M[0].is_near(Vector3<T>::Zero) &&
			M[1].is_near(Vector3<T>::Zero) &&
			M[2].is_near(Vector3<T>::Zero));
	}
	/*---------------------------------------------------------------------
	* すべてがゼロか
	*---------------------------------------------------------------------*/
	bool is_zero() const
	{
		return (M[0].is_zero() && M[1].is_zero() && M[2].is_zero());
	}

	/*---------------------------------------------------------------------
	* 単位行列に近いか
	*---------------------------------------------------------------------*/
	bool is_near_identity() const
	{
		return (M[0].is_near(Vector3<T>::UnitX) &&
			M[1].is_near(Vector3<T>::UnitY) &&
			M[2].is_near(Vector3<T>::UnitZ));
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
		return M[0];
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	Matrix3x3& right(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Right = v;
#else
		M[0] = v;
#endif /* _USE_ANONYMOUS_NON_POD */
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
		return M[1];
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	Matrix3x3& up(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Up = v;
#else
		M[1] = v;
#endif /* _USE_ANONYMOUS_NON_POD */
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
		return M[2];
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	Matrix3x3& forward(const Vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Forward = v;
#else
		M[2] = v;
#endif /* _USE_ANONYMOUS_NON_POD */
		return *this;
	}
	/*---------------------------------------------------------------------
	* 座標取得設定
	*---------------------------------------------------------------------*/
	const Vector2<T>& position() const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return Position;
#else
		const Vector3<T>& m = M[2];
		return reinterpret_cast<const Vector2<T>&>(m);
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	Matrix3x3& position(const Vector2<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		Position = v;
#else
		Vector3<T>& m = M[2];
		m.X = v.X;
		m.Y = v.Y;
#endif /* _USE_ANONYMOUS_NON_POD */
		return *this;
	}

	/*---------------------------------------------------------------------
	* 拡大縮小取得
	*---------------------------------------------------------------------*/
	Vector2<T> scale2d() const
	{
		const Vector2<T>& r = reinterpret_cast<const Vector2<T>&>(M[0]);
		const Vector2<T>& u = reinterpret_cast<const Vector2<T>&>(M[1]);
		return Vector2<T>(r.length(), u.length());
	}
	Vector2<T>& scale2d(Vector2<T>& result) const
	{
		const Vector2<T>& r = reinterpret_cast<const Vector2<T>&>(M[0]);
		const Vector2<T>& u = reinterpret_cast<const Vector2<T>&>(M[1]);
		result.X = r.length();
		result.Y = u.length();
		return result;
	}
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
		/* 前ベクトルのY軸の状態から計算する */
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
	Matrix3x3& load_zero()
	{
		*this = Matrix3x3::Zero;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 単位行列にする
	*---------------------------------------------------------------------*/
	Matrix3x3& load_identity()
	{
		*this = Matrix3x3::Identity;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする（2D）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_scale2d(T x, T y)
	{
		load_identity();
		M[0].X = x;
		M[1].Y = y;
		return *this;
	}
	Matrix3x3& load_scale2d(const Vector2<T>& v)
	{
		return load_scale2d(v.X, v.Y);
	}
	Matrix3x3& load_scale2d(T s)
	{
		return load_scale2d(s, s);
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする（3D）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_scale(T x, T y, T z)
	{
		load_identity();
		M[0].X = x;
		M[1].Y = y;
		M[2].Z = z;
		return *this;
	}
	Matrix3x3& load_scale(const Vector3<T>& v)
	{
		return load_scale(v.X, v.Y, v.Z);
	}
	Matrix3x3& load_scale(T s)
	{
		return load_scale(s, s, s);
	}

	/*---------------------------------------------------------------------
	* 回転行列にする（2D）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_rotate(T r)
	{
		return load_rotate_z(r);
	}

	/*---------------------------------------------------------------------
	* X軸回転行列にする
	*---------------------------------------------------------------------*/
	Matrix3x3& load_rotate_x(T r)
	{
		load_identity();
		/* サインとコサインを角度から求める */
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector3<T>* p = &M[1];
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
	Matrix3x3& load_rotate_y(T r)
	{
		load_identity();
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector3<T>* p = &M[0];
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
	Matrix3x3& load_rotate_z(T r)
	{
		load_identity();
		T S = math_type::sin(r);
		T C = math_type::cos(r);
		Vector3<T>* p = &M[0];
		p->X = C;
		p->Y = S;
		p = &M[1];
		p->X = -S;
		p->Y = C;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（ZYX）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_rotate_roll_pitch_yaw(T roll, T pitch, T yaw)
	{
		T rs = math_type::sin(roll);
		T rc = math_type::cos(roll);
		T ps = math_type::sin(pitch);
		T pc = math_type::cos(pitch);
		T ys = math_type::sin(yaw);
		T yc = math_type::cos(yaw);

		Vector3<T>* p = &M[0];
		p->X = rc * yc + rs * ps * ys;
		p->Y = rs * pc;
		p->Z = rc * -ys + rs * ps * yc;

		p = &M[1];
		p->X = -rs * yc + rc * ps * ys;
		p->Y = rc * pc;
		p->Z = rs * ys + rc * ps * yc;

		p = &M[2];
		p->X = pc * ys;
		p->Y = -ps;
		p->Z = pc * yc;

		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（クォータニオン）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_rotate_quaternion(const Quaternion<T>& q)
	{
		T xx = math_type::Two * math_type::sqr(q.X);
		T yy = math_type::Two * math_type::sqr(q.Y);
		T zz = math_type::Two * math_type::sqr(q.Z);
		T xy = math_type::Two * q.X * q.Y;
		T xz = math_type::Two * q.X * q.Z;
		T yz = math_type::Two * q.Y * q.Z;
		T wx = math_type::Two * q.W * q.X;
		T wy = math_type::Two * q.W * q.Y;
		T wz = math_type::Two * q.W * q.Z;

		Vector3<T>* p = &M[0];
		p->X = math_type::One - (yy + zz);
		p->Y = xy + wz;
		p->Z = xz - wy;

		p = &M[1];
		p->X = xy - wz;
		p->Y = math_type::One - (xx + zz);
		p->Z = yz + wz;

		p = &M[2];
		p->X = xz + wy;
		p->Y = yz + wx;
		p->Z = math_type::One - (xx + yy);

		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（任意軸＋角度）
	*---------------------------------------------------------------------*/
	Matrix3x3& load_rotate_axis_angle(const Vector3<T>& axis, T angle)
	{
		/* 四元数での計算を行なう */
		return load_rotate_quaternion(Quaternion<T>(axis, angle));
	}
	/*---------------------------------------------------------------------
	* 座標変換行列にする
	*---------------------------------------------------------------------*/
	Matrix3x3& load_translate(T x, T y)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		Position.X = x;
		Position.Y = y;
#else
		/* ４の位置が座標を扱う要素 */
		Vector3<T>* p = &M[2];
		p->X = x;
		p->Y = y;
#endif /* _USE_ANONYMOUS_NON_POD */
		return *this;
	}
	Matrix3x3& load_translate(const Vector2<T>& v)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		Position = v;
#else
		Vector3<T>* p = &M[2];
		p->X = v.X;
		p->Y = v.Y;
#endif /* _USE_ANONYMOUS_NON_POD */
		return *this;
	}

	/*---------------------------------------------------------------------
	* ワールド変換行列にする
	*---------------------------------------------------------------------*/
	Matrix3x3& load_world(const Vector2<T>& s, T rotate, const Vector2<T>& t)
	{
		T S = math_type::sin(rotate);
		T C = math_type::cos(rotate);

		Vector3<T>* p = &M[0];
		p->X = s.X * C;
		p->Y = s.X * S;
		p->Z = math_type::Zero;

		p = &M[1];
		p->X = s.Y * -S;
		p->Y = s.Y * C;
		p->Z = math_type::Zero;

		p = &M[2];
		p->X = t.X;
		p->Y = t.Y;
		p->Z = math_type::One;

		return *this;
	}

	/*---------------------------------------------------------------------
	* 拡大縮小と回転を行なう行列にする
	*---------------------------------------------------------------------*/
	Matrix3x3& load_scale_rotate(const Vector2<T>& s, T rotate)
	{
		T S = math_type::sin(rotate);
		T C = math_type::cos(rotate);

		Vector3<T>* p = &M[0];
		p->X = s.X * C;
		p->Y = s.X * S;
		p->Z = math_type::Zero;

		p = &M[1];
		p->X = s.Y * -S;
		p->Y = s.Y * C;
		p->Z = math_type::Zero;

		M[2] = Vector3<T>::UnitZ;

		return *this;
	}
	Matrix3x3& load_scale_rotate(const Vector3<T>& s, T roll, T pitch, T yaw)
	{
		T rs = math_type::sin(roll);
		T rc = math_type::cos(roll);
		T ps = math_type::sin(pitch);
		T pc = math_type::cos(pitch);
		T ys = math_type::sin(yaw);
		T yc = math_type::cos(yaw);

		Vector3<T>* p = &M[0];
		p->X = s.X * rc * yc + s.X * rs * ps * ys;
		p->Y = s.X * rs * pc;
		p->Z = s.X * rc * -ys + s.X * rs * ps * yc;

		p = &M[1];
		p->X = s.Y * -rs * yc + s.Y * rc * ps * ys;
		p->Y = s.Y * rc * pc;
		p->Z = s.Y * rs * ys + s.Y * rc * ps * yc;

		p = &M[2];
		p->X = s.Z * pc * ys;
		p->Y = s.Z * -ps;
		p->Z = s.Z * pc * yc;

		return *this;
	}
	Matrix3x3& load_scale_rotate(const Vector3<T>& s, const Quaternion<T>& q)
	{
		T xx = math_type::Two * math_type::sqr(q.X);
		T yy = math_type::Two * math_type::sqr(q.Y);
		T zz = math_type::Two * math_type::sqr(q.Z);
		T xy = math_type::Two * q.X * q.Y;
		T xz = math_type::Two * q.X * q.Z;
		T yz = math_type::Two * q.Y * q.Z;
		T wx = math_type::Two * q.W * q.X;
		T wy = math_type::Two * q.W * q.Y;
		T wz = math_type::Two * q.W * q.Z;

		Vector3<T>* p = &M[0];
		p->X = s.X * (math_type::One - (yy + zz));
		p->Y = s.X * (xy + wz);
		p->Z = s.X * (xz - wy);
		p->W = math_type::Zero;

		p = &M[1];
		p->X = s.Y * (xy - wz);
		p->Y = s.Y * (math_type::One - (xx + zz));
		p->Z = s.Y * (yz + wz);
		p->W = math_type::Zero;

		p = &M[2];
		p->X = s.Z * (xz + wy);
		p->Y = s.Z * (yz + wx);
		p->Z = s.Z * (math_type::One - (xx + yy));
		p->W = math_type::Zero;

		return *this;
	}

	/*---------------------------------------------------------------------
	* 転置行列
	*---------------------------------------------------------------------*/
	Matrix3x3& transpose()
	{
		Vector3<T>* v0 = &M[0];
		Vector3<T>* v1 = &M[1];
		Vector3<T>* v2 = &M[2];

		/* [0][1] = [1][0] */
		T tmp = v0->Y;
		v0->Y = v1->X;
		v1->X = tmp;
		/* [0][2] = [2][0] */
		tmp = v0->Z;
		v0->Z = v2->X;
		v2->X = tmp;
		/* [1][2] = [2][1] */
		tmp = v1->Z;
		v1->Z = v2->Y;
		v2->Y = tmp;

		return *this;
	}
	/*---------------------------------------------------------------------
	* 転置行列を求める
	*---------------------------------------------------------------------*/
	Matrix3x3 transposed() const
	{
		const Vector3<T>* v0 = &M[0];
		const Vector3<T>* v1 = &M[1];
		const Vector3<T>* v2 = &M[2];
		return Matrix3x3(v0->X, v1->X, v2->X,
			v0->Y, v1->Y, v2->Y,
			v0->Z, v1->Z, v2->Z);
	}
	Matrix3x3& transposed(Matrix3x3& result) const
	{
		const Vector3<T>* v0 = &M[0];
		const Vector3<T>* v1 = &M[1];
		const Vector3<T>* v2 = &M[2];
		Vector3<T>* r0 = &result.M[0];
		Vector3<T>* r1 = &result.M[1];
		Vector3<T>* r2 = &result.M[2];

		/* [0][1] = [1][0] */
		r0->Y = v1->X;
		r1->X = v0->Y;
		/* [0][2] = [2][0] */
		r0->Z = v2->X;
		r2->X = v0->Z;
		/* [1][2] = [2][1] */
		r1->Z = v2->Y;
		r2->Y = v1->Z;

		r0->X = v0->X;
		r1->Y = v1->Y;
		r2->Z = v2->Z;

		return result;
	}

	/*---------------------------------------------------------------------
	* ベクトル座標変換
	*---------------------------------------------------------------------*/
	Vector2<T> transform(const Vector2<T>& v) const
	{
		Vector2<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	Vector2<T>& transform(const Vector2<T>& v, Vector2<T>& result) const
	{
		const Vector3<T>* v0 = &M[0];
		const Vector3<T>* v1 = &M[1];
		const Vector3<T>* v2 = &M[2];

		result.X = v0->X * v.X + v1->X * v.Y + v2->X;
		result.Y = v0->Y * v.X + v1->Y * v.Y + v2->Y;
		result.Z = v0->Z * v.X + v1->Z * v.Y + v2->Z;

		return result;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）
	*---------------------------------------------------------------------*/
	Vector2<T> transform_coord(const Vector2<T>& v) const
	{
		Vector2<T> r(behavior::noinitialize);
		return transform_coord(v, r);
	}
	Vector2<T>& transform_coord(const Vector2<T>& v, Vector2<T>& result) const
	{
		const Vector3<T>* v0 = &M[0];
		const Vector3<T>* v1 = &M[1];
		const Vector3<T>* v2 = &M[2];

		T w = math_type::One / (v0->Z * v.X + v1->Z * v.Y + v2->Z);
		result.X = (v0->X * v.X + v1->X * v.Y + v2->X) * w;
		result.Y = (v0->Y * v.X + v1->Y * v.Y + v2->Y) * w;

		return result;
	}
	/*---------------------------------------------------------------------
	* 法線ベクトル座標変換
	*---------------------------------------------------------------------*/
	Vector3<T> transform(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	Vector3<T>& transform(const Vector3<T>& v, Vector3<T>& result) const
	{
		const Vector3<T>* v0 = &M[0];
		const Vector3<T>* v1 = &M[1];
		const Vector3<T>* v2 = &M[2];

		result.X = v0->X * v.X + v1->X * v.Y + v2->X * v.Z;
		result.Y = v0->Y * v.X + v1->Y * v.Y + v2->Y * v.Z;
		result.Z = v0->Z * v.X + v1->Z * v.Y + v2->Z * v.Z;

		return result;
	}
	Vector3<T> transform_normal(const Vector3<T>& v) const
	{
		Vector3<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	Vector3<T>& transform_normal(const Vector3<T>& v, Vector3<T>& result) const
	{
		return transform(v, result);
	}


	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	Vector3<T>& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
		return M[i];
	}
	const Vector3<T>& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
		return M[i];
	}
	T& operator [] (const MatrixPoint& p)
	{
		_DEB_RANGE_ASSERT(p.X, 0, 2);
		_DEB_RANGE_ASSERT(p.Y, 0, 2);
		return (*this)(p.Y, p.X);
	}
	const T& operator [] (const MatrixPoint& p) const
	{
		_DEB_ASSERT(p.X < 3 && p.Y < 3);
		return (*this)(p.Y, p.X);
	}
	T& operator () (int y, int x)
	{
		_DEB_RANGE_ASSERT(x, 0, 2);
		_DEB_RANGE_ASSERT(y, 0, 2);
		return M[y][x];
	}
	const T& operator () (int y, int x) const
	{
		_DEB_RANGE_ASSERT(x, 0, 2);
		_DEB_RANGE_ASSERT(y, 0, 2);
		return M[y][x];
	}

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	template <typename U>
	_CXX11_EXPLICIT operator Matrix3x3<U>() const
	{
		return Matrix3x3<U>(
			static_cast<Vector3<U> >(M[0]),
			static_cast<Vector3<U> >(M[1]),
			static_cast<Vector3<U> >(M[2]));
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &M[0].X;
#endif /* _USE_ANONYMOUS_NON_POD */
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Data[0];
#else
		return &M[0].X;
#endif /* _USE_ANONYMOUS_NON_POD */
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Matrix3x3& v) const
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
	bool operator != (const Matrix3x3& v) const
	{
		return !(*this == v);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	Matrix3x3 operator + () const
	{
		return *this;
	}
	Matrix3x3 operator - () const
	{
		return Matrix3x3(-M[0], -M[1], -M[2]);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	Matrix3x3 operator + (const Matrix3x3& m) const
	{
		Matrix3x3 result(behavior::noinitialize);
		return add(m, result);
	}
	Matrix3x3 operator - (const Matrix3x3& m) const
	{
		Matrix3x3 result(behavior::noinitialize);
		return subtract(m, result);
	}
	Matrix3x3 operator * (const Matrix3x3& m) const
	{
		Matrix3x3 result(behavior::noinitialize);
		return multiply(m, result);
	}
	Matrix3x3 operator * (T f) const
	{
		Matrix3x3 result(behavior::noinitialize);
		return multiply(f, result);
	}
	Matrix3x3 operator / (T f) const
	{
		Matrix3x3 result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	Matrix3x3& operator = (const behavior::_zero_t&)
	{
		return load_zero();
	}
	Matrix3x3& operator = (const behavior::_identity_t&)
	{
		return load_identity();
	}
#ifdef _USE_CXX11
	Matrix3x3& operator = (const std::initializer_list<T>& t)
	{
		_DEB_ASSERT(t.size() <= 9);

		typename std::initializer_list<T>::const_iterator o = t.begin();
		T* p = static_cast<T*>(M[0]);
		for (typename std::initializer_list<T>::size_type i = 0, size = t.size(); i < size; ++i, ++p, ++o)
		{
			*p = *o;
		}
		return *this;
	}
#endif /* _USE_CXX11 */

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	Matrix3x3& operator += (const Matrix3x3& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i += *oi;
		}
		return *this;
	}
	Matrix3x3& operator -= (const Matrix3x3& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i -= *oi;
		}
		return *this;
	}
	Matrix3x3& operator *= (const Matrix3x3& m)
	{
		const Matrix3x3 c = *this;
		return c.multiply(m, *this);
	}
	Matrix3x3& operator *= (T s)
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
	Matrix3x3& operator /= (T s)
	{
		_DEB_ASSERT(s != math_type::Zero);
		s = math_type::One / s;
		return operator*=(s);
	}
};

template <typename T>
const Matrix3x3<T> Matrix3x3<T>::Zero(math_type::Zero);
template <typename T>
const Matrix3x3<T> Matrix3x3<T>::Identity(math_type::One);

/*---------------------------------------------------------------------
* Vector2.transform
*---------------------------------------------------------------------*/
template <typename T> inline
Vector2<T>& Vector2<T>::transform(const Matrix3x3<T>& m)
{
	const Vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector2<T>& Vector2<T>::transformed(const Matrix3x3<T>& m, Vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector2<T> Vector2<T>::transformed(const Matrix3x3<T>& m) const
{
	Vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
Vector2<T>& Vector2<T>::transform_coord(const Matrix3x3<T>& m)
{
	const Vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector2<T>& Vector2<T>::transformed_coord(const Matrix3x3<T>& m, Vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector2<T> Vector2<T>::transformed_coord(const Matrix3x3<T>& m) const
{
	Vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
Vector2<T>& Vector2<T>::transform_normal(const Matrix3x3<T>& m)
{
	const Vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector2<T>& Vector2<T>::transformed_normal(const Matrix3x3<T>& m, Vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector2<T> Vector2<T>::transformed_normal(const Matrix3x3<T>& m) const
{
	Vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* Vector3.transform
*---------------------------------------------------------------------*/
template <typename T> inline
Vector3<T>& Vector3<T>::transform(const Matrix3x3<T>& m)
{
	const Vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transformed(const Matrix3x3<T>& m, Vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector3<T> Vector3<T>::transformed(const Matrix3x3<T>& m) const
{
	Vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transform_normal(const Matrix3x3<T>& m)
{
	const Vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
Vector3<T>& Vector3<T>::transformed_normal(const Matrix3x3<T>& m, Vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
Vector3<T> Vector3<T>::transformed_normal(const Matrix3x3<T>& m) const
{
	Vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* ベクトルとの二項演算子
*---------------------------------------------------------------------*/
template <typename T> inline
Vector2<T> Vector2<T>::operator * (const Matrix3x3<T>& m) const
{
	Vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
Vector3<T> Vector3<T>::operator * (const Matrix3x3<T>& m) const
{
	Vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* 行列からクォータニオンを求める
*---------------------------------------------------------------------*/
template <typename T> inline
Quaternion<T>& Quaternion<T>::from_matrix(const Matrix3x3<T>& m)
{
	const Vector3<T>* v0 = &m.M[0];
	const Vector3<T>* v1 = &m.M[1];
	const Vector3<T>* v2 = &m.M[2];

	T tr = v0->X + v1->Y + v2->Z + math_type::One;
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

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Matrix3x3<T>& v)
{
	//{
	//	V0
	//	V1
	//	V2
	//}
	os << out_char::braces_left << out_char::line;
	for (typename Matrix3x3<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Matrix3x3<T>& v)
{
	is.ignore();
	for (typename Matrix3x3<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Matrix3x3<T>& v)
{
	os << out_char::braces_left << out_char::line;
	for (typename Matrix3x3<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Matrix3x3<T>& v)
{
	is.ignore();
	for (typename Matrix3x3<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* __MATH_MATRIX3X3_H__ */
