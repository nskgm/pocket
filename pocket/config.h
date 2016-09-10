#ifndef __POCKET_CONFIG_H__
#define __POCKET_CONFIG_H__

/*---------------------------------------------------------------------------------------
* #pragma onceが使用できるか設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_USE_PRAGMA_ONCE
#	if (defined(_MSC_VER) && _MSC_VER >= 1100) ||\
		(defined(__GNUC__) && __GNUC__ >= 3 && __GNUC_MINOR__ >= 4) ||\
		defined(__clang__)
#		define POCKET_USE_PRAGMA_ONCE
#	endif
#endif // POCKET_USE_PRAGMA_ONCE

#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

/*---------------------------------------------------------------------
* コンパイラ種類
*---------------------------------------------------------------------*/
#ifndef POCKET_COMPILER_GCC
#	define POCKET_COMPILER_GCC (0)
#endif // POCKET_COMPILER_GCC
#ifndef POCKET_COMPILER_CLANG
#	define POCKET_COMPILER_CLANG (1)
#endif // POCKET_COMPILER_CLANG
#ifndef POCKET_COMPILER_VC
#	define POCKET_COMPILER_VC (2)
#endif // POCKET_USE_VC

#ifndef POCKET_COMPILER_UNKNOWN
#	define POCKET_COMPILER_UNKNOWN (0xFFFFFFFF)
#endif // POCKET_COMPILER_UNKNOWN

/*---------------------------------------------------------------------
* コンパイラ種類の統合
*---------------------------------------------------------------------*/
#ifndef POCKET_COMPILER
#	ifdef __GNUC__
#		define POCKET_COMPILER POCKET_COMPILER_GCC
#	elif defined(__clang__)
#		define POCKET_COMPILER POCKET_COMPILER_CLANG
#	elif defined(_MSC_VER)
#		define POCKET_COMPILER POCKET_COMPILER_VC
#	else
#		define POCKET_COMPILER POCKET_COMPILER_UNKNOWN
#	endif
#endif // POCKET_COMPILER

/*---------------------------------------------------------------------
* GCC: POCKET_COMPILER_IF(GCC)
* clang: POCKET_COMPILER_IF(CLANG)
* visual c: POCKET_COMPILER_IF(VC)
*---------------------------------------------------------------------*/
#ifndef POCKET_COMPILER_IF
#	define POCKET_COMPILER_IF(C) (POCKET_COMPILER == POCKET_COMPILER_##C)
#endif // POCKET_COMPILER_IF

/*---------------------------------------------------------------------
* 64bitか
*---------------------------------------------------------------------*/
#ifndef POCKET_USE_X64
#	if defined(_MSC_VER) && defined(_WIN64)
#		define POCKET_USE_X64
#	elif (defined(__clang__) || defined(__GNUC__)) && defined(__x86_64__)
#		define POCKET_USE_X64
#	endif
#endif // POCKET_USE_X64

/*---------------------------------------------------------------------
* namespace pocketの使用
*---------------------------------------------------------------------*/
#ifndef POCKET_USING_NS
#	define POCKET_USING_NS using namespace pocket;
#endif // POCKET_USING_NS

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
#	define __VCXX10 (1600)
#endif // __VCXX10
#ifndef __VCXX11
#	define __VCXX11 (1700)
#endif // __VCXX11
#ifndef __VCXX12
#	define __VCXX12 (1800)
#endif // __VCXX12
#ifndef __VCXX14
#	define __VCXX14 (1900)
#endif // __VCXX14

// ?
#ifndef __VCXX15
#	define __VCXX15 (2000)
#endif // __VCXX15

/*---------------------------------------------------------------------
* C++バージョンチェック
*---------------------------------------------------------------------*/
#ifndef POCKET_CXX_VER
#	ifdef __cplusplus
#		define POCKET_CXX_VER(N) (__cplusplus >= __CXX##N)
#	else
#		define POCKET_CXX_VER(N) (0)
#	endif // __cplusplus
#endif // POCKET_CXX_VER

/*---------------------------------------------------------------------
* VC++バージョンチェック
*---------------------------------------------------------------------*/
#ifndef POCKET_VCXX_VER
#	if defined(_MSC_VER)
#		define POCKET_VCXX_VER(N) (_MSC_VER >= __VCXX##N)
#	else
#		define POCKET_VCXX_VER(N) (0)
#	endif // defined(_MSC_VER)
#endif // POCKET_VCXX_VER

