#ifndef __MATH_CONFIG_H__
#define __MATH_CONFIG_H__

/*---------------------------------------------------------------------------------------
* #pragma onceが使用できるか設定
*---------------------------------------------------------------------------------------*/
#ifndef _USE_PRAGMA_ONCE
#	if (defined(_MSC_VER) && _MSC_VER >= 1100) ||\
		(defined(__GNUC__) && __GNUC__ >= 3 && __GNUC_MINOR__ >= 4) ||\
		(defined(__clang__))
#		define _USE_PRAGMA_ONCE
#	endif
#endif // _USE_PRAGMA_ONCE

#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

/*---------------------------------------------------------------------
* 64bitか
*---------------------------------------------------------------------*/
#ifndef _USE_x64
#	if defined(_MSC_VER) && defined(_WIN64)
#		define _USE_x64
#	elif (defined(__clang__) || defined(__GNUC__)) && defined(__x86_64__)
#		define _USE_x64
#	endif
#endif // _USE_x64

/*---------------------------------------------------------------------
* Windowsのいらないものを軽減
*---------------------------------------------------------------------*/
#ifndef NOMINMAX
#	define NOMINMAX
#endif // NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

/*---------------------------------------------------------------------
* namespace pocketの使用
*---------------------------------------------------------------------*/
#ifndef _NS_USING_POCKET
#	define _NS_USING_POCKET using namespace pocket
#endif // _NS_USING_POCKET

/*---------------------------------------------------------------------------------------
* なにも使用しないように設定された
*---------------------------------------------------------------------------------------*/
#if defined(_UNUSING_MATH_INT_FLOAT) && !defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#error "request define _USING_MATH_DOUBLE or _USING_MATH_LONG_DOUBLE. or undefine _UNUSING_MATH_INT_FLOAT"
#endif

/*---------------------------------------------------------------------------------------
* __cplusplusでのバージョン定義
*---------------------------------------------------------------------------------------*/
#ifndef __CXX98
#	define __CXX98 199711L
#endif // __CXX98
#ifndef __CXX03
#	define __CXX03 199711L
#endif // __CXX03
#ifndef __CXX11
#	define __CXX11 201103L
#endif // __CXX11
#ifndef __CXX14
#	define __CXX14 201402L
#endif // __CXX14

#ifndef __VCXX10
#	define __VCXX10 1600
#endif // __VCXX10
#ifndef __VCXX11
#	define __VCXX11 1700
#endif // __VCXX11
#ifndef __VCXX12
#	define __VCXX12 1800
#endif // __VCXX12
#ifndef __VCXX14
#	define __VCXX14 1900
#endif // __VCXX14

#ifndef __VS2010
#	define __VS2010 1600
#endif // __VS2010
#ifndef __VS2012
#	define __VS2012 1700
#endif // __VS2012
#ifndef __VS2013
#	define __VS2013 1800
#endif // __VS2013
#ifndef __VS2015
#	define __VS2015 1900
#endif // __VS2015

/*---------------------------------------------------------------------
* C++バージョンチェック
*---------------------------------------------------------------------*/
#ifndef _CXX_VER
#	define _CXX_VER(N) (__cplusplus >= __CXX##N)
#endif // _CXX_VER

/*---------------------------------------------------------------------
* VC++バージョンチェック
*---------------------------------------------------------------------*/
#ifndef _VCXX_VER
#	define _VCXX_VER(N) (_MSC_VER >= __VCXX##N)
#endif // _VCXX_VER

/*---------------------------------------------------------------------
* Visual Studioバージョンチェック
*---------------------------------------------------------------------*/
#ifndef _VS_VER
#	define _VS_VER(N) (_MSC_VER >= __VS##N)
#endif // _VS_VER

/*---------------------------------------------------------------------------------------
* C++11が使用できるか設定. VC++の場合はVC++12以上
*---------------------------------------------------------------------------------------*/
#ifndef _USE_CXX11
#	if ((defined(_MSC_VER) && (_MSC_VER >= 1800)) || _CXX_VER(11))
#		define _USE_CXX11
#	endif
#endif // _USE_CXX11

