#ifndef __POCKET_CONFIG_H__
#define __POCKET_CONFIG_H__

//---------------------------------------------------------------------------------------
// コンパイラ種類
//---------------------------------------------------------------------------------------
#ifndef POCKET_COMPILER_TYPE_GCC
#	define POCKET_COMPILER_TYPE_GCC (0)
#endif // POCKET_COMPILER_TYPE_GCC
#ifndef POCKET_COMPILER_TYPE_CLANG
#	define POCKET_COMPILER_TYPE_CLANG (1)
#endif // POCKET_COMPILER_TYPE_CLANG
#ifndef POCKET_COMPILER_TYPE_VC
#	define POCKET_COMPILER_TYPE_VC (2)
#endif // POCKET_USE_VC

#ifndef POCKET_COMPILER_TYPE_UNKNOWN
#	define POCKET_COMPILER_TYPE_UNKNOWN (0xFFFFFFFF)
#endif // POCKET_COMPILER_TYPE_UNKNOWN

//---------------------------------------------------------------------------------------
// コンパイラ種類の統合
//---------------------------------------------------------------------------------------
#ifndef POCKET_COMPILER
#	ifdef __GNUC__
#		define POCKET_COMPILER POCKET_COMPILER_TYPE_GCC
#	elif defined(__clang__)
#		define POCKET_COMPILER POCKET_COMPILER_TYPE_CLANG
#	elif defined(_MSC_VER)
#		define POCKET_COMPILER POCKET_COMPILER_TYPE_VC
#	else
#		define POCKET_COMPILER POCKET_COMPILER_TYPE_UNKNOWN
#	endif
#endif // POCKET_COMPILER

//---------------------------------------------------------------------------------------
// GCC: POCKET_COMPILER_IF(GCC)
// clang: POCKET_COMPILER_IF(CLANG)
// visual c++: POCKET_COMPILER_IF(VC)
//---------------------------------------------------------------------------------------
#ifndef POCKET_COMPILER_IF
#	define POCKET_COMPILER_IF(C) (POCKET_COMPILER == POCKET_COMPILER_TYPE_##C)
#endif // POCKET_COMPILER_IF

#if POCKET_COMPILER_IF(UNKNOWN)
#error "unsupported compiler type."
#endif // POCKET_COMPILER_IF(UNKNOWN)

//---------------------------------------------------------------------------------------
// GCCバージョン
//---------------------------------------------------------------------------------------
#if !defined(POCKET_GCC_MAKE_VERSION) && !defined(POCKET_GCC_VERSION) && !defined(POCKET_GCC_HAS_VERSION) && !defined(POCKET_GCC_VERSION_IF)
#	if POCKET_COMPILER_IF(GCC)
#		define POCKET_GCC_MAKE_VERSION(MAJOR, MINOR) (MAJOR * 100 + MINOR)
#		define POCKET_GCC_VERSION POCKET_GCC_MAKE_VERSION(__GNUC__, __GNUC_MINOR__)
#		define POCKET_GCC_HAS_VERSION(MAJOR, MINOR) (POCKET_GCC_VERSION >= POCKET_GCC_MAKE_VERSION(MAJOR, MINOR))
#		define POCKET_GCC_VERSION_IF(MAJOR, MINOR) (POCKET_GCC_VERSION == POCKET_GCC_MAKE_VERSION(MAJOR, MINOR))
#	else
#		define POCKET_GCC_MAKE_VERSION(MAJOR, MINOR) (0)
#		define POCKET_GCC_VERSION (0)
#		define POCKET_GCC_HAS_VERSION(MAJOR, MINOR) (0)
#		define POCKET_GCC_VERSION_IF(MAJOR, MINOR) (0)
#	endif // POCKET_COMPILER_IF(GCC)
#endif // !POCKET_GCC_MAKE_VERSION && !POCKET_GCC_VERSION && !POCKET_GCC_HAS_VERSION