/*---------------------------------------------------------------------------------------
* C++11が使用できるか設定. VC++の場合はVC++12以上
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX11
#	if POCKET_VCXX_VER(12) || POCKET_CXX_VER(11) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#		define POCKET_USE_CXX11
#	endif //
#endif // POCKET_USE_CXX11

#ifdef POCKET_USE_CXX11
#include <utility>
#endif // POCKET_USE_CXX11

/*---------------------------------------------------------------------------------------
* C++14が使用できるか設定. VC++の場合は不明
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_USE_CXX14
#	if POCKET_VCXX_VER(15) || POCKET_CXX_VER(14)
#		define POCKET_USE_CXX14
#	endif //
#endif // POCKET_USE_CXX14

/*---------------------------------------------------------------------------------------
* 匿名が使用できるか設定.
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_USE_ANONYMOUS
#	if defined(_MSC_VER)
#		define POCKET_USE_ANONYMOUS
#	elif defined(__GNUC__)
#		if __GNUC__ >= 4
#			if __GNUC__ == 4
#				if __GNUC_MINOR__ >= 5
#					define POCKET_USE_ANONYMOUS
#				endif // __GNUC_MINOR__ >= 5
#			else
#				define POCKET_USE_ANONYMOUS
#			endif
#		endif
#	elif defined(__clang__)
#		if __clang_major__ >= 3
#			if __clang_major__ == 3
#				if __clang_minor__ >= 1
#					define POCKET_USE_ANONYMOUS
#				endif // __clang_minor__ >= 1
#			else
#				define POCKET_USE_ANONYMOUS
#			endif
#		endif
#	endif
#endif // POCKET_USE_ANONYMOUS

#ifndef POCKET_USE_ANONYMOUS_NON_POD
#	ifdef _MSC_VER
#		define POCKET_USE_ANONYMOUS_NON_POD
#	else
#		if defined(POCKET_USE_ANONYMOUS) && POCKET_CXX_VER(11)
#			define POCKET_USE_ANONYMOUS_NON_POD
#		endif
#	endif
#endif // !POCKET_USE_ANONYMOUS_NON_POD

/*---------------------------------------------------------------------------------------
* 匿名が使用できた場合の警告を消す＆使用を許可
*---------------------------------------------------------------------------------------*/
#ifdef POCKET_USE_ANONYMOUS
#	if defined(_MSC_VER)
#		pragma warning(disable: 4201)
#	elif defined(__GNUC__)
//#		pragma anon_unions
#	endif // _MSC_VER
#endif // POCKET_USE_ANONYMOUS

/*---------------------------------------------------------------------------------------
* 匿名使用時のコンストラクタ挙動が違った場合の設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#	ifndef POCKET_USE_ANONYMOUS_NON_POD
#		define POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#	else
		// 定義されていてもgccとclangでは通常コンストラクタ
#		if defined(__GNUC__) || defined(__clang__)
#			define POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#		endif
#	endif
#endif // POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT

/*---------------------------------------------------------------------------------------
* 匿名が使用できてC++11が使用できるか設定
*---------------------------------------------------------------------------------------*/
#if defined(POCKET_USE_ANONYMOUS) && defined(POCKET_USE_CXX11)
#	ifndef POCKET_USE_ANONYMOUS_CXX11
#		define POCKET_USE_ANONYMOUS_CXX11
#	endif // POCKET_USE_ANONYMOUS_CXX11
#endif // defined(POCKET_USE_ANONYMOUS) && defined(POCKET_USE_CXX11)

