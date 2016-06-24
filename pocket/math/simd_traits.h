#ifndef __POCKET_MATH_SIMD_TRAITS_H__
#define __POCKET_MATH_SIMD_TRAITS_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "math_traits.h"
#include <limits>
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

namespace pocket
{

namespace detail
{
template <typename T, size_t N>
struct _mvector_i
{
	typedef T type[N];

	type mm;
};

template <typename T, size_t N>
struct _mvector
{
	typedef T type[N];
	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef type& type_reference;
	typedef const type& type_const_reference;

	typedef math_traits<T> math_type;
	typedef typename math_type::value_int_type value_int_type;
	typedef _mvector_i<value_int_type, N> int_t;

	union
	{
		type mm;
		int_t i;
	};
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

#ifndef _SIMD_BINOMIAL_OPERATOR_2
#	define _SIMD_BINOMIAL_OPERATOR_2(operate) \
	mm1.mm[0] operate mm2.mm[0],\
	mm1.mm[1] operate mm2.mm[1]
#endif // _SIMD_BINOMIAL_OPERATOR_2
#ifndef _SIMD_BINOMIAL_OPERATOR_3
#	define _SIMD_BINOMIAL_OPERATOR_3(operate) \
	mm1.mm[0] operate mm2.mm[0],\
	mm1.mm[1] operate mm2.mm[1],\
	mm1.mm[2] operate mm2.mm[2]
#endif // _SIMD_BINOMIAL_OPERATOR_3
#ifndef _SIMD_BINOMIAL_OPERATOR_4
#	define _SIMD_BINOMIAL_OPERATOR_4(operate) \
	mm1.mm[0] operate mm2.mm[0],\
	mm1.mm[1] operate mm2.mm[1],\
	mm1.mm[2] operate mm2.mm[2],\
	mm1.mm[3] operate mm2.mm[3]
#endif // _SIMD_BINOMIAL_OPERATOR_4

/*---------------------------------------------------------------------
* 空の宣言のみで特殊化で中身を実装していく
*---------------------------------------------------------------------*/
template <typename T>
struct simd_traits
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef math_traits<T> math_type;
	typedef detail::_mvector<T, 4> type;
	typedef typename math_type::value_int_type value_int_type;
	typedef typename type::int_t type_int;
	typedef typename detail::up_type<T>::type type_up;

	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef type& type_reference;
	typedef const type& type_const_reference;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	// none

	/*---------------------------------------------------------------------------------------
	* Constants
	*---------------------------------------------------------------------------------------*/

	// none

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 並列計算はできない
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE bool is_vectorize()
	{
		return false;
	}

