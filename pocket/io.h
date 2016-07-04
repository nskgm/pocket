#ifndef __POCKET_IO_H__
#define __POCKET_IO_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "type_traits.h"
#include <iostream>
#include <limits>

namespace pocket
{
namespace io
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

template <typename CharT>
struct ignore_delimiter_t
{
	typedef CharT char_type;

	char_type delimiter;

	ignore_delimiter_t(char_type d) :
		delimiter(d)
	{

	}
};
struct ignore_count_t
{
	std::streamsize count;

	ignore_count_t(std::streamsize c) :
		count(c)
	{

	}
};

// 指定の文字までスキップ
//template <typename CharT, typename CharTraits> inline
//std::basic_istream<CharT, CharTraits>& ignore(std::basic_istream<CharT, CharTraits>& is, CharT delim)
//{
//	return is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
//}
//template <typename CharT, typename CharTraits> inline
//std::basic_iostream<CharT, CharTraits>& ignore(std::basic_iostream<CharT, CharTraits>& is, CharT delim)
//{
//	return is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
//}

namespace detail
{
template <typename T>
struct is_char : type_traits::false_type
{

};
template <>
struct is_char<char> : type_traits::true_type
{

};
template <>
struct is_char<wchar_t> : type_traits::true_type
{

};

template <typename CharT> inline
ignore_delimiter_t<CharT> ignore_impl(CharT d, type_traits::true_type)
{
	return ignore_delimiter_t<CharT>(d);
}
template <typename T> inline
ignore_count_t ignore_impl(T c, type_traits::false_type)
{
	return ignore_count_t(static_cast<std::streamsize>(c));
}

template <typename CharT>
struct conditional_ignore
{
	typedef typename type_traits::remove_cv_reference<CharT>::type literal_char_type;
	typedef is_char<literal_char_type> _is_char;
	typedef typename type_traits::conditional<_is_char::value, ignore_delimiter_t<literal_char_type>, ignore_count_t>::type type;
};
}

/*---------------------------------------------------------------------
* ignore
*---------------------------------------------------------------------*/
template <typename CharT> inline
typename detail::conditional_ignore<CharT>::type ignore(CharT c)
{
	typedef typename detail::conditional_ignore<CharT>::_is_char _is_char;
	return detail::ignore_impl(c, _is_char());
}

template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, const ignore_delimiter_t<CharT>& ig)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), ig.delimiter);
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, const ignore_delimiter_t<CharT>& ig)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), ig.delimiter);
}
template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, const ignore_count_t& ig)
{
	return is.ignore(ig.count, CharTraits::eof());
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, const ignore_count_t& ig)
{
	return is.ignore(ig.count, CharTraits::eof());
}

} // namespace io
} // namespace pocket

#endif // __POCKET_IO_H__