/*---------------------------------------------------------------------------------------
* インライン化を強制させるための設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_INLINE_FORCE
#	if defined(_MSC_VER)
#		if _MSC_VER < 1200
#			define POCKET_INLINE_FORCE __inline
#		else
#			define POCKET_INLINE_FORCE __forceinline
#		endif
#	elif defined(__GNUC__)
#		define POCKET_INLINE_FORCE inline __attribute__((__always_inline__))
#	elif defined(__clang__)
#		define POCKET_INLINE_FORCE __attribute__((__always_inline__))
#	else
#		define POCKET_INLINE_FORCE inline
#	endif
#endif // POCKET_INLINE_FORCE

/*---------------------------------------------------------------------------------------
* インライン化を抑制するための設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_INLINE_NEVER
#	if defined(_MSC_VER)
#		define POCKET_INLINE_NEVER __declspec(noinline)
#	elif defined(__GNUC__) || defined(__clang__)
#		define POCKET_INLINE_NEVER __attribute__((__noinline__))
#	else
#		define POCKET_INLINE_NEVER
#	endif
#endif // POCKET_INLINE_NEVER

/*---------------------------------------------------------------------------------------
* C++11が使用できる場合は暗黙的な型変換演算子を利用できなくするための設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_CXX11_EXPLICIT
#	ifdef POCKET_USE_CXX11
#		define POCKET_CXX11_EXPLICIT explicit
#	else
#		define POCKET_CXX11_EXPLICIT
#	endif // POCKET_USE_CXX11
#endif // POCKET_CXX11_EXPLICIT

/*---------------------------------------------------------------------------------------
* C++11が使用できる場合は戻り値の時にmoveを使用
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_CXX11_MOVE
#	ifdef POCKET_USE_CXX11
#		define POCKET_CXX11_MOVE(V) std::move((V))
#	else
#		define POCKET_CXX11_MOVE(V) (V)
#	endif // POCKET_USE_CXX11
#endif // POCKET_CXX11_MOVE

/*---------------------------------------------------------------------------------------
* const または constexprを定義するための設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_CONST_OR_CONSTEXPR
#	if POCKET_VCXX_VER(14) || POCKET_CXX_VER(11)
#		define POCKET_CONST_OR_CONSTEXPR constexpr
#	else
#		define POCKET_CONST_OR_CONSTEXPR const
#	endif
#endif // POCKET_CONST_OR_CONSTEXPR

/*---------------------------------------------------------------------------------------
* C++11のconstexprが使用できるか
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_CXX11_CONSTEXPR
#	if POCKET_VCXX_VER(14) || POCKET_CXX_VER(11)
#		define POCKET_CXX11_CONSTEXPR constexpr
#	else
#		define POCKET_CXX11_CONSTEXPR
#	endif
#endif // POCKET_CXX11_CONSTEXPR

/*---------------------------------------------------------------------------------------
* C++14のconstexprが使用できるか
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_CXX14_CONSTEXPR
#	if POCKET_VCXX_VER(15) || POCKET_CXX_VER(14)
#		define POCKET_CXX14_CONSTEXPR constexpr
#	else
#		define POCKET_CXX14_CONSTEXPR
#	endif
#endif // POCKET_CXX14_CONSTEXPR

/*---------------------------------------------------------------------------------------
* 定数の定義
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_STATICAL_CONSTANT
#	define POCKET_STATICAL_CONSTANT static POCKET_CONST_OR_CONSTEXPR
#endif // POCKET_STATICAL_CONSTANT

/*---------------------------------------------------------------------------------------
* デフォルトコンストラクタを定義するための設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_DEFAULT_DECLARE_RVALUES
#	if POCKET_VCXX_VER(14) || POCKET_CXX_VER(11)
#		define POCKET_DEFAULT_DECLARE_RVALUES(CLASS) CLASS(CLASS&&) = default;\
												CLASS& operator = (CLASS&&) = default
#	else
#		define POCKET_DEFAULT_DECLARE_RVALUES(CLASS)
#	endif //
#endif // POCKET_DEFAULT_DECLARE_RVALUES

#ifndef POCKET_DEFAULT_CONSTRUCTOR
#	ifdef POCKET_USE_CXX11
#		define POCKET_DEFAULT_CONSTRUCTOR(CLASS) CLASS() = default;\
											CLASS(const CLASS&) = default;\
											CLASS& operator = (const CLASS&) = default;\
											POCKET_DEFAULT_DECLARE_RVALUES(CLASS)
#	else
#		define POCKET_DEFAULT_CONSTRUCTOR(CLASS) CLASS() {}
#	endif // POCKET_USE_CXX11
#endif // POCKET_DEFAULT_CONSTRUCTOR

/*------------------------------------------------------------------------------------------
* 一度のみ宣言するタグ用
*------------------------------------------------------------------------------------------*/
#ifndef POCKET_DECL_ONCE_ARGMENT_TAG
// pragma once が使用できる場合はstruct
#	ifdef POCKET_USE_PRAGMA_ONCE
#		define POCKET_DECL_ONCE_ARGMENT_TAG(NAME) struct _##NAME##_t {}; static const _##NAME##_t NAME = {}
// 以外はenumで型判定
#	else
#		define POCKET_DECL_ONCE_ARGMENT_TAG(NAME) enum _##NAME##_t { NAME }
#	endif
#endif // POCKET_DECL_ONCE_ARGMENT_TAG