//---------------------------------------------------------------------------------------
// clangバージョン
//---------------------------------------------------------------------------------------
#if !defined(POCKET_CLANG_MAKE_VERSION) && !defined(POCKET_CLANG_VERSION) && !defined(POCKET_CLANG_HAS_VERSION) && !defined(POCKET_CLANG_VERSION_IF)
#	if POCKET_COMPILER_IF(CLANG)
#		define POCKET_CLANG_MAKE_VERSION(MAJOR, MINOR) (MAJOR * 100 + MINOR)
#		define POCKET_CLANG_VERSION POCKET_CLANG_MAKE_VERSION(__clang_major__, __clang_minor__)
#		define POCKET_CLANG_HAS_VERSION(MAJOR, MINOR) (POCKET_CLANG_VERSION >= POCKET_CLANG_MAKE_VERSION(MAJOR, MINOR))
#		define POCKET_CLANG_VERSION_IF(MAJOR, MINOR) (POCKET_CLANG_VERSION == POCKET_CLANG_MAKE_VERSION(MAJOR, MINOR))
#	else
#		define POCKET_CLANG_MAKE_VERSION(MAJOR, MINOR) (0)
#		define POCKET_CLANG_VERSION (0)
#		define POCKET_CLANG_HAS_VERSION(MAJOR, MINOR) (0)
#		define POCKET_CLANG_VERSION_IF(MAJOR, MINOR) (0)
#	endif // POCKET_COMPILER_IF(CLANG)
#endif // !POCKET_CLANG_MAKE_VERSION && !POCKET_CLANG_VERSION && !POCKET_CLANG_HAS_VERSION

//---------------------------------------------------------------------------------------
// VCバージョン
//---------------------------------------------------------------------------------------
#if !defined(POCKET_VC_MAKE_VERSION) && !defined(POCKET_VC_VERSION) && !defined(POCKET_VC_HAS_VERSION) && !defined(POCKET_VC_VERSION_IF)
#	if POCKET_COMPILER_IF(VC)
#		define POCKET_VC_MAKE_VERSION(MAJOR, MINOR) (MAJOR * 1000 + MINOR)
#		define POCKET_VC_VERSION (_MSC_FULL_VER)
#		define POCKET_VC_HAS_VERSION(MAJOR, MINOR) (POCKET_VC_VERSION >= POCKET_VC_MAKE_VERSION(MAJOR, MINOR))
#		define POCKET_VC_VERSION_IF(MAJOR, MINOR) (POCKET_VC_VERSION == POCKET_VC_MAKE_VERSION(MAJOR, MINOR))
#	else
#		define POCKET_VC_MAKE_VERSION(MAJOR, MINOR) (0)
#		define POCKET_VC_VERSION (0)
#		define POCKET_VC_HAS_VERSION(MAJOR, MINOR) (0)
#		define POCKET_VC_VERSION_IF(MAJOR, MINOR) (0)
#	endif // POCKET_COMPILER_IF(VC)
#endif // !POCKET_VC_MAKE_VERSION && !POCKET_VC_VERSION && !POCKET_VC_HAS_VERSION

//---------------------------------------------------------------------------------------
// コンパイラーとバージョン確認
//---------------------------------------------------------------------------------------
#ifndef POCKET_COMPILER_IF_WITH_HAS_VERSION
#	define POCKET_COMPILER_IF_WITH_HAS_VERSION(C, MAJOR, MINOR) (POCKET_COMPILER_IF(C) && POCKET_##C##_HAS_VERSION(MAJOR, MINOR))
#endif // POCKET_COMPILER_IF_WITH_HAS_VERSION

//---------------------------------------------------------------------------------------
// #pragma onceが使用できるか設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_PRAGMA_ONCE
#	if POCKET_COMPILER_IF_WITH_HAS_VERSION(VC, 1100, 0) || POCKET_COMPILER_IF_WITH_HAS_VERSION(GCC, 3, 4) || POCKET_COMPILER_IF(CLANG)
#		define POCKET_USE_PRAGMA_ONCE
#	endif
#endif // POCKET_USE_PRAGMA_ONCE

#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

//---------------------------------------------------------------------------------------
// 64bitか
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_X64
#	if defined(_WIN64) || defined(__x86_64__)
#		define POCKET_USE_X64
#	endif
#endif // POCKET_USE_X64

//---------------------------------------------------------------------------------------
// namespace pocketの使用
//---------------------------------------------------------------------------------------
#ifndef POCKET_USING_NS
#	define POCKET_USING_NS using namespace pocket;
#endif // POCKET_USING_NS

