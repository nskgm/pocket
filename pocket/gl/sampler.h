#ifndef __POCKET_GL_SAMPLER_H__
#define __POCKET_GL_SAMPLER_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "gl.h"
#include "../io.h"
#include <algorithm>

namespace pocket
{
namespace gl
{

// forward
class sampler;

class sampler
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef binder1<sampler, GLuint> binder_type;

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

	sampler() :
		_id(0), _error_bitfield(0)
	{}
	~sampler()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize(wrap_type_t s, wrap_type_t t, wrap_type_t r, filter_type_t min, filter_type_t mag, compare_func_type_t com, int lodmin = 0, int lodmax = 0)
	{
		finalize();

		glGenSamplers(1, &_id);
		if (_id == 0)
		{
			_error_bitfield |= error_creating;
			return false;
		}

		// テクスチャのラップ法
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, s);
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, t);
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, r);
		// テクスチャのフィルタリング法
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, min);
		glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, mag);

		// 比較法
		glSamplerParameteri(_id, GL_TEXTURE_COMPARE_FUNC, com);

		// LODの設定
		if (lodmin != 0)
		{
			// clamp
			lodmin = std::min(0, std::max(lodmin, -1000));
			glSamplerParameteri(_id, GL_TEXTURE_MIN_LOD, lodmin);
		}
		if (lodmax != 0)
		{
			lodmax = std::min(1000, std::max(lodmax, 0));
			glSamplerParameteri(_id, GL_TEXTURE_MAX_LOD, lodmax);
		}

		return true;
	}
	bool initialize(wrap_type_t w, filter_type_t f, compare_func_type_t com, int lodmin = 0, int lodmax = 0)
	{
		// すべて同じ設定で作成
		return initialize(w, w, w, f, f, com, lodmin, lodmax);
	}

	// 終了処理
	void finalize()
	{
		if (_id != 0)
		{
			glDeleteSamplers(1, &_id);
			_id = 0;
		}
		_error_bitfield = 0;
	}

	// バインド
	void bind(GLuint point) const
	{
		glBindSampler(point, _id);
	}
	// バインド解除
	void unbind(GLuint point) const
	{
		glBindSampler(point, 0);
	}
	// バインドされているか
	bool binding() const
	{
		GLuint i;
		glGetIntegerv(GL_SAMPLER_BINDING, reinterpret_cast<GLint*>(&i));
		// バインドされていない
		if (i == 0)
		{
			return false;
		}
		// 現在バインドされているIDが同じか
		return i == _id;
	}

	// バインド状態を管理するオブジェクト生成
	binder_type make_binder(GLuint p) const
	{
		return binder_type(*this, p);
	}

	// ラップ法を設定・取得
	void wrap_s(wrap_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, type);
	}
	wrap_type_t wrap_s() const
	{
		GLint s = 0;
		glGetSamplerParameteriv(_id, GL_TEXTURE_WRAP_S, &s);
		return static_cast<wrap_type_t>(s);
	}

	void wrap_t(wrap_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, type);
	}
	wrap_type_t wrap_t() const
	{
		GLint t = 0;
		glGetSamplerParameteriv(_id, GL_TEXTURE_WRAP_T, &t);
		return static_cast<wrap_type_t>(t);
	}

	void wrap_r(wrap_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, type);
	}
	wrap_type_t wrap_r() const
	{
		GLint r = 0;
		glGetSamplerParameteriv(_id, GL_TEXTURE_WRAP_R, &r);
		return static_cast<wrap_type_t>(r);
	}

	// ラップ法をすべて同じに設定
	void wrap(wrap_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, type);
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, type);
		glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, type);
	}

	// フィルタリング法を設定・取得
	void filter_min(filter_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, type);
	}
	filter_type_t filter_min() const
	{
		GLint f = 0;
		glGetSamplerParameteriv(_id, GL_TEXTURE_MIN_FILTER, &f);
		return static_cast<filter_type_t>(f);
	}

	void filter_mag(filter_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, type);
	}
	filter_type_t filter_mag() const
	{
		GLint f = 0;
		glGetSamplerParameteriv(_id, GL_TEXTURE_MAG_FILTER, &f);
		return static_cast<filter_type_t>(f);
	}

	// フィルタリングを全て同じに設定
	void filter(filter_type_t type) const
	{
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, type);
		glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, type);
	}

	// エラー文
	std::string error() const
	{
		// シェーダーハンドルの作成に失敗している
		if (error_status(error_creating))
		{
			return "failed. glGenSamplers().";
		}
		// 作成されていない
		// またはすでに破棄済み
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
		return glIsSampler(_id) == GL_TRUE;
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

	POCKET_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const sampler& s) const
	{
		return _id == s._id;
	}
	bool operator != (const sampler& s) const
	{
		return !(*this == s);
	}

	sampler& operator = (const sampler& s)
	{
		_id = s._id;
		_error_bitfield = s._error_bitfield;
		return *this;
	}
#ifdef POCKET_USE_CXX11
	sampler& operator = (sampler&& s)
	{
		_id = std::move(s._id);
		_error_bitfield = std::move(s._error_bitfield);
		s._id = 0;
		s._error_bitfield = 0;
		return *this;
	}

	sampler& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // POCKET_USE_CXX11
};

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const sampler& v)
{
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, sampler& v)
{
	return is;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_SAMPLER_H__
