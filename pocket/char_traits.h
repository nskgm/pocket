#ifndef __POCKET_CHAR_TRAITS_H__
#define __POCKET_CHAR_TRAITS_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include <ios>
#include <cwchar>
#include <cstring>
#include <cstddef>

namespace pocket
{
namespace detail
{
template <typename> struct char_traits_types;

template <>
struct char_traits_types<char>
{
	typedef char char_type;
	typedef int int_type;
	typedef std::streamoff off_type;
	typedef std::streampos pos_type;

	static inline char_type char_default()
	{
		return 'a';
	}
	static inline int_type eof()
	{
		return EOF;
	}
};
template <>
struct char_traits_types<wchar_t>
{
	typedef wchar_t char_type;
	typedef std::wint_t int_type;
	typedef std::streamoff off_type;
	typedef std::wstreampos pos_type;

	static inline char_type char_default()
	{
		return L'a';
	}
	static inline int_type eof()
	{
		return WEOF;
	}
};

}

template <typename T>
struct char_traits
{
	typedef detail::char_traits_types<T> types;
	typedef typename types::char_type char_type;
	typedef typename types::int_type int_type;
	typedef typename types::off_type off_type;
	typedef typename types::pos_type pos_type;
	typedef mbstate_t state_type;

	// assign
	static inline void assign(char_type& a, char_type b)
	{
		a = b;
	}
	static inline char_type* assign(char_type* p, std::size_t n, char_type a)
	{
		while (n > 0)
		{
			*p = a;
			++p;
			--n;
		}
		return p;
	}

	// eq
	static inline bool eq(char_type a, char_type b)
	{
		return a == b;
	}

	// lt
	static inline bool lt(char_type a, char_type b)
	{
		return a < b;
	}

	// gt (extends)
	static inline bool gt(char_type a, char_type b)
	{
		return a > b;
	}

	// compare
	static inline int compare(const char_type* a, const char_type* b, std::size_t n)
	{
		while (n > 0)
		{
			if (lt(*a, *b))
			{
				return -1;
			}
			if (gt(*a, *b))
			{
				return 1;
			}
			++a;
			++b;
			--n;
		}
		// equals
		return 0;
	}

	// length
	static inline std::size_t length(const char_type* a)
	{
		char_type eos = char_type();
		std::size_t n = 0;
		while (*a != eos)
		{
			++n;
			++a;
		}
		return n;
	}

	// find
	static inline const char_type* find(const char_type* a, std::size_t n, char_type b)
	{
		while (n > 0)
		{
			if (eq(*a, b))
			{
				return a;
			}
			++a;
			--n;
		}
#ifdef _USE_CXX11
		return nullptr;
#else
		return NULL;
#endif // _USE_CXX11
	}

	// move
	static inline char_type* move(char_type* a, const char_type* b, std::size_t n)
	{
#if 1
		return static_cast<char_type*>(std::memmove(static_cast<void*>(a), static_cast<const void*>(b), n*sizeof(char_type)));
#else
		while (n > 0)
		{
			*a = *b;
			++a;
			++b;
			--n;
		}
		return a;
#endif
	}

	// copy
	static inline char_type* copy(char_type* a, const char_type* b, std::size_t n)
	{
#if 1
		return static_cast<char_type*>(std::memcpy(static_cast<void*>(a), static_cast<const void*>(b), n*sizeof(char_type)));
#else
		while (n > 0)
		{
			*a = *b;
			++a;
			++b;
			--n;
		}
		return a;
#endif
	}

	// not_eof
	static inline int_type not_eof(int_type a)
	{
		return eq_int_type(a, eof()) ? types::char_default() : a;
	}

	// to_char_type
	static inline char_type to_char_type(int_type a)
	{
		return static_cast<char_type>(a);
	}

	// to_int_type
	static inline int_type to_int_type(char_type a)
	{
		return static_cast<int_type>(a);
	}

	// eq_int_type
	static inline bool eq_int_type(int_type a, int_type b)
	{
		return a == b;
	}

	// eof
	static inline int_type eof()
	{
		return types::eof();
	}
};

} // namespace pocket

#endif // __POCKET_CHAR_TRAITS_H__
