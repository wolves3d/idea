////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

struct TCtrlData
{
	int nTag;
	CBaseControl *	pParent;

	CStr			sText;

	vec2			vPos;		// позиция ( относительно родителя )
	vec2			vSize;
	vec4			m_vClientRect;
	vec2			vClip;

	bool			bFocused;
	bool			bEnabled;
	bool			bVisible;
	bool			bClip;
	bool			bDirtyVB;

	dword			dwColor;
	dword			dwCtrlFlag;

	dword	dwBackColor;	// Цвет фона
	dword	dwTextColor;	// Цвет текста

	uint			nID;

	ISprite2D		*	m_pSprite;
	IMaterial		*	pMaterial;
	IText2D			*	m_pText;

	CContainer <CBaseControl *> m_pChilds;

	// border - чёрная рамка внутри
	// client_edge - вдавленный как EditControl
	// modal_frame - выпуклый как кнопка

	ISprite2D		*	pOutLineLT;
	ISprite2D		*	pOutLineTP;
	ISprite2D		*	pOutLineRT;
	ISprite2D		*	pOutLineBM;
	ISprite2D		*	pInLineLT;
	ISprite2D		*	pInLineTP;
	ISprite2D		*	pInLineRT;
	ISprite2D		*	pInLineBM;
};


/**
*
*/
CBaseControl::CBaseControl( CBaseControl * pParent )
{
	if ( m_pData = NEW TCtrlData )
	{
		m_pData->pParent	= pParent;
		m_pData->bFocused	= false;
		m_pData->bEnabled	= true;
		m_pData->bVisible	= true;
		m_pData->dwColor	= 0xFFD4D0C8;
		m_pData->dwCtrlFlag	= 0; // UCF_BORDER;
		m_pData->bDirtyVB	= true;
		m_pData->bClip		= false;

		m_pData->vPos.Set( 0, 0 );
		m_pData->vSize.Set( 40, 15 );


		m_pData->m_pSprite = g_pUIManager->CreateSprite();
		m_pData->m_pSprite->SetPos( m_pData->vPos.x, m_pData->vPos.y );
		m_pData->m_pSprite->SetSize( m_pData->vSize.x, m_pData->vSize.y );

		if ( m_pData->pMaterial = g_pEngine->CreateMaterial() )
		{
			m_pData->pMaterial->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
			m_pData->pMaterial->EnableAlphaTest( false );
			m_pData->pMaterial->EnableDepthTest( false );
			m_pData->pMaterial->EnableDepthWrite( false );

			m_pData->pMaterial->SetTexture( DIFFUSE_MAP,
				g_pRenderer->GetSysTexture( EST_WHITE ) );

			CUniform uColorMod( "vColorMod", g_pRenderer->GetShader( UI_SHADER ) );
			uColorMod.Connect( m_pData->pMaterial );
			uColorMod.SetValue( &vec4( 0xFF, 0xD4, 0xD0, 0xC8 ), 1 );
			//uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );

			m_pData->m_pSprite->SetMaterial( m_pData->pMaterial );
		}

		// borders
		m_pData->pOutLineLT = g_pUIManager->CreateSprite();
		m_pData->pOutLineTP = g_pUIManager->CreateSprite();
		m_pData->pOutLineRT = g_pUIManager->CreateSprite();
		m_pData->pOutLineBM = g_pUIManager->CreateSprite();

		m_pData->pOutLineLT->SetPos( m_pData->vPos.x, m_pData->vPos.y );
		m_pData->pOutLineTP->SetPos( m_pData->vPos.x, m_pData->vPos.y );
		m_pData->pOutLineRT->SetPos( m_pData->vPos.x + m_pData->vSize.x, m_pData->vPos.y );
		m_pData->pOutLineBM->SetPos( m_pData->vPos.x, m_pData->vPos.y + m_pData->vSize.y );

		m_pData->pOutLineLT->SetSize( 1, m_pData->vSize.y );
		m_pData->pOutLineTP->SetSize( m_pData->vSize.x, 1 );
		m_pData->pOutLineRT->SetSize( 1, m_pData->vSize.y );
		m_pData->pOutLineBM->SetSize( m_pData->vSize.x, 1 );

		IMaterial * pMat = NULL;

		if ( pMat = g_pEngine->CreateMaterial() )
		{
			pMat->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
			pMat->EnableAlphaTest( false );
			pMat->EnableDepthTest( false );
			pMat->EnableDepthWrite( false );

			pMat->SetTexture( DIFFUSE_MAP,
				g_pRenderer->GetSysTexture( EST_BLACK ) );

			CUniform uColorMod( "vColorMod", g_pRenderer->GetShader( UI_SHADER ) );
			uColorMod.Connect( pMat );
			uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );

			m_pData->pOutLineLT->SetMaterial( pMat );
			m_pData->pOutLineTP->SetMaterial( pMat );
			m_pData->pOutLineRT->SetMaterial( pMat );
			m_pData->pOutLineBM->SetMaterial( pMat );
		}

		m_pData->m_pText	= NULL;

		g_pUIManager->InitControl( this );

		SetColor( 0xFFD4D0C8 );
	}
}


