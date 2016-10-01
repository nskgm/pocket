#ifndef __POCKET_COMMON_TYPE_H__
#define __POCKET_COMMON_TYPE_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"

namespace pocket
{
namespace gl
{

//---------------------------------------------------------------------------------------
// GL側タイプ値
//---------------------------------------------------------------------------------------
template <typename T>
struct gl_type
{
	static const GLenum value = POCKET_GL_UNINITIALIZED_VALUE;
};
template <>
struct gl_type<char>
{
	static const GLenum value = GL_BYTE;
};
template <>
struct gl_type<unsigned char>
{
	static const GLenum value = GL_UNSIGNED_BYTE;
};
template <>
struct gl_type<short>
{
	static const GLenum value = GL_SHORT;
};
template <>
struct gl_type<unsigned short>
{
	static const GLenum value = GL_UNSIGNED_SHORT;
};
template <>
struct gl_type<int>
{
	static const GLenum value = GL_INT;
};
template <>
struct gl_type<unsigned int>
{
	static const GLenum value = GL_UNSIGNED_INT;
};

template <>
struct gl_type<float>
{
	static const GLenum value = GL_FLOAT;
};
template <>
struct gl_type<double>
{
	static const GLenum value = GL_DOUBLE;
};

template <typename T, template <typename> class V>
struct gl_type<V<T> >
{
	static const GLenum value = gl_type<T>::value;
};

//---------------------------------------------------------------------------------------
// GL側真偽値
//---------------------------------------------------------------------------------------
template <bool COND>
struct gl_bool
{
	static const GLboolean value = COND ? GL_TRUE : GL_FALSE;
};

//---------------------------------------------------------------------------------------
// GL側比較値
//---------------------------------------------------------------------------------------
struct compare_func_type
{
	enum type
	{
		less_equal = GL_LEQUAL,
		greater_equal = GL_GEQUAL,
		less = GL_LESS,
		greater = GL_GREATER,
		equal = GL_EQUAL,
		not_equal = GL_NOTEQUAL,
		always = GL_ALWAYS,
		never = GL_NEVER,

		unknown = 0,
	};
};
typedef compare_func_type::type compare_func_type_t;

//---------------------------------------------------------------------------------------
// GL側ラッピング値
//---------------------------------------------------------------------------------------
struct wrap_type
{
	enum type
	{
		clamp_to_edge = GL_CLAMP_TO_EDGE,	// 色が引き伸ばされる
		repeat = GL_REPEAT, // 超えたら0.0から開始される
		mirror = GL_MIRRORED_REPEAT, // 超えたら反転する形で1.0~0.0

		clamp = clamp_to_edge,

		unknown = 0,
	};
};
typedef wrap_type::type wrap_type_t;

//---------------------------------------------------------------------------------------
// GL側フィルタリング値
//---------------------------------------------------------------------------------------
struct filter_type
{
	enum type
	{
		nearest = GL_NEAREST,
		linear = GL_LINEAR,

		nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
		linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
		nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
		linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,

		unknown = 0,
	};
};
typedef filter_type::type filter_type_t;

//---------------------------------------------------------------------------------------
// GL側バッファバインディング種類値
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// GL側バッファ種類値
//---------------------------------------------------------------------------------------
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

};
typedef buffer_type::type buffer_type_t;

// バインド中の型へ変換
inline
buffer_binding_type_t to_binding_type(buffer_type_t a)
{
#define __POCKET_CASE_TO_BINDING(c) case buffer_type:: c: return buffer_binding_type:: c

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
		__POCKET_CASE_TO_BINDING(uniform);

		case buffer_type::unknown:
		default:
			return buffer_binding_type::unknown;
	}

#undef __POCKET_CASE_TO_BINDING
}

// バッファの型へ変換
inline
buffer_type_t to_buffer_type(buffer_binding_type_t a)
{
#define __POCKET_CASE_TO_BINDING(c) case buffer_binding_type:: c: return buffer_type:: c

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
		__POCKET_CASE_TO_BINDING(uniform);

		case buffer_binding_type::unknown:
		default:
			return buffer_type::unknown;
	}

#undef __POCKET_CASE_TO_BINDING
}

//---------------------------------------------------------------------------------------
// GL側バッファ扱い値
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// GL側バッファ展開値
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// GL側シェーダー種類値
//---------------------------------------------------------------------------------------
struct shader_type
{
	enum type
	{
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER,
		tess_control = GL_TESS_CONTROL_SHADER,
		tess_evaluate = GL_TESS_EVALUATION_SHADER,
		compute = GL_COMPUTE_SHADER,

		unknown = 0,
	};
};
typedef shader_type::type shader_type_t;

//---------------------------------------------------------------------------------------
// GL側描画法
//---------------------------------------------------------------------------------------
struct draw_type
{
	enum type
	{
		points = GL_POINTS,

		lines = GL_LINES,
		line_strip = GL_LINE_STRIP,
		line_loop = GL_LINE_LOOP,

		triangles = GL_TRIANGLES,
		triangle_strip = GL_TRIANGLE_STRIP,
		triangle_fan = GL_TRIANGLE_FAN,

		lines_adjacency = GL_LINES_ADJACENCY,
		line_strip_adjacency = GL_LINE_STRIP_ADJACENCY,
		triangles_adjacency = GL_TRIANGLES_ADJACENCY,
		triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY,

		patch = GL_PATCHES, // geometry, tess_control, tess_evaluate

		unknown = 0,
	};
};
typedef draw_type::type draw_type_t;

} // namespace gl
} // namespace pocket

#endif // __POCKET_COMMON_TYPE_H__
