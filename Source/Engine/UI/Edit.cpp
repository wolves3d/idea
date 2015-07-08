////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*
void CEdit::Render()
{
	if ( !IsVisible() )
		return;

	CWindow::Render();

	
	vec2 vPos = CalcPos();


	if ( GetTextLength() )
	{
		//g_pUIManager->PrintChar( vPos, GetText() );
		vPos = g_pUIManager->GetCaretPos();
	}

	if ( g_pUIManager->GetActiveControl() == this )
	{
		//g_pUIManager->PrintChar( vPos, "_" );
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CEdit::ProcessInput( const TInputMsg * pData )
{
	if ( !pData )
		return false;

	if ( IMT_MOUSE_LKEY == pData->eType )
	{
		if ( pData->dwData & 0x80 )
		{
			if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
			{
				SetFocus();
				return true;
			}
		}
	}

	if ( IMT_KEY_DOWN == pData->eType )
	{
		if ( 0x0E /*DIK_BACK*/ == pData->dwData )
		{
			size_t nSize = GetTextLength();

			/*if ( nSize )
				m_sText.Cut( --nSize );*/
		}
		else
		{
			char szBuffer[] = { g_pInput->CodeToChar( pData->dwData ), 0 };
			//m_sText.Add( szBuffer );
		}

		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////