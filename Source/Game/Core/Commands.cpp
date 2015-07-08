////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Engine/UI.h"

#include "Desktop.h"
#include "../Editor/Editor.h"

////////////////////////////////////////////////////////////////////////////////

const int	nMaxCommands = 256;
TCommand	g_pCommands[ nMaxCommands ];
int			nNumCommands = 0;

/**
*
*/
DECL_COMMAND( toggle_editor )
{
	//g_pEngine->MaterialEditor();
	g_pEditor->Activate( !g_pEditor->IsActive() );
}

////////////////////////////////////////////////////////////////////////////////

void CGame::RegisterCommands()
{
	for ( int i = 0; i < nNumCommands; ++i )
	{
		g_pEngine->RegisterCommand( g_pCommands[ i ] );
	}

	if ( nMaxCommands == ( nNumCommands + 1 ) )
	{
		DEBUG_ASSERT( false );
	}
}

////////////////////////////////////////////////////////////////////////////////