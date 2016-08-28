#ifndef __POCKET_GL_BUFFER_H__
#define __POCKET_GL_BUFFER_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "buffer_base.h"
#include "../debug.h"
#include "../io.h"

namespace pocket
{
namespace gl
{

// forward
class buffer;
template <int> class buffers;
template <typename, typename> class binder_map;
class buffer_view;
class uniform_buffer;

template <>
class binder<buffer>
{
public:
	explicit binder(const buffer& a);
	~binder();
	int size() const;
	int count(int type_size) const;
	template <typename T> int count() const;
	buffer_usage_t usage() const;
	void* map(buffer_map_t) const;
	template <typename T> T* map(buffer_map_t) const;
	template <typename F> bool map(buffer_map_t, F) const;
	template <typename T, typename F> bool map(buffer_map_t, F) const;
	void unmap() const;

	binder_map<buffer, void> make_binder_map(buffer_map_t) const;
	template <buffer_map_t U> binder_map<buffer, void> make_binder_map() const;
	template <typename T> binder_map<buffer, T> make_binder_map(buffer_map_t) const;
	template <typename T, buffer_map_t U> binder_map<buffer, T> make_binder_map() const;

	bool binding() const
	{
		return true;
	}

	const buffer* operator -> () const
	{
		return _address;
	}
	const buffer& operator * () const
	{
		return *_address;
	}

	operator const buffer& () const
	{
		return *_address;
	}
	operator const buffer* () const
	{
		return _address;
	}

private:
	const buffer* _address;
};

class buffer
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	friend class buffer_view;
	friend class uniform_buffer;

	typedef binder<buffer> binder_type;
	typedef binder_map<buffer, void> binder_map_type;

	template <typename U>
	struct rebinder1
	{
		typedef binder1<buffer, U> type;
	};
	template <typename U>
	struct rebinder_map
	{
		typedef binder_map<buffer, U> type;
	};

	enum identifier_t
	{
		identifier = GL_BUFFER
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _id;
	buffer_type_t _type;
	int _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffer() :
		_id(0),
		_type(buffer_type::unknown),
		_error_bitfield(0)
	{}
	explicit buffer(GLuint id, buffer_type_t type) :
		_id(id),
		_type(type),
		_error_bitfield(0)
	{}
	explicit buffer(buffer_type_t type, buffer_usage_t usg, int size, const void* data) :
		_id(0)
	{
		initialize(type, usg, size, data);
	}
	template <typename T>
	explicit buffer(buffer_type_t type, buffer_usage_t usg, const T& a) :
		_id(0)
	{
		initialize(type, usg, a);
	}
	template <typename T, int N>
	explicit buffer(buffer_type_t type, buffer_usage_t usg, const T(&a)[N]) :
		_id(0)
	{
		initialize(type, usg, a);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	explicit buffer(buffer_type_t type, buffer_usage_t usg, const ARRAY<T, N>& a) :
		_id(0)
	{
		initialize(type, usg, a);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	explicit buffer(buffer_type_t type, buffer_usage_t usg, const VECTOR<T, ALLOC>& a) :
		_id(0)
	{
		initialize(type, usg, a);
	}
	buffer(const buffer& b) :
		_id(b._id),
		_type(b._type),
		_error_bitfield(b._error_bitfield)
	{}
#ifdef _USE_CXX11
	buffer(buffer&& b) :
		_id(std::move(b._id)),
		_type(std::move(b._type)),
		_error_bitfield(std::move(b._error_bitfield))
	{
		b._id = 0;
		b._type = buffer_type::unknown;
		b._error_bitfield = 0;
	}
#endif // _USE_CXX11
	~buffer()
	{
		finalize();
	}

private:
	buffer(buffer_type_t type, int err, GLuint id) :
		_id(id),
		_type(type),
		_error_bitfield(err)
	{}

public:
	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	bool initialize(buffer_type_t type, buffer_usage_t usg, int size, const void* data)
	{
		finalize();

		_type = type;

		glGenBuffers(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		// 値の設定のためにバインド
		glBindBuffer(_type, _id);

		// バインドの状態を確認
		if (glIsBuffer(_id) == GL_FALSE)
		{
			// バインド出来る状態ではない
			_error_bitfield |= error_binding;
			return false;
		}

		// 動的バッファの場合は容量を確保してからデータを設定する
		if (usg == buffer_usage::dynamic_draw ||
			usg == buffer_usage::dynamic_read ||
			usg == buffer_usage::dynamic_copy)
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), NULL, usg);
			if (data != NULL)
			{
				glBufferSubData(_type, 0, static_cast<GLsizeiptr>(size), data);
			}
		}
		else
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), data, usg);
		}

