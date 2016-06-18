#ifndef __POCKET_FIXED_ARRAY_H__
#define __POCKET_FIXED_ARRAY_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "container/array.h"
#ifdef _USING_MATH_IO
#include "io.h"
#endif // _USING_MATH_IO

namespace pocket
{
// forward
template <typename, size_t>
struct fixed_array;

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

/*------------------------------------------------------------------------------------------
* Impl
*------------------------------------------------------------------------------------------*/

template <typename T, size_t N>
struct fixed_array
{
	typedef container::array<T, N> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	array_type data;

	fixed_array& operator = (value_type a)
	{
		data.fill(a);
		return *this;
	}
	reference operator [] (int i)
	{
		return data[i];
	}
	const_reference operator [] (int i) const
	{
		return data[i];
	}
};
template <size_t N>
struct fixed_array<bool, N>
{
	typedef container::array<bool, N> array_type;
	typedef typename array_type::value_type value_type;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	array_type data;

	bool all() const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (!data[i])
			{
				return false;
			}
		}
		return true;
	}
	bool any() const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (data[i])
			{
				return true;
			}
		}
		return false;
	}
	bool none() const
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (data[i])
			{
				return false;
			}
		}
		return true;
	}

	fixed_array& operator = (value_type a)
	{
		data.fill(a);
		return *this;
	}
	reference operator [] (int i)
	{
		return data[i];
	}
	const_reference operator [] (int i) const
	{
		return data[i];
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
			T x;
			T y;
		};
		array_type data;
	};

	fixed_array()
	{

	}
	fixed_array(value_type x, value_type y) :
		x(x), y(y)
	{

	}
	fixed_array(value_type f) :
		x(f), y(f)
	{

	}
	fixed_array(const fixed_array<T, 3>&);
	fixed_array(const fixed_array<T, 4>&);

	fixed_array& operator = (value_type a)
	{
		x = y = a;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 3>&);
	fixed_array& operator = (const fixed_array<T, 4>&);
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		x = a[0];
		y = a[1];
		return *this;
	}

	reference operator [] (int i)
	{
		return data[i];
	}
	const_reference operator [] (int i) const
	{
		return data[i];
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
			T x;
			T y;
			T z;
		};
		struct
		{
			T r;
			T g;
			T b;
		};
		array_type data;
	};

	fixed_array()
	{

	}
	fixed_array(value_type x, value_type y, value_type z) :
		x(x), y(y), z(z)
	{

	}
	fixed_array(value_type f) :
		x(f), y(f), z(f)
	{

	}
	fixed_array(const fixed_array<T, 2>& xy, value_type z) :
		x(xy.x), y(xy.y), z(z)
	{

	}
	fixed_array(value_type x, const fixed_array<T, 2>& yz) :
		x(x), y(yz.x), z(yz.y)
	{

	}
	fixed_array(const fixed_array<T, 4>&);

	fixed_array& operator = (value_type a)
	{
		x = y = z = a;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 2>& a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 4>&);
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		x = a[0];
		y = a[1];
		z = a[2];
		return *this;
	}

	reference operator [] (int i)
	{
		return data[i];
	}
	const_reference operator [] (int i) const
	{
		return data[i];
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
			T x;
			T y;
			T z;
			T w;
		};
		struct
		{
			T r;
			T g;
			T b;
			T a;
		};
		array_type data;
	};

	fixed_array()
	{

	}
	fixed_array(value_type x, value_type y, value_type z, value_type w) :
		x(x), y(y), z(z), w(w)
	{

	}
	fixed_array(value_type f) :
		x(f), y(f), z(f), w(f)
	{

	}
	fixed_array(const fixed_array<T, 2>& xy, value_type z, value_type w) :
		x(xy.x), y(xy.y), z(z), w(w)
	{

	}
	fixed_array(value_type x, const fixed_array<T, 2>& yz, value_type w) :
		x(x), y(yz.x), z(yz.y), w(w)
	{

	}
	fixed_array(value_type x, value_type y, const fixed_array<T, 2>& xw) :
		x(x), y(y), z(xw.x), w(xw.y)
	{

	}
	fixed_array(const fixed_array<T, 3>& xyz, value_type w) :
		x(xyz.x), y(xyz.y), z(xyz.z), w(w)
	{

	}
	fixed_array(value_type x, const fixed_array<T, 3>& yzw) :
		x(x), y(yzw.x), z(yzw.y), w(yzw.z)
	{

	}

	fixed_array& operator = (value_type a)
	{
		x = y = z = w = a;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 2>& a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}
	fixed_array& operator = (const fixed_array<T, 3>& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}
	template <size_t N>
	fixed_array& operator = (const fixed_array<T, N>& a)
	{
		x = a[0];
		y = a[1];
		z = a[2];
		w = a[3];
		return *this;
	}

	reference operator [] (int i)
	{
		return data[i];
	}
	const_reference operator [] (int i) const
	{
		return data[i];
	}
};

