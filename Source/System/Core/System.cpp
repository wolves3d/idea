////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "malloc.h"
//#include "_Shared/API/IRenderer.h"
#include "_Shared/API/IGame.h"
#include "StackWalker/StackWalker.h"


#include <delayimp.h>
#pragma comment(lib, "delayimp" )

////////////////////////////////////////////////////////////////////////////////

ISystem *		g_pSystem	= NULL;
StackWalker *	pSW			= NULL;

////////////////////////////////////////////////////////////////////////////////

struct TTypeInfo
{
	char *	szName;
	uint	nSize;
	ETypeID eTypeID;
} g_pTypeInfoTable[ LAST_TYPE ];

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
__declspec( dllexport ) ISystem * CreateSystem()
{
	DEBUG_ASSERT( NULL == g_pSystem  );

	if ( NULL == g_pSystem )
	{
		g_pSystem = new CSystem;
	}

	return g_pSystem;
}


/**
*
*/
CSystem::CSystem() :
	m_pMemCounter	( NULL ),
	m_pLogFile		( NULL )
{
	#define REG_TYPE( type_name, type_id )										\
	g_pTypeInfoTable[ type_id ].szName	= #type_name;							\
	g_pTypeInfoTable[ type_id ].nSize	= sizeof( type_name );					\
	g_pTypeInfoTable[ type_id ].eTypeID	= type_id;

	REG_TYPE( bool,		TYPE_BOOL	);
	REG_TYPE( short,	TYPE_SHORT	);
	REG_TYPE( int,		TYPE_INT	);
	REG_TYPE( dword,	TYPE_DWORD	);
	REG_TYPE( float,	TYPE_FLOAT	);
	REG_TYPE( vec2,		TYPE_VEC2	);
	REG_TYPE( vec3,		TYPE_VEC3	);
	REG_TYPE( vec4,		TYPE_VEC4	);
	REG_TYPE( mat4,		TYPE_MAT4	);
	REG_TYPE( CStr,		TYPE_STRING	);

	for ( int n = 0; n < IT_MAX_TYPES; ++n )
		m_pModules[ n ] = NULL;
}


/**
Инициализация стек-волкера
*/
bool CSystem::Init( const char * szPath )
{
	if ( CreateVarStorage() )
	{
		m_pLogFile		= fopen( "SystemLog.txt", "wt" );
		m_pMemCounter	= AddCounter( "Memory" );

		//pSW = NEW StackWalker();

		SysWrite( "--------------------------------------------------------------------------------" );
		SysWrite( "Initializing system libray" );
		SysWrite( "--------------------------------------------------------------------------------" );

		InitModules( szPath );

		SysWrite( "--------------------------------------------------------------------------------" );
		SysWrite( "System init successful" );
		SysWrite( "--------------------------------------------------------------------------------" );
		return true;
	}

	return false;
}


/*
================================================================================
//	Name: Release
//	Desc:
================================================================================
*/
void CSystem::Release()
{
	if ( g_pSystem )
	{
		for ( index_t n = INVALID_INDEX; m_Librarys.Iterate( n ); )
		{
			TLibrary * pLib = m_Librarys[ n ];
			pLib->pUninitLibrary();

			GetLastError();
			FreeLibrary( pLib->hModule );
			//__FUnloadDelayLoadedDLL2( "Engine.dll" );

			////////////////////////////////////////////////////////////////////
/*
			DWORD dwErr = GetLastError();

			TCHAR szBuf[80]; 
			LPVOID lpMsgBuf;
			DWORD dw = GetLastError(); 

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

			wsprintf( szBuf, "failed with error %d: %s", dw, lpMsgBuf );
			MessageBox(NULL, szBuf, "Error", MB_OK); 

			LocalFree(lpMsgBuf);
*/
			////////////////////////////////////////////////////////////////////

			DEL( pLib );
		}

		m_Interfaces.Delete();
		m_Librarys.Delete();

		DEL( pSW );
		DumpLeaks();

		if ( m_pLogFile )
		{
			fclose( m_pLogFile );
		}

		free	( g_pSystem );

		g_pSystem = NULL;
	}
}


/**
*
*/
ETypeID CSystem::GetType( const char * szType )
{
	for ( uint n = INVALID_TYPE + 1; n < LAST_TYPE; ++n )
	{
		if ( CStr::Compare( szType, g_pTypeInfoTable[ n ].szName ) )
			return g_pTypeInfoTable[ n ].eTypeID;
	}

	DEBUG_ASSERT( false );
	return INVALID_TYPE;
}


/**
*
*/
uint CSystem::GetSizeOf( ETypeID eType )
{
	if ( ( eType > INVALID_TYPE ) && ( eType < LAST_TYPE ) )
	{
		return g_pTypeInfoTable[ eType ].nSize;
	}

	DEBUG_ASSERT( !"invalid type" );
	return 0;
}

////////////////////////////////////////////////////////////////////////////////