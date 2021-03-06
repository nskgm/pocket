﻿#ifndef __POCKET_GL_GL_H__
#define __POCKET_GL_GL_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "config.h"
#include "../io.h"
#include "../debug.h"

#ifdef POCKET_INTERNAL_USE_GLEW
#include <GL/glew.h>
#else
#	if defined(__APPLE__) || defined(__OSX__)
#include <OpenGL/gl.h>
#	else
#include <GL/gl.h>
#	endif
#endif // POCKET_INTERNAL_USE_GLEW

#include <vector>
#include <cstring> // for std::strstr, std::strcmp, std::strlen
#include <string>
#include <cstdio>
#include <cstdarg>

#ifndef POCKET_GL_UNINITIALIZED_VALUE
#	ifdef GL_DONT_CARE
#		define POCKET_GL_UNINITIALIZED_VALUE GL_DONT_CARE
#	else
#		define POCKET_GL_UNINITIALIZED_VALUE (~0)
#	endif // GL_DONT_CARE
#endif // POCKET_GL_UNINITIALIZED_VALUE

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
#define __POCKET_CASE_TO_STRING(e) case e: return #e

	switch (err)
	{
		__POCKET_CASE_TO_STRING(GL_INVALID_OPERATION);
		__POCKET_CASE_TO_STRING(GL_INVALID_ENUM);
		__POCKET_CASE_TO_STRING(GL_INVALID_VALUE);
		__POCKET_CASE_TO_STRING(GL_STACK_OVERFLOW);
		__POCKET_CASE_TO_STRING(GL_STACK_UNDERFLOW);
		__POCKET_CASE_TO_STRING(GL_OUT_OF_MEMORY);
		__POCKET_CASE_TO_STRING(GL_INVALID_FRAMEBUFFER_OPERATION);
	}
	return NULL;

#undef __POCKET_CASE_TO_STRING
}

// エラーの出力
template <typename CharT, typename CharTraits> inline
bool output_error(std::basic_ostream<CharT, CharTraits>& os, GLenum err, const char* file, const char* func, int line, const char* msg, ...)
{
	// エラーなし
	if (err == GL_NO_ERROR)
	{
		return false;
	}

	os << std::endl;
#ifdef POCKET_USE_COLOR_OUTPUT
	os << io::red << std::flush;
#endif // POCKET_USE_COLOR_OUTPUT

	// メッセージを表示
	if (msg != NULL)
	{
		char buf[512];
		// 可変長リスト
		std::va_list ap;
		va_start(ap, msg);
#if POCKET_COMPILER_IF(VC)
		vsprintf_s(&buf[0], 512, msg, ap);
#else
		vsprintf(&buf[0], msg, ap);
#endif // POCKET_COMPILER_IF(VC)
		va_end(ap);
		os << io::widen("### ") << io::widen(static_cast<const char*>(&buf[0])) << io::widen(" ###") << std::endl;
	}
	else
	{
		os << io::widen("### opengl error ###") << std::endl;
	}

	do
	{
		const char* _string = get_error_string(err);
		if (_string == NULL)
		{
			// 16進で出力するので現在の状態をとっておく
			std::ios_base::fmtflags flag = os.flags();
			// 文字列がNULLじゃないモノまで検索
			do
			{
				// とりあえずエラーコードを出力
				os << io::widen("unknown error code: 0x") << err << std::endl;
				err = glGetError();
			} while (err != GL_NO_ERROR);
			os.flags(flag);
			// NO_ERRORじゃない場合はまた取得
			if (err != GL_NO_ERROR)
			{
				_string = get_error_string(err);
			}
		}
		if (_string != NULL)
		{
			os << io::widen(_string) << std::endl;
		}
		else
		{
			break;
		}
		// 次を検索
		err = glGetError();
	} while (err != GL_NO_ERROR);

	// ファイル、関数、行数は必ず出力
	os << io::widen("-- file: ") << io::widen(file) << std::endl <<
		io::widen("-- func: ") << io::widen(func) << std::endl <<
		io::widen("-- line: ") << line << std::endl << std::endl;

#ifdef POCKET_USE_COLOR_OUTPUT
	os << io::reset << std::flush;
#endif // POCKET_USE_COLOR_OUTPUT
	return true;
}

// cerrへのエラー出力
#ifndef POCKET_GL_ERROR_MSG
#	define POCKET_GL_ERROR_MSG(MSG, ...) pocket::gl::output_error(std::cerr, glGetError(), __FILE__, __FUNCTION__, __LINE__, MSG, ##__VA_ARGS__)
#endif // POCKET_GL_ERROR_MSG
#ifndef POCKET_GL_ERROR
#	define POCKET_GL_ERROR() POCKET_GL_ERROR_MSG(NULL)
#endif // POCKET_GL_ERROR
#ifndef POCKET_GL_ASSERT_MSG
#	define POCKET_GL_ASSERT_MSG(MSG, ...) POCKET_DEBUG_ASSERT(!POCKET_GL_ERROR_MSG(MSG, ##__VA_ARGS__))
#endif // POCKET_GL_ASSERT_MSG
#ifndef POCKET_GL_ASSERT
#	define POCKET_GL_ASSERT() POCKET_DEBUG_ASSERT(!POCKET_GL_ERROR())
#endif // POCKET_GL_ASSERT

