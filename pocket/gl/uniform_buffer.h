#ifndef __POCKET_GL_UNIFORM_BUFFER_H__
#define __POCKET_GL_UNIFORM_BUFFER_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "buffer.h"
#include "program.h"
#include <cstring> // for std::memcpy
#include <algorithm>

namespace pocket
{
namespace gl
{

// forward
class uniform_buffer;

class uniform_buffer
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef buffer_base::buffer_type buffer_type;
	typedef buffer_base::usage_type usage_type;
	typedef buffer_base::map_usage_type map_usage_type;

	typedef binder<uniform_buffer> binder_type;
	typedef buffer::binder_type buffer_binder_type;

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	buffer _buffer;
	GLuint _index;
	GLuint _binding_point;
	int _size;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	uniform_buffer() :
		_buffer(),
		_index(GL_INVALID_INDEX),
		_binding_point(0),
		_size(0)
	{}
	explicit uniform_buffer(const program& prog, const char* name, GLuint point, const void* data, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, data, usg);
	}
	explicit uniform_buffer(const program& prog, const std::string& name, GLuint point, const void* data, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, data, usg);
	}
	uniform_buffer(const uniform_buffer& u) :
		_buffer(u._buffer),
		_index(u._index),
		_binding_point(u._binding_point),
		_size(u._size)
	{}
#ifdef _USE_CXX11
	uniform_buffer(uniform_buffer&& u) :
		_buffer(std::move(u._buffer)),
		_index(std::move(u._index)),
		_binding_point(std::move(u._binding_point)),
		_size(std::move(u._size))
	{
		u._index = GL_INVALID_INDEX;
		u._binding_point = 0;
		u._size = 0;
	}
#endif // _USE_CXX11
	~uniform_buffer()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(const program& prog, const char* name, GLuint point, const void* data, usage_type usg = buffer_base::dynamic_draw)
	{
		finalize();

		// バインドポイントを保存
		_binding_point = point;
		// プログラム中のユニフォームバッファの位置を取得
		if (!prog.index_uniform_block(name, _index))
		{
			// バッファのエラーコードを利用
			_buffer._error_bitfield |= error_invalid_index;
			return false;
		}
		// サイズの取得
		_size = prog.size_uniform_block(_index);
		// バッファの作成
		return _buffer.initialize(buffer_base::uniform, usg, _size, data);
	}
	bool initialize(const program& prog, const std::string& name, GLuint point, const void* data, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name.c_str(), point, data, usg);
	}

	// 終了処理
	void finalize()
	{
		_buffer.finalize();
		_index = GL_INVALID_INDEX;
		_binding_point = 0;
		_size = 0;
	}

	// グローバルにバインド
	// 値はシェーダー間で共有される
	void bind() const
	{
		_buffer.bind_base(_binding_point);
	}
	void bind(GLuint point)
	{
		if (_binding_point != point)
		{
			unbind();
			_binding_point = point;
			bind();
		}
	}

	// バインドの解除
	void unbind() const
	{
		_buffer.unbind_base(_binding_point);
	}

	// 値の更新
	void uniform(int offset, int size, const void* data) const
	{
		char* write_data = _buffer.map<char>(buffer_base::write);
		size = (std::min)(size, _size);
		std::memcpy(write_data + offset, data, size);
		_buffer.unmap();
	}
	void uniform(const void* data) const
	{
		void* write_data = _buffer.map(buffer_base::write);
		std::memcpy(write_data, data, _size);
		_buffer.unmap();
	}
	template <typename T>
	void uniform(int offset, const T& a) const
	{
		uniform(offset, sizeof(T), static_cast<const void*>(&a));
	}
	template <typename T>
	void uniform(const T& a) const
	{
		uniform(static_cast<const void*>(&a));
	}

	// バインド位置の更新
	void binding_point(GLuint point)
	{
		if (_binding_point != point)
		{
			// 位置が違う場合は現在のバインド位置を無効に
			unbind();
			_binding_point = point;
		}
	}
	// バインド位置の取得
	GLuint binding_point() const
	{
		return _binding_point;
	}

	// サイズの取得
	int size() const
	{
		return _size;
	}

	// インデックスの取得
	GLuint index() const
	{
		return _index;
	}

	// バッファのバインダー作成
	buffer_binder_type make_buffer_binder() const
	{
		return buffer_binder_type(_buffer);
	}

	std::string error() const
	{
		// インデックスが無効（指定された名前が見つからない）
		if (error_status(error_invalid_index))
		{
			return "failed. invalid name.";
		}
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
		if (_index == GL_INVALID_INDEX)
		{
			return false;
		}
		return _buffer.valid();
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

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	_CXX11_EXPLICIT operator GLuint () const
	{
		return _buffer.get();
	}

	_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const uniform_buffer& u) const
	{
		return _buffer == u._buffer &&
			_binding_point == u._binding_point &&
			_index == u._index &&
			_size == u._size;
	}
	bool operator != (const uniform_buffer& u) const
	{
		return !(*this == u);
	}

	uniform_buffer& operator = (const uniform_buffer& u)
	{
		_buffer = u._buffer;
		_index = u._index;
		_binding_point = u._binding_point;
		_size = u._size;
		return *this;
	}
#ifdef _USE_CXX11
	uniform_buffer& operator = (uniform_buffer&& u)
	{
		_buffer = std::move(u._buffer);
		_index = std::move(u._index);
		_binding_point = std::move(u._binding_point);
		_size = std::move(u._size);
		u._index = GL_INVALID_INDEX;
		u._binding_point = 0;
		u._size = 0;
		return *this;
	}

	uniform_buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11

	template <typename T>
	void operator = (const T& a) const
	{
		uniform(a);
	}
	template <typename T>
	void operator << (const T& a) const
	{
		uniform(a);
	}
};

// programからuniform_bufferの作成
inline
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const void* data, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, data, usg);
}
inline
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const void* data, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, data, usg);
}
inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const void* data, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, data, usg);
	return b;
}
inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const void* data, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, data, usg);
	return b;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const uniform_buffer& v)
{
	const bool valid = v.valid();
	const char* valid_str = valid ? "true" : "false";
	os << io::widen("uniform_buffer: ") << io::braces_left << std::endl <<
		io::tab << io::widen("type: uniform") << std::endl <<
		io::tab << io::widen("size: ") << v.size() << std::endl <<
		io::tab << io::widen("binding point: ") << v.binding_point() << std::endl <<
		io::tab << io::widen("index: ") << v.index() << std::endl <<
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
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const uniform_buffer& v)
{
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_UNIFORM_BUFFER_H__
