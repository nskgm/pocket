#ifndef __POCKET_GL_INDEX_BUFFER_H__
#define __POCKET_GL_INDEX_BUFFER_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "buffer.h"
#include <algorithm>

namespace pocket
{
namespace gl
{

// forward
template <typename> class index_buffer;
class draw_indirect_buffer;

template <typename T>
class binder<index_buffer<T> >
{
private:
	const index_buffer<T>* _address;

public:
	explicit binder(const index_buffer<T>& a);
	~binder();
	int size() const;
	int count() const;
	buffer_usage_type_t usage() const;
	T* map(buffer_map_type_t) const;
	template <typename F> bool map(buffer_map_type_t, F) const;
	void unmap() const;

	binder_map<index_buffer<T>, T> make_binder_map(buffer_map_type_t) const;
	template <buffer_map_type_t U> binder_map<index_buffer<T>, T> make_binder_map() const;

	bool binding() const
	{
		return true;
	}

	const index_buffer<T>* operator -> () const
	{
		return _address;
	}
	const index_buffer<T>& operator * () const
	{
		return *_address;
	}

	operator const index_buffer<T>& () const
	{
		return *_address;
	}
	operator const index_buffer<T>* () const
	{
		return _address;
	}
};

template <typename T>
class binder_map<index_buffer<T>, T>
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
	const index_buffer<T>* _address;
	T* _data;
	state_type _state;

public:
	explicit binder_map(const index_buffer<T>&, buffer_map_type_t);
	explicit binder_map(const binder<index_buffer<T> >&, buffer_map_type_t);
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

	POCKET_CXX11_EXPLICIT operator bool () const
	{
		return _data != NULL;
	}
	bool operator ! () const
	{
		return _data == NULL;
	}

	POCKET_CXX11_EXPLICIT operator T* () const
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
class index_buffer
{
public:
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef T index_type;

	typedef binder<index_buffer<T> > binder_type;
	typedef binder_map<buffer, index_type> binder_map_type;

	enum identifier_t
	{
		identifier = GL_BUFFER
	};

private:
	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

