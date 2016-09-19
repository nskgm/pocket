#ifndef __POCKET_GL_DRAW_INDIRECT_BUFFER_H__
#define __POCKET_GL_DRAW_INDIRECT_BUFFER_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "buffer.h"
#include "commands.h"

namespace pocket
{
namespace gl
{

// forward
class draw_indirect_buffer;

class draw_indirect_buffer
{
public:
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef binder<draw_indirect_buffer> binder_type;

	enum command_type
	{
		arrays,
		elements
	};

	enum identifier_t
	{
		identifier = GL_BUFFER
	};

private:
	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

	buffer _buffer;

public:
	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	draw_indirect_buffer() :
		_buffer()
	{}
	explicit draw_indirect_buffer(const commands::draw_arrays& arys, buffer_usage_t usg = buffer_usage::immutable_read) :
		_buffer()
	{
		initialize(arys, usg);
	}
	explicit draw_indirect_buffer(const commands::draw_elements& elems, buffer_usage_t usg = buffer_usage::immutable_read) :
		_buffer()
	{
		initialize(elems, usg);
	}
	explicit draw_indirect_buffer(command_type type, GLuint count, buffer_usage_t usg = buffer_usage::immutable_read) :
		_buffer()
	{
		initialize(type, count, usg);
	}
	explicit draw_indirect_buffer(const GLuint(&cmds)[4], buffer_usage_t usg = buffer_usage::immutable_read) :
		_buffer()
	{
		initialize(cmds, usg);
	}
	explicit draw_indirect_buffer(const GLuint(&cmds)[5], buffer_usage_t usg = buffer_usage::immutable_read) :
		_buffer()
	{
		initialize(cmds, usg);
	}
	draw_indirect_buffer(const draw_indirect_buffer& b) :
		_buffer(b._buffer)
	{}
#ifdef POCKET_USE_CXX11
	draw_indirect_buffer(draw_indirect_buffer&& v) :
		_buffer(std::move(v._buffer))
	{}
#endif // POCKET_USE_CXX11
	~draw_indirect_buffer()
	{
		finalize();
	}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 初期化
	bool initialize(const commands::draw_arrays& arys, buffer_usage_t usg = buffer_usage::immutable_read)
	{
		return _buffer.initialize(buffer_type::draw_indirect, usg, sizeof(commands::draw_arrays), static_cast<const void*>(&arys));
	}
	bool initialize(const commands::draw_elements& elems, buffer_usage_t usg = buffer_usage::immutable_read)
	{
		return _buffer.initialize(buffer_type::draw_indirect, usg, sizeof(commands::draw_elements), static_cast<const void*>(&elems));
	}
	bool initialize(command_type type, GLuint count, buffer_usage_t usg = buffer_usage::immutable_read)
	{
		if (type == arrays)
		{
			return initialize(commands::draw_arrays(count), usg);
		}
		return initialize(commands::draw_elements(count), usg);
	}
	bool initialize(const GLuint(&cmds)[4], buffer_usage_t usg = buffer_usage::immutable_read)
	{
		return _buffer.initialize(buffer_type::draw_indirect, usg, sizeof(commands::draw_arrays), static_cast<const void*>(&cmds[0]));
	}
	bool initialize(const GLuint(&cmds)[5], buffer_usage_t usg = buffer_usage::immutable_read)
	{
		return _buffer.initialize(buffer_type::draw_indirect, usg, sizeof(commands::draw_elements), static_cast<const void*>(&cmds[0]));
	}

	// 終了処理
	void finalize()
	{
		_buffer.finalize();
	}

	// エラー状態クリア
	void clear()
	{
		_buffer.clear();
	}

	// バインド
	void bind() const
	{
		_buffer.bind();
	}

	// バインド解除
	void unbind() const
	{
		_buffer.unbind();
	}

	// 現在のバッファーがバインドされているか
	bool binding() const
	{
		return _buffer.binding();
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}

	// バッファを展開して先頭アドレスを取得
	void* map(buffer_map_t type) const
	{
		return _buffer.map(type);
	}
	template <typename T>
	T* map(buffer_map_t type) const
	{
		return _buffer.map<T>(type);
	}
	void* map_binding(buffer_map_t type) const
	{
		return _buffer.map_binding(type);
	}
	template <typename T>
	T* map_binding(buffer_map_t type) const
	{
		return _buffer.map_binding<T>(type);
	}