		glBindBuffer(_type, 0);
		return true;
	}
	template <typename T>
	bool initialize(buffer_type_t type, buffer_usage_t usg, const T& a)
	{
		return initialize(type, usg, sizeof(T), static_cast<const void*>(&a));
	}
	template <typename T, int N>
	bool initialize(buffer_type_t type, buffer_usage_t usg, const T(&a)[N])
	{
		return initialize(type, usg, sizeof(T)*N, static_cast<const void*>(&a[0]));
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(buffer_type_t type, buffer_usage_t usg, const ARRAY<T, N>& a)
	{
		return initialize(type, usg, sizeof(T)*N, static_cast<const void*>(&a[0]));
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(buffer_type_t type, buffer_usage_t usg, const VECTOR<T, ALLOC>& a)
	{
		return initialize(type, usg, sizeof(T)*a.size(), static_cast<const void*>(&a[0]));
	}

	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
		_error_bitfield = 0;
		_type = buffer_type::unknown;
	}

	// エラー状態クリア
	void clear()
	{
		_error_bitfield = 0;
	}

	// バインド
	void bind() const
	{
		glBindBuffer(_type, _id);
	}
	void bind(buffer_type_t type) const
	{
		glBindBuffer(type, _id);
	}
	void bind_base(GLuint point) const
	{
		glBindBufferBase(_type, point, _id);
	}
	void bind_base(buffer_type_t type) const
	{
		glBindBufferBase(type, 0, _id);
	}
	void bind_base(buffer_type_t type, GLuint point) const
	{
		glBindBufferBase(type, point, _id);
	}
	void bind_vertex(GLuint index, GLintptr offset, GLsizei stride) const
	{
		glBindVertexBuffer(index, _id, offset, stride);
	}
	template <typename T>
	void bind_vertex(GLuint index) const
	{
		glBindVertexBuffer(index, _id, 0, sizeof(T));
	}

	// バインド解除
	void unbind() const
	{
		glBindBuffer(_type, 0);
	}
	void unbind(buffer_type_t type) const
	{
		glBindBuffer(type, 0);
	}
	void unbind_base(GLuint point) const
	{
		glBindBufferBase(_type, point, 0);
	}
	void unbind_base(buffer_type_t type) const
	{
		glBindBufferBase(type, 0, 0);
	}
	void unbind_base(buffer_type_t type, GLuint point) const
	{
		glBindBufferBase(type, point, 0);
	}

	// 現在のバッファーがバインドされているか
	bool binding() const
	{
		buffer_binding_type_t type = buffer_type::to_binding_type(_type);
		GLuint i = 0;
		glGetIntegerv(type, reinterpret_cast<GLint*>(&i));
		return i != 0 && i == _id;
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}
	template <buffer_type_t T>
	rebinder1<buffer_type_t>::type make_binder() const
	{
		return rebinder1<buffer_type_t>::type(*this, T);
	}
	rebinder1<buffer_type_t>::type make_binder(buffer_type_t type) const
	{
		return rebinder1<buffer_type_t>::type(*this, type);
	}

	// バッファを展開して先頭アドレスを取得
	void* map(buffer_map_t type) const
	{
		bind();
		return map_binding(type);
	}
	template <typename T>
	T* map(buffer_map_t type) const
	{
		return static_cast<T*>(map(type));
	}
	void* map_binding(buffer_map_t type) const
	{
		return glMapBuffer(_type, type);
	}
	template <typename T>
	T* map_binding(buffer_map_t type) const
	{
		return static_cast<T*>(map_binding(type));
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
		void* address = map_binding(type);
		if (address == NULL)
		{
			return false;
		}
		func(static_cast<T*>(address));
		unmap_binding();
		return true;
	}

	// バッファの展開を解除
	void unmap() const
	{
		unmap_binding();
		unbind();
	}
	void unmap_binding() const
	{
		glUnmapBuffer(_type);
	}

	// 展開されている状態か
	bool mapping() const
	{
		GLint mapped;
		glGetBufferParameteriv(_type, GL_BUFFER_MAPPED, &mapped);
		return mapped == GL_TRUE;
	}

	// マップしたものを管理するオブジェクトを作成
	binder_map_type make_binder_map(buffer_map_t) const;
	template <buffer_map_t U> binder_map_type make_binder_map() const;
	template <typename T> typename rebinder_map<T>::type make_binder_map(buffer_map_t) const;
	template <typename T, buffer_map_t U> typename rebinder_map<T>::type make_binder_map() const;

	binder_map_type make_binder_map(const binder_type&, buffer_map_t) const;
	template <buffer_map_t U> binder_map_type make_binder_map(const binder_type&) const;
	template <typename T> typename rebinder_map<T>::type make_binder_map(const binder_type&, buffer_map_t) const;
	template <typename T, buffer_map_t U> typename rebinder_map<T>::type make_binder_map(const binder_type&) const;

	// バッファサイズ
	int size() const
	{
		binder_type lock(*this);
		return size_binding();
	}
	int size_binding() const
	{
		GLint i = 0;
		glGetBufferParameteriv(_type, GL_BUFFER_SIZE, &i);
		return static_cast<int>(i);
	}

	// 型の数
	int count(int type_size) const
	{
		return size() / type_size;
	}
	template <typename T>
	int count() const
	{
		return size() / sizeof(T);
	}
	int count_binding(int type_size) const
	{
		return size_binding() / type_size;
	}
	template <typename T>
	int count_binding() const
	{
		return size_binding() / sizeof(T);
	}

	// 設定した時の扱い法
	buffer_usage_t usage() const
	{
		binder_type lock(*this);
		return usage_binding();
	}
	buffer_usage_t usage_binding() const
	{
		GLint u = 0;
		glGetBufferParameteriv(_type, GL_BUFFER_USAGE, &u);
		return static_cast<buffer_usage_t>(u);
	}

	// エラー文
	std::string error() const
	{
		// IDの作成に失敗している
		if (error_status(error_creating))
		{
			return "failed. glGenBuffers().";
		}
		if (error_status(error_binding))
		{
			return "failed. can not bind.";
		}
		// 作成されていない またはすでに破棄済み
		if (_type == buffer_type::unknown ||
			_id == 0)
		{
			return "not created. or already destroyed.";
		}
		// エラーは起こしていない
		return "";
	}

	// エラーのステータス確認
	bool error_status(error_bitfield bit) const
	{
		return (_error_bitfield & bit) != 0;
	}

	// 有効な状態か
	bool valid() const
	{
		if (_id == 0 ||
			_error_bitfield != 0)
		{
			return false;
		}
		return glIsBuffer(_id) == GL_TRUE;
	}

	// バッファ種類
	buffer_type_t kind() const
	{
		return _type;
	}

	// バッファ種類の比較
	bool kind_of(buffer_type_t type) const
	{
		return _type == type;
	}

	// ハンドルの取得
	GLuint& get()
	{
		return _id;
	}
	const GLuint& get() const
	{
		return _id;
	}

	// 同じ設定のバッファをクローン
	buffer clone() const
	{
		// 構成する同じ設定のもの
		GLuint id = 0;

		glGenBuffers(1, &id);
		if (id == 0)
		{
			return buffer(_type, error_creating, 0);
		}

		// サイズの取得
		bind();
		int sz = size_binding();
		buffer_usage_t usg = usage_binding();
		unbind();

		// 空の値の設定
		glBindBuffer(_type, id);

		// バインド出来ない
		if (glIsBuffer(id) == GL_FALSE)
		{
			glBindBuffer(_type, 0);
			// 作成されたIDは削除
			glDeleteBuffers(1, &id);
			return buffer(_type, error_binding, 0);
		}

		glBufferData(_type, sz, NULL, usg);
		glBindBuffer(_type, 0);

		// コピー元とコピー先を設定
		glBindBuffer(GL_COPY_READ_BUFFER, _id); // 読み取られるバッファ
		glBindBuffer(GL_COPY_WRITE_BUFFER, id); // 書き込まれるバッファ
		// 読み取りバッファから書き込みバッファへ値のコピー
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sz);

		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		return buffer(_type, 0, id);
	}
	bool clone(buffer& c) const
	{
		c.finalize();
		c._type = _type;

		glGenBuffers(1, &c._id);
		if (c._id == 0)
		{
			c._error_bitfield |= error_creating;
			return false;
		}

		// サイズの取得
		bind();
		int sz = size_binding();
		buffer_usage_t usg = usage_binding();
		unbind();

		c.bind();
		if (glIsBuffer(c._id) == GL_FALSE)
		{
			c.unbind();
			// 作成されたIDは削除
			glDeleteBuffers(1, &c._id);
			c._id = 0;
			c._error_bitfield |= error_binding;
			return false;
		}
		glBufferData(_type, sz, NULL, usg);
		c.unbind();

		glBindBuffer(GL_COPY_READ_BUFFER, _id);
		glBindBuffer(GL_COPY_WRITE_BUFFER, c._id);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sz);

		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		return true;
	}

