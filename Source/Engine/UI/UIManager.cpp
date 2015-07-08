////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"
#include "Console.h"

#include <stdio.h>
#include <gl/gl.h>
#pragma comment ( lib, "opengl32.lib" )

//#include "d3d10.h"
//#include "d3dx10.h"
//#pragma comment ( lib, "d3dx10.lib" )

////////////////////////////////////////////////////////////////////////////////

CUIManager * g_pUIManager = NULL;

/**
*
*/
CUIManager::CUIManager() :
	m_pInput			( NULL	),
	m_pActiveControl	( NULL	),
	m_pDesktop			( NULL	),

	m_pWindowMaterial	( NULL	),
	m_pFontMaterial		( NULL	),
	m_pCursorMaterial	( NULL	),
	m_pCursorSprite		( NULL	),
	m_pDefaultFont		( NULL	),
	m_bShowCursor		( true	),

	m_pSpriteVrts		( NULL	),
	m_nVertexCount		( 0		),
	m_nMaxVertices		( 0		)
{
	g_pUIManager = this;

	int w = 800, h = 600;// - 25; // clien area @@@
	g_pRenderer->SetViewPort( ivec2( w, h ) );

	const ivec2 & vViewPort = g_pRenderer->GetViewPort();

	const TIntfDesc * pDesc = g_pRenderer->GetInterfaceDesc();

	m_mWorld.Identity();
	//m_mProj.Ortho( 0, (float)vViewPort.x, (float)vViewPort.y, 0, -1.f, 1.f );

	if ( CStr::Compare( "GL Renderer by WW v0.1", pDesc->szDescString ) )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, (float)vViewPort.x, 0, (float)vViewPort.y, -1, 1 );
		glGetFloatv( GL_PROJECTION_MATRIX, m_mProj.pArray );

		m_mWorld.m22 = -1.0f; // -1
		m_mWorld.m42 = 600; // 600
	}
	else
	{
		/*
		D3DXMatrixOrthoOffCenterLH(
		(D3DXMATRIX *)&m_mProj,
		0,
		(float)vViewPort.x,
		(float)vViewPort.y,
		0,
		-1,
		1 );
		*/
	}

	m_vMousePos.x = 20;
	m_vMousePos.y = 20;

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------

	m_pInput = (IInput *)g_pSystem->GetInterface( IT_INPUT );

	LoadFont( "UI/MyFont.tab", "UI/MyFont000.tga" );

	InitSprites();

	IShader * pUIShader = g_pRenderer->GetShader( UI_SHADER );
	CUniform uColorMod( "vColorMod", pUIShader );
	//uColorMod.Connect( m_pFontMaterial );
	//uColorMod.SetValue( &vec4( 1, 0, 0, 1 ), 1 );

	//--------------------------------------------------------------------------
	// Tip: Создание материала для окошек
	//--------------------------------------------------------------------------
	if ( m_pWindowMaterial = g_pEngine->CreateMaterial() )
	{
		m_pWindowMaterial->SetShader( pUIShader );
		m_pWindowMaterial->EnableAlphaTest( false );
		m_pWindowMaterial->EnableDepthTest( false );
		m_pWindowMaterial->EnableDepthWrite( false );

		m_pWindowMaterial->SetTexture( DIFFUSE_MAP,
			g_pRenderer->GetSysTexture( EST_GREY ) );

		uColorMod.Connect( m_pWindowMaterial );
		uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );
	}

	//--------------------------------------------------------------------------
	// Tip: Создание материала для курсора мышки
	//--------------------------------------------------------------------------
	if ( m_pCursorMaterial = g_pEngine->CreateMaterial() )
	{
		m_pCursorMaterial->SetShader( pUIShader );

		uColorMod.Connect( m_pCursorMaterial );
		uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );

		// g_pEngine->LoadMaterial( "Materials/Cursor.xml" );

		PTexture pTex;

		if ( !g_pEngine->LoadTexture( pTex, "UI/pointer.tga" ) )
		{
			pTex = g_pRenderer->GetSysTexture( EST_DEFAULT );
		}
		else
		{
			//pTex->SetFilter( FILTER_NEAREST );
		}

		m_pCursorMaterial->SetTexture( DIFFUSE_MAP, pTex );
		m_pCursorMaterial->EnableAlphaTest( true );
		m_pCursorMaterial->EnableDepthTest( false );
		m_pWindowMaterial->EnableDepthWrite( false );
	}

	m_pCursorSprite = CreateSprite();
	m_pCursorSprite->SetSize( 32, 32 );
	m_pCursorSprite->SetMaterial( m_pCursorMaterial );
	//m_pCursorSprite->SetClip( 128, 64 );

	g_pInput->RegHandler( this );

	//NEW CConsole();

	m_pSpriteCount		= g_pSystem->AddCounter( "UI: sprite count" );
	m_pSpriteDIPcount	= g_pSystem->AddCounter( "UI: sprite DIP count" );
	m_pDebugText		= CreateText();

	m_pDebugText->SetPos( 10.f, float( vViewPort.y - 80.f ) );
}


