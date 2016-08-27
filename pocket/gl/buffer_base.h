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

struct buffer_binding_type
{
	enum type
	{
		array = GL_ARRAY_BUFFER_BINDING,
		atomic_counter = GL_ATOMIC_COUNTER_BUFFER_BINDING,
		copy_read = GL_COPY_READ_BUFFER_BINDING,
		copy_write = GL_COPY_WRITE_BUFFER_BINDING,
		dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER_BINDING,
		draw_indirect = GL_DRAW_INDIRECT_BUFFER_BINDING,
		element_array = GL_ELEMENT_ARRAY_BUFFER_BINDING,
		pixel_pack = GL_PIXEL_PACK_BUFFER_BINDING,
		pixel_unpack = GL_PIXEL_UNPACK_BUFFER_BINDING,
		query = GL_QUERY_BUFFER_BINDING,
		shader_storage = GL_SHADER_STORAGE_BUFFER_BINDING,
		texture = GL_TEXTURE_BUFFER_BINDING,
		transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
		uniform = GL_UNIFORM_BUFFER_BINDING,

		unknown = 0,
	};
};
typedef buffer_binding_type::type buffer_binding_type_t;

struct buffer_type
{
	enum type
	{
		array = GL_ARRAY_BUFFER,
		atomic_counter = GL_ATOMIC_COUNTER_BUFFER,
		copy_read = GL_COPY_READ_BUFFER,
		copy_write = GL_COPY_WRITE_BUFFER,
		dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER,
		draw_indirect = GL_DRAW_INDIRECT_BUFFER,
		element_array = GL_ELEMENT_ARRAY_BUFFER,
		pixel_pack = GL_PIXEL_PACK_BUFFER,
		pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
		query = GL_QUERY_BUFFER,
		shader_storage = GL_SHADER_STORAGE_BUFFER,
		texture = GL_TEXTURE_BUFFER,
		transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		uniform = GL_UNIFORM_BUFFER,

		unknown = 0,
	};

	static inline buffer_binding_type_t to_binding_type(type a)
	{
#define __POCKET_CASE_TO_BINDING(c) case c: return buffer_binding_type:: c

		switch (a)
		{
			__POCKET_CASE_TO_BINDING(array);
			__POCKET_CASE_TO_BINDING(atomic_counter);
			__POCKET_CASE_TO_BINDING(copy_read);
			__POCKET_CASE_TO_BINDING(copy_write);
			__POCKET_CASE_TO_BINDING(dispatch_indirect);
			__POCKET_CASE_TO_BINDING(draw_indirect);
			__POCKET_CASE_TO_BINDING(element_array);
			__POCKET_CASE_TO_BINDING(pixel_pack);
			__POCKET_CASE_TO_BINDING(pixel_unpack);
			__POCKET_CASE_TO_BINDING(query);
			__POCKET_CASE_TO_BINDING(shader_storage);
			__POCKET_CASE_TO_BINDING(texture);
			__POCKET_CASE_TO_BINDING(transform_feedback);

			case unknown:
			default:
				return buffer_binding_type::unknown;
		}

#undef __POCKET_CASE_TO_BINDING
	}
};
typedef buffer_type::type buffer_type_t;

struct buffer_usage
{
	enum type
	{
		stream_draw = GL_STREAM_DRAW,
		stream_read = GL_STREAM_READ,
		stream_copy = GL_STREAM_COPY,

		static_draw = GL_STATIC_DRAW,
		static_read = GL_STATIC_READ,
		static_copy = GL_STATIC_COPY,

		dynamic_draw = GL_DYNAMIC_DRAW,
		dynamic_read = GL_DYNAMIC_READ,
		dynamic_copy = GL_DYNAMIC_COPY,

		immutable_draw = static_draw,
		immutable_read = static_read,
		immutable_copy = static_copy,

		statical = static_draw,

		stream = stream_draw,
		immutable = immutable_draw,
		dynamic = dynamic_draw,

		unknown = 0,
	};
};
typedef buffer_usage::type buffer_usage_t;

struct buffer_map
{
	enum type
	{
		read = GL_READ_ONLY,
		write = GL_WRITE_ONLY,
		read_write = GL_READ_WRITE,

		unknown = 0,
	};
};
typedef buffer_map::type buffer_map_t;

} // namespace gl
} // namespace pocket

#endif // __POCKET_BUFFER_BASE_H__
