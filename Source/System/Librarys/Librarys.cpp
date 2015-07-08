////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
ILibrary * CSystem::LoadLibrary( const char * szFileName )
{
	DEBUG_ASSERT( szFileName );

	if ( !szFileName )
		return NULL;

	if ( !strstr( szFileName, ".dll" ) )
		return NULL;

	HMODULE	hModule = ::LoadLibrary( szFileName );

	if ( hModule )
	{
		TLibrary::PFNInitLibrary pInitLibrary = (TLibrary::PFNInitLibrary)
			GetProcAddress( hModule, "InitLibrary" );

		TLibrary::PFNUninitLibrary pUninitLibrary = (TLibrary::PFNUninitLibrary)
			GetProcAddress( hModule, "UninitLibrary" );

		if ( !pInitLibrary || !pUninitLibrary )
		{
			SysWrite( Va( "System::LoadLibrary wrong DLL \"%s\"!", szFileName ) );

			FreeLibrary( hModule );
			return NULL;
		}

		ILibrary * pLib = pInitLibrary( g_pSystem );

		if ( pLib )
		{
			SysWrite( Va( "\tLibrary \"%s\" ( %d interface(s) )", szFileName, pLib->GetNumInterfaces() ) );

			for ( size_t n = 0; n < pLib->GetNumInterfaces(); ++n )
			{
				const TIntfDesc * pDesc = pLib->GetInterfaceDesc( n );

				SysWrite( Va( "\t\t%d - %s", n, pDesc->szDescString ) );

				TIntfPath tPath;

				tPath.nGUID		= pDesc->nGUID;
				tPath.pLibrary	= pLib;
				tPath.nIntfID	= (int)n;

				m_Interfaces.Add( tPath );
			}
		}

		TLibrary * pLibDesc = NEW TLibrary;

		if ( pLibDesc )
		{
			pLibDesc->hModule		= hModule;
			pLibDesc->pInitLibrary	= pInitLibrary;
			pLibDesc->pUninitLibrary= pUninitLibrary;
			m_Librarys.Add( pLibDesc );
		}
	}
	else
	{
		SysWrite( Va( "System::LoadLibrary DLL load failed \"%s\"!", szFileName ) );
	}

	return NULL;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CSystem::InitModules( const char * szPath )
{
	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	if ( !szPath )
		szPath = "Modules";

	CStr sModulesPath = Va( "%s/*.*", szPath );

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	SysWrite( Va( "\tSearching modules ( %s )", sModulesPath.GetString() ) );

	WIN32_FIND_DATA	tFindData;
	HANDLE hFind = FindFirstFile( sModulesPath.GetString(), &tFindData );

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		do
		{
			if ( !( tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				char szFileName[ MAX_PATH ];
				sprintf( szFileName, "%s/%s", szPath, tFindData.cFileName );

				if ( CStr::Compare( "System.dll", tFindData.cFileName ) )
					continue;

				LoadLibrary( szFileName );
			}
		}
		while ( FindNextFile( hFind, &tFindData ) );

		FindClose( hFind );
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
IBaseIntf * CSystem::CreateInterface( int nIntfGUID )
{
	for ( index_t nID = INVALID_INDEX; m_Interfaces.Iterate( nID ); )
	{
		if ( nIntfGUID == m_Interfaces[ nID ].nGUID )
		{
			IBaseIntf * pIntf = m_Interfaces[ nID ].pLibrary->CreateInterface(
				m_Interfaces[ nID ].nIntfID );

			if ( pIntf )
			{
				const TIntfDesc * pDesc = pIntf->GetInterfaceDesc();

				m_pModules[ pDesc->eType ] = pIntf;
				return pIntf;
			}
			
			return NULL;
		}
	}

	return NULL;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
IBaseIntf *	CSystem::GetInterface( EInterfaceType eType )
{
	return m_pModules[ eType ];
}

////////////////////////////////////////////////////////////////////////////////