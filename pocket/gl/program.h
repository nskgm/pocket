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

namespace pocket
{
namespace gl
{

// forward
class program;

// Uniform変数に代入するための型
struct program_uniform_assign
{
	const GLuint& prog;
	const GLint location;

	program_uniform_assign(const GLuint& p, GLint l) :
		prog(p), location(l)
	{

	}

	template <typename T>
	void operator = (const T&) const
	{

	}
};

namespace detail
{

// ロケーションを取得する型
template <typename T>
struct call_uniform
{
	static void call(GLuint, const T&)
	{}
};
template <>
struct call_uniform<char*>
{
	static GLint call(GLuint prog, const char* name)
	{
		return glGetUniformLocation(prog, name);
	}
};
template <std::size_t N>
struct call_uniform<char[N]>
{
	static GLint call(GLuint prog, const char(&name)[N])
	{
		return glGetUniformLocation(prog, &name[0]);
	}
};
template <>
struct call_uniform<std::string>
{
	static GLint call(GLuint prog, const std::string& name)
	{
		return glGetUniformLocation(prog, name.c_str());
	}
};

// ロケーションから代入するための型
template <>
struct call_uniform<int>
{
	static program_uniform_assign call(const GLuint& prog, int location)
	{
		return program_uniform_assign(prog, static_cast<GLint>(location));
	}
};
template <>
struct call_uniform<unsigned int>
{
	static program_uniform_assign call(const GLuint& prog, unsigned int location)
	{
		return program_uniform_assign(prog, static_cast<GLint>(location));
	}
};

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
struct is_uniform_assign_type : type_traits::false_type
{};
template <>
struct is_uniform_assign_type<int> : type_traits::true_type
{};
template <>
struct is_uniform_assign_type<unsigned int> : type_traits::true_type
{};

// 型に対する戻り値
template <typename T>
struct uniform_return_type
{
	typedef typename type_traits::remove_cv_reference<T>::type _type;

	typedef typename type_traits::conditional<
		// 文字列の場合はint
		is_uniform_get_location_type<_type>::value,
		GLint,
		// 整数の場合は代入するためのクラス
		typename type_traits::conditional<
			is_uniform_assign_type<_type>::value,
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

	typedef binder<program> binder_type;

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _id;
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
		_id(0),
		_error_bitfield(0)
	{}
	explicit program(const shader& s, bool save_bin = false) :
		_error_bitfield(0)
	{
		initialize(s, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, bool save_bin = false) :
		_error_bitfield(0)
	{
		initialize(s1, s2, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, bool save_bin = false) :
		_error_bitfield(0)
	{
		initialize(s1, s2, s3, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, bool save_bin = false) :
		_error_bitfield(0)
	{
		initialize(s1, s2, s3, s4, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5, bool save_bin = false) :
		_error_bitfield(0)
	{
		initialize(s1, s2, s3, s4, s5, save_bin);
	}
	explicit program(const char* path, GLenum format, bool file_front_format_written = true) :
		_error_bitfield(0)
	{
		initialize(path, format, file_front_format_written);
	}
	explicit program(const std::string& path, GLenum format, bool file_front_format_written = true) :
		_error_bitfield(0)
	{
		initialize(path, format, file_front_format_written);
	}
	explicit program(const char* path, bool file_front_format_written = true) :
		_error_bitfield(0)
	{
		initialize(path, file_front_format_written);
	}
	explicit program(const std::string& path, bool file_front_format_written = true) :
		_error_bitfield(0)
	{
		initialize(path, file_front_format_written);
	}
	program(const program& s) :
		_id(s._id),
		_error_bitfield(s._error_bitfield)
	{}
#ifdef _USE_CXX11
	program(program&& s) :
		_id(std::move(s._id)),
		_error_bitfield(std::move(s._error_bitfield))
	{
		s._id = 0;
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
	bool initialize(const shader& s, bool save_bin = false)
	{
		if (!create(save_bin))
		{
			return false;
		}
		// シェーダーのアタッチ
		s.attach(_id);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, bool save_bin = false)
	{
		if (!create(save_bin))
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_id);
		s2.attach(_id);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3, bool save_bin = false)
	{
		if (!create(save_bin))
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_id);
		s2.attach(_id);
		s3.attach(_id);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3, const shader& s4, bool save_bin = false)
	{
		if (!create(save_bin))
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_id);
		s2.attach(_id);
		s3.attach(_id);
		s4.attach(_id);
		if (!link())
		{
			return false;
		}
		return true;
	}
	bool initialize(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5, bool save_bin = false)
	{
		if (!create(save_bin))
		{
			return false;
		}
		// シェーダーのアタッチ
		s1.attach(_id);
		s2.attach(_id);
		s3.attach(_id);
		s4.attach(_id);
		s5.attach(_id);
		if (!link())
		{
			return false;
		}
		return true;
	}
	// バイナリファイルから作成
	bool initialize(const char* path, GLenum format, bool file_front_format_written = true)
	{
		if (!create(false))
		{
			return false;
		}
		// ファイルの情報を読み取る
		std::string bin;
		GLsizei length;
		if (!read(path, bin, format, length, file_front_format_written))
		{
			return false;
		}
		// バイナリ情報を送る
		glProgramBinary(_id, format, bin.data(), length);
		// リンクできているか
		return is_linked();
	}
	bool initialize(const std::string& path, GLenum format, bool file_front_format_written = true)
	{
		return initialize(path.c_str(), format, file_front_format_written);
	}
	bool initialize(const char* path, bool file_front_format_written = true)
	{
		if (!create(false))
		{
			return false;
		}
		std::string bin;
		GLenum format = GL_INVALID_ENUM;
		GLsizei length;
		if (!read(path, bin, format, length, file_front_format_written))
		{
			return false;
		}
		glProgramBinary(_id, format, bin.data(), length);
		return is_linked();
	}
	bool initialize(const std::string& path, bool file_front_format_written = true)
	{
		return initialize(path.c_str(), file_front_format_written);
	}

	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteProgram(_id);
			_id = 0;
		}
		_error_bitfield = 0;
	}

