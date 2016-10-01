#ifndef __POCKET_GL_LAYERED_VERTEX_BUFFER_H__
#define __POCKET_GL_LAYERED_VERTEX_BUFFER_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "buffer.h"
#include "vertex_array.h"

namespace pocket
{
namespace gl
{

// forward
template <typename> class layered_vertex_buffer;
class draw_indirect_buffer;

// vertex_layout vertex_layout_index用宣言
#define __POCKET_LAYERED_VERTEX_BUFFER_INITIALIZE(TYPE) \
	template <int N>\
	bool initialize(POCKET_CREF_ARRAY_ARG(vertex_type, vertices, N), const TYPE* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], N, layouts, count, usg);\
	}\
	template <size_t N, template <typename, size_t> class ARRAY>\
	bool initialize(const ARRAY<vertex_type, N>& vertices, const TYPE* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], N, layouts, count, usg);\
	}\
	template <typename ALLOC, template <typename, typename> class VECTOR>\
	bool initialize(const VECTOR<vertex_type, ALLOC>& vertices, const TYPE* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], vertices.size(), layouts, count, usg);\
	}\
	template <int N>\
	bool initialize(const vertex_type* vertices, int count, POCKET_CREF_ARRAY_ARG(TYPE, layouts, N), buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(vertices, count, &layouts[0], N, usg);\
	}\
	template <size_t N, template <typename, size_t> class ARRAY>\
	bool initialize(const vertex_type* vertices, int count, const ARRAY<TYPE, N>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(vertices, count, &layouts[0], N, usg);\
	}\
	template <typename ALLOC, template <typename, typename> class VECTOR>\
	bool initialize(const vertex_type* vertices, int count, const VECTOR<TYPE, ALLOC>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(vertices, count, &layouts[0], layouts.size(), usg);\
	}\
	template <int VN, int LN>\
	bool initialize(POCKET_CREF_ARRAY_ARG(vertex_type, vertices, VN), POCKET_CREF_ARRAY_ARG(TYPE, layouts, LN), buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], LN, usg);\
	}\
	template <size_t VN, size_t LN, template <typename, size_t> class ARRAY, template <typename, size_t> class ARRAY2>\
	bool initialize(const ARRAY<vertex_type, VN>& vertices, const ARRAY2<TYPE, LN>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], LN, usg);\
	}\
	template <typename ALLOC, typename ALLOC2, template <typename, typename> class VECTOR, template <typename, typename> class VECTOR2>\
	bool initialize(const VECTOR<vertex_type, ALLOC>& vertices, const VECTOR2<TYPE, ALLOC2>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], vertices.size(), &layouts[0], layouts.size(), usg);\
	}\
	template <int VN, size_t LN, template <typename, size_t> class ARRAY>\
	bool initialize(POCKET_CREF_ARRAY_ARG(vertex_type, vertices, VN), const ARRAY<TYPE, LN>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], LN, usg);\
	}\
	template <int VN, typename ALLOC, template <typename, typename> class VECTOR>\
	bool initialize(POCKET_CREF_ARRAY_ARG(vertex_type, vertices, VN), const VECTOR<TYPE, ALLOC>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], layouts.size(), usg);\
	}\
	template <size_t VN, int LN, template <typename, size_t> class ARRAY>\
	bool initialize(const ARRAY<vertex_type, VN>& vertices, POCKET_CREF_ARRAY_ARG(TYPE, layouts, LN), buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], LN, usg);\
	}\
	template <size_t VN, typename ALLOC, template <typename, size_t> class ARRAY, template <typename, typename> class VECTOR>\
	bool initialize(const ARRAY<vertex_type, VN>& vertices, const VECTOR<TYPE, ALLOC>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], VN, &layouts[0], layouts.size(), usg);\
	}\
	template <int LN, typename ALLOC, template <typename, typename> class VECTOR>\
	bool initialize(const VECTOR<vertex_type, ALLOC>& vertices, POCKET_CREF_ARRAY_ARG(TYPE, layouts, LN), buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], vertices.size(), &layouts[0], LN, usg);\
	}\
	template <size_t LN, typename ALLOC, template <typename, size_t> class ARRAY, template <typename, typename> class VECTOR>\
	bool initialize(const VECTOR<vertex_type, ALLOC>& vertices, const ARRAY<TYPE, LN>& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)\
	{\
		return initialize(&vertices[0], vertices.size(), &layouts[0], LN, usg);\
	}

template <typename T>
class layered_vertex_buffer
{
public:
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef T vertex_type;

