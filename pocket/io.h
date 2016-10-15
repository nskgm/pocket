#ifndef __POCKET_IO_H__
#define __POCKET_IO_H__

#include "config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "type_traits.h"
#include <iostream>
#include <limits>

#if (defined(_WIN32) || defined(_WIN64)) && defined(POCKET_USE_COLOR_OUTPUT)
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif // WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace pocket
{
namespace io
{
template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& empty(std::basic_ostream<CharT, CharTraits>& os)
{
	return os;
}

#define __POCKET_DECL_OUT_CHAR_FUNCTION(NAME, CHAR) template <typename CharT, typename CharTraits> inline \
	std::basic_ostream<CharT, CharTraits>& NAME(std::basic_ostream<CharT, CharTraits>& os) \
	{\
		os.put(os.widen((CHAR)));\
		return os;\
	}

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

#ifdef POCKET_USE_COLOR_OUTPUT
#	if defined(__APPLE__) || defined(__OSX__)
#		define __POCKET_DECL_OUT_CHAR_FUNCTION(NAME, STR) \
	template <typename CharT, typename CharTraits> inline\
	std::basic_ostream<CharT, CharTraits>& NAME(std::basic_ostream<CharT, CharTraits>& os)\
	{\
		os << widen(STR) << std::flush;\
		return os;\
	}
#	else
#		define __POCKET_DECL_OUT_CHAR_FUNCTION(NAME, TYPE) \
	template <typename CharT, typename CharTraits> inline\
	std::basic_ostream<CharT, CharTraits>& NAME(std::basic_ostream<CharT, CharTraits>& os)\
	{\
		::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), (TYPE));\
		return os;\
	}
#	endif // __POCKET_DECL_OUT_CHAR_FUNCTION

#	if defined(__APPLE__) || defined(__OSX__)
__POCKET_DECL_OUT_CHAR_FUNCTION(reset, "\033[0m");
__POCKET_DECL_OUT_CHAR_FUNCTION(red, "\033[31m");
__POCKET_DECL_OUT_CHAR_FUNCTION(green, "\033[32m");
__POCKET_DECL_OUT_CHAR_FUNCTION(yellow, "\033[33m");
__POCKET_DECL_OUT_CHAR_FUNCTION(blue, "\033[34m");
__POCKET_DECL_OUT_CHAR_FUNCTION(magenta, "\033[35m");
__POCKET_DECL_OUT_CHAR_FUNCTION(cyan, "\033[36m");
__POCKET_DECL_OUT_CHAR_FUNCTION(white, "\033[37m");
#	else
__POCKET_DECL_OUT_CHAR_FUNCTION(reset, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
__POCKET_DECL_OUT_CHAR_FUNCTION(red, FOREGROUND_RED | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(green, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(yellow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(blue, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(magenta, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(cyan, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
__POCKET_DECL_OUT_CHAR_FUNCTION(white, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#	endif

//__POCKET_DECL_OUT_CHAR_FUNCTION(black_bold, "\033[1m\033[30m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(red_bold, "\033[1m\033[31m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(green_bold, "\033[1m\033[32m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(yellow_bold, "\033[1m\033[33m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(blue_bold, "\033[1m\033[34m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(magenta_bold, "\033[1m\033[35m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(cyan_bold, "\033[1m\033[36m");
//__POCKET_DECL_OUT_CHAR_FUNCTION(white_bold, "\033[1m\033[37m");

#undef __POCKET_DECL_OUT_CHAR_FUNCTION

#endif // POCKET_USE_COLOR_OUTPUT

//---------------------------------------------------------------------
// widen
//---------------------------------------------------------------------

template <typename T>
class widen_holder;

template <>
class widen_holder<char>
{
	char c;
public:
	widen_holder(char c) :
		c(c)
	{}
	template <typename CharT, typename CharTraits>
	std::basic_ostream<CharT, CharTraits>& apply(std::basic_ostream<CharT, CharTraits>& os) const
	{
		os.put(os.widen(c));
		return os;
	}
};
template <>
class widen_holder<const char*>
{
	const char* str;
public:
	widen_holder(const char* s) :
		str(s)
	{}
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
class widen_holder<const char[N]>
{
	const char* str;
public:
	widen_holder(const char(&s)[N]) :
		str(&s[0])
	{}
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
class widen_holder<bool>
{
	bool cond;
public:
	widen_holder(bool c) :
		cond(c)
	{}
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

//---------------------------------------------------------------------
// ignore
//---------------------------------------------------------------------

template <typename CharT>
struct ignore_delimiter
{
	typedef CharT char_type;
	char_type delimiter;

	ignore_delimiter(char_type d) :
		delimiter(d)
	{}
};
struct ignore_count
{
	std::streamsize count;

	ignore_count(std::streamsize c) :
		count(c)
	{}
};

namespace detail
{
template <typename T>
struct is_char : type_traits::false_type
{};
template <>
struct is_char<char> : type_traits::true_type
{};
template <>
struct is_char<wchar_t> : type_traits::true_type
{};

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
