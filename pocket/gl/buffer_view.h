#ifndef __POCKET_BUFFER_VIEW_H__
#define __POCKET_BUFFER_VIEW_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "buffer.h"

namespace pocket
{
namespace gl
{

// forward
class buffer_view;

// 現在のバインドされているバッファを取得
GLuint get_binding_buffer(buffer_type_t);
bool get_binding_buffer(buffer_type_t, GLuint&, GLenum&);
buffer_view get_binding_buffer_view(buffer_type_t);
bool get_binding_buffer_view(buffer_type_t, buffer_view&);

template <>
class binder<buffer_view>
{
public:
	explicit binder(const buffer_view& a);
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

	binder_map<buffer_view, void> make_binder_map(buffer_map_t) const;
	template <buffer_map_t U> binder_map<buffer_view, void> make_binder_map() const;
	template <typename T> binder_map<buffer_view, T> make_binder_map(buffer_map_t) const;
	template <typename T, buffer_map_t U> binder_map<buffer_view, T> make_binder_map() const;

	bool binding() const
	{
		return true;
	}

	const buffer_view* operator -> () const
	{
		return _address;
	}
	const buffer_view& operator * () const
	{
		return *_address;
	}

	operator const buffer_view& () const
	{
		return *_address;
	}
	operator const buffer_view* () const
	{
		return _address;
	}

private:
	const buffer_view* _address;
};

class buffer_view
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	friend bool get_binding_buffer_view(buffer_type_t, buffer_view&);

	typedef binder<buffer_view> binder_type;
	typedef binder_map<buffer_view, void> binder_map_type;

