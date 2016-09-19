#ifndef __POCKET_DEBUG_H__
#define __POCKET_DEBUG_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

//---------------------------------------------------------------------
// デバッグを使用するか
//---------------------------------------------------------------------
#if (defined(_DEBUG) || defined(DEBUG) || defined(POCKET_USE_DEBUG)) && !defined(NDEBUG)
#	ifndef POCKET_DEBUG
#		define POCKET_DEBUG
#	endif // POCKET_DEBUG
#	if POCKET_COMPILER_IF(VC)
#include <crtdbg.h>
#	else
#include <cassert>
#	endif // POCKET_COMPILER_IF
#include <iostream>
#endif // defined(_DEBUG) || defined(DEBUG)

//---------------------------------------------------------------------
// デバッグ時のみ判定
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_ASSERT
#	ifdef POCKET_DEBUG
#		if POCKET_COMPILER_IF(VC)
#			define POCKET_DEBUG_ASSERT _ASSERTE
#		else
#			define POCKET_DEBUG_ASSERT assert
#		endif
#	else
#		define POCKET_DEBUG_ASSERT(COND)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_ASSERT

//---------------------------------------------------------------------
// デバッグ時のみ判定
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_ASSERT_MSG
#	ifdef POCKET_DEBUG
#		if POCKET_COMPILER_IF(VC)
#			define POCKET_DEBUG_ASSERT_MSG(COND, MSG) _ASSERT_EXPR((COND), MSG)
#		else
#			define POCKET_DEBUG_ASSERT_MSG(COND, MSG) assert((COND) && (MSG))
#		endif
#	else
#		define POCKET_DEBUG_ASSERT_MSG(COND, MSG)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_ASSERT_MSG

//---------------------------------------------------------------------
// 範囲判定
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_RANGE_ASSERT
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_RANGE_ASSERT(N, MIN, MAX) POCKET_DEBUG_ASSERT_MSG(((N) >= (MIN) && (N) <= (MAX)), "out of range")
#	else
#		define POCKET_DEBUG_RANGE_ASSERT(N, MIN, MAX)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_RANGE_ASSERT

//---------------------------------------------------------------------
// デバッグ時のみストリームに対して出力
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STREAM
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STREAM(STREAM, FORMAT) STREAM << FORMAT
#	else
#		define POCKET_DEBUG_OUTPUT_STREAM(STREAM, FORMAT)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STREAM

//---------------------------------------------------------------------
// 改行付き
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STREAM_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STREAM_ONCE(STREAM, VALUE) POCKET_DEBUG_OUTPUT_STREAM(STREAM, (VALUE) << std::endl)
#	else
#		define POCKET_DEBUG_OUTPUT_STREAM_ONCE(STREAM, VALUE)
#	endif
#endif // POCKET_DEBUG_OUTPUT_STREAM_ONCE

//---------------------------------------------------------------------
// 標準出力へデバッグ時のみ出力
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STDOUT
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDOUT(FORMAT) POCKET_DEBUG_OUTPUT_STREAM(std::cout, FORMAT)
#	else
#		define POCKET_DEBUG_OUTPUT_STDOUT(FORMAT)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDOUT

//---------------------------------------------------------------------
// 改行付き
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STDOUT_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDOUT_ONCE(VALUE) POCKET_DEBUG_OUTPUT_STREAM_LINE(std::cout, VALUE)
#	else
#		define POCKET_DEBUG_OUTPUT_STDOUT_ONCE(VALUE)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDOUT_ONCE

//---------------------------------------------------------------------
// 標準エラー出力へデバッグ時のみ出力, 即出力される
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STDERR
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDERR(FORMAT) POCKET_DEBUG_OUTPUT_STREAM(std::cerr, FORMAT)
#	else
#		define POCKET_DEBUG_OUTPUT_STDERR(FORMAT)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDERR

//---------------------------------------------------------------------
// 改行付き
//---------------------------------------------------------------------
#ifndef POCKET_DEBUG_OUTPUT_STDERR_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDERR_ONCE(VALUE) POCKET_DEBUG_OUTPUT_STREAM_LINE(std::cerr, VALUE)
#	else
#		define POCKET_DEBUG_OUTPUT_STDERR_ONCE(VALUE)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDERR_ONCE

#endif // __POCKET_DEBUG_H__
