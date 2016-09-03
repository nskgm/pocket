#ifndef __POCKET_GL_VERTEX_BUFFER_H__
#define __POCKET_GL_VERTEX_BUFFER_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "buffer.h"

namespace pocket
{
namespace gl
{

// forward
template <typename> class vertex_buffer;

template <typename T>
class binder<vertex_buffer<T> >
{
private:
	const vertex_buffer<T>* _address;

public:
	explicit binder(const vertex_buffer<T>& a);
	~binder();
	int size() const;
	int count() const;
	buffer_usage_t usage() const;
	T* map(buffer_map_t) const;
	template <typename F> bool map(buffer_map_t, F) const;
	void unmap() const;

	binder_map<vertex_buffer<T>, T> make_binder_map(buffer_map_t) const;
	template <buffer_map_t U> binder_map<vertex_buffer<T>, T> make_binder_map() const;

	bool binding() const
	{
		return true;
	}

	const vertex_buffer<T>* operator -> () const
	{
		return _address;
	}
	const vertex_buffer<T>& operator * () const
	{
		return *_address;
	}

	operator const vertex_buffer<T>& () const
	{
		return *_address;
	}
	operator const vertex_buffer<T>* () const
	{
		return _address;
	}
};

template <typename T>
class binder_map<vertex_buffer<T>, T>
{
public:
	typedef detail::binder_map_iterator<T> iterator;
	typedef detail::binder_map_iterator<const T> const_iterator;
	enum state_type
	{
		none_state = 0,
		binding_state = 1,
	};

private:
	const vertex_buffer<T>* _address;
	T* _data;
	state_type _state;

public:
	explicit binder_map(const vertex_buffer<T>&, buffer_map_t);
	explicit binder_map(const binder<vertex_buffer<T> >&, buffer_map_t);
	~binder_map();

	T* get() const
	{
		return _data;
	}
	template <typename U>
	U* get() const
	{
		return reinterpret_cast<U*>(_data);
	}

	iterator begin()
	{
		return iterator(_data);
	}
	const_iterator begin() const
	{
		return const_iterator(_data);
	}
	const_iterator cbegin() const
	{
		return const_iterator(_data);
	}

	iterator end();
	const_iterator end() const;
	const_iterator cend() const;

	int count() const;

	_CXX11_EXPLICIT operator bool () const
	{
		return _data != NULL;
	}
	bool operator ! () const
	{
		return _data == NULL;
	}

	_CXX11_EXPLICIT operator T* () const
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

	T& operator * () const
	{
		return *_data;
	}

	T* operator -> () const
	{
		return _data;
	}

	T& operator [] (int n) const
	{
		return _data[n];
	}
};

template <typename T>
class vertex_buffer
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef T vertex_type;

	typedef binder<vertex_buffer<T> > binder_type;
	typedef binder_map<buffer, vertex_type> binder_map_type;

