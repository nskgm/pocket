#ifndef __MATH_DEBUG_H__
#define __MATH_DEBUG_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#if defined(_DEBUG) || defined(DEBUG)
#	ifndef __DEBUG
#		define __DEBUG
#	endif // __DEBUG
#include <cassert>
#include <iostream>
#endif // defined(_DEBUG) || defined(DEBUG)

/*---------------------------------------------------------------------
* デバッグ時のみ判定
*---------------------------------------------------------------------*/
#ifndef _DEB_ASSERT
#	ifdef __DEBUG
#		define _DEB_ASSERT assert
#	else
#		define _DEB_ASSERT(c)
#	endif // __DEBUG
#endif // _DEB_ASSERT

/*---------------------------------------------------------------------
* 範囲判定
*---------------------------------------------------------------------*/
#ifndef _DEB_RANGE_ASSERT
#	ifdef __DEBUG
#		define _DEB_RANGE_ASSERT(N, MIN, MAX) assert(N >= (MIN) && N <= (MAX) && "out of range")
#	else
#		define _DEB_RANGE_ASSERT(N, MIN, MAX)
#	endif // __DEBUG
#endif // _DEB_RANGE_ASSERT

/*---------------------------------------------------------------------
* デバッグ時のみストリームに対して出力
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT
#	ifdef __DEBUG
#		define _DEB_OUTPUT(stream, format) stream << format
#	else
#		define _DEB_OUTPUT(stream, format)
#	endif // __DEBUG
#endif // _DEB_OUTPUT

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT_LINE
#	ifdef __DEBUG
#		define _DEB_OUTPUT_LINE(stream, value) _DEB_OUTPUT(stream, (value) << std::endl)
#	else
#		define _DEB_OUTPUT_LINE(stream, value)
#	endif
#endif // _DEB_OUTPUT_LINE

/*---------------------------------------------------------------------
* 標準出力へデバッグ時のみ出力
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT_STDOUT
#	ifdef __DEBUG
#		define _DEB_OUTPUT_STDOUT(format) _DEB_OUTPUT(std::cout, format)
#	else
#		define _DEB_OUTPUT_STDOUT(format)
#	endif // __DEBUG
#endif // _DEB_OUTPUT_STDOUT

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT_STDOUT_LINE
#	ifdef __DEBUG
#		define _DEB_OUTPUT_STDOUT_LINE(value) _DEB_OUTPUT_LINE(std::cout, value)
#	else
#		define _DEB_OUTPUT_STDOUT_LINE(value)
#	endif // __DEBUG
#endif // _DEB_OUTPUT_STDOUT_LINE

/*---------------------------------------------------------------------
* 標準エラー出力へデバッグ時のみ出力, 即出力される
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT_STDERR
#	ifdef __DEBUG
#		define _DEB_OUTPUT_STDERR(format) _DEB_OUTPUT(std::cerr, format)
#	else
#		define _DEB_OUTPUT_STDERR(format)
#	endif // __DEBUG
#endif // _DEB_OUTPUT_STDERR

/*---------------------------------------------------------------------
* 改行付き
*---------------------------------------------------------------------*/
#ifndef _DEB_OUTPUT_STDERR_LINE
#	ifdef __DEBUG
#		define _DEB_OUTPUT_STDERR_LINE(value) _DEB_OUTPUT_LINE(std::cerr, value)
#	else
#		define _DEB_OUTPUT_STDERR_LINE(value)
#	endif // __DEBUG
#endif // _DEB_OUTPUT_STDERR_LINE

#endif // __MATH_DEBUG_H__