/*---------------------------------------------------------------------------------------
* C++14が使用できるか設定. VC++の場合は不明
*---------------------------------------------------------------------------------------*/
#ifndef _USE_CXX14
#	if (defined(_MSC_VER) && _MSC_VER >= 2000) || _CXX_VER(14)
#		define _USE_CXX14
#	endif
#endif // _USE_CXX14

/*---------------------------------------------------------------------------------------
* 匿名が使用できるか設定.
*---------------------------------------------------------------------------------------*/
#ifndef _USE_ANONYMOUS
#	if defined(_MSC_VER)
#		define _USE_ANONYMOUS
#	elif defined(__GNUC__)
#		if __GNUC__ >= 4
#			if __GNUC__ == 4
#				if __GNUC_MINOR__ >= 5
#					define _USE_ANONYMOUS
#				endif // __GNUC_MINOR__ >= 5
#			else
#				define _USE_ANONYMOUS
#			endif
#		endif
#	elif defined(__clang__)
#		if __clang_major__ >= 3
#			if __clang_major__ == 3
#				if __clang_minor__ >= 1
#					define _USE_ANONYMOUS
#				endif // __clang_minor__ >= 1
#			else
#				define _USE_ANONYMOUS
#			endif
#		endif
#	endif
#endif // _USE_ANONYMOUS

#ifndef _USE_ANONYMOUS_NON_POD
#	ifdef _MSC_VER
#		define _USE_ANONYMOUS_NON_POD
#	else
#		if defined(_USE_ANONYMOUS) && _CXX_VER(11)
#			define _USE_ANONYMOUS_NON_POD
#		endif
#	endif
#endif // !_USE_ANONYMOUS_NON_POD

/*---------------------------------------------------------------------------------------
* 匿名が使用できた場合の警告を消す＆使用を許可
*---------------------------------------------------------------------------------------*/
#ifdef _USE_ANONYMOUS
#	if defined(_MSC_VER)
#		pragma warning(disable: 4201)
#	elif defined(__GNUC__)
#		pragma anon_unions
#	endif // _MSC_VER
#endif // _USE_ANONYMOUS

/*---------------------------------------------------------------------------------------
* 匿名使用時のコンストラクタ挙動が違った場合の設定
*---------------------------------------------------------------------------------------*/
#ifndef _USE_ANONYMOUS_NORMAL_CONSTRUCT
#	ifndef _USE_ANONYMOUS_NON_POD
#		define _USE_ANONYMOUS_NORMAL_CONSTRUCT
#	else
		// 定義されていてもgccとclangでは通常コンストラクタ
#		if defined(__GNUC__) || defined(__clang__)
#			define _USE_ANONYMOUS_NORMAL_CONSTRUCT
#		endif
#	endif
#endif // _USE_ANONYMOUS_NORMAL_CONSTRUCT

/*---------------------------------------------------------------------------------------
* 匿名が使用できてC++11が使用できるか設定
*---------------------------------------------------------------------------------------*/
#if defined(_USE_ANONYMOUS) && defined(_USE_CXX11)
#	ifndef _USE_ANONYMOUS_CXX11
#		define _USE_ANONYMOUS_CXX11
#	endif // _USE_ANONYMOUS_CXX11
#endif // defined(_USE_ANONYMOUS) && defined(_USE_CXX11)

/*---------------------------------------------------------------------------------------
* インライン化を強制させるための設定
*---------------------------------------------------------------------------------------*/
#ifndef _INLINE_FORCE
#	if defined(_MSC_VER)
#		if _MSC_VER < 1200
#			define _INLINE_FORCE __inline
#		else
#			define _INLINE_FORCE __forceinline
#		endif
#	elif defined(__GNUC__)
#		define _INLINE_FORCE inline __attribute__((__always_inline__))
#	elif defined(__clang__)
#		define _INLINE_FORCE __attribute__((__always_inline__))
#	else
#		define _INLINE_FORCE inline
#	endif
#endif // _INLINE_FORCE