	/*---------------------------------------------------------------------
	* 整数型に変換
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type_int to_i(type_const_reference mm)
	{
		type_int result = {
			static_cast<value_int_type>(mm.mm[0]),
			static_cast<value_int_type>(mm.mm[1]),
			static_cast<value_int_type>(mm.mm[2]),
			static_cast<value_int_type>(mm.mm[3]),
		};
		return result;
	}
	/*---------------------------------------------------------------------
	* 浮動小数型に変換
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type to_f(const type_int& mm)
	{
		type result = {
			static_cast<value_type>(mm.mm[0]),
			static_cast<value_type>(mm.mm[1]),
			static_cast<value_type>(mm.mm[2]),
			static_cast<value_type>(mm.mm[3])
		};
		return result;
	}

	/*---------------------------------------------------------------------
	* 数値反転
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type negate(type_const_reference mm)
	{
		type result = {
			-mm.mm[0],
			-mm.mm[1],
			-mm.mm[2],
			-mm.mm[3]
		};
		return result;
	}

	/*---------------------------------------------------------------------
	* 絶対値
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type abs(type_const_reference mm)
	{
		type result = {
			math_type::abs(mm.mm[0]),
			math_type::abs(mm.mm[1]),
			math_type::abs(mm.mm[2]),
			math_type::abs(mm.mm[3])
		};
		return result;
	}

	/*---------------------------------------------------------------------
	* 定数生成
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type zero()
	{
		type result = {
			math_type::zero,
			math_type::zero,
			math_type::zero,
			math_type::zero
		};
		return result;
	}
	static _INLINE_FORCE type one()
	{
		type result = {
			math_type::one,
			math_type::one,
			math_type::one,
			math_type::one
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

	/*---------------------------------------------------------------------
	* 演算
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type add(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(+)
		};
		return result;
	}
	static _INLINE_FORCE type sub(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(-)
		};
		return result;
	}
	static _INLINE_FORCE type mul(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(*)
		};
		return result;
	}
	static _INLINE_FORCE type mul(type_const_reference mm, value_type f)
	{
		type result = {
			mm.mm[0] * f,
			mm.mm[1] * f,
			mm.mm[2] * f,
			mm.mm[3] * f
		};
		return result;
	}
	static _INLINE_FORCE type mad(type_const_reference mm1, type_const_reference mm2, type_const_reference mm3)
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
			_SIMD_BINOMIAL_OPERATOR_4(/ )
		};
		return result;
	}
	static _INLINE_FORCE type div(type_const_reference mm, value_type f)
	{
		type result = {
			mm.mm[0] / f,
			mm.mm[1] / f,
			mm.mm[2] / f,
			mm.mm[3] / f
		};
		return result;
	}
	static _INLINE_FORCE type or_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(| )
		};
		return result;
	}
	static _INLINE_FORCE type and_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(&)
		};
		return result;
	}
	static _INLINE_FORCE type xor_(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(^)
		};
		return result;
	}
	static _INLINE_FORCE type rem(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			_SIMD_BINOMIAL_OPERATOR_4(%)
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
	static _INLINE_FORCE type clamp01(type_const_reference mm)
	{
		type result = {
			math_type::clamp01(mm.mm[0]),
			math_type::clamp01(mm.mm[1]),
			math_type::clamp01(mm.mm[2]),
			math_type::clamp01(mm.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type reciprocal(type_const_reference mm)
	{
		type result = {
			math_type::reciprocal(mm.mm[0]),
			math_type::reciprocal(mm.mm[1]),
			math_type::reciprocal(mm.mm[2]),
			math_type::reciprocal(mm.mm[3])
		};
		return result;
	}

	/*---------------------------------------------------------------------
	* 指定の部分を取得
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE value_type first(type_const_reference mm)
	{
		return mm.mm[0];
	}
	static _INLINE_FORCE value_type x(type_const_reference mm)
	{
		return mm.mm[0];
	}
	static _INLINE_FORCE value_type y(type_const_reference mm)
	{
		return mm.mm[1];
	}
	static _INLINE_FORCE value_type z(type_const_reference mm)
	{
		return mm.mm[2];
	}
	static _INLINE_FORCE value_type w(type_const_reference mm)
	{
		return mm.mm[3];
	}

	template <int INDEX>
	static _INLINE_FORCE value_type at(type_const_reference mm)
	{
		return mm.mm[INDEX];
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type shuffle(type_const_reference mm1, type_const_reference mm2)
	{
		type result = {
			mm1.mm[X],
			mm1.mm[Y],
			mm2.mm[Z],
			mm2.mm[W]
		};
		return result;
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type permute(type_const_reference mm)
	{
		type result = {
			mm.mm[X],
			mm.mm[Y],
			mm.mm[Z],
			mm.mm[W]
		};
		return result;
	}
	static _INLINE_FORCE type permute_x(type_const_reference mm)
	{
		type result = {
			mm.mm[0],
			mm.mm[0],
			mm.mm[0],
			mm.mm[0]
		};
		return result;
	}
	static _INLINE_FORCE type permute_y(type_const_reference mm)
	{
		type result = {
			mm.mm[1],
			mm.mm[1],
			mm.mm[1],
			mm.mm[1]
		};
		return result;
	}
	static _INLINE_FORCE type permute_z(type_const_reference mm)
	{
		type result = {
			mm.mm[2],
			mm.mm[2],
			mm.mm[2],
			mm.mm[2]
		};
		return result;
	}
	static _INLINE_FORCE type permute_w(type_const_reference mm)
	{
		type result = {
			mm.mm[3],
			mm.mm[3],
			mm.mm[3],
			mm.mm[3]
		};
		return result;
	}

	/*---------------------------------------------------------------------
	* 選択 [0より大きい値を選択できるようにする]
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type selector(value_type x, value_type y, value_type z, value_type w)
	{
		type result = {
			x > math_type::zero ? std::numeric_limits<value_int_type>::max() : 0,
			y > math_type::zero ? std::numeric_limits<value_int_type>::max() : 0,
			z > math_type::zero ? std::numeric_limits<value_int_type>::max() : 0,
			w > math_type::zero ? std::numeric_limits<value_int_type>::max() : 0
		};
		return result;
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type selector()
	{
		const type result = {
			X > 0 ? std::numeric_limits<value_int_type>::max() : 0,
			Y > 0 ? std::numeric_limits<value_int_type>::max() : 0,
			Z > 0 ? std::numeric_limits<value_int_type>::max() : 0,
			W > 0 ? std::numeric_limits<value_int_type>::max() : 0
		};
		return result;
	}
	static _INLINE_FORCE type select(type_const_reference mm1, type_const_reference mm2, type_const_reference mm_select)
	{
		type result = {
			(mm1.i.mm[0] & ~mm_select.i.mm[0]) | (mm2.i.mm[0] & mm_select.i.mm[0]),
			(mm1.i.mm[1] & ~mm_select.i.mm[1]) | (mm2.i.mm[1] & mm_select.i.mm[1]),
			(mm1.i.mm[2] & ~mm_select.i.mm[2]) | (mm2.i.mm[2] & mm_select.i.mm[2]),
			(mm1.i.mm[3] & ~mm_select.i.mm[3]) | (mm2.i.mm[3] & mm_select.i.mm[3])
		};
		return result;
	}
	static _INLINE_FORCE type select(type_const_reference mm, type_const_reference mm_select)
	{
		return select(mm, zero(), mm_select);
	}
	static _INLINE_FORCE type select0111()
	{
		type result = {
			0,
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max()
		};
		return result;
	}
	static _INLINE_FORCE type select0111(type_const_reference mm1, type_const_reference mm2)
	{
		return select(mm1, mm2, select0111());
	}
	static _INLINE_FORCE type select0111(type_const_reference mm)
	{
		return select(mm, select0111());
	}
	static _INLINE_FORCE type select1011()
	{
		type result = {
			std::numeric_limits<value_int_type>::max(),
			0,
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max()
		};
		return result;
	}
	static _INLINE_FORCE type select1011(type_const_reference mm1, type_const_reference mm2)
	{
		return select(mm1, mm2, select1011());
	}
	static _INLINE_FORCE type select1011(type_const_reference mm)
	{
		return select(mm, select1011());
	}
	static _INLINE_FORCE type select1101()
	{
		type result = {
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max(),
			0,
			std::numeric_limits<value_int_type>::max()
		};
		return result;
	}
	static _INLINE_FORCE type select1101(type_const_reference mm1, type_const_reference mm2)
	{
		return select(mm1, mm2, select1101());
	}
	static _INLINE_FORCE type select1101(type_const_reference mm)
	{
		return select(mm, select1101());
	}
	static _INLINE_FORCE type select1110()
	{
		type result = {
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max(),
			std::numeric_limits<value_int_type>::max(),
			0
		};
		return result;
	}
	static _INLINE_FORCE type select1110(type_const_reference mm1, type_const_reference mm2)
	{
		return select(mm1, mm2, select1110());
	}
	static _INLINE_FORCE type select1110(type_const_reference mm)
	{
		return select(mm, select1110());
	}

	/*---------------------------------------------------------------------
	* 比較
	*---------------------------------------------------------------------*/
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
		return mm1.mm[0] > mm2.mm[0] &&
			mm1.mm[1] > mm2.mm[1] &&
			mm1.mm[2] > mm2.mm[2] &&
			mm1.mm[3] > mm2.mm[3];
	}
	static _INLINE_FORCE bool greater_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] >= mm2.mm[0] &&
			mm1.mm[1] >= mm2.mm[1] &&
			mm1.mm[2] >= mm2.mm[2] &&
			mm1.mm[3] >= mm2.mm[3];
	}
	static _INLINE_FORCE bool less(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] < mm2.mm[0] &&
			mm1.mm[1] < mm2.mm[1] &&
			mm1.mm[2] < mm2.mm[2] &&
			mm1.mm[3] < mm2.mm[3];
	}
	static _INLINE_FORCE bool less_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return mm1.mm[0] <= mm2.mm[0] &&
			mm1.mm[1] <= mm2.mm[1] &&
			mm1.mm[2] <= mm2.mm[2] &&
			mm1.mm[3] <= mm2.mm[3];
	}
	static _INLINE_FORCE bool near_equal_zero(type_const_reference mm1)
	{
		return math_type::is_near_zero(mm1.mm[0]) &&
			math_type::is_near_zero(mm1.mm[1]) &&
			math_type::is_near_zero(mm1.mm[2]) &&
			math_type::is_near_zero(mm1.mm[3]);
	}
	static _INLINE_FORCE bool near_equal(type_const_reference mm1, type_const_reference mm2)
	{
		return math_type::is_near(mm1.mm[0], mm2.mm[0]) &&
			math_type::is_near(mm1.mm[1], mm2.mm[1]) &&
			math_type::is_near(mm1.mm[2], mm2.mm[2]) &&
			math_type::is_near(mm1.mm[3], mm2.mm[3]);
	}

	/*---------------------------------------------------------------------
	* load, store
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type load1(const value_type* f)
	{
		type result = {
			*f, math_type::zero, math_type::zero, math_type::zero
		};
		return result;
	}
	static _INLINE_FORCE type load2(const value_type* f)
	{
		type result = {
			f[0], f[1], math_type::zero, math_type::zero
		};
		return result;
	}
	static _INLINE_FORCE type load3(const value_type* f)
	{
		type result = {
			f[0], f[1], f[2], math_type::zero
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
	static _INLINE_FORCE void store1(value_type* f, type_const_reference mm)
	{
		*f = mm.mm[0];
	}
	static _INLINE_FORCE void store2(value_type* f1, value_type* f2, type_const_reference mm)
	{
		*f1 = mm.mm[0];
		*f2 = mm.mm[1];
	}
	static _INLINE_FORCE void store3(value_type* f1, value_type* f2, value_type* f3, type_const_reference mm)
	{
		*f1 = mm.mm[0];
		*f2 = mm.mm[1];
		*f3 = mm.mm[2];
	}
	static _INLINE_FORCE void store(value_type* f, type_const_reference mm)
	{
		*f = mm.mm[0];
		++f;
		*f = mm.mm[1];
		++f;
		*f = mm.mm[2];
		++f;
		*f = mm.mm[3];
	}

	/*---------------------------------------------------------------------
	* 計算
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type dot(type_const_reference mm1, type_const_reference mm2)
	{
		value_type d = mm1.mm[0] * mm2.mm[0] + mm1.mm[1] * mm2.mm[1] + mm1.mm[2] * mm2.mm[2] + mm1.mm[3] * mm2.mm[3];
		type result = {
			d, d, d, d
		};
		return result;
	}
	static _INLINE_FORCE type length_sq(type_const_reference mm)
	{
		return dot(mm, mm);
	}
	static _INLINE_FORCE type length(type_const_reference mm)
	{
		value_type d = mm.mm[0] * mm.mm[0] + mm.mm[1] * mm.mm[1] + mm.mm[2] * mm.mm[2] + mm.mm[3] * mm.mm[3];
		value_type l = math_type::sqrt(d);
		type result = {
			l, l, l, l
		};
		return result;
	}
	static _INLINE_FORCE type lerp(type_const_reference from, type_const_reference to, value_type f)
	{
		return lerp(from, to, set(f));
	}
	static _INLINE_FORCE type lerp(type_const_reference from, type_const_reference to, type_const_reference f)
	{
		type result = {
			math_type::lerp(from.mm[0], to.mm[0], f.mm[0]),
			math_type::lerp(from.mm[1], to.mm[1], f.mm[1]),
			math_type::lerp(from.mm[2], to.mm[2], f.mm[2]),
			math_type::lerp(from.mm[3], to.mm[3], f.mm[3])
		};
		return result;
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	// none

private:
	union conv_t
	{
		value_type f;
		value_int_type i;
	};
	static _INLINE_FORCE value_int_type as_int(value_type f)
	{
		conv_t c;
		c.f = f;
		return c.i;
	}
	static _INLINE_FORCE value_type as_float(value_int_type i)
	{
		conv_t c;
		c.i = i;
		return c.f;
	}
};

} // namespace pocket

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::detail::_mvector<T, N>& v)
{
	// [X, Y, Z, W]
	os << pocket::io::box_brackets_left << v.mm[0] << pocket::io::comma_space
		<< v.mm[1] << pocket::io::comma_space
		<< v.mm[2] << pocket::io::comma_space
		<< v.mm[3] << pocket::io::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const pocket::detail::_mvector<T, N>& v)
{
	os << pocket::io::box_brackets_left << v.mm[0] << pocket::io::comma_space
		<< v.mm[1] << pocket::io::comma_space
		<< v.mm[2] << pocket::io::comma_space
		<< v.mm[3] << pocket::io::box_brackets_right;
	return os;
}
#endif // _USING_MATH_IO

#undef _SIMD_BINOMIAL_OPERATOR_2
#undef _SIMD_BINOMIAL_OPERATOR_3
#undef _SIMD_BINOMIAL_OPERATOR_4

#ifdef _USE_SIMD // ファイル終端まで

/*---------------------------------------------------------------------
* simd_traitsが使用できる場合の特殊化
*---------------------------------------------------------------------*/

