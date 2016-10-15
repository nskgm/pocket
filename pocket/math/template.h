#ifndef __POCKET_MATH_TEMPLATE_H__
#define __POCKET_MATH_TEMPLATE_H__

#include "../config.h"
#ifdef POCKET_USE_PRAGMA_ONCE
#pragma once
#endif // POCKET_USE_PRAGMA_ONCE

#include "../behavior.h"
#include "fwd.h"
#include "../container/array.h"
#include "math_traits.h"
#include "../io.h"

namespace pocket
{

template <typename> struct __template_struct;

#ifndef POCKET_NO_USING_MATH_INT_FLOAT
typedef __template_struct<float> __template_structf;
#endif // POCKET_NO_USING_MATH_INT_FLOAT
#ifdef POCKET_USING_MATH_DOUBLE
typedef __template_struct<double> __template_structd;
#endif // POCKET_USING_MATH_DOUBLE
#ifdef POCKET_USING_MATH_LONG_DOUBLE
typedef __template_struct<long double> __template_structld;
#endif // POCKET_USING_MATH_LONG_DOUBLE

template <typename T>
struct __template_struct
{
	POCKET_MATH_STATICAL_ASSERT_FLOATING(T);

	//------------------------------------------------------------------------------------------
	// Types
	//------------------------------------------------------------------------------------------

	typedef container::array<T, 1> array_type;
	typedef typename array_type::iterator iterator;
	typedef typename array_type::const_iterator const_iterator;
	typedef typename array_type::pointer pointer;
	typedef typename array_type::const_pointer const_pointer;
	typedef typename array_type::reference reference;
	typedef typename array_type::const_reference const_reference;

	//------------------------------------------------------------------------------------------
	// Members
	//------------------------------------------------------------------------------------------

#ifdef POCKET_USE_ANONYMOUS
	union
	{
		struct
		{
#endif // POCKET_USE_ANONYMOUS

			// none

#ifdef POCKET_USE_ANONYMOUS
		};

		// none
	};
#endif // POCKET_USE_ANONYMOUS

	template <typename> friend struct __template_struct;

	//------------------------------------------------------------------------------------------
	// Constants
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Constructors
	//------------------------------------------------------------------------------------------

	POCKET_DEFAULT_CONSTRUCTOR(__template_struct);
	explicit __template_struct(const behavior::_noinitialize_t&)
	{

	}

	//------------------------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------------------------

	// none

	//------------------------------------------------------------------------------------------
	// Operators
	//------------------------------------------------------------------------------------------

#if 0
	T& operator [] (int i)
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif // POCKET_USE_ANONYMOUS
	}
	const T& operator [] (int i) const
	{
		POCKET_DEBUG_RANGE_ASSERT(i, 0, 3);
#ifdef POCKET_USE_ANONYMOUS
		return Data[i];
#else
		return (&X)[i];
#endif // POCKET_USE_ANONYMOUS
	}

	template <typename U>
	POCKET_CXX11_EXPLICIT operator __template_struct<U>() const
	{
		return __template_struct<U>();
	}
	POCKET_CXX11_EXPLICIT operator T* ()
	{
#ifdef POCKET_USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // POCKET_USE_ANONYMOUS
	}
	POCKET_CXX11_EXPLICIT operator const T* () const
	{
#ifdef POCKET_USE_ANONYMOUS
		return &Data[0];
#else
		return &X;
#endif // POCKET_USE_ANONYMOUS
	}
#endif

	//---------------------------------------------------------------------
	// 代入演算子
	//---------------------------------------------------------------------
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

#endif // __POCKET_MATH_TEMPLATE_H__