/**
*
*/
CBaseControl::~CBaseControl()
{
	g_pUIManager->DeleteSprite( m_pData->m_pSprite );
	RELEASE( m_pData->m_pText );

	for ( index_t n = INVALID_INDEX; m_pData->m_pChilds.Iterate( n ); )
	{
		DEL( m_pData->m_pChilds[ n ] );
	}

	m_pData->m_pChilds.Delete();

	DEL( m_pData );
}


/**
*
*/
void CBaseControl::Release()
{
	PURE_DEL( this );
}


/**
*
*/
int CBaseControl::GetTag() const
{
	return m_pData ? m_pData->nTag : NULL;
}


/**
*
*/
CBaseControl * CBaseControl::GetParent()
{
	return m_pData ? m_pData->pParent : NULL;
}


/**
*
*/
dword CBaseControl::GetFlag() const
{
	return m_pData ? m_pData->dwCtrlFlag : 0;
}


/**
*
*/
void CBaseControl::SetFlag( dword dwFlag )
{
	if ( m_pData )
		m_pData->dwCtrlFlag = dwFlag;
}

/**
*
*/
bool CBaseControl::IsFocused() const
{
	if ( m_pData )
	{
		return m_pData->bFocused;
	}

	return false;
}


/**
*
*/
void CBaseControl::SetFocus( bool bFocus )
{
	if ( m_pData )
	{
		g_pUIManager->SetActiveControl( this );
		m_pData->bFocused = bFocus;
	}
}


/**
*
*/
const char * CBaseControl::GetText() const
{
	return m_pData ? m_pData->sText.GetString() : NULL;
}


/**
*
*/
uint CBaseControl::GetTextLength() const
{
	return m_pData ? m_pData->sText.GetSize() : 0;
}


/**
*
*/
void CBaseControl::SetText( const char * szText )
{
	if ( !m_pData )
		return;

	if ( NULL == m_pData->m_pText )
	{
		m_pData->m_pText = g_pUIManager->CreateText(); // LEAK!!!
	}

	if ( m_pData->m_pText )
	{
		m_pData->m_pText->SetText( szText );
	}

	/*

	uint nPrevTextLength = GetTextLength();

	m_pData->sText.Set( szText );

	uint nNewTextLength = GetTextLength();

	const uint nBufferSize = ( nNewTextLength * 4 ) * sizeof ( vec4 );

	if ( nPrevTextLength != nNewTextLength )
	{
		// Длина текста изменилась, следовательно, нужно поменять и размер
		// вертекс буффера. Для начала удалим старый
		// ( а вдруг новая длина равна нулю? )

		if ( m_pData->pTextVB )
		{
			m_pData->pTextVB->Free();
		}

		if ( 0 != nNewTextLength )
		{
			// Новая длина текста не равна нулю,
			// нужно создать заново вертекс буффер

			if ( !m_pData->pTextVB )
			{
				m_pData->pTextVB = g_pRenderer->CreateVB();

				if ( !m_pData->pTextVB )
				{
					// Жопа!
					SetText( NULL );
				}
			}

			if ( R_OK != m_pData->pTextVB->Alloc( nBufferSize, 0 ) )
			{
				// Жопа! Смена текста не удалась, вертекс буффер вместо
				// аллокации памяти выписал бороду по колено!
				SetText( NULL );
			}
		}
	}

	vec4 * pVB = (vec4 *)m_pData->pTextVB->Lock( 0, nBufferSize );

	if ( !pVB )
	{
		// Жопа! Смена текста не удалась, вертекс буффер вместо
		// лока памяти выписал бороду по колено!
		SetText( NULL );
	}

	//pVB += 2; // skip background rect

	vec2 vPos( 0, 0 );

	for ( uint n = 0; n < nNewTextLength; ++n )
	{
		const CFont * pFont = g_pUIManager->GetFont( NULL );

		const vec4 &	vUV		( pFont->GetCharUV( szText[ n ] ) );
		const vec2		vUVSize	( vUV.z, vUV.w );
		const vec2		vUVPos	( vUV.x, 1 - vUV.y - vUVSize.y );

		//				vUVPos.y = 1 - vUVPos.y - vUVSize.y;

		float			fHeight	= 16;
		float			fWidth	= ( fHeight * ( vUVSize.u / vUVSize.v ) );

		const vec2 vSize( fWidth, fHeight );

		pVB[ 0 ] = vec4(	vPos.x,
			vPos.y,
			vUVPos.x,
			vUVPos.y + vUVSize.y );

		pVB[ 1 ] = vec4(	vPos.x + vSize.x,
			vPos.y,
			vUVPos.x + vUVSize.x,
			vUVPos.y + vUVSize.y );

		pVB[ 2 ] = vec4(	vPos.x + vSize.x,
			vPos.y + vSize.y,
			vUVPos.x + vUVSize.x,
			vUVPos.y );

		pVB[ 3 ] = vec4(	vPos.x,
			vPos.y + vSize.y,
			vUVPos.x,
			vUVPos.y );

		pVB += 4;
		vPos.x += fWidth;
	}

	m_pData->pTextVB->Unlock();
	*/
}