/*---------------------------------------------------------------------------------------
* インライン化を抑制するための設定
*---------------------------------------------------------------------------------------*/
#ifndef _INLINE_NEVER
#	if defined(_MSC_VER)
#		define _INLINE_NEVER __declspec(noinline)
#	elif defined(__GNUC__) || defined(__clang__)
#		define _INLINE_NEVER __attribute__((__noinline__))
#	else
#		define _INLINE_NEVER
#	endif
#endif // _INLINE_NEVER

/*---------------------------------------------------------------------------------------
* C++11が使用できる場合は暗黙的な型変換演算子を利用できなくするための設定
*---------------------------------------------------------------------------------------*/
#ifndef _CXX11_EXPLICIT
#	ifdef _USE_CXX11
#		define _CXX11_EXPLICIT explicit
#	else
#		define _CXX11_EXPLICIT
#	endif // _USE_CXX11
#endif // _CXX11_EXPLICIT

/*---------------------------------------------------------------------------------------
* const または constexprを定義するための設定
*---------------------------------------------------------------------------------------*/
#ifndef _CONST_OR_CONSTEXPR
#	ifdef _MSC_VER
#		if _MSC_VER >= 1900
#			define _CONST_OR_CONSTEXPR constexpr
#		else
#			define _CONST_OR_CONSTEXPR const
#		endif // _MSC_VER >= 1900
#	else
#		ifdef _USE_CXX11 // C++11以上
#			define _CONST_OR_CONSTEXPR constexpr
#		else
#			define _CONST_OR_CONSTEXPR const
#		endif // _USE_CXX11
#	endif // _MSC_VER
#endif // _CONST_OR_CONSTEXPR

/*---------------------------------------------------------------------------------------
* C++11のconstexprが使用できるか
*---------------------------------------------------------------------------------------*/
#ifndef _CXX11_CONSTEXPR
#	ifdef _MSC_VER
#		if _MSC_VER >= 1900 // VC++14以上
#			define _CXX11_CONSTEXPR constexpr
#		else
#			define _CXX11_CONSTEXPR
#		endif // _MSC_VER > 1800
#	else
#		ifdef _USE_CXX11 // C++11以上
#			define _CXX11_CONSTEXPR constexpr
#		else
#			define _CXX11_CONSTEXPR
#		endif // _USE_CXX11
#	endif // _MSC_VER
#endif // _CXX11_CONSTEXPR

/*---------------------------------------------------------------------------------------
* C++14のconstexprが使用できるか
*---------------------------------------------------------------------------------------*/
#ifndef _CXX14_CONSTEXPR
#	ifdef _MSC_VER
#		if _MSC_VER >= 2000
#			define _CXX14_CONSTEXPR constexpr
#		else
#			define _CXX14_CONSTEXPR
#		endif // _MSC_VER > 1800
#	else
#		ifdef _USE_CXX14
#			define _CXX14_CONSTEXPR constexpr
#		else
#			define _CXX14_CONSTEXPR
#		endif // _USE_CXX14
#	endif // _MSC_VER
#endif // _CXX14_CONSTEXPR

/*---------------------------------------------------------------------------------------
* 定数の定義
*---------------------------------------------------------------------------------------*/
#ifndef _STATICAL_CONSTANT
#	define _STATICAL_CONSTANT static _CONST_OR_CONSTEXPR
#endif // _STATICAL_CONSTANT

/*---------------------------------------------------------------------------------------
* デフォルトコンストラクタを定義するための設定
*---------------------------------------------------------------------------------------*/
#ifndef _DEFAULT_DECLARE_RVALUES
#	if (defined(_MSC_VER) && _VCXX_VER(14)) || _CXX_VER(11)
#		define _DEFAULT_DECLARE_RVALUES(CLASS) CLASS(CLASS&&) = default;\
												CLASS& operator = (CLASS&&) = default
#	else
#		define _DEFAULT_DECLARE_RVALUES(CLASS)
#	endif // 
#endif // _DEFAULT_DECLARE_RVALUES

#ifndef _DEFAULT_CONSTRUCTOR
#	ifdef _USE_CXX11
#		define _DEFAULT_CONSTRUCTOR(CLASS) CLASS() = default;\
											CLASS(const CLASS&) = default;\
											CLASS& operator = (const CLASS&) = default;\
											_DEFAULT_DECLARE_RVALUES(CLASS)