//---------------------------------------------------------------------------------------
// なにも使用しないように設定された
//---------------------------------------------------------------------------------------
#if defined(POCKET_NO_USING_MATH_INT_FLOAT) && !defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#error "request define POCKET_USING_MATH_DOUBLE or POCKET_USING_MATH_LONG_DOUBLE. or undefine POCKET_NO_USING_MATH_INT_FLOAT"
#endif // defined(POCKET_NO_USING_MATH_INT_FLOAT) && !defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)

//---------------------------------------------------------------------------------------
// __cplusplusでのバージョン定義
//---------------------------------------------------------------------------------------
#ifndef __POCKET_CXX98
#	define __POCKET_CXX98 (199711L)
#endif // __POCKET_CXX98
#ifndef __POCKET_CXX03
#	define __POCKET_CXX03 (199711L)
#endif // __POCKET_CXX03
#ifndef __POCKET_CXX11
#	define __POCKET_CXX11 (201103L)
#endif // __POCKET_CXX11
#ifndef __POCKET_CXX14
#	define __POCKET_CXX14 (201402L)
#endif // __POCKET_CXX14

//---------------------------------------------------------------------------------------
// VC++バージョン
//---------------------------------------------------------------------------------------
#ifndef __POCKET_VCXX05
#	define __POCKET_VCXX05 (1100)
#endif // __POCKET_VCXX05
#ifndef __POCKET_VCXX06
#	define __POCKET_VCXX06 (1200)
#endif // __POCKET_VCXX06
#ifndef __POCKET_VCXX07
#	define __POCKET_VCXX07 (1300)
#endif // __POCKET_VCXX07
#ifndef __POCKET_VCXX07_1
#	define __POCKET_VCXX07_1 (1310)
#endif // __POCKET_VCXX07_1
#ifndef __POCKET_VCXX08
#	define __POCKET_VCXX08 (1400)
#endif // __POCKET_VCXX08
#ifndef __POCKET_VCXX09
#	define __POCKET_VCXX09 (1500)
#endif // __POCKET_VCXX09
#ifndef __POCKET_VCXX10
#	define __POCKET_VCXX10 (1600)
#endif // __POCKET_VCXX10
#ifndef __POCKET_VCXX11
#	define __POCKET_VCXX11 (1700)
#endif // __POCKET_VCXX11
#ifndef __POCKET_VCXX12
#	define __POCKET_VCXX12 (1800)
#endif // __POCKET_VCXX12
#ifndef __POCKET_VCXX14
#	define __POCKET_VCXX14 (1900)
#endif // __POCKET_VCXX14
#ifndef __POCKET_VCXX15
#	define __POCKET_VCXX15 (2000)
#endif // __POCKET_VCXX15

//---------------------------------------------------------------------------------------
// C++バージョンチェック
//---------------------------------------------------------------------------------------
#if !defined(POCKET_CXX_HAS_VERSION) && !defined(POCKET_CXX_VERSION_IF)
#	ifdef __cplusplus
#		define POCKET_CXX_HAS_VERSION(N) (__cplusplus >= __POCKET_CXX##N)
#		define POCKET_CXX_VERSION_IF(N) (__cplusplus == __POCKET_CXX##N)
#	else
#		define POCKET_CXX_HAS_VERSION(N) (0)
#		define POCKET_CXX_VERSION_IF(N) (0)
#	endif // __cplusplus
#endif // POCKET_CXX_HAS_VERSION

//---------------------------------------------------------------------------------------
// VC++バージョンチェック
//---------------------------------------------------------------------------------------
#if !defined(POCKET_VCXX_HAS_VERSION) && !defined(POCKET_VCXX_VERSION_IF)
#	if POCKET_COMPILER_IF(VC)
#		define POCKET_VCXX_HAS_VERSION(N) (_MSC_VER >= __POCKET_VCXX##N)
#		define POCKET_VCXX_VERSION_IF(N) (_MSC_VER == __POCKET_VCXX##N)
#	else
#		define POCKET_VCXX_HAS_VERSION(N) (0)
#		define POCKET_VCXX_VERSION_IF(N) (0)
#	endif // POCKET_COMPILER_IF(VC)
#endif // POCKET_VCXX_HAS_VERSION

