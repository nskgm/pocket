#ifndef __POCKET_MATH_FORWARD_H__
#define __POCKET_MATH_FORWARD_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../fwd.h"
#include <complex>

namespace pocket
{

/*------------------------------------------------------------------------------------------
* Math
*------------------------------------------------------------------------------------------*/
template <typename> class Math;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Math<int> Mathi;
typedef Math<float> Mathf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Math<double> Mathd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Math<long double> Mathld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector2
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector2;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector2<int> Point2;
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector2<double> Vector2d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector2<long double> Vector2ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector3
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector3;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector3<int> Point3;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector3<double> Vector3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector3<long double> Vector3ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Vector4
*------------------------------------------------------------------------------------------*/
template <typename> struct Vector4;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Vector4<int> Point4;
typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Vector4<double> Vector4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Vector4<long double> Vector4ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Matrix4x4
*------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------
* Quaternion
*------------------------------------------------------------------------------------------*/
template <typename> struct Quaternion;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Quaternion<float> Quaternionf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Quaternion<double> Quaterniond;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Quaternion<long double> Quaternionld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Plane
*------------------------------------------------------------------------------------------*/
template <typename> struct Plane;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Plane<float> Planef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Plane<double> Planed;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Plane<long double> Planeld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Color
*------------------------------------------------------------------------------------------*/
template <typename> struct Color;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Color<float> Colorf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Color<double> Colord;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Color<long double> Colorld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Range, Rectangle
*------------------------------------------------------------------------------------------*/
template <typename> struct Range;
template <typename> struct Rectangle;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Range<int> Rangei;
typedef Range<float> Rangef;
typedef Rectangle<int> Rectanglei;
typedef Rectangle<float> Rectanglef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Range<double> Ranged;
typedef Rectangle<double> Rectangled;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Range<long double> Rangeld;
typedef Rectangle<long double> Rectangleld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Line
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct Line;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Line<float, Vector2> Line2f;
typedef Line<float, Vector3> Line3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Line<double, Vector2> Line2d;
typedef Line<double, Vector3> Line3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Line<long double, Vector2> Line2ld;
typedef Line<long double, Vector3> Line3ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Ray
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct Ray;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef Ray<float, Vector2> Ray2f;
typedef Ray<float, Vector3> Ray3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef Ray<double, Vector2> Ray2d;
typedef Ray<double, Vector3> Ray3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef Ray<long double, Vector2> Ray2ld;
typedef Ray<long double, Vector3> Ray3ld;
#endif // _USING_MATH_LONG_DOUBLE

} // namespace pocket

#endif // __POCKET_MATH_FORWARD_H__
