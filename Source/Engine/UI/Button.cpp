////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

////////////////////////////////////////////////////////////////////////////////

void CButton::Render()
{

	CBaseControl::Render();

	vec2 vPos = GetPos();
	vec2 vSize = GetSize();

	if ( GetParent() )
		vPos.Add( GetParent()->GetPos() );

	g_pRenderer->Line2D( vPos, vec2( vPos.x + vSize.x, vPos.y + vSize.y ), 0xFFFFFFFF );

	if ( m_bPressed )
		g_pRenderer->Line2D( vPos, vec2( vPos.x + vSize.x, vPos.y + vSize.y ), 0xFFFFFFFF );
}

////////////////////////////////////////////////////////////////////////////////

bool CButton::ProcessInput( const TInputMsg & tData )
{
	if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
		SetText( "Over" );
	else
		SetText( "OK" );

	switch ( tData.eType )
	{
		case IMT_MOUSE_MOVE_X:
		case IMT_MOUSE_MOVE_Y:
		{
			if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
			{
				if ( this == g_pUIManager->GetActiveControl() )
				{
					m_bPressed = true;
				}
			}
			else
			{
				m_bPressed = false;
			}
		}
		break;

		case IMT_MOUSE_LKEY:
		{
			if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
			{
				if ( tData.dwData & 0x80 )
				{
					Press();
					g_pUIManager->SetActiveControl( this );
				}
				else
				{
					if ( this == g_pUIManager->GetActiveControl() )
					{
						g_pUIManager->SetActiveControl( NULL );
						MessageBeep( MB_OK );

						//if ( m_pParent )
						//	m_pParent->OnChildEvent( this );

						Press( false );
					}
				}

				return true;
			}
			else if ( this == g_pUIManager->GetActiveControl() )
			{
				if ( !( tData.dwData & 0x80 ) )
					g_pUIManager->SetActiveControl( NULL );
			}
		}
		break;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////