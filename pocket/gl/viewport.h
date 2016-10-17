#ifndef __POCKET_GL_VIEWPORT_H__
#define __POCKET_GL_VIEWPORT_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include <string>

namespace pocket
{
namespace gl
{

// forward
struct viewport;
viewport get_binding_viewport();
viewport& get_binding_viewport(viewport&);

struct viewport
{
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef GLint array_type[4];

	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			GLint x;
			GLint y;
			GLint w;
			GLint h;

#ifdef POCKET_USE_ANONYMOUS
		};
		array_type data;
	};
#endif // POCKET_USE_ANONYMOUS

	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	viewport() :
		x(0), y(0), w(0), h(0)
	{}
	viewport(GLint x, GLint y, GLint w, GLint h) :
		x(x), y(y), w(w), h(h)
	{}
	viewport(GLint w, GLint h) :
		x(0), y(0), w(w), h(h)
	{}
	viewport(const GLint* v) :
		x(v[0]), y(v[1]), w(v[2]), h(v[3])
	{}
	viewport(const array_type& v) :
		x(v[0]), y(v[1]), w(v[2]), h(v[3])
	{}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 現在の値を渡す
	void bind() const
	{
		glViewport(x, y, w, h);
	}

	// 現在設定されている値を取得
	void load()
	{
		glGetIntegerv(GL_VIEWPORT, &x);
	}

	// x方向の差を求める
	int diff_x() const
	{
		return w - x;
	}
	// y方向の差を求める
	int diff_y() const
	{
		return h - y;
	}

	// アスペクト比
	float aspect() const
	{
		return static_cast<float>(diff_x()) / static_cast<float>(diff_y());
	}
	template <typename T>
	T aspect() const
	{
		return static_cast<T>(diff_x()) / static_cast<T>(diff_y());
	}

	// エラー文
	std::string error() const
	{
		if (diff_x() <= 0)
		{
			return "x is greater w.";
		}
		if (diff_y() <= 0)
		{
			return "y is greater h.";
		}
		return "";
	}

	// エラーのステータス確認
	bool error_status(error_bitfield) const
	{
		return false;
	}

	// 有効な状態か
	bool valid() const
	{
		return ((w - x) > 0) && ((h - y) > 0);
	}

	//------------------------------------------------------------------------------------------
	// Operators
	//------------------------------------------------------------------------------------------

	POCKET_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const viewport& v) const
	{
		return x == v.x && y == v.y && w == v.w && h == v.h;
	}
	bool operator != (const viewport& v) const
	{
		return !(*this == v);
	}

	GLint& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const GLint& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&x)[i];
#endif // POCKET_USE_ANONYMOUS
	}
};

inline
viewport get_binding_viewport()
{
	viewport::array_type a;
	glGetIntegerv(GL_VIEWPORT, &a[0]);
	return viewport(a);
}
inline
viewport& get_binding_viewport(viewport& v)
{
	v.load();
	return v;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const viewport& v)
{
	os << io::widen("viewport: {") << std::endl <<
		io::tab << io::widen("x: ") << v.x << std::endl <<
		io::tab << io::widen("y: ") << v.y << std::endl <<
		io::tab << io::widen("width: ") << v.w << std::endl <<
		io::tab << io::widen("height: ") << v.h << std::endl;
	if (!v.valid())
	{
		std::string error = v.error();
		os << io::tab << io::widen("error: ") << io::widen(error.c_str()) << std::endl;
	}
	os << io::braces_right;
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_VIEWPORT_H__
