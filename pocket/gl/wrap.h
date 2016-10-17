#ifndef __POCLET_GL_WRAP_H__
#define __POCLET_GL_WRAP_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "common_type.h"
#include "../math/color.h"

namespace pocket
{
namespace gl
{

// 画面のクリア、クリアする値
template <typename T> inline
void clear(const math::color<T>& color, clear_type_t type)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(type);
}
template <typename T> inline
void clear(const math::color<T>& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(clear_type::color);
}
inline
void clear(clear_type_t type)
{
	glClear(type);
}

} // namespace gl
} // namespace pocket

#endif // __POCLET_GL_WRAP_H__
