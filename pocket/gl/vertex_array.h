#ifndef __POCKET_GL_VERTEX_ARRAY_H__
#define __POCKET_GL_VERTEX_ARRAY_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "../container/array.h"
#include "buffer.h"
#include "vertex_buffer.h"
#include "common_type.h"

namespace pocket
{
namespace gl
{

// forward
class vertex_array;

//------------------------------
// レイアウト指定用構造体
//------------------------------
struct vertex_layout
{
	GLenum type; // 要素の型
	int count; // 要素数
	int normalized; // 正規化するか
	int offset; // 構造体のオフセット
};

#ifndef POCKET_LAYOUT
#	define POCKET_LAYOUT(TYPE, COUNT, NORMALIZED, OFFSET) {\
	pocket::gl::gl_type<TYPE>::value,\
	(COUNT),\
	static_cast<int>(pocket::gl::gl_bool<NORMALIZED>::value),\
	(OFFSET)\
	}
#endif // POCKET_LAYOUT
#ifndef POCKET_LAYOUT_OFFSETOF
#	define POCKET_LAYOUT_OFFSETOF(TYPE, COUNT, NORMALIZED, VERTEX_TYPE, MEM) {\
	pocket::gl::gl_type<TYPE>::value,\
	(COUNT),\
	static_cast<int>(pocket::gl::gl_bool<NORMALIZED>::value),\
	static_cast<int>(POCKET_MEMBER_OFFSETOF(VERTEX_TYPE, MEM))\
	}
#endif // POCKET_LAYOUT_OFFSETOF

//------------------------------
// レイアウト指定用構造体（インデックス指定）
//------------------------------
struct vertex_layout_index
{
	int index; // 有効にするインデックス
	GLenum type;
	int count;
	int normalized;
	int offset;
};

#ifndef POCKET_LAYOUT_INDEX
#	define POCKET_LAYOUT_INDEX(INDEX, TYPE, COUNT, NORMALIZED, OFFSET) {\
	(INDEX),\
	pocket::gl::gl_type<TYPE>::value,\
	(COUNT),\
	static_cast<int>(pocket::gl::gl_bool<NORMALIZED>::value),\
	(OFFSET)\
	}
#endif // POCKET_LAYOUT_INDEX
#ifndef POCKET_LAYOUT_INDEX_OFFSETOF
#	define POCKET_LAYOUT_INDEX_OFFSETOF(INDEX, TYPE, COUNT, NORMALIZED, VERTEX_TYPE, MEM) {\
	(INDEX),\
	pocket::gl::gl_type<TYPE>::value,\
	(COUNT),\
	static_cast<int>(pocket::gl::gl_bool<NORMALIZED>::value),\
	static_cast<int>(POCKET_MEMBER_OFFSETOF(VERTEX_TYPE, MEM))\
	}
#endif // POCKET_LAYOUT_INDEX_OFFSETOF

class vertex_array
{
public:
	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef binder<vertex_array> binder_type;

	template <int N>
	struct layout_type
	{
		typedef container::array<vertex_layout, N> type;
	};
	template <int N>
	struct layout_index_type
	{
		typedef container::array<vertex_layout_index, N> type;
	};

	enum identifier_t
	{
		identifier = GL_VERTEX_ARRAY
	};

private:
	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

