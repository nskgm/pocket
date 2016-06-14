#ifndef __POCKET_MATH_ARRAY_H__
#define __POCKET_MATH_ARRAY_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../debug.h"
#include "fwd.h"
#ifdef _USING_MATH_IO
#include "../io.h"
#endif // _USING_MATH_IO

#ifdef _USE_CXX11
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

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const std::array<T, N>& ary)
{
	typedef typename std::array<T, N>::const_iterator const_iterator;
	os << pocket::out_char::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), line = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != line)
		{
			os << pocket::out_char::space;
		}
	}
	os << pocket::out_char::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const std::array<T, N>& ary)
{
	typedef typename std::array<T, N>::const_iterator const_iterator;
	os << pocket::out_char::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), line = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != line)
		{
			os << pocket::out_char::space;
		}
	}
	os << pocket::out_char::box_brackets_right;
	return os;
}
#endif // _USING_MATH_IO

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
	/*---------------------------------------------------------------------------------------
	* Types
	*---------------------------------------------------------------------------------------*/

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

	/*---------------------------------------------------------------------------------------
	* Members
	*---------------------------------------------------------------------------------------*/

	T _Data[N];

	/*---------------------------------------------------------------------------------------
	* Constants
	*---------------------------------------------------------------------------------------*/

	// None

	/*---------------------------------------------------------------------------------------
	* Constructs
	*---------------------------------------------------------------------------------------*/

	// None

	/*---------------------------------------------------------------------------------------
	* Mathods
	*---------------------------------------------------------------------------------------*/

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
		_DEB_RANGE_ASSERT(i, 0, N - 1);
		return _Data[i];
	}
	const_reference at(int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, N - 1);
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

	/*---------------------------------------------------------------------------------------
	* Operatots
	*---------------------------------------------------------------------------------------*/

	reference operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, N - 1);
		return _Data[i];
	}
	const_reference operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, N - 1);
		return _Data[i];
	}
};
} // namespace container
} // namespace pocket

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::container::array<T, N>& ary)
{
	typedef typename pocket::container::array<T, N>::const_iterator const_iterator;
	os << pocket::out_char::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), line = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != line)
		{
			os << pocket::out_char::space;
		}
	}
	os << pocket::out_char::box_brackets_right;
	return os;
}
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const pocket::container::array<T, N>& ary)
{
	typedef typename pocket::container::array<T, N>::const_iterator const_iterator;
	os << pocket::out_char::box_brackets_left;
	for (const_iterator i = ary.begin(), end = ary.end(), line = end - 1; i != end; ++i)
	{
		os << *i;
		if (i != line)
		{
			os << pocket::out_char::space;
		}
	}
	os << pocket::out_char::box_brackets_right;
	return os;
}
#endif // _USING_MATH_IO

namespace std
{
	template <typename T, size_t N> inline
	void swap(pocket::container::array<T, N>& lhs, pocket::container::array<T, N>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // _USE_CXX11

/*---------------------------------------------------------------------------------------
* 配列クラス定義
*---------------------------------------------------------------------------------------*/

namespace pocket
{
template <typename T, size_t N>
struct fixed_array
{
	typedef container::array<T, N> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	array_type Data;

	reference operator [] (int i)
	{
		return Data[i];
	}
	const_reference operator [] (int i) const
	{
		return Data[i];
	}
};
template <typename T>
struct fixed_array<T, 2>
{
	typedef container::array<T, 2> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	union
	{
		struct
		{
			T X;
			T Y;
		};
		array_type Data;
	};

	fixed_array& operator = (const fixed_array<T, 3>&);
	fixed_array& operator = (const fixed_array<T, 4>&);
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		_DEB_ASSERT(N >= 2);
		X = a[0];
		Y = a[1];
		return *this;
	}

	reference operator [] (int i)
	{
		return Data[i];
	}
	const_reference operator [] (int i) const
	{
		return Data[i];
	}
};
template <typename T>
struct fixed_array<T, 3>
{
	typedef container::array<T, 3> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	union
	{
		struct
		{
			T X;
			T Y;
			T Z;
		};
		struct
		{
			T R;
			T G;
			T B;
		};
		array_type Data;
	};

	fixed_array& operator = (const fixed_array<T, 2>& a)
	{
		X = a.X;
		Y = a.Y;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 4>&);
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		_DEB_ASSERT(N >= 3);
		X = a[0];
		Y = a[1];
		Z = a[2];
		return *this;
	}

	reference operator [] (int i)
	{
		return Data[i];
	}
	const_reference operator [] (int i) const
	{
		return Data[i];
	}
};
template <typename T>
struct fixed_array<T, 4>
{
	typedef container::array<T, 4> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	union
	{
		struct
		{
			T X;
			T Y;
			T Z;
			T W;
		};
		struct
		{
			T R;
			T G;
			T B;
			T A;
		};
		array_type Data;
	};

