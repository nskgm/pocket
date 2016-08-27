#ifndef __POCKET_GL_COMMANDS_H__
#define __POCKET_GL_COMMANDS_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"

namespace pocket
{
namespace gl
{
// buffer_type::draw_indirect
// buffer_type::dispatch_compute
namespace commands
{

// glDrawArraysIndirect用
struct draw_arrays
{
	// 頂点数
	GLuint arrays;
	// 描画インスタンス数
	GLuint instance;
	// 開始インデックス
	GLuint index;
	// インスタンス基点値（0）
	GLuint instance_base;

	draw_arrays() :
		arrays(0), instance(1),
		index(0), instance_base(0)
	{}
	draw_arrays(GLuint arrays, GLuint instance = 1, GLuint index = 0) :
		arrays(arrays), instance(instance),
		index(index), instance_base(0)
	{}
};
// glDrawElementsIndirect用
struct draw_elements
{
	// 要素数
	GLuint elements;
	// 描画インスタンス数
	GLuint instance;
	// 開始インデックス
	GLuint index;
	// 頂点基点値
	GLuint vertex_base;
	// インスタンス基点値（0）
	GLuint instance_base;

	draw_elements() :
		elements(0), instance(1),
		index(0), vertex_base(0),
		instance_base(0)
	{}
	draw_elements(GLuint elements, GLuint instance = 1, GLuint index = 0, GLuint vertex_base = 0) :
		elements(elements), instance(instance),
		index(index), vertex_base(vertex_base),
		instance_base(0)
	{}
};
// glDispatchComputeIndirect用
struct dispatch_compute
{
	// Xグループ
	GLuint x;
	// Yグループ
	GLuint y;
	// Zグループ
	GLuint z;

	dispatch_compute() :
		x(0), y(0), z(0)
	{}
	dispatch_compute(GLuint x, GLuint y, GLuint z) :
		x(x), y(y), z(z)
	{}
};

} // namespace commands
} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_COMMANDS_H__
