////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

/**
*
*/
void CUIManager::RenderDebugInfo()
{
	static CStr sStr;

	sStr.Clear();

	for ( uint n = 0; n < g_pSystem->GetCounterCount(); ++n )
	{
		ICounter * pCounter = g_pSystem->GetCounter( n );

		sStr.Add( pCounter->GetName() );
		sStr.Add( " " );
		sStr.AddNumber( pCounter->GetValue() );
		sStr.Add( "\n" );
		sStr.Add( " " );
	}

	m_pDebugText->SetColor( vec4( 1, 1, 1, 1 ) );
	m_pDebugText->SetText( sStr.GetString() );
	m_pDebugText->Render();
}

////////////////////////////////////////////////////////////////////////////////