#if (_USE_SIMD_TYPE == _SIMD_TYPE_AVX2) || (_USE_SIMD_TYPE == _SIMD_TYPE_AVX)
#include <immintrin.h>
#elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE4_2
#include <nmmintrin.h>
#elif (_USE_SIMD_TYPE == _SIMD_TYPE_SSE4) || (_USE_SIMD_TYPE == _SIMD_TYPE_SSE4_1)
#include <smmintrin.h>
#elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE3
#include <pmmintrin.h>
#elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE2
#include <emmintrin.h>
#elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE
#include <xmmintrin.h>
#endif // _USE_SIMD_TYPE == _SIMD_TYPE_XXX

namespace pocket
{

namespace detail
{
#if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
template <typename T>
struct __mvec3;

#	ifndef _UNUSING_MATH_INT_FLOAT
template <>
struct __mvec3<float>
{
	typedef float value_type;
	typedef __m64 type;

	type mm;
	value_type f;
};
#	endif // _UNUSING_MATH_INT_FLOAT

#	ifdef _USING_MATH_DOUBLE
template <>
struct __mvec3<double>
{
	typedef double value_type;
	typedef __m128d type;

	type mm;
	value_type f;
};
#	endif // _USING_MATH_DOUBLE
#endif // _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
}

template <>
struct simd_traits<float>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef math_traits<float> math_type;