	buffer _buffer;
	int _count;

public:
	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	index_buffer() :
		_buffer(),
		_count(0)
	{}
	explicit index_buffer(const index_type* indices, int count, buffer_usage_type_t usg = buffer_usage_type::immutable_draw) :
		_buffer()
	{
		initialize(indices, count, usg);
	}
	template <int N>
	explicit index_buffer(const index_type(&indices)[N], buffer_usage_type_t usg = buffer_usage_type::immutable_draw) :
		_buffer()
	{
		initialize(indices, usg);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit index_buffer(const ARRAY<index_type, N>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw) :
		_buffer()
	{
		initialize(indices, usg);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit index_buffer(const VECTOR<index_type, ALLOC>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw) :
		_buffer()
	{
		initialize(indices, usg);
	}
	explicit index_buffer(int count) :
		_buffer()
	{
		initialize(count);
	}
	index_buffer(const index_buffer& b) :
		_buffer(b._buffer),
		_count(b._count)
	{}
#ifdef POCKET_USE_CXX11
	index_buffer(index_buffer&& v) :
		_buffer(std::move(v._buffer)),
		_count(std::move(v._count))
	{}
#endif // POCKET_USE_CXX11
	~index_buffer()
	{
		finalize();
	}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 初期化
	bool initialize(const index_type* indices, int n, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
	{
		_count = n;
		return _buffer.initialize(buffer_type::element_array, usg, sizeof(index_type)*n, static_cast<const void*>(indices));
	}
	template <int N>
	bool initialize(const index_type(&indices)[N], buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
	{
		return initialize(&indices[0], N, usg);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const ARRAY<index_type, N>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
	{
		return initialize(&indices[0], N, usg);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const VECTOR<index_type, ALLOC>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
	{
		return initialize(&indices[0], indices.size(), usg);
	}
	bool initialize(int n)
	{
		// サイズのみ確保
		_count = n;
		return _buffer.initialize(buffer_type::element_array, buffer_usage_type::dynamic_draw, sizeof(index_type)*n, NULL);
	}

	// 終了処理
	void finalize()
	{
		_buffer.finalize();
		_count = 0;
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

	// 描画
	void draw(draw_type_t type) const
	{
		glDrawElements(type, _count, gl_type<T>::value, NULL);
	}
	void draw(draw_type_t type, GLsizei n) const
	{
		n = std::min(n, _count);
		glDrawElements(type, n, gl_type<T>::value, NULL);
	}
	void draw(draw_type_t type, const draw_indirect_buffer&) const
	{
		glDrawElementsIndirect(type, gl_type<T>::value, NULL);
	}

	// バッファを展開して先頭アドレスを取得
	index_type* map(buffer_map_type_t type) const
	{
		return _buffer.map<index_type>(type);
	}
	index_type* map_binding(buffer_map_type_t type) const
	{
		return _buffer.map_binding<index_type>(type);
	}

	// 展開してアドレスを取得できていたら渡された関数を実行
	template <typename F>
	bool map(buffer_map_type_t type, F func) const
	{
		binder_type lock(*this);
		return map_binding(type, func);
	}
	template <typename F>
	bool map_binding(buffer_map_type_t type, F func) const
	{
		index_type* address = map_binding(type);
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

	// 展開を管理するオブジェクト生成
	binder_map_type make_binder_map(buffer_map_type_t type) const
	{
		return binder_map_type(*this, type);
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
		return _count;
	}

	// 設定した時の扱い法
	buffer_usage_type_t usage() const
	{
		return _buffer.usage();
	}
	buffer_usage_type_t usage_binding() const
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
		return buffer_type::element_array;
	}

	// バッファ種類の比較
	bool kind_of(buffer_type_t type) const
	{
		return type == buffer_type::element_array;
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

	bool operator == (const index_buffer& b) const
	{
		return _buffer == b._buffer;
	}
	bool operator != (const index_buffer& b) const
	{
		return !(*this == b);
	}

	index_buffer& operator = (const index_buffer& b)
	{
		_buffer = b._buffer;
		return *this;
	}
#ifdef POCKET_USE_CXX11
	index_buffer& operator = (index_buffer&& b)
	{
		_buffer = std::move(b._buffer);
		return *this;
	}

	index_buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // POCKET_USE_CXX11
};


template <typename T> inline
binder<index_buffer<T> >::binder(const index_buffer<T>& a) :
	_address(&a)
{
	a.bind();
}
template <typename T> inline
binder<index_buffer<T> >::~binder()
{
	_address->unbind();
}
template <typename T> inline
int binder<index_buffer<T> >::size() const
{
	return _address->size_binding();
}
template <typename T> inline
int binder<index_buffer<T> >::count() const
{
	return _address->count_binding();
}
template <typename T> inline
buffer_usage_type_t binder<index_buffer<T> >::usage() const
{
	return _address->usage_binding();
}
template <typename T> inline
T* binder<index_buffer<T> >::map(buffer_map_type_t type) const
{
	return _address->map_binding(type);
}
template <typename T>
template <typename F> inline
bool binder<index_buffer<T> >::map(buffer_map_type_t type, F func) const
{
	return _address->map_binding(type, func);
}
template <typename T> inline
void binder<index_buffer<T> >::unmap() const
{
	_address->unmap_binding();
}
template <typename T> inline
binder_map<index_buffer<T>, T> binder<index_buffer<T> >::make_binder_map(buffer_map_type_t type) const
{
	return binder_map<index_buffer<T>, T>(*this, type);
}
template <typename T>
template <buffer_map_type_t U> inline
binder_map<index_buffer<T>, T> binder<index_buffer<T> >::make_binder_map() const
{
	return binder_map<index_buffer<T>, T>(*this, U);
}

template <typename T> inline
binder_map<index_buffer<T>, T>::binder_map(const index_buffer<T>& a, buffer_map_type_t type) :
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
binder_map<index_buffer<T>, T>::binder_map(const binder<index_buffer<T> >& a, buffer_map_type_t type) :
	_address(a), _data(a.map(type)), _state(binding_state)
{}
template <typename T> inline
binder_map<index_buffer<T>, T>::~binder_map()
{
	_address->unmap_binding();
	if (_state != binding_state)
	{
		_address->unbind();
	}
}
template <typename T> inline
typename binder_map<index_buffer<T>, T>::iterator binder_map<index_buffer<T>, T>::end()
{
	return iterator(_data + _address->count_binding());
}
template <typename T> inline
typename binder_map<index_buffer<T>, T>::const_iterator binder_map<index_buffer<T>, T>::end() const
{
	return const_iterator(_data + _address->count_binding());
}
template <typename T> inline
typename binder_map<index_buffer<T>, T>::const_iterator binder_map<index_buffer<T>, T>::cend() const
{
	return const_iterator(_data + _address->count_binding());
}

template <typename T> inline
index_buffer<T> make_index_buffer(const T* indices, int count, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	return index_buffer<T>(indices, count, usg);
}
template <typename T, int N> inline
index_buffer<T> make_index_buffer(const T(&indices)[N], buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	return index_buffer<T>(indices, usg);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
index_buffer<T> make_index_buffer(const ARRAY<T, N>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	return index_buffer<T>(indices, usg);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
index_buffer<T> make_index_buffer(const VECTOR<T, ALLOC>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	return index_buffer<T>(indices, usg);
}
template <typename T> inline
index_buffer<T> make_index_buffer(int count)
{
	return index_buffer<T>(count);
}

template <typename T> inline
index_buffer<T>& make_index_buffer(index_buffer<T>& b, const T* indices, int count, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	b.initialize(indices, count, usg);
	return b;
}
template <typename T, int N> inline
index_buffer<T>& make_index_buffer(index_buffer<T>& b, const T(&indices)[N], buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	b.initialize(indices, usg);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
index_buffer<T>& make_index_buffer(index_buffer<T>& b, const ARRAY<T, N>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	b.initialize(indices, usg);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
index_buffer<T>& make_index_buffer(index_buffer<T>& b, const VECTOR<T, ALLOC>& indices, buffer_usage_type_t usg = buffer_usage_type::immutable_draw)
{
	b.initialize(indices, usg);
	return b;
}
template <typename T> inline
index_buffer<T>& make_index_buffer(index_buffer<T>& b, int count)
{
	b.initialize(count);
	return b;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const index_buffer<T>& v)
{
	os << io::widen("index_buffer: {") << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl <<
		io::tab << io::widen("type size: ") << sizeof(T) << std::endl <<
		io::tab << io::widen("count: ") << v.scount() << std::endl <<
		io::tab << io::widen("size: ") << (v.count() * sizeof(T)) << std::endl;
	if (v.binding())
	{
		std::ios_base::fmtflags flag = os.flags();
		os << std::hex << io::tab << io::widen("usage: 0x") << v.usage_binding() << std::endl;
		os.flags(flag);
		os << io::tab << io::widen("writable: ") << io::widen(v.writable_binding()) << std::endl;
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

#endif // __POCKET_GL_INDEX_BUFFER_H__