	GLuint _id;
	int _error_bitfield;

public:
	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	vertex_array() :
		_id(0),
		_error_bitfield(0)
	{}
	explicit vertex_array(GLuint id) :
		_id(id),
		_error_bitfield(0)
	{}
	explicit vertex_array(GLuint vbo, GLuint stride, const vertex_layout* layouts, int count) :
		_id(0)
	{
		initialize(vbo, stride, layouts, count);
	}
	explicit vertex_array(GLuint vbo, GLuint stride, const vertex_layout_index* layouts, int count) :
		_id(0)
	{
		initialize(vbo, stride, layouts, count);
	}
	template <int N>
	explicit vertex_array(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N)) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <int N>
	explicit vertex_array(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N)) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit vertex_array(GLuint vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit vertex_array(GLuint vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit vertex_array(GLuint vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit vertex_array(GLuint vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}

	explicit vertex_array(const buffer& vbo, GLuint stride, const vertex_layout* layouts, int count) :
		_id(0)
	{
		initialize(vbo, stride, layouts, count);
	}
	explicit vertex_array(const buffer& vbo, GLuint stride, const vertex_layout_index* layouts, int count) :
		_id(0)
	{
		initialize(vbo, stride, layouts, count);
	}
	template <int N>
	explicit vertex_array(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N)) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <int N>
	explicit vertex_array(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N)) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit vertex_array(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	explicit vertex_array(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit vertex_array(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	explicit vertex_array(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts) :
		_id(0)
	{
		initialize(vbo, stride, layouts);
	}
	vertex_array(const vertex_array& b) :
		_id(b._id),
		_error_bitfield(b._error_bitfield)
	{}
#ifdef POCKET_USE_CXX11
	vertex_array(vertex_array&& v) :
		_id(std::move(v._id)),
		_error_bitfield(std::move(v._error_bitfield))
	{
		v._id = 0;
		v._error_bitfield = 0;
	}
#endif // POCKET_USE_CXX11
	~vertex_array()
	{
		finalize();
	}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// 初期化
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout* layouts, int count)
	{
		finalize();

		// バッファか班別
		if (glIsBuffer(vbo) != GL_TRUE)
		{
			_error_bitfield |= error_unsupported;
			return false;
		}

		// 作成
		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		// attributeの設定を行なう
		glBindVertexArray(_id);

		if (glIsVertexArray(_id) == GL_FALSE)
		{
			// バインド出来る状態ではない
			_error_bitfield |= error_binding;
			return false;
		}

		// 設定するVBOIDのバインド
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// 頂点の情報を登録
		for (int i = 0; i < count; ++i)
		{
			const vertex_layout& layout = layouts[i];

			// 有効
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				stride, POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}
	template <int N>
	bool initialize(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(GLuint vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(GLuint vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], layouts.size());
	}
	template <typename VERTEX>
	bool initialize(GLuint vbo, const vertex_layout* layouts, int count)
	{
		return initialize(vbo, sizeof(VERTEX), layouts, count);
	}
	template <typename VERTEX, int N>
	bool initialize(GLuint vbo, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(GLuint vbo, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(GLuint vbo, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], layouts.size());
	}

	bool initialize(const buffer& vbo, GLuint stride, const vertex_layout* layouts, int count)
	{
		finalize();

		// 渡されたバッファがVBO用のバッファではない
		if (!vbo.kind_of(buffer_type::array))
		{
			_error_bitfield |= error_unsupported;
			return false;
		}

		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		glBindVertexArray(_id);
		if (glIsVertexArray(_id) == GL_FALSE)
		{
			_error_bitfield |= error_binding;
			return false;
		}
		vbo.bind();

		for (int i = 0; i < count; ++i)
		{
			const vertex_layout& layout = layouts[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				stride, POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindVertexArray(0);
		vbo.unbind();

		return true;
	}
	template <int N>
	bool initialize(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], layouts.size());
	}
	template <typename VERTEX>
	bool initialize(const buffer& vbo, const vertex_layout* layouts, int count)
	{
		return initialize(vbo, sizeof(VERTEX), layouts, count);
	}
	template <typename VERTEX, int N>
	bool initialize(const buffer& vbo, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const buffer& vbo, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const buffer& vbo, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], layouts.size());
	}

	template <typename T>
	bool initialize(const vertex_buffer<T>& vbo, const vertex_layout* layouts, int count)
	{
		finalize();

		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		glBindVertexArray(_id);
		if (glIsVertexArray(_id) == GL_FALSE)
		{
			_error_bitfield |= error_binding;
			return false;
		}
		vbo.bind();

		for (int i = 0; i < count; ++i)
		{
			const vertex_layout& layout = layouts[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				sizeof(T), POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindVertexArray(0);
		vbo.unbind();

		return true;
	}
	template <typename T, int N>
	bool initialize(const vertex_buffer<T>& vbo, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
	{
		return initialize(vbo, &layouts[0], N);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const vertex_buffer<T>& vbo, const ARRAY<vertex_layout, N>& layouts)
	{
		return initialize(vbo, &layouts[0], N);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const vertex_buffer<T>& vbo, const VECTOR<vertex_layout, ALLOC>& layouts)
	{
		return initialize(vbo, &layouts[0], layouts.size());
	}

	// インデックス指定の初期化
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout_index* layouts, int count)
	{
		finalize();

		if (glIsBuffer(vbo) != GL_TRUE)
		{
			_error_bitfield |= error_unsupported;
			return false;
		}

		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}
		glBindVertexArray(_id);
		if (glIsVertexArray(_id) == GL_FALSE)
		{
			_error_bitfield |= error_binding;
			return false;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for (int i = 0; i < count; ++i)
		{
			const vertex_layout_index& layout = layouts[i];
			glEnableVertexAttribArray(layout.index);
			glVertexAttribPointer(layout.index, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				stride, POCKET_BUFFER_OFFSET(layout.offset));
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
	template <int N>
	bool initialize(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(GLuint vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(GLuint vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], layouts.size());
	}
	template <typename VERTEX>
	bool initialize(GLuint vbo, const vertex_layout_index* layouts, int count)
	{
		return initialize(vbo, sizeof(VERTEX), layouts, count);
	}
	template <typename VERTEX, int N>
	bool initialize(GLuint vbo, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(GLuint vbo, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(GLuint vbo, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], layouts.size());
	}

	bool initialize(const buffer& vbo, GLuint stride, const vertex_layout_index* layouts, int count)
	{
		finalize();

		if (!vbo.kind_of(buffer_type::array))
		{
			_error_bitfield |= error_unsupported;
			return false;
		}
		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}
		glBindVertexArray(_id);
		if (glIsVertexArray(_id) == GL_FALSE)
		{
			_error_bitfield |= error_binding;
			return false;
		}
		vbo.bind();

		for (int i = 0; i < count; ++i)
		{
			const vertex_layout_index& layout = layouts[i];
			glEnableVertexAttribArray(layout.index);
			glVertexAttribPointer(layout.index, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				stride, POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindVertexArray(0);
		vbo.unbind();

		return true;
	}
	template <int N>
	bool initialize(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], N);
	}
	template <typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(vbo, stride, &layouts[0], layouts.size());
	}
	template <typename VERTEX>
	bool initialize(const buffer& vbo, const vertex_layout_index* layouts, int count)
	{
		return initialize(vbo, sizeof(VERTEX), layouts, count);
	}
	template <typename VERTEX, int N>
	bool initialize(const buffer& vbo, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const buffer& vbo, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], N);
	}
	template <typename VERTEX, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const buffer& vbo, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(vbo, sizeof(VERTEX), &layouts[0], layouts.size());
	}
	template <typename T>
	bool initialize(const vertex_buffer<T>& vbo, const vertex_layout_index* layouts, int count)
	{
		finalize();

		glGenVertexArrays(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		glBindVertexArray(_id);
		if (glIsVertexArray(_id) == GL_FALSE)
		{
			_error_bitfield |= error_binding;
			return false;
		}
		vbo.bind();

		for (int i = 0; i < count; ++i)
		{
			const vertex_layout_index& layout = layouts[i];
			glEnableVertexAttribArray(layout.index);
			glVertexAttribPointer(layout.index, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				sizeof(T), POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindVertexArray(0);
		vbo.unbind();

		return true;
	}
	template <typename T, int N>
	bool initialize(const vertex_buffer<T>& vbo, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
	{
		return initialize(vbo, &layouts[0], N);
	}
	template <typename T, size_t N, template <typename, size_t> class ARRAY>
	bool initialize(const vertex_buffer<T>& vbo, const ARRAY<vertex_layout_index, N>& layouts)
	{
		return initialize(vbo, &layouts[0], N);
	}
	template <typename T, typename ALLOC, template <typename, typename> class VECTOR>
	bool initialize(const vertex_buffer<T>& vbo, const VECTOR<vertex_layout_index, ALLOC>& layouts)
	{
		return initialize(vbo, &layouts[0], layouts.size());
	}

	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteVertexArrays(1, &_id);
			_id = 0;
		}
		_error_bitfield = 0;
	}

	// エラー状態クリア
	void clear()
	{
		_error_bitfield = 0;
	}

	// バインド
	void bind() const
	{
		glBindVertexArray(_id);
	}

	// バインド解除
	void unbind() const
	{
		glBindVertexArray(0);
	}

	// バインドされているか
	bool binding() const
	{
		return gl::is_binding(GL_VERTEX_ARRAY_BINDING, _id);
	}

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
	}

	// インデックスが有効になっているか
	bool enabled(int i) const
	{
		// 無効になっている場合は次
		GLint enable;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enable);
		return enable == GL_TRUE;
	}
	// 無効になっているか
	bool disabled(int i) const
	{
		return !enabled(i);
	}

	// 要素数
	int count(int i) const
	{
		GLint n;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &n);
		return static_cast<int>(n);
	}

	// サイズ
	int size(int i) const
	{
		return count(i) * gl::get_type_size(type(i));
	}

	// バインドされているか
	bool binding(int i) const
	{
		GLint n;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &n);
		return n != 0;
	}

	// バインドされているか
	GLenum type(int i) const
	{
		GLenum type;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, reinterpret_cast<GLint*>(&type));
		return type;
	}

	// エラー文
	std::string error() const
	{
		// IDの作成に失敗している
		if (error_status(error_creating))
		{
			return "failed. glGenVertexArrays().";
		}
		if (error_status(error_binding))
		{
			return "failed. can not bind.";
		}
		// VBOではないバッファを渡された
		if (error_status(error_unsupported))
		{
			return "failed. not vertex buffer object.";
		}
		// 作成されていない またはすでに破棄済み
		if (_id == 0)
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
		return glIsVertexArray(_id) == GL_TRUE;
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

	bool operator == (const vertex_array& b) const
	{
		return _id == b._id;
	}
	bool operator != (const vertex_array& b) const
	{
		return !(*this == b);
	}

	vertex_array& operator = (const vertex_array& b)
	{
		_id = b._id;
		_error_bitfield = b._error_bitfield;
		return *this;
	}
#ifdef POCKET_USE_CXX11
	vertex_array& operator = (vertex_array&& b)
	{
		_id = std::move(b._id);
		_error_bitfield = std::move(b._error_bitfield);
		b._id = 0;
		b._error_bitfield = 0;
		return *this;
	}

	vertex_array& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // POCKET_USE_CXX11
};

inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const vertex_layout* layouts, int count)
{
	return vertex_array(vbo, stride, layouts, count);
}
inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const vertex_layout_index* layouts, int count)
{
	return vertex_array(vbo, stride, layouts, count);
}
template <int N> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
{
	return vertex_array(vbo, stride, layouts);
}
template <int N> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
{
	return vertex_array(vbo, stride, layouts);
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array make_vertex_array(GLuint vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}

inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const vertex_layout* layouts, int count)
{
	return vertex_array(vbo, stride, layouts, count);
}
inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const vertex_layout_index* layouts, int count)
{
	return vertex_array(vbo, stride, layouts, count);
}
template <int N> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
{
	return vertex_array(vbo, stride, layouts);
}
template <int N> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
{
	return vertex_array(vbo, stride, layouts);
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array make_vertex_array(const buffer& vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
{
	return vertex_array(vbo, stride, layouts);
}

// ------------------------------------
inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const vertex_layout* layouts, int count)
{
	a.initialize(vbo, stride, layouts, count);
	return a;
}
inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const vertex_layout_index* layouts, int count)
{
	a.initialize(vbo, stride, layouts, count);
	return a;
}
template <int N> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <int N> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array& make_vertex_array(vertex_array& a, GLuint vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}

inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const vertex_layout* layouts, int count)
{
	a.initialize(vbo, stride, layouts, count);
	return a;
}
inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const vertex_layout_index* layouts, int count)
{
	a.initialize(vbo, stride, layouts, count);
	return a;
}
template <int N> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout, layouts, N))
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <int N> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, POCKET_CREF_ARRAY_ARG(vertex_layout_index, layouts, N))
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const ARRAY<vertex_layout, N>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <size_t N, template <typename, size_t> class ARRAY> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const ARRAY<vertex_layout_index, N>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const VECTOR<vertex_layout, ALLOC>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}
template <typename ALLOC, template <typename, typename> class VECTOR> inline
vertex_array& make_vertex_array(vertex_array& a, const buffer& vbo, GLuint stride, const VECTOR<vertex_layout_index, ALLOC>& layouts)
{
	a.initialize(vbo, stride, layouts);
	return a;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vertex_array& v)
{
	os << io::widen("vertex_array: {") << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl;
	// バインド状態だったら有効状態を表示
	if (v.binding())
	{
		os << io::tab << io::widen("enable: {") << std::endl;
		GLint n = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		for (int i = 0; i < n; ++i)
		{
			if (v.enabled(i))
			{
				os << io::tab2 << i << io::colon << io::space << io::box_brackets_left <<
					io::widen("count: ") << v.count(i) << io::comma_space;
				std::ios_base::fmtflags flag = os.flags();
				os << std::hex << io::widen("type: 0x") << v.type(i) << io::comma_space;
				os.flags(flag);
				os << io::widen("size: ") << v.size(i) << io::box_brackets_right << std::endl;
			}
		}
		os << io::tab << io::braces_right << std::endl;
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

#endif // __POCKET_GL_VERTEX_ARRAY_H__
