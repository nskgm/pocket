#ifndef __POCKET_GL_GL_H__
#define __POCKET_GL_GL_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "config.h"
#include "../io.h"

#ifdef _INTERNAL_USE_GLEW
#include <GL/glew.h>
#else
#	if defined(__APPLE__) || defined(__OSX__)
#include <OpenGL/gl.h>
#	else
#include <GL/gl.h>
#	endif
#endif

#include <vector>
#include <cstring>

#ifndef _GL_UNINITIALIZED_VALUE
#	ifdef GL_DONT_CARE
#		define _GL_UNINITIALIZED_VALUE GL_DONT_CARE
#	else
#		define _GL_UNINITIALIZED_VALUE GL_INVALID_VALUE
#	endif // GL_DONT_CARE
#endif // _GL_UNINITIALIZED_VALUE

namespace pocket
{
namespace gl
{

// エラー内容
enum error_bitfield
{
	error_file_not_exist = 1 << 0,		// ファイルが存在していない
	error_file_not_write = 1 << 1,		// ファイルが書き込めない
	error_unsupported = 1 << 2,			// サポートされていない
	error_insufficient_count = 1 << 3,	// 数が不十分

	error_compiling = 1 << 4,			// コンパイルエラー
	error_creating = 1 << 5,			// ID作成失敗
	//error_validate = 1 << 6,			// プログラムの検証にエラー(glValidateProgram)
	error_link = 1 << 6,				// プログラムのリンク中にエラー
	error_binding = 1 << 7,				// バインド中にエラー

	error_invalid_data = 1 << 8,		// 無効なデータ
	error_invalid_index = 1 << 9,		// 無効なインデックス

	// 共通エラーエラー
	//error_bitfield_last_bit = error_binding
};

// 生成された時にバインドをして、破棄時にバウンドを解除
template <typename T>
class binder
{
public:
	explicit binder(const T& a) :
		address(&a)
	{
		a.bind();
	}
	~binder()
	{
		address->unbind();
	}

	bool binding() const
	{
		return true;
	}

	const T* operator -> () const
	{
		return address;
	}

	const T& operator * () const
	{
		return *address;
	}

	operator const T& () const
	{
		return *address;
	}
	operator const T* () const
	{
		return address;
	}

private:
	const T* address;
};
template <typename T, typename U>
class binder1
{
public:
	explicit binder1(const T& a, const U b) :
		address(&a), value(b)
	{
		a.bind(b);
	}
	~binder1()
	{
		address->unbind(value);
	}

	const T* operator -> () const
	{
		return address;
	}

	const T& operator * () const
	{
		return *address;
	}

	operator const T& () const
	{
		return *address;
	}
	operator const T* () const
	{
		return address;
	}

private:
	const T* address;
	const U value;
};

// エラーの文字列
inline
const char* get_error_string(GLenum err)
{
#define __POCKET_CASE_TO_STRING(e, s) case e: return s

	switch (err)
	{
		__POCKET_CASE_TO_STRING(GL_INVALID_OPERATION, "invalid operate.");
		__POCKET_CASE_TO_STRING(GL_INVALID_ENUM, "invalid enum.");
		__POCKET_CASE_TO_STRING(GL_INVALID_VALUE, "invalid value.");
		__POCKET_CASE_TO_STRING(GL_STACK_OVERFLOW, "stack overflow.");
		__POCKET_CASE_TO_STRING(GL_STACK_UNDERFLOW, "stack underflow.");
		__POCKET_CASE_TO_STRING(GL_OUT_OF_MEMORY, "out of memory.");
		__POCKET_CASE_TO_STRING(GL_INVALID_FRAMEBUFFER_OPERATION, "invalid framebuffer operate.");
	}
	return "no error.";

#undef __POCKET_CASE_TO_STRING
}

// エラーの出力
template <typename CharT, typename CharTraits> inline
bool output_error(std::basic_ostream<CharT, CharTraits>& os, const char* func, int line, const char* msg)
{
	GLenum _err = glGetError();
	if (_err == GL_NO_ERROR)
	{
		return false;
	}
	if (msg != NULL)
	{
		os << io::widen("## ") << io::widen(msg) << io::widen(" ##") << std::endl;
	}
	do
	{
		const char* _string = get_error_string(_err);
		if (msg != NULL)
		{
			os << io::tab;
		}
		os << io::widen(_string) << io::widen(" #") << io::widen(func) << io::widen(" : ") << line << std::endl;
		_err = glGetError();
	} while (_err != GL_NO_ERROR);
	return true;
}

#ifndef POCKET_GL_ERROR_MSG
#define POCKET_GL_ERROR_MSG(msg) pocket::gl::output_error(std::cout, __FUNCTION__, __LINE__, msg)
#endif // POCKET_GL_ERROR_MSG
#ifndef POCKET_GL_ERROR
#define POCKET_GL_ERROR() POCKET_GL_ERROR_MSG(NULL)
#endif // POCKET_GL_ERROR

#ifndef POCKET_GL_ERROR_MSG_W
#define POCKET_GL_ERROR_MSG_W(msg) pocket::gl::output_error(std::wcout, __FUNCTION__, __LINE__, msg)
#endif // POCKET_GL_ERROR_MSG
#ifndef POCKET_GL_ERROR_W
#define POCKET_GL_ERROR_W() POCKET_GL_ERROR_MSG_W(NULL)
#endif // POCKET_GL_ERROR_W

// バージョン（メジャー）取得
inline
int get_version_major()
{
	GLint major;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	return static_cast<int>(major);
}
// バージョン（マイナー）取得
inline
int get_version_minor()
{
	GLint minor;
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	return static_cast<int>(minor);
}

// バージョン取得
inline
void get_version(int& major, int& minor)
{
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
}
inline
void get_version(int* major, int* minor)
{
	if (major != NULL)
	{
		glGetIntegerv(GL_MAJOR_VERSION, major);
	}
	if (minor != NULL)
	{
		glGetIntegerv(GL_MINOR_VERSION, minor);
	}
}

// バージョンの文字列を取得
inline
const char* get_version_string()
{
	return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}
// ベンダーの名前を取得
inline
const char* get_vendor_name()
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}
// レンダラーの名前を取得
inline
const char* get_renderer_name()
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

// フォーマット一覧取得
inline
bool get_formats(GLenum*& formats, int* count)
{
	GLint format_count;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &format_count);
	if (count != NULL)
	{
		*count = static_cast<int>(format_count);
	}
	if (format_count == 0)
	{
		formats = NULL;
		return false;
	}
	formats = new GLenum[format_count];
	if (formats == NULL)
	{
		return false;
	}
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, reinterpret_cast<GLint*>(formats));
	return true;
}
inline
bool get_formats(std::vector<GLenum>& formats)
{
	formats.clear();

	GLint format_count;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &format_count);
	if (format_count == 0)
	{
		return false;
	}
	formats.resize(format_count);
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, reinterpret_cast<GLint*>(&formats[0]));
	return true;
}