	enum identifier_t
	{
		identifier = GL_BUFFER
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	buffer _buffer;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	vertex_buffer() :
		_buffer()
	{}
	explicit vertex_buffer(const vertex_type* vertices, int count, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_buffer()
	{
		initialize(vertices, count, usg);
	}
	template <int N>
	explicit vertex_buffer(const vertex_type(&vertices)[N], buffer_usage_t usg = buffer_usage::immutable_draw) :
		_buffer()
	{
		initialize(vertices, usg);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit vertex_buffer(const ARRAY<vertex_type, N>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_buffer()
	{
		initialize(vertices, usg);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit vertex_buffer(const VECTOR<vertex_type, ALLOC>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_buffer()
	{
		initialize(vertices, usg);
	}
	explicit vertex_buffer(int count) :
		_buffer()
	{
		initialize(count);
	}
	~vertex_buffer()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(const vertex_type* vertices, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		return _buffer.initialize(buffer_type::array, usg, sizeof(vertex_type)*count, static_cast<const void*>(vertices));
	}
	template <int N>
	bool initialize(const vertex_type(&vertices)[N], buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		return initialize(&vertices[0], N, usg);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const ARRAY<vertex_type, N>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		return initialize(&vertices[0], N, usg);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const VECTOR<vertex_type, ALLOC>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		return initialize(&vertices[0], vertices.size(), usg);
	}
	bool initialize(int count)
	{
		// サイズのみ確保
		return _buffer.initialize(buffer_type::array, buffer_usage::dynamic_draw, sizeof(vertex_type)*count, NULL);
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
	void bind(buffer_type_t type) const
	{
		_buffer.bind(type);
	}
	void bind_base(GLuint point) const
	{
		_buffer.bind_base(point);
	}
	void bind_base(buffer_type_t type) const
	{
		_buffer.bind_base(type);
	}
	void bind_base(buffer_type_t type, GLuint point) const
	{
		_buffer.bind_base(type, point);
	}
	void bind_vertex(GLuint index, GLintptr offset, GLsizei stride) const
	{
		_buffer.bind_vertex(index, offset, stride);
	}
	template <typename U>
	void bind_vertex(GLuint index) const
	{
		_buffer.bind_vertex<U>(index);
	}

	// バインド解除
	void unbind() const
	{
		_buffer.unbind();
	}
	void unbind(buffer_type_t type) const
	{
		_buffer.unbind(type);
	}
	void unbind_base(GLuint point) const
	{
		_buffer.unbind_base(point);
	}
	void unbind_base(buffer_type_t type) const
	{
		_buffer.unbind_base(type);
	}
	void unbind_base(buffer_type_t type, GLuint point) const
	{
		_buffer.unbind_base(type, point);
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
	vertex_type* map(buffer_map_t type) const
	{
		return _buffer.map<vertex_type>(type);
	}
	vertex_type* map_binding(buffer_map_t type) const
	{
		return _buffer.map_binding<vertex_type>(type);
	}

	// 展開してアドレスを取得できていたら渡された関数を実行
	template <typename F>
	bool map(buffer_map_t type, F func) const
	{
		binder_type lock(*this);
		return map_binding(type, func);
	}
	template <typename F>
	bool map_binding(buffer_map_t type, F func) const
	{
		vertex_type* address = map_binding(type);
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

	// 展開を管理するオブジェクト生成
	binder_map_type make_binder_map(buffer_map_t type) const
	{
		return binder_map_type(*this, type);
	}

	// ストリーミング
	void stream(GLuint point) const
	{
		_buffer.bind_base(buffer_type::transform_feedback, point);
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
		return _buffer.count<vertex_type>();
	}
	int count_binding() const
	{
		return _buffer.count_binding<vertex_type>();
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
		return buffer_type::array;
	}

	// バッファ種類の比較
	bool kind_of(buffer_type_t type) const
	{
		return type == buffer_type::array;
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

	_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const vertex_buffer& b) const
	{
		return _buffer == b._buffer;
	}
	bool operator != (const vertex_buffer& b) const
	{
		return !(*this == b);
	}

	vertex_buffer& operator = (const vertex_buffer& b)
	{
		_buffer = b._buffer;
		return *this;
	}
#ifdef _USE_CXX11
	vertex_buffer& operator = (vertex_buffer&& b)
	{
		_buffer = std::move(b._buffer);
		return *this;
	}

	vertex_buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};


template <typename T> inline
binder<vertex_buffer<T> >::binder(const vertex_buffer<T>& a) :
	_address(&a)
{
	a.bind();
}
template <typename T> inline
binder<vertex_buffer<T> >::~binder()
{
	_address->unbind();
}
template <typename T> inline
int binder<vertex_buffer<T> >::size() const
{
	return _address->size_binding();
}
template <typename T> inline
int binder<vertex_buffer<T> >::count() const
{
	return _address->count_binding();
}
template <typename T> inline
buffer_usage_t binder<vertex_buffer<T> >::usage() const
{
	return _address->usage_binding();
}
template <typename T> inline
T* binder<vertex_buffer<T> >::map(buffer_map_t type) const
{
	return _address->map_binding(type);
}
template <typename T>
template <typename F> inline
bool binder<vertex_buffer<T> >::map(buffer_map_t type, F func) const
{
	return _address->map_binding(type, func);
}
template <typename T> inline
void binder<vertex_buffer<T> >::unmap() const
{
	_address->unmap_binding();
}
template <typename T> inline
binder_map<vertex_buffer<T>, T> binder<vertex_buffer<T> >::make_binder_map(buffer_map_t type) const
{
	return binder_map<vertex_buffer<T>, T>(*this, type);
}
template <typename T>
template <buffer_map_t U> inline
binder_map<vertex_buffer<T>, T> binder<vertex_buffer<T> >::make_binder_map() const
{
	return binder_map<vertex_buffer<T>, T>(*this, U);
}

template <typename T> inline
binder_map<vertex_buffer<T>, T>::binder_map(const vertex_buffer<T>& a, buffer_map_t type) :
	_address(&a)
{
	if (a.binding())
	{
		_data = a.map_binding(type);
		_state = binding_state;
	}
	else
	{
		_data = a.map(type);
		_state = none_state;
	}
}
template <typename T> inline
binder_map<vertex_buffer<T>, T>::binder_map(const binder<vertex_buffer<T> >& a, buffer_map_t type) :
	_address(a), _data(a.map(type)), _state(binding_state)
{}
template <typename T> inline
binder_map<vertex_buffer<T>, T>::~binder_map()
{
	_address->unmap_binding();
	if (_state != binding_state)
	{
		_address->unbind();
	}
}
template <typename T> inline
typename binder_map<vertex_buffer<T>, T>::iterator binder_map<vertex_buffer<T>, T>::end()
{
	return iterator(_data + _address->count());
}
template <typename T> inline
typename binder_map<vertex_buffer<T>, T>::const_iterator binder_map<vertex_buffer<T>, T>::end() const
{
	return const_iterator(_data + _address->count());
}
template <typename T> inline
typename binder_map<vertex_buffer<T>, T>::const_iterator binder_map<vertex_buffer<T>, T>::cend() const
{
	return const_iterator(_data + _address->count());
}

template <typename T> inline
vertex_buffer<T> make_vertex_buffer(const T* vertices, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return vertex_buffer<T>(vertices, count, usg);
}
template <typename T, int N> inline
vertex_buffer<T> make_vertex_buffer(const T(&vertices)[N], buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return vertex_buffer<T>(vertices, usg);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
vertex_buffer<T> make_vertex_buffer(const ARRAY<T, N>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return vertex_buffer<T>(vertices, usg);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_buffer<T> make_vertex_buffer(const VECTOR<T, ALLOC>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return vertex_buffer<T>(vertices, usg);
}
template <typename T> inline
vertex_buffer<T> make_vertex_buffer(int count)
{
	return vertex_buffer<T>(count);
}

template <typename T> inline
vertex_buffer<T>& make_vertex_buffer(vertex_buffer<T>& b, const T* vertices, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, count, usg);
	return b;
}
template <typename T, int N> inline
vertex_buffer<T>& make_vertex_buffer(vertex_buffer<T>& b, const T(&vertices)[N], buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, usg);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
vertex_buffer<T>& make_vertex_buffer(vertex_buffer<T>& b, const ARRAY<T, N>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, usg);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_buffer<T>& make_vertex_buffer(vertex_buffer<T>& b, const VECTOR<T, ALLOC>& vertices, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, usg);
	return b;
}
template <typename T> inline
vertex_buffer<T>& make_vertex_buffer(vertex_buffer<T>& b, int count)
{
	b.initialize(count);
	return b;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vertex_buffer<T>& v)
{
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_VERTEX_BUFFER_H__
