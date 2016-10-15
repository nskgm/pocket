#ifndef __POCKET_GL_DEPTH_RANGE_H__
#define __POCKET_GL_DEPTH_RANGE_H__

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
struct depth_range;
depth_range get_binding_depth_range();
depth_range& get_binding_depth_range(depth_range&);

struct depth_range
{
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			GLfloat n;
			GLfloat f;

#ifdef POCKET_USE_ANONYMOUS
		};
		GLfloat data[2];
	};
#endif // POCKET_USE_ANONYMOUS

	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	depth_range() :
		n(0.0f), f(1.0f)
	{}
	depth_range(GLfloat n, GLfloat f) :
		n(n), f(f)
	{}
	depth_range(GLfloat f) :
		n(0.0f), f(f)
	{}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 現在の値を渡す
	void bind() const
	{
		glDepthRange(n, f);
	}

	// 現在設定されている値を取得
	void load()
	{
		glGetFloatv(GL_DEPTH_RANGE, &n);
	}

	// 差を求める
	float diff() const
	{
		return f - n;
	}

	// エラー文
	std::string error() const
	{
		if (diff() <= 0.0f)
		{
			return "n is greater f.";
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
		return diff() > 0.0f;
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

	bool operator == (const depth_range& v) const
	{
		return n == v.n && f == v.f;
	}
	bool operator != (const depth_range& v) const
	{
		return !(*this == v);
	}

	GLfloat& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 1);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&n)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const GLfloat& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 1);
#ifdef POCKET_USE_ANONYMOUS
		return data[i];
#else
		return (&n)[i];
#endif // POCKET_USE_ANONYMOUS
	}
};

inline
depth_range get_binding_depth_range()
{
	GLfloat a[2];
	glGetFloatv(GL_DEPTH_RANGE, &a[0]);
	return depth_range(a[0], a[1]);
}
inline
depth_range& get_binding_depth_range(depth_range& v)
{
	v.load();
	return v;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const depth_range& v)
{
	os << io::widen("depth_range: {") << std::endl <<
		io::tab << io::widen("near: ") << v.n << std::endl <<
		io::tab << io::widen("far: ") << v.f << std::endl;
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

#endif // __POCKET_GL_DEPTH_RANGE_H__