// 拡張がサポートされているか
inline
bool is_extension_support(const char* name)
{
	// 拡張の数
	GLint count;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);
	// 拡張が何もない
	if (count == 0)
	{
		return false;
	}

	int major = get_version_major();

	// バージョンが３以上の場合はインデックスで文字列を取得できる
	if (major >= 3)
	{
		for (int i = 0; i < count; ++i)
		{
			const char* extension_indexed = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			// 名前が一致したら拡張を使用できる
			if (std::strcmp(name, extension_indexed) == 0)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		const char* extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
		if (extensions == NULL)
		{
			return false;
		}

		// 文字列の中から一致する文字列の先頭アドレスを取得
		// 含まれていない場合はNULL
		const char* address = std::strstr(extensions, name);
		if (address == NULL)
		{
			return false;
		}

		std::size_t length = std::strlen(name);
		char last = *(address + length);

		// スペース区切りで取得できているので名前の最後がスペースか
		// または一番最後の拡張だったか
		return last == ' ' || last == '\0';
	}
}

inline
GLsizei get_type_size(GLenum type)
{
#define __POCKET_TYPE_CASE_SIZE(TYPE, SIZE) case TYPE: return SIZE

	switch (type)
	{
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT, 4);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_VEC2, 8);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_VEC3, 12);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_VEC4, 16);

		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT2, 16);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT3, 36);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT4, 64);

		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT2x3, 24);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT2x4, 32);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT3x2, 24);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT3x4, 48);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT4x2, 32);
		__POCKET_TYPE_CASE_SIZE(GL_FLOAT_MAT4x3, 48);

		__POCKET_TYPE_CASE_SIZE(GL_INT, 4);
		__POCKET_TYPE_CASE_SIZE(GL_INT_VEC2, 8);
		__POCKET_TYPE_CASE_SIZE(GL_INT_VEC3, 12);
		__POCKET_TYPE_CASE_SIZE(GL_INT_VEC4, 16);

		__POCKET_TYPE_CASE_SIZE(GL_UNSIGNED_INT, 4);
		__POCKET_TYPE_CASE_SIZE(GL_UNSIGNED_INT_VEC2, 8);
		__POCKET_TYPE_CASE_SIZE(GL_UNSIGNED_INT_VEC3, 12);
		__POCKET_TYPE_CASE_SIZE(GL_UNSIGNED_INT_VEC4, 16);

		__POCKET_TYPE_CASE_SIZE(GL_BOOL, 1);
		__POCKET_TYPE_CASE_SIZE(GL_BOOL_VEC2, 2);
		__POCKET_TYPE_CASE_SIZE(GL_BOOL_VEC3, 3);
		__POCKET_TYPE_CASE_SIZE(GL_BOOL_VEC4, 4);
	}
	return 0;
#undef __POCKET_TYPE_CASE_SIZE
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_GL_H__
