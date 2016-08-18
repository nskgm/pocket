#ifndef __POCKET_GL_SHADER_H__
#define __POCKET_GL_SHADER_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include <string>
#include <fstream>

namespace pocket
{
namespace gl
{

// forward
class shader;

class shader
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/
	// シェーダー種類
	enum shader_type
	{
		vertex,
		fragment,
		geometry,
		tess_control,
		tess_evaluate,
		compute,

		unknown
	};
	// コンパイル種類
	enum compile_type
	{
		file,
		string,

		memory = string
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	shader_type _type;
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

	shader() :
		_type(unknown),
		_id(0),
		_error_bitfield(0)
	{}
	explicit shader(shader_type type, const char* str, compile_type comp = file) :
		_error_bitfield(0)
	{
		initialize(type, str, comp);
	}
	template <int N>
	explicit shader(shader_type type, const char*(&str)[N], compile_type comp = file) :
		_error_bitfield(0)
	{
		initialize(type, str, comp);
	}
	shader(const shader& s) :
		_type(s._type),
		_id(s._id),
		_error_bitfield(s._error_bitfield)
	{}
#ifdef _USE_CXX11
	shader(shader&& s) :
		_type(std::move(s._type)),
		_id(std::move(s._id)),
		_error_bitfield(std::move(s._error_bitfield))
	{
		s._type = unknown;
		s._id = 0;
		s._error_bitfield = 0;
	}
#endif // _USE_CXX11
	~shader()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(shader_type type, const char* s, compile_type compile = file)
	{
		finalize();
		// ファイルから作成
		if (compile == file)
		{
			return create_from_file(type, s);
		}
		// 文字列から作成
		return create_from_memory(type, 1, &s);
	}
	template <int N>
	bool initialize(shader_type type, const char*(&s)[N], compile_type compile = file)
	{
		finalize();
		// ファイルから作成
		if (compile == file)
		{
			return create_from_files(type, s);
		}
		// 文字列から作成
		return create_from_memory(type, N, &s[0]);
	}
	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteShader(_id);
			_id = 0;
		}
		_type = unknown;
		_error_bitfield = 0;
	}

	// エラーの状態をクリア
	void clear()
	{
		_error_bitfield = 0;
	}

	// プログラムへアタッチ
	void attach(GLuint prog) const
	{
		glAttachShader(prog, _id);
	}

	// アタッチの解除
	void detach(GLuint prog) const
	{
		glDetachShader(prog, _id);
	}

	// コード取得
	std::string code(GLsizei length = 1024) const
	{
		std::string source(length, '\0');
		glGetShaderSource(_id, length, NULL, &source[0]);
		return _CXX11_MOVE(source);
	}

	// エラー文
	std::string error() const
	{
		// シェーダーハンドルの作成に失敗している
		if (error_status(error_creating))
		{
			return "failed. glCreateShader().";
		}
		// ファイルから読み込む際にファイルが存在していなかった
		if (error_status(error_file_not_exist))
		{
			return "failed. file does not exist.";
		}
		// コンパイル中に失敗している
		if (error_status(error_compiling))
		{
			// GL側エラー文の取得
			if (_id != 0)
			{
				GLint length = 0;
				glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
				if (length > 0)
				{
					std::string log(length, '\0');
					glGetShaderInfoLog(_id, length, NULL, &log[0]);
					return "failed. compile shader. # " + log;
				}
			}
			return "failed. compile shader.";
		}
		// 作成されていない
		// またはすでに破棄済み
		if (_type == unknown ||
			_id == 0)
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
		return glIsShader(_id) == GL_TRUE;
	}

	// シェーダー種類
	shader_type kind() const
	{
		return _type;
	}