//---------------------------------------------------------------------------------------
// C++11が使用できるか設定. VC++の場合はVC++12以上
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_CXX11
#	if POCKET_VCXX_HAS_VERSION(12) || POCKET_CXX_HAS_VERSION(11) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#		define POCKET_USE_CXX11
#	endif //
#endif // POCKET_USE_CXX11

#ifdef POCKET_USE_CXX11
#include <utility>
#endif // POCKET_USE_CXX11

//---------------------------------------------------------------------------------------
// C++14が使用できるか設定. VC++の場合は不明
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_CXX14
#	if POCKET_VCXX_HAS_VERSION(15) || POCKET_CXX_HAS_VERSION(14)
#		define POCKET_USE_CXX14
#	endif //
#endif // POCKET_USE_CXX14

//---------------------------------------------------------------------------------------
// 匿名が使用できるか設定.
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_ANONYMOUS
#	if POCKET_COMPILER_IF(VC) || POCKET_COMPILER_IF_WITH_HAS_VERSION(GCC, 4, 5) || POCKET_COMPILER_IF_WITH_HAS_VERSION(CLANG, 3, 1)
#		define POCKET_USE_ANONYMOUS
#	endif // POCKET_COMPILER_IF(VC) || POCKET_COMPILER_IF_WITH_HAS_VERSION(GCC, 4, 5) || POCKET_COMPILER_IF_WITH_HAS_VERSION(CLANG, 3, 1)
#endif // POCKET_USE_ANONYMOUS

#ifndef POCKET_USE_ANONYMOUS_NON_POD
#	if POCKET_COMPILER_IF(VC)
#		define POCKET_USE_ANONYMOUS_NON_POD
#	else
#		if defined(POCKET_USE_ANONYMOUS) && POCKET_CXX_HAS_VERSION(11)
#			define POCKET_USE_ANONYMOUS_NON_POD
#		endif // defined(POCKET_USE_ANONYMOUS) && POCKET_CXX_HAS_VERSION(11)
#	endif // POCKET_COMPILER_IF(VC)
#endif // !POCKET_USE_ANONYMOUS_NON_POD

//---------------------------------------------------------------------------------------
// 匿名が使用できた場合の警告を消す＆使用を許可
//---------------------------------------------------------------------------------------
#ifdef POCKET_USE_ANONYMOUS
#	if POCKET_COMPILER_IF(VC)
#		pragma warning(disable: 4201)
#	endif // POCKET_COMPILER_IF(VC)
#endif // POCKET_USE_ANONYMOUS

//---------------------------------------------------------------------------------------
// 匿名使用時のコンストラクタ挙動が違った場合の設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#	ifndef POCKET_USE_ANONYMOUS_NON_POD
#		define POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#	else
#		if POCKET_COMPILER_IF(GCC) || POCKET_COMPILER_IF(CLANG)
#			define POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT
#		endif // POCKET_COMPILER_IF(GCC) || POCKET_COMPILER_IF(CLANG)
#	endif // POCKET_USE_ANONYMOUS_NON_POD
#endif // POCKET_USE_ANONYMOUS_NORMAL_CONSTRUCT

//---------------------------------------------------------------------------------------
// 匿名が使用できてC++11が使用できるか設定
//---------------------------------------------------------------------------------------
#if defined(POCKET_USE_ANONYMOUS) && defined(POCKET_USE_CXX11)
#	ifndef POCKET_USE_ANONYMOUS_CXX11
#		define POCKET_USE_ANONYMOUS_CXX11
#	endif // POCKET_USE_ANONYMOUS_CXX11
#endif // defined(POCKET_USE_ANONYMOUS) && defined(POCKET_USE_CXX11)

//---------------------------------------------------------------------------------------
// インライン化を強制させるための設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_INLINE_FORCE
#	if POCKET_COMPILER_IF(VC)
#		if POCKET_VCXX_HAS_VERSION(06)
#			define POCKET_INLINE_FORCE __forceinline
#		else
#			define POCKET_INLINE_FORCE __inline
#		endif
#	elif POCKET_COMPILER_IF(GCC)
#		define POCKET_INLINE_FORCE inline __attribute__((__always_inline__))
#	elif POCKET_COMPILER_IF(CLANG)
#		define POCKET_INLINE_FORCE __attribute__((__always_inline__))
#	else
#		define POCKET_INLINE_FORCE inline
#	endif // POCKET_COMPILER_IF
#endif // POCKET_INLINE_FORCE

