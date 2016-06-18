#ifndef __POCKET_MATH_MATRIX3X3_H__
#define __POCKET_MATH_MATRIX3X3_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../container/array.h"
#include "math_traits.h"
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct matrix3x3;
template <typename> struct matrix4x4;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef matrix3x3<float> matrix3x3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef matrix3x3<double> matrix3x3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef matrix3x3<long double> matrix3x3ld;
#endif // _USING_MATH_LONG_DOUBLE

// 行列の配列位置を示す
struct matrix_point
{
	enum element
#ifdef _USE_CXX11
		: unsigned int
#endif // _USE_CXX11
	{

#ifdef _USE_CXX11
		_0, _1, _2, _3,
#else
		_0 = 0x00U, _1 = 0x01U, _2 = 0x02U, _3 = 0x03U,
#endif // _USE_CXX11

		_X = _0,
		_Y = _1,
		_Z = _2,
		_W = _3
	};

	element x;
	element y;

	_DEFAULT_CONSTRUCTOR(matrix_point);
	matrix_point(element x, element y) :
		x(x), y(y)
	{

	}
	matrix_point(unsigned int y, unsigned int x) :
		x(static_cast<element>(x)),
		y(static_cast<element>(y))
	{

	}
};

// カンマでmatrix_pointを生成する演算子

inline matrix_point operator , (matrix_point::element y, matrix_point::element x)
{
	return matrix_point(y, x);
}

template <typename T>
struct matrix3x3
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef vector3<T> vector3_type;
	typedef vector3<T> row_type;
	typedef vector3<T> column_type;
	typedef typename math_type::sin_cos_t sin_cos_type;

	typedef container::array<row_type, 3> array3x3_type;
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
#endif // _USE_ANONYMOUS_NON_POD

			array3x3_type M; // 行3

#ifdef _USE_ANONYMOUS_NON_POD
		};
		// 1行ごと
		struct
		{
			row_type mv0;
			row_type mv1;
			row_type mv2;
		};
		// 行列内ベクトル
		struct
		{
			vector3<T> _right; // x方向ベクトル
			vector3<T> _up; // y方向ベクトル
			union
			{
				vector3<T> _forward; // z方向ベクトル
				struct
				{
					vector2<T> _position; // 2D時座標
					T _position_z;
				};
			};
		};

		// それぞれの要素へアクセス
		struct
		{
			T m11; T m12; T m13;
			T m21; T m22; T m23;
			T m31; T m32; T m33;
		};
		array9_type data;
	};
#endif // _USE_ANONYMOUS_NON_POD

	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	static const matrix3x3 zero; // 0.0 x 9
	static const matrix3x3 identity; // 単位行列

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(matrix3x3);
	explicit matrix3x3(const behavior::_noinitialize_t&)
	{

	}
	explicit matrix3x3(const behavior::_zero_t&)
	{
		M[0] = row_type::zero;
		M[1] = row_type::zero;
		M[2] = row_type::zero;
	}
	explicit matrix3x3(const behavior::_identity_t&)
	{
		M[0] = row_type::unit_x;
		M[1] = row_type::unit_y;
		M[2] = row_type::unit_z;
	}
	explicit matrix3x3(T t)
	{
		M[0] = row_type(t, math_type::zero, math_type::zero);
		M[1] = row_type(math_type::zero, t, math_type::zero);
		M[2] = row_type(math_type::zero, math_type::zero, t);
	}
	matrix3x3(T M11, T M12, T M13,
		T M21, T M22, T M23,
		T M31, T M32, T M33)
#ifdef _USE_ANONYMOUS_NON_POD
		: mv0(M11, M12, M13), mv1(M21, M22, M23), mv2(M31, M32, M33)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		M[0] = vector3<T>(M11, M12, M13);
		M[1] = vector3<T>(M21, M22, M23);
		M[2] = vector3<T>(M31, M32, M33);
#endif // _USE_ANONYMOUS_NON_POD
	}
	matrix3x3(const vector3<T>& M1, const vector3<T>& M2, const vector3<T>& M3)