	typedef binder<layered_vertex_buffer<T> > binder_type;
	typedef binder_map<vertex_buffer<T>, T> binder_map_type;

	template <int N>
	struct layout_type
	{
		typedef typename vertex_array::layout_type<N>::type type;
	};
	template <int N>
	struct layout_index_type
	{
		typedef typename vertex_array::layout_index_type<N>::type type;
	};

	// エラーを起こしているタイプ
	enum error_object_type
	{
		none,
		vertex_buffer_object,
		vertex_array_object,
	};

	enum identifier_t
	{
		identifier = GL_BUFFER
	};

private:
	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

	vertex_buffer<T> _vbo;
	vertex_array _vao;
	error_object_type _error;

public:
	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	layered_vertex_buffer() :
		_vbo(),
		_vao(),
		_error(none)
	{}
	explicit layered_vertex_buffer(const vertex_type* vertices, int vcount, const vertex_layout* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_vbo(),
		_vao()
	{
		initialize(vertices, vcount, layouts, lcount, usg);
	}
	explicit layered_vertex_buffer(const vertex_type* vertices, int vcount, const vertex_layout_index* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_vbo(),
		_vao()
	{
		initialize(vertices, vcount, layouts, lcount, usg);
	}
	template <typename U>
	explicit layered_vertex_buffer(const U& vertices, const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_vbo(),
		_vao()
	{
		initialize(vertices, layouts, count, usg);
	}
	template <typename U>
	explicit layered_vertex_buffer(const vertex_type* vertices, int count, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_vbo(),
		_vao()
	{
		initialize(vertices, count, layouts, usg);
	}
	template <typename U, typename V>
	explicit layered_vertex_buffer(const U& vertices, const V& layouts, buffer_usage_t usg = buffer_usage::immutable_draw) :
		_vbo(),
		_vao()
	{
		initialize(vertices, layouts, usg);
	}
	layered_vertex_buffer(const layered_vertex_buffer& b) :
		_vbo(b._vbo),
		_vao(b._vao),
		_error(b._error)
	{}
#ifdef POCKET_USE_CXX11
	layered_vertex_buffer(layered_vertex_buffer&& v) :
		_vbo(std::move(v._vbo)),
		_vao(std::move(v._vao)),
		_error(std::move(v._error))
	{
		v._error = none;
	}
#endif // POCKET_USE_CXX11
	~layered_vertex_buffer()
	{
		finalize();
	}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 初期化
	bool initialize(const vertex_type* vertices, int vcount, const vertex_layout* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		finalize();

		// VBO作成
		if (!_vbo.initialize(vertices, vcount, usg))
		{
			// VBOでエラーを起こしている
			_error = vertex_buffer_object;
			return false;
		}
		// VAO作成
		if (!_vao.initialize(_vbo, layouts, lcount))
		{
			_error = vertex_array_object;
			return false;
		}
		return true;
	}
	bool initialize(const vertex_type* vertices, int vcount, const vertex_layout_index* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
	{
		finalize();

		if (!_vbo.initialize(vertices, vcount, usg))
		{
			_error = vertex_buffer_object;
			return false;
		}
		if (!_vao.initialize(_vbo, layouts, lcount))
		{
			_error = vertex_array_object;
			return false;
		}
		return true;
	}

	// vertex_layout用初期化関数定義
	__POCKET_LAYERED_VERTEX_BUFFER_INITIALIZE(vertex_layout);
	// vertex_layout_index用初期化関数定義
	__POCKET_LAYERED_VERTEX_BUFFER_INITIALIZE(vertex_layout_index);

	bool initialize(int vcount, const vertex_layout* layouts, int lcount)
	{
		finalize();

		// 容量だけ確保
		if (!_vbo.initialize(vcount))
		{
			_error = vertex_buffer_object;
			return false;
		}

		if (!_vao.initialize(_vbo, layouts, lcount))
		{
			_error = vertex_array_object;
			return false;
		}
		return true;
	}
	template <int N>
	bool initialize(int count, const vertex_layout(&layouts)[N])
	{
		return initialize(count, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(int count, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(count, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(int count, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(count, &layouts[0], layouts.size());
	}
	bool initialize(int vcount, const vertex_layout_index* layouts, int lcount)
	{
		finalize();

		if (!_vbo.initialize(vcount))
		{
			_error = vertex_buffer_object;
			return false;
		}
		if (!_vao.initialize(_vbo, layouts, lcount))
		{
			_error = vertex_array_object;
			return false;
		}
		return true;
	}
	template <int N>
	bool initialize(int count, const vertex_layout_index(&layouts)[N])
	{
		return initialize(count, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(int count, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(count, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(int count, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(count, &layouts[0], layouts.size());
	}

	// 終了処理
	void finalize()
	{
		_vbo.finalize();
		_vao.finalize();
		_error = none;
	}

	// エラー状態クリア
	void clear()
	{
		_vbo.clear();
		_vao.clear();
		_error = none;
	}

	// バインド
	void bind() const
	{
		_vao.bind();
	}

	// バインド解除
	void unbind() const
	{
		_vao.unbind();
	}

	// バインドされているか
	bool binding() const
	{
		return _vao.binding();
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}

	// 描画
	void draw(draw_type_t type) const
	{
		_vbo.draw(type);
	}
	void draw(draw_type_t type, GLint offset) const
	{
		_vbo.draw(type, offset);
	}
	void draw(draw_type_t type, GLint first, GLsizei n) const
	{
		_vbo.draw(type, first, n);
	}
	void draw(draw_type_t type, const draw_indirect_buffer& i) const
	{
		_vbo.draw(type, i);
	}

	// バッファを展開して先頭アドレスを取得
	vertex_type* map(buffer_map_t type) const
	{
		return _vbo.map(type);
	}
	vertex_type* map_binding(buffer_map_t type) const
	{
		return _vbo.map_binding(type);
	}

	// 展開してアドレスを取得できていたら渡された関数を実行
	template <typename F>
	bool map(buffer_map_t type, F func) const
	{
		buffer::binder_type lock(_vbo);
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
		_vbo.unmap();
	}
	void unmap_binding() const
	{
		_vbo.unmap_binding();
	}

	// 展開を管理するオブジェクト生成
	binder_map_type make_binder_map(buffer_map_t type) const
	{
		return binder_map_type(_vbo, type);
	}

	// ストリーミングへのバインド
	void stream(GLuint point) const
	{
		_vbo.stream(point);
	}

	// 頂点数
	int count() const
	{
		return _vbo.count();
	}

	// エラー分
	std::string error() const
	{
		if (_error == vertex_buffer_object)
		{
			return "vbo: " + _vbo.error();
		}
		if (_error == vertex_array_object)
		{
			return "vao: " + _vao.error();
		}
		// エラーは起きていない
		return "";
	}

	// エラーの状態を確認
	bool error_status(error_object_type type) const
	{
		return _error == type;
	}
	bool error_status(error_object_type type, error_bitfield bit) const
	{
		if (type == vertex_buffer_object)
		{
			return _vbo.error_status(bit);
		}
		if (type == vertex_array_object)
		{
			return _vao.error_status(bit);
		}
		return false;
	}

	// 有効な状態か
	bool valid() const
	{
		return _error == none;
	}

	// VBOの取得
	const vertex_buffer<T>& vbo() const
	{
		return _vbo;
	}
	const vertex_array& vao() const
	{
		return _vao;
	}

	// デフォルトではVBOを取得させる
	GLuint& get()
	{
		return _vbo.get();
	}
	const GLuint& get() const
	{
		return _vbo.get();
	}
	GLuint& get_vbo()
	{
		return _vbo.get();
	}
	const GLuint& get_vbo() const
	{
		return _vbo.get();
	}
	GLuint& get_vao()
	{
		return _vao.get();
	}
	const GLuint& get_vao() const
	{
		return _vao.get();
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

	bool operator == (const layered_vertex_buffer& b) const
	{
		return _vbo == b._vbo && _vao == b._vao;
	}
	bool operator != (const layered_vertex_buffer& b) const
	{
		return !(*this == b);
	}

	layered_vertex_buffer& operator = (const layered_vertex_buffer& b)
	{
		_vbo = b._vbo;
		_vao = b._vao;
		_error = b._error;
		return *this;
	}
#ifdef POCKET_USE_CXX11
	layered_vertex_buffer& operator = (layered_vertex_buffer&& b)
	{
		_vbo = std::move(b._vbo);
		_vao = std::move(b._vao);
		_error = std::move(b._error);

		b._error = none;
		return *this;
	}

	layered_vertex_buffer& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // POCKET_USE_CXX11
};

#undef __POCKET_LAYERED_VERTEX_BUFFER_INITIALIZE

template <typename T> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const T* vertices, int vcount, const vertex_layout* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, vcount, layouts, lcount, usg);
}
template <typename T> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const T* vertices, int vcount, const vertex_layout_index* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, vcount, layouts, lcount, usg);
}
template <typename T, int N> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const T(&vertices)[N], const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, count, usg);
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const ARRAY<T, N>& vertices, const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, count, usg);
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const VECTOR<T, ALLOC>& vertices, const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, count, usg);
}
template <typename T, typename U> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const T* vertices, int count, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, count, layouts, usg);
}
template <typename T, int N, typename U> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const T(&vertices)[N], const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, usg);
}
template <typename T, size_t N, typename U, template <typename, size_t> class ARRAY> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const ARRAY<T, N>& vertices, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, usg);
}
template <typename T, typename ALLOC, typename U, template <typename, typename> class VECTOR> inline
layered_vertex_buffer<T> make_layered_vertex_buffer(const VECTOR<T, ALLOC>& vertices, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	return layered_vertex_buffer<T>(vertices, layouts, usg);
}

