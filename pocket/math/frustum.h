#ifndef __POCKET_MATH_FRUSTUM_H__
#define __POCKET_MATH_FRUSTUM_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "../container/array.h"
#include "math_traits.h"
#include "vector3.h"
#include "plane.h"
#include "matrix4x4.h"
#include "../io.h"

namespace pocket
{

template <typename> struct frustum;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef frustum<float> frustumf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef frustum<double> frustumd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef frustum<long double> frustumld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct frustum
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef plane<T> plane_type;
	typedef matrix4x4<T> matrix4x4_type;
	typedef typename plane_type::intersect_result intersect_result;

	typedef container::array<plane_type, 6> array_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	struct look_to_t
	{
		vector3<T> eye;
		vector3<T> direction;
		vector3<T> up;

		look_to_t() :
			up(vector3<T>::up)
		{

		}
		look_to_t(const vector3<T>& eye, const vector3<T>& direction, const vector3<T>& up = vector3<T>::up) :
			eye(eye), direction(direction), up(up)
		{

		}
	};
	struct look_at_t
	{
		vector3<T> eye;
		vector3<T> center;
		vector3<T> up;

		look_at_t() :
			up(vector3<T>::up)
		{

		}
		look_at_t(const vector3<T>& eye, const vector3<T>& center, const vector3<T>& up = vector3<T>::up) :
			eye(eye), center(center), up(up)
		{

		}
	};
	struct projection_field_of_view_t
	{
		T fovy;
		T aspect;
		T n;
		T f;

		projection_field_of_view_t()
		{

		}
		projection_field_of_view_t(T fy, T a, T n, T f) :
			fovy(fy), aspect(a), n(n), f(f)
		{

		}
	};

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS_NON_POD
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS_NON_POD

			array_type planes;

#ifdef _USE_ANONYMOUS_NON_POD
		};
		struct
		{
			plane_type left;
			plane_type right;
			plane_type up;
			plane_type down;
			plane_type n;
			plane_type f;
		};
	};
