#ifndef __POCKET_BUFFER_BASE_H__
#define __POCKET_BUFFER_BASE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"

namespace pocket
{
namespace gl
{

// forward
class buffer_base;

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
	// マップ方法
	enum map_usage_type
	{
		read,
		write,
		read_write
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
	static const GLenum gl_buffer_type_table[14];
	static const GLenum gl_usage_type_table[9];
	static const GLenum gl_map_usage_type_table[3];
	static const GLenum gl_binding_buffer_type_table[14];

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
	buffer_base(GLenum type, int err) :
		_type(type), _error_bitfield(err)
	{}

public:
	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// エラー状態をクリア
	void clear()
	{
		_error_bitfield = 0;
	}

protected:
	// members ------------------------------------------------------------------------------------------

	GLenum to_gl_binding_type() const
	{
		return buffer_base::to_gl_binding_type(_type);
	}
	buffer_type to_buffer_type() const
	{
		return buffer_base::to_buffer_type(_type);
	}

public:
	// statics ------------------------------------------------------------------------------------------

	// バッファ型の値に変換
	static inline GLenum to_gl_type(buffer_type type)
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
				return _GL_UNINITIALIZED_VALUE;
		}
#endif
		return buffer_base::gl_buffer_type_table[type];
	}

	// バッファの扱い方の値に変換
	static inline GLenum to_gl_usage(usage_type type)
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
				return _GL_UNINITIALIZED_VALUE;
		}
#endif
		return buffer_base::gl_usage_type_table[type];
	}

	// 展開の方法の値に変換
	static inline GLenum to_gl_map_usage(map_usage_type type)
	{
#if 0
		switch (type)
		{
			case read:
				return GL_READ_ONLY;
			case write:
				return GL_READ_ONLY;
			case read_write:
				return GL_READ_WRITE;

			default:
				return _GL_UNINITIALIZED_VALUE;
		}
#endif
		return buffer_base::gl_map_usage_type_table[type];
	}

	// バインドの状態を取得する値に変換
	static inline GLenum to_gl_binding_type(buffer_type type)
	{
#if 0
		switch (type)
		{
			case array:
				return GL_ARRAY_BUFFER_BINDING;
			case atomic_counter:
				return GL_ATOMIC_COUNTER_BUFFER_BINDING;
			case copy_read:
				return GL_COPY_READ_BUFFER_BINDING;
			case copy_write:
				return GL_COPY_WRITE_BUFFER_BINDING;
			case dispatch_indirect:
				return GL_DISPATCH_INDIRECT_BUFFER_BINDING;
			case draw_indirect:
				return GL_DRAW_INDIRECT_BUFFER_BINDING;
			case element:
				return GL_ELEMENT_ARRAY_BUFFER_BINDING;
			case pixel_pack:
				return GL_PIXEL_PACK_BUFFER_BINDING;
			case pixel_unpack:
				return GL_PIXEL_UNPACK_BUFFER_BINDING;
			case query:
				return GL_QUERY_BUFFER_BINDING;
			case shader_storage:
				return GL_SHADER_STORAGE_BUFFER_BINDING;
			case texture:
				return GL_TEXTURE_BUFFER_BINDING;
			case transform_feedback:
				return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
			case uniform:
				return GL_UNIFORM_BUFFER_BINDING;

			case unknown:
			default:
				return _GL_UNINITIALIZED_VALUE;
		}
#endif
		return buffer_base::gl_binding_buffer_type_table[type];
	}
	static inline GLenum to_gl_binding_type(GLenum type)
	{
		switch (type)
		{
			case GL_ARRAY_BUFFER:
				return GL_ARRAY_BUFFER_BINDING;
			case GL_ATOMIC_COUNTER_BUFFER:
				return GL_ATOMIC_COUNTER_BUFFER_BINDING;
			case GL_COPY_READ_BUFFER:
				return GL_COPY_READ_BUFFER_BINDING;
			case GL_COPY_WRITE_BUFFER:
				return GL_COPY_WRITE_BUFFER_BINDING;
			case GL_DISPATCH_INDIRECT_BUFFER:
				return GL_DISPATCH_INDIRECT_BUFFER_BINDING;
			case GL_DRAW_INDIRECT_BUFFER:
				return GL_DRAW_INDIRECT_BUFFER_BINDING;
			case GL_ELEMENT_ARRAY_BUFFER:
				return GL_ELEMENT_ARRAY_BUFFER_BINDING;
			case GL_PIXEL_PACK_BUFFER:
				return GL_PIXEL_PACK_BUFFER_BINDING;
			case GL_PIXEL_UNPACK_BUFFER:
				return GL_PIXEL_UNPACK_BUFFER_BINDING;
			case GL_QUERY_BUFFER:
				return GL_QUERY_BUFFER_BINDING;
			case GL_SHADER_STORAGE_BUFFER:
				return GL_SHADER_STORAGE_BUFFER_BINDING;
			case GL_TEXTURE_BUFFER:
				return GL_TEXTURE_BUFFER_BINDING;
			case GL_TRANSFORM_FEEDBACK_BUFFER:
				return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
			case GL_UNIFORM_BUFFER:
				return GL_UNIFORM_BUFFER_BINDING;

			default:
				return _GL_UNINITIALIZED_VALUE;
		}
	}

	// enumの型に変換
	static inline buffer_type to_buffer_type(GLenum type)
	{
		switch (type)
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

const GLenum buffer_base::gl_buffer_type_table[14] = {
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
	GL_UNIFORM_BUFFER
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
const GLenum buffer_base::gl_map_usage_type_table[3] = {
	GL_READ_ONLY,
	GL_WRITE_ONLY,
	GL_READ_WRITE
};
const GLenum buffer_base::gl_binding_buffer_type_table[14] = {
	GL_ARRAY_BUFFER_BINDING,
	GL_ATOMIC_COUNTER_BUFFER_BINDING,
	GL_COPY_READ_BUFFER_BINDING,
	GL_COPY_WRITE_BUFFER_BINDING,
	GL_DISPATCH_INDIRECT_BUFFER_BINDING,
	GL_DRAW_INDIRECT_BUFFER_BINDING,
	GL_ELEMENT_ARRAY_BUFFER_BINDING,
	GL_PIXEL_PACK_BUFFER_BINDING,
	GL_PIXEL_UNPACK_BUFFER_BINDING,
	GL_QUERY_BUFFER_BINDING,
	GL_SHADER_STORAGE_BUFFER_BINDING,
	GL_TEXTURE_BUFFER_BINDING,
	GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
	GL_UNIFORM_BUFFER_BINDING
};

} // namespace gl
} // namespace pocket

#endif // __POCKET_BUFFER_BASE_H__
