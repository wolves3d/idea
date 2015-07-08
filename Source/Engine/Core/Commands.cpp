////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Commands.h"

//#include "../../Engine/ui/UI.h"
//#include "Desktop.h"
//#include "../Editor/Editor.h"

////////////////////////////////////////////////////////////////////////////////

const int	nMaxCommands = 256;
TCommand	g_pCommands[ nMaxCommands ];
int			nNumCommands = 0;

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
DECL_COMMAND( bind )
{
	if ( pArgs.GetCount() > 2 )
	{
		CStr * pArg = pArgs.GetArray();
		dword dwCode = g_pInput->NameToCode( pArg[ 1 ].GetString() );

		if ( dwCode )
		{
			g_pEngine->AddBind( dwCode, pArg[ 2 ].GetString() );
		}
	}
}


/**
*
*
DECL_COMMAND( toggle_console )
{
	IUIManager *	pMgr = g_pEngine->GetUIManager();
	CBaseControl *	pForm= g_pEditor->GetForm();

	if ( pMgr->GetDesktop() == pForm  )
	{
		pMgr->SetDesktop( g_pGame->m_pDeskTop );
	}
	else
	{
		pMgr->SetDesktop( pForm );
	}
}


/**
*
*/
void CEngine::Command( const char * szCommandLine )
{
	TStringStack pArgs;

	char szBuffer[ 256 ];
	int nBuffer = 0;

	while ( true )
	{
		if (	( ' ' == (*szCommandLine ) ) ||
				(  0  == (*szCommandLine ) )		)
		{
			szBuffer[ nBuffer ] = 0;
			nBuffer = 0;

			if ( *szBuffer )
				pArgs.Push( szBuffer );

			if ( 0 == (*szCommandLine ) )
				break;
		}
		else
		{
			szBuffer[ nBuffer ] = (*szCommandLine);
			++nBuffer;
		}

		++szCommandLine;
	}

	if ( pArgs.GetCount() )
	{
		TCommand tCmd;

		if ( m_pCommandHash.GetItem( pArgs.GetArray()[ 0 ].GetString(), tCmd ) )
		{
			tCmd.Evaluate( pArgs );
		}
		else
		{
			// unknown command
		}
	}

	pArgs.Delete();
}


/**
*
*/
void CEngine::RegisterCommand( const TCommand & tCmd )
{
	m_pCommandHash.AddItem( tCmd.sName.GetString(), tCmd );
}


/**
*
*/
void CEngine::RegisterCommands()
{
	for ( int i = 0; i < nNumCommands; ++i )
	{
		const TCommand & tCmd = g_pCommands[ i ];
		m_pCommandHash.AddItem( tCmd.sName.GetString(), tCmd );
	}

	if ( nMaxCommands == ( nNumCommands + 1 ) )
	{
		DEBUG_ASSERT( false );
	}
}

////////////////////////////////////////////////////////////////////////////////