////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

////////////////////////////////////////////////////////////////////////////////

CWindow::CWindow( CBaseControl * pParent ) :
	CBaseControl( pParent ),
	m_bDragging	( false ),
	m_pClose	( NULL )
{
	/*m_dwCtrlFlag |= UCF_SYSMENU;

	if ( m_pClose = NEW CButton( this ) )
	{
		m_pClose->SetSize( 10, 10 );
		m_pClose->SetPos( m_vSize.x - 5, m_vPos.y + 5 );
	}*/

	//g_pUIManager->m_WindowList.Add( this );
}


/**
*
*/
CWindow::~CWindow()
{
}


/**
*
*/
void CWindow::SetSize( float fWidth, float fHeight )
{
	CBaseControl::SetSize( fWidth, fHeight );

	if ( UCF_SYSMENU & GetFlag() )
	{
		if ( m_pClose )
		{
			m_pClose->SetPos( GetSize().x - 15, 5 );
		}
	}
}


////////////////////////////////////////////////////////////////////////////////

bool CWindow::ProcessInput( const TInputMsg & tData )
{
	if ( CBaseControl::ProcessInput( tData ) )
		return true;

	//--------------------------------------------------------------------------
	// Tip: Process input for "this"
	//--------------------------------------------------------------------------
	switch ( tData.eType )
	{
		case IMT_MOUSE_MOVE_X:
		{
			if ( m_bDragging )
			{
				vec2 vOldPos = GetPos();
				vOldPos.x += (float)( (int)tData.dwData );
				SetPos( vOldPos.x, vOldPos.y );
				return true;
			}
		}
		break;

		case IMT_MOUSE_MOVE_Y:
		{
			if ( m_bDragging )
			{
				vec2 vOldPos = GetPos();
				vOldPos.y += (float)( (int)tData.dwData );
				SetPos( vOldPos.x, vOldPos.y );
				return true;
			}
		}
		break;

		//----------------------------------------------------------------------
		// Tip: Нажатие или отжатие левой кнопочки над окошком
		//----------------------------------------------------------------------
		case IMT_MOUSE_LKEY:
		{
			if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
			{
				m_bDragging = ( NULL != ( tData.dwData & 0x80 ) );
				
				if ( m_bDragging )
					SetFocus();

				return true;
			}
			else if ( m_bDragging )
			{
				m_bDragging = false;
			}
		}
		break;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void CWindow::Render()
{
	if ( !IsVisible() )
		return;

	CBaseControl::Render();
}

////////////////////////////////////////////////////////////////////////////////

void CWindow::OnChildEvent( CBaseControl * pChild )
{
	if ( pChild )
	{
		if ( 123 == pChild->GetTag() )
		{
			PURE_DEL( this );
			//g_pUIManager->DestroyWin( this );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////