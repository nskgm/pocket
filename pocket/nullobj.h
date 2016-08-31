#ifndef __POCKET_NULLOBJ_H__
#define __POCKET_NULLOBJ_H__

#include "config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include <cstring> // for memset
#ifdef _USE_CXX11
#include <utility>
#endif // _USE_CXX11

namespace pocket
{
struct nullobj_t
{
	nullobj_t()
	{
	}

	operator bool () const
	{
		return false;
	}
	operator char () const
	{
		return '\0';
	}
	operator unsigned char () const
	{
		return '\0';
	}
	operator short () const
	{
		return 0;
	}
	operator unsigned short () const
	{
		return 0U;
	}
	operator int () const
	{
		return 0;
	}
	operator unsigned int () const
	{
		return 0U;
	}
	operator long () const
	{
		return 0L;
	}
	operator unsigned long () const
	{
		return 0UL;
	}
	operator long long () const
	{
		return 0LL;
	}
	operator unsigned long long () const
	{
		return 0ULL;
	}
	operator float () const
	{
		return 0.0f;
	}
	operator double () const
	{
		return 0.0;
	}
	operator long double () const
	{
		return 0.0L;
	}
	operator const char* () const
	{
		return "";
	}
	template <typename T>
	operator T* () const
	{
		return NULL;
	}
	template <typename T>
	operator const T* () const
	{
		return operator T*();
	}
	template <typename R, typename C>
	operator R C::* () const
	{
		return NULL;
	}

#ifdef _USE_CXX11
	operator std::nullptr_t () const
	{
		return nullptr;
	}
#endif // _USE_CXX11

	template <typename T>
	operator T () const
	{
		T r;
		std::memset(&r, 0, sizeof(T));
		return _CXX11_MOVE(r);
	}

	const nullobj_t& operator + () const
	{
		return *this;
	}
	const nullobj_t& operator - () const
	{
		return *this;
	}
	const nullobj_t& operator ~ () const
	{
		return *this;
	}
	const nullobj_t& operator * () const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator & () const
	{
		return *this;
	}
	const nullobj_t& operator ++ () const
	{
		return *this;
	}
	const nullobj_t& operator ++ (int) const
	{
		return *this;
	}
	const nullobj_t& operator -- () const
	{
		return *this;
	}
	const nullobj_t& operator -- (int) const
	{
		return *this;
	}

	const nullobj_t& operator [] (int) const
	{
		return *this;
	}
	const nullobj_t* operator -> () const
	{
		return this;
	}

	template <typename T>
	const nullobj_t& operator + (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator += (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator - (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator -= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator * (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator *= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator / (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator /= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator & (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator &= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator | (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator |= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator ^ (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator ^= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator % (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator %= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator << (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator <<= (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator >> (const T&) const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator >>= (const T&) const
	{
		return *this;
	}

#ifdef _USE_CXX11
	template <typename... Args>
	const nullobj_t& operator () (Args&&...) const
	{
		return *this;
	}
#else
	const nullobj_t& operator () () const
	{
		return *this;
	}
	template <typename T>
	const nullobj_t& operator () (const T&) const
	{
		return *this;
	}
	template <typename T, typename U>
	const nullobj_t& operator () (const T&, const U&) const
	{
		return *this;
	}
	template <typename T, typename U, typename V>
	const nullobj_t& operator () (const T&, const U&, const V&) const
	{
		return *this;
	}
	template <typename T, typename U, typename V, typename W>
	const nullobj_t& operator () (const T&, const U&, const V&, const W&) const
	{
		return *this;
	}
	template <typename T, typename U, typename V, typename W, typename X>
	const nullobj_t& operator () (const T&, const U&, const V&, const W&, const X&) const
	{
		return *this;
	}
	template <typename T, typename U, typename V, typename W, typename X, typename Y>
	const nullobj_t& operator () (const T&, const U&, const V&, const W&, const X&, const Y&) const
	{
		return *this;
	}
	template <typename T, typename U, typename V, typename W, typename X, typename Y, typename Z>
	const nullobj_t& operator () (const T&, const U&, const V&, const W&, const X&, const Y&, const Z&) const
	{
		return *this;
	}
#endif // _USE_CXX11

private:
	nullobj_t(const nullobj_t&);
	template <typename T>
	nullobj_t(const T&);
	nullobj_t& operator = (const nullobj_t&);
	template <typename T>
	nullobj_t& operator = (const T&);
#ifdef _USE_CXX11
	nullobj_t(nullobj_t&&);
	template <typename T>
	nullobj_t(T&&);
	nullobj_t& operator = (nullobj_t&&);
	template <typename T>
	nullobj_t& operator = (T&&);
#endif // _USE_CXX11
};

// +
template <typename T> inline
const T& operator + (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator += (const T& a, const nullobj_t&)
{
	return a;
}
// -
template <typename T> inline
const T& operator - (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator -= (const T& a, const nullobj_t&)
{
	return a;
}
// *
template <typename T> inline
const T& operator * (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator *= (const T& a, const nullobj_t&)
{
	return a;
}
// /
template <typename T> inline
const T& operator / (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator /= (const T& a, const nullobj_t&)
{
	return a;
}
// %
template <typename T> inline
const T& operator % (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator %= (const T& a, const nullobj_t&)
{
	return a;
}
// &
template <typename T> inline
const T& operator & (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator &= (const T& a, const nullobj_t&)
{
	return a;
}
// |
template <typename T> inline
const T& operator | (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator |= (const T& a, const nullobj_t&)
{
	return a;
}
// ^
template <typename T> inline
const T& operator ^ (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator ^= (const T& a, const nullobj_t&)
{
	return a;
}
// <<
template <typename T> inline
const T& operator << (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator <<= (const T& a, const nullobj_t&)
{
	return a;
}
// >>
template <typename T> inline
const T& operator >> (const T& a, const nullobj_t&)
{
	return a;
}
template <typename T> inline
const T& operator >>= (const T& a, const nullobj_t&)
{
	return a;
}
// <
template <typename T> inline
bool operator < (const T&, const nullobj_t&)
{
	return false;
}
template <typename T> inline
bool operator < (const nullobj_t&, const T&)
{
	return true;
}
// >
template <typename T> inline
bool operator > (const T&, const nullobj_t&)
{
	return true;
}
template <typename T> inline
bool operator > (const nullobj_t&, const T&)
{
	return false;
}
// >=
template <typename T> inline
bool operator >= (const T&, const nullobj_t&)
{
	return false;
}
template <typename T> inline
bool operator >= (const nullobj_t&, const T&)
{
	return false;
}
// <=
template <typename T> inline
bool operator <= (const T&, const nullobj_t&)
{
	return false;
}
template <typename T> inline
bool operator <= (const nullobj_t&, const T&)
{
	return false;
}
// ==
template <typename T> inline
bool operator == (const T&, const nullobj_t&)
{
	return false;
}
template <typename T> inline
bool operator == (const nullobj_t&, const T&)
{
	return false;
}
// !=
template <typename T> inline
bool operator != (const T&, const nullobj_t&)
{
	return true;
}
template <typename T> inline
bool operator != (const nullobj_t&, const T&)
{
	return true;
}

static const nullobj_t nullobj = {};

} // namespace pocket

#endif // __POCKET_NULLOBJ_H__