	// エラーの状態をクリア
	void clear()
	{
		_error_bitfield = 0;
	}

	// バインド
	void bind() const
	{
		glUseProgram(_id);
	}

	// バインド解除
	void unbind() const
	{
		glUseProgram(0);
	}

	bool binding() const
	{
		GLuint i = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&i));
		if (i == 0)
		{
			return false;
		}
		return i == _id;
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}

	// シェーダーの解除
	void detach(const shader& s) const
	{
		s.detach(_id);
	}

	// シェーダーから作成されたアセンブラバイナリを保存する
	bool save_binary(const char* path, GLenum& format, bool file_front_format_write = true)
	{
#if 0
#ifdef _INTERNAL_USE_GLEW
		// サポートされていない
		if (!GLEW_ARB_get_program_binary)
#else
		// あまり効率は良くないが拡張が使用できるかを判断
		if (!gl::is_extension_support("GL_ARB_get_program_binary"))
#endif // _INTERNAL_USE_GLEW
		{
			_error_bitfield |= error_unsupported;
			return false;
		}
#endif

		// 使用できるバイナリフォーマット数
		GLint format_count = 0;
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &format_count);
		if (format_count == 0)
		{
			// 一つ以上ないと対応していない
			_error_bitfield |= error_unsupported;
			return false;
		}

		// バイナリファイルのサイズ
		GLint binary_length;
		glGetProgramiv(_id, GL_PROGRAM_BINARY_LENGTH, &binary_length);
		if (binary_length == 0)
		{
			_error_bitfield |= error_unsupported;
			return false;
		}

		// フォーマット一覧を取得
		//std::vector<GLint> formats(format_count);
		//glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, &formats[0]);

		// バイナリ情報を取得
		std::string binary(binary_length, '\0');
		glGetProgramBinary(_id, binary_length, NULL, &format, &binary[0]);

		// ファイルに書き込む
		std::ofstream file(path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		// 書き込み、ファイルの作成ができない
		if (!file.is_open())
		{
			_error_bitfield |= error_file_not_write;
			return false;
		}
		// ファイルの最初にフォーマットを保存
		if (file_front_format_write)
		{
			// 独自のフォーマットとなるので引数で変えられるように
			file.write(reinterpret_cast<char*>(&format), sizeof(GLenum));
		}
		// フォーマットを書き込んだらバイナリデータを保存
		file.write(&binary[0], binary_length);
		file.close();

		return true;
	}
	bool save_binary(const std::string& path, GLenum& format, bool file_front_format_write = true)
	{
		return save_binary(path.c_str(), format, file_front_format_write);
	}
	// フォーマットを一時的にのみ保持
	bool save_binary(const char* path, bool file_front_format_write = true)
	{
		GLenum format;
		return save_binary(path, format, file_front_format_write);
	}
	bool save_binary(const std::string& path, bool file_front_format_write = true)
	{
		GLenum format;
		return save_binary(path, format, file_front_format_write);
	}

	// エラー文
	std::string error() const
	{
		// シェーダーハンドルの作成に失敗している
		if (error_status(error_creating))
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
			if (_id != 0)
			{
				GLint length = 0;
				glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
				if (length > 0)
				{
					std::string log(length, '\0');
					glGetProgramInfoLog(_id, length, NULL, &log[0]);
					return "failed. link program. #" + log;
				}
			}
			return "failed. link program.";
		}

		// サポートされていない
		if (error_status(error_unsupported))
		{
			return "failed. unsupported.";
		}

		// ファイルが書き込みできない
		if (error_status(error_file_not_write))
		{
			return "failed. file not write.";
		}

		// 作成されていない
		// またはすでに破棄済み
		if (_id == 0)
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
		if (_id == 0 ||
			_error_bitfield != 0)
		{
			return false;
		}
		return glIsProgram(_id) == GL_TRUE;
	}