//---------------------------------------------------------------------------------------
// インライン化を抑制するための設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_INLINE_NEVER
#	if POCKET_COMPILER_IF(VC)
#		define POCKET_INLINE_NEVER __declspec(noinline)
#	elif POCKET_COMPILER_IF(GCC) || POCKET_COMPILER_IF(CLANG)
#		define POCKET_INLINE_NEVER __attribute__((__noinline__))
#	else
#		define POCKET_INLINE_NEVER
#	endif // POCKET_COMPILER_IF
#endif // POCKET_INLINE_NEVER

//---------------------------------------------------------------------------------------
// C++11が使用できる場合は暗黙的な型変換演算子を利用できなくするための設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_CXX11_EXPLICIT
#	ifdef POCKET_USE_CXX11
#		define POCKET_CXX11_EXPLICIT explicit
#	else
#		define POCKET_CXX11_EXPLICIT
#	endif // POCKET_USE_CXX11
#endif // POCKET_CXX11_EXPLICIT

//---------------------------------------------------------------------------------------
// C++11が使用できる場合は戻り値の時にmoveを使用
//---------------------------------------------------------------------------------------
#ifndef POCKET_CXX11_MOVE
#	ifdef POCKET_USE_CXX11
#		define POCKET_CXX11_MOVE(V) std::move(V)
#	else
#		define POCKET_CXX11_MOVE(V) (V)
#	endif // POCKET_USE_CXX11
#endif // POCKET_CXX11_MOVE

//---------------------------------------------------------------------------------------
// constexprが使用できるか
//---------------------------------------------------------------------------------------
#ifndef POCKET_HAS_CONSTEXPR
#	if POCKET_VCXX_HAS_VERSION(14) || POCKET_CXX_HAS_VERSION(11)
#		define POCKET_HAS_CONSTEXPR
#	endif // POCKET_VCXX_HAS_VERSION(14) || POCKET_CXX_HAS_VERSION(11)
#endif // POCKET_HAS_CONSTEXPR

//---------------------------------------------------------------------------------------
// const または constexprを定義するための設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_CONST_OR_CONSTEXPR
#	ifdef POCKET_HAS_CONSTEXPR
#		define POCKET_CONST_OR_CONSTEXPR constexpr
#	else
#		define POCKET_CONST_OR_CONSTEXPR const
#	endif // POCKET_HAS_CONSTEXPR
#endif // POCKET_CONST_OR_CONSTEXPR

//---------------------------------------------------------------------------------------
// C++11のconstexprが使用できるか
//---------------------------------------------------------------------------------------
#ifndef POCKET_CXX11_CONSTEXPR
#	ifdef POCKET_HAS_CONSTEXPR
#		define POCKET_CXX11_CONSTEXPR constexpr
#	else
#		define POCKET_CXX11_CONSTEXPR
#	endif // POCKET_HAS_CONSTEXPR
#endif // POCKET_CXX11_CONSTEXPR

//---------------------------------------------------------------------------------------
// C++14のconstexprが使用できるか
//---------------------------------------------------------------------------------------
#ifndef POCKET_CXX14_CONSTEXPR
#	if POCKET_VCXX_HAS_VERSION(15) || POCKET_CXX_HAS_VERSION(14)
#		define POCKET_CXX14_CONSTEXPR constexpr
#	else
#		define POCKET_CXX14_CONSTEXPR
#	endif
#endif // POCKET_CXX14_CONSTEXPR

//---------------------------------------------------------------------------------------
// 定数の定義
//---------------------------------------------------------------------------------------
#ifndef POCKET_STATICAL_CONSTANT
#	define POCKET_STATICAL_CONSTANT static POCKET_CONST_OR_CONSTEXPR
#endif // POCKET_STATICAL_CONSTANT

//---------------------------------------------------------------------------------------
// 構造体メンバーへのオフセット
//---------------------------------------------------------------------------------------
#ifndef POCKET_MEMBER_OFFSETOF
#	define POCKET_MEMBER_OFFSETOF(TYPE, MEM) reinterpret_cast<size_t>(&(reinterpret_cast<const TYPE*>(0)->MEM))
#endif // POCKET_MEMBER_OFFSETOF

