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
#include "../container/array.h"
#include "shader.h"
#include "buffer_base.h"
#include <string>
#include <fstream>

namespace pocket
{
namespace gl
{

// forward
class program;
class uniform_buffer;

// Uniform変数に代入するための型
class program_uniform_assign
{
	const GLuint& prog;
	const GLint location;

public:
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
	static inline void call(GLuint, const T&)
	{}
};
template <>
struct call_uniform<char*>
{
	static inline GLint call(GLuint prog, const char* name)
	{
		return glGetUniformLocation(prog, name);
	}
};
template <>
struct call_uniform<char const*>
{
	static inline GLint call(GLuint prog, const char* const name)
	{
		return glGetUniformLocation(prog, name);
	}
};
template <std::size_t N>
struct call_uniform<char[N]>
{
	static inline GLint call(GLuint prog, const char* name)
	{
		return glGetUniformLocation(prog, name);
	}
};
template <>
struct call_uniform<std::string>
{
	static inline GLint call(GLuint prog, const std::string& name)
	{
		return glGetUniformLocation(prog, name.c_str());
	}
};

// ロケーションから代入するための型
template <>
struct call_uniform<int>
{
	static inline program_uniform_assign call(const GLuint& prog, int location)
	{
		return program_uniform_assign(prog, static_cast<GLint>(location));
	}
};
template <>
struct call_uniform<unsigned int>
{
	static inline program_uniform_assign call(const GLuint& prog, unsigned int location)
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
template <>
struct is_uniform_get_location_type<char const*> : type_traits::true_type
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

// VScodeシンタックスハイライト解除回避
// defineのスコープもおかしくなっているのでdetailの中に宣言
#define __POCKET_STD_STRING_ARRAY_REF(NAME, N) const std::string(&NAME)[N]

}

class program
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef binder<program> binder_type;

	template <int N>
	struct indices_t
	{
		typedef container::array<GLuint, N> type;
	};

	enum identifier_t
	{
		identifier = GL_PROGRAM
	};

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
		_id(0)
	{
		initialize(s, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, bool save_bin = false) :
		_id(0)
	{
		initialize(s1, s2, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, bool save_bin = false) :
		_id(0)
	{
		initialize(s1, s2, s3, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, bool save_bin = false) :
		_id(0)
	{
		initialize(s1, s2, s3, s4, save_bin);
	}
	explicit program(const shader& s1, const shader& s2, const shader& s3, const shader& s4, const shader& s5, bool save_bin = false) :
		_id(0)
	{
		initialize(s1, s2, s3, s4, s5, save_bin);
	}
	explicit program(const char* path, GLenum format, bool file_front_format_written = true) :
		_id(0)
	{
		initialize(path, format, file_front_format_written);
	}
	explicit program(const std::string& path, GLenum format, bool file_front_format_written = true) :
		_id(0)
	{
		initialize(path, format, file_front_format_written);
	}
	explicit program(const char* path, bool file_front_format_written = true) :
		_id(0)
	{
		initialize(path, file_front_format_written);
	}
	explicit program(const std::string& path, bool file_front_format_written = true) :
		_id(0)
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
		return link();
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
		return link();
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
		return link();
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
		return link();
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
		return link();
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
		GLuint i;
		glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&i));
		// バインドされていない
		if (i == 0)
		{
			return false;
		}
		// 現在バインドされているIDが同じか
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

	// プログラムが保存可能か
	bool savable() const
	{
		// 使用できるバイナリフォーマット数
		GLint format_count = 0;
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &format_count);
		return format_count != 0;
	}

	// シェーダーから作成されたアセンブラバイナリを保存する
	bool save_binary(const char* path, GLenum& format, bool file_front_format_write = true)
	{
		// 使用できるバイナリフォーマット数
		if (!savable())
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

	// 現在描画が可能か検証
	bool drawable() const
	{
		// 初期化時には行わない様にする
		glValidateProgram(_id);
		GLint validated;
		glGetProgramiv(_id, GL_VALIDATE_STATUS, &validated);
		return validated == GL_TRUE;
	}
	bool drawable(std::string& log) const
	{
		if (!drawable())
		{
			GLint length = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				log.resize(static_cast<size_t>(length));
				glGetProgramInfoLog(_id, length, NULL, &log[0]);
			}
			return false;
		}
		return true;
	}
	template <int N>
	bool drawable(char(&log)[N]) const
	{
		if (!drawable())
		{
			GLint length = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				length = (std::min)(length, N);
				glGetProgramInfoLog(_id, length, NULL, &log[0]);
			}
			return false;
		}
		return true;
	}