	// ハンドルの取得
	GLuint& get()
	{
		return _id;
	}
	const GLuint& get() const
	{
		return _id;
	}

private:
	// 共通事前初期化
	bool create(bool save_bin)
	{
		finalize();
		// プログラム作成
		_id = glCreateProgram();
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}
		if (save_bin)
		{
			glProgramParameteri(_id, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
		}
		return glIsProgram(_id) == GL_TRUE;
	}
	bool link()
	{
		// プログラムのリンク
		glLinkProgram(_id);

		// プログラムの検証
		// 現在描画できるにあるか検証してくれる
		// 初期化時には行わない様にする
		/*glValidateProgram(_id);
		GLint validated;
		glGetProgramiv(_id, GL_VALIDATE_STATUS, &validated);
		if (validated == GL_FALSE)
		{
			_error_bitfield |= error_validate;
			return false;
		}*/
		return is_linked();
	}
	bool is_linked()
	{
		// リンクできているか
		GLint linked;
		glGetProgramiv(_id, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE)
		{
			_error_bitfield |= error_link;
			return false;
		}
		return true;
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
			list->attach(_id);
			// 次の可変長引数へ
			list = &va_arg(ap, const shader&);
		}
		va_end(ap);
	}*/

	bool read(const char* path, std::string& bin, GLenum& format, GLsizei& length, bool file_front_format_written)
	{
		std::ifstream fs(path, std::ios_base::in | std::ios_base::ate | std::ios_base::binary);
		if (!fs.is_open())
		{
			_error_bitfield |= error_file_not_exist;
			return false;
		}
		// ファイル全体のサイズ
		std::size_t file_size = static_cast<std::size_t>(fs.tellg());
		fs.seekg(0, std::ios_base::beg);
		// 最初に書き込まれている場合はGLenum文引く
		std::size_t bin_size = file_front_format_written ? file_size - sizeof(GLenum) : file_size;
		// フォーマットを取得
		if (file_front_format_written)
		{
			// 指定された値がすでに渡されている場合は無視
#ifdef GL_DONT_CARE
			if (format != GL_INVALID_ENUM && format != GL_DONT_CARE)
#else
			if (format != GL_INVALID_ENUM)
#endif // GL_DONT_CARE
			{
				// std::ifstream::traits_type::eof()
				fs.ignore(sizeof(GLenum));
			}
			else
			{
				fs.read(reinterpret_cast<char*>(&format), sizeof(GLenum));
			}
		}
		bin.resize(bin_size);
		fs.read(&bin[0], bin_size);
		fs.close();

		length = static_cast<GLsizei>(bin_size);

		return true;
	}

public:
	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	_CXX11_EXPLICIT operator GLuint () const
	{
		return _id;
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
		return _id == p._id;
	}
	bool operator != (const program& p) const
	{
		return !(*this == p);
	}

	program& operator = (const program& p)
	{
		_id = p._id;
		_error_bitfield = p._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	program& operator = (program&& p)
	{
		_id = std::move(p._id);
		_error_bitfield = std::move(p._error_bitfield);
		p._id = 0;
		p._error_bitfield = 0;
		return *this;
	}

	program& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11

	template <typename T
		//, typename type_traits::enable_if<(!type_traits::is_same<typename detail::uniform_return_type<T>::type, void>::value), T>::type
	>
	typename detail::uniform_return_type<T>::type operator [] (const T& v) const
	{
		typedef typename type_traits::remove_cv_reference<T>::type _type;
		return detail::call_uniform<_type>::call(_id, v);
	}
};

// プログラム作成
inline
program make_program(const shader& s, bool save_bin = false)
{
	return program(s, save_bin);
}
inline
program& make_program(program& prog, const shader& s, bool save_bin = false)
{
	prog.initialize(s, save_bin);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, bool save_bin = false)
{
	return program(s1, s2, save_bin);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, bool save_bin = false)
{
	prog.initialize(s1, s2, save_bin);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3, bool save_bin = false)
{
	return program(s1, s2, s3, save_bin);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3, bool save_bin = false)
{
	prog.initialize(s1, s2, s3, save_bin);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, bool save_bin = false)
{
	return program(s1, s2, s3, s4, save_bin);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3, const shader& s4, bool save_bin = false)
{
	prog.initialize(s1, s2, s3, s4, save_bin);
	return prog;
}
inline
program make_program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5, bool save_bin = false)
{
	return program(s1, s2, s3, s4, s5, save_bin);
}
inline
program& make_program(program& prog, const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5, bool save_bin = false)
{
	prog.initialize(s1, s2, s3, s4, s5, save_bin);
	return prog;
}

// バイナリファイルから作成
inline
program make_program(const char* path, GLenum format, bool file_front_format_written = true)
{
	return program(path, format, file_front_format_written);
}
inline
program make_program(const std::string& path, GLenum format, bool file_front_format_written = true)
{
	return program(path, format, file_front_format_written);
}
inline
program make_program(const char* path, bool file_front_format_written = true)
{
	return program(path, file_front_format_written);
}
inline
program make_program(const std::string& path, bool file_front_format_written = true)
{
	return program(path, file_front_format_written);
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