#	else
#		define _DEFAULT_CONSTRUCTOR(CLASS) CLASS () {}
#	endif // _USE_CXX11
#endif // _DEFAULT_CONSTRUCTOR

/*---------------------------------------------------------------------------------------
* 静的なアサーションをするための設定. msgはすべてつなげて書く
*---------------------------------------------------------------------------------------*/
#ifndef _STATICAL_ASSERT
#	ifdef _USE_CXX11
#		define _STATICAL_ASSERT(b, msg) static_assert(b, #msg)
#	else
#		define _STATICAL_ASSERT(b, msg) typedef int __##msg[(b) ? 1 : -1]
#	endif
#endif // _STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* アライメントの指定
*---------------------------------------------------------------------------------------*/
#ifndef _ALIGNED
#	if defined(_MSC_VER)
#		if _MSC_VER >= 1900
#			define _ALIGNED(N) alignas((N))
#		else
#			define _ALIGNED(N) __declspec(align(N))
#		endif
#	else
#		ifdef _USE_CXX11
#			define _ALIGNED(N) alignas((N))
#		else
#			if defined(__GNUC__) || defined(__clang__)
#				define _ALIGNED(N) __attribute__((aligned((N))))
#			else
#				define _ALIGNED(N)
#			endif
#		endif
#	endif
#endif // _ALIGNED

/*---------------------------------------------------------------------------------------
* アライメントの際に適切なサイズを指定
*---------------------------------------------------------------------------------------*/
#ifndef _ALIGNED_SIZE_INT_COUNT
#	define _ALIGNED_SIZE_INT_COUNT(N) (sizeof(int)*(N))
#endif // _ALIGNED_SIZE_INT_COUNT
#ifndef _ALIGNED_SIZEABLE_POT_MAX_64
#	define _ALIGNED_SIZEABLE_POT_MAX_64(N) ((N) <= _ALIGNED_SIZE_INT_COUNT(1) ? _ALIGNED_SIZE_INT_COUNT(1) :\
											(N) <= _ALIGNED_SIZE_INT_COUNT(2) ? _ALIGNED_SIZE_INT_COUNT(2) :\
											(N) <= _ALIGNED_SIZE_INT_COUNT(4) ? _ALIGNED_SIZE_INT_COUNT(4) :\
											(N) <= _ALIGNED_SIZE_INT_COUNT(8) ? _ALIGNED_SIZE_INT_COUNT(8) :\
											(N) <= _ALIGNED_SIZE_INT_COUNT(16) ? _ALIGNED_SIZE_INT_COUNT(16) : _ALIGNED_SIZE_INT_COUNT(0))
#endif // _ALIGNED_SIZEABLE_POT_MAX_64

#ifndef _DECLARE_ALIGNED_STRUCT
#	if defined(_MSC_VER)
#		if _MSC_VER >= 1900
#			define _DECLARE_ALIGNED_STRUCT(N) struct _ALIGNED(N)
#		else
#			define _DECLARE_ALIGNED_STRUCT(N) _ALIGNED(N) struct
#		endif
#	else
#		define _DECLARE_ALIGNED_STRUCT(N) struct _ALIGNED(N)
#	endif
#endif // _DECLARE_ALIGNED_STRUCT

#ifndef _DECLARE_ALIGNED_APPROPRIATE_STRUCT
#	define _DECLARE_ALIGNED_APPROPRIATE_STRUCT(N) _DECLARE_ALIGNED_STRUCT(_ALIGNED_SIZEABLE_POT_MAX_64(N))
#endif // _DECLARE_ALIGNED_APPROPRIATE_STRUCT

