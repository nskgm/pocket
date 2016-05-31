#ifndef __MATH_SIMD_H__
#define __MATH_SIMD_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif /* _USE_PRAGMA_ONCE */

#include "Math.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif /* _USING_MATH_IO */

namespace pocket
{

namespace detail
{
template <typename T>
struct __vector4
{
	typedef T type[4];
	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef type& type_reference;
	typedef const type& type_const_reference;

	type mm;
};

template <typename T>
struct up_type
{
	typedef T type;
};
template <>
struct up_type<float>
{
	typedef double type;
};
template <>
struct up_type<double>
{
	typedef long double type;
};
}

#ifndef _SIMD_BINOMIAL_OPERATOR
#	define _SIMD_BINOMIAL_OPERATOR(operate) \
	mm1.mm[0] operate mm2.mm[0],\
	mm1.mm[1] operate mm2.mm[1],\
	mm1.mm[2] operate mm2.mm[2],\
	mm1.mm[3] operate mm2.mm[3]

#endif /* _SIMD_BINOMIAL_OPERATOR */

template <typename T>
struct simd_t
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef detail::__vector4<T> type;
	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef type& type_reference;
	typedef const type& type_const_reference;

	typedef Math<T> math_type;

	typedef typename detail::up_type<T> up_type;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	 /* None */

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	static _INLINE_FORCE bool is_vectorize()
	{
		return false;
	}

