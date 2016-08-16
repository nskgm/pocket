#ifndef __POCKET_GL_BUFFER_H__
#define __POCKET_GL_BUFFER_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"

namespace pocket
{
namespace gl
{

// forward
class buffer;
template <int> class buffers;

class buffer_base
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// バッファ種類
	enum buffer_type
	{
		array,
		atomic_counter,
		copy_read,
		copy_write,
		dispatch_indirect,
		draw_indirect,
		element,
		pixel_pack,
		pixel_unpack,
		query,
		shader_storage,
		texture,
		transform_feedback,
		uniform,

		unknown
	};
	// バッファの扱い方法
	enum usage_type
	{
		stream_draw,
		stream_read,
		stream_copy,

		static_draw,
		static_read,
		static_copy,

		dynamic_draw,
		dynamic_read,
		dynamic_copy,

		immutable_draw = static_draw,
		immutable_read = static_read,
		immutable_copy = static_copy,

		statical = static_draw,

		stream = stream_draw,
		immutable = immutable_draw,
		dynamic = dynamic_draw,
	};

protected:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLenum _type;
	int _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// 変換テーブル
	static const GLenum gl_buffer_type_table[15];
	static const GLenum gl_usage_type_table[9];

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffer_base() :
		_type(0),
		_error_bitfield(0)
	{}
	buffer_base(buffer_type type) :
		_type(to_gl_type(type)),
		_error_bitfield(0)
	{}
	buffer_base(GLenum type) :
		_type(type),
		_error_bitfield(0)
	{}
	buffer_base(const buffer_base& b) :
		_type(b._type),
		_error_bitfield(b._error_bitfield)
	{}
#ifdef _USE_CXX11
	buffer_base(buffer_base&& b) :
		_type(std::move(b._type)),
		_error_bitfield(std::move(b._error_bitfield))
	{
		b._type = 0;
		b._error_bitfield = 0;
	}
#endif // _USE_CXX11

	virtual ~buffer_base()
	{}

protected:
	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	GLenum to_gl_type(buffer_type type) const
	{
#if 0
		switch (type)
		{
			case array:
				return GL_ARRAY_BUFFER;
			case atomic_counter:
				return GL_ATOMIC_COUNTER_BUFFER;
			case copy_read:
				return GL_COPY_READ_BUFFER;
			case copy_write:
				return GL_COPY_WRITE_BUFFER;
			case dispatch_indirect:
				return GL_DISPATCH_INDIRECT_BUFFER;
			case draw_indirect:
				return GL_DRAW_INDIRECT_BUFFER;
			case element:
				return GL_ELEMENT_ARRAY_BUFFER;
			case pixel_pack:
				return GL_PIXEL_PACK_BUFFER;
			case pixel_unpack:
				return GL_PIXEL_UNPACK_BUFFER;
			case query:
				return GL_QUERY_BUFFER;
			case shader_storage:
				return GL_SHADER_STORAGE_BUFFER;
			case texture:
				return GL_TEXTURE_BUFFER;
			case transform_feedback:
				return GL_TRANSFORM_FEEDBACK_BUFFER;
			case uniform:
				return GL_UNIFORM_BUFFER;

			case unknown:
			default:
				return GL_INVALID_VALUE;
		}
#endif
		return buffer_base::gl_buffer_type_table[type];
	}
	GLenum to_gl_usage(usage_type type) const
	{
#if 0
		switch (type)
		{
			case stream_draw:
				return GL_STREAM_DRAW;
			case stream_read:
				return GL_STREAM_READ;
			case stream_copy:
				return GL_STREAM_COPY;

			case immutable_draw:
				return GL_STATIC_DRAW;
			case immutable_read:
				return GL_STATIC_READ;
			case immutable_copy:
				return GL_STATIC_COPY;

			case dynamic_draw:
				return GL_DYNAMIC_DRAW;
			case dynamic_read:
				return GL_DYNAMIC_READ;
			case dynamic_copy:
				return GL_DYNAMIC_COPY;

			default:
				return GL_INVALID_VALUE;
		}
#endif
		return buffer_base::gl_usage_type_table[type];
	}