	typedef __m128 type;
	typedef __m128i type_int;
#ifdef _USE_SIMD_256
	typedef __m256 type_up;
#else
	typedef type type_up;
#endif // _USE_SIMD_256
	typedef float value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef type& type_reference;
	typedef const type& type_const_reference;

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	// none

	/*---------------------------------------------------------------------------------------
	* Constants
	*---------------------------------------------------------------------------------------*/

	// none

	/*---------------------------------------------------------------------------------------
	* Functions
	*---------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------
	* 並列計算ができる
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE bool is_vectorize()
	{
		return true;
	}

	/*---------------------------------------------------------------------
	* 整数型に変換
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type_int to_i(type mm)
	{
		return _mm_cvtps_epi32(mm);
	}
	/*---------------------------------------------------------------------
	* 浮動小数型に変換
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type to_f(type_int mm)
	{
		return _mm_cvtepi32_ps(mm);
	}

	/*---------------------------------------------------------------------
	* 反転
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type negate(type mm)
	{
		const type z = _mm_setzero_ps();
		return _mm_sub_ps(z, mm);
	}
	/*---------------------------------------------------------------------
	* 絶対値
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type abs(type mm)
	{
		const type z = _mm_setzero_ps();
		type s = _mm_sub_ps(z, mm);
		return _mm_max_ps(s, mm);
	}

	/*---------------------------------------------------------------------
	* 定数生成
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type zero()
	{
		return _mm_setzero_ps();
	}
	static _INLINE_FORCE type one()
	{
		return _mm_set_ps1(math_type::one);
	}
	static _INLINE_FORCE type set(value_type f)
	{
		return _mm_set_ps1(f);
	}
	static _INLINE_FORCE type set(value_type x, value_type y, value_type z, value_type w)
	{
		return _mm_set_ps(w, z, y, x);
	}

	/*---------------------------------------------------------------------
	* 演算
	*---------------------------------------------------------------------*/
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
	static _INLINE_FORCE type mul(type mm, value_type f)
	{
		return _mm_mul_ps(mm, _mm_set_ps1(f));
	}
	static _INLINE_FORCE type mad(type mm1, type mm2, type mm3)
	{
		return _mm_add_ps(_mm_mul_ps(mm1, mm2), mm3);
	}
	static _INLINE_FORCE type div(type mm1, type mm2)
	{
		return _mm_div_ps(mm1, mm2);
	}
	static _INLINE_FORCE type div(type mm, value_type f)
	{
		return _mm_div_ps(mm, _mm_set_ps1(f));
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
		type_int mi1 = to_i(mm1);
		type_int mi2 = to_i(mm2);
		type_int r = _mm_and_si128(mi1, mi2);
		return to_f(r);
	}
	static _INLINE_FORCE type sqrt(type mm)
	{
		return _mm_sqrt_ps(mm);
	}
	static _INLINE_FORCE type rsqrt(type mm)
	{
		return _mm_rsqrt_ps(mm);
	}
	static _INLINE_FORCE type(max)(type mm1, type mm2)
	{
		return _mm_max_ps(mm1, mm2);
	}
	static _INLINE_FORCE type(min)(type mm1, type mm2)
	{
		return _mm_min_ps(mm1, mm2);
	}
	static _INLINE_FORCE type clamp(type mm, type mn, type mx)
	{
		return _mm_max_ps(mn, _mm_min_ps(mm, mx));
	}
	static _INLINE_FORCE type clamp01(type mm)
	{
		const type z = _mm_setzero_ps();
		const type o = _mm_set_ps1(math_type::one);
		return _mm_max_ps(z, _mm_min_ps(mm, o));
	}
	static _INLINE_FORCE type reciprocal(type mm)
	{
		return _mm_rcp_ps(mm);
	}