	// 展開してアドレスを取得できていたら渡された関数を実行
	template <typename F>
	bool map(buffer_map_t type, F func) const
	{
		binder_type lock(*this);
		return map_binding(type, func);
	}
	template <typename T, typename F>
	bool map(buffer_map_t type, F func) const
	{
		binder_type lock(*this);
		return map_binding<T>(type, func);
	}
	template <typename F>
	bool map_binding(buffer_map_t type, F func) const
	{
		void* address = map_binding(type);
		if (address == NULL)
		{
			return false;
		}
		func(address);
		unmap_binding();
		return true;
	}
	template <typename T, typename F>
	bool map_binding(buffer_map_t type, F func) const
	{
		T* address = map_binding<T>(type);
		if (address == NULL)
		{
			return false;
		}
		func(address);
		unmap_binding();
		return true;
	}

	// バッファの展開を解除
	void unmap() const
	{
		_buffer.unmap();
	}
	void unmap_binding() const
	{
		_buffer.unmap_binding();
	}

	// 展開されている状態か
	bool mapping() const
	{
		return _buffer.mapping();
	}

	// 書き込み可能か
	bool writable() const
	{
		return _buffer.writable();
	}
	bool writable_binding() const
	{
		return _buffer.writable_binding();
	}

	// ストリーミング可能か
	bool streamable() const
	{
		return _buffer.streamable();
	}
	bool streamable_binding() const
	{
		return _buffer.streamable_binding();
	}

	// バッファサイズ
	int size() const
	{
		return _buffer.size();
	}
	int size_binding() const
	{
		return _buffer.size_binding();
	}

	// 型の数
	int count() const
	{
		binder_type lock(*this);
		return count_binding();
	}
	int count_binding() const
	{
		const GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::read);
		int c = *pointer; // どちらも先頭に描画頂点数が入っている
		_buffer.unmap_binding();
		return c;
	}
	void count(GLuint n) const
	{
		binder_type lock(*this);
		count_binding(n);
	}
	void count_binding(GLuint n) const
	{
		GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::write);
		*pointer = n;
		_buffer.unmap_binding();
	}

	// 描画インスタンス数
	int instance_count() const
	{
		binder_type lock(*this);
		return instance_count_binding();
	}
	int instance_count_binding() const
	{
		const GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::read);
		int c = *(pointer + 1);
		_buffer.unmap_binding();
		return c;
	}
	void instance_count(GLuint n) const
	{
		binder_type lock(*this);
		instance_count_binding(n);
	}
	void instance_count_binding(GLuint n) const
	{
		GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::write);
		*(pointer + 1) = n;
		_buffer.unmap_binding();
	}

	// 描画開始地点
	int first() const
	{
		binder_type lock(*this);
		return first_binding();
	}
	int first_binding() const
	{
		const GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::read);
		int c = *(pointer + 2);
		_buffer.unmap_binding();
		return c;
	}
	void first(GLuint n) const
	{
		binder_type lock(*this);
		first_binding(n);
	}
	void first_binding(GLuint n) const
	{
		GLuint* pointer = _buffer.map_binding<GLuint>(buffer_map::write);
		*(pointer + 2) = n;
		_buffer.unmap_binding();
	}

	// 設定した時の扱い法
	buffer_usage_t usage() const
	{
		return _buffer.usage();
	}
	buffer_usage_t usage_binding() const
	{
		return _buffer.usage_binding();
	}

	// エラー文
	std::string error() const
	{
		return _buffer.error();
	}

	// エラーのステータス確認
	bool error_status(error_bitfield bit) const
	{
		return _buffer.error_status(bit);
	}

	// 有効な状態か
	bool valid() const
	{
		return _buffer.valid();
	}

	// バッファ種類
	buffer_type_t kind() const
	{
		return buffer_type::draw_indirect;
	}

	// バッファ種類の比較
	bool kind_of(buffer_type_t type) const
	{
		return type == buffer_type::draw_indirect;
	}

	// ハンドルの取得
	GLuint& get()
	{
		return _buffer.get();
	}
	const GLuint& get() const
	{
		return _buffer.get();
	}

	//------------------------------------------------------------------------------------------
	// Operators
	//------------------------------------------------------------------------------------------

	POCKET_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const draw_indirect_buffer& b) const
	{
		return _buffer == b._buffer;
	}
	bool operator != (const draw_indirect_buffer& b) const
	{
		return !(*this == b);
	}

	draw_indirect_buffer& operator = (const draw_indirect_buffer& b)
	{
		_buffer = b._buffer;
		return *this;
	}