	buffer_type to_buffer_type() const
	{
		switch (_type)
		{
			case GL_ARRAY_BUFFER:
				return array;
			case GL_ATOMIC_COUNTER_BUFFER:
				return atomic_counter;
			case GL_COPY_READ_BUFFER:
				return copy_read;
			case GL_COPY_WRITE_BUFFER:
				return copy_write;
			case GL_DISPATCH_INDIRECT_BUFFER:
				return dispatch_indirect;
			case GL_DRAW_INDIRECT_BUFFER:
				return draw_indirect;
			case GL_ELEMENT_ARRAY_BUFFER:
				return element;
			case GL_PIXEL_PACK_BUFFER:
				return pixel_pack;
			case GL_PIXEL_UNPACK_BUFFER:
				return pixel_unpack;
			case GL_QUERY_BUFFER:
				return query;
			case GL_SHADER_STORAGE_BUFFER:
				return shader_storage;
			case GL_TEXTURE_BUFFER:
				return texture;
			case GL_TRANSFORM_FEEDBACK_BUFFER:
				return transform_feedback;
			case GL_UNIFORM_BUFFER:
				return uniform;

			default:
				return unknown;
		}
	}

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	// none
};

const GLenum buffer_base::gl_buffer_type_table[15] = {
	GL_ARRAY_BUFFER,
	GL_ATOMIC_COUNTER_BUFFER,
	GL_COPY_READ_BUFFER,
	GL_COPY_WRITE_BUFFER,
	GL_DISPATCH_INDIRECT_BUFFER,
	GL_DRAW_INDIRECT_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
	GL_PIXEL_PACK_BUFFER,
	GL_PIXEL_UNPACK_BUFFER,
	GL_QUERY_BUFFER,
	GL_SHADER_STORAGE_BUFFER,
	GL_TEXTURE_BUFFER,
	GL_TRANSFORM_FEEDBACK_BUFFER,
	GL_UNIFORM_BUFFER,
#ifdef GL_DONT_CARE
	GL_DONT_CARE
#else
	GL_INVALID_VALUE
#endif // GL_DONT_CARE
};
const GLenum buffer_base::gl_usage_type_table[9] = {
	GL_STREAM_DRAW,
	GL_STREAM_READ,
	GL_STREAM_COPY,
	GL_STATIC_DRAW,
	GL_STATIC_READ,
	GL_STATIC_COPY,
	GL_DYNAMIC_DRAW,
	GL_DYNAMIC_READ,
	GL_DYNAMIC_COPY
};

class buffer : public buffer_base
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef binder<buffer> binder_type;
	typedef buffer_base base;

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _id;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffer() :
		base(),
		_id(0)
	{}
	explicit buffer(GLenum id, buffer_type type) :
		base(type),
		_id(id)
	{}
	explicit buffer(GLenum id, GLenum type) :
		base(type),
		_id(id)
	{}
	explicit buffer(buffer_type type, usage_type usage, int size, const void* data)
	{
		initialize(type, usage, size, data);
	}
	buffer(const buffer& b) :
		base(b),
		_id(b._id)
	{}
#ifdef _USE_CXX11
	buffer(buffer&& b) :
		base(std::forward<buffer_base>(b)),
		_id(std::move(b._id))
	{
		b._id = 0;
	}
#endif // _USE_CXX11
	~buffer()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	bool initialize(buffer_type type, usage_type usg, int size, const void* data)
	{
		finalize();

		// OpenGL側での種類
		_type = to_gl_type(type);

		glGenBuffers(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		glBindBuffer(_type, _id);

		// 扱い方
		GLenum gl_usage = to_gl_usage(usg);
		// 動的バッファの場合は容量を確保してからデータを設定する
		if ((usg == dynamic_draw || usg == dynamic_read || usg == dynamic_copy) &&
			data != NULL)
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), NULL, gl_usage);
			glBufferSubData(_type, 0, static_cast<GLsizeiptr>(size), data);
		}
		else
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), data, gl_usage);
		}

		bool vld = glIsBuffer(_id) == GL_TRUE;
		if (!vld)
		{
			_error_bitfield |= error_binding;
		}

		glBindBuffer(_type, 0);
		return vld;
	}

	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
		_error_bitfield = 0;
		_type = 0;
	}

	// バインド
	void bind() const
	{
		glBindBuffer(_type, _id);
	}
	void bind(GLenum type) const
	{
		glBindBuffer(type, _id);
	}
	void bind(buffer_type type) const
	{
		bind(to_gl_type(type));
	}
	void bind_base(GLuint point) const
	{
		glBindBufferBase(_type, point, _id);
	}
	void bind_base(buffer_type type) const
	{
		GLenum t = to_gl_type(type);
		glBindBufferBase(t, 0, _id);
	}
	void bind_base(buffer_type type, GLuint point) const
	{
		GLenum t = to_gl_type(type);
		glBindBufferBase(t, point, _id);
	}
	void bind_vertex(GLuint index, GLintptr offset, GLsizei stride) const
	{
		glBindVertexBuffer(index, _id, offset, stride);
	}
	template <typename T>
	void bind_vertex(GLuint index) const
	{
		glBindVertexBuffer(index, _id, 0, sizeof(T));
	}

	// バインド解除
	void unbind() const
	{
		glBindBuffer(_type, 0);
	}
	void unbind(GLenum type) const
	{
		glBindBuffer(type, 0);
	}
	void unbind(buffer_type type) const
	{
		unbind(to_gl_type(type));
	}

	// バッファサイズ
	int size() const
	{
		binder_type lock(*this);
		return size_binding();
	}
	// バインドされていることを想定
	int size_binding() const
	{
		GLint i = 0;
		glGetBufferParameteriv(_type, GL_BUFFER_SIZE, &i);
		return static_cast<int>(i);
	}

	// 設定した時の扱い法
	GLenum usage() const
	{
		binder_type lock(*this);
		return usage_binding();
	}
	// バインドされていることを想定
	GLenum usage_binding() const
	{
		GLint u = 0;
		glGetBufferParameteriv(_type, GL_BUFFER_USAGE, &u);
		return static_cast<GLenum>(u);
	}

	// エラー文
	std::string error() const
	{
		// IDの作成に失敗している
		if (error_status(error_creating))
		{
			return "failed. glGenBuffers().";
		}
		if (error_status(error_binding))
		{
			return "failed. binding.";
		}
		// 作成されていない
		// またはすでに破棄済み
		buffer_type t = to_buffer_type();
		if (t == unknown ||
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
		return glIsBuffer(_id) == GL_TRUE;
	}

	// バッファ種類
	buffer_type kind() const
	{
		return to_buffer_type();
	}

	// バッファ種類の比較
	bool kind_of(buffer_type type) const
	{
		return kind() == type;
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

	bool operator == (const buffer& b) const
	{
		return _id == b._id && _type == b._type;
	}
	bool operator != (const buffer& b) const
	{
		return !(*this == b);
	}

	buffer& operator = (const buffer& b)
	{
		_type = b._type;
		_id = b._id;
		_error_bitfield = b._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	buffer& operator = (buffer&& b)
	{
		_type = std::move(b._type);
		_id = std::move(b._id);
		_error_bitfield = std::move(b._error_bitfield);
		b._type = 0;
		b._id = 0;
		b._error_bitfield = 0;
		return *this;
	}

	buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};

template <int N>
class buffers : public buffer_base
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// IDを保持と関数を定義する型
	// 関数実装はここに記述
	class holder
	{
	private:
		template <int>
		friend class buffers;
		GLuint _id;

	public:
		holder() :
			_id(0)
		{}

		// ハンドルの取得
		GLuint& get()
		{
			return _id;
		}
		const GLuint& get() const
		{
			return _id;
		}
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	// 数分保持
	holder _ids[N];
	int _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffers() :
		_error_bitfield(0)
	{}
	buffers(const buffers& b) :
		_error_bitfield(b._error_bitfield)
	{
		for (int i = 0; i < N; ++i)
		{
			_ids[i]._id = b._ids[i]._id;
		}
	}
#ifdef _USE_CXX11
	buffers(buffers&& b) :
		_ids(std::move(b._ids)),
		_error_bitfield(std::move(b._error_bitfield))
	{
		for (int i = 0; i < N; ++i)
		{
			b._ids[i]._id = 0;
		}
		b._error_bitfield = 0;
	}
#endif // _USE_CXX11

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	// none
};

/*------------------------------
* make_buffer
* make_[type]_buffer
* make_[type]_buffer_[usage(immutable, dynamic, stream)]
*------------------------------*/

#define __POCKET_MAKE_BUFFER_USAGE(TYPE, USAGE) inline \
	buffer make_##TYPE##_buffer_##USAGE(int size, const void* data) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, int size, const void* data) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T), &a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T(&a)[N]) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, &a[0]); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T(&a)[N]) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, &a[0]); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer_##USAGE(const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*a.size(), a.data()); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*a.size(), a.data()); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer_##USAGE(const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, a.data()); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, a.data()); \
		return b; \
	}

#define __POCKET_MAKE_BUFFER_TYPE(TYPE) inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, int size, const void* data) \
	{ \
		return buffer(buffer_base::TYPE, usage, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, int size, const void* data) \
	{ \
		b.initialize(buffer_base::TYPE, usage, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const T& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T), &a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const T& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const T(&a)[N]) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*N, &a[0]); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const T(&a)[N]) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*a.size(), a.data()); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T)*a.size(), a.data()); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*N, a.data()); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T)*N, a.data()); \
		return b; \
	} \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, immutable) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, dynamic) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, stream)

inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, int size, const void* data)
{
	return buffer(type, usage, size, data);
}
inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, int size, const void* data)
{
	b.initialize(type, usage, size, data);
	return b;
}
template <typename T> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const T& a)
{
	return buffer(type, usage, sizeof(T), &a);
}
template <typename T> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const T& a)
{
	b.initialize(type, usage, sizeof(T), &a);
	return b;
}
template <typename T, int N> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const T(&a)[N])
{
	return buffer(type, usage, sizeof(T)*N, &a[0]);
}
template <typename T, int N> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const T(&a)[N])
{
	b.initialize(type, usage, sizeof(T)*N, &a[0]);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a)
{
	return buffer(type, usage, sizeof(T)*a.size(), a.data());
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a)
{
	b.initialize(type, usage, sizeof(T)*a.size(), a.data());
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const ARRAY<T, N>& a)
{
	return buffer(type, usage, sizeof(T)*N, a.data());
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const ARRAY<T, N>& a)
{
	b.initialize(type, usage, sizeof(T)*N, a.data());
	return b;
}

__POCKET_MAKE_BUFFER_TYPE(array);
__POCKET_MAKE_BUFFER_TYPE(atomic_counter);
__POCKET_MAKE_BUFFER_TYPE(copy_read);
__POCKET_MAKE_BUFFER_TYPE(copy_write);
__POCKET_MAKE_BUFFER_TYPE(dispatch_indirect);
__POCKET_MAKE_BUFFER_TYPE(draw_indirect);
__POCKET_MAKE_BUFFER_TYPE(element);
__POCKET_MAKE_BUFFER_TYPE(pixel_pack);
__POCKET_MAKE_BUFFER_TYPE(pixel_unpack);
__POCKET_MAKE_BUFFER_TYPE(query);
__POCKET_MAKE_BUFFER_TYPE(shader_storage);
__POCKET_MAKE_BUFFER_TYPE(texture);
__POCKET_MAKE_BUFFER_TYPE(transform_feedback);
__POCKET_MAKE_BUFFER_TYPE(uniform);

