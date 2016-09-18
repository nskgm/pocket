#ifndef __POCKET_CONTAINER_ARRAY_H__
#define __POCKET_CONTAINER_ARRAY_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../debug.h"
#include "../io.h"

#ifdef POCKET_USE_CXX11
// C++11が使用できる場合は標準のものを使用
#include <array>

namespace pocket
{
namespace container
{
template <typename T, size_t N>
using array = std::array<T, N>;
}
}

template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const std::array<T, N>& ary)
{
	typedef typename std::array<T, N>::const_iterator const_iterator;
	os << pocket::io::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), last = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != last)
		{
			os << pocket::io::space;
		}
	}
	os << pocket::io::box_brackets_right;
	return os;
}

#else
// 使えない場合は簡単なクラスを定義
#include "../debug.h"
#include <cstddef>
#include <iterator>

namespace pocket
{
namespace container
{

template <typename T, size_t N>
class array
{
public:
	//---------------------------------------------------------------------------------------
	// Types
	//---------------------------------------------------------------------------------------

	typedef array<T, N> self;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	//---------------------------------------------------------------------------------------
	// Members
	//---------------------------------------------------------------------------------------

	T _Data[N];

	//---------------------------------------------------------------------------------------
	// Constants
	//---------------------------------------------------------------------------------------

	// None

	//---------------------------------------------------------------------------------------
	// Constructs
	//---------------------------------------------------------------------------------------

	// None

	//---------------------------------------------------------------------------------------
	// math_traitsods
	//---------------------------------------------------------------------------------------

	void assign(const T& t)
	{
		for (iterator i = begin(), end = end(); i != end; ++i)
		{
			*i = t;
		}
	}
	void fill(const T& t)
	{
		for (iterator i = begin(), end = end(); i != end; ++i)
		{
			*i = t;
		}
	}
	void swap(self& other)
	{
#if 0
		iterator oi = other.begin();
		for (iterator i = begin(), end = end(); i != end; ++i, ++oi)
		{
			T tmp = *i;
			*i = *oi;
			*oi = tmp;
		}
#endif
		// サイズも型も同じなのでポインタの差し替え
		pointer p = _Data;
		_Data = other._Data;
		other._Data = p;
	}
	iterator begin()
	{
		return &_Data[0];
	}
	const_iterator begin() const
	{
		return &_Data[0];
	}
	const_iterator cbegin() const
	{
		return &_Data[0];
	}
	iterator end()
	{
		return &_Data[N];
	}
	const_iterator end() const
	{
		return &_Data[N];
	}
	const_iterator cend() const
	{
		return &_Data[N];
	}
	bool empty() const
	{
		return (N == 0);
	}
	size_t size() const
	{
		return N;
	}
	size_t max_size() const
	{
		return N;
	}
	reference at(int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, N - 1);
		return _Data[i];
	}
	const_reference at(int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, N - 1);
		return _Data[i];
	}
	reference front()
	{
		return _Data[0];
	}
	const_reference front() const
	{
		return _Data[0];
	}
	reference back()
	{
		return _Data[N - 1];
	}
	const_reference back() const
	{
		return _Data[N - 1];
	}
	pointer data()
	{
		return &_Data[0];
	}
	const_pointer data() const
	{
		return &_Data[0];
	}

	//---------------------------------------------------------------------------------------
	// Operatots
	//---------------------------------------------------------------------------------------

	reference operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, static_cast<int>(N - 1));
		return _Data[i];
	}
	const_reference operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, static_cast<int>(N - 1));
		return _Data[i];
	}
};
} // namespace container
} // namespace pocket

template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::container::array<T, N>& ary)
{
	typedef typename pocket::container::array<T, N>::const_iterator const_iterator;
	os << pocket::io::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), line = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != line)
		{
			os << pocket::io::space;
		}
	}
	os << pocket::io::box_brackets_right;
	return os;
}

namespace std
{
	template <typename T, size_t N> inline
	void swap(pocket::container::array<T, N>& lhs, pocket::container::array<T, N>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // POCKET_USE_CXX11

#endif // __POCKET_CONTAINER_ARRAY_H__
