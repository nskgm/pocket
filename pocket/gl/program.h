#ifndef __POCKET_GL_PROGRAM_H__
#define __POCKET_GL_PROGRAM_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "../type_traits.h"
#include "shader.h"
#include <string>
#include <fstream>
#ifdef _USE_CXX11
#include <utility>
#endif // _USE_CXX11

namespace pocket
{
namespace gl
{

// forward
class program;

struct program_uniform_assign
{

};

namespace detail
{

// ロケーションを取得する型
template <typename T>
struct is_uniform_get_location_type : type_traits::false_type
{};
template <>
struct is_uniform_get_location_type<char*> : type_traits::true_type
{};
template <std::size_t N>
struct is_uniform_get_location_type<char[N]> : type_traits::true_type
{};
template <>
struct is_uniform_get_location_type<std::string> : type_traits::true_type
{};

template <typename T>
struct is_uniform_type : is_uniform_get_location_type<typename type_traits::remove_cv_reference<T>::type>
{};

// ロケーションから代入するための型
template <typename T>
struct is_uniform_assign_type_base : type_traits::false_type
{};
template <>
struct is_uniform_assign_type_base<int> : type_traits::true_type
{};
template <>
struct is_uniform_assign_type_base<unsigned int> : type_traits::true_type
{};

template <typename T>
struct is_uniform_assign_type : is_uniform_assign_type_base<typename type_traits::remove_cv_reference<T>::type>
{};

// 型に対する戻り値
template <typename T>
struct uniform_return_type
{
	typedef typename type_traits::conditional<
		// 文字列の場合はint
		is_uniform_get_location_type<T>::value,
		int,

		// 整数の場合は代入するためのクラス
		typename type_traits::conditional<
			is_uniform_assign_type<T>::value,
			program_uniform_assign,
			void
		>::type
	>::type type;
};

}

class program
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	class lock
	{
	public:
		lock(const program& p) :
			prog(&p)
		{
			p.bind();
		}
		~lock()
		{
			prog->unbind();
		}

		const program* operator -> () const
		{
			return prog;
		}

	private:
		const program* prog;
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _handle;
	int _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	program() :
		_handle(0),
		_error_bitfield(0)
	{

	}
	program(const shader& s)
	{
		initialize(s);
	}
	program(const shader& s1, const shader& s2)
	{
		initialize(s1, s2);
	}
	program(const shader& s1, const shader& s2, const shader& s3)
	{
		initialize(s1, s2, s3);
	}
	program(const shader& s1, const shader& s2, const shader& s3, const shader& s4)
	{
		initialize(s1, s2, s3, s4);
	}
	program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5)
	{
		initialize(s1, s2, s3, s4, s5);
	}
	program(const program& s) :
		_handle(s._handle),
		_error_bitfield(s._error_bitfield)
	{

	}
#ifdef _USE_CXX11
	program(program&& s) :
		_handle(std::move(s._handle)),
		_error_bitfield(std::move(s._error_bitfield))
	{
		s._handle = 0;
		s._error_bitfield = 0;
	}
#endif // _USE_CXX11
	~program()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(const shader& s)
	{
		if (!create())
		{
			return false;
		}
		// シェーダーのアタッチ
		s.attach(_handle);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2)
	{
		if (!create())
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_handle);
		s2.attach(_handle);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3)
	{
		if (!create())
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_handle);
		s2.attach(_handle);
		s3.attach(_handle);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3, const shader& s4)
	{
		if (!create())
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_handle);
		s2.attach(_handle);
		s3.attach(_handle);
		s4.attach(_handle);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5)
	{
		if (!create())
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_handle);
		s2.attach(_handle);
		s3.attach(_handle);
		s4.attach(_handle);
		s5.attach(_handle);
		if (!link())
		{
			return false;
		}
		return true;
	}
	// 終了処理
	void finalize()
	{
		if (_handle != 0)
		{
			glDeleteProgram(_handle);
			_handle = 0;
		}
		_error_bitfield = 0;
	}

	// バインド
	void bind() const
	{
		glUseProgram(_handle);
	}

	// バインド解除
	void unbind() const
	{
		glUseProgram(0);
	}

	// シェーダーの解除
	void detach(const shader& s) const
	{
		s.detach(_handle);
	}

	// エラー文
	std::string error() const
	{
		// シェーダーハンドルの作成に失敗している
		if (error_status(error_create_program))
		{
			return "failed. glCreateProgram().";
		}
		// バイナリファイルから読み込む際にファイルが存在していなかった
		if (error_status(error_file_not_exist))
		{
			return "failed. file does not exist.";
		}
		// リンク中か検証中にエラー
		//if (error_status(static_cast<error_bitfield>(error_link | error_validate)))
		if (error_status(error_link))
		{
			// GL側エラー文の取得
			if (_handle != 0)
			{
				GLint length = 0;
				glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &length);
				if (length > 0)
				{
					std::string log(length, '\0');
					glGetProgramInfoLog(_handle, length, NULL, &log[0]);
					return "failed. link program. #" + log;
				}
				else
				{
					return "failed. link program.";
				}
			}
			else
			{
				return "failed. link program.";
			}
		}
		// 作成されていない
		// またはすでに破棄済み
		if (_handle == 0)
		{
			return "not created. or already destroyed.";
		}
		// エラーは起こしていない
		return "";
	}

	// エラーのステータス確認
	bool error_status(error_bitfield bit) const
	{
		return (_error_bitfield & bit) != 0;
	}

	// 有効な状態か
	bool valid() const
	{
		if (_handle == 0 ||
			_error_bitfield != 0)
		{
			return false;
		}
		return glIsProgram(_handle) == GL_TRUE;
	}

	// ハンドルの取得
	GLuint& get()
	{
		return _handle;
	}
	const GLuint& get() const
	{
		return _handle;
	}