	/*---------------------------------------------------------------------
	* 指定要素を取得
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE float first(type mm)
	{
		return _mm_cvtss_f32(mm);
	}

	static _INLINE_FORCE float x(type mm)
	{
		return _mm_cvtss_f32(mm);
	}
	static _INLINE_FORCE float y(type mm)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(mm, mm, _MM_SHUFFLE(1, 1, 1, 1)));
	}
	static _INLINE_FORCE float z(type mm)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(mm, mm, _MM_SHUFFLE(2, 2, 2, 2)));
	}
	static _INLINE_FORCE float w(type mm)
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(mm, mm, _MM_SHUFFLE(3, 3, 3, 3)));
	}

	template <int INDEX>
	static _INLINE_FORCE float at(type mm)
	{
		return _mm_cvtss_f32(
			simd_traits::template permute<INDEX, INDEX, INDEX, INDEX>(mm)
		);
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type shuffle(type mm1, type mm2)
	{
		return _mm_shuffle_ps(mm1, mm2, _MM_SHUFFLE(W, Z, Y, X));
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type permute(type mm)
	{
#ifdef _USE_SIMD_256
		return _mm_permute_ps(mm, _MM_SHUFFLE(W, Z, Y, X));
#else
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(W, Z, Y, X));
#endif // _USE_SIMD_256
	}
	static _INLINE_FORCE type permute_x(type mm)
	{
#ifdef _USE_SIMD_256
		return _mm_permute_ps(mm, _MM_SHUFFLE(0, 0, 0, 0));
#else
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(0, 0, 0, 0));
#endif // _USE_SIMD_256
	}
	static _INLINE_FORCE type permute_y(type mm)
	{
#ifdef _USE_SIMD_256
		return _mm_permute_ps(mm, _MM_SHUFFLE(1, 1, 1, 1));
#else
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(1, 1, 1, 1));
#endif // _USE_SIMD_256
	}
	static _INLINE_FORCE type permute_z(type mm)
	{
#ifdef _USE_SIMD_256
		return _mm_permute_ps(mm, _MM_SHUFFLE(2, 2, 2, 2));
#else
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(2, 2, 2, 2));
#endif // _USE_SIMD_256
	}
	static _INLINE_FORCE type permute_w(type mm)
	{
#ifdef _USE_SIMD_256
		return _mm_permute_ps(mm, _MM_SHUFFLE(3, 3, 3, 3));
#else
		return _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _USE_SIMD_256
	}

	/*---------------------------------------------------------------------
	* 選択
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type selector(value_type x, value_type y, value_type z, value_type w)
	{
		// ゼロより大きいものをマスクとする
		const type mask = set(x, y, z, w);
		const type zero = _mm_setzero_ps();
		return _mm_cmpgt_ps(zero, mask);
	}
	template <int X, int Y, int Z, int W>
	static _INLINE_FORCE type selector()
	{
		const type mask = _mm_set_ps(static_cast<float>(W), static_cast<float>(Z), static_cast<float>(Y), static_cast<float>(X));
		const type zero = _mm_set_ps1(math_type::half);
		return _mm_cmpgt_ps(zero, mask);
	}
	static _INLINE_FORCE type select(type mm1, type mm2, type mm_select)
	{
		type m1 = _mm_andnot_ps(mm_select, mm1);
		type m2 = _mm_and_ps(mm2, mm_select);
		return _mm_or_ps(m1, m2);
	}
	static _INLINE_FORCE type select(type mm, type mm_select)
	{
		const type zero = _mm_setzero_ps();
		return select(mm, zero, mm_select);
	}

	/*------------------------------
	* 0111
	*------------------------------*/
	static _INLINE_FORCE type select0111()
	{
		// -1, 1, 1, 1 => 0111
		return selector(-math_type::one, math_type::one, math_type::one, math_type::one);
	}
	static _INLINE_FORCE type select0111(type mm1, type mm2)
	{
		return select(mm1, mm2, select0111());
	}
	static _INLINE_FORCE type select0111(type mm)
	{
		return select(mm, select0111());
	}
	/*------------------------------
	* 1011
	*------------------------------*/
	static _INLINE_FORCE type select1011()
	{
		// 1, -1, 1, 1 => 1011
		return selector(math_type::one, -math_type::one, math_type::one, math_type::one);
	}
	static _INLINE_FORCE type select1011(type mm1, type mm2)
	{
		return select(mm1, mm2, select1011());
	}
	static _INLINE_FORCE type select1011(type mm)
	{
		return select(mm, select1011());
	}
	/*------------------------------
	* 1101
	*------------------------------*/
	static _INLINE_FORCE type select1101()
	{
		// 1, 1, -1, 1 => 1101
		return selector(math_type::one, math_type::one, -math_type::one, math_type::one);
	}
	static _INLINE_FORCE type select1101(type mm1, type mm2)
	{
		return select(mm1, mm2, select1101());
	}
	static _INLINE_FORCE type select1101(type mm)
	{
		return select(mm, select1101());
	}
	/*------------------------------
	* 1110
	*------------------------------*/
	static _INLINE_FORCE type select1110()
	{
		// 1, 1, 1, -1 => 1110
		return selector(math_type::one, math_type::one, math_type::one, -math_type::one);
	}
	static _INLINE_FORCE type select1110(type mm1, type mm2)
	{
		return select(mm1, mm2, select1110());
	}
	static _INLINE_FORCE type select1110(type mm)
	{
		return select(mm, select1110());
	}

