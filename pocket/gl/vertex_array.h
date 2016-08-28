#ifndef __POCKET_GL_VERTEX_ARRAY_H__
#define __POCKET_GL_VERTEX_ARRAY_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"
#include "../container/array.h"
#include "buffer.h"

namespace pocket
{
namespace gl
{

// forward
class vertex_array;

struct vertex_layout
{
	int count;
	int type;
	int normalized;
	int offset;
};
struct vertex_layout_index
{
	int index;
	int count;
	int type;
	int normalized;
	int offset;
};

#define __POCKET_BUFFER_OFFSET(OFFSET) (static_cast<const void*>(reinterpret_cast<const char*>(0)+(OFFSET)))

class vertex_array
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

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

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLuint _id;
	int _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	vertex_array() :
		_id(0),
		_error_bitfield(0)
	{}
	explicit vertex_array(GLuint id) :
		_id(id),
		_error_bitfield(0)
	{}
	~vertex_array()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout* layouts, int count)
	{
		finalize();

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
				stride, __POCKET_BUFFER_OFFSET(layout.offset));
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return true;
	}
	template <int N>
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout(&layouts)[N])
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
	bool initialize(GLuint vbo, const vertex_layout(&layouts)[N])
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
				stride, __POCKET_BUFFER_OFFSET(layout.offset));
		}

		vbo.unbind();
		glBindVertexArray(0);

		return true;
	}
	template <int N>
	bool initialize(const buffer& vbo, GLuint stride, const vertex_layout(&layouts)[N])
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
	bool initialize(const buffer& vbo, const vertex_layout(&layouts)[N])
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

	// インデックス指定の初期化
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout_index* layouts, int count)
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
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for (int i = 0; i < count; ++i)
		{
			const vertex_layout_index& layout = layouts[i];
			glEnableVertexAttribArray(layout.index);
			glVertexAttribPointer(layout.index, layout.count, layout.type, static_cast<GLboolean>(layout.normalized),
				stride, __POCKET_BUFFER_OFFSET(layout.offset));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}
	template <int N>
	bool initialize(GLuint vbo, GLuint stride, const vertex_layout_index(&layouts)[N])
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
	bool initialize(GLuint vbo, const vertex_layout_index(&layouts)[N])
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
				stride, __POCKET_BUFFER_OFFSET(layout.offset));
		}

		vbo.unbind();
		glBindVertexArray(0);

		return true;
	}
	template <int N>
	bool initialize(const buffer& vbo, GLuint stride, const vertex_layout_index(&layouts)[N])
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
	bool initialize(const buffer& vbo, const vertex_layout_index(&layouts)[N])
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
		// バインド出来る最大数
		GLint n;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		// 無効になっている数
		GLint disable = 0;
		for (int i = 0; i < n; ++i)
		{
			// 無効になっている場合は次
			GLint enable;
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enable);
			if (enable == GL_FALSE)
			{
				++disable;
				continue;
			}
			// バインドされているか
			GLint binded;
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &binded);
			// 有効になっている状態でバインドされていない
			if (binded == GL_FALSE)
			{
				return false;
			}
		}
		// 全てが無効になっていないか
		return n > disable;
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

	// バインド状態を管理するオブジェクト作成
	binder_type make_binder() const
	{
		return binder_type(*this);
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
#ifdef _USE_CXX11
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
#endif // _USE_CXX11
};

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const vertex_array& v)
{
	os << io::widen("vertex_array: ") << io::braces_left << std::endl <<
		io::tab << io::widen("id: ") << v.get() << std::endl;
	GLint n;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
	for (int i = 0; i < n; ++i)
	{
		const char* s = v.enabled(i) ? "true" : "fales";
		os << io::tab << io::widen("index") << i << io::widen(": ") << io::widen(s) << std::endl;
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
