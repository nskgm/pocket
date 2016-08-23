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
#include <algorithm> // for std::min

namespace pocket
{
namespace gl
{

// forward
class uniform_buffer;

template <typename M>
class binder_map<uniform_buffer, M>
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// none

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	const uniform_buffer* _address;
	M* _data;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	explicit binder_map(const uniform_buffer& a, buffer_base::map_usage_type type);
	~binder_map();

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	M* get() const
	{
		return _data;
	}
	template <typename U>
	U* get() const
	{
		return reinterpret_cast<U*>(_data);
	}

	int size() const;

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	_CXX11_EXPLICIT operator bool () const
	{
		return _data != NULL;
	}
	bool operator ! () const
	{
		return _data == NULL;
	}

	_CXX11_EXPLICIT operator M* () const
	{
		return _data;
	}

	binder_map& operator ++ ()
	{
		++_data;
		return *this;
	}
	binder_map& operator -- ()
	{
		--_data;
		return *this;
	}

	M& operator * () const
	{
		return *_data;
	}

	M* operator -> () const
	{
		return _data;
	}

	M& operator [] (int n) const
	{
		return _data[n];
	}
};

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

	template <typename T>
	struct rebinder_map
	{
		typedef binder_map<uniform_buffer, T> type;
	};

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
	template <typename T>
	explicit uniform_buffer(const program& prog, const char* name, GLuint point, const T& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T>
	explicit uniform_buffer(const program& prog, const std::string& name, GLuint point, const T& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, int N>
	explicit uniform_buffer(const program& prog, const char* name, GLuint point, const T(&a)[N], usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, int N>
	explicit uniform_buffer(const program& prog, const std::string& name, GLuint point, const T(&a)[N], usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	explicit uniform_buffer(const program& prog, const char* name, GLuint point, const ARRAY<T, N>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	explicit uniform_buffer(const program& prog, const std::string& name, GLuint point, const ARRAY<T, N>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	explicit uniform_buffer(const program& prog, const char* name, GLuint point, const VECTOR<T, ALLOC>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	explicit uniform_buffer(const program& prog, const std::string& name, GLuint point, const VECTOR<T, ALLOC>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		initialize(prog, name, point, a, usg);
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
		if (!prog.uniform_block_index(name, _index))
		{
			// バッファのエラーコードを利用
			_buffer._error_bitfield |= error_invalid_index;
			return false;
		}
		// サイズの取得
		_size = prog.uniform_block_size(_index);
		// バッファの作成
		return _buffer.initialize(buffer_base::uniform, usg, _size, data);
	}
	bool initialize(const program& prog, const std::string& name, GLuint point, const void* data, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name.c_str(), point, data, usg);
	}
	template <typename T>
	bool initialize(const program& prog, const char* name, GLuint point, const T& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a), usg);
	}
	template <typename T>
	bool initialize(const program& prog, const std::string& name, GLuint point, const T& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a), usg);
	}
	template <typename T, int N>
	bool initialize(const program& prog, const char* name, GLuint point, const T(&a)[N], usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}
	template <typename T, int N>
	bool initialize(const program& prog, const std::string& name, GLuint point, const T(&a)[N], usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const program& prog, const char* name, GLuint point, const ARRAY<T, N>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const program& prog, const std::string& name, GLuint point, const ARRAY<T, N>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const program& prog, const char* name, GLuint point, const VECTOR<T, ALLOC>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const program& prog, const std::string& name, GLuint point, const VECTOR<T, ALLOC>& a, usage_type usg = buffer_base::dynamic_draw)
	{
		return initialize(prog, name, point, static_cast<const void*>(&a[0]), usg);
	}

	// 終了処理
	void finalize()
	{
		_buffer.finalize();
		_index = GL_INVALID_INDEX;
		_binding_point = 0;
		_size = 0;
	}

	// エラー状態のクリア
	void clear()
	{
		_buffer.clear();
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

	// データの展開
	void* map(map_usage_type type) const
	{
		return _buffer.map(type);
	}
	template <typename T>
	T* map(map_usage_type type) const
	{
		return _buffer.map<T>(type);
	}

	// 展開の解除
	void unmap() const
	{
		_buffer.unmap();
	}

	// バッファの展開
	template <typename T>
	typename rebinder_map<T>::type make_binder_map(map_usage_type type) const
	{
		return typename rebinder_map<T>::type(*this, type);
	}
	template <typename T, map_usage_type U>
	typename rebinder_map<T>::type make_binder_map() const
	{
		return typename rebinder_map<T>::type(*this, U);
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
	bool operator < (const uniform_buffer& u) const
	{
		return _binding_point < u._binding_point;
	}
	bool operator <= (const uniform_buffer& u) const
	{
		return _binding_point <= u._binding_point;
	}
	bool operator > (const uniform_buffer& u) const
	{
		return _binding_point > u._binding_point;
	}
	bool operator >= (const uniform_buffer& u) const
	{
		return _binding_point >= u._binding_point;
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

template <typename M> inline
binder_map<uniform_buffer, M>::binder_map(const uniform_buffer& a, buffer_base::map_usage_type type) :
	_address(&a), _data(a.map<M>(type))
{}
template <typename M> inline
binder_map<uniform_buffer, M>::~binder_map()
{
	_address->unmap();
}

// programからuniform_bufferの作成

inline
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const void* a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T> inline
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const T& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, int N> inline
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const T(&a)[N], buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const ARRAY<T, N>& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
uniform_buffer program::make_uniform_buffer(const char* name, GLuint point, const VECTOR<T, ALLOC>& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}

inline
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const void* a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T> inline
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const T& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, int N> inline
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const T(&a)[N], buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const ARRAY<T, N>& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
uniform_buffer program::make_uniform_buffer(const std::string& name, GLuint point, const VECTOR<T, ALLOC>& a, buffer_base::usage_type usg) const
{
	return uniform_buffer(*this, name, point, a, usg);
}

inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const void* a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const T& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, int N> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const T(&a)[N], buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const ARRAY<T, N>& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const char* name, GLuint point, const VECTOR<T, ALLOC>& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}

inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const void* a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const T& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, int N> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const T(&a)[N], buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const ARRAY<T, N>& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
uniform_buffer& program::make_uniform_buffer(uniform_buffer& b, const std::string& name, GLuint point, const VECTOR<T, ALLOC>& a, buffer_base::usage_type usg) const
{
	b.initialize(*this, name, point, a, usg);
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
