#ifndef __POCKET_MATH_FRUSTUM_H__
#define __POCKET_MATH_FRUSTUM_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../debug.h"
#include "../behavior.h"
#include "array.h"
#include "Math.h"
#include "Vector3.h"
#include "Plane.h"
#include "Matrix4x4.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

template <typename> struct Frustum;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Frustum<float> Frustumf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Frustum<double> Frustumd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Frustum<long double> Frustumld;
#endif // _USING_MATH_LONG_DOUBLE

#ifdef _USE_CXX11
template <typename T>
using frustum = Frustum<T>;
#ifndef _UNUSING_MATH_INT_FLOAT
using frustumf = frustum<float>;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
using frustumd = frustum<double>;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
using frustumld = frustum<long double>;
#endif // _USING_MATH_LONG_DOUBLE
#endif // _USE_CXX11

template <typename T>
struct Frustum
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef Math<T> math_type;
	typedef container::array<Plane<T>, 6> array_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;
	typedef Plane<T> plane_type;
	typedef typename plane_type::IntersectType IntersectType;

	enum Index
	{
		eLeft,
		eRight,
		eUp,
		eDown,
		eNear,
		eFar
	};

	struct LookTo
	{
		Vector3<T> Eye;
		Vector3<T> Direction;
		Vector3<T> Up;

		LookTo() :
			Up(Vector3<T>::Up)
		{

		}
		LookTo(const Vector3<T>& eye, const Vector3<T>& direction, const Vector3<T>& up = Vector3<T>::Up) :
			Eye(eye), Direction(direction), Up(up)
		{

		}
	};
	struct LookAt
	{
		Vector3<T> Eye;
		Vector3<T> Center;
		Vector3<T> Up;

		LookAt() :
			Up(Vector3<T>::Up)
		{

		}
		LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up = Vector3<T>::Up) :
			Eye(eye), Center(center), Up(up)
		{

		}
	};
	struct ProjectionFieldOfView
	{
		T FovY;
		T Aspect;
		T Near;
		T Far;

		ProjectionFieldOfView()
		{

		}
		ProjectionFieldOfView(T fy, T a, T n, T f) :
			FovY(fy), Aspect(a), Near(n), Far(f)
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

			array_type Planes;

#ifdef _USE_ANONYMOUS_NON_POD
		};
		struct
		{
			Plane<T> Left;
			Plane<T> Right;
			Plane<T> Up;
			Plane<T> Down;
			Plane<T> Near;
			Plane<T> Far;
		};
	};
