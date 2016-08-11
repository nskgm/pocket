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
#ifdef _USE_CXX11
#include <utility>
#endif // _USE_CXX11

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
		tessellation_control,
		tessellation_evaluate,
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

	shader() :
		_type(unknown),
		_handle(0),
		_error_bitfield(0)
	{

	}
	shader(shader_type type, const char* str, compile_type comp = file)
	{
		initialize(type, str, comp);
	}
	shader(const shader& s) :
		_type(s._type),
		_handle(s._handle),
		_error_bitfield(s._error_bitfield)
	{

	}
#ifdef _USE_CXX11
	shader(shader&& s) :
		_type(std::move(s._type)),
		_handle(std::move(s._handle)),
		_error_bitfield(std::move(s._error_bitfield))
	{
		s._type = unknown;
		s._handle = 0;
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
	bool initialize(shader_type type, const char* s, compile_type comp = file)
	{
		finalize();
		// ファイルから作成
		if (comp == file)
		{
			return create_from_file(type, s);
		}
		// 文字列から作成
		return create_from_memory(type, s);
	}
	// 終了処理
	void finalize()
	{
		if (_handle != 0)
		{
			glDeleteShader(_handle);
			_handle = 0;
		}
		_type = unknown;
		_error_bitfield = 0;
	}

	// プログラムへアタッチ
	void attach(GLuint prog) const
	{
		_DEB_ASSERT(_handle != 0 && prog != 0);
		glAttachShader(prog, _handle);
	}

	// アタッチの解除
	void detach(GLuint prog) const
	{
		_DEB_ASSERT(_handle != 0 && prog != 0);
		glDetachShader(prog, _handle);
	}

	// コード取得
	std::string code(GLsizei length = 1024) const
	{
		_DEB_ASSERT(valid());
		std::string source(length, '\0');
		glGetShaderSource(_handle, length, NULL, &source[0]);
		return source;
	}

	// エラー文
	std::string error() const
	{
		// シェーダーハンドルの作成に失敗している
		if (error_status(error_create_shader))
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
			if (_handle != 0)
			{
				GLint length = 0;
				glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &length);
				if (length > 0)
				{
					std::string log(length, '\0');
					glGetShaderInfoLog(_handle, length, NULL, &log[0]);
					return "failed. compile shader. # " + log;
				}
				else
				{
					return "failed. compile shader.";
				}
			}
			else
			{
				return "failed. compile shader.";
			}
		}
		// 作成されていない
		// またはすでに破棄済み
		if (_type == unknown ||
			_handle == 0)
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
		return glIsShader(_handle) == GL_TRUE;
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
		return _handle;
	}
	const GLuint& get() const
	{
		return _handle;
	}

private:
	bool create_from_file(shader_type type, const char* path)
	{
		_DEB_ASSERT(path != NULL);

		std::ifstream fs(path, std::ios::in | std::ios::ate);
		// ファイルが存在していない
		if (fs.fail())
		{
			_error_bitfield |= error_file_not_exist;
			return false;
		}

		// ファイルサイズ取得
		std::ifstream::pos_type size = fs.tellg();
		fs.seekg(0, std::ios::beg);
		// ファイル全内容
		std::string source(size, '\0');
		fs.read(&source[0], size);

		fs.close();

		// ファイル内容からコンパイル
		return create_from_memory(type, source.c_str());
	}
	bool create_from_memory(shader_type type, const char* s)
	{
		_DEB_ASSERT(s != NULL);

		_type = type;

		// シェーダーハンドル作成
		GLuint gl_type = to_gl_shader_type();
		_handle = glCreateShader(gl_type);
		// 作成失敗
		if (_handle == 0)
		{
			_error_bitfield |= error_create_shader;
			return false;
		}
		// ソースファイルのコンパイル
		glShaderSource(_handle, 1, &s, NULL);
		glCompileShader(_handle);

		GLint compiled;
		glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);
		// コンパイルが成功していない
		if (compiled == GL_FALSE)
		{
			_error_bitfield |= error_compiling;
			return false;
		}
		return glIsShader(_handle) == GL_TRUE;
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
			case tessellation_control:
				return GL_TESS_CONTROL_SHADER;
			case tessellation_evaluate:
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

	bool operator == (const shader& s) const
	{
		return _handle == s._handle && _type == s._type;
	}
	bool operator != (const shader& s) const
	{
		return !(*this == s);
	}

	shader& operator = (const shader& s)
	{
		_type = s._type;
		_handle = s._handle;
		_error_bitfield = s._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	shader& operator = (shader&& s)
	{
		_type = std::move(s._type);
		_handle = std::move(s._handle);
		_error_bitfield = std::move(s._error_bitfield);
		s._type = unknown;
		s._handle = 0;
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

// 頂点シェーダー作成
inline
shader make_vertex_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::vertex, s, compile);
}
inline
shader& make_vertex_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::vertex, s, compile);
	return sdr;
}
// ピクセルシェーダー作成
inline
shader make_fragment_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::fragment, s, compile);
}
inline
shader& make_fragment_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::fragment, s, compile);
	return sdr;
}
// ジオメトリシェーダー作成
inline
shader make_geometry_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::geometry, s, compile);
}
inline
shader& make_geometry_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::geometry, s, compile);
	return sdr;
}
// テセレーション制御シェーダー作成
inline
shader make_tess_control_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::tessellation_control, s, compile);
}
inline
shader& make_tess_control_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::tessellation_control, s, compile);
	return sdr;
}
// テセレーション評価シェーダー作成
inline
shader make_tess_eval_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::tessellation_evaluate, s, compile);
}
inline
shader& make_tess_eval_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::tessellation_evaluate, s, compile);
	return sdr;
}
// コンピュートシェーダー作成
inline
shader make_compute_shader(const char* s, shader::compile_type compile = shader::file)
{
	return shader(shader::compute, s, compile);
}
inline
shader& make_computel_shader(shader& sdr, const char* s, shader::compile_type compile = shader::file)
{
	sdr.initialize(shader::compute, s, compile);
	return sdr;
}

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
		case shader::tessellation_control:
			type = "tessellation_control";
			break;
		case shader::tessellation_evaluate:
			type = "tessellation_evaluate";
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