#endif // _USE_ANONYMOUS_NON_POD

	template <typename> friend struct frustum;

	/*---------------------------------------------------------------------------------------
	* Constants
	*---------------------------------------------------------------------------------------*/

	// none

	/*---------------------------------------------------------------------------------------
	* Constructors
	*---------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(frustum);
	explicit frustum(const behavior::_noinitialize_t&)
	{

	}
	frustum(const plane_type& left, const plane_type& right, const plane_type& up, const plane_type& down, const plane_type& n, const plane_type& f)
#ifdef _USE_ANONYMOUS_NON_POD
		: left(left), right(right),
		up(up), down(down),
		n(n), f(f)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		planes[0] = left;
		planes[1] = right;
		planes[2] = up;
		planes[3] = down;
		planes[4] = n;
		planes[5] = f;
#endif // _USE_ANONYMOUS_NON_POD
	}
	template <typename U>
	frustum(const plane<U>& left, const plane<U>& right, const plane<U>& up, const plane<U>& down, const plane<U>& n, const plane<U>& f)
#ifdef _USE_ANONYMOUS_NON_POD
		: left(static_cast<plane_type>(left)), right(static_cast<plane_type>(right)),
		up(static_cast<plane_type>(up)), down(static_cast<plane_type>(down)),
		n(static_cast<plane_type>(n)), f(static_cast<plane_type>(f))
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		planes[0] = static_cast<plane_type>(left);
		planes[1] = static_cast<plane_type>(right);
		planes[2] = static_cast<plane_type>(up);
		planes[3] = static_cast<plane_type>(down);
		planes[4] = static_cast<plane_type>(n);
		planes[5] = static_cast<plane_type>(f);
#endif // _USE_ANONYMOUS_NON_POD
	}

	explicit frustum(const look_at_t& lookat, const projection_field_of_view_t& fov)
	{
		from_view_projection(lookat, fov);
	}
	explicit frustum(const look_to_t& lookto, const projection_field_of_view_t& fov)
	{
		from_view_projection(lookto, fov);
	}
	explicit frustum(const matrix4x4_type& view, const matrix4x4_type& projection)
	{
		from_view_projection_matrix(view, projection);
	}
	explicit frustum(const matrix4x4_type& clip)
	{
		from_clip_matrix(clip);
	}

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 行列を求めるのに必要な情報から求める
	*---------------------------------------------------------------------*/
	frustum& from_view_projection(const look_to_t& lookto, const projection_field_of_view_t& fov)
	{
		matrix4x4_type view(behavior::noinitialize), proj(behavior::noinitialize);
		view.load_lookto(lookto.eye, lookto.direction, lookto.up);
		proj.load_perspective_field_of_view(fov.fovy, fov.aspect, fov.n, fov.f);

		return from_view_projection_matrix(view, proj);
	}
	frustum& from_view_projection(const look_at_t& lookat, const projection_field_of_view_t& fov)
	{
		matrix4x4_type view(behavior::noinitialize), proj(behavior::noinitialize);
		view.load_lookat(lookat.eye, lookat.center, lookat.up);
		proj.load_perspective_field_of_view(fov.fovy, fov.aspect, fov.n, fov.f);

		return from_view_projection_matrix(view, proj);
	}
	/*---------------------------------------------------------------------
	* 視野変換行列と射影変換行列から求める
	*---------------------------------------------------------------------*/
	frustum& from_view_projection_matrix(const matrix4x4_type& view, const matrix4x4_type& projection)
	{
		// クリップ行列を計算
		matrix4x4_type clip(behavior::noinitialize);
		view.multiply(projection, clip);

		return from_clip_matrix(clip);
	}
	/*---------------------------------------------------------------------
	* クリップ座標変換行列から求める
	*---------------------------------------------------------------------*/
	frustum& from_clip_matrix(const matrix4x4_type& clip)
	{
		const vector4<T>& v0 = clip[0];
		const vector4<T>& v1 = clip[1];
		const vector4<T>& v2 = clip[2];
		const vector4<T>& v3 = clip[3];

		// それぞれの面情報を計算

		// 左
		plane_type& p0 = planes[0];
		p0 = plane_type(v0.w + v0.x, v1.w + v1.x, v2.w + v2.x, v3.w + v3.x);
		p0.normalize();

		// 右
		plane_type& p1 = planes[1];
		p1 = plane_type(v0.w - v0.x, v1.w - v1.x, v2.w - v2.x, v3.w - v3.x);
		p1.normalize();

		// 上
		plane_type& p2 = planes[2];
		p2 = plane_type(v0.w - v0.y, v1.w - v1.y, v2.w - v2.y, v3.w - v3.y);
		p2.normalize();

		// 下
		plane_type& p3 = planes[3];
		p3 = plane_type(v0.w + v0.y, v1.w + v1.y, v2.w + v2.y, v3.w + v3.y);
		p3.normalize();

		// 近
		plane_type& p4 = planes[4];
		p4 = plane_type(v0.w - v0.z, v1.w - v1.z, v2.w - v2.z, v3.w - v3.z);
		p4.normalize();

		// 遠
		plane_type& p5 = planes[5];
		p5 = plane_type(v0.w + v0.z, v1.w + v1.z, v2.w + v2.z, v3.w + v3.z);
		p5.normalize();
		return *this;
	}

	/*---------------------------------------------------------------------
	* 座標が中に存在しているか
	*---------------------------------------------------------------------*/
	bool is_inside_point(const vector3<T>& point) const
	{
		for (const_iterator i = planes.begin(), end = planes.end(); i != end; ++i)
		{
			// 背面に存在している
			if (i->intersect_point(point) == plane_type::on_backward)
			{
				return false;
			}
		}
		return true;
	}
	/*---------------------------------------------------------------------
	* すべての座標が中に存在しているか
	*---------------------------------------------------------------------*/
	bool is_inside_points(const vector3<T>* points, int n) const
	{
		int j;
		for (const_iterator i = planes.begin(), end = planes.end(); i != end; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if (i->intersect_point(points[j]) == plane_type::on_backward)
				{
					return false;
				}
			}
		}
		return true;
	}
	template <size_t S>
	bool is_inside_points(const vector3<T>(&points)[S]) const
	{
		return is_inside_points(&points[0], static_cast<int>(S));
	}
	template <size_t S, template <typename, size_t> class ARRAY>
	bool is_inside_points(const ARRAY<vector3<T>, S>& points) const
	{
		typedef typename ARRAY<vector3<T>, S>::const_iterator array_iterator;

		array_iterator array_begin = points.begin();
		array_iterator array_end = points.end();
		array_iterator j;
		for (const_iterator i = planes.begin(), end = planes.end(); i != end; ++i)
		{
			for (j = array_begin; j != array_end; ++j)
			{
				if (i->intersect_point(*j) == plane_type::on_backward)
				{
					return false;
				}
			}
		}
		return true;
	}

	/*---------------------------------------------------------------------
	* 球が中に存在しているか
	*---------------------------------------------------------------------*/
	bool is_inside_sphere(const vector3<T>& center, T radius) const
	{
		for (const_iterator i = planes.begin(), end = planes.end(); i != end; ++i)
		{
			if (i->intersect_sphere(center, radius) == plane_type::on_backward)
			{
				return false;
			}
		}
		return true;
	}

	/*---------------------------------------------------------------------
	* 近くの面から遠くの面までの距離を求める
	*---------------------------------------------------------------------*/
	T distance() const
	{
		return planes[5].d - planes[4].d;
	}

	/*---------------------------------------------------------------------
	* 向いている方向を求める
	*---------------------------------------------------------------------*/
	const vector3<T>& direction() const
	{
		return planes[4].normal();
	}
	vector3<T>& direction(vector3<T>& result) const
	{
		result = planes[4].normal();
		return result;
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	plane_type& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 5);
		return planes[i];
	}
	const plane_type& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 5);
		return planes[i];
	}

	/*---------------------------------------------------------------------
	* 変換演算子
	*---------------------------------------------------------------------*/
	_CXX11_EXPLICIT operator plane_type* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &left;