#ifdef _USE_ANONYMOUS_NON_POD
		: mv0(M1), mv1(M2), mv2(M3)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		M[0] = M1;
		M[1] = M2;
		M[2] = M3;
#endif // _USE_ANONYMOUS_NON_POD
	}
	explicit matrix3x3(const matrix4x4<T>&); // matrix4x4.h

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 足し算
	*---------------------------------------------------------------------*/
	matrix3x3& add(const matrix3x3& m, matrix3x3& result) const
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
	matrix3x3& subtract(const matrix3x3& m, matrix3x3& result) const
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
	matrix3x3& multiply(const matrix3x3& m, matrix3x3& result) const
	{
#if defined(__DEBUG)
		matrix3x3 t = m.transposed();
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
		}
#else
		const_iterator oi0 = m.M.begin();
		const_iterator oi1 = oi0 + 1;
		const_iterator oi2 = oi0 + 2;
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->x = i->x * oi0->x + i->y * oi1->x + i->z * oi2->x;
			ri->y = i->x * oi0->y + i->y * oi1->y + i->z * oi2->y;
			ri->z = i->x * oi0->z + i->y * oi1->z + i->z * oi2->z;
		}
#endif
		return result;
	}
	matrix3x3& multiply(T s, matrix3x3& result) const
	{
		iterator ri = result.M.begin();
		for (const_iterator i = M.begin(), end = M.end(); i != end; ++i, ++ri)
		{
			ri->x = i->x * s;
			ri->y = i->y * s;
			ri->z = i->z * s;
		}
		return result;
	}
	/*---------------------------------------------------------------------
	* 割り算
	*---------------------------------------------------------------------*/
	matrix3x3& divide(T s, matrix3x3& result) const
	{
		_DEB_ASSERT(s != math_type::zero);
		return multiply(math_type::reciprocal(s), result);
	}

	/*---------------------------------------------------------------------
	* 値が近いか
	*---------------------------------------------------------------------*/
	bool is_near(const matrix3x3& m) const
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
		return (M[0].is_near(vector3<T>::zero) &&
			M[1].is_near(vector3<T>::zero) &&
			M[2].is_near(vector3<T>::zero));
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
		return (M[0].is_near(vector3<T>::unit_x) &&
			M[1].is_near(vector3<T>::unit_y) &&
			M[2].is_near(vector3<T>::unit_z));
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
		return M[0];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& right() const
	{
		return right();
	}
	matrix3x3& right(const vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		_right = v;
#else
		M[0] = v;
#endif // _USE_ANONYMOUS_NON_POD
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
		return M[1];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& up() const
	{
		return up();
	}
	matrix3x3& up(const vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		_up = v;
#else
		M[1] = v;
#endif // _USE_ANONYMOUS_NON_POD
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
		return M[2];
#endif // _USE_ANONYMOUS_NON_POD
	}
	const vector3<T>& forward() const
	{
		return forward();
	}
	matrix3x3& forward(const vector3<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		_forward = v;
#else
		M[2] = v;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	/*---------------------------------------------------------------------
	* 座標取得設定
	*---------------------------------------------------------------------*/
	vector2<T>& position()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return _position;
#else
		return reinterpret_cast<vector2<T>&>(M[2]);
#endif // _USE_ANONYMOUS_NON_POD
	}
	const vector2<T>& position() const
	{
		return position();
	}
	matrix3x3& position(const vector2<T>& v)
	{
#ifdef _USE_ANONYMOUS_NON_POD
		_position = v;
#else
		vector3<T>& m = M[2];
		m.x = v.x;
		m.y = v.y;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}

	/*---------------------------------------------------------------------
	* 拡大縮小取得
	*---------------------------------------------------------------------*/
	vector2<T> scale2d() const
	{
		const vector2<T>& r = reinterpret_cast<const vector2<T>&>(M[0]);
		const vector2<T>& u = reinterpret_cast<const vector2<T>&>(M[1]);
		return vector2<T>(r.length(), u.length());
	}
	vector2<T>& scale2d(vector2<T>& result) const
	{
		const vector2<T>& r = reinterpret_cast<const vector2<T>&>(M[0]);
		const vector2<T>& u = reinterpret_cast<const vector2<T>&>(M[1]);
		result.x = r.length();
		result.y = u.length();
		return result;
	}
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
	matrix3x3& load_zero()
	{
		*this = matrix3x3::zero;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 単位行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_identity()
	{
		*this = matrix3x3::identity;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする（2D）
	*---------------------------------------------------------------------*/
	matrix3x3& load_scale2d(T x, T y)
	{
		load_identity();
		M[0].x = x;
		M[1].y = y;
		return *this;
	}
	matrix3x3& load_scale2d(const vector2<T>& v)
	{
		return load_scale2d(v.x, v.y);
	}
	matrix3x3& load_scale2d(T s)
	{
		return load_scale2d(s, s);
	}
	/*---------------------------------------------------------------------
	* 拡大縮小行列にする（3D）
	*---------------------------------------------------------------------*/
	matrix3x3& load_scale(T x, T y, T z)
	{
		load_identity();
		M[0].x = x;
		M[1].y = y;
		M[2].z = z;
		return *this;
	}
	matrix3x3& load_scale(const vector3<T>& v)
	{
		return load_scale(v.x, v.y, v.z);
	}
	matrix3x3& load_scale(T s)
	{
		return load_scale(s, s, s);
	}

	/*---------------------------------------------------------------------
	* 回転行列にする（2D）
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate(T r)
	{
		return load_rotate_z(r);
	}

	/*---------------------------------------------------------------------
	* x軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_x(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type::unit_x;
		M[1] = row_type(math_type::zero, a.cos, a.sin);
		M[2] = row_type(math_type::zero, -a.sin, a.cos);
		return *this;
	}
	/*---------------------------------------------------------------------
	* y軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_y(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, math_type::zero, -a.sin);
		M[1] = row_type::unit_y;
		M[2] = row_type(a.sin, math_type::zero, a.cos);
		return *this;
	}
	/*---------------------------------------------------------------------
	* z軸回転行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_z(T r)
	{
		sin_cos_type a = r;
		M[0] = row_type(a.cos, a.sin, math_type::zero);
		M[1] = row_type(-a.sin, a.cos, math_type::zero);
		M[2] = row_type::unit_z;
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（ZYX）
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_roll_pitch_yaw(T roll, T pitch, T yaw)
	{
		sin_cos_type r = roll;
		sin_cos_type p = pitch;
		sin_cos_type y = yaw;

		M[0] = row_type(r.cos * y.cos + r.sin * p.sin * y.sin,
			r.sin * p.cos,
			r.cos * -y.sin + r.sin * p.sin * y.cos);
		M[1] = row_type(-r.sin * y.cos + r.cos * p.sin * y.sin,
			r.cos * p.cos,
			r.sin * y.sin + r.cos * p.sin * y.cos);
		M[2] = row_type(p.cos * y.sin, -p.sin, p.cos * y.cos);
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（クォータニオン）
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_quaternion(const quaternion<T>& q)
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

		M[0] = row_type(math_type::one - (yy + zz), xy + wz, xz - wy);
		M[1] = row_type(xy - wz, math_type::one - (xx + zz), yz + wz);
		M[2] = row_type(xz + wy, yz + wx, math_type::one - (xx + yy));
		return *this;
	}
	/*---------------------------------------------------------------------
	* 回転行列にする（任意軸＋角度）
	*---------------------------------------------------------------------*/
	matrix3x3& load_rotate_axis_angle(const vector3<T>& axis, T angle)
	{
		// 四元数での計算を行なう
		return load_rotate_quaternion(quaternion<T>(axis, angle));
	}
	/*---------------------------------------------------------------------
	* 座標変換行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_translate(T x, T y)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		_position = vector2<T>(x, y);
#else
		// ４の位置が座標を扱う要素
		vector3<T>& m = M[2];
		m.x = x;
		m.y = y;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}
	matrix3x3& load_translate(const vector2<T>& v)
	{
		load_identity();
#ifdef _USE_ANONYMOUS_NON_POD
		_position = v;
#else
		vector3<T>& m = M[2];
		m.x = v.x;
		m.y = v.y;
#endif // _USE_ANONYMOUS_NON_POD
		return *this;
	}

	/*---------------------------------------------------------------------
	* ワールド変換行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_world(const vector2<T>& s, T rotate, const vector2<T>& t)
	{
		sin_cos_type a = rotate;
		M[0] = row_type(s.x * a.cos, s.x * a.sin, math_type::zero);
		M[1] = row_type(s.y * -a.sin, s.y * a.cos, math_type::zero);
		M[2] = row_type(t.x, t.y, math_type::one);
		return *this;
	}

	/*---------------------------------------------------------------------
	* 拡大縮小と回転を行なう行列にする
	*---------------------------------------------------------------------*/
	matrix3x3& load_scale_rotate(const vector2<T>& s, T rotate)
	{
		sin_cos_type a = rotate;
		M[0] = row_type(s.x * a.cos, s.x * a.sin, math_type::zero);
		M[1] = row_type(s.y * -a.sin, s.y * a.cos, math_type::zero);
		M[2] = row_type::unit_z;
		return *this;
	}
	matrix3x3& load_scale_rotate(const vector3<T>& s, T roll, T pitch, T yaw)
	{
		sin_cos_type r = roll;
		sin_cos_type p = pitch;
		sin_cos_type y = yaw;

		M[0] = row_type(s.x * r.cos * y.cos + s.x * r.sin * p.sin * y.sin,
			s.x * r.sin * p.cos,
			s.x * r.cos * -y.sin + s.x * r.sin * p.sin * y.cos);
		M[1] = row_type(s.y * -r.sin * y.cos + s.y * r.cos * p.sin * y.sin,
			s.y * r.cos * p.cos,
			s.y * r.sin * y.sin + s.y * r.cos * p.sin * y.cos);
		M[2] = row_type(s.z * p.cos * y.sin, s.z * -p.sin, s.z * p.cos * y.cos);
		return *this;
	}
	matrix3x3& load_scale_rotate(const vector3<T>& s, const quaternion<T>& q)
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

		M[0] = row_type(s.x * (math_type::one - yy + zz), s.x * (xy + wz), s.x * (xz - wy));
		M[1] = row_type(s.y * (xy - wz), s.y * (math_type::one - xx + zz), s.y * (yz + wz));
		M[2] = row_type(s.z * (xz + wy), s.z * (yz + wx), s.z * (math_type::one - xx + yy));
		return *this;
	}

	/*---------------------------------------------------------------------
	* 転置行列
	*---------------------------------------------------------------------*/
	matrix3x3& transpose()
	{
		row_type& v0 = M[0];
		row_type& v1 = M[1];
		row_type& v2 = M[2];

		// [0][1] = [1][0]
		T tmp = v0.y;
		v0.y = v1.x;
		v1.x = tmp;
		// [0][2] = [2][0]
		tmp = v0.z;
		v0.z = v2.x;
		v2.x = tmp;
		// [1][2] = [2][1]
		tmp = v1.z;
		v1.z = v2.y;
		v2.y = tmp;

		return *this;
	}
	matrix3x3& transpose(matrix3x3& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		row_type& r0 = result.M[0];
		row_type& r1 = result.M[1];
		row_type& r2 = result.M[2];

		// [0][1] = [1][0]
		r0.y = v1.x;
		r1.x = v0.y;
		// [0][2] = [2][0]
		r0.z = v2.x;
		r2.x = v0.z;
		// [1][2] = [2][1]
		r1.z = v2.y;
		r2.y = v1.z;

		r0.x = v0.x;
		r1.y = v1.y;
		r2.z = v2.z;

		return result;
	}
	/*---------------------------------------------------------------------
	* 転置行列を求める
	*---------------------------------------------------------------------*/
	matrix3x3 transposed() const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];
		return matrix3x3(v0.x, v1.x, v2.x,
			v0.y, v1.y, v2.y,
			v0.z, v1.z, v2.z);
	}

	/*---------------------------------------------------------------------
	* ベクトル座標変換
	*---------------------------------------------------------------------*/
	vector2<T> transform(const vector2<T>& v) const
	{
		vector2<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	vector2<T>& transform(const vector2<T>& v, vector2<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];

		result.x = v0.x * v.x + v1.x * v.y + v2.x;
		result.y = v0.y * v.x + v1.y * v.y + v2.y;
		result.z = v0.z * v.x + v1.z * v.y + v2.z;

		return result;
	}
	/*---------------------------------------------------------------------
	* ベクトル座標変換（0～1）
	*---------------------------------------------------------------------*/
	vector2<T> transform_coord(const vector2<T>& v) const
	{
		vector2<T> r(behavior::noinitialize);
		return transform_coord(v, r);
	}
	vector2<T>& transform_coord(const vector2<T>& v, vector2<T>& result) const
	{
		const row_type& v0 = M[0];
		const row_type& v1 = M[1];
		const row_type& v2 = M[2];

		T w = math_type::reciprocal(v0.z * v.x + v1.z * v.y + v2.z);
		result.x = (v0.x * v.x + v1.x * v.y + v2.x) * w;
		result.y = (v0.y * v.x + v1.y * v.y + v2.y) * w;

		return result;
	}
	/*---------------------------------------------------------------------
	* 法線ベクトル座標変換
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

		result.x = v0.x * v.x + v1.x * v.y + v2.x * v.z;
		result.y = v0.y * v.x + v1.y * v.y + v2.y * v.z;
		result.z = v0.z * v.x + v1.z * v.y + v2.z * v.z;

		return result;
	}
	vector3<T> transform_normal(const vector3<T>& v) const
	{
		vector3<T> r(behavior::noinitialize);
		return transform(v, r);
	}
	vector3<T>& transform_normal(const vector3<T>& v, vector3<T>& result) const
	{
		return transform(v, result);
	}


	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	vector3<T>& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
		return M[i];
	}
	const vector3<T>& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
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
	_CXX11_EXPLICIT operator matrix3x3<U>() const
	{
		typedef vector3<U> other_vector3_type;
		return matrix3x3<U>(static_cast<other_vector3_type>(M[0]),
			static_cast<other_vector3_type>(M[1]),
			static_cast<other_vector3_type>(M[2]));
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
	bool operator == (const matrix3x3& v) const
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
	bool operator != (const matrix3x3& v) const
	{
		return !(*this == v);
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	matrix3x3 operator + () const
	{
		return *this;
	}
	matrix3x3 operator - () const
	{
		return matrix3x3(-M[0], -M[1], -M[2]);
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	matrix3x3 operator + (const matrix3x3& m) const
	{
		matrix3x3 result(behavior::noinitialize);
		return add(m, result);
	}
	matrix3x3 operator - (const matrix3x3& m) const
	{
		matrix3x3 result(behavior::noinitialize);
		return subtract(m, result);
	}
	matrix3x3 operator * (const matrix3x3& m) const
	{
		matrix3x3 result(behavior::noinitialize);
		return multiply(m, result);
	}
	matrix3x3 operator * (T f) const
	{
		matrix3x3 result(behavior::noinitialize);
		return multiply(f, result);
	}
	matrix3x3 operator / (T f) const
	{
		matrix3x3 result(behavior::noinitialize);
		return divide(f, result);
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	matrix3x3& operator = (const behavior::_zero_t&)
	{
		return load_zero();
	}
	matrix3x3& operator = (const behavior::_identity_t&)
	{
		return load_identity();
	}
#ifdef _USE_CXX11
	matrix3x3& operator = (const std::initializer_list<T>& t)
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
#endif // _USE_CXX11

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	matrix3x3& operator += (const matrix3x3& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i += *oi;
		}
		return *this;
	}
	matrix3x3& operator -= (const matrix3x3& m)
	{
		const_iterator oi = m.M.begin();
		for (iterator i = M.begin(), end = M.end(); i != end; ++i, ++oi)
		{
			*i -= *oi;
		}
		return *this;
	}
	matrix3x3& operator *= (const matrix3x3& m)
	{
		const matrix3x3 c = *this;
		return c.multiply(m, *this);
	}
	matrix3x3& operator *= (T s)
	{
		for (iterator i = M.begin(), end = M.end(); i != end; ++i)
		{
			i->x *= s;
			i->y *= s;
			i->z *= s;
		}
		return *this;
	}
	matrix3x3& operator /= (T s)
	{
		_DEB_ASSERT(s != math_type::zero);
		return operator*=(math_type::reciprocal(s));
	}
};

template <typename T>
const matrix3x3<T> matrix3x3<T>::zero(math_type::zero);
template <typename T>
const matrix3x3<T> matrix3x3<T>::identity(math_type::one);

/*---------------------------------------------------------------------
* vector2.transform
*---------------------------------------------------------------------*/
template <typename T> inline
vector2<T>& vector2<T>::transform(const matrix3x3<T>& m)
{
	const vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector2<T>& vector2<T>::transform(const matrix3x3<T>& m, vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector2<T> vector2<T>::transformed(const matrix3x3<T>& m) const
{
	vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
vector2<T>& vector2<T>::transform_coord(const matrix3x3<T>& m)
{
	const vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector2<T>& vector2<T>::transform_coord(const matrix3x3<T>& m, vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector2<T> vector2<T>::transformed_coord(const matrix3x3<T>& m) const
{
	vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
vector2<T>& vector2<T>::transform_normal(const matrix3x3<T>& m)
{
	const vector2<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector2<T>& vector2<T>::transform_normal(const matrix3x3<T>& m, vector2<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector2<T> vector2<T>::transformed_normal(const matrix3x3<T>& m) const
{
	vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* vector3.transform
*---------------------------------------------------------------------*/
template <typename T> inline
vector3<T>& vector3<T>::transform(const matrix3x3<T>& m)
{
	const vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector3<T>& vector3<T>::transform(const matrix3x3<T>& m, vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector3<T> vector3<T>::transformed(const matrix3x3<T>& m) const
{
	vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
vector3<T>& vector3<T>::transform_normal(const matrix3x3<T>& m)
{
	const vector3<T> v = *this;
	return m.transform(v, *this);
}
template <typename T> inline
vector3<T>& vector3<T>::transform_normal(const matrix3x3<T>& m, vector3<T>& result) const
{
	return m.transform(*this, result);
}
template <typename T> inline
vector3<T> vector3<T>::transformed_normal(const matrix3x3<T>& m) const
{
	vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* ベクトルとの二項演算子
*---------------------------------------------------------------------*/
template <typename T> inline
vector2<T> vector2<T>::operator * (const matrix3x3<T>& m) const
{
	vector2<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}
template <typename T> inline
vector3<T> vector3<T>::operator * (const matrix3x3<T>& m) const
{
	vector3<T> r(behavior::noinitialize);
	return m.transform(*this, r);
}

/*---------------------------------------------------------------------
* 行列からクォータニオンを求める
*---------------------------------------------------------------------*/
template <typename T> inline
quaternion<T>& quaternion<T>::from_matrix(const matrix3x3<T>& m)
{
	const vector3<T>* v0 = &m.M[0];
	const vector3<T>* v1 = &m.M[1];
	const vector3<T>* v2 = &m.M[2];

	T tr = v0->x + v1->y + v2->z + math_type::one;
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

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const matrix3x3<T>& v)
{
	//{
	//	V0
	//	V1
	//	V2
	//}
	os << io::braces_left << io::line;
	for (typename matrix3x3<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, matrix3x3<T>& v)
{
	is.ignore();
	for (typename matrix3x3<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const matrix3x3<T>& v)
{
	os << io::braces_left << io::line;
	for (typename matrix3x3<T>::const_iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, matrix3x3<T>& v)
{
	is.ignore();
	for (typename matrix3x3<T>::iterator i = v.M.begin(), end = v.M.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_MATRIX3X3_H__
