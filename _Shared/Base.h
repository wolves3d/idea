///////////////////////////////////////////////////////////////////////////

#ifndef __Base_h_included__
#define __Base_h_included__

/** @defgroup base_types Base Types
*  ������� ����
*  @{
*/

//--------------------------------------------------------------------------
//	Tip: Type definitions
//--------------------------------------------------------------------------
typedef unsigned char   uchar;
typedef unsigned char   byte;

typedef char            int8;
typedef short           int16;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint;
typedef unsigned long   dword;

typedef __int64          pointer;
//typedef __int64         addr_t;
typedef size_t			index_t;

struct TMemChunk
{
	void	*	pData;
	size_t		nSize;
};

/** @} */

////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
	#define NULL			0
#endif // #ifndef NULL

#define INVALID_INDEX	-1

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
#define DefineIndexedType( X ) struct X	\
{										\
	inline X() :						\
		nID( INVALID_INDEX )			\
	{									\
	}									\
										\
	inline X( int _nID ) :				\
		nID( _nID )						\
	{									\
	}									\
										\
	int nID;							\
};

////////////////////////////////////////////////////////////////////////////////

enum ETypeID
{
	//--------------------------------------------------------------------------
	// Tip: WARNING! Keep connected with CSystem::CSystem()
	//--------------------------------------------------------------------------
	INVALID_TYPE = 0,

	TYPE_BOOL,
	TYPE_SHORT,
	TYPE_INT,
	TYPE_DWORD,
	TYPE_FLOAT,
	TYPE_VEC2,
	TYPE_VEC3,
	TYPE_VEC4,
	TYPE_MAT4,
	TYPE_STRING,

	LAST_TYPE
};

////////////////////////////////////////////////////////////////////////////////

#include <_Shared/API/ISystem.h>
#include <_Shared/Memory.h>

///////////////////////////////////////////////////////////////////////////

struct TImage
{
	inline TImage() :
		nWidth	( 0					),
		nHeight	( 0					),
		eFormat	( IMG_FMT_UNKNOWN	),
		nSize	( 0					),
		pData	( NULL				)
	{
	}

	inline ~TImage()
	{
		//DEL_ARRAY( pData );
	}

	enum EImageFmt
	{
		IMG_FMT_UNKNOWN = INVALID_INDEX,
		IMG_FMT_RGB8,
		IMG_FMT_RGBA8,
		IMG_FMT_DXT1,
		IMG_FMT_DXT3,
		IMG_FMT_DXT5
	};

	uint		nWidth;
	uint		nHeight;
	EImageFmt	eFormat;
	uint		nSize;
	byte *		pData;
};

///////////////////////////////////////////////////////////////////////////

inline const char * Va( const char * szFmt, ... )
{
	#define MAX_STR_BUFFER 1024
	static char szBuffer[ MAX_STR_BUFFER ];

	va_list vArgs;

	va_start( vArgs, szFmt );
	vsprintf_s( szBuffer, szFmt, vArgs );
	va_end	( vArgs );

	DEBUG_ASSERT( strlen( szBuffer ) < MAX_STR_BUFFER );

	return szBuffer;
}

///////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Base_h_included__

///////////////////////////////////////////////////////////////////////////