#ifdef POCKET_USE_CXX11
	draw_indirect_buffer& operator = (draw_indirect_buffer&& b)
	{
		_buffer = std::move(b._buffer);
		return *this;
	}

	draw_indirect_buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // POCKET_USE_CXX11
};

inline
draw_indirect_buffer make_draw_indirect_buffer(const commands::draw_arrays& arys, buffer_usage_t usg = buffer_usage::immutable_read)
{
	return draw_indirect_buffer(arys, usg);
}
inline
draw_indirect_buffer make_draw_indirect_buffer(const commands::draw_elements& elems, buffer_usage_t usg = buffer_usage::immutable_read)
{
	return draw_indirect_buffer(elems, usg);
}
inline
draw_indirect_buffer make_draw_indirect_buffer(draw_indirect_buffer::command_type type, GLuint count, buffer_usage_t usg = buffer_usage::immutable_read)
{
	return draw_indirect_buffer(type, count, usg);
}
inline
draw_indirect_buffer make_draw_indirect_buffer(const GLuint(&cmds)[4], buffer_usage_t usg = buffer_usage::immutable_read)
{
	return draw_indirect_buffer(cmds, usg);
}
inline
draw_indirect_buffer make_draw_indirect_buffer(const GLuint(&cmds)[5], buffer_usage_t usg = buffer_usage::immutable_read)
{
	return draw_indirect_buffer(cmds, usg);
}

inline
draw_indirect_buffer& make_draw_indirect_buffer(draw_indirect_buffer& b, const commands::draw_arrays& arys, buffer_usage_t usg = buffer_usage::immutable_read)
{
	b.initialize(arys, usg);
	return b;
}
inline
draw_indirect_buffer& make_draw_indirect_buffer(draw_indirect_buffer& b, const commands::draw_elements& elems, buffer_usage_t usg = buffer_usage::immutable_read)
{
	b.initialize(elems, usg);
	return b;
}
inline
draw_indirect_buffer& make_draw_indirect_buffer(draw_indirect_buffer& b, draw_indirect_buffer::command_type type, GLuint count, buffer_usage_t usg = buffer_usage::immutable_read)
{
	b.initialize(type, count, usg);
	return b;
}
inline
draw_indirect_buffer& make_draw_indirect_buffer(draw_indirect_buffer& b, const GLuint(&cmds)[4], buffer_usage_t usg = buffer_usage::immutable_read)
{
	b.initialize(cmds, usg);
	return b;
}
inline
draw_indirect_buffer& make_draw_indirect_buffer(draw_indirect_buffer& b, const GLuint(&cmds)[5], buffer_usage_t usg = buffer_usage::immutable_read)
{
	b.initialize(cmds, usg);
	return b;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const draw_indirect_buffer& v)
{
	os << io::widen("draw_indirect_buffer: {") << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl;
	if (v.binding())
	{
		const bool elem = v.size_binding() == sizeof(commands::draw_elements);
		const char* type = elem ? "draw_elements" : "draw_arrays";
		os << io::tab << io::widen("type: ") << io::widen(type) << io::widen(" {") << std::endl;
		if (elem)
		{
			const commands::draw_elements* pointer = v.map_binding<commands::draw_elements>(buffer_map::read);
			os << io::tab2 << io::widen("elements: ") << pointer->elements << std::endl <<
				io::tab2 << io::widen("instance_count: ") << pointer->instance_count << std::endl <<
				io::tab2 << io::widen("first: ") << pointer->first << std::endl <<
				io::tab2 << io::widen("vertex_base: ") << pointer->vertex_base << std::endl <<
				io::tab2 << io::widen("instance_count_base: ") << pointer->instance_count_base << std::endl;
		}
		else
		{
			const commands::draw_arrays* pointer = v.map_binding<commands::draw_arrays>(buffer_map::read);
			os << io::tab2 << io::widen("arrays: ") << pointer->arrays << std::endl <<
				io::tab2 << io::widen("instance_count: ") << pointer->instance_count << std::endl <<
				io::tab2 << io::widen("first: ") << pointer->first << std::endl <<
				io::tab2 << io::widen("instance_count_base: ") << pointer->instance_count_base << std::endl;
		}
		v.unmap_binding();
		os << io::tab << io::braces_right << std::endl <<
			io::tab << io::widen("writable: ") << io::widen(v.writable_binding()) << std::endl;
	}
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

#endif // __POCKET_GL_DRAW_INDIRECT_BUFFER_H__