namespace detail
{
struct bool_reference
{
	int& a;
	int bit;

	bool_reference(int& a, int i) :
		a(a), bit(0x01 << i)
	{

	}

	bool operator = (bool b)
	{
		if (b)
		{
			a |= bit;
		}
		else
		{
			a &= ~bit;
		}
		return b;
	}

	_CXX11_EXPLICIT operator bool () const
	{
		return (a & bit) != 0x00;
	}
	bool operator () () const
	{
		return (a & bit) != 0x00;
	}
};

inline _CXX11_CONSTEXPR int or_bit(bool a, int shift)
{
	return a ? 0x01 << shift : 0x00;
}
}

template <>
struct fixed_array<bool, 2>
{
	typedef detail::bool_reference reference;

	int data;

	fixed_array() :
		data(0)
	{

	}
	fixed_array(bool a) :
		data(a ? 0x03 : 0x00)
	{

	}
	fixed_array(bool a, bool b) :
		data(detail::or_bit(a, 0) | detail::or_bit(b, 1))
	{

	}
	fixed_array(const fixed_array<bool, 3>&);
	fixed_array(const fixed_array<bool, 4>&);

	bool all() const
	{
		return (data & 0x03) == 0x03;
	}
	bool any() const
	{
		return (data & 0x03) != 0x00;
	}
	bool none() const
	{
		return (data & 0x03) == 0x00;
	}

	fixed_array& operator = (bool a)
	{
		data = a ? 0x03 : 0x00;
		return *this;
	}
	reference operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
		return reference(data, i);
	}
	bool operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 1);
		return (data & (0x01 << i)) != 0;
	}
};
template <>
struct fixed_array<bool, 3>
{
	typedef detail::bool_reference reference;

	int data;

	fixed_array() :
		data(0)
	{

	}
	fixed_array(bool a) :
		data(a ? 0x07 : 0x00)
	{

	}
	fixed_array(bool a, bool b, bool c) :
		data(detail::or_bit(a, 0) | detail::or_bit(b, 1) | detail::or_bit(c, 2))
	{

	}
	fixed_array(const fixed_array<bool, 2>& a, bool c) :
		data((a.data & 0x03) | detail::or_bit(c, 2))
	{

	}
	fixed_array(bool a, const fixed_array<bool, 2>& b) :
		data(detail::or_bit(a, 0) | ((b.data & 0x03) << 1))
	{

	}
	fixed_array(const fixed_array<bool, 4>&);

	bool all() const
	{
		return (data & 0x07) == 0x07;
	}
	bool any() const
	{
		return (data & 0x07) != 0x00;
	}
	bool none() const
	{
		return (data & 0x07) == 0x00;
	}

	fixed_array& operator = (bool a)
	{
		data = a ? 0x07 : 0x00;
		return *this;
	}
	reference operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
		return reference(data, i);
	}
	bool operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 2);
		return (data & (0x01 << i)) != 0;
	}
};
template <>
struct fixed_array<bool, 4>
{
	typedef detail::bool_reference reference;

	int data;

	fixed_array() :
		data(0)
	{

	}
	fixed_array(bool a) :
		data(a ? 0x0F : 0x00)
	{

	}
	fixed_array(bool a, bool b, bool c, bool d) :
		data(detail::or_bit(a, 0) | detail::or_bit(b, 1) | detail::or_bit(c, 2) | detail::or_bit(d, 3))
	{

	}
	fixed_array(const fixed_array<bool, 2>& a, bool c, bool d) :
		data((a.data & 0x03) | detail::or_bit(c, 2) | detail::or_bit(d, 3))
	{

	}
	fixed_array(bool a, const fixed_array<bool, 2>& b, bool d) :
		data(detail::or_bit(a, 0) | ((b.data & 0x03) << 1) | detail::or_bit(d, 3))
	{

	}
	fixed_array(bool a, bool b, fixed_array<bool, 2>& c) :
		data(detail::or_bit(a, 0) | detail::or_bit(b, 1) | ((c.data & 0x03) << 2))
	{

	}
	fixed_array(const fixed_array<bool, 3>& a, bool b) :
		data((a.data & 0x07) | detail::or_bit(b, 3))
	{

	}
	fixed_array(bool a, const fixed_array<bool, 3>& b) :
		data(detail::or_bit(a, 0) | ((b.data & 0x07) << 1))
	{

	}

