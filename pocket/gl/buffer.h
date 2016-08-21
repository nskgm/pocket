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
	GLenum usage() const;
	void* map(buffer_base::map_usage_type) const;
	template <typename T> T* map(buffer_base::map_usage_type) const;
	template <typename F> bool map(buffer_base::map_usage_type, F) const;
	template <typename T, typename F> bool map(buffer_base::map_usage_type, F) const;
	void unmap() const;

	binder_map<buffer, void> make_binder_map(buffer_base::map_usage_type) const;
	template <buffer_base::map_usage_type U> binder_map<buffer, void> make_binder_map() const;
	template <typename T> binder_map<buffer, T> make_binder_map(buffer_base::map_usage_type) const;
	template <typename T, buffer_base::map_usage_type U> binder_map<buffer, T> make_binder_map() const;

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

class buffer : public buffer_base
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

	typedef buffer_base base;

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _id;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	buffer() :
		base(),
		_id(0)
	{}
	explicit buffer(GLenum id, buffer_type type) :
		base(type),
		_id(id)
	{}
	explicit buffer(GLenum id, GLenum type) :
		base(type),
		_id(id)
	{}
	explicit buffer(buffer_type type, usage_type usg, int size, const void* data)
	{
		initialize(type, usg, size, data);
	}
	template <typename T>
	explicit buffer(buffer_type type, usage_type usg, int size, const T& a)
	{
		initialize(type, usg, size, a);
	}
	template <typename T, int N>
	explicit buffer(buffer_type type, usage_type usg, int size, const T(&a)[N])
	{
		initialize(type, usg, size, a);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	explicit buffer(buffer_type type, usage_type usg, int size, const ARRAY<T, N>& a)
	{
		initialize(type, usg, size, a);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	explicit buffer(buffer_type type, usage_type usg, int size, const VECTOR<T, ALLOC>& a)
	{
		initialize(type, usg, size, a);
	}
	buffer(const buffer& b) :
		base(b),
		_id(b._id)
	{}
#ifdef _USE_CXX11
	buffer(buffer&& b) :
		base(std::forward<buffer_base>(b)),
		_id(std::move(b._id))
	{
		b._id = 0;
	}
#endif // _USE_CXX11
	~buffer()
	{
		finalize();
	}

private:
	buffer(GLenum type, int err, GLuint id) :
		base(type, err),
		_id(id)
	{}

public:
	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	bool initialize(buffer_type type, usage_type usg, int size, const void* data)
	{
		finalize();

		// OpenGL側での種類
		_type = buffer_base::to_gl_type(type);

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

		// 扱い方
		GLenum gl_usage = buffer_base::to_gl_usage(usg);
		// 動的バッファの場合は容量を確保してからデータを設定する
		if (usg == dynamic_draw || usg == dynamic_read || usg == dynamic_copy)
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), NULL, gl_usage);
			if (data != NULL)
			{
				glBufferSubData(_type, 0, static_cast<GLsizeiptr>(size), data);
			}
		}
		else
		{
			glBufferData(_type, static_cast<GLsizeiptr>(size), data, gl_usage);
		}

		glBindBuffer(_type, 0);
		return true;
	}
	template <typename T>
	bool initialize(buffer_type type, usage_type usg, const T& a)
	{
		return initialize(type, usg, sizeof(T), static_cast<const void*>(&a));
	}
	template <typename T, int N>
	bool initialize(buffer_type type, usage_type usg, const T(&a)[N])
	{
		return initialize(type, usg, sizeof(T)*N, static_cast<const void*>(&a[0]));
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(buffer_type type, usage_type usg, const ARRAY<T, N>& a)
	{
		return initialize(type, usg, sizeof(T)*N, static_cast<const void*>(&a[0]));
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(buffer_type type, usage_type usg, const VECTOR<T, ALLOC>& a)
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
		_type = 0;
	}

	// バインド
	void bind() const
	{
		glBindBuffer(_type, _id);
	}
	void bind(GLenum type) const
	{
		glBindBuffer(type, _id);
	}
	void bind(buffer_type type) const
	{
		bind(buffer_base::to_gl_type(type));
	}
	void bind_base(GLuint point) const
	{
		glBindBufferBase(_type, point, _id);
	}
	void bind_base(buffer_type type) const
	{
		glBindBufferBase(buffer_base::to_gl_type(type), 0, _id);
	}
	void bind_base(buffer_type type, GLuint point) const
	{
		glBindBufferBase(buffer_base::to_gl_type(type), point, _id);
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
	void unbind(GLenum type) const
	{
		glBindBuffer(type, 0);
	}
	void unbind(buffer_type type) const
	{
		unbind(buffer_base::to_gl_type(type));
	}
	void unbind_base(GLuint point) const
	{
		glBindBufferBase(_type, point, 0);
	}
	void unbind_base(buffer_type type) const
	{
		glBindBufferBase(buffer_base::to_gl_type(type), 0, 0);
	}
	void unbind_base(buffer_type type, GLuint point) const
	{
		glBindBufferBase(buffer_base::to_gl_type(type), point, 0);
	}

	// 現在のバッファーがバインドされているか
	bool binding() const
	{
		GLenum type = to_gl_binding_type();
		GLuint i = 0;
		glGetIntegerv(type, reinterpret_cast<GLint*>(&i));
		return i != 0 && i == _id;
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}
	template <GLenum V>
	rebinder1<GLenum>::type make_binder() const
	{
		return rebinder1<GLenum>::type(*this, V);
	}
	rebinder1<GLenum>::type make_binder(GLenum type) const
	{
		return rebinder1<GLenum>::type(*this, type);
	}
	template <buffer_type T>
	rebinder1<buffer_type>::type make_binder() const
	{
		return rebinder1<buffer_type>::type(*this, T);
	}
	rebinder1<buffer_type>::type make_binder(buffer_type type) const
	{
		return rebinder1<buffer_type>::type(*this, type);
	}

	// バッファを展開して先頭アドレスを取得
	void* map(map_usage_type type) const
	{
		bind();
		return map_binding(type);
	}
	template <typename T>
	T* map(map_usage_type type) const
	{
		return static_cast<T*>(map(type));
	}
	void* map_binding(map_usage_type type) const
	{
		return glMapBuffer(_type, buffer_base::to_gl_map_usage(type));
	}
	template <typename T>
	T* map_binding(map_usage_type type) const
	{
		return static_cast<T*>(map_binding(type));
	}

	// 展開してアドレスを取得できていたら渡された関数を実行
	template <typename F>
	bool map(map_usage_type type, F func) const
	{
		binder_type lock(*this);
		return map_binding(type, func);
	}
	template <typename T, typename F>
	bool map(map_usage_type type, F func) const
	{
		binder_type lock(*this);
		return map_binding<T>(type, func);
	}
	template <typename F>
	bool map_binding(map_usage_type type, F func) const
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
	bool map_binding(map_usage_type type, F func) const
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
	binder_map_type make_binder_map(map_usage_type) const;
	template <map_usage_type U> binder_map_type make_binder_map() const;
	template <typename T> typename rebinder_map<T>::type make_binder_map(map_usage_type) const;
	template <typename T, map_usage_type U> typename rebinder_map<T>::type make_binder_map() const;

	binder_map_type make_binder_map(const binder_type&, map_usage_type) const;
	template <map_usage_type U> binder_map_type make_binder_map(const binder_type&) const;
	template <typename T> typename rebinder_map<T>::type make_binder_map(const binder_type&, map_usage_type) const;
	template <typename T, map_usage_type U> typename rebinder_map<T>::type make_binder_map(const binder_type&) const;

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
	GLenum usage() const
	{
		binder_type lock(*this);
		return usage_binding();
	}
	GLenum usage_binding() const
	{
		GLint u = 0;
		glGetBufferParameteriv(_type, GL_BUFFER_USAGE, &u);
		return static_cast<GLenum>(u);
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
		if (to_buffer_type() == unknown ||
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
		// バインド中はidの確認を行なう
		if (binding())
		{
			return glIsBuffer(_id) == GL_TRUE;
		}
		return true;
	}

	// バッファ種類
	buffer_type kind() const
	{
		return to_buffer_type();
	}

	// バッファ種類の比較
	bool kind_of(buffer_type type) const
	{
		return kind() == type;
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
		GLenum usg = usage_binding();
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
		GLenum usg = usage_binding();
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

	_CXX11_EXPLICIT operator GLuint () const
	{
		return _id;
	}

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
		b._type = 0;
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
class buffers : public buffer_base
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

	typedef buffer_base base;

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

	buffers() :
		base()
	{}
	buffers(const buffers& b) :
		base()
	{
		for (int i = 0; i < N; ++i)
		{
			_ids[i]._id = b._ids[i]._id;
		}
	}
#ifdef _USE_CXX11
	buffers(buffers&& b) :
		base(std::forward<buffer_base>(b)),
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
	{

	}
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

	explicit binder_map(const T& a, buffer_base::map_usage_type type) :
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
	explicit binder_map(const binder<T>& a, buffer_base::map_usage_type type) :
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

	explicit binder_map(const T& a, buffer_base::map_usage_type type) :
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
	explicit binder_map(const binder<T>& a, buffer_base::map_usage_type type) :
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
GLenum binder<buffer>::usage() const
{
	return _address->usage_binding();
}
inline
void* binder<buffer>::map(buffer_base::map_usage_type usg) const
{
	return _address->map_binding(usg);
}
template <typename T> inline
T* binder<buffer>::map(buffer_base::map_usage_type usg) const
{
	return _address->map_binding<T>(usg);
}
template <typename F> inline
bool binder<buffer>::map(buffer_base::map_usage_type usg, F func) const
{
	return _address->map_binding(usg, func);
}
template <typename T, typename F> inline
bool binder<buffer>::map(buffer_base::map_usage_type usg, F func) const
{
	return _address->map_binding<T>(usg, func);
}
inline
void binder<buffer>::unmap() const
{
	_address->unmap_binding();
}
inline
binder_map<buffer, void> binder<buffer>::make_binder_map(buffer_base::map_usage_type type) const
{
	return binder_map<buffer, void>(*this, type);
}
template <buffer_base::map_usage_type U> inline
binder_map<buffer, void> binder<buffer>::make_binder_map() const
{
	return binder_map<buffer, void>(*this, U);
}
template <typename T> inline
binder_map<buffer, T> binder<buffer>::make_binder_map(buffer_base::map_usage_type type) const
{
	return binder_map<buffer, T>(*this, type);
}
template <typename T, buffer_base::map_usage_type U> inline
binder_map<buffer, T> binder<buffer>::make_binder_map() const
{
	return binder_map<buffer, T>(*this, U);
}

inline
buffer::binder_map_type buffer::make_binder_map(map_usage_type usg) const
{
	return binder_map_type(*this, usg);
}
template <buffer_base::map_usage_type U> inline
buffer::binder_map_type buffer::make_binder_map() const
{
	return binder_map_type(*this, U);
}
template <typename T> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map(map_usage_type usg) const
{
	return typename rebinder_map<T>::type(*this, usg);
}
template <typename T, buffer_base::map_usage_type U> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map() const
{
	return typename rebinder_map<T>::type(*this, U);
}
inline
buffer::binder_map_type buffer::make_binder_map(const binder_type& a, map_usage_type usg) const
{
	return binder_map_type(a, usg);
}
template <buffer_base::map_usage_type U> inline
buffer::binder_map_type buffer::make_binder_map(const binder_type& a) const
{
	return binder_map_type(a, U);
}
template <typename T> inline
typename buffer::template rebinder_map<T>::type buffer::make_binder_map(const binder_type& a, map_usage_type usg) const
{
	return typename rebinder_map<T>::type(a, usg);
}
template <typename T, buffer_base::map_usage_type U> inline
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
		return buffer(buffer_base::TYPE, buffer_base::USAGE, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, int size, const void* data) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T), &a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer_##USAGE(const T(&a)[N]) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, &a[0]); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const T(&a)[N]) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, &a[0]); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer_##USAGE(const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*a.size(), a.data()); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*a.size(), a.data()); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer_##USAGE(const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, a.data()); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer_##USAGE(buffer& b, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_base::TYPE, buffer_base::USAGE, sizeof(T)*N, a.data()); \
		return b; \
	}

#define __POCKET_MAKE_BUFFER_TYPE(TYPE) inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, int size, const void* data) \
	{ \
		return buffer(buffer_base::TYPE, usage, size, data); \
	} \
	inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, int size, const void* data) \
	{ \
		b.initialize(buffer_base::TYPE, usage, size, data); \
		return b; \
	} \
	template <typename T> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const T& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T), &a); \
	} \
	template <typename T> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const T& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, int N> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const T(&a)[N]) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*N, &a[0]); \
	} \
	template <typename T, int N> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const T(&a)[N]) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T), &a); \
		return b; \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*a.size(), a.data()); \
	} \
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T)*a.size(), a.data()); \
		return b; \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer make_##TYPE##_buffer(buffer_base::usage_type usage, const ARRAY<T, N>& a) \
	{ \
		return buffer(buffer_base::TYPE, usage, sizeof(T)*N, a.data()); \
	} \
	template <typename T, size_t N, template <typename, size_t> class ARRAY> inline \
	buffer& make_##TYPE##_buffer(buffer& b, buffer_base::usage_type usage, const ARRAY<T, N>& a) \
	{ \
		b.initialize(buffer_base::TYPE, usage, sizeof(T)*N, a.data()); \
		return b; \
	} \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, immutable) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, dynamic) \
	__POCKET_MAKE_BUFFER_USAGE(TYPE, stream)

inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, int size, const void* data)
{
	return buffer(type, usage, size, data);
}
inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, int size, const void* data)
{
	b.initialize(type, usage, size, data);
	return b;
}
template <typename T> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const T& a)
{
	return buffer(type, usage, sizeof(T), &a);
}
template <typename T> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const T& a)
{
	b.initialize(type, usage, sizeof(T), &a);
	return b;
}
template <typename T, int N> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const T(&a)[N])
{
	return buffer(type, usage, sizeof(T)*N, &a[0]);
}
template <typename T, int N> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const T(&a)[N])
{
	b.initialize(type, usage, sizeof(T)*N, &a[0]);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a)
{
	return buffer(type, usage, sizeof(T)*a.size(), a.data());
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const VECTOR<T, ALLOC>& a)
{
	b.initialize(type, usage, sizeof(T)*a.size(), a.data());
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer make_buffer(buffer_base::buffer_type type, buffer_base::usage_type usage, const ARRAY<T, N>& a)
{
	return buffer(type, usage, sizeof(T)*N, a.data());
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
buffer& make_buffer(buffer& b, buffer_base::buffer_type type, buffer_base::usage_type usage, const ARRAY<T, N>& a)
{
	b.initialize(type, usage, sizeof(T)*N, a.data());
	return b;
}

__POCKET_MAKE_BUFFER_TYPE(array);
__POCKET_MAKE_BUFFER_TYPE(atomic_counter);
__POCKET_MAKE_BUFFER_TYPE(copy_read);
__POCKET_MAKE_BUFFER_TYPE(copy_write);
__POCKET_MAKE_BUFFER_TYPE(dispatch_indirect);
__POCKET_MAKE_BUFFER_TYPE(draw_indirect);
__POCKET_MAKE_BUFFER_TYPE(element);
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
		case buffer_base::array:
			type = "array";
			break;
		case buffer_base::atomic_counter:
			type = "atomic_counter";
			break;
		case buffer_base::copy_read:
			type = "copy_read";
			break;
		case buffer_base::copy_write:
			type = "copy_write";
			break;
		case buffer_base::dispatch_indirect:
			type = "dispatch_indirect";
			break;
		case buffer_base::draw_indirect:
			type = "draw_indirect";
			break;
		case buffer_base::element:
			type = "element";
			break;
		case buffer_base::pixel_pack:
			type = "pixel_pack";
			break;
		case buffer_base::pixel_unpack:
			type = "pixel_unpack";
			break;
		case buffer_base::query:
			type = "query";
			break;
		case buffer_base::shader_storage:
			type = "shader_storage";
			break;
		case buffer_base::texture:
			type = "texture";
			break;
		case buffer_base::transform_feedback:
			type = "transform_feedback";
			break;
		case buffer_base::uniform:
			type = "uniform";
			break;

		case buffer_base::unknown:
		default:
			type = "unknown";
			break;
	}

	const bool valid = v.valid();
	const char* valid_str = valid ? "true" : "false";
	os << io::widen("buffer: ") << io::braces_left << std::endl <<
		io::tab << io::widen("type: ") << io::widen(type) << std::endl <<
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
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const buffer& v)
{
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_BUFFER_H__