/*---------------------------------------------------------------------------------------
* 静的なアサーションをするための設定. msgはすべてつなげて書く
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_STATICAL_ASSERT
#	ifdef POCKET_USE_CXX11
#		define POCKET_STATICAL_ASSERT(b, msg) static_assert(b, #msg)
#	else
#		define POCKET_STATICAL_ASSERT(b, msg) typedef int __##msg[(b) ? 1 : -1]
#	endif
#endif // POCKET_STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* アライメントの指定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_ALIGNED
#	if defined(_MSC_VER)
#		if POCKET_VCXX_VER(14)
#			define POCKET_ALIGNED(N) alignas((N))
#		else
#			define POCKET_ALIGNED(N) __declspec(align(N))
#		endif
#	else
#		ifdef POCKET_USE_CXX11
#			define POCKET_ALIGNED(N) alignas((N))
#		else
#			if defined(__GNUC__) || defined(__clang__)
#				define POCKET_ALIGNED(N) __attribute__((aligned((N))))
#			else
#				define POCKET_ALIGNED(N)
#			endif
#		endif
#	endif
#endif // POCKET_ALIGNED

/*---------------------------------------------------------------------------------------
* アライメントの際に適切なサイズを指定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_ALIGNED_SIZE_INT_COUNT
#	define POCKET_ALIGNED_SIZE_INT_COUNT(N) (sizeof(int)*(N))
#endif // POCKET_ALIGNED_SIZE_INT_COUNT
#ifndef POCKET_ALIGNED_SIZEABLE_POT_MAX_64
#	define POCKET_ALIGNED_SIZEABLE_POT_MAX_64(N) ((N) <= POCKET_ALIGNED_SIZE_INT_COUNT(1) ? POCKET_ALIGNED_SIZE_INT_COUNT(1) :\
											(N) <= POCKET_ALIGNED_SIZE_INT_COUNT(2) ? POCKET_ALIGNED_SIZE_INT_COUNT(2) :\
											(N) <= POCKET_ALIGNED_SIZE_INT_COUNT(4) ? POCKET_ALIGNED_SIZE_INT_COUNT(4) :\
											(N) <= POCKET_ALIGNED_SIZE_INT_COUNT(8) ? POCKET_ALIGNED_SIZE_INT_COUNT(8) :\
											(N) <= POCKET_ALIGNED_SIZE_INT_COUNT(16) ? POCKET_ALIGNED_SIZE_INT_COUNT(16) : POCKET_ALIGNED_SIZE_INT_COUNT(0))
#endif // POCKET_ALIGNED_SIZEABLE_POT_MAX_64

#ifndef POCKET_DECLARE_ALIGNED_STRUCT
#	if defined(_MSC_VER)
#		if POCKET_VCXX_VER(14)
#			define POCKET_DECLARE_ALIGNED_STRUCT(N) struct POCKET_ALIGNED(N)
#		else
#			define POCKET_DECLARE_ALIGNED_STRUCT(N) POCKET_ALIGNED(N) struct
#		endif
#	else
#		define POCKET_DECLARE_ALIGNED_STRUCT(N) struct POCKET_ALIGNED(N)
#	endif
#endif // POCKET_DECLARE_ALIGNED_STRUCT

#ifndef POCKET_DECLARE_ALIGNED_APPROPRIATE_STRUCT
#	define POCKET_DECLARE_ALIGNED_APPROPRIATE_STRUCT(N) POCKET_DECLARE_ALIGNED_STRUCT(POCKET_ALIGNED_SIZEABLE_POT_MAX_64(N))
#endif // POCKET_DECLARE_ALIGNED_APPROPRIATE_STRUCT

/*---------------------------------------------------------------------------------------
* 数学クラスで使用する静的アサーションの設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_MATH_STATICAL_ASSERT
#	ifdef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_STATICAL_ASSERT(TYPE)
#	else
#		ifdef _UNUSING_MATH_INT_FLOAT
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, longdouble_only)
#			endif
#		else
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_longdouble_only)
#			else
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_only)
#			endif
#		endif
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* 上記の浮動小数点の設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_MATH_STATICAL_ASSERT_FLOATING
#	ifdef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE)
#	else
#		ifdef _UNUSING_MATH_INT_FLOAT
#			define POCKET_MATH_STATICAL_ASSERT_FLOATING POCKET_MATH_STATICAL_ASSERT
#		else
#			if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_or_longdouble_only)
#			elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_only)
#			elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_longdouble_only)
#			else
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_only)
#			endif
#		endif
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_STATICAL_ASSERT

/*---------------------------------------------------------------------------------------
* テンプレートクラスの暗黙定義を抑制するための設定. 使用するにはPOCKET_USING_MATH_EXTERN_TYPEを定義する
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_MATH_EXTERN_TYPE
#	ifndef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_EXTERN_TYPE(TYPE)
#	else
#		ifdef POCKET_USE_CXX11
#			ifdef _UNUSING_MATH_INT_FLOAT
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<double>
#				else
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			else
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE)
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<double>
#				else
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			endif
#		else
#			define POCKET_MATH_EXTERN_TYPE(TYPE)
#		endif // POCKET_USE_CXX11
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_EXTERN_TYPE

/*---------------------------------------------------------------------------------------
* 上記の浮動小数点の設定
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_MATH_EXTERN_FLOATING_TYPE
#	ifndef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE)
#	else
#		ifdef POCKET_USE_CXX11
#			ifdef _UNUSING_MATH_INT_FLOAT
#				if defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>
#				elif defined(_USING_MATH_DOUBLE) && !defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(_USING_MATH_DOUBLE) && defined(_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>
#				else
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			else // _UNUSING_MATH_INT_FLOAT
#				define POCKET_MATH_EXTERN_FLOATING_TYPE POCKET_MATH_EXTERN_TYPE
#			endif // _UNUSING_MATH_INT_FLOAT
#		else
#			define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE)
#		endif // POCKET_USE_CXX11
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_EXTERN_TYPE

/*---------------------------------------------------------------------------------------
* 配列サイズ
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_ARRAY_SIZE
#	define POCKET_ARRAY_SIZE(ARY) (sizeof(ARY) / sizeof(ARY[0]))
#endif // POCKET_ARRAY_SIZE

/*---------------------------------------------------------------------------------------
* SIMD種類
*---------------------------------------------------------------------------------------*/
#ifndef POCKET_SIMD_TYPE_SSE
#	define POCKET_SIMD_TYPE_SSE (0)
#endif // POCKET_SIMD_TYPE_SSE
#ifndef POCKET_SIMD_TYPE_SSE2
#	define POCKET_SIMD_TYPE_SSE2 (1)
#endif // POCKET_SIMD_TYPE_SSE2
#ifndef POCKET_SIMD_TYPE_SSE3
#	define POCKET_SIMD_TYPE_SSE3 (2)
#endif // POCKET_SIMD_TYPE_SSE3
#ifndef POCKET_SIMD_TYPE_SSE4
#	define POCKET_SIMD_TYPE_SSE4 (3)
#endif // POCKET_SIMD_TYPE_SSE4
#ifndef POCKET_SIMD_TYPE_SSE4_1
#	define POCKET_SIMD_TYPE_SSE4_1 (4)
#endif // POCKET_SIMD_TYPE_SSE4_1
#ifndef POCKET_SIMD_TYPE_SSE4_2
#	define POCKET_SIMD_TYPE_SSE4_2 (5)
#endif // POCKET_SIMD_TYPE_SSE4_2
#ifndef POCKET_SIMD_TYPE_AVX
#	define POCKET_SIMD_TYPE_AVX (6)
#endif // POCKET_SIMD_TYPE_AVX
#ifndef POCKET_SIMD_TYPE_AVX2
#	define POCKET_SIMD_TYPE_AVX2 (7)
#endif // POCKET_SIMD_TYPE_AVX2