public:
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

	bool operator == (const buffer& b) const
	{
		return _id == b._id && _type == b._type;
	}
	bool operator != (const buffer& b) const
	{
		return !(*this == b);
	}

	buffer& operator = (const buffer& b)
	{
		_type = b._type;
		_id = b._id;
		_error_bitfield = b._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	buffer& operator = (buffer&& b)
	{
		_type = std::move(b._type);
		_id = std::move(b._id);
		_error_bitfield = std::move(b._error_bitfield);
		b._type = buffer_type::unknown;
		b._id = 0;
		b._error_bitfield = 0;
		return *this;
	}

	buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};

template <int N>
class buffers
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// IDを保持と関数を定義する型
	// 関数実装はここに記述
	class holder
	{
	private:
		template <int> friend class buffers;

		GLuint _id;

	public:
		holder() :
			_id(0)
		{}

		// ハンドルの取得
		GLuint& get()
		{
			return _id;
		}
		const GLuint& get() const
		{
			return _id;
		}
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	// 数分保持
	holder _ids[N];

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffers()
	{}
	buffers(const buffers& b)
	{
		for (int i = 0; i < N; ++i)
		{
			_ids[i]._id = b._ids[i]._id;
		}
	}
#ifdef _USE_CXX11
	buffers(buffers&& b) :
		_ids(std::move(b._ids))
	{
		for (int i = 0; i < N; ++i)
		{
			b._ids[i]._id = 0;
		}
	}
#endif // _USE_CXX11

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	// none
};

