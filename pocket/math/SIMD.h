#ifndef __MATH_SIMD_H__
#define __MATH_SIMD_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{
template <typename> struct simd_t;

/*---------------------------------------------------------------------------------------
* 簡易作成関数
*---------------------------------------------------------------------------------------*/
template <typename T> inline simd_t<T> make_simd();
template <typename T> inline simd_t<T> make_simd(T, T, T, T);

#ifndef __SIMD_OPERATOR_FOR_DECL
#define __SIMD_OPERATOR_FOR_DECL(opr) \
	simd_t& operator opr (const simd_t& s)\
		{\
		for (int i = 0; i < 4; ++i)\
			mm[i] opr s.mm[i];\
		return *this;\
		}
#endif /* __SIMD_OPERATOR_FOR_DECL */

#ifndef __SIMD_OPERATOR_BINOMIAL_DECL
#define __SIMD_OPERATOR_BINOMIAL_DECL(opr) \
	simd_t operator opr (const simd_t& s) const\
		{\
		simd_t result;\
		for (int i = 0; i < 4; ++i)\
			result.mm[i] = mm[i] opr s.mm[i];\
		return result;\
		}
#endif /* __SIMD_OPERATOR_BINOMIAL_DECL */

template <typename T>
struct simd_t
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef T type[4];
	typedef T value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	type mm;

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 先頭の値取得
	*---------------------------------------------------------------------*/
	T first() const
	{
		return mm[0];
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* アクセス演算子
	*---------------------------------------------------------------------*/
	T operator [] (int i) const
	{
		return mm[i];
	}

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	simd_t operator + () const
	{
		return *this;
	}
	simd_t operator - () const
	{
		simd_t result;
		for (int i = 0; i < 4; ++i)
		{
			result.mm[i] = -mm[i];
		}
		return result;
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	simd_t& operator = (T t)
	{
		for (int i = 0; i < 4; ++i)
		{
			mm[i] = t;
		}
		return *this;
	}
	simd_t& operator = (type_const_reference s)
	{
		for (int i = 0; i < 4; ++i)
		{
			mm[i] = s[i];
		}
		return *this;
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	__SIMD_OPERATOR_FOR_DECL(+= );
	__SIMD_OPERATOR_FOR_DECL(-= );
	__SIMD_OPERATOR_FOR_DECL(*= );
	simd_t& operator *= (T s)
	{
		for (int i = 0; i < 4; ++i)
			mm[i] *= s;
		return *this;
	}
	__SIMD_OPERATOR_FOR_DECL(/= );
	simd_t& operator /= (T s)
	{
		for (int i = 0; i < 4; ++i)
			mm[i] /= s;
		return *this;
	}
	__SIMD_OPERATOR_FOR_DECL(%= );
	__SIMD_OPERATOR_FOR_DECL(&= );
	__SIMD_OPERATOR_FOR_DECL(|= );
	__SIMD_OPERATOR_FOR_DECL(^= );

	__SIMD_OPERATOR_BINOMIAL_DECL(+);
	__SIMD_OPERATOR_BINOMIAL_DECL(-);
	__SIMD_OPERATOR_BINOMIAL_DECL(*);
	__SIMD_OPERATOR_BINOMIAL_DECL(/ );
	__SIMD_OPERATOR_BINOMIAL_DECL(%);
	__SIMD_OPERATOR_BINOMIAL_DECL(&);
	__SIMD_OPERATOR_BINOMIAL_DECL(| );
	__SIMD_OPERATOR_BINOMIAL_DECL(^);
};

#undef __SIMD_OPERATOR_FOR_DECL
#undef __SIMD_OPERATOR_BINOMIAL_DECL

template <typename T> inline
simd_t<T> make_simd()
{
	static const T ZERO = static_cast<T>(0);
	static const simd_t<T> result = {ZERO, ZERO, ZERO, ZERO};
	return result;
}
template <typename T> inline
simd_t<T> make_simd(T x, T y, T z, T w)
{
	simd_t<T> result = {x, y, z, w};
	return result;
}

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const simd_t<T>& v)
{
	os << out_char::box_brackets_left << v.mm[0] << out_char::comma_space
		<< v.mm[1] << out_char::comma_space
		<< v.mm[2] << out_char::comma_space
		<< v.mm[3] << out_char::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const simd_t<T>& v)
{
	os << out_char::box_brackets_left << v.mm[0] << out_char::comma_space
		<< v.mm[1] << out_char::comma_space
		<< v.mm[2] << out_char::comma_space
		<< v.mm[3] << out_char::box_brackets_right;
	return os;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

/*---------------------------------------------------------------------
* SIMDが使用できる場合の特殊化
*---------------------------------------------------------------------*/

#ifdef _USE_SIMD // ファイル終端まで

#	if _USE_SIMD_TYPE == _SIMD_TYPE_AVX2
#include <immintrin.h>
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_AVX
#include <immintrin.h>
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE4
#include <smmintrin.h>
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE3
#include <pmmintrin.h>
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE2
#include <emmintrin.h>
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE
#include <xmmintrin.h>
#	endif

namespace pocket
{

/* SIMDの特殊化 */
template <>
struct simd_t<float>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m128 type;
	typedef float value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	union
	{
		type mm;
		value_type _data[4];
	};

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 先頭の値取得
	*---------------------------------------------------------------------*/
	float first() const
	{
		return _mm_cvtss_f32(mm);
	}

	/*---------------------------------------------------------------------
	* 指定インデックス取得
	*---------------------------------------------------------------------*/
	template <int X, int Y, int Z, int W>
	simd_t parmute() const
	{
		simd_t result = {_mm_shuffle_ps(mm, mm, _MM_SHUFFLE(W, Z, Y, X))};
		return result;
	}
	template <int X, int Y, int Z, int W>
	simd_t& parmute(simd_t& s) const
	{
		s.mm = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(W, Z, Y, X));
		return s;
	}

	/*---------------------------------------------------------------------
	* 特定の値を取得
	*---------------------------------------------------------------------*/
	template <int INDEX>
	float at() const
	{
		type m = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(INDEX, INDEX, INDEX, INDEX));
		return _mm_cvtss_f32(m);
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 型変換演算子
	*---------------------------------------------------------------------*/
	_CXX11_EXPLICIT operator type () const
	{
		return mm;
	}
#if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
	_CXX11_EXPLICIT operator __m128i () const
	{
		return _mm_cvtps_epi32(mm);
	}
#endif /* _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2 */

#if _USE_SIMD_TYPE >= _SIMD_TYPE_AVX
	_CXX11_EXPLICIT operator __mm256d () const
	{
		return _mm256_cvtps_pd(mm);
	}
#endif /* _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2 */

	/*---------------------------------------------------------------------
	* 単項演算子
	*---------------------------------------------------------------------*/
	simd_t operator + () const
	{
		return *this;
	}
	simd_t operator - () const
	{
		simd_t result = {_mm_sub_ps(_mm_setzero_ps(), mm)};
		return result;
	}

	/*---------------------------------------------------------------------
	* 二項演算子
	*---------------------------------------------------------------------*/
	simd_t operator + (const simd_t& s) const
	{
		simd_t result = {_mm_add_ps(mm, s.mm)};
		return result;
	}
	simd_t operator - (const simd_t& s) const
	{
		simd_t result = {_mm_sub_ps(mm, s.mm)};
		return result;
	}
	simd_t operator * (const simd_t& s) const
	{
		simd_t result = {_mm_mul_ps(mm, s.mm)};
		return result;
	}
	simd_t operator / (const simd_t& s) const
	{
		simd_t result = {_mm_div_ps(mm, s.mm)};
		return result;
	}
	simd_t operator % (const simd_t& s) const
	{
		simd_t result = {_mm_and_ps(mm, s.mm)};
		return result;
	}
	simd_t operator & (const simd_t& s) const
	{
		simd_t result = {_mm_and_ps(mm, s.mm)};
		return result;
	}
	simd_t operator | (const simd_t& s) const
	{
		simd_t result = {_mm_or_ps(mm, s.mm)};
		return result;
	}
	simd_t operator ^ (const simd_t& s) const
	{
		simd_t result = {_mm_xor_ps(mm, s.mm)};
		return result;
	}

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	simd_t& operator = (float f)
	{
		mm = _mm_set1_ps(f);
		return *this;
	}
	simd_t& operator = (type_const_reference s)
	{
		mm = s;
		return *this;
	}

	/*---------------------------------------------------------------------
	* 複合演算子
	*---------------------------------------------------------------------*/
	simd_t& operator += (const simd_t& s)
	{
		mm = _mm_add_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator -= (const simd_t& s)
	{
		mm = _mm_sub_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator *= (const simd_t& s)
	{
		mm = _mm_mul_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator *= (float s)
	{
		mm = _mm_mul_ps(mm, _mm_set1_ps(s));
		return *this;
	}
	simd_t& operator /= (const simd_t& s)
	{
		mm = _mm_div_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator /= (float s)
	{
		mm = _mm_div_ps(mm, _mm_set1_ps(s));
		return *this;
	}
	simd_t& operator %= (const simd_t& s)
	{
		mm = _mm_and_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator &= (const simd_t& s)
	{
		mm = _mm_and_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator |= (const simd_t& s)
	{
		mm = _mm_or_ps(mm, s.mm);
		return *this;
	}
	simd_t& operator ^= (const simd_t& s)
	{
		mm = _mm_xor_ps(mm, s.mm);
		return *this;
	}
};

#	if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
template <>
struct simd_t<int32_t>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m128i type;
	typedef int32_t value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
template <>
struct simd_t<uint32_t>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m128i type;
	typedef uint32_t value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
#	endif

#	if _USE_SIMD_TYPE >= _SIMD_TYPE_AVX
template <>
struct simd_t<double>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m256d type;
	typedef double value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
template <>
struct simd_t<int64_t>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m256i type;
	typedef int64_t value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
template <>
struct simd_t<uint64_t>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m256i type;
	typedef uint64_t value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
#	endif

template <> inline
simd_t<float> make_simd()
{
	static const simd_t<float> result = {_mm_setzero_ps()};
	return result;
}
template <> inline
simd_t<float> make_simd(float x, float y, float z, float w)
{
	simd_t<float> result = {_mm_set_ps(w, z, y, x)};
	return result;
}


#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const simd_t<float>& v)
{
	os << out_char::box_brackets_left << v._data[0] << out_char::comma_space
		<< v._data[1] << out_char::comma_space
		<< v._data[2] << out_char::comma_space
		<< v._data[3] << out_char::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const simd_t<float>& v)
{
	os << out_char::box_brackets_left << v._data[0] << out_char::comma_space
		<< v._data[1] << out_char::comma_space
		<< v._data[2] << out_char::comma_space
		<< v._data[3] << out_char::box_brackets_right;
	return os;
}
#endif /* _USING_MATH_IO */

} /* namespace pocket */

#endif /* _USE_SIMD */

#endif /* __MATH_SIMD_H__ */