/*---------------------------------------------------------------------------------------
* SIMD設定
*---------------------------------------------------------------------------------------*/
#if !defined(POCKET_USE_SIMD) && !defined(POCKET_USE_SIMD_TYPE) && !defined(POCKET_UNUSING_SIMD)
#	if defined(__AVX2__) || defined(__AVX__) // AVXは共通処理
#		define POCKET_USE_SIMD
#		ifdef __AVX2__
#			define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_AVX2
#		else
#			define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_AVX
#		endif // __AVX2__
#	elif defined(_MSC_VER)
#		if defined(_M_IX86_FP) || defined(_M_IX86) || defined(_M_IX64_FP) || defined(_M_IX64)
#			define POCKET_USE_SIMD
#			if defined(_M_IX86_FP)
#				if _M_IX86_FP == 2
#					define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE2
#				elif _M_IX86_FP == 1
#					define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE
#				endif
#			elif defined(_M_IX64_FP)
#				if _M_IX64_FP == 2
#					define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE2
#				elif _M_IX64_FP == 1
#					define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE
#				endif
#			else
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE2
#			endif
#		elif defined(POCKET_USE_X64) // 64bitが使用できる場合は必ずSSE2以上が使用可能, AVXかは上で判定
#			define POCKET_USE_SIMD
#			define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE2
#		endif
#	elif defined(__GNUC__) || defined(__clang__)
#		if defined(__SSE4_2__) || defined(__SSE4_1__) || defined(__SSE4__) || defined(__SSE3__) || defined(__SSE2__)
#			define POCKET_USE_SIMD
#			if defined(__SSE4_2__)
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE4_2
#			elif defined(__SSE4_1__)
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE4_1
#			elif defined(__SSE4__)
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE4
#			elif defined(__SSE3__)
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE3
#			else
#				define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_SSE2
#			endif
#		endif // __SSE4_2__ || __SSE4_1__ || __SSE4__ || __SSE3__ || __SSE2__
#	endif
#endif // POCKET_USE_SIMD

