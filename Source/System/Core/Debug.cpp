////////////////////////////////////////////////////////////////////////////////

#include "Common.h"




/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CSystem::AssertMsg( const char * szFile, int nLine, const char * szString )
{
	SysWrite( "Assertion Failed: " );

	CStr sStr( szString );

	char szBuffer[ 256 ];
	sprintf( szBuffer, "\nFile: \"%s\" Line: %d", szFile, nLine );
	sStr.Add( szBuffer );
	SysWrite( szBuffer );
	
	sStr.Add( "\nYes - break into debugger, No - Exit program, Cancel - skip" );

	int nResult =  MessageBox( NULL, sStr.GetString(), "Assertion Failed", MB_ICONSTOP | MB_YESNOCANCEL );

	switch ( nResult )
	{
		case IDYES:
			SysWrite( "User decided to break into debuger.\n" );
			__debugbreak(); //__asm { int 3 }
		return;

		case IDNO:
			SysWrite( "User decided close application." );
			exit( -1 );
		return;
	}

	SysWrite( "User ignored assertion." );
}


void CSystem::WriteLog( const char * szLog )
{
	DEBUG_ASSERT( NULL != szLog );

	if ( m_pLogFile && szLog )
	{
		fprintf( m_pLogFile, "%s\n", szLog );
		fflush( m_pLogFile );
	}
}

////////////////////////////////////////////////////////////////////////////////