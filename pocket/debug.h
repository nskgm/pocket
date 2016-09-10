#ifndef __POCKET_DEBUG_H__
#define __POCKET_DEBUG_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#if (defined(_DEBUG) || defined(DEBUG) || defined(POCKET_USE_DEBUG)) && !defined(NDEBUG)
#	ifndef POCKET_DEBUG
#		define POCKET_DEBUG
#	endif // POCKET_DEBUG
#include <cassert>
#include <iostream>
#endif // defined(_DEBUG) || defined(DEBUG)

/*---------------------------------------------------------------------
* デバッグ時のみ判定
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_ASSERT
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_ASSERT assert
#	else
#		define POCKET_DEBUG_ASSERT(c)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_ASSERT

/*---------------------------------------------------------------------
* 範囲判定
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_RANGE_ASSERT
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_RANGE_ASSERT(N, MIN, MAX) assert((N) >= (MIN) && (N) <= (MAX) && "out of range")
#	else
#		define POCKET_DEBUG_RANGE_ASSERT(N, MIN, MAX)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_RANGE_ASSERT

/*---------------------------------------------------------------------
* デバッグ時のみストリームに対して出力
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STREAM
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STREAM(stream, format) stream << format
#	else
#		define POCKET_DEBUG_OUTPUT_STREAM(stream, format)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STREAM

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STREAM_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STREAM_ONCE(stream, value) POCKET_DEBUG_OUTPUT_STREAM(stream, (value) << std::endl)
#	else
#		define POCKET_DEBUG_OUTPUT_STREAM_ONCE(stream, value)
#	endif
#endif // POCKET_DEBUG_OUTPUT_STREAM_ONCE

/*---------------------------------------------------------------------
* 標準出力へデバッグ時のみ出力
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STDOUT
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDOUT(format) POCKET_DEBUG_OUTPUT_STREAM(std::cout, format)
#	else
#		define POCKET_DEBUG_OUTPUT_STDOUT(format)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDOUT

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STDOUT_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDOUT_ONCE(value) POCKET_DEBUG_OUTPUT_STREAM_LINE(std::cout, value)
#	else
#		define POCKET_DEBUG_OUTPUT_STDOUT_ONCE(value)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDOUT_ONCE

/*---------------------------------------------------------------------
* 標準エラー出力へデバッグ時のみ出力, 即出力される
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STDERR
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDERR(format) POCKET_DEBUG_OUTPUT_STREAM(std::cerr, format)
#	else
#		define POCKET_DEBUG_OUTPUT_STDERR(format)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDERR

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef POCKET_DEBUG_OUTPUT_STDERR_ONCE
#	ifdef POCKET_DEBUG
#		define POCKET_DEBUG_OUTPUT_STDERR_ONCE(value) POCKET_DEBUG_OUTPUT_STREAM_LINE(std::cerr, value)
#	else
#		define POCKET_DEBUG_OUTPUT_STDERR_ONCE(value)
#	endif // POCKET_DEBUG
#endif // POCKET_DEBUG_OUTPUT_STDERR_ONCE

#endif // __POCKET_DEBUG_H__