//---------------------------------------------------------------------------------------
// 構造体メンバーの型サイズ
//---------------------------------------------------------------------------------------
#ifndef POCKET_MEMBER_SIZEOF
#	ifdef POCKET_USE_CXX11
#		define POCKET_MEMBER_SIZEOF(TYPE, MEM) sizeof(TYPE :: MEM)
#	else
#		define POCKET_MEMBER_SIZEOF(TYPE, MEM) sizeof(reinterpret_cast<const volatile TYPE*>(0)->MEM)
#	endif // POCKET_USE_CXX11
#endif // POCKET_MEMBER_SIZEOF

//---------------------------------------------------------------------------------------
// スコープ付き列挙型. NAME + NAME##_tを定義
//---------------------------------------------------------------------------------------
#ifndef POCKET_SCOPED_ENUM_T
#	ifdef POCKET_USE_CXX11
#		define POCKET_SCOPED_ENUM_T(NAME, ...) enum class NAME {\
		__VA_ARGS__\
	};\
	typedef NAME NAME##_t
#	else
#		define POCKET_SCOPED_ENUM_T(NAME, ...) struct NAME {\
		enum type {\
			__VA_ARGS__\
		};\
	};\
	typedef NAME::type NAME##_t
#	endif // POCKET_USE_CXX11
#endif // POCKET_SCOPED_ENUM_T

//---------------------------------------------------------------------------------------
// デフォルトコンストラクタを定義するための設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_DEFAULT_DECLARE_RVALUES
#	if POCKET_VCXX_HAS_VERSION(14) || POCKET_CXX_HAS_VERSION(11)
#		define POCKET_DEFAULT_DECLARE_RVALUES(CLASS) CLASS(CLASS&&)=default; CLASS& operator=(CLASS&&)=default
#	else
#		define POCKET_DEFAULT_DECLARE_RVALUES(CLASS)
#	endif //
#endif // POCKET_DEFAULT_DECLARE_RVALUES

#ifndef POCKET_DEFAULT_CONSTRUCTOR
#	ifdef POCKET_USE_CXX11
#		define POCKET_DEFAULT_CONSTRUCTOR(CLASS) CLASS()=default; CLASS(const CLASS&)=default; CLASS& operator=(const CLASS&)=default; POCKET_DEFAULT_DECLARE_RVALUES(CLASS)
#	else
#		define POCKET_DEFAULT_CONSTRUCTOR(CLASS) CLASS() {}
#	endif // POCKET_USE_CXX11
#endif // POCKET_DEFAULT_CONSTRUCTOR

//---------------------------------------------------------------------------------------
// 静的なアサーションをするための設定. msgはすべてつなげて書く
//---------------------------------------------------------------------------------------
#ifndef POCKET_STATICAL_ASSERT
#	ifdef POCKET_USE_CXX11
#		define POCKET_STATICAL_ASSERT(b, msg) static_assert(b, #msg)
#	else
#		define POCKET_STATICAL_ASSERT(b, msg) typedef int __##msg[(b) ? 1 : -1]
#	endif
#endif // POCKET_STATICAL_ASSERT

//---------------------------------------------------------------------------------------
// アライメントの指定
//---------------------------------------------------------------------------------------
#ifndef POCKET_ALIGNED
#	if POCKET_COMPILER_IF(VC)
#		if POCKET_VCXX_HAS_VERSION(14)
#			define POCKET_ALIGNED(N) alignas((N))
#		else
#			define POCKET_ALIGNED(N) __declspec(align(N))
#		endif
#	else
#		ifdef POCKET_USE_CXX11
#			define POCKET_ALIGNED(N) alignas((N))
#		else
#			if POCKET_COMPILER_IF(GCC) || POCKET_COMPILER_IF(CLANG)
#				define POCKET_ALIGNED(N) __attribute__((aligned((N))))
#			else
#				define POCKET_ALIGNED(N)
#			endif
#		endif
#	endif
#endif // POCKET_ALIGNED