	// ユニフォーム変数のローケーション取得
	GLint uniform_location(const char* name) const
	{
		return glGetUniformLocation(_id, name);
	}
	GLint uniform_location(const std::string& name) const
	{
		return glGetUniformLocation(_id, name.c_str());
	}
	bool uniform_location(const char* name, GLint& loc) const
	{
		loc = uniform_location(name);
		return loc >= 0;
	}
	bool uniform_location(const std::string& name, GLint& loc) const
	{
		loc = uniform_location(name);
		return loc >= 0;
	}

	template <int N>
	void uniform_indices(const char*(&s)[N], GLuint(&i)[N]) const
	{
		glGetUniformIndices(_id, N, &s[0], &i[0]);
	}
	template <int N>
	void uniform_indices(__POCKET_STD_STRING_ARRAY_REF(s, N), GLuint(&a)[N]) const
	{
		const char* c_str[N];
		for (int i = 0; i < N; ++i)
		{
			c_str[i] = s[i].c_str();
		}
		glGetUniformIndices(_id, N, &c_str[0], &a[0]);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	void uniform_indices(const char*(&s)[N], ARRAY<GLuint, N>& i)
	{
		glGetUniformIndices(_id, N, &s[0], &i[0]);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	void uniform_indices(__POCKET_STD_STRING_ARRAY_REF(s, N), ARRAY<GLuint, N>& a) const
	{
		const char* c_str[N];
		for (int i = 0; i < N; ++i)
		{
			c_str[i] = s[i].c_str();
		}
		glGetUniformIndices(_id, static_cast<GLsizei>(N), &c_str[0], &a[0]);
	}
	template <int N>
	typename indices_t<N>::type uniform_indices(const char*(&s)[N])
	{
		typename indices_t<N>::type a;
		glGetUniformIndices(_id, N, &s[0], &a[0]);
		return _CXX11_MOVE(a);
	}
	template <int N>
	typename indices_t<N>::type uniform_indices(__POCKET_STD_STRING_ARRAY_REF(s, N))
	{
		typename indices_t<N>::type a;
		const char* c_str[N];
		for (int i = 0; i < N; ++i)
		{
			c_str[i] = s[i].c_str();
		}
		glGetUniformIndices(_id, N, &c_str[0], &a[0]);
		return _CXX11_MOVE(a);
	}

	// uniform block数
	int uniform_block_count() const
	{
		GLint count;
		glGetProgramiv(_id, GL_ACTIVE_UNIFORM_BLOCKS, &count);
		return static_cast<int>(count);
	}
	// uniform block 内ブロック数
	int uniform_block_count(GLuint index) const
	{
		GLint count = 0;
		glGetActiveUniformBlockiv(_id, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count);
		return static_cast<int>(count);
	}
	int uniform_block_count(const char* name) const
	{
		return uniform_block_count(uniform_block_index(name));
	}
	int uniform_block_count(const std::string& name) const
	{
		return uniform_block_count(uniform_block_index(name));
	}

	// uniform block合計サイズ
	int uniform_block_size(GLuint index) const
	{
		GLint size = 0;
		glGetActiveUniformBlockiv(_id, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
		return static_cast<int>(size);
	}
	int uniform_block_size(const char* name) const
	{
		return uniform_block_size(uniform_block_index(name));
	}
	int uniform_block_size(const std::string& name) const
	{
		return uniform_block_size(uniform_block_index(name));
	}

	// 名前に対するuniform blockのインデックス
	GLuint uniform_block_index(const char* name) const
	{
		return glGetUniformBlockIndex(_id, name);
	}
	bool uniform_block_index(const char* name, GLuint& index) const
	{
		index = glGetUniformBlockIndex(_id, name);
		// インデックスが無効ではないか
		return index != GL_INVALID_INDEX;
	}
	GLuint uniform_block_index(const std::string& name) const
	{
		return uniform_block_index(name.c_str());
	}
	bool uniform_block_index(const std::string& name, GLuint& index) const
	{
		return uniform_block_index(name.c_str(), index);
	}

	// uniform bufferのバインド位置を更新
	void uniform_block_bind(GLuint index, GLuint point) const
	{
		glUniformBlockBinding(_id, index, point);
	}
	void uniform_block_bind(const uniform_buffer&) const;

	// インデックスから名前を取得
	template <int N>
	void uniform_block_name_from_location(GLuint location, char(&name)[N]) const
	{
		glGetActiveUniformName(_id, location, N, NULL, &name[0]);
	}
	std::string uniform_block_name_from_location(GLuint location, GLsizei length = 32) const
	{
		std::string name(static_cast<size_t>(length), '\0');
		glGetActiveUniformName(_id, location, length, &length, &name[0]);
		name.resize(static_cast<size_t>(length));
		return _CXX11_MOVE(name);
	}
	template <int N>
	void uniform_block_name(GLuint index, char(&name)[N]) const
	{
		glGetActiveUniformBlockName(_id, index, N, NULL, &name[0]);
	}
	std::string uniform_block_name(GLuint index, GLsizei length = 32) const
	{
		std::string name(static_cast<size_t>(length), '\0');
		glGetActiveUniformBlockName(_id, index, length, &length, &name[0]);
		name.resize(static_cast<size_t>(length));
		return _CXX11_MOVE(name);
	}

	// uniform block情報の出力
	template <typename CharT, typename CharTraits>
	void refrect_uniform_block(std::basic_ostream<CharT, CharTraits>& os, std::basic_ostream<CharT, CharTraits>&(*func)(std::basic_ostream<CharT, CharTraits>&) = NULL) const
	{
		// uniform block数
		int count = uniform_block_count();
		for (int i = 0; i < count; ++i)
		{
			char name[256];
			uniform_block_name(i, name);
			if (func != NULL)
			{
				func(os);
			}
			os << io::widen(static_cast<const char*>(&name[0])) << io::widen(": {") << std::endl;

			if (func != NULL)
			{
				func(os);
			}
			int size = uniform_block_size(i);
			os << io::tab << io::widen("size: ") << size << std::endl;

			int ubcount = uniform_block_count(i);
			if (func != NULL)
			{
				func(os);
			}
			os << io::tab << io::widen("variable count: ") << ubcount << std::endl;

			// インデックス取得
			std::vector<GLint> indices(ubcount);
			glGetActiveUniformBlockiv(_id, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &indices[0]);

			if (func != NULL)
			{
				func(os);
			}
			os << io::tab << io::widen("variables: [") << std::endl;

			// uniform block内 uniform変数情報
			for (int j = 0; j < ubcount; ++j)
			{
				GLuint idx = indices[j];

				if (func != NULL)
				{
					func(os);
				}
				// 変数名
				char varname[256];
				uniform_block_name_from_location(idx, varname);
				os << io::tab2 << io::widen(static_cast<const char*>(&varname[0])) << io::widen(": {") << std::endl;

				GLint varsize, varoffset, array_stride, matrix_stride, row_major, type;
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_SIZE, &varsize);
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_OFFSET, &varoffset);
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_ARRAY_STRIDE, &array_stride);
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_MATRIX_STRIDE, &matrix_stride);
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_IS_ROW_MAJOR, &row_major);
				glGetActiveUniformsiv(_id, 1, &idx, GL_UNIFORM_TYPE, &type);

				// ロケーション
				if (func != NULL)
				{
					func(os);
				}
				os << io::tab3 << io::widen("location: ") << idx << std::endl;
				// 型
				if (func != NULL)
				{
					func(os);
				}
				std::ios_base::fmtflags flag = os.flags();
				os << std::hex << io::tab3 << io::widen("type: 0x") << type << std::endl;
				os.flags(flag);
				// サイズ
				if (func != NULL)
				{
					func(os);
				}
				varsize *= gl::get_type_size(type); // 配列のサイズが入ってくるから型のサイズを掛け合わせる
				os << io::tab3 << io::widen("size: ") << varsize << std::endl;
				// オフセット
				if (func != NULL)
				{
					func(os);
				}
				os << io::tab3 << io::widen("offset: ") << varoffset << std::endl;
				if (func != NULL)
				{
					func(os);
				}
				os << io::tab3 << io::widen("array stride: ") << array_stride << std::endl;
				if (func != NULL)
				{
					func(os);
				}
				os << io::tab3 << io::widen("matrix stride: ") << matrix_stride << std::endl;
				// 行優先か
				if (func != NULL)
				{
					func(os);
				}
				os << io::tab3 << io::widen("is row major: ") << io::widen(row_major == GL_TRUE) << std::endl;

				if (func != NULL)
				{
					func(os);
				}
				os << io::tab2 << io::braces_right;

				if (j + 1 < ubcount)
				{
					os << io::comma;
				}
				os << std::endl;
			}
			if (func != NULL)
			{
				func(os);
			}
			os << io::tab << io::box_brackets_right << std::endl;

