////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"


/**
*
*/
struct TListData
{
	struct node_t
	{
		CBaseControl *	pCtrl;
		CStr			sText;
		void *			pUserData;
		bool			bSelected;
	};

	CContainer <node_t> m_pNodes;
};


/**
*
*/
class CListViewItem : public CBaseControl
{
	public:

	inline CListViewItem( CBaseControl * pParent ) :
		CBaseControl( pParent )
	{
	}

	bool ProcessInput( const TInputMsg & tData )
	{
		switch ( tData.eType )
		{
			case IMT_MOUSE_LKEY:
			{
				if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
				{
					for ( uint n = 0; n < GetParent()->GetChildCount(); ++n )
					{
						// hack - clear all list items color
						GetParent()->GetChildControl( n )->SetColor( 0xFFFFFFFF );
					}
					
					SetColor( 0xFF0000FF );
					return true;
				}
			}
			break;

			case IMT_MOUSE_MOVE_X:
			case IMT_MOUSE_MOVE_Y:
			{
				if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
				{
					for ( uint n = 0; n < GetParent()->GetChildCount(); ++n )
					{
						// hack - clear all list items color
						GetParent()->GetChildControl( n )->SetColor( 0xFFFFFFFF );
					}

					SetColor( 0xFF5087E7 );
					return true;
				}
			}
			break;

			default:
				return false;
		}
	}
};


/**
*
*/
CListView::CListView( CBaseControl * pParent ) :
	CBaseControl( pParent )
{
	m_pData = NEW TListData;
	//m_dwCtrlFlag |= UCF_STATIC_EDGE;
	SetFlag( GetFlag() | UCF_CLIENT_EDGE );
	SetColor( 0xFFFFFFFF );
}


/**
*
*/
CListView::~CListView()
{
	for ( index_t n = INVALID_INDEX; m_pData->m_pNodes.Iterate( n ); )
	{
		TListData::node_t & tNode = m_pData->m_pNodes[ n ];

		DEL( tNode.pCtrl );
	}

	m_pData->m_pNodes.Delete();

	DEL( m_pData );
}


/**
*
*/
bool CListView::AddNode( const char * szName  )
{
	if ( szName )
	{
		TListData::node_t tNode;

		tNode.pCtrl = NEW CListViewItem( this );
		tNode.pCtrl->SetText( szName );
		tNode.pCtrl->SetColor( 0xFFFFFFFF );
		tNode.pCtrl->SetPos( 1, (float)m_pData->m_pNodes.GetNumItems() * 14 );
		tNode.pCtrl->SetSize( GetSize().x - 2, 14 );

		return ( INVALID_INDEX != m_pData->m_pNodes.Add( tNode ) );
	}

	return false;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CListView::Render()
{
//	SetText("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
	CBaseControl::Render();

/*

	const ivec4 & vRect = CalcClientRect();

	ivec2 vPos = CalcPos();
	vPos.x += vRect.pos_x;
	vPos.y += vRect.pos_y;

	ivec2 vSize( vRect.width, 16 );
	ivec2 v = vPos;
*
	for ( index_t i = INVALID_INDEX; m_pData->m_pNodes.Iterate( i ); )
	{	
		const TListData::node_t & tNode = m_pData->m_pNodes[ i ];

		tNode.pCtrl->Render();

		/*IMaterial * pMaterial = g_pEngine->CreateMaterial();

		pMaterial->EnableDepthTest( false );
		pMaterial->SetShader( g_pRenderer->GetShader( "UI" ) );
		pMaterial->SetTexture( DIFFUSE_MAP, g_pRenderer->GetSysTexture( EST_WHITE ) );
		pMaterial->AddConstant( CConstant( "vColorMod", vec4( 0, 0, 1, 1 ) ) );

		g_pEngine->Sprite2D( pMaterial, v, vSize );
		v.y += 16;

		DEL( pMaterial );
		*
	}
/*
	for ( index_t i = INVALID_INDEX; m_pNodes.Iterate( i ); )
	{
		g_pUIManager->PrintChar( vPos, m_pNodes[ i ].sText.GetString() );
		vPos.y += 16;
	}
/*
	ivec2 vPos = CalcPos();

	IShader * pShader = g_pRenderer->GetShader( "FixedFunction" );
	pShader->SetSampler( 0, g_pRenderer->GetSysTexture( EST_WHITE ) );
	g_pRenderer->Sprite2D( pShader, vPos, m_vSize );

	if ( UCF_BORDER & m_dwCtrlFlag )
	{
		g_pRenderer->Line2D( vPos, ivec2( vPos.x, vPos.y + m_vSize.y ), 0xFFFFFFFF );
		g_pRenderer->Line2D( ivec2( vPos.x, vPos.y + m_vSize.y ), vPos + m_vSize, 0xFF808080 );
		g_pRenderer->Line2D( vPos + m_vSize, ivec2( vPos.x + m_vSize.x, vPos.y ), 0xFF808080 );
		g_pRenderer->Line2D( vPos , ivec2( vPos.x + m_vSize.x, vPos.y ), 0xFFFFFFFF );
	}

	if ( m_sText.GetSize() )
	{
		g_pUIManager->PrintChar( vPos, m_sText.GetString() );
		vPos = g_pUIManager->GetCaretPos();
	}

	if ( g_pUIManager->GetActiveControl() == this )
	{
		g_pUIManager->PrintChar( vPos, "_" );
	}
	*/
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CListView::ProcessInput( const TInputMsg * pData )
{
	if ( !pData )
		return false;

	/*if ( IMT_MOUSE_KEY_DOWN == pData->eType )
	{
		if ( IsMouseOver( g_pUIManager->GetMousePos() ) )
		{
			SetFocus();
			return true;
		}
	}*/
/*
	if ( IMT_KEY_DOWN == pData->eType )
	{
		if ( 0x0E //DIK_BACK// == pData->dwData )
		{
			size_t nSize = m_sText.GetSize();

			if ( nSize )
				m_sText.Cut( --nSize );
		}
		else
		{
			char szBuffer[] = { g_pInput->CodeToChar( pData->dwData ), 0 };
			m_sText.Add( szBuffer );
		}

		return true;
	}
*/
	return false;
}

////////////////////////////////////////////////////////////////////////////////