/*---------------------------------------------------------------------------------------
* SIMDと無名が使用できるか
*---------------------------------------------------------------------------------------*/
#if defined(POCKET_USE_SIMD) && defined(POCKET_USE_ANONYMOUS)
#	define POCKET_USE_SIMD_ANONYMOUS
#endif // POCKET_USE_SIMD && POCKET_USE_ANONYMOUS

/*---------------------------------------------------------------------------------------
* 128 と 256 が使用できるか
*---------------------------------------------------------------------------------------*/
#ifdef POCKET_USE_SIMD
#	ifndef POCKET_USE_SIMD_128 // 128bitが使用できる
#		if POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_SSE2
#			define POCKET_USE_SIMD_128
#		endif // POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_SSE2
#	endif // POCKET_USE_SIMD_128
#	ifndef POCKET_USE_SIMD_256 // 256btiが使用できる
#		if POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_AVX
#			define POCKET_USE_SIMD_256
#		endif // POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_SSE2
#	endif // POCKET_USE_SIMD_256
#endif // POCKET_USE_SIMD

/*---------------------------------------------------------------------------------------
* SIMDのタイプを文字列定義
*---------------------------------------------------------------------------------------*/
#if !defined(POCKET_USE_SIMD_TYPE_STRING) && defined(POCKET_USE_SIMD)
#	if POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_AVX2
#		define POCKET_USE_SIMD_TYPE_STRING "AVX2"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_AVX
#		define POCKET_USE_SIMD_TYPE_STRING "AVX"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE4_2
#		define POCKET_USE_SIMD_TYPE_STRING "SSE4.2"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE4_1
#		define POCKET_USE_SIMD_TYPE_STRING "SSE4.1"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE4
#		define POCKET_USE_SIMD_TYPE_STRING "SSE4"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE3
#		define POCKET_USE_SIMD_TYPE_STRING "SSE3"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE2
#		define POCKET_USE_SIMD_TYPE_STRING "SSE2"
#	elif POCKET_USE_SIMD_TYPE == POCKET_SIMD_TYPE_SSE
#		define POCKET_USE_SIMD_TYPE_STRING "SSE"
#	endif
#endif // POCKET_USE_SIMD_TYPE_STRING

#endif // __POCKET_CONFIG_H__