template <typename T> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const T* vertices, int vcount, const vertex_layout* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, vcount, layouts, lcount, usg);
	return b;
}
template <typename T> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const T* vertices, int vcount, const vertex_layout_index* layouts, int lcount, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, vcount, layouts, lcount, usg);
	return b;
}
template <typename T, int N> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const T(&vertices)[N], const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, count, usg);
	return b;
}
template <typename T, size_t N, template <typename, size_t> class ARRAY> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const ARRAY<T, N>& vertices, const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, count, usg);
	return b;
}
template <typename T, typename ALLOC, template <typename, typename> class VECTOR> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const VECTOR<T, ALLOC>& vertices, const vertex_layout* layouts, int count, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, count, usg);
	return b;
}
template <typename T, typename U> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const T* vertices, int count, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, count, layouts, usg);
	return b;
}
template <typename T, typename U, typename V>
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const U& vertices, const V& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, usg);
	return b;
}
template <typename T, int N, typename U> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const T(&vertices)[N], const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, usg);
	return b;
}
template <typename T, size_t N, typename U, template <typename, size_t> class ARRAY> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const ARRAY<T, N>& vertices, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, usg);
	return b;
}
template <typename T, typename ALLOC, typename U, template <typename, typename> class VECTOR> inline
layered_vertex_buffer<T>& make_layered_vertex_buffer(layered_vertex_buffer<T>& b, const VECTOR<T, ALLOC>& vertices, const U& layouts, buffer_usage_t usg = buffer_usage::immutable_draw)
{
	b.initialize(vertices, layouts, usg);
	return b;
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const layered_vertex_buffer<T>& v)
{
	const vertex_array& vao = v.vao();
	const vertex_buffer<T>& vbo = v.vbo();

	os << io::widen("layered_vertex_buffer: {") << std::endl <<
		io::tab << io::widen("vao: {") << std::endl <<
		io::tab2 << io::widen("id: ") << vao.get() << std::endl;
	if (vao.binding())
	{
		os << io::tab2 << io::widen("enable: {") << std::endl;
		GLint n = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		for (int i = 0; i < n; ++i)
		{
			if (vao.enabled(i))
			{
				os << io::tab3 << i << io::colon << io::space << io::box_brackets_left <<
					io::widen("count: ") << vao.count(i) << io::comma_space;
				std::ios_base::fmtflags flag = os.flags();
				os << std::hex << io::widen("type: 0x") << vao.type(i) << io::comma_space;
				os.flags(flag);
				os << io::widen("size: ") << vao.size(i) << io::box_brackets_right << std::endl;
			}
		}
		os << io::tab2 << io::braces_right << std::endl;
	}
	os << io::tab << io::braces_right << std::endl;

	os << io::tab << io::widen("vbo: {") << std::endl <<
		io::tab2 << io::widen("id: ") << vbo.get() << std::endl <<
		io::tab2 << io::widen("type size: ") << sizeof(T) << std::endl <<
		io::tab2 << io::widen("count: ") << vbo.count() << std::endl <<
		io::tab2 << io::widen("size: ") << (vbo.count() * sizeof(T)) << std::endl;
	if (vbo.binding())
	{
		std::ios_base::fmtflags flag = os.flags();
		os << std::hex << io::tab2 << io::widen("usage: 0x") << vbo.usage_binding() << std::endl;
		os.flags(flag);
		os << io::tab2 << io::widen("writable: ") << io::widen(vbo.writable_binding()) << std::endl;
	}
	os << io::tab << io::braces_right << std::endl;
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

#endif // __POCKET_GL_LAYERED_VERTEX_BUFFER_H__