	bool all() const
	{
		return (data & 0x0F) == 0x0F;
	}
	bool any() const
	{
		return (data & 0x0F) != 0x00;
	}
	bool none() const
	{
		return (data & 0x0F) == 0x00;
	}

	fixed_array& operator = (bool a)
	{
		data = a ? 0x0F : 0x00;
		return *this;
	}
	reference operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
		return reference(data, i);
	}
	bool operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
		return (data & (0x01 << i)) != 0;
	}
};

template <typename T> inline
fixed_array<T, 2>::fixed_array(const fixed_array<T, 3>& a) :
	x(a.x), y(a.y)
{

}
template <typename T> inline
fixed_array<T, 2>::fixed_array(const fixed_array<T, 4>& a) :
	x(a.x), y(a.y)
{

}
template <typename T> inline
fixed_array<T, 2>& fixed_array<T, 2>::operator = (const fixed_array<T, 3>& a)
{
	x = a.x;
	y = a.y;
	return *this;
}
template <typename T> inline
fixed_array<T, 2>& fixed_array<T, 2>::operator = (const fixed_array<T, 4>& a)
{
	x = a.x;
	y = a.y;
	return *this;
}
template <typename T> inline
fixed_array<T, 3>::fixed_array(const fixed_array<T, 4>& a) :
	x(a.x), y(a.y), z(a.z)
{

}
template <typename T> inline
fixed_array<T, 3>& fixed_array<T, 3>::operator = (const fixed_array<T, 4>& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

inline
fixed_array<bool, 2>::fixed_array(const fixed_array<bool, 3>& a) :
	data(a.data & 0x03)
{

}
inline
fixed_array<bool, 2>::fixed_array(const fixed_array<bool, 4>& a) :
	data(a.data & 0x03)
{

}
inline
fixed_array<bool, 3>::fixed_array(const fixed_array<bool, 4>& a) :
	data(a.data & 0x07)
{

}

} // namespace pocket

#ifdef _USING_MATH_IO
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::fixed_array<T, N>& ary)
{
	os << ary.data;
	return os;
}
template <typename CharT, typename CharTraits, typename T, size_t N> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const pocket::fixed_array<T, N>& ary)
{
	os << ary.data;
	return os;
}

#ifndef _POCKET_OUTPUT_BOOL_FIXED_ARRAY
#define _POCKET_OUTPUT_BOOL_FIXED_ARRAY(N) template <typename CharT, typename CharTraits> inline\
	std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const pocket::fixed_array<bool, N>& ary)\
	{\
		os << pocket::io::box_brackets_left;\
		for (int i = 0; i < N; ++i)\
		{\
			os << ary[i];\
			if (i < N-1)\
			{\
				os << pocket::io::space;\
			}\
		}\
		os << pocket::io::box_brackets_right;\
		return os;\
	}\
	template <typename CharT, typename CharTraits> inline\
	std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const pocket::fixed_array<bool, N>& ary)\
	{\
		os << pocket::io::box_brackets_left;\
		for (int i = 0; i < N; ++i)\
		{\
			os << ary[i];\
			if (i < N-1)\
			{\
				os << pocket::io::space;\
			}\
		}\
		os << pocket::io::box_brackets_right;\
		return os;\
	}
#endif // _POCKET_OUTPUT_BOOL_FIXED_ARRAY

_POCKET_OUTPUT_BOOL_FIXED_ARRAY(2);
_POCKET_OUTPUT_BOOL_FIXED_ARRAY(3);
_POCKET_OUTPUT_BOOL_FIXED_ARRAY(4);

#undef _POCKET_OUTPUT_BOOL_FIXED_ARRAY

#endif // _USING_MATH_IO

#endif // __POCKET_FIXED_ARRAY_H__
