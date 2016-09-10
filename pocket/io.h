#ifndef __POCKET_IO_H__
#define __POCKET_IO_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "type_traits.h"
#include <iostream>
#include <limits>

namespace pocket
{
namespace io
{
#ifndef __POCKET_DECL_OUT_CHAR_FUNCTION
#define __POCKET_DECL_OUT_CHAR_FUNCTION(NAME, CHAR) template <typename CharT, typename CharTraits> inline \
		std::basic_ostream<CharT, CharTraits>& NAME(std::basic_ostream<CharT, CharTraits>& os) \
	{\
		os.put(os.widen((CHAR)));\
		return os;\
	}
#endif // __POCKET_DECL_OUT_CHAR_FUNCTION

__POCKET_DECL_OUT_CHAR_FUNCTION(bel, '\7');
__POCKET_DECL_OUT_CHAR_FUNCTION(space, ' ');
__POCKET_DECL_OUT_CHAR_FUNCTION(line_feed, '\n');
__POCKET_DECL_OUT_CHAR_FUNCTION(parentheses_left, '(');
__POCKET_DECL_OUT_CHAR_FUNCTION(parentheses_right, ')');
__POCKET_DECL_OUT_CHAR_FUNCTION(braces_left, '{');
__POCKET_DECL_OUT_CHAR_FUNCTION(braces_right, '}');
__POCKET_DECL_OUT_CHAR_FUNCTION(box_brackets_left, '[');
__POCKET_DECL_OUT_CHAR_FUNCTION(box_brackets_right, ']');
__POCKET_DECL_OUT_CHAR_FUNCTION(angle_brackets_left, '<');
__POCKET_DECL_OUT_CHAR_FUNCTION(angle_brackets_right, '>');
__POCKET_DECL_OUT_CHAR_FUNCTION(comma, ',');
__POCKET_DECL_OUT_CHAR_FUNCTION(tab, '\t');
__POCKET_DECL_OUT_CHAR_FUNCTION(carriage_return, '\r');
__POCKET_DECL_OUT_CHAR_FUNCTION(single_quotation, '\'');
__POCKET_DECL_OUT_CHAR_FUNCTION(double_quotation, '\"');
__POCKET_DECL_OUT_CHAR_FUNCTION(quation, '\?');
__POCKET_DECL_OUT_CHAR_FUNCTION(exclamation, '!');
__POCKET_DECL_OUT_CHAR_FUNCTION(colon, ':');
__POCKET_DECL_OUT_CHAR_FUNCTION(semicolon, ';');
__POCKET_DECL_OUT_CHAR_FUNCTION(sharp, '#');
__POCKET_DECL_OUT_CHAR_FUNCTION(dollars, '$');
__POCKET_DECL_OUT_CHAR_FUNCTION(percent, '%');
__POCKET_DECL_OUT_CHAR_FUNCTION(asterisk, '*');

#undef __POCKET_DECL_OUT_CHAR_FUNCTION

// [, ]
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& comma_space(std::basic_ostream<CharT, CharTraits>& os)
{
	os << comma << space;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& tab2(std::basic_ostream<CharT, CharTraits>& os)
{
	os << tab << tab;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& tab3(std::basic_ostream<CharT, CharTraits>& os)
{
	os << tab2 << tab;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& tab4(std::basic_ostream<CharT, CharTraits>& os)
{
	os << tab3 << tab;
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& tab5(std::basic_ostream<CharT, CharTraits>& os)
{
	os << tab4 << tab;
	return os;
}

/*---------------------------------------------------------------------
* widen
*---------------------------------------------------------------------*/

template <typename T>
struct widen_holder;

template <>
struct widen_holder<char>
{
	char c;

	widen_holder(char c) :
		c(c)
	{

	}

	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& apply(std::basic_ostream<CharT, CharTraits>& os) const
	{
		os.put(os.widen(c));
		return os;
	}
};
template <>
struct widen_holder<const char*>
{
	const char* str;

	widen_holder(const char* s) :
		str(s)
	{

	}

	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& apply(std::basic_ostream<CharT, CharTraits>& os) const
	{
		const char* s = str;
		while (*s != '\0')
		{
			os.put(os.widen(*s));
			++s;
		}
		return os;
	}
};
template <std::size_t N>
struct widen_holder<const char[N]>
{
	const char* str;

	widen_holder(const char(&s)[N]) :
		str(&s[0])
	{

	}

	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& apply(std::basic_ostream<CharT, CharTraits>& os) const
	{
		const char* s = str;
		while (*s != '\0')
		{
			os.put(os.widen(*s));
			++s;
		}
		return os;
	}
};
template <>
struct widen_holder<bool>
{
	bool cond;

	widen_holder(bool c) :
		cond(c)
	{

	}

	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& apply(std::basic_ostream<CharT, CharTraits>& os) const
	{
		const char* s = cond ? "true" : "false";
		while (*s != '\0')
		{
			os.put(os.widen(*s));
			++s;
		}
		return os;
	}
};

template <typename T> inline
widen_holder<T> widen(T s)
{
	return widen_holder<T>(s);
}

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const widen_holder<T>& holder)
{
	return holder.apply(os);
}

/*---------------------------------------------------------------------
* ignore
*---------------------------------------------------------------------*/

template <typename CharT>
struct ignore_delimiter
{
	typedef CharT char_type;

	char_type delimiter;

	ignore_delimiter(char_type d) :
		delimiter(d)
	{

	}
};
struct ignore_count
{
	std::streamsize count;

	ignore_count(std::streamsize c) :
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
ignore_delimiter<CharT> ignore_impl(CharT d, type_traits::true_type)
{
	return ignore_delimiter<CharT>(d);
}
template <typename T> inline
ignore_count ignore_impl(T c, type_traits::false_type)
{
	return ignore_count(static_cast<std::streamsize>(c));
}

template <typename CharT>
struct conditional_ignore
{
	typedef typename type_traits::remove_cv_reference<CharT>::type literal_char_type;
	typedef is_char<literal_char_type> _is_char;
	typedef typename type_traits::conditional<_is_char::value, ignore_delimiter<literal_char_type>, ignore_count>::type type;
};
}

template <typename CharT> inline
typename detail::conditional_ignore<CharT>::type ignore(CharT c)
{
	typedef typename detail::conditional_ignore<CharT>::_is_char _is_char;
	return detail::ignore_impl(c, _is_char());
}

template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, const ignore_delimiter<CharT>& ig)
{
	return is.ignore(std::numeric_limits<std::streamsize>::max(), ig.delimiter);
}
template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, const ignore_count& ig)
{
	return is.ignore(ig.count, CharTraits::eof());
}

} // namespace io
} // namespace pocket

#endif // __POCKET_IO_H__
