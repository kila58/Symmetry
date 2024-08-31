/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

#include "types.h"
#include "windows_headers.h"

enum Types
{
	TYPE_UNKNOWN	= 0x0,

	// Floating points
	TYPE_DOUBLE		= 0x1,
	TYPE_FLOAT		= 0x2,

	// Integers
	TYPE_INT16		= 0x3,
	TYPE_INT32		= 0x4,
	TYPE_INT64		= 0x5,

	// Unsigned integers
	TYPE_UINT16		= 0x6,
	TYPE_UINT32		= 0x7,
	TYPE_UINT64		= 0x8,

	// Strings
	TYPE_STRINGP	= 0x9,
	TYPE_STRING		= 0xA,
	
	// Characters
	TYPE_CHAR		= 0xB,
	TYPE_UCHAR		= 0xC
};

// Contains an in-order list of names.  do TypeNames[ TYPE_ENUM ] to get the 
// name of a type. 
static const char* TypeNames[] =
{
	"UNKNOWN",

	// Floating points
	"DOUBLE",
	"FLOAT",

	// Integers
	"INT16",
	"INT32",
	"INT64",

	// Unsigned integers
	"UINT16",
	"UINT32",
	"UINT64",

	// Strings
	"STRING [ptr]",
	"STRING [managed]",

	// Characters
	"CHAR",
	"UCHAR"
};

// Used to determine what type a template is, sort of
// Base, we don't know what this type is
template <typename T>
struct TypeInfo
{
	static const int		iType = TYPE_UNKNOWN;
};

// Floating points
template<>
struct TypeInfo< double >
{ 
	static const int		iType = TYPE_DOUBLE;
};

template<>
struct TypeInfo< float >
{ 
	static const int		iType = TYPE_FLOAT;
};

// Integers
template<>
struct TypeInfo< int16 >
{ 
	static const int		iType = TYPE_INT16; 
};

template<>
struct TypeInfo< int32 >
{
	static const int		iType = TYPE_INT32;
};

template<>
struct TypeInfo< int64 >
{ 
	static const int		iType = TYPE_INT64;
};

// Unsigned integers
template<>
struct TypeInfo< uint16 >
{ 
	static const int		iType = TYPE_UINT16;
};

template<>
struct TypeInfo< uint32 >
{ 
	static const int		iType = TYPE_UINT32; 
};

template<>
struct TypeInfo< uint64 >
{ 
	static const int		iType = TYPE_UINT64;
};

// Strings
template<>
struct TypeInfo< char* >
{ 
	static const int		iType = TYPE_STRINGP;
};

template<>
struct TypeInfo< string >
{ 
	static const int		iType = TYPE_STRING; 
};

// Characters
template<>
struct TypeInfo< char >
{
	static const int		iType = TYPE_CHAR;
};

template<>
struct TypeInfo< uchar >
{ 
	static const int		iType = TYPE_UCHAR;
};