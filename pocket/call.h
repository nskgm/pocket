﻿#ifndef __POCKET_CALL_H__
#define __POCKET_CALL_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

//------------------------------------------------------------------------------------------
// 一度のみ宣言するタグ用
//------------------------------------------------------------------------------------------
#ifndef POCKET_DECL_CALL_ARG_TYPE
#	define POCKET_DECL_CALL_ARG_TYPE(NAME) enum NAME##_t { NAME }
#endif // POCKET_DECL_CALL_ARG_TYPE

namespace pocket
{
namespace call
{
POCKET_DECL_CALL_ARG_TYPE(noinitialize);
POCKET_DECL_CALL_ARG_TYPE(zero);
POCKET_DECL_CALL_ARG_TYPE(one);
POCKET_DECL_CALL_ARG_TYPE(half);
POCKET_DECL_CALL_ARG_TYPE(half_of_half);
POCKET_DECL_CALL_ARG_TYPE(identity);

POCKET_DECL_CALL_ARG_TYPE(plus);
POCKET_DECL_CALL_ARG_TYPE(negate);
POCKET_DECL_CALL_ARG_TYPE(add);
POCKET_DECL_CALL_ARG_TYPE(add_assign);
POCKET_DECL_CALL_ARG_TYPE(sub);
POCKET_DECL_CALL_ARG_TYPE(sub_assign);
POCKET_DECL_CALL_ARG_TYPE(mul);
POCKET_DECL_CALL_ARG_TYPE(mul_assign);
POCKET_DECL_CALL_ARG_TYPE(div);
POCKET_DECL_CALL_ARG_TYPE(div_assign);
POCKET_DECL_CALL_ARG_TYPE(rem);
POCKET_DECL_CALL_ARG_TYPE(rem_assign);

POCKET_DECL_CALL_ARG_TYPE(increment);
POCKET_DECL_CALL_ARG_TYPE(increment_back);
POCKET_DECL_CALL_ARG_TYPE(decrement);
POCKET_DECL_CALL_ARG_TYPE(decrement_back);

POCKET_DECL_CALL_ARG_TYPE(at);
POCKET_DECL_CALL_ARG_TYPE(pointer);

POCKET_DECL_CALL_ARG_TYPE(x);
POCKET_DECL_CALL_ARG_TYPE(y);
POCKET_DECL_CALL_ARG_TYPE(z);

POCKET_DECL_CALL_ARG_TYPE(roll);
POCKET_DECL_CALL_ARG_TYPE(yaw);
POCKET_DECL_CALL_ARG_TYPE(pitch);

#ifdef POCKET_USE_CXX11
POCKET_DECL_CALL_ARG_TYPE(sqr);
POCKET_DECL_CALL_ARG_TYPE(radians);
POCKET_DECL_CALL_ARG_TYPE(degrees);
POCKET_DECL_CALL_ARG_TYPE(sin);
POCKET_DECL_CALL_ARG_TYPE(cos);
POCKET_DECL_CALL_ARG_TYPE(tan);
POCKET_DECL_CALL_ARG_TYPE(asin);
POCKET_DECL_CALL_ARG_TYPE(acos);
POCKET_DECL_CALL_ARG_TYPE(atan);
POCKET_DECL_CALL_ARG_TYPE(sin_cos);

POCKET_DECL_CALL_ARG_TYPE(round);
POCKET_DECL_CALL_ARG_TYPE(abs);
POCKET_DECL_CALL_ARG_TYPE(ceil);
POCKET_DECL_CALL_ARG_TYPE(floor);
POCKET_DECL_CALL_ARG_TYPE(sqrt);
POCKET_DECL_CALL_ARG_TYPE(pot);
POCKET_DECL_CALL_ARG_TYPE(is_pot);
POCKET_DECL_CALL_ARG_TYPE(clamp01);

POCKET_DECL_CALL_ARG_TYPE(vec2);
POCKET_DECL_CALL_ARG_TYPE(vec3);
POCKET_DECL_CALL_ARG_TYPE(vec4);
#endif // POCKET_USE_CXX11

POCKET_DECL_CALL_ARG_TYPE(equal);
POCKET_DECL_CALL_ARG_TYPE(not_equal);
POCKET_DECL_CALL_ARG_TYPE(near);
POCKET_DECL_CALL_ARG_TYPE(near_zero);
POCKET_DECL_CALL_ARG_TYPE(length);
POCKET_DECL_CALL_ARG_TYPE(length_square);
POCKET_DECL_CALL_ARG_TYPE(dot);
POCKET_DECL_CALL_ARG_TYPE(normalize);
POCKET_DECL_CALL_ARG_TYPE(normalized);
POCKET_DECL_CALL_ARG_TYPE(cross);
POCKET_DECL_CALL_ARG_TYPE(lerp);
POCKET_DECL_CALL_ARG_TYPE(distance);
POCKET_DECL_CALL_ARG_TYPE(direction);
POCKET_DECL_CALL_ARG_TYPE(saturate);
POCKET_DECL_CALL_ARG_TYPE(saturated);
POCKET_DECL_CALL_ARG_TYPE(swizzle);

POCKET_DECL_CALL_ARG_TYPE(inner);
POCKET_DECL_CALL_ARG_TYPE(angle);
POCKET_DECL_CALL_ARG_TYPE(from_angle);

POCKET_DECL_CALL_ARG_TYPE(from_pitch_yaw);
POCKET_DECL_CALL_ARG_TYPE(transform);
POCKET_DECL_CALL_ARG_TYPE(transform_coord);
POCKET_DECL_CALL_ARG_TYPE(transform_normal);
POCKET_DECL_CALL_ARG_TYPE(rotate);

POCKET_DECL_CALL_ARG_TYPE(dot_normal);
POCKET_DECL_CALL_ARG_TYPE(dot_coord);
POCKET_DECL_CALL_ARG_TYPE(from_points);
POCKET_DECL_CALL_ARG_TYPE(from_normal_point);
POCKET_DECL_CALL_ARG_TYPE(point);
POCKET_DECL_CALL_ARG_TYPE(intersect);
POCKET_DECL_CALL_ARG_TYPE(intersect_line);
POCKET_DECL_CALL_ARG_TYPE(intersect_ray);

POCKET_DECL_CALL_ARG_TYPE(from_axis_angle);
POCKET_DECL_CALL_ARG_TYPE(from_matrix);
POCKET_DECL_CALL_ARG_TYPE(conjugate);
POCKET_DECL_CALL_ARG_TYPE(axis);
POCKET_DECL_CALL_ARG_TYPE(slerp);

POCKET_DECL_CALL_ARG_TYPE(right);
POCKET_DECL_CALL_ARG_TYPE(left);
POCKET_DECL_CALL_ARG_TYPE(up);
POCKET_DECL_CALL_ARG_TYPE(down);
POCKET_DECL_CALL_ARG_TYPE(back);
POCKET_DECL_CALL_ARG_TYPE(front);

POCKET_DECL_CALL_ARG_TYPE(position);
POCKET_DECL_CALL_ARG_TYPE(scale);
POCKET_DECL_CALL_ARG_TYPE(translate);
POCKET_DECL_CALL_ARG_TYPE(world);
POCKET_DECL_CALL_ARG_TYPE(perspective_field_of_view);
POCKET_DECL_CALL_ARG_TYPE(orthographics);
POCKET_DECL_CALL_ARG_TYPE(orthographics2d);
POCKET_DECL_CALL_ARG_TYPE(look_to);
POCKET_DECL_CALL_ARG_TYPE(look_at);
POCKET_DECL_CALL_ARG_TYPE(transpose);
POCKET_DECL_CALL_ARG_TYPE(transposed);
POCKET_DECL_CALL_ARG_TYPE(determinant);

POCKET_DECL_CALL_ARG_TYPE(inverse);
} // namespace call
} // namespace pocket

#endif // __POCKET_CALL_H__