/**
*
*/
void CBaseControl::SetPos( float x, float y )
{
	if ( NULL == m_pData )
		return;

	m_pData->vPos.Set( x, y );

	for ( index_t n = INVALID_INDEX; m_pData->m_pChilds.Iterate( n ); )
	{
		// каждому ребёнку нужно вызвать SetPos
		const vec2 & vChildPos = m_pData->m_pChilds[ n ]->GetPos();
		m_pData->m_pChilds[ n ]->SetPos( vChildPos.x, vChildPos.y );
	}

	////////////////////////////////////////////////////////////////////////
	// absolute coords

	//m_pData->m_pSprite->SetPos( x, y );

	// getting abs position
	CBaseControl * pIter = m_pData->pParent;
	while ( pIter )
	{
		vec2 v = pIter->GetPos();
		pIter = pIter->m_pData->pParent;
		
		x += v.x;
		y += v.y;
	}

	if ( m_pData->m_pText )
		m_pData->m_pText->SetPos( x + 1, y );

	m_pData->pOutLineLT->SetPos( x, y );
	m_pData->pOutLineTP->SetPos( x, y );
	m_pData->pOutLineRT->SetPos( x + m_pData->vSize.x, y );
	m_pData->pOutLineBM->SetPos( x, y + m_pData->vSize.y );

	m_pData->m_pSprite->SetPos( x, y );
}


/**
*
*/
const vec2 & CBaseControl::GetPos() const
{
	if ( m_pData )
	{
		return m_pData->vPos;
	}

	return vec2::vNull;
}


/**
*
*/
void CBaseControl::SetSize( float fWidth, float fHeight )
{
	m_pData->vSize.x = fWidth;
	m_pData->vSize.y = fHeight;
	m_pData->bDirtyVB = true;

	m_pData->m_pSprite->SetSize( fWidth, fHeight );

	// borders

	m_pData->pOutLineLT->SetSize( 1, fHeight );
	m_pData->pOutLineTP->SetSize( fWidth, 1 );
	m_pData->pOutLineRT->SetSize( 1, fHeight );
	m_pData->pOutLineBM->SetSize( fWidth, 1 );

	SetPos( m_pData->vPos.x, m_pData->vPos.y );
}


/**
*
*/
const vec2 & CBaseControl::GetSize() const
{
	return m_pData->vSize;
}


/**
*
*/
void CBaseControl::SetClip( float fWidth, float fHeight )
{
	m_pData->vClip.x	= fWidth;
	m_pData->vClip.y	= fHeight;
	m_pData->bDirtyVB	= true;
	m_pData->bClip		= true;
}


/**
*
*/
const vec2 & CBaseControl::GetClip() const
{
	return m_pData->vClip;
}

/**
*
*/
vec2 CBaseControl::CalcPos() const
{
	vec2 vRes = m_pData->vPos;

	if ( m_pData->pParent )
		vRes.Add( m_pData->pParent->CalcPos() );

	return vRes;
}


