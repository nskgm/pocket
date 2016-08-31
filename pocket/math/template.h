#ifndef __MATH_TEMPLATE_H__
#define __MATH_TEMPLATE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "../behavior.h"
#include "fwd.h"
#include "../container/array.h"
#include "math_traits.h"
#include "../io.h"

namespace pocket
{

template <typename> struct __template_struct;

#ifndef _UNUSING_MATH_INT_FLOAT
typedef __template_struct<float> __template_structf;
#endif // _UNUSING_MATH_INT_FLOAT
#ifdef _USING_MATH_DOUBLE
typedef __template_struct<double> __template_structd;
#endif // _USING_MATH_DOUBLE
#ifdef _USING_MATH_LONG_DOUBLE
typedef __template_struct<long double> __template_structld;
#endif // _USING_MATH_LONG_DOUBLE

template <typename T>
struct __template_struct
{
	_MATH_STATICAL_ASSERT_FLOATING(T);

	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	typedef container::array<T, 1> array_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

#ifdef _USE_ANONYMOUS
	union
	{
		struct
		{
#endif // _USE_ANONYMOUS

			// Noen

#ifdef _USE_ANONYMOUS
		};

		// none
	};
#endif // _USE_ANONYMOUS

	template <typename> friend struct __template_struct;

	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	_DEFAULT_CONSTRUCTOR(__template_struct);
	explicit __template_struct(const behavior::_noinitialize_t&)
	{

	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

#if 0
	T& operator [] (int i)
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif // _USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		_DEB_RANGE_ASSERT(i, 0, 3);
#ifdef _USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif // _USE_ANONYMOUS
	}

	template <typename U>
	_CXX11_EXPLICIT operator __template_struct<U>() const
	{
		return __template_struct<U>();
	}
	_CXX11_EXPLICIT operator T* ()
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // _USE_ANONYMOUS
	}
	_CXX11_EXPLICIT operator const T* () const
	{
#ifdef _USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // _USE_ANONYMOUS
	}
#endif

	/*---------------------------------------------------------------------
	* 代入演算子
	*---------------------------------------------------------------------*/
	bool operator == (const __template_struct&) const
	{
		return true;
	}
	bool operator != (const __template_struct& v) const
	{
		return !(*this == v);
	}
};

template <typename CharT, typename CharTraits, typename T> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const __template_struct<T>& v)
{
	return os;
}
template <typename CharT, typename CharTraits, typename T> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, __template_struct<T>& v)
{
	return is;
}

} // namespace pocket

#endif // __MATH_TEMPLATE_H__
