#ifndef __POCKET_MATH_FORWARD_H__
#define __POCKET_MATH_FORWARD_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

namespace pocket
{
namespace math
{

/*------------------------------------------------------------------------------------------
* math_traits
*------------------------------------------------------------------------------------------*/
template <typename> class math_traits;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef math_traits<int> math_traitsi;
typedef math_traits<float> math_traitsf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef math_traits<double> math_traitsd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef math_traits<long double> math_traitsld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* vector2
*------------------------------------------------------------------------------------------*/
template <typename> struct vector2;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef vector2<int> point2;
typedef vector2<int> vector2i;
typedef vector2<float> vector2f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef vector2<double> vector2d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef vector2<long double> vector2ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* vector3
*------------------------------------------------------------------------------------------*/
template <typename> struct vector3;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef vector3<int> point3;
typedef vector3<int> vector3i;
typedef vector3<float> vector3f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef vector3<double> vector3d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef vector3<long double> vector3ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* vector4
*------------------------------------------------------------------------------------------*/
template <typename> struct vector4;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef vector4<int> point4;
typedef vector4<int> vector4i;
typedef vector4<float> vector4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef vector4<double> vector4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef vector4<long double> vector4ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* matrix4x4
*------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------
* quaternion
*------------------------------------------------------------------------------------------*/
template <typename> struct quaternion;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef quaternion<float> quaternionf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef quaternion<double> quaterniond;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef quaternion<long double> quaternionld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* plane
*------------------------------------------------------------------------------------------*/
template <typename> struct plane;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef plane<float> planef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef plane<double> planed;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef plane<long double> planeld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* color
*------------------------------------------------------------------------------------------*/
template <typename> struct color;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef color<float> colorf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef color<double> colord;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef color<long double> colorld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* Range, Rectangle
*------------------------------------------------------------------------------------------*/
template <typename> struct range;
template <typename> struct rectangle;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef range<int> rangei;
typedef range<float> rangef;
typedef rectangle<int> rectanglei;
typedef rectangle<float> rectanglef;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef range<double> ranged;
typedef rectangle<double> rectangled;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef range<long double> rangeld;
typedef rectangle<long double> rectangleld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* line
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct line;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef line<float, vector2> line2f;
typedef line<float, vector3> line3f;
typedef line<float, vector4> line4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef line<double, vector2> line2d;
typedef line<double, vector3> line3d;
typedef line<double, vector4> line4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef line<long double, vector2> line2ld;
typedef line<long double, vector3> line3ld;
typedef line<long double, vector4> line4ld;
#endif // _USING_MATH_LONG_DOUBLE

/*------------------------------------------------------------------------------------------
* ray
*------------------------------------------------------------------------------------------*/
template <typename, template<typename> class> struct ray;
#ifndef _UNUSING_MATH_INT_FLOAT
typedef ray<float, vector2> ray2f;
typedef ray<float, vector3> ray3f;
typedef ray<float, vector4> ray4f;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef ray<double, vector2> ray2d;
typedef ray<double, vector3> ray3d;
typedef ray<double, vector4> ray4d;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef ray<long double, vector2> ray2ld;
typedef ray<long double, vector3> ray3ld;
typedef ray<long double, vector4> ray4ld;
#endif // _USING_MATH_LONG_DOUBLE

} // namespace math
} // namespace pocket

#endif // __POCKET_MATH_FORWARD_H__