namespace detail
{

template <typename T>
class binder_map_iterator
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

	T* _data;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	explicit binder_map_iterator(T* data) :
		_data(data)
	{}
	~binder_map_iterator()
	{}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	bool operator == (const binder_map_iterator& i) const
	{
		return _data == i._data;
	}
	bool operator != (const binder_map_iterator& i) const
	{
		return !(*this == i);
	}

	binder_map_iterator& operator ++ ()
	{
		++_data;
		return *this;
	}
	binder_map_iterator operator ++ (int)
	{
		T* a = _data;
		++_data;
		return binder_map_iterator(a);
	}
	binder_map_iterator& operator -- ()
	{
		--_data;
		return *this;
	}
	binder_map_iterator operator -- (int)
	{
		T* a = _data;
		--_data;
		return binder_map_iterator(a);
	}

	binder_map_iterator& operator += (ptrdiff_t diff)
	{
		_data += diff;
		return *this;
	}
	binder_map_iterator& operator -= (ptrdiff_t diff)
	{
		_data -= diff;
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
};

}

template <typename T, typename M>
class binder_map
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef detail::binder_map_iterator<M> iterator;
	typedef detail::binder_map_iterator<const M> const_iterator;

	// 状態を表す
	enum state_type
	{
		binding_bit = 1 << 0,
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	const T* _address;
	M* _data;
	int _state_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	explicit binder_map(const T& a, buffer_map_t type) :
		_address(&a), _data(NULL), _state_bitfield(0)
	{
		// バインドされている時はデストラクタ時に解除しない
		if (a.binding())
		{
			_state_bitfield |= binding_bit;
		}
		else
		{
			a.bind();
		}
		_data = static_cast<M*>(a.map_binding(type));
	}
	explicit binder_map(const binder<T>& a, buffer_map_t type) :
		_address(a),
		_data(static_cast<M*>(a->map_binding(type))),
		_state_bitfield(binding_bit)
	{}
	~binder_map()
	{
		_address->unmap_binding();
		if ((_state_bitfield & binding_bit) == 0)
		{
			_address->unbind();
		}
	}

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

	iterator end()
	{
		return iterator(_data + _address->template count_binding<M>());
	}
	const_iterator end() const
	{
		return const_iterator(_data + _address->template count_binding<M>());
	}
	const_iterator cend() const
	{
		return const_iterator(_data + _address->template count_binding<M>());
	}

	int count() const
	{
		return _address->template count_binding<M>();
	}

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

template <typename T>
class binder_map<T, void>
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	enum state_type
	{
		binding_bit = 1 << 0,
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	const T* _address;
	void* _data;
	int _state_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	explicit binder_map(const T& a, buffer_map_t type) :
		_address(&a), _data(NULL), _state_bitfield(0)
	{
		// バインドされている時はデストラクタ時に解除しない
		if (a.binding())
		{
			_state_bitfield |= binding_bit;
		}
		else
		{
			a.bind();
		}
		_data = a.map_binding(type);
	}
	explicit binder_map(const binder<T>& a, buffer_map_t type) :
		_address(a),
		_data(a->map_binding(type)),
		_state_bitfield(binding_bit)
	{}
	~binder_map()
	{
		_address->unmap_binding();
		if ((_state_bitfield & binding_bit) == 0)
		{
			_address->unbind();
		}
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	void* get() const
	{
		return _data;
	}
	template <typename U>
	U* get() const
	{
		return static_cast<U*>(_data);
	}

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
};

inline
binder<buffer>::binder(const buffer& a) :
	_address(&a)
{
	a.bind();
}
inline
binder<buffer>::~binder()
{
	_address->unbind();
}
inline
int binder<buffer>::size() const
{
	return _address->size_binding();
}
inline
int binder<buffer>::count(int type_size) const
{
	return _address->count_binding(type_size);
}
template <typename T> inline
int binder<buffer>::count() const
{
	return _address->count_binding<T>();
}
inline
buffer_usage_t binder<buffer>::usage() const
{
	return _address->usage_binding();
}
inline
void* binder<buffer>::map(buffer_map_t usg) const
{
	return _address->map_binding(usg);
}
template <typename T> inline
T* binder<buffer>::map(buffer_map_t usg) const
{
	return _address->map_binding<T>(usg);
}
template <typename F> inline
bool binder<buffer>::map(buffer_map_t usg, F func) const
{
	return _address->map_binding(usg, func);
}
template <typename T, typename F> inline
bool binder<buffer>::map(buffer_map_t usg, F func) const
{
	return _address->map_binding<T>(usg, func);
}
inline
void binder<buffer>::unmap() const
{
	_address->unmap_binding();
}
inline
binder_map<buffer, void> binder<buffer>::make_binder_map(buffer_map_t type) const
{
	return binder_map<buffer, void>(*this, type);
}
template <buffer_map_t U> inline
binder_map<buffer, void> binder<buffer>::make_binder_map() const
{
	return binder_map<buffer, void>(*this, U);
}
template <typename T> inline
binder_map<buffer, T> binder<buffer>::make_binder_map(buffer_map_t type) const
{
	return binder_map<buffer, T>(*this, type);
}
template <typename T, buffer_map_t U> inline
binder_map<buffer, T> binder<buffer>::make_binder_map() const
{
	return binder_map<buffer, T>(*this, U);
}

inline
buffer::binder_map_type buffer::make_binder_map(buffer_map_t usg) const
{
	return binder_map_type(*this, usg);
}
template <buffer_map_t U> inline
buffer::binder_map_type buffer::make_binder_map() const
{
	return binder_map_type(*this, U);
}
template <typename T> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map(buffer_map_t usg) const
{
	return typename rebinder_map<T>::type(*this, usg);
}
template <typename T, buffer_map_t U> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map() const
{
	return typename rebinder_map<T>::type(*this, U);
}
inline
buffer::binder_map_type buffer::make_binder_map(const binder_type& a, buffer_map_t usg) const
{
	return binder_map_type(a, usg);
}
template <buffer_map_t U> inline
buffer::binder_map_type buffer::make_binder_map(const binder_type& a) const
{
	return binder_map_type(a, U);
}
template <typename T> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map(const binder_type& a, buffer_map_t usg) const
{
	return typename rebinder_map<T>::type(a, usg);
}
template <typename T, buffer_map_t U> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map(const binder_type& a) const
{
	return typename rebinder_map<T>::type(a, U);
}

/*------------------------------
* make_buffer
* make_[type]_buffer
* make_[type]_buffer_[usage(immutable, dynamic, stream)]
*------------------------------*/

#define __POCKET_MAKE_BUFFER_USAGE(TYPE, USAGE) inline \
	buffer make_##TYPE##_buffer_##USAGE(int size, const void* data) \
	{ \
		return buffer(buffer_type::TYPE, buffer_usage::USAGE, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, int size, const void* data) \
	{ \
		b.initialize(buffer_type::TYPE, buffer_usage::USAGE, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T& a) \
	{ \
		return buffer(buffer_type::TYPE, buffer_usage::USAGE, a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T& a) \
	{ \
		b.initialize(buffer_type::TYPE, buffer_usage::USAGE, a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T(&a)[N]) \
	{ \
		return buffer(buffer_type::TYPE, buffer_usage::USAGE, a); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T(&a)[N]) \
	{ \
		b.initialize(buffer_type::TYPE, buffer_usage::USAGE, a); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer_##USAGE(const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_type::TYPE, buffer_usage::USAGE, a); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_type::TYPE, buffer_usage::USAGE, a); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer_##USAGE(const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_type::TYPE, buffer_usage::USAGE, a); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_type::TYPE, buffer_usage::USAGE, a); \
		return b; \
	}

#define __POCKET_MAKE_BUFFER_TYPE(TYPE) inline \
	buffer make_##TYPE##_buffer(buffer_usage_t usage, int size, const void* data) \
	{ \
		return buffer(buffer_type::TYPE, usage, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_usage_t usage, int size, const void* data) \
	{ \
		b.initialize(buffer_type::TYPE, usage, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer(buffer_usage_t usage, const T& a) \
	{ \
		return buffer(buffer_type::TYPE, usage, a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_usage_t usage, const T& a) \
	{ \
		b.initialize(buffer_type::TYPE, usage, a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer(buffer_usage_t usage, const T(&a)[N]) \
	{ \
		return buffer(buffer_type::TYPE, usage, a); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_usage_t usage, const T(&a)[N]) \
	{ \
		b.initialize(buffer_type::TYPE, usage, a); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer(buffer_usage_t usage, const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_type::TYPE, usage, a); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_usage_t usage, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_type::TYPE, usage, a); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer(buffer_usage_t usage, const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_type::TYPE, usage, a); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_usage_t usage, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_type::TYPE, usage, a); \
		return b; \
	} \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, immutable) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, dynamic) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, stream)

inline
buffer make_buffer(buffer_type_t type, buffer_usage_t usage, int size, const void* data)
{
	return buffer(type, usage, size, data);
}
inline
buffer& make_buffer(buffer& b, buffer_type_t type, buffer_usage_t usage, int size, const void* data)
{
	b.initialize(type, usage, size, data);
	return b;
}
template <typename T> inline
buffer make_buffer(buffer_type_t type, buffer_usage_t usage, const T& a)
{
	return buffer(type, usage, a);
}
template <typename T> inline
buffer& make_buffer(buffer& b, buffer_type_t type, buffer_usage_t usage, const T& a)
{
	b.initialize(type, usage, a);
	return b;
}
template <typename T, int N> inline
buffer make_buffer(buffer_type_t type, buffer_usage_t usage, const T(&a)[N])
{
	return buffer(type, usage, a);
}
template <typename T, int N> inline
buffer& make_buffer(buffer& b, buffer_type_t type, buffer_usage_t usage, const T(&a)[N])
{
	b.initialize(type, usage, a);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer make_buffer(buffer_type_t type, buffer_usage_t usage, const VECTOR<T, ALLOC>& a)
{
	return buffer(type, usage, a);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer& make_buffer(buffer& b, buffer_type_t type, buffer_usage_t usage, const VECTOR<T, ALLOC>& a)
{
	b.initialize(type, usage, a);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer make_buffer(buffer_type_t type, buffer_usage_t usage, const ARRAY<T, N>& a)
{
	return buffer(type, usage, a);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer& make_buffer(buffer& b, buffer_type_t type, buffer_usage_t usage, const ARRAY<T, N>& a)
{
	b.initialize(type, usage, a);
	return b;
}

__POCKET_MAKE_BUFFER_TYPE(array);
__POCKET_MAKE_BUFFER_TYPE(atomic_counter);
__POCKET_MAKE_BUFFER_TYPE(copy_read);
__POCKET_MAKE_BUFFER_TYPE(copy_write);
__POCKET_MAKE_BUFFER_TYPE(dispatch_indirect);
__POCKET_MAKE_BUFFER_TYPE(draw_indirect);
__POCKET_MAKE_BUFFER_TYPE(element_array);
__POCKET_MAKE_BUFFER_TYPE(pixel_pack);
__POCKET_MAKE_BUFFER_TYPE(pixel_unpack);
__POCKET_MAKE_BUFFER_TYPE(query);
__POCKET_MAKE_BUFFER_TYPE(shader_storage);
__POCKET_MAKE_BUFFER_TYPE(texture);
__POCKET_MAKE_BUFFER_TYPE(transform_feedback);
__POCKET_MAKE_BUFFER_TYPE(uniform);

#undef __POCKET_MAKE_BUFFER_TYPE
#undef __POCKET_MAKE_BUFFER_USAGE

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const buffer& v)
{
	const char* type;
	switch (v.kind())
	{
		case buffer_type::array:
			type = "array";
			break;
		case buffer_type::atomic_counter:
			type = "atomic_counter";
			break;
		case buffer_type::copy_read:
			type = "copy_read";
			break;
		case buffer_type::copy_write:
			type = "copy_write";
			break;
		case buffer_type::dispatch_indirect:
			type = "dispatch_indirect";
			break;
		case buffer_type::draw_indirect:
			type = "draw_indirect";
			break;
		case buffer_type::element_array:
			type = "element_array";
			break;
		case buffer_type::pixel_pack:
			type = "pixel_pack";
			break;
		case buffer_type::pixel_unpack:
			type = "pixel_unpack";
			break;
		case buffer_type::query:
			type = "query";
			break;
		case buffer_type::shader_storage:
			type = "shader_storage";
			break;
		case buffer_type::texture:
			type = "texture";
			break;
		case buffer_type::transform_feedback:
			type = "transform_feedback";
			break;
		case buffer_type::uniform:
			type = "uniform";
			break;

		case buffer_type::unknown:
		default:
			type = "unknown";
			break;
	}

	os << io::widen("buffer: ") << io::braces_left << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl <<
		io::tab << io::widen("type: ") << io::widen(type) << std::endl;
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

#endif // __POCKET_GL_BUFFER_H__