#endif // _USE_ANONYMOUS_NON_POD

	template <typename> friend struct Frustum;

	/*---------------------------------------------------------------------------------------
	* Constants
	*---------------------------------------------------------------------------------------*/

	// None

	/*---------------------------------------------------------------------------------------
	* Constructors
	*---------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(Frustum);
	explicit Frustum(const behavior::_noinitialize_t&)
	{

	}
	Frustum(const Plane<T>& left, const Plane<T>& right, const Plane<T>& up, const Plane<T>& down, const Plane<T>& near, const Plane<T>& far)
#ifdef _USE_ANONYMOUS_NON_POD
		: Left(left), Right(right),
		Up(up), Down(down),
		Near(near), Far(far)
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		Planes[0] = left;
		Planes[1] = right;
		Planes[2] = up;
		Planes[3] = down;
		Planes[4] = near;
		Planes[5] = far;
#endif // _USE_ANONYMOUS_NON_POD
	}
	template <typename U>
	Frustum(const Plane<U>& left, const Plane<U>& right, const Plane<U>& up, const Plane<U>& down, const Plane<U>& near, const Plane<U>& far)
#ifdef _USE_ANONYMOUS_NON_POD
		: Left(static_cast<Plane<T> >(left)), Right(static_cast<Plane<T> >(right)),
		Up(static_cast<Plane<T> >(up)), Down(static_cast<Plane<T> >(down)),
		Near(static_cast<Plane<T> >(near)), Far(static_cast<Plane<T> >(far))
#endif // _USE_ANONYMOUS_NON_POD
	{
#ifndef _USE_ANONYMOUS_NON_POD
		Planes[0] = static_cast<Plane<T> >(left);
		Planes[1] = static_cast<Plane<T> >(right);
		Planes[2] = static_cast<Plane<T> >(up);
		Planes[3] = static_cast<Plane<T> >(down);
		Planes[4] = static_cast<Plane<T> >(near);
		Planes[5] = static_cast<Plane<T> >(far);
#endif // _USE_ANONYMOUS_NON_POD
	}

	explicit Frustum(const LookAt& lookat, const ProjectionFieldOfView& fov)
	{
		from_view_projection(lookat, fov);
	}
	explicit Frustum(const LookTo& lookto, const ProjectionFieldOfView& fov)
	{
		from_view_projection(lookto, fov);
	}
	explicit Frustum(const Matrix4x4<T>& view, const Matrix4x4<T>& projection)
	{
		from_view_projection_matrix(view, projection);
	}
	explicit Frustum(const Matrix4x4<T>& clip)
	{
		from_clip_matrix(clip);
	}

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 行列を求めるのに必要な情報から求める
	*---------------------------------------------------------------------*/
	Frustum& from_view_projection(const LookAt& lookat, const ProjectionFieldOfView& fov)
	{
		Matrix4x4<T> view(behavior::noinitialize), proj(behavior::noinitialize);
		view.load_lookat(lookat.Eye, lookat.Center, lookat.Up);
		proj.load_perspective_field_of_view(fov.FovY, fov.Aspect, fov.Near, fov.Far);

		return from_view_projection_matrix(view, proj);
	}
	Frustum& from_view_projection(const LookTo& lookto, const ProjectionFieldOfView& fov)
	{
		Matrix4x4<T> view(behavior::noinitialize), proj(behavior::noinitialize);
		view.load_lookto(lookto.Eye, lookto.Direction, lookto.Up);
		proj.load_perspective_field_of_view(fov.FovY, fov.Aspect, fov.Near, fov.Far);

		return from_view_projection_matrix(view, proj);
	}
	/*---------------------------------------------------------------------
	* 視野変換行列と射影変換行列から求める
	*---------------------------------------------------------------------*/
	Frustum& from_view_projection_matrix(const Matrix4x4<T>& view, const Matrix4x4<T>& projection)
	{
		// クリップ行列を計算
		Matrix4x4<T> clip(behavior::noinitialize);
		view.multiply(projection, clip);

		return from_clip_matrix(clip);
	}
	/*---------------------------------------------------------------------
	* クリップ座標変換行列から求める
	*---------------------------------------------------------------------*/
	Frustum& from_clip_matrix(const Matrix4x4<T>& clip)
	{
		const Vector4<T>* v0 = &clip[0];
		const Vector4<T>* v1 = &clip[1];
		const Vector4<T>* v2 = &clip[2];
		const Vector4<T>* v3 = &clip[3];

		// それぞれの面情報を計算
		Plane<T>* p = &Planes[0];

		// 左
		p->Normal.X = v0->W + v0->X;
		p->Normal.Y = v1->W + v1->X;
		p->Normal.Z = v2->W + v2->X;
		p->D = v3->W + v3->X;
		p->normalize();

		// 右
		p = &Planes[1];
		p->Normal.X = v0->W - v0->X;
		p->Normal.Y = v1->W - v1->X;
		p->Normal.Z = v2->W - v2->X;
		p->D = v3->W - v3->X;
		p->normalize();

		// 上
		p = &Planes[2];
		p->Normal.X = v0->W - v0->Y;
		p->Normal.Y = v1->W - v1->Y;
		p->Normal.Z = v2->W - v2->Y;
		p->D = v3->W - v3->Y;
		p->normalize();

		// 下
		p = &Planes[3];
		p->Normal.X = v0->W + v0->Y;
		p->Normal.Y = v1->W + v1->Y;
		p->Normal.Z = v2->W + v2->Y;
		p->D = v3->W + v3->Y;
		p->normalize();

		// 近
		p = &Planes[4];
		p->Normal.X = v0->W - v0->Z;
		p->Normal.Y = v1->W - v1->Z;
		p->Normal.Z = v2->W - v2->Z;
		p->D = v3->W - v3->Z;
		p->normalize();

		// 遠
		p = &Planes[5];
		p->Normal.X = v0->W + v0->Z;
		p->Normal.Y = v1->W + v1->Z;
		p->Normal.Z = v2->W + v2->Z;
		p->D = v3->W + v3->Z;
		p->normalize();

		return *this;
	}

	/*---------------------------------------------------------------------
	* 座標が中に存在しているか
	*---------------------------------------------------------------------*/
	bool is_inside_point(const Vector3<T>& point) const
	{
		for (const_iterator i = Planes.begin(), end = Planes.end(); i != end; ++i)
		{
			// 背面に存在している
			if (i->intersect_point(point) == Plane<T>::eOnBackward)
			{
				return false;
			}
		}
		return true;
	}
	/*---------------------------------------------------------------------
	* すべての座標が中に存在しているか
	*---------------------------------------------------------------------*/
	bool is_inside_points(const Vector3<T>* points, int n) const
	{
		int j;
		for (const_iterator i = Planes.begin(), end = Planes.end(); i != end; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				if (i->intersect_point(points[j]) == Plane<T>::eOnBackward)
				{
					return false;
				}
			}
		}
		return true;
	}
	template <size_t S>
	bool is_inside_points(const Vector3<T>(&points)[S]) const
	{
		return is_inside_points(&points[0], static_cast<int>(S));
	}
	template <size_t S, template<typename, size_t> class ARRAY>
	bool is_inside_points(const ARRAY<Vector3<T>, S>& points) const
	{
		typedef typename ARRAY<Vector3<T>, S>::const_iterator array_iterator;

		array_iterator array_begin = points.begin();
		array_iterator array_end = points.end();
		array_iterator j;
		for (const_iterator i = Planes.begin(), end = Planes.end(); i != end; ++i)
		{
			for (j = array_begin; j != array_end; ++j)
			{
				if (i->intersect_point(*j) == Plane<T>::eOnBackward)
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
	bool is_inside_sphere(const Vector3<T>& center, T radius) const
	{
		for (const_iterator i = Planes.begin(), end = Planes.end(); i != end; ++i)
		{
			if (i->intersect_sphere(center, radius) == Plane<T>::eOnBackward)
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
		return Planes[5].D - Planes[4].D;
	}

	/*---------------------------------------------------------------------
	* 向いている方向を求める
	*---------------------------------------------------------------------*/
	const Vector3<T>& direction() const
	{
		return Planes[4].Normal;
	}
	Vector3<T>& direction(Vector3<T>& result) const
	{
		result = Planes[4].Normal;
		return result;
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	Plane<T>& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 5);
		return Planes[i];
	}
	const Plane<T>& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 5);
		return Planes[i];
	}
	Plane<T>& operator [] (typename Frustum::Index i)
	{
		_DEB_RANGE_ASSERT(i, eLeft, eNear);
		return Planes[i];
	}
	const Plane<T>& operator [] (typename Frustum::Index i) const
	{
		_DEB_RANGE_ASSERT(i, eLeft, eNear);
		return Planes[i];
	}

	/*---------------------------------------------------------------------
	* 変換演算子
	*---------------------------------------------------------------------*/
	_CXX11_EXPLICIT operator Plane<T>* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Left;
#else
		return &Planes[0];
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const Plane<T>* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Left;
#else
		return &Planes[0];
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Left.Normal.X;
#else
		return &Planes[0].Normal.X;
#endif // _USE_ANONYMOUS_NON_POD
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS_NON_POD
		return &Left.Normal.X;
#else
		return &Planes[0].Normal.X;
#endif // _USE_ANONYMOUS_NON_POD
	}

	/*---------------------------------------------------------------------
	* 比較演算子
	*---------------------------------------------------------------------*/
	bool operator == (const Frustum& f) const
	{
		const_iterator oi = f.Planes.begin();
		for (const_iterator i = Planes.begin(), end = Planes.end(); i != end; ++i, ++oi)
		{
			if (*i != *oi)
			{
				return false;
			}
		}
		return true;
	}
	bool operator != (const Frustum& f) const
	{
		return !(*this == f);
	}
};