/**
*
*/
const vec4 & CBaseControl::CalcClientRect()
{
	m_pData->m_vClientRect.pos_x = 0;//m_vPos.x;
	m_pData->m_vClientRect.pos_y = 0;//m_vPos.y;
	m_pData->m_vClientRect.width = m_pData->vSize.x;
	m_pData->m_vClientRect.height= m_pData->vSize.y;

	if ( UCF_STATIC_EDGE & m_pData->dwCtrlFlag )
	{
		m_pData->m_vClientRect.pos_x += 1;
		m_pData->m_vClientRect.pos_y += 1;
		m_pData->m_vClientRect.width -= 2;
		m_pData->m_vClientRect.height -= 2;
	}

	if ( UCF_CLIENT_EDGE &m_pData-> dwCtrlFlag )
	{
		m_pData->m_vClientRect.pos_x += 2;
		m_pData->m_vClientRect.pos_y += 2;
		m_pData->m_vClientRect.width -= 4;
		m_pData->m_vClientRect.height -= 4;
	}

	return m_pData->m_vClientRect;
}


/**
*
*/
const vec4 & CBaseControl::GetClientRect() const
{
	return m_pData->m_vClientRect;
}


/**
*
*/
bool CBaseControl::IsEnabled() const
{
	return m_pData->bEnabled;
}


/**
*
*/
void CBaseControl::Enable( bool bEnable )
{
	m_pData->bEnabled = bEnable;
}


/**
*
*/
void CBaseControl::Show( bool bShow )
{
	m_pData->bVisible = bShow;
}


/**
*
*/
bool CBaseControl::IsVisible() const
{
	return m_pData->bVisible;
}


/**
*
*/
bool CBaseControl::IsMouseOver( const ivec2 & vMouse )
{
	vec2 vAbsPos = CalcPos();

	if ( vMouse.x > vAbsPos.x )
	{
		if ( vMouse.y > vAbsPos.y )
		{
			if ( vMouse.x < ( vAbsPos.x + m_pData->vSize.x ) )
			{
				if ( vMouse.y < ( vAbsPos.y + m_pData->vSize.y ) )
				{
					for ( uint n = 0; n < GetChildCount(); ++n )
					{
						CBaseControl * pCtrl = GetChildControl( n );

						if ( pCtrl->IsMouseOver( vMouse ) )
							return false;
					}

					return true;
				}
			}
		}
	}

	return false;
}


/**
*
*/
dword CBaseControl::GetColor() const
{
	return m_pData->dwColor;
}


/**
*
*/
void CBaseControl::SetColor( dword dwColor )
{
	m_pData->dwColor = dwColor;

	float a = (float)( ( dwColor & 0xFF000000 ) >> 24 ) / 255.f;
	float r = (float)( ( dwColor & 0x00FF0000 ) >> 16 ) / 255.f;
	float g = (float)( ( dwColor & 0x0000FF00 ) >> 8  ) / 255.f;
	float b = (float)( ( dwColor & 0x000000FF )       ) / 255.f;

	CUniform uColorMod( "vColorMod", g_pRenderer->GetShader( UI_SHADER ) );
	uColorMod.Connect( m_pData->pMaterial );
	uColorMod.SetValue( &vec4( r, g, b, a ), 1 );
}


/**
*
*/
uint CBaseControl::GetChildCount() const
{
	return ( NULL != m_pData ) ? m_pData->m_pChilds.GetNumItems() : 0;
}


/**
*
*/
int CBaseControl::AddControl( CBaseControl * pControl, int nTag )
{
	if ( pControl )
	{
		pControl->m_pData->nTag = nTag;
		return m_pData->m_pChilds.Add( (CBaseControl *)pControl );
	}

	return INVALID_INDEX;
}


/**
*
*/
CBaseControl * CBaseControl::GetChildControl( uint nCtrlID )
{
	return ( nCtrlID < GetChildCount() ) ?  m_pData->m_pChilds[ nCtrlID ] : NULL;
}


////////////////////////////////////////////////////////////////////////////////

void CBaseControl::Render()
{
	if ( NULL == m_pData )
		return;

	if ( !IsVisible() )
		return;

	g_pUIManager->PushSprite( m_pData->m_pSprite );
	
	g_pUIManager->PushSprite( m_pData->pOutLineLT );
	g_pUIManager->PushSprite( m_pData->pOutLineTP );
	g_pUIManager->PushSprite( m_pData->pOutLineRT );
	g_pUIManager->PushSprite( m_pData->pOutLineBM );

	if ( m_pData->m_pText )
		m_pData->m_pText->Render();
		

	index_t nIndex = INVALID_INDEX;

	while ( m_pData->m_pChilds.Iterate( nIndex ) )
	{
		m_pData->m_pChilds[ nIndex ]->Render();
	}
};

////////////////////////////////////////////////////////////////////////////////