	static _INLINE_FORCE type zero()
	{
		type result = {
			math_type::Zero,
			math_type::Zero,
			math_type::Zero,
			math_type::Zero
		};
		return result;
	}
	static _INLINE_FORCE type one()
	{
		type result = {
			math_type::One,
			math_type::One,
			math_type::One,
			math_type::One
		};
		return result;
	}
	static _INLINE_FORCE type add(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(+)
		};
		return result;
	}
	static _INLINE_FORCE type sub(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(-)
		};
		return result;
	}
	static _INLINE_FORCE type mul(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(*)
		};
		return result;
	}
	static _INLINE_FORCE type mul_add(type_const_reference mm1, type_const_reference mm2, type_const_reference mm3)
	{
		type result = {
			mm1.mm[0] * mm2.mm[0] + mm3.mm[0],
			mm1.mm[1] * mm2.mm[1] + mm3.mm[1],
			mm1.mm[2] * mm2.mm[2] + mm3.mm[2],
			mm1.mm[3] * mm2.mm[3] + mm3.mm[3],
		};
		return result;
	}
	static _INLINE_FORCE type div(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(/)
		};
		return result;
	}
	static _INLINE_FORCE type or_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(|)
		};
		return result;
	}
	static _INLINE_FORCE type and_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(&)
		};
		return result;
	}
	static _INLINE_FORCE type xor_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(^)
		};
		return result;
	}
	static _INLINE_FORCE type rem(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR(%)
		};
		return result;
	}
	static _INLINE_FORCE type sqrt(type_const_reference mm)
	{
		type result = {
			math_type::sqrt(mm.mm[0]),
			math_type::sqrt(mm.mm[1]),
			math_type::sqrt(mm.mm[2]),
			math_type::sqrt(mm.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type rsqrt(type_const_reference mm)
	{
		type result = {
			math_type::rsqrt(mm.mm[0]),
			math_type::rsqrt(mm.mm[1]),
			math_type::rsqrt(mm.mm[2]),
			math_type::rsqrt(mm.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type(max)(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			math_type::max(mm1.mm[0], mm2.mm[0]),
			math_type::max(mm1.mm[1], mm2.mm[1]),
			math_type::max(mm1.mm[2], mm2.mm[2]),
			math_type::max(mm1.mm[3], mm2.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type(min)(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			math_type::min(mm1.mm[0], mm2.mm[0]),
			math_type::min(mm1.mm[1], mm2.mm[1]),
			math_type::min(mm1.mm[2], mm2.mm[2]),
			math_type::min(mm1.mm[3], mm2.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type clamp(type_const_reference mm, type_const_reference mn, type_const_reference mx)
	{
		type result = {
			math_type::clamp(mm.mm[0], mn.mm[0], mx.mm[0]),
			math_type::clamp(mm.mm[1], mn.mm[1], mx.mm[1]),
			math_type::clamp(mm.mm[2], mn.mm[2], mx.mm[2]),
			math_type::clamp(mm.mm[3], mn.mm[3], mx.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type set(value_type f)
	{
		type result = {
			f, f, f, f
		};
		return result;
	}
	static _INLINE_FORCE type set(value_type x, value_type y, value_type z, value_type w)
	{
		type result = {
			x, y, z, w
		};
		return result;
	}
	static _INLINE_FORCE value_type first(type_const_reference mm)
	{
		return mm.mm[0];
	}
	template <int INDEX>
	static _INLINE_FORCE value_type at(type_const_reference mm)
	{
		return mm.mm[INDEX];
	}
	template <int W, int Z, int Y, int X>
	static _INLINE_FORCE type parmute(type_const_reference mm)
	{
		type result = {
			mm.mm[X],
			mm.mm[Y],
			mm.mm[Z],
			mm.mm[W]
		};
		return result;
	}
	template <int I>
	static _INLINE_FORCE type parmute(type_const_reference mm)
	{
		type result = {
			mm.mm[I],
			mm.mm[I],
			mm.mm[I],
			mm.mm[I]
		};
		return result;
	}

	static _INLINE_FORCE bool equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] == mm2.mm[0] &&
			mm1.mm[1] == mm2.mm[1] &&
			mm1.mm[2] == mm2.mm[2] &&
			mm1.mm[3] == mm2.mm[3];
	}
	static _INLINE_FORCE bool not_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] != mm2.mm[0] &&
			mm1.mm[1] != mm2.mm[1] &&
			mm1.mm[2] != mm2.mm[2] &&
			mm1.mm[3] != mm2.mm[3];
	}
	static _INLINE_FORCE bool greater(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] < mm2.mm[0] &&
			mm1.mm[1] < mm2.mm[1] &&
			mm1.mm[2] < mm2.mm[2] &&
			mm1.mm[3] < mm2.mm[3];
	}
	static _INLINE_FORCE bool greater_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] <= mm2.mm[0] &&
			mm1.mm[1] <= mm2.mm[1] &&
			mm1.mm[2] <= mm2.mm[2] &&
			mm1.mm[3] <= mm2.mm[3];
	}
	static _INLINE_FORCE bool less(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] > mm2.mm[0] &&
			mm1.mm[1] > mm2.mm[1] &&
			mm1.mm[2] > mm2.mm[2] &&
			mm1.mm[3] > mm2.mm[3];
	}
	static _INLINE_FORCE bool less_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] >= mm2.mm[0] &&
			mm1.mm[1] >= mm2.mm[1] &&
			mm1.mm[2] >= mm2.mm[2] &&
			mm1.mm[3] >= mm2.mm[3];
	}

	static _INLINE_FORCE type load1(const value_type* f)
	{
		type result = {
			*f, math_type::One, math_type::One, math_type::One
		};
		return result;
	}
	static _INLINE_FORCE type load2(const value_type* f)
	{
		type result = {
			f[0], f[1], math_type::One, math_type::One
		};
		return result;
	}
	static _INLINE_FORCE type load3(const value_type* f)
	{
		type result = {
			f[0], f[1], f[2], math_type::One
		};
		return result;
	}
	static _INLINE_FORCE type load(const value_type* f)
	{
		type result = {
			f[0], f[1], f[2], f[3]
		};
		return result;
	}
	static _INLINE_FORCE void store1(T* f, type_const_reference mm)
	{
		*f = mm.mm[0];
	}
	static _INLINE_FORCE void store2(T* f1, T* f2, type_const_reference mm)
	{
		*f1 = mm.mm[0];
		*f2 = mm.mm[1];
	}
	static _INLINE_FORCE void store3(T* f1, T* f2, T* f3, type_const_reference mm)
	{
		*f1 = mm.mm[0];
		*f2 = mm.mm[1];
		*f3 = mm.mm[2];
	}
	static _INLINE_FORCE void store(T* f, type_const_reference mm)
	{
		*f = mm.mm[0];
		++f;
		*f = mm.mm[1];
		++f;
		*f = mm.mm[2];
		++f;
		*f = mm.mm[3];
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/* None */
};

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

	typedef Math<float> math_type;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	/* None */

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	static _INLINE_FORCE bool is_vectorize()
	{
		return true;
	}

	static _INLINE_FORCE type zero()
	{
		return _mm_setzero_ps();
	}
	static _INLINE_FORCE type one()
	{
		return _mm_set_ps1(math_type::One);
	}
	static _INLINE_FORCE type add(type mm1, type mm2)
	{
		return _mm_add_ps(mm1, mm2);
	}
	static _INLINE_FORCE type sub(type mm1, type mm2)
	{
		return _mm_sub_ps(mm1, mm2);
	}
	static _INLINE_FORCE type mul(type mm1, type mm2)
	{
		return _mm_mul_ps(mm1, mm2);
	}
	static _INLINE_FORCE type mul_add(type mm1, type mm2, type mm3)
	{
		return _mm_add_ps(_mm_mul_ps(mm1, mm2), mm3);
	}
	static _INLINE_FORCE type div(type mm1, type mm2)
	{
		return _mm_div_ps(mm1, mm2);
	}
	static _INLINE_FORCE type or_(type mm1, type mm2)
	{
		return _mm_or_ps(mm1, mm2);
	}
	static _INLINE_FORCE type and_(type mm1, type mm2)
	{
		return _mm_and_ps(mm1, mm2);
	}
	static _INLINE_FORCE type xor_(type mm1, type mm2)
	{
		return _mm_xor_ps(mm1, mm2);
	}
	static _INLINE_FORCE type rem(type mm1, type mm2)
	{
		return _mm_and_ps(mm1, mm2);
	}
	static _INLINE_FORCE type sqrt(type mm)
	{
		return _mm_sqrt_ps(mm);
	}
	static _INLINE_FORCE type rsqrt(type mm)
	{
		return _mm_rsqrt_ps(mm);
	}
	static _INLINE_FORCE type (max)(type mm1, type mm2)
	{
		return _mm_max_ps(mm1, mm2);
	}
	static _INLINE_FORCE type (min)(type mm1, type mm2)
	{
		return _mm_min_ps(mm1, mm2);
	}
	static _INLINE_FORCE type clamp(type mm, type mn, type mx)
	{
		return _mm_max_ps(mn, _mm_min_ps(mm, mx));
	}
	static _INLINE_FORCE type set(value_type f)
	{
		return _mm_set_ps1(f);
	}
	static _INLINE_FORCE type set(value_type x, value_type y, value_type z, value_type w)
	{
		return _mm_set_ps(w, z, y, x);
	}
	static _INLINE_FORCE float first(type mm)
	{
		return _mm_cvtss_f32(mm);
	}
	template <int INDEX>
	static _INLINE_FORCE float at(type mm)
	{
		type m = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(INDEX, INDEX, INDEX, INDEX));
		return _mm_cvtss_f32(m);
	}
	template <int W, int Z, int Y, int X>
	static _INLINE_FORCE type parmute(type mm)
	{
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(W, Z, Y, X));
	}
	template <int I>
	static _INLINE_FORCE type parmute(type mm)
	{
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(I, I, I, I));
	}

	static _INLINE_FORCE bool equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmpeq_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool not_equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmpneq_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool greater(type mm1, type mm2)
	{
		return mask_all(_mm_cmpngt_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool greater_equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmpnge_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool less(type mm1, type mm2)
	{
		return mask_all(_mm_cmpnlt_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool less_equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmpnle_ps(mm1, mm2));
	}

	static _INLINE_FORCE type load1(const value_type* f)
	{
		return _mm_load_ss(f);
	}
	static _INLINE_FORCE type load2(const value_type* f)
	{
		return _mm_set_ps(f[0], f[1], math_type::One, math_type::One);
	}
	static _INLINE_FORCE type load3(const value_type* f)
	{
		return _mm_set_ps(f[0], f[1], f[2], math_type::One);
	}
	static _INLINE_FORCE type load(const value_type* f)
	{
		return _mm_load_ps(f);
	}
	static _INLINE_FORCE void store1(float* f, type mm)
	{
		_mm_store_ss(f, mm);
	}
	static _INLINE_FORCE void store2(float* f1, float* f2, type mm)
	{
		type m = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(f1, mm);
		_mm_store_ss(f2, m);
	}
	static _INLINE_FORCE void store3(float* f1, float* f2, float* f3, type mm)
	{
		type m1 = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(1, 1, 1, 1));
		type m2 = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(f1, mm);
		_mm_store_ss(f2, m1);
		_mm_store_ss(f3, m2);
	}
	static _INLINE_FORCE void store(float* f, type mm)
	{
		_mm_store_ps(f, mm);
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	/* None */

#if 0
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
#endif

private:
	static _INLINE_FORCE bool mask_all(type mm)
	{
		// それぞれの最上ビットが立っていたら1が入る
		// 2進で1111が入るので0x0F
		return _mm_movemask_ps(mm) == 0x0F;
	}
};

#if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
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
#endif

#if _USE_SIMD_TYPE >= _SIMD_TYPE_AVX
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
#endif

} /* namespace pocket */

#endif /* _USE_SIMD */

#endif /* __MATH_SIMD_H__ */