	// シェーダー種類の比較
	bool kind_of(shader_type type) const
	{
		return _type == type;
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
	bool create_from_file(shader_type type, const char* path)
	{
		std::ifstream fs(path, std::ios_base::in | std::ios_base::ate);
		// ファイルが存在していない
		if (!fs.is_open())
		{
			_error_bitfield |= error_file_not_exist;
			return false;
		}

		// ファイルサイズ取得
		std::ifstream::pos_type size = fs.tellg();
		fs.seekg(0, std::ios_base::beg);
		// ファイル全内容
		std::string source(static_cast<size_t>(size), '\0');
		fs.read(&source[0], size);
		fs.close();

		// ファイル内容からコンパイル
		const char* c_str = source.c_str();
		return create_from_memory(type, 1, &c_str);
	}
	template <int N>
	bool create_from_files(shader_type type, const char*(&path)[N])
	{
		// ファイルごとの文字列
		std::string sources[N];
		const char* c_sources[N];

		for (int i = 0; i < N; ++i)
		{
			std::ifstream fs(path[i], std::ios_base::in | std::ios_base::ate);
			// ファイルが存在していない
			if (!fs.is_open())
			{
				_error_bitfield |= error_file_not_exist;
				return false;
			}
			std::ifstream::pos_type size = fs.tellg();
			fs.seekg(0, std::ios_base::beg);
			sources[i].resize(size, '\0');
			fs.read(&sources[i][0], size);
			fs.close();

			// ポインタを渡す
			c_sources[i] = sources[i].c_str();
		}

		// ファイル内容からコンパイル
		return create_from_memory(type, N, &c_sources[0]);
	}
	bool create_from_memory(shader_type type, GLsizei count, const char** str)
	{
		_DEB_ASSERT(str != NULL);

		_type = type;

		// シェーダーハンドル作成
		_id = glCreateShader(to_gl_shader_type());
		// 作成失敗
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}
		// ソースファイルのコンパイル
		glShaderSource(_id, count, str, NULL);
		glCompileShader(_id);

		GLint compiled;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
		// コンパイルが成功していない
		if (compiled == GL_FALSE)
		{
			_error_bitfield |= error_compiling;
			return false;
		}
		return glIsShader(_id) == GL_TRUE;
	}
	GLuint to_gl_shader_type() const
	{
		switch (_type)
		{
			case vertex:
				return GL_VERTEX_SHADER;
			case fragment:
				return GL_FRAGMENT_SHADER;
			case geometry:
				return GL_GEOMETRY_SHADER;
			case tess_control:
				return GL_TESS_CONTROL_SHADER;
			case tess_evaluate:
				return GL_TESS_EVALUATION_SHADER;
			case compute:
				return GL_COMPUTE_SHADER;

			case unknown:
			default:
				return GL_INVALID_VALUE;
		}
	}

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

public:
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

	bool operator == (const shader& s) const
	{
		return _id == s._id && _type == s._type;
	}
	bool operator != (const shader& s) const
	{
		return !(*this == s);
	}

	shader& operator = (const shader& s)
	{
		_type = s._type;
		_id = s._id;
		_error_bitfield = s._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	shader& operator = (shader&& s)
	{
		_type = std::move(s._type);
		_id = std::move(s._id);
		_error_bitfield = std::move(s._error_bitfield);
		s._type = unknown;
		s._id = 0;
		s._error_bitfield = 0;
		return *this;
	}

	shader& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};

// シェーダー作成
template <shader::shader_type T> inline
shader make_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(T, s, compile);
}
template <shader::shader_type T> inline
shader& make_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(T, s, compile);
	return sdr;
}

#define __POCKET_MAKE_SHADER_TYPE(TYPE) inline \
	shader make_##TYPE##_shader(const char* s, shader::compile_type compile = shader::file) \
	{ \
		return shader(shader::TYPE, s, compile); \
	} \
	template <int N> inline \
	shader make_##TYPE##_shader(const char*(&s)[N], shader::compile_type compile = shader::file) \
	{ \
		return shader(shader::TYPE, s, compile); \
	} \
	inline \
	shader& make_##TYPE##_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file) \
	{ \
		sdr.initialize(shader::TYPE, s, compile); \
		return sdr; \
	} \
	template <int N> inline \
	shader& make_##TYPE##_shader(shader& sdr, const char*(&s)[N], shader::compile_type compile = shader::file) \
	{ \
		sdr.initialize(shader::TYPE, s, compile); \
		return sdr; \
	}

// 頂点シェーダー作成（サンプルでマクロを使用しない）
inline
shader make_vertex_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::vertex, s, compile);
}
template <int N> inline
shader make_vertex_shader(const char*(&s)[N], shader::compile_type compile = shader::file)
{
	return shader(shader::vertex, s, compile);
}
inline
shader& make_vertex_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::vertex, s, compile);
	return sdr;
}
template <int N> inline
shader& make_vertex_shader(shader& sdr, const char*(&s)[N], shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::vertex, s, compile);
	return sdr;
}
// ピクセルシェーダー作成
__POCKET_MAKE_SHADER_TYPE(fragment);
// ジオメトリシェーダー作成
__POCKET_MAKE_SHADER_TYPE(geometry);
// テセレーション制御シェーダー作成
__POCKET_MAKE_SHADER_TYPE(tess_control);
// テセレーション評価シェーダー作成
__POCKET_MAKE_SHADER_TYPE(tess_evaluate);
// コンピュートシェーダー作成
__POCKET_MAKE_SHADER_TYPE(compute);

#undef __POCKET_MAKE_SHADER_TYPE

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const shader& v)
{
	const char* type;
	switch (v.kind())
	{
		case shader::vertex:
			type = "vertex";
			break;
		case shader::fragment:
			type = "fragment";
			break;
		case shader::geometry:
			type = "geometry";
			break;
		case shader::tess_control:
			type = "tess_control";
			break;
		case shader::tess_evaluate:
			type = "tess_evaluate";
			break;
		case shader::compute:
			type = "compute";
			break;
		case shader::unknown:
		default:
			type = "unknown";
			break;
	}

	const bool valid = v.valid();
	const char* valid_str = valid ? "true" : "false";
	os << io::widen("shader: ") << io::braces_left << std::endl <<
		io::tab << io::widen("type: ") << io::widen(type) << std::endl <<
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
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const shader& v)
{
	os << v;
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_SHADER_H__
