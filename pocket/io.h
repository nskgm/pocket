#ifndef __POCKET_IO_H__
#define __POCKET_IO_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include <iostream>
#include <limits>

namespace pocket
{
namespace out_char
{
#ifndef _DECL_OUT_CHAR_FUNCTION
#define _DECL_OUT_CHAR_FUNCTION(NAME, CHAR) template <typename CharT, typename CharTraits> inline \
		std::basic_ostream<CharT, CharTraits>& NAME(std::basic_ostream<CharT, CharTraits>& os) \
	{\
		os.put(os.widen((CHAR)));\
		return os;\
	}\
	template <typename CharT, typename CharTraits> inline \
		std::basic_iostream<CharT, CharTraits>& NAME(std::basic_iostream<CharT, CharTraits>& os) \
	{\
		os.put(os.widen((CHAR)));\
		return os;\
	}
#endif // _DECL_OUT_CHAR_FUNCTION

_DECL_OUT_CHAR_FUNCTION(space, ' ');
_DECL_OUT_CHAR_FUNCTION(line, '\n');
_DECL_OUT_CHAR_FUNCTION(parentheses_left, '(');
_DECL_OUT_CHAR_FUNCTION(parentheses_right, ')');
_DECL_OUT_CHAR_FUNCTION(braces_left, '{');
_DECL_OUT_CHAR_FUNCTION(braces_right, '}');
_DECL_OUT_CHAR_FUNCTION(box_brackets_left, '[');
_DECL_OUT_CHAR_FUNCTION(box_brackets_right, ']');
_DECL_OUT_CHAR_FUNCTION(angle_brackets_left, '<');
_DECL_OUT_CHAR_FUNCTION(angle_brackets_right, '>');
_DECL_OUT_CHAR_FUNCTION(comma, ',');
_DECL_OUT_CHAR_FUNCTION(tab, '\t');
_DECL_OUT_CHAR_FUNCTION(carriage_return, '\r');
_DECL_OUT_CHAR_FUNCTION(single_quotation, '\'');
_DECL_OUT_CHAR_FUNCTION(double_quotation, '\"');
_DECL_OUT_CHAR_FUNCTION(quation, '\?');
_DECL_OUT_CHAR_FUNCTION(exclamation, '!');
_DECL_OUT_CHAR_FUNCTION(colon, ':');
_DECL_OUT_CHAR_FUNCTION(semicolon, ';');

// [, ]
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& comma_space(std::basic_ostream<CharT, CharTraits>& os)
{
	os << comma << space;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& comma_space(std::basic_iostream<CharT, CharTraits>& os)
{
	os << comma << space;
	return os;
}

#undef _DECL_OUT_CHAR_FUNCTION
} // namespace out_char

namespace skip_char
{
// 指定の文字までスキップ
template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& ignore(std::basic_istream<CharT, CharTraits>& is, CharT delim)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& ignore(std::basic_iostream<CharT, CharTraits>& is, CharT delim)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}
} // namespace skip_char
} // namespace pocket

#endif // __POCKET_IO_H__