//---------------------------------------------------------------------------------------
// アライメントの際に適切なサイズを指定
//---------------------------------------------------------------------------------------
#ifndef POCKET_ALIGNED_DECL
#	if POCKET_COMPILER_IF(VC)
#		if POCKET_VCXX_HAS_VERSION(14)
#			define POCKET_ALIGNED_DECL(IDENTIFIER, N) IDENTIFIER POCKET_ALIGNED(N)
#		else
#			define POCKET_ALIGNED_DECL(IDENTIFIER, N) POCKET_ALIGNED(N) IDENTIFIER
#		endif
#	else
#		define POCKET_ALIGNED_DECL(IDENTIFIER, N) IDENTIFIER POCKET_ALIGNED(N)
#	endif
#endif // POCKET_ALIGNED_DECL

#ifndef POCKET_ALIGNED_STRUCT
#	define POCKET_ALIGNED_STRUCT(N) POCKET_ALIGNED_DECL(struct, N)
#endif // POCKET_ALIGNED_STRUCT
#ifndef POCKET_ALIGNED_CLASS
#	define POCKET_ALIGNED_CLASS(N) POCKET_ALIGNED_DECL(class, N)
#endif // POCKET_ALIGNED_CLASS

//---------------------------------------------------------------------------------------
// 数学クラスで使用する静的アサーションの設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_MATH_STATICAL_ASSERT
#	ifdef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_STATICAL_ASSERT(TYPE)
#	else
#		ifdef POCKET_NO_USING_MATH_INT_FLOAT
#			if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_or_longdouble_only)
#			elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, double_only)
#			elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, longdouble_only)
#			endif
#		else
#			if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_or_longdouble_only)
#			elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_double_only)
#			elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_or_longdouble_only)
#			else
#				define POCKET_MATH_STATICAL_ASSERT(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_type< TYPE >::value, int_or_float_only)
#			endif
#		endif
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_STATICAL_ASSERT

//---------------------------------------------------------------------------------------
// 上記の浮動小数点の設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_MATH_STATICAL_ASSERT_FLOATING
#	ifdef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE)
#	else
#		ifdef POCKET_NO_USING_MATH_INT_FLOAT
#			define POCKET_MATH_STATICAL_ASSERT_FLOATING POCKET_MATH_STATICAL_ASSERT
#		else
#			if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_or_longdouble_only)
#			elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_double_only)
#			elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_or_longdouble_only)
#			else
#				define POCKET_MATH_STATICAL_ASSERT_FLOATING(TYPE) POCKET_STATICAL_ASSERT(type_traits::is_math_floating_type< TYPE >::value, float_only)
#			endif
#		endif
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_STATICAL_ASSERT

//---------------------------------------------------------------------------------------
// 配列サイズ
//---------------------------------------------------------------------------------------
#ifndef POCKET_ARRAY_SIZE
#	define POCKET_ARRAY_SIZE(ARY) (sizeof(ARY) / sizeof(ARY[0]))
#endif // POCKET_ARRAY_SIZE

//---------------------------------------------------------------------------------------
// 引数用配列参照(シンタックスハイライト解除回避用)
//---------------------------------------------------------------------------------------
#ifndef POCKET_REF_ARRAY_ARG
#	define POCKET_REF_ARRAY_ARG(TYPE, NAME, N) TYPE (&NAME)[N]
#endif // POCKET_REF_ARRAY_ARG
#ifndef POCKET_CREF_ARRAY_ARG
#	define POCKET_CREF_ARRAY_ARG(TYPE, NAME, N) POCKET_REF_ARRAY_ARG(const TYPE, NAME, N)
#endif // POCKET_CREF_ARRAY_ARG

//---------------------------------------------------------------------------------------
// テンプレート引数用array, vector
//---------------------------------------------------------------------------------------
#ifndef POCKET_TEMPLATE_PARAM_ARRAY
#	define POCKET_TEMPLATE_PARAM_ARRAY(N_NAME, NAME) size_t N_NAME, template <typename, size_t> class NAME
;
#endif // POCKET_TEMPLATE_PARAM_ARRAY
#ifndef POCKET_TEMPLATE_PARAM_VECTOR
#	define POCKET_TEMPLATE_PARAM_VECTOR(ALLOC_NAME, NAME) typename ALLOC_NAME, template <typename, typename> class NAME
;
#endif // POCKET_TEMPLATE_PARAM_VECTOR

