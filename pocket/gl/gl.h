#ifndef __POCKET_GL_GL_H__
#define __POCKET_GL_GL_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "config.h"
#ifdef _INTERNAL_USE_GLEW
#include <GL/glew.h>
#else
#	if defined(__APPLE__) || defined(__OSX__)
#include <OpenGL/gl.h>
#	else
#include <GL/gl.h>
#	endif
#endif

namespace pocket
{
namespace gl
{
// エラー内容
enum error_bitfield
{
	error_file_not_exist = 1 << 0,
	error_compiling = 1 << 1,
	error_create_shader = 1 << 2,
	error_create_program = 1 << 3,
	//error_validate = 1 << 4,
	error_link = 1 << 4,
};

inline
const char* get_version()
{
	return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}
inline
const char* get_vendor_name()
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}
inline
const char* get_renderer_name()
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_GL_H__