	fixed_array& operator = (const fixed_array<T, 2>& a)
	{
		X = a.X;
		Y = a.Y;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 3>& a)
	{
		X = a.X;
		Y = a.Y;
		Z = a.Z;
		return *this;
	}
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		_DEB_ASSERT(N >= 4);
		X = a[0];
		Y = a[1];
		Z = a[2];
		W = a[3];
		return *this;
	}

	reference operator [] (int i)
	{
		return Data[i];
	}
	const_reference operator [] (int i) const
	{
		return Data[i];
	}
};

template <typename T>
fixed_array<T, 2>& fixed_array<T, 2>::operator = (const fixed_array<T, 3>& a)
{
	X = a.X;
	Y = a.Y;
	return *this;
}
template <typename T>
fixed_array<T, 2>& fixed_array<T, 2>::operator = (const fixed_array<T, 4>& a)
{
	X = a.X;
	Y = a.Y;
	return *this;
}
template <typename T>
fixed_array<T, 3>& fixed_array<T, 3>::operator = (const fixed_array<T, 4>& a)
{
	X = a.X;
	Y = a.Y;
	Z = a.Z;
	return *this;
}

typedef fixed_array<bool, 2> bool2;
typedef fixed_array<bool, 3> bool3;
typedef fixed_array<bool, 4> bool4;

typedef fixed_array<sbyte, 2> sbyte2;
typedef fixed_array<sbyte, 3> sbyte3;
typedef fixed_array<sbyte, 4> sbyte4;
typedef fixed_array<byte, 2> byte2;
typedef fixed_array<byte, 3> byte3;
typedef fixed_array<byte, 4> byte4;

typedef fixed_array<s16, 2> short2;
typedef fixed_array<s16, 3> short3;
typedef fixed_array<s16, 4> short4;
typedef fixed_array<u16, 2> ushort2;
typedef fixed_array<u16, 3> ushort3;
typedef fixed_array<u16, 4> ushort4;

typedef fixed_array<s32, 2> int2;
typedef fixed_array<s32, 3> int3;
typedef fixed_array<s32, 4> int4;
typedef fixed_array<u32, 2> uint2;
typedef fixed_array<u32, 3> uint3;
typedef fixed_array<u32, 4> uint4;

typedef fixed_array<s64, 2> long2;
typedef fixed_array<s64, 3> long3;
typedef fixed_array<s64, 4> long4;
typedef fixed_array<u64, 2> ulong2;
typedef fixed_array<u64, 3> ulong3;
typedef fixed_array<u64, 4> ulong4;

typedef fixed_array<f32, 2> float2;
typedef fixed_array<f32, 3> float3;
typedef fixed_array<f32, 4> float4;

typedef fixed_array<f64, 2> double2;
typedef fixed_array<f64, 3> double3;
typedef fixed_array<f64, 4> double4;

} // namespace pocket

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::fixed_array<T, N>& ary)
{
	os << ary.Data;
	return os;
}
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const pocket::fixed_array<T, N>& ary)
{
	os << ary.Data;
	return os;
}
#endif // _USING_MATH_IO

#endif // __POCKET_MATH_ARRAY_H__
