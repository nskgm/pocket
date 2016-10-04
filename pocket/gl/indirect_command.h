#ifndef __POCKET_GL_COMMANDS_H__
#define __POCKET_GL_COMMANDS_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"

namespace pocket
{
namespace gl
{
// buffer_type::draw_indirect
// buffer_type::dispatch_compute

struct command_type
{
	enum type
	{
		arrays,
		elements,
		dispatch
	};
};
typedef command_type::type command_type_t;

// glDrawArraysIndirect用
struct draw_arrays_cmd
{
	// 頂点数
	GLuint arrays;
	// 描画インスタンス数
	GLuint instance_count;
	// 開始頂点
	GLuint first;
	// インスタンス基点値（0）
	GLuint instance_count_base;

	draw_arrays_cmd() :
		arrays(0),
		instance_count(1),
		first(0),
		instance_count_base(0)
	{}
	draw_arrays_cmd(GLuint arrays, GLuint instance_count = 1, GLuint first = 0) :
		arrays(arrays),
		instance_count(instance_count),
		first(first),
		instance_count_base(0)
	{}
};
// glDrawElementsIndirect用
struct draw_elements_cmd
{
	// 要素数
	GLuint elements;
	// 描画インスタンス数
	GLuint instance_count;
	// 開始インデックス
	GLuint first;
	// 頂点基点値
	GLuint vertex_base;
	// インスタンス基点値（0）
	GLuint instance_count_base;

	draw_elements_cmd() :
		elements(0),
		instance_count(1),
		first(0),
		vertex_base(0),
		instance_count_base(0)
	{}
	draw_elements_cmd(GLuint elements, GLuint instance_count = 1, GLuint first = 0, GLuint vertex_base = 0) :
		elements(elements),
		instance_count(instance_count),
		first(first),
		vertex_base(vertex_base),
		instance_count_base(0)
	{}
};
// glDispatchComputeIndirect用
struct dispatch_compute_cmd
{
	// Xグループ
	GLuint x;
	// Yグループ
	GLuint y;
	// Zグループ
	GLuint z;

	dispatch_compute_cmd() :
		x(0),
		y(0),
		z(0)
	{}
	dispatch_compute_cmd(GLuint x, GLuint y, GLuint z) :
		x(x),
		y(y),
		z(z)
	{}
};

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_COMMANDS_H__
