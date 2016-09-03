#ifndef __POCKET_GL_TYPE_H__
#define __POCKET_GL_TYPE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"

namespace pocket
{
namespace gl
{

template <typename T>
struct gl_type
{
	static const GLenum value = _GL_UNINITIALIZED_VALUE;
};
template <>
struct gl_type<char>
{
	static const GLenum value = GL_BYTE;
};
template <>
struct gl_type<unsigned char>
{
	static const GLenum value = GL_UNSIGNED_BYTE;
};
template <>
struct gl_type<short>
{
	static const GLenum value = GL_SHORT;
};
template <>
struct gl_type<unsigned short>
{
	static const GLenum value = GL_UNSIGNED_SHORT;
};
template <>
struct gl_type<int>
{
	static const GLenum value = GL_INT;
};
template <>
struct gl_type<unsigned int>
{
	static const GLenum value = GL_UNSIGNED_INT;
};

template <>
struct gl_type<float>
{
	static const GLenum value = GL_FLOAT;
};
template <>
struct gl_type<double>
{
	static const GLenum value = GL_DOUBLE;
};

template <typename T, template <typename> class V>
struct gl_type<V<T> >
{
	static const GLenum value = gl_type<T>::value;
};

template <bool COND>
struct gl_bool
{
	static const int value = COND ? GL_TRUE : GL_FALSE;
};

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_TYPE_H__
