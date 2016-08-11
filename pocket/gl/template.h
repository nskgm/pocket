#ifndef __POCKET_GL_TEMPLATE_H__
#define __POCKET_GL_TEMPLATE_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../debug.h"
#include "../io.h"

// forward
class __class;

class __class
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// none

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	// none

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	// none
};

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const __class& v)
{
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_istream<CharT, CharTraits>& operator >> (std::basic_istream<CharT, CharTraits>& is, __class& v)
{
	return is;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator << (std::basic_iostream<CharT, CharTraits>& os, const __class& v)
{
	return os;
}
template <typename CharT, typename CharTraits> inline
std::basic_iostream<CharT, CharTraits>& operator >> (std::basic_iostream<CharT, CharTraits>& is, __class& v)
{
	return is;
}

#endif // __POCKET_GL_TEMPLATE_H__