#undef __POCKET_MAKE_BUFFER_TYPE
#undef __POCKET_MAKE_BUFFER_USAGE

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const buffer& v)
{
	const char* type;
	switch (v.kind())
	{
		case buffer_base::array:
			type = "array";
			break;
		case buffer_base::atomic_counter:
			type = "atomic_counter";
			break;
		case buffer_base::copy_read:
			type = "copy_read";
			break;
		case buffer_base::copy_write:
			type = "copy_write";
			break;
		case buffer_base::dispatch_indirect:
			type = "dispatch_indirect";
			break;
		case buffer_base::draw_indirect:
			type = "draw_indirect";
			break;
		case buffer_base::element:
			type = "element";
			break;
		case buffer_base::pixel_pack:
			type = "pixel_pack";
			break;
		case buffer_base::pixel_unpack:
			type = "pixel_unpack";
			break;
		case buffer_base::query:
			type = "query";
			break;
		case buffer_base::shader_storage:
			type = "shader_storage";
			break;
		case buffer_base::texture:
			type = "texture";
			break;
		case buffer_base::transform_feedback:
			type = "transform_feedback";
			break;
		case buffer_base::uniform:
			type = "uniform";
			break;

		case buffer_base::unknown:
		default:
			type = "unknown";
			break;
	}

	const bool valid = v.valid();
	const char* valid_str = valid ? "true" : "false";
	os << io::widen("buffer: ") << io::braces_left << std::endl <<
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
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const buffer& v)
{
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_BUFFER_H__