	template <typename U>
	struct rebinder1
	{
		typedef binder1<buffer_view, U> type;
	};
	template <typename U>
	struct rebinder_map
	{
		typedef binder_map<buffer_view, U> type;
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	buffer_type_t _type;
	GLuint _id;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	explicit buffer_view(buffer_type_t type, GLuint id) :
		_type(type), _id(id)
	{}
	explicit buffer_view(const buffer& b) :
		_type(b._type), _id(b._id)
	{}
	buffer_view(const buffer_view& b) :
		_type(b._type), _id(b._id)
	{}
#ifdef _USE_CXX11
	buffer_view(buffer_view&& b) :
		_type(std::move(b._type)),
		_id(std::move(b._id))
	{
		b._id = 0;
		b._type = buffer_type::unknown;
	}
#endif // _USE_CXX11
	~buffer_view()
	{}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

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
	void* map_binding(buffer_map_t type) const
	{
		return glMapBuffer(_type, type);
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
	binder_map_type make_binder_map(buffer_map_t usg) const
	{
		return binder_map_type(*this, usg);
	}
	template <buffer_map_t U>
	binder_map_type make_binder_map() const
	{
		return binder_map_type(*this, U);
	}
	template <typename T>
	typename rebinder_map<T>::type make_binder_map(buffer_map_t usg) const
	{
		return typename rebinder_map<T>::type(*this, usg);
	}
	template <typename T, buffer_map_t U>
	typename rebinder_map<T>::type make_binder_map() const
	{
		return typename rebinder_map<T>::type(*this, U);
	}

	binder_map_type make_binder_map(const binder_type& a, buffer_map_t usg) const
	{
		return binder_map_type(a, usg);
	}
	template <buffer_map_t U>
	binder_map_type make_binder_map(const binder_type& a) const
	{
		return binder_map_type(a, U);
	}
	template <typename T>
	typename rebinder_map<T>::type make_binder_map(const binder_type& a, buffer_map_t usg) const
	{
		return typename rebinder_map<T>::type(a, usg);
	}
	template <typename T, buffer_map_t U>
	typename rebinder_map<T>::type make_binder_map(const binder_type& a) const
	{
		return typename rebinder_map<T>::type(a, U);
	}

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

	// エラーの状態
	std::string error() const
	{
		if (_type == buffer_type::unknown)
		{
			return "failed. buffer type unknown.";
		}
		if (_id == 0)
		{
			return "failed. buffer not binding.";
		}
		return "";
	}

	// 有効な状態か
	bool valid() const
	{
		return _id != 0 && _type != buffer_type::unknown;
	}

	// バッファ種類
	buffer_type_t kind() const
	{
		return _type;
	}

	// バッファ種類の比較
	bool kind_of(buffer_type_t type) const
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

	// クローン
	buffer_view clone() const
	{
		return buffer_view(_type, _id);
	}
	bool clone(buffer_view& b) const
	{
		b._type = _type;
		b._id = _id;
		return true;
	}

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

	bool operator == (const buffer_view& b) const
	{
		return _id == b._id && _type == b._type;
	}
	bool operator != (const buffer_view& b) const
	{
		return !(*this == b);
	}

	buffer_view& operator = (const buffer_view& b)
	{
		_type = b._type;
		_id = b._id;
		return *this;
	}
#ifdef _USE_CXX11
	buffer_view& operator = (buffer_view&& b)
	{
		_type = std::move(b._type);
		_id = std::move(b._id);
		b._type = buffer_type::unknown;
		b._id = 0;
		return *this;
	}

	buffer_view& operator = (std::nullptr_t)
	{
		_type = buffer_type::unknown;
		_id = 0;
		return *this;
	}
#endif // _USE_CXX11
};

inline
binder<buffer_view>::binder(const buffer_view& a) :
	_address(&a)
{
	a.bind();
}
inline
binder<buffer_view>::~binder()
{
	_address->unbind();
}
inline
int binder<buffer_view>::size() const
{
	return _address->size_binding();
}
inline
int binder<buffer_view>::count(int type_size) const
{
	return _address->count_binding(type_size);
}
template <typename T> inline
int binder<buffer_view>::count() const
{
	return _address->count_binding<T>();
}
inline
buffer_usage_t binder<buffer_view>::usage() const
{
	return _address->usage_binding();
}
inline
void* binder<buffer_view>::map(buffer_map_t usg) const
{
	return _address->map_binding(usg);
}
template <typename T> inline
T* binder<buffer_view>::map(buffer_map_t usg) const
{
	return _address->map_binding<T>(usg);
}
template <typename F> inline
bool binder<buffer_view>::map(buffer_map_t usg, F func) const
{
	return _address->map_binding(usg, func);
}
template <typename T, typename F> inline
bool binder<buffer_view>::map(buffer_map_t usg, F func) const
{
	return _address->map_binding<T>(usg, func);
}
inline
void binder<buffer_view>::unmap() const
{
	_address->unmap_binding();
}
inline
binder_map<buffer_view, void> binder<buffer_view>::make_binder_map(buffer_map_t type) const
{
	return binder_map<buffer_view, void>(*this, type);
}
template <buffer_map_t U> inline
binder_map<buffer_view, void> binder<buffer_view>::make_binder_map() const
{
	return binder_map<buffer_view, void>(*this, U);
}
template <typename T> inline
binder_map<buffer_view, T> binder<buffer_view>::make_binder_map(buffer_map_t type) const
{
	return binder_map<buffer_view, T>(*this, type);
}
template <typename T, buffer_map_t U> inline
binder_map<buffer_view, T> binder<buffer_view>::make_binder_map() const
{
	return binder_map<buffer_view, T>(*this, U);
}

// 現在バインドされているバッファを取得
inline
GLuint get_binding_buffer(buffer_type_t type)
{
	buffer_binding_type_t binding_type = buffer_type::to_binding_type(type);
	GLint i = 0;
	glGetIntegerv(binding_type, &i);
	return static_cast<GLuint>(i);
}
inline
bool get_binding_buffer(buffer_type_t type, GLuint& id, GLenum& bind_type)
{
	buffer_binding_type_t binding_type = buffer_type::to_binding_type(type);
	bind_type = type;
	id = 0;
	glGetIntegerv(binding_type, reinterpret_cast<GLint*>(&id));
	return id != 0;
}
inline
buffer_view get_binding_buffer_view(buffer_type_t type)
{
	buffer_binding_type_t binding_type = buffer_type::to_binding_type(type);
	GLint i = 0;
	glGetIntegerv(binding_type, &i);
	return buffer_view(type, static_cast<GLuint>(i));
}
inline
bool get_binding_buffer_view(buffer_type_t type, buffer_view& view)
{
	buffer_binding_type_t binding_type = buffer_type::to_binding_type(type);
	view._type = type;
	view._id = 0;
	glGetIntegerv(binding_type, reinterpret_cast<GLint*>(&view._id));
	return view._id != 0;
}

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const buffer_view& v)
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

	os << io::widen("buffer_view: ") << io::braces_left << std::endl <<
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

#endif // __POCKET_BUFFER_VIEW_H__