private:
	// 共通事前初期化
	bool create()
	{
		finalize();
		// プログラム作成
		_handle = glCreateProgram();
		if (_handle == 0)
		{
			_error_bitfield |= error_create_program;
			return false;
		}
		return glIsProgram(_handle) == GL_TRUE;
	}
	bool link()
	{
		// プログラムのリンク
		glLinkProgram(_handle);
		// リンクできているか
		GLint linked;
		glGetProgramiv(_handle, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE)
		{
			_error_bitfield |= error_link;
			return false;
		}
		// プログラムの検証
		// 現在描画できるにあるか検証してくれる
		// 初期化時には行わない様にする
		/*glValidateProgram(_handle);
		GLint validated;
		glGetProgramiv(_handle, GL_VALIDATE_STATUS, &validated);
		if (validated == GL_FALSE)
		{
			_error_bitfield |= error_validate;
			return false;
		}*/
		return glIsProgram(_handle) == GL_TRUE;
	}
	// シェーダーリンク
	/*bool link(const shader& first, ...)
	{
		const shader* list = &first;

		va_list ap;
		va_start(ap, s);
		// 可変長引数の全引数をアタッチ
		while (list != NULL)
		{
			list->attach(_handle);
			// 次の可変長引数へ
			list = &va_arg(ap, const shader&);
		}
		va_end(ap);
	}*/

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

public:
	_CXX11_EXPLICIT operator GLuint () const
	{
		return _handle;
	}

	_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const program& p) const
	{
		return _handle == p._handle;
	}
	bool operator != (const program& p) const
	{
		return !(*this == p);
	}

	program& operator = (const program& p)
	{
		_handle = p._handle;
		_error_bitfield = p._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	program& operator = (program&& p)
	{
		_handle = std::move(p._handle);
		_error_bitfield = std::move(p._error_bitfield);
		p._handle = 0;
		p._error_bitfield = 0;
		return *this;
	}

	program& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};

// プログラム作成
inline
program make_program(const shader& s)
{
	return program(s);
}
inline
program& make_program(program& prog, const shader& s)
{
	prog.initialize(s);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2)
{
	return program(s1, s2);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2)
{
	prog.initialize(s1, s2);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3)
{
	return program(s1, s2, s3);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3)
{
	prog.initialize(s1, s2, s3);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3, const shader& s4)
{
	return program(s1, s2, s3, s4);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3, const shader& s4)
{
	prog.initialize(s1, s2, s3, s4);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5)
{
	return program(s1, s2, s3, s4, s5);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5)
{
	prog.initialize(s1, s2, s3, s4, s5);
	return prog;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const program& v)
{
	const bool valid = v.valid();
	const char* valid_str = valid ? "true" : "false";
	os << io::widen("program: ") << io::braces_left << std::endl <<
		io::tab << io::widen("valid: ") << io::widen(valid_str) << std::endl;
	if (!valid)
	{
		std::string error = v.error();
		os << io::tab << io::widen("error: ") << io::widen(error.c_str()) << std::endl;
	}
	os << io::braces_right;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const program& v)
{
	os << v;
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_PROGRAM_H__