/**
*
*/
CUIManager::~CUIManager()
{
	RELEASE( m_pDebugText );

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	for( index_t n = INVALID_INDEX; m_WindowList.Iterate( n ); )
	{
		CBaseControl * pCtrl = m_WindowList[ n ];
		DEL( pCtrl );
	}

	m_WindowList.Delete();

	//--------------------------------------------------------------------------
	//	Tip: удаление спрайтов
	//--------------------------------------------------------------------------
	for( uint n = 0; n < MAX_SPRITES; ++n )
	{
		DEL( m_pSprites[ n ] );
	}

	RELEASE( m_pSpritesVB );
	RELEASE( m_pSpriteIB );

	m_pSpriteDIPs.Delete();
	m_FreeSpriteIDs.Delete();

	DEL_ARRAY( m_pSpriteVrts );
	DEL_ARRAY( m_pSpriteIndices );

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	DEL( m_pWindowMaterial );
	DEL( m_pFontMaterial );
	DEL( m_pDefaultFont );
	DEL( m_pCursorMaterial );
}


/**
*
*/
IUIManager * CEngine::GetUIManager()
{
	return m_pUIManager;
}


/**
*
*/
void CUIManager::Render()
{
	if ( !m_pDesktop )
	{
		DEBUG_MSG( "aaa :)" );
		return;
	}

	m_pDesktop->Render();

	m_pCursorSprite->SetPos( (float)m_vMousePos.x, (float)m_vMousePos.y );
	PushSprite( m_pCursorSprite );

	//--------------------------------------------------------------------------
	// Tip: Debug
	//--------------------------------------------------------------------------
	RenderDebugInfo();
	m_pSpriteCount->ResetValue();
	m_pSpriteDIPcount->ResetValue();

	RenderSprites();
}

////////////////////////////////////////////////////////////////////////////////

bool CUIManager::ProcInputEvent( const TInputMsg & tData )
{
	const ivec2 & vViewPort = g_pRenderer->GetViewPort();
	
	//------------------------------------------------------------------
	// Tip: Move cursor
	//------------------------------------------------------------------
	switch ( tData.eType )
	{
		case IMT_MOUSE_MOVE_X:

			m_vMousePos.x += tData.dwData;

			if ( m_vMousePos.x < 0 )
				m_vMousePos.x = 0;

			if ( m_vMousePos.x > vViewPort.x )
				m_vMousePos.x = vViewPort.x;

		break;

		case IMT_MOUSE_MOVE_Y:

			m_vMousePos.y += tData.dwData;

			if ( m_vMousePos.y < 0 )
				m_vMousePos.y = 0;

			if ( m_vMousePos.y > vViewPort.y )
				m_vMousePos.y = vViewPort.y;

		break;
	}

	if ( m_pDesktop )
	{
		m_pDesktop->ProcessInput( tData );
//		m_pDesktop->ProcInputEvent( tData );
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void CUIManager::InitControl( CBaseControl * pCtrl )
{
	if ( pCtrl )
	{
		CBaseControl * pParent = pCtrl->GetParent();

		if ( pParent )
			pParent->AddControl( pCtrl, 0 );

		if ( pCtrl->Init() )
		{
			if ( !pParent )
				m_WindowList.Add( pCtrl );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////