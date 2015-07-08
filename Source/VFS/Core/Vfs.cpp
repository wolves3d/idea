////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CVfs::ms_ClassDesc = 
{
	8080,
	IT_VFS,
	1,
	"VFS by WW v0.1"
};


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
const TIntfDesc * CVfs::GetInterfaceDesc() const
{
	return &ms_ClassDesc;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CVfs::Init()
{
	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CVfs::Release()
{
	DEL( g_pVfs );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CVfs::Delete()
{
	DEL( g_pVfs );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
int CVfs::Seek( IFile * pFile, long nOffset, EVfsEnum eOrigin )
{
	if ( !pFile )
	{
		return -1;
	}

	return ((CFile*)pFile)->Seek( nOffset, eOrigin );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
size_t CVfs::Write( IFile * pFile, const void * pBuffer, size_t nSize )
{
	if (	!pFile		||
			!pBuffer	||
			!nSize			)
	{
		return 0;
	}

	return ((CFile*)pFile)->Write( pBuffer, nSize );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
size_t CVfs::Read( IFile * pFile, void * pBuffer, size_t nSize )
{
	if (	!pFile		||
			!pBuffer	||
			!nSize			)
	{
		return 0;
	}

	return ((CFile*)pFile)->Read( pBuffer, nSize );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CVfs::IsFileExist( const char * szFileName ) const
{
	FILE *	pFile = fopen( szFileName, "rb" );

	if ( pFile )
	{
		fclose( pFile );
		return true;
	}

	return false;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CVfs::OpenSearch( IFileSearch ** ppOutSearch )
{
	return R_INVALID_OP;
}

bool CVfs::SearchNext( IFileSearch * pSearch )
{
	return false;
}

EResult CVfs::CloseSearch( IFileSearch ** ppSearch )
{
	return R_INVALID_OP;
}


/**
*
*/
char * CVfs::LoadToMem( const char * szFileName, int nAccess )
{
	IFile * pFile = NULL;

	if ( R_OK == OpenFile( &pFile, szFileName, nAccess ) )
	{
		size_t nSize = pFile->GetSize();

		char * pBuffer = NEW char [ nSize + 1 ];

		if ( pFile )
		{
			Read( pFile, pBuffer, nSize );
		}

		pBuffer[ nSize ] = 0;

		CloseFile( &pFile );

		return pBuffer;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////