/*---------------------------------------------------------------------------------------
* 数学クラスで使用する静的アサーションの設定
*---------------------------------------------------------------------------------------*/
#ifndef _MATH_STATICAL_ASSERT
#	ifdef _USING_MATH_EXTERN_TYPE
#		define _MATH_STATICAL_ASSERT(TYPE)
#	else
#		ifdef _UNUSING_MATH_INT_FLOAT
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, longdouble_only)
#			endif
#		else
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_longdouble_only)
#			else
#				define _MATH_STATICAL_ASSERT(TYPE) _STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_only)
#			endif
#		endif
#	endif // _USING_MATH_EXTERN_TYPE
#endif // _MATH_STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* 上記の浮動小数点の設定
*---------------------------------------------------------------------------------------*/
#ifndef _MATH_STATICAL_ASSERT_FLOATING
#	ifdef _USING_MATH_EXTERN_TYPE
#		define _MATH_STATICAL_ASSERT_FLOATING(TYPE)
#	else
#		ifdef _UNUSING_MATH_INT_FLOAT
#			define _MATH_STATICAL_ASSERT_FLOATING _MATH_STATICAL_ASSERT
#		else
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT_FLOATING(TYPE) _STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT_FLOATING(TYPE) _STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define _MATH_STATICAL_ASSERT_FLOATING(TYPE) _STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_longdouble_only)
#			else
#				define _MATH_STATICAL_ASSERT_FLOATING(TYPE) _STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_only)
#			endif
#		endif
#	endif // _USING_MATH_EXTERN_TYPE
#endif // _MATH_STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* テンプレートクラスの暗黙定義を抑制するための設定. 使用するには_USING_MATH_EXTERN_TYPEを定義する
*---------------------------------------------------------------------------------------*/
#ifndef _MATH_EXTERN_TYPE
#	ifndef _USING_MATH_EXTERN_TYPE
#		define _MATH_EXTERN_TYPE(TYPE)
#	else
#		ifdef _USE_CXX11
#			ifdef _UNUSING_MATH_INT_FLOAT
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<double>
#				else
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			else
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE)
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<double>
#				else
#					define _MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			endif
#		else
#			define _MATH_EXTERN_TYPE(TYPE)
#		endif // _USE_CXX11
#	endif // _USING_MATH_EXTERN_TYPE
#endif // _MATH_EXTERN_TYPE

/*---------------------------------------------------------------------------------------
* 上記の浮動小数点の設定
*---------------------------------------------------------------------------------------*/
#ifndef _MATH_EXTERN_FLOATING_TYPE
#	ifndef _USING_MATH_EXTERN_TYPE
#		define _MATH_EXTERN_FLOATING_TYPE(TYPE)
#	else
#		ifdef _USE_CXX11
#			ifdef _UNUSING_MATH_INT_FLOAT
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define _MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>
#				else
#					define _MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			else // _UNUSING_MATH_INT_FLOAT
#				define _MATH_EXTERN_FLOATING_TYPE _MATH_EXTERN_TYPE
#			endif // _UNUSING_MATH_INT_FLOAT
#		else
#			define _MATH_EXTERN_FLOATING_TYPE(TYPE)
#		endif // _USE_CXX11
#	endif // _USING_MATH_EXTERN_TYPE
#endif // _MATH_EXTERN_TYPE

/*---------------------------------------------------------------------------------------
* SIMDの設定
*---------------------------------------------------------------------------------------*/

#ifndef _SIMD_TYPE_SSE
#	define _SIMD_TYPE_SSE (0)
#endif // _SIMD_TYPE_SSE
#ifndef _SIMD_TYPE_SSE2
#	define _SIMD_TYPE_SSE2 (1)
#endif // _SIMD_TYPE_SSE2
#ifndef _SIMD_TYPE_SSE3
#	define _SIMD_TYPE_SSE3 (2)
#endif // _SIMD_TYPE_SSE3
#ifndef _SIMD_TYPE_SSE4
#	define _SIMD_TYPE_SSE4 (3)
#endif // _SIMD_TYPE_SSE4
#ifndef _SIMD_TYPE_AVX
#	define _SIMD_TYPE_AVX (4)
#endif // _SIMD_TYPE_AVX
#ifndef _SIMD_TYPE_AVX2
#	define _SIMD_TYPE_AVX2 (5)
#endif // _SIMD_TYPE_AVX2