//---------------------------------------------------------------------------------------
// SIMD種類
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// SIMD設定
//---------------------------------------------------------------------------------------
#if !defined(POCKET_USE_SIMD) && !defined(POCKET_USE_SIMD_TYPE) && !defined(POCKET_UNUSING_SIMD)
#	if defined(__AVX2__) || defined(__AVX__) // AVXは共通処理
#		define POCKET_USE_SIMD
#		ifdef __AVX2__
#			define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_AVX2
#		else
#			define POCKET_USE_SIMD_TYPE POCKET_SIMD_TYPE_AVX
#		endif // __AVX2__
#	elif POCKET_COMPILER_IF(VC)
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
#	elif POCKET_COMPILER_IF(GCC) || POCKET_COMPILER_IF(CLANG)
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

//---------------------------------------------------------------------------------------
// SIMDと無名が使用できるか
//---------------------------------------------------------------------------------------
#ifndef POCKET_USE_SIMD_ANONYMOUS
#	if defined(POCKET_USE_SIMD) && defined(POCKET_USE_ANONYMOUS)
#		define POCKET_USE_SIMD_ANONYMOUS
#	endif // POCKET_USE_SIMD && POCKET_USE_ANONYMOUS
#endif // POCKET_USE_SIMD_ANONYMOUS

//---------------------------------------------------------------------------------------
// 128と256 が使用できるか
//---------------------------------------------------------------------------------------
#ifdef POCKET_USE_SIMD
#	ifndef POCKET_USE_SIMD_128 // 128bitが使用できる
#		if POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_SSE2
#			define POCKET_USE_SIMD_128
#		endif // POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_SSE2
#	endif // POCKET_USE_SIMD_128
#	ifndef POCKET_USE_SIMD_256 // 256btiが使用できる
#		if POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_AVX
#			define POCKET_USE_SIMD_256
#		endif // POCKET_USE_SIMD_TYPE >= POCKET_SIMD_TYPE_AVX
#	endif // POCKET_USE_SIMD_256
#endif // POCKET_USE_SIMD

//---------------------------------------------------------------------------------------
// SIMDのタイプを文字列定義
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// テンプレートクラスの暗黙定義を抑制するための設定. 使用するにはPOCKET_USING_MATH_EXTERN_TYPEを定義する
//---------------------------------------------------------------------------------------
#ifndef POCKET_MATH_EXTERN_TYPE
#	ifndef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_EXTERN_TYPE(TYPE)
#	else
#		ifdef POCKET_USE_CXX11
#			ifdef POCKET_NO_USING_MATH_INT_FLOAT
#				if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>
#				elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<int>;\
												extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
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
#				if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE)
#				elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_TYPE(TYPE) extern template struct TYPE<long double>
#				elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
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

//---------------------------------------------------------------------------------------
// 上記の浮動小数点の設定
//---------------------------------------------------------------------------------------
#ifndef POCKET_MATH_EXTERN_FLOATING_TYPE
#	ifndef POCKET_USING_MATH_EXTERN_TYPE
#		define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE)
#	else
#		ifdef POCKET_USE_CXX11
#			ifdef POCKET_NO_USING_MATH_INT_FLOAT
#				if defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>
#				elif defined(POCKET_USING_MATH_DOUBLE) && !defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<long double>
#				elif !defined(POCKET_USING_MATH_DOUBLE) && defined(POCKET_USING_MATH_LONG_DOUBLE)
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>
#				else
#					define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE) extern template struct TYPE<float>;\
												extern template struct TYPE<double>;\
												extern template struct TYPE<long double>
#				endif
#			else // POCKET_NO_USING_MATH_INT_FLOAT
#				define POCKET_MATH_EXTERN_FLOATING_TYPE POCKET_MATH_EXTERN_TYPE
#			endif // POCKET_NO_USING_MATH_INT_FLOAT
#		else
#			define POCKET_MATH_EXTERN_FLOATING_TYPE(TYPE)
#		endif // POCKET_USE_CXX11
#	endif // POCKET_USING_MATH_EXTERN_TYPE
#endif // POCKET_MATH_EXTERN_TYPE

#endif // __POCKET_CONFIG_H__