// wcerrへのエラー出力
#ifndef POCKET_GL_ERROR_MSG_W
#	define POCKET_GL_ERROR_MSG_W(MSG, ...) pocket::gl::output_error(std::wcerr, glGetError(), __FILE__, __FUNCTION__, __LINE__, MSG, ##__VA_ARGS__)
#endif // POCKET_GL_ERROR_MSG
#ifndef POCKET_GL_ERROR_W
#	define POCKET_GL_ERROR_W() POCKET_GL_ERROR_MSG_W(NULL)
#endif // POCKET_GL_ERROR_W
#ifndef POCKET_GL_ASSERT_MSG_W
#	define POCKET_GL_ASSERT_MSG_W(MSG, ...) POCKET_DEBUG_ASSERT(!POCKET_GL_ERROR_MSG_W(MSG, ##__VA_ARGS__))
#endif // POCKET_GL_ASSERT_MSG_W
#ifndef POCKET_GL_ASSERT_W
#	define POCKET_GL_ASSERT_W() POCKET_DEBUG_ASSERT(!POCKET_GL_ERROR_W())
#endif // POCKET_GL_ASSERT_W

// エラーチェック付きOpenGL関数呼び出し
#ifndef POCKET_GL_FUNC
#	define POCKET_GL_FUNC(FUNC, ...) (FUNC)(__VA_ARGS__); POCKET_GL_ASSERT()
#endif // POCKET_GL_FUNC

#ifndef POCKET_GL_FUNC_W
#	define POCKET_GL_FUNC_W(FUNC, ...) (FUNC)(__VA_ARGS__); POCKET_GL_ASSERT_W()
#endif // POCKET_GL_FUNC_W

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

inline
int calc_version(int major, int minor)
{
	// XXYY
	// XX: major
	// YY: minor
	return major*100 + minor;
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

inline
int get_version()
{
	int major, minor;
	get_version(major, minor);
	return calc_version(major, minor);
}
inline
bool has_version(int major, int minor)
{
	if (major < 1 || minor < 0)
	{
		return false;
	}
	return get_version() >= calc_version(major, minor);
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
	if (formats != NULL)
	{
		delete formats;
		formats = NULL;
	}
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
#if defined(GL_VERSION_3_0) && defined(GL_EXTENSIONS)
	// 拡張の数
	GLint count;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);
	// 拡張が何もない
	if (count == 0)
	{
		return false;
	}
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

#elif defined(GL_VERSION_1_1) && defined(GL_EXTENSIONS)
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
	return (last == ' ' || last == '\0');

#else
	static_cast<void>(name);
	return false;
#endif // GL_VERSION_3_0
}

inline
int get_type_size(GLenum type)
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

//---------------------------------------------------------------------
// タイプに対する値が指定した値と一致しているか
//---------------------------------------------------------------------
inline
bool is_binding(GLenum type, GLint value)
{
	GLint i;
	glGetIntegerv(type, &i);
	// 0の場合は設定されていない
	return i != 0 && i == value;
}
inline
bool is_binding(GLenum type, GLuint value)
{
	GLuint i;
	glGetIntegerv(type, reinterpret_cast<GLint*>(&i));
	// 0の場合は設定されていない
	return i != 0 && i == value;
}

//---------------------------------------------------------------------
// デバッグ用名前設定・取得
//---------------------------------------------------------------------
template <typename T> inline
void set_object_name(const T& obj, const char* name)
{
	GLsizei length = std::strlen(name);
	glObjectLabel(T::identifier, obj.get(), length, name);
}
template <typename T> inline
void set_object_name(const T& obj, const std::string& name)
{
	glObjectLabel(T::identifier, obj.get(), static_cast<GLsizei>(name.length()), name.c_str());
}
template <typename T, int N> inline
void get_object_name(const T& obj, char(&name)[N])
{
	glGetObjectLabel(T::identifier, obj.get(), N, NULL, &name[0]);
}
template <typename T> inline
void get_object_name(const T& obj, std::string& name)
{
	name.clear();
	GLsizei length = 0;
	glGetObjectLabel(T::identifier, obj.get(), 0, &length, NULL);
	if (length == 0)
	{
		return;
	}
	name.resize(static_cast<size_t>(length));
	glGetObjectLabel(T::identifier, obj.get(), length, NULL, &name[0]);
}
template <typename T> inline
std::string get_object_name(const T& obj)
{
	GLsizei length = 0;
	glGetObjectLabel(T::identifier, obj.get(), 0, &length, NULL);
	if (length == 0)
	{
		return "";
	}
	std::string name(static_cast<size_t>(length), '\0');
	glGetObjectLabel(T::identifier, obj.get(), length, NULL, &name[0]);
	return POCKET_CXX11_MOVE(name);
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_GL_H__