#else
		return &planes[0];
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const plane_type* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &left;
#else
		return &planes[0];
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &left.a;
#else
		return &planes[0].a;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &left.a;
#else
		return &planes[0].a;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const frustum& f) const
	{
		const_iterator oi = f.planes.begin();
		for (const_iterator i = planes.begin(), end = planes.end(); i != end; ++i, ++oi)
		{
			if (*i != *oi)
			{
				return false;
			}
		}
		return true;
	}
	bool operator != (const frustum& f) const
	{
		return !(*this == f);
	}
};

/*---------------------------------------------------------------------
* 座標が中に存在しているか演算子
*---------------------------------------------------------------------*/
template <typename T> inline
bool operator << (const frustum<T>& f, const vector3<T>& v)
{
	return f.is_inside_point(v);
}
template <typename T> inline
bool operator >> (const vector3<T>& v, const frustum<T>& f)
{
	return f << v;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
bool operator << (const frustum<T>& f, const ARRAY<T, N>& v)
{
	return f.is_inside_points(v);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
bool operator >> (const ARRAY<T, N>& v, const frustum<T>& f)
{
	return f << v;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const frustum<T>& v)
{
	os << io::braces_left << io::line_feed;
	for (typename frustum<T>::const_iterator i = v.planes.begin(), end = v.planes.end(); i != end; ++i)
	{
		os << io::tab << *i << io::line_feed;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, frustum<T>& v)
{
	is.ignore();
	for (typename frustum<T>::iterator i = v.planes.begin(), end = v.planes.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}

} // namespace pocket

#endif // __POCKET_MATH_FRUSTUM_H__