/*---------------------------------------------------------------------
* 座標が中に存在しているか演算子
*---------------------------------------------------------------------*/
template <typename T> inline
bool operator << (const Frustum<T>& f, const Vector3<T>& v)
{
	return f.is_inside_point(v);
}
template <typename T> inline
bool operator >> (const Vector3<T>& v, const Frustum<T>& f)
{
	return f << v;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
bool operator << (const Frustum<T>& f, const ARRAY<T, N>& v)
{
	return f.is_inside_points(v);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
bool operator >> (const ARRAY<T, N>& v, const Frustum<T>& f)
{
	return f << v;
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const Frustum<T>& v)
{
	os << out_char::braces_left << out_char::line;
	for (typename Frustum<T>::const_iterator i = v.Planes.begin(), end = v.Planes.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, Frustum<T>& v)
{
	is.ignore();
	for (typename Frustum<T>::iterator i = v.Planes.begin(), end = v.Planes.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const Frustum<T>& v)
{
	os << out_char::braces_left << out_char::line;
	for (typename Frustum<T>::const_iterator i = v.Planes.begin(), end = v.Planes.end(); i != end; ++i)
	{
		os << out_char::tab << *i << out_char::line;
	}
	os << out_char::braces_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, Frustum<T>& v)
{
	is.ignore();
	for (typename Frustum<T>::iterator i = v.Planes.begin(), end = v.Planes.end(); i != end; ++i)
	{
		is >> *i;
	}
	is.ignore();
	return is;
}
#endif // _USING_MATH_IO

} // namespace pocket

#endif // __POCKET_MATH_FRUSTUM_H__