			if (func != NULL)
			{
				func(os);
			}
			os << io::braces_right << std::endl;
		}
	}

	// UBO作成
	uniform_buffer make_uniform_buffer(const char*, GLuint, const void*, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T>
	uniform_buffer make_uniform_buffer(const char*, GLuint, const T&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, int N>
	uniform_buffer make_uniform_buffer(const char*, GLuint, const T(&)[N], buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	uniform_buffer make_uniform_buffer(const char*, GLuint, const ARRAY<T, N>&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	uniform_buffer make_uniform_buffer(const char*, GLuint, const VECTOR<T, ALLOC>&, buffer_usage_t = buffer_usage::dynamic_draw) const;

	uniform_buffer make_uniform_buffer(const std::string&, GLuint, const void*, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T>
	uniform_buffer make_uniform_buffer(const std::string&, GLuint, const T&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, int N>
	uniform_buffer make_uniform_buffer(const std::string&, GLuint, const T(&)[N], buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	uniform_buffer make_uniform_buffer(const std::string&, GLuint, const ARRAY<T, N>&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	uniform_buffer make_uniform_buffer(const std::string&, GLuint, const VECTOR<T, ALLOC>&, buffer_usage_t = buffer_usage::dynamic_draw) const;

	uniform_buffer& make_uniform_buffer(uniform_buffer&, const char*, GLuint, const void*, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const char*, GLuint, const T&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, int N>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const char*, GLuint, const T(&)[N], buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const char*, GLuint, const ARRAY<T, N>&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const char*, GLuint, const VECTOR<T, ALLOC>&, buffer_usage_t = buffer_usage::dynamic_draw) const;

	uniform_buffer& make_uniform_buffer(uniform_buffer&, const std::string&, GLuint, const void*, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const std::string&, GLuint, const T&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, int N>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const std::string&, GLuint, const T(&)[N], buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const std::string&, GLuint, const ARRAY<T, N>&, buffer_usage_t = buffer_usage::dynamic_draw) const;
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	uniform_buffer& make_uniform_buffer(uniform_buffer&, const std::string&, GLuint, const VECTOR<T, ALLOC>&, buffer_usage_t = buffer_usage::dynamic_draw) const;

	// サブルーチン数
	int subroutine_count(GLuint index, shader::shader_type type) const
	{
		GLint n = 0;
		glGetActiveSubroutineUniformiv(_id, type, index, GL_NUM_COMPATIBLE_SUBROUTINES, &n);
		return static_cast<int>(n);
	}

	// サブルーチンインデックス取得
	GLuint subroutine_index(const char* name, shader::shader_type type) const
	{
		return glGetSubroutineIndex(_id, type, name);
	}
	GLuint subroutine_index(const std::string& name, shader::shader_type type) const
	{
		return glGetSubroutineIndex(_id, type, name.c_str());
	}
	GLuint subroutine_index(const char* name, const shader& s) const
	{
		return glGetSubroutineIndex(_id, s.kind(), name);
	}
	GLuint subroutine_index(const std::string& name, const shader& s) const
	{
		return glGetSubroutineIndex(_id, s.kind(), name.c_str());
	}
	template <shader::shader_type T>
	GLuint subroutine_index(const char* name) const
	{
		return glGetSubroutineIndex(_id, T, name);
	}
	template <shader::shader_type T>
	GLuint subroutine_index(const std::string& name) const
	{
		return glGetSubroutineIndex(_id, T, name.c_str());
	}
	bool subroutine_index(const char* name, shader::shader_type type, GLuint& index) const
	{
		index = subroutine_index(name, type);
		return index != GL_INVALID_INDEX;
	}
	bool subroutine_index(const std::string& name, shader::shader_type type, GLuint& index) const
	{
		index = subroutine_index(name, type);
		return index != GL_INVALID_INDEX;
	}
	bool subroutine_index(const char* name, const shader& s, GLuint& index) const
	{
		return subroutine_index(name, s.kind(), index);
	}
	bool subroutine_index(const std::string& name, const shader& s, GLuint& index) const
	{
		return subroutine_index(name, s.kind(), index);
	}
	template <shader::shader_type T>
	bool subroutine_index(const char* name, GLuint& index) const
	{
		index = subroutine_index(name, T);
		return index != GL_INVALID_INDEX;
	}
	template <shader::shader_type T>
	bool subroutine_index(const std::string& name, GLuint& index) const
	{
		index = subroutine_index(name, T);
		return index != GL_INVALID_INDEX;
	}

	// サブルーチン設定
	void subroutine(GLuint index, shader::shader_type type) const
	{
		glUniformSubroutinesuiv(type, 1, &index);
	}
	template <int N>
	void subroutine(const GLuint(&index)[N], shader::shader_type type) const
	{
		glUniformSubroutinesuiv(type, N, &index[0]);
	}
	void subroutine(GLuint index, const shader& s) const
	{
		glUniformSubroutinesuiv(s.kind(), 1, &index);
	}
	template <int N>
	void subroutine(const GLuint(&index)[N], const shader& s) const
	{
		glUniformSubroutinesuiv(s.kind(), N, &index[0]);
	}

	bool subroutine(const char* name, shader::shader_type type) const
	{
		GLuint index;
		if (!subroutine_index(name, type, index))
		{
			return false;
		}
		glUniformSubroutinesuiv(type, 1, &index);
		return true;
	}
	bool subroutine(const std::string& name, shader::shader_type type) const
	{
		return subroutine(name.c_str(), type);
	}
	bool subroutine(const char* name, const shader& s) const
	{
		return subroutine(name, s.kind());
	}
	bool subroutine(const std::string& name, const shader& s) const
	{
		return subroutine(name.c_str(), s.kind());
	}
	template <shader::shader_type T>
	bool subroutine(const char* name) const
	{
		return subroutine(name, T);
	}
	template <shader::shader_type T>
	bool subroutine(const std::string& name) const
	{
		return subroutine(name, T);
	}

	template <int N>
	bool subroutine(const char*(&name)[N], shader::shader_type type) const
	{
		GLuint indices[N];
		for (int i = 0; i < N; ++i)
		{
			if (!subroutine_index(name[i], type, indices[i]))
			{
				return false;
			}
		}
		glUniformSubroutinesuiv(type, N, &indices[0]);
		return true;
	}

	template <int N>
	bool subroutine(__POCKET_STD_STRING_ARRAY_REF(name, N), shader::shader_type type) const
	{
		GLuint indices[N];
		for (int i = 0; i < N; ++i)
		{
			if (!subroutine_index(name[i].c_str(), type, indices[i]))
			{
				return false;
			}
		}
		glUniformSubroutinesuiv(type, N, &indices[0]);
		return true;
	}
	template <int N>
	bool subroutine(const char*(&name)[N], const shader& s) const
	{
		return subroutine(name, s.kind());
	}
	template <int N>
	bool subroutine(__POCKET_STD_STRING_ARRAY_REF(name, N), const shader& s) const
	{
		return subroutine(name, s.kind());
	}
	template <shader::shader_type T, int N>
	bool subroutine(const char*(&name)[N]) const
	{
		return subroutine(name, T);
	}
	template <shader::shader_type T, int N>
	bool subroutine(__POCKET_STD_STRING_ARRAY_REF(name, N)) const
	{
		return subroutine(name, T);
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

	// 戻り値で型チェック
	template <typename T>
	typename type_traits::enable_if<
	 	!type_traits::is_same<typename detail::uniform_return_type<T>::type, void>::value,
	 	typename detail::uniform_return_type<T>::type
	 >::type
		operator [] (const T& v) const
	{
		typedef typename type_traits::remove_cv_reference<T>::type _type;
		return detail::call_uniform<_type>::call(_id, v);
	}
};


inline
bool shader::subroutine(const char* name, const program& prog) const
{
	return prog.subroutine(name, _type);
}
inline
bool shader::subroutine(const std::string& name, const program& prog) const
{
	return prog.subroutine(name, _type);
}
template <int N> inline
bool shader::subroutine(const char*(&name)[N], const program& prog) const
{
	return prog.subroutine(name, _type);
}
template <int N> inline
bool shader::subroutine(const std::string(&name)[N], const program& prog) const
{
	return prog.subroutine(name, _type);
}

#undef __POCKET_STD_STRING_ARRAY_REF

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
	os << io::widen("program: {") << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl;
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

#endif // __POCKET_GL_PROGRAM_H__