	/*---------------------------------------------------------------------
	* 比較
	*---------------------------------------------------------------------*/
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
		return mask_all(_mm_cmpgt_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool greater_equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmpge_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool less(type mm1, type mm2)
	{
		return mask_all(_mm_cmplt_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool less_equal(type mm1, type mm2)
	{
		return mask_all(_mm_cmple_ps(mm1, mm2));
	}
	static _INLINE_FORCE bool near_equal_zero(type mm)
	{
		const type z = _mm_setzero_ps();
		return near_equal(z, mm);
	}
	static _INLINE_FORCE bool near_equal(type mm1, type mm2)
	{
		const type epsilon = _mm_set_ps1(math_type::epsilon);
		type delta = _mm_sub_ps(mm1, mm2);
		return mask_all(_mm_cmple_ps(abs(delta), epsilon));
	}

	/*---------------------------------------------------------------------
	* load, store
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type load1(const value_type* f)
	{
		return _mm_load_ss(f);
	}
	static _INLINE_FORCE type load2(const value_type* f)
	{
		return _mm_set_ps(math_type::zero, math_type::zero, f[1], f[0]);
	}
	static _INLINE_FORCE type load3(const value_type* f)
	{
		return _mm_set_ps(math_type::zero, f[2], f[1], f[0]);
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

	/*---------------------------------------------------------------------
	* 計算
	*---------------------------------------------------------------------*/
	static _INLINE_FORCE type dot(type mm1, type mm2)
	{
		// SSE4.1以上はサポートされている
#if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE4_1
		return _mm_dp_ps(mm1, mm2, 0xFF);
#else
		// X*X, Y*Y, Z*Z, W*W
		type r = _mm_mul_ps(mm1, mm2);
		// W*W, Z*Z, Y*Y, X*X
		type perm = _mm_shuffle_ps(r, r, _MM_SHUFFLE(0, 1, 2, 3));
		// X*X+W*W, Y*Y+Z*Z, Z*Z+Y*Y, W*W+X*X
		r = _mm_add_ps(r, perm);
		// Z*Z+Y*Y, W*W+X*X, X*X+W*W, Y*Y+Z*Z
		perm = _mm_shuffle_ps(r, r, _MM_SHUFFLE(1, 0, 3, 2));
		// X*X+W*W+Z*Z+Y*Y, Y*Y+Z*Z+W*W+X*X, Z*Z+Y*Y+X*X+W*W, W*W+X*X+Y*Y+Z*Z
		return _mm_add_ps(r, perm);
#endif // _USE_SIMD_TYPE >= _SIMD_TYPE_SSE4_1
	}
	static _INLINE_FORCE type length_sq(type mm)
	{
		return dot(mm, mm);
	}
	static _INLINE_FORCE type length(type mm)
	{
		return _mm_sqrt_ps(length_sq(mm));
	}
	static _INLINE_FORCE type lerp(type from, type to, value_type f)
	{
		return lerp(from, to, set(f));
	}
	static _INLINE_FORCE type lerp(type from, type to, type f)
	{
		// from*(1.0 - t) + to*t
		type ft = _mm_sub_ps(one(), f);
		return _mm_add_ps(_mm_mul_ps(from, ft), _mm_mul_ps(to, f));
	}

	/*---------------------------------------------------------------------------------------
	* Operators
	*---------------------------------------------------------------------------------------*/

	// none

#if 0
	_CXX11_EXPLICIT operator type () const
	{
		return mm;
	}
#ifdef _USE_SIMD_128
	_CXX11_EXPLICIT operator __m128i () const
	{
		return _mm_cvtps_epi32(mm);
	}
#endif

#ifdef _USE_SIMD_256
	_CXX11_EXPLICIT operator __mm256d () const
	{
		return _mm256_cvtps_pd(mm);
	}
#endif
#endif

private:
	static _INLINE_FORCE bool mask_all(type mm)
	{
		// それぞれの最上ビットが立っていたら1が入る
		// 2進で1111が入るので0x0F
		return _mm_movemask_ps(mm) == 0x0F;
	}
};

template <>
struct simd_traits<int32_t>
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
struct simd_traits<uint32_t>
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

#ifdef _USE_SIMD_256
template <>
struct simd_traits<double>
{
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

	typedef __m256 type;
	typedef double value_type;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef type& type_reference;
	typedef const type& type_const_reference;
};
template <>
struct simd_traits<int64_t>
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
struct simd_traits<uint64_t>
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
#endif // _USE_SIMD_256

} // namespace pocket

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const __m128& v)
{
	_ALIGNED(16) float mm[4];
	_mm_store_ps(&mm[0], v);

	// [X, Y, Z, W]
	os << pocket::io::box_brackets_left << mm[0] << pocket::io::comma_space
		<< mm[1] << pocket::io::comma_space
		<< mm[2] << pocket::io::comma_space
		<< mm[3] << pocket::io::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const __m128& v)
{
	_ALIGNED(16) float mm[4];
	_mm_store_ps(&mm[0], v);

	os << pocket::io::box_brackets_left << mm[0] << pocket::io::comma_space
		<< mm[1] << pocket::io::comma_space
		<< mm[2] << pocket::io::comma_space
		<< mm[3] << pocket::io::box_brackets_right;
	return os;
}
#endif // _USING_MATH_IO

#endif // _USE_SIMD

#endif // __POCKET_MATH_SIMD_TRAITS_H__
