﻿#ifndef __POCKET_BEHAVIOR_H__
#define __POCKET_BEHAVIOR_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#ifndef _DECL_CHANGE_BEHAVIOR
// pragma once が使用できる場合はstruct
#	ifdef _USE_PRAGMA_ONCE
#		define _DECL_CHANGE_BEHAVIOR(NAME) struct _##NAME##_t {}; static const _##NAME##_t NAME = {}
// 以外はenumで型判定
#	else
#		define _DECL_CHANGE_BEHAVIOR(NAME) enum _##NAME##_t { NAME }
#	endif
#endif // _DECL_CHANGE_BEHAVIOR

namespace pocket
{
namespace behavior
{
_DECL_CHANGE_BEHAVIOR(noinitialize);
_DECL_CHANGE_BEHAVIOR(zero);
_DECL_CHANGE_BEHAVIOR(one);
_DECL_CHANGE_BEHAVIOR(half);
_DECL_CHANGE_BEHAVIOR(half_of_half);
_DECL_CHANGE_BEHAVIOR(identity);

_DECL_CHANGE_BEHAVIOR(plus);
_DECL_CHANGE_BEHAVIOR(negate);
_DECL_CHANGE_BEHAVIOR(add);
_DECL_CHANGE_BEHAVIOR(add_assign);
_DECL_CHANGE_BEHAVIOR(sub);
_DECL_CHANGE_BEHAVIOR(sub_assign);
_DECL_CHANGE_BEHAVIOR(mul);
_DECL_CHANGE_BEHAVIOR(mul_assign);
_DECL_CHANGE_BEHAVIOR(div);
_DECL_CHANGE_BEHAVIOR(div_assign);
_DECL_CHANGE_BEHAVIOR(rem);
_DECL_CHANGE_BEHAVIOR(rem_assign);

_DECL_CHANGE_BEHAVIOR(increment);
_DECL_CHANGE_BEHAVIOR(increment_back);
_DECL_CHANGE_BEHAVIOR(decrement);
_DECL_CHANGE_BEHAVIOR(decrement_back);

_DECL_CHANGE_BEHAVIOR(at);
_DECL_CHANGE_BEHAVIOR(pointer);

_DECL_CHANGE_BEHAVIOR(x);
_DECL_CHANGE_BEHAVIOR(y);
_DECL_CHANGE_BEHAVIOR(z);

_DECL_CHANGE_BEHAVIOR(roll);
_DECL_CHANGE_BEHAVIOR(yaw);
_DECL_CHANGE_BEHAVIOR(pitch);
_DECL_CHANGE_BEHAVIOR(roll_pitch_yaw);

#ifdef _USE_CXX11
_DECL_CHANGE_BEHAVIOR(sqr);
_DECL_CHANGE_BEHAVIOR(radians);
_DECL_CHANGE_BEHAVIOR(degrees);
_DECL_CHANGE_BEHAVIOR(sin);
_DECL_CHANGE_BEHAVIOR(cos);
_DECL_CHANGE_BEHAVIOR(tan);
_DECL_CHANGE_BEHAVIOR(asin);
_DECL_CHANGE_BEHAVIOR(acos);
_DECL_CHANGE_BEHAVIOR(atan);
_DECL_CHANGE_BEHAVIOR(sin_cos);

_DECL_CHANGE_BEHAVIOR(round);
_DECL_CHANGE_BEHAVIOR(abs);
_DECL_CHANGE_BEHAVIOR(ceil);
_DECL_CHANGE_BEHAVIOR(floor);
_DECL_CHANGE_BEHAVIOR(sqrt);
_DECL_CHANGE_BEHAVIOR(pot);
_DECL_CHANGE_BEHAVIOR(is_pot);
_DECL_CHANGE_BEHAVIOR(clamp01);

_DECL_CHANGE_BEHAVIOR(vec2);
_DECL_CHANGE_BEHAVIOR(vec3);
_DECL_CHANGE_BEHAVIOR(vec4);
#endif // _USE_CXX11

_DECL_CHANGE_BEHAVIOR(equal);
_DECL_CHANGE_BEHAVIOR(not_equal);
_DECL_CHANGE_BEHAVIOR(near);
_DECL_CHANGE_BEHAVIOR(near_zero);
_DECL_CHANGE_BEHAVIOR(length);
_DECL_CHANGE_BEHAVIOR(length_square);
_DECL_CHANGE_BEHAVIOR(dot);
_DECL_CHANGE_BEHAVIOR(normalize);
_DECL_CHANGE_BEHAVIOR(normalized);
_DECL_CHANGE_BEHAVIOR(cross);
_DECL_CHANGE_BEHAVIOR(lerp);
_DECL_CHANGE_BEHAVIOR(distance);
_DECL_CHANGE_BEHAVIOR(direction);
_DECL_CHANGE_BEHAVIOR(saturate);
_DECL_CHANGE_BEHAVIOR(saturated);
_DECL_CHANGE_BEHAVIOR(swizzle);

_DECL_CHANGE_BEHAVIOR(inner);
_DECL_CHANGE_BEHAVIOR(angle);
_DECL_CHANGE_BEHAVIOR(from_angle);

_DECL_CHANGE_BEHAVIOR(from_pitch_yaw);
_DECL_CHANGE_BEHAVIOR(transform);
_DECL_CHANGE_BEHAVIOR(transform_coord);
_DECL_CHANGE_BEHAVIOR(transform_normal);
_DECL_CHANGE_BEHAVIOR(rotate);

_DECL_CHANGE_BEHAVIOR(dot_normal);
_DECL_CHANGE_BEHAVIOR(dot_coord);
_DECL_CHANGE_BEHAVIOR(from_points);
_DECL_CHANGE_BEHAVIOR(from_normal_point);
_DECL_CHANGE_BEHAVIOR(point);
_DECL_CHANGE_BEHAVIOR(intersect);
_DECL_CHANGE_BEHAVIOR(intersect_line);
_DECL_CHANGE_BEHAVIOR(intersect_ray);

_DECL_CHANGE_BEHAVIOR(from_axis_angle);
_DECL_CHANGE_BEHAVIOR(from_matrix);
_DECL_CHANGE_BEHAVIOR(conjugate);
_DECL_CHANGE_BEHAVIOR(axis);
_DECL_CHANGE_BEHAVIOR(slerp);

_DECL_CHANGE_BEHAVIOR(right);
_DECL_CHANGE_BEHAVIOR(left);
_DECL_CHANGE_BEHAVIOR(up);
_DECL_CHANGE_BEHAVIOR(down);
_DECL_CHANGE_BEHAVIOR(back);
_DECL_CHANGE_BEHAVIOR(front);

_DECL_CHANGE_BEHAVIOR(position);
_DECL_CHANGE_BEHAVIOR(scale);
_DECL_CHANGE_BEHAVIOR(translate);
_DECL_CHANGE_BEHAVIOR(world);
_DECL_CHANGE_BEHAVIOR(perspective_field_of_view);
_DECL_CHANGE_BEHAVIOR(orthographics);
_DECL_CHANGE_BEHAVIOR(orthographics2d);
_DECL_CHANGE_BEHAVIOR(look_to);
_DECL_CHANGE_BEHAVIOR(look_at);
_DECL_CHANGE_BEHAVIOR(transpose);
_DECL_CHANGE_BEHAVIOR(transposed);
_DECL_CHANGE_BEHAVIOR(determinant);

_DECL_CHANGE_BEHAVIOR(inverse);
} // namespace behavior
} // namespace pocket

#endif // __POCKET_BEHAVIOR_H__