// _UNUSING_SIMDを定義されていたら使用しない
#if !defined(_USE_SIMD) && !defined(_USE_SIMD_TYPE) && !defined(_UNUSING_SIMD)
#	if defined(__AVX2__) || defined(__AVX__) // AVXは共通処理
#		define _USE_SIMD
#		ifdef __AVX2__
#			define _USE_SIMD_TYPE _SIMD_TYPE_AVX2
#		else
#			define _USE_SIMD_TYPE _SIMD_TYPE_AVX
#		endif // __AVX2__
#	elif defined(_MSC_VER)
#		if defined(_M_IX86_FP) || defined(_M_IX86) || defined(_M_IX64_FP) || defined(_M_IX64)
#			define _USE_SIMD
#			if defined(_M_IX86_FP)
#				if _M_IX86_FP == 2
#					define _USE_SIMD_TYPE _SIMD_TYPE_SSE2
#				elif _M_IX86_FP == 1
#					define _USE_SIMD_TYPE _SIMD_TYPE_SSE
#				endif
#			elif defined(_M_IX64_FP)
#				if _M_IX64_FP == 2
#					define _USE_SIMD_TYPE _SIMD_TYPE_SSE2
#				elif _M_IX64_FP == 1
#					define _USE_SIMD_TYPE _SIMD_TYPE_SSE
#				endif
#			else
#				define _USE_SIMD_TYPE _SIMD_TYPE_SSE2
#			endif
#		elif defined(_USE_x64) // 64bitが使用できる場合は必ずSSE2以上が使用可能, AVXかは上で判定
#			define _USE_SIMD
#			define _USE_SIMD_TYPE _SIMD_TYPE_SSE2
#		endif
#	elif defined(__GNUC__) || defined(__clang__)
#		if defined(__SSE4_2__) || defined(__SSE4_1__) || defined(__SSE4__) || defined(__SSE3__) || defined(__SSE2__)
#			define _USE_SIMD
#			if defined(__SSE4_2__) || defined(__SSE4_1__) || defined(__SSE4__)
#				define _USE_SIMD_TYPE _SIMD_TYPE_SSE4
#			elif defined(__SSE3__)
#				define _USE_SIMD_TYPE _SIMD_TYPE_SSE3
#			else
#				define _USE_SIMD_TYPE _SIMD_TYPE_SSE2
#			endif
#		endif // __SSE4_2__ || __SSE4_1__ || __SSE4__ || __SSE3__ || __SSE2__
#	endif
#endif // _USE_SIMD

#if defined(_USE_SIMD) && defined(_USE_ANONYMOUS)
#	define _USE_SIMD_ANONYMOUS
#endif // _USE_SIMD && _USE_ANONYMOUS

#ifdef _USE_SIMD
#	ifndef _USE_SIMD_128 // 128bitが使用できる
#		if _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
#			define _USE_SIMD_128
#		endif // _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
#	endif // _USE_SIMD_128
#	ifndef _USE_SIMD_256 // 256btiが使用できる
#		if _USE_SIMD_TYPE >= _SIMD_TYPE_AVX
#			define _USE_SIMD_256
#		endif // _USE_SIMD_TYPE >= _SIMD_TYPE_SSE2
#	endif // _USE_SIMD_256
#endif // _USE_SIMD

/*---------------------------------------------------------------------------------------
* SIMDのタイプを文字列定義
*---------------------------------------------------------------------------------------*/
#if !defined(_USE_SIMD_TYPE_STRING) && defined(_USE_SIMD)
#	if _USE_SIMD_TYPE == _SIMD_TYPE_AVX2
#		define _USE_SIMD_TYPE_STRING "AVX2"
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_AVX
#		define _USE_SIMD_TYPE_STRING "AVX"
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE4
#		define _USE_SIMD_TYPE_STRING "SSE4"
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE3
#		define _USE_SIMD_TYPE_STRING "SSE3"
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE2
#		define _USE_SIMD_TYPE_STRING "SSE2"
#	elif _USE_SIMD_TYPE == _SIMD_TYPE_SSE
#		define _USE_SIMD_TYPE_STRING "SSE"
#	endif
#endif // _USE_SIMD_TYPE_STRING

#endif // _MATH_CONFIG_H__
