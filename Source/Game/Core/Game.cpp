////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Engine/UI.h"

#define DIRECTINPUT_VERSION  0x0800
#include "dinput.h"
#include "DeskTop.h"
#include "ProjectedGrid.h"

#include "../Editor/Editor.h"
#include "../Animation.h"

#include "../Locations/BaseLocation.h"

////////////////////////////////////////////////////////////////////////////////

CGame *				g_pGame			= NULL;
CEditor *			g_pEditor		= NULL;
ILandscape *		g_pLandscape	= NULL;
IEntityManager *	g_pEM			= NULL;
IVarStorage *		g_pVarStorage	= NULL;
IWorldIter *		g_pWorld		= NULL;
IResManager *		g_pResManager	= NULL;

////////////////////////////////////////////////////////////////////////////////

#include "_Shared/UnitTest.h"

////////////////////////////////////////////////////////////////////////////////

void myFunc()
{
	DEBUG_ASSERT( false );
}

////////////////////////////////////////////////////////////////////////////////

UNIT_TEST( CMyTest )
{
	UNIT_ASSERT( 1 == 1 );

	myFunc();

	UNIT_ASSERT( 0 == 1 );
}

////////////////////////////////////////////////////////////////////////////////

UNIT_TEST( CMyTest2 )
{
	UNIT_ASSERT( 0 == strcmp( "One", "One" ) );
}

////////////////////////////////////////////////////////////////////////////////


void RegVars()
{
	IVar * pVar = g_pVarStorage->RegVar( "render_module" );

	pVar->SetString( "RendererD3D" );

	const char * szTest = pVar->GetString();
	/*
	uint nRenderModuleVar = g_pVarStorage->RegVar( "render_module" );

	g_pVarStorage->SetString( nRenderModuleVar, "RendererD3D" );


	REG_VAR_STRING( "render_module", "RendererD3D", NULL );
	*/
};



//!

//!
//!
EResult CGame::Init()
{
	ITestRes * pRes = g_pSystem->CreateTestRes();

	if ( pRes )
	{
		pRes->Init( "game_unit_test.txt" );
		CTestRunner::Run( pRes );
		pRes->Release();
	}

	////////////////////////////////////////////////////////////////////////////

	g_pEditor = NEW CEditor;
	RegisterCommands();

	g_pEngine->Command( "bind space toggle_editor" );

	return R_OK;
}

float t = 0;


class CBruteIter : public IWorldIter
{
	bool		AddEntity( IEntity * pEnt )
	{
		if ( pEnt )
		{
			return ( INVALID_INDEX != m_pEntities.Add( pEnt ) );
		}

		return false;
	}

	bool Reset()
	{
		m_iCurrent = INVALID_INDEX;
		return true;
	}

	IEntity	* Next()
	{
		if ( m_pEntities.Iterate( m_iCurrent ) )
			return m_pEntities[ m_iCurrent ];

		return NULL;
	}

	CContainer <IEntity *> m_pEntities;
	index_t m_iCurrent;
};


/**
*
*/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd )
{


	char szBuffer[ 64 ];
	szBuffer[ 0 ] = 0;

	PROC_CREATESYSTEM SystemCreate = NULL;
	HMODULE hSysModule = LoadLibrary( "Modules/System.dll" );

	if ( NULL == hSysModule )
		return -1;
	
	SystemCreate = (PROC_CREATESYSTEM)GetProcAddress( hSysModule, "CreateSystem" );
	
	if ( NULL == SystemCreate )
		return -1;

	g_pSystem = SystemCreate();

	if ( NULL == g_pSystem )
		return -1;

	g_pSystem->Init();
	g_pVarStorage = g_pSystem->GetVarStorage();
	RegVars();

								g_pSystem->CreateInterface( 2020 ); // XML WRAPPER
	g_pRenderer = (IRenderer *)	g_pSystem->CreateInterface( 8084 ); // RENDERER
	g_pVfs		= (IVfs *)		g_pSystem->CreateInterface( 8080 ); // VFS
	g_pInput	= (IInput *)	g_pSystem->CreateInterface( 0 );	// INPUT
	g_pEngine	= (IEngine *)	g_pSystem->CreateInterface( 7070 ); // ENGINE
	g_pSoundMgr = (ISoundMgr *) g_pSystem->CreateInterface( 111 );

	g_pWorld	= NEW CBruteIter;

	if ( g_pEngine )
	{
		g_pEngine->Init( hInst );

		g_pResManager	= g_pEngine->GetResManager();
		g_pUIManager	= g_pEngine->GetUIManager();
		g_pEM			= g_pEngine->GetEntityManager();

		g_pGame = NEW CGame;
		g_pGame->Init();

		g_pGame->m_pDeskTop = NEW CDeskTop;

		if ( g_pGame->m_pDeskTop )
		{
			g_pGame->m_pDeskTop->Show( false );
			g_pGame->m_pDeskTop->SetPos( 100, 100 );
			g_pGame->m_pDeskTop->SetSize( 100, 100 );
			g_pGame->m_pDeskTop->SetText( "Desktop" );
			g_pUIManager->SetDesktop( g_pGame->m_pDeskTop );

			//g_pInput->RegHandler( g_pGame->m_pDeskTop );
		}
	}

	IEasyWin * pWin = g_pEngine->GetWindow();
	g_pGame->m_pCamera = g_pEngine->CreateCamera();
	g_pEngine->SetCamera( g_pGame->m_pCamera );

	dword dwNumFPS = 0;
	dword dwShowTime = GetTickCount() + 1000;

//	IScene * pScene = g_pEngine->GetScene();

	//////////////////////////////////////////////////////////////////////////

	IBaseLoc * pLoc = CreateRoom();
	pLoc->Init();

	//////////////////////////////////////////////////////////////////////////
	
	IMaterial * pParticleMat = g_pEngine->CreateMaterial();

	pParticleMat->SetShader( NULL /*g_pRenderer->GetShader( "Particle" )*/ );
	pParticleMat->EnableAlphaTest();
	pParticleMat->EnableDepthTest( false );
	pParticleMat->EnableAlphaBlend();
	pParticleMat->BlendFunc( BF_SRC_ALPHA, BF_ONE );

	PTexture pParticleTex;
	g_pEngine->LoadTexture( pParticleTex, "particle.tga" );
	pParticleMat->SetTexture( DIFFUSE_MAP, pParticleTex );

	IParticleSystem * pEmitter = g_pEngine->CreateParticleSys();
	pEmitter->Init();

	////////////////////////////////////////////////////////////////////////

	IMaterial * pMat = g_pEngine->CreateMaterial();

	g_pLandscape = g_pEngine->CreateLandscape();

	PTexture pTex;
	g_pEngine->LoadTexture( pTex, "texture.tga" );
	pMat->SetTexture( DIFFUSE_MAP, pTex );
	pMat->SetShader( g_pRenderer->GetShader( LAND_SHADER ) );

	g_pLandscape->Load( "heightmap.raw" );
	g_pLandscape->SetMaterial( pMat );

	////////////////////////////////////////////////////////////////////////

	//IEntity * pEnt = g_pEM->CreateEntity( "Entities/first.xml" );
	IEntity * pEnt2 = g_pEM->CreateEntity( "Entities/Chars/Crown.xml" );

	g_pWorld->AddEntity( pEnt2 );

	////////////////////////////////////////////////////////////////////////

	IMaterial * pRTM = g_pEngine->CreateMaterial();

	ivec2 vSize( 800, 600 );
	IRenderTarget * pRT = g_pRenderer->CreateRenderTarget();

	/*
	PTexture pBackTex;

	if ( pRT )
	{
		pRT->Init( vSize.x, vSize.y );
		pRTM->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
		pRTM->SetTexture( DIFFUSE_MAP, g_pRenderer->GetSysTexture( EST_DEFAULT ) );
		//g_pEngine->LoadTexture( pBackTex, "back.tga" );
		//pRTM->SetTexture( DIFFUSE_MAP, pBackTex );
		//pRTM->EnableAlphaTest( true );
		//pRTM->EnableAlphaBlend( true );
		//pRTM->BlendFunc( BF_SRC_ALPHA, BF_ONE );

		pRTM->EnableDepthTest( false );
		pRTM->EnableDepthWrite( false );

		CUniform uColorMod( "vColorMod", pRTM->GetShader() );
		uColorMod.Connect( pRTM );
		uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );
	}*/

	////////////////////////////////////////////////////////////////////////

	CProjectedGrid grid;

	SysWrite( "Enetring main loop" );
	SysWrite( "--------------------------------------------------------------------------------" );

	while ( pWin->Idle() )
	{
		g_pRenderer->SetViewPort( vSize );
/*
		ivec2 vp = g_pRenderer->GetViewPort();
		g_pRenderer->SetRenderTarget( pRT );
		{
			g_pRenderer->SetViewPort( vSize );
			g_pRenderer->ClearScene();

			mat4 m;
			m.Projection( 34.516f, 4.f / 3.f, 1, 200 );
			g_pRenderer->SetMatrix( MM_PROJECTION, m );

			m.Identity();
			g_pRenderer->Circle3D( m, 5, 0xFFFF0000 );

			ICamera * pCam = pScene->GetCamera();
			pCam->BuildMatrix( m );
			g_pRenderer->SetMatrix( MM_MODELVIEW, m );

			if ( pEnt )
				pEnt->Render();
		}
		g_pRenderer->SetRenderTarget( NULL );
		g_pRenderer->SetViewPort( vp );
*/
		g_pRenderer->ClearScene();

			//mat4 mCam;
			//pCam->BuildMatrix( mCam );

//			pScene->Update();
//			ICamera * pCam = pScene->GetCamera();

//			pLoc->Update();
			
			g_pGame->ProcKeyboard();
			g_pGame->m_pCamera->Update();
			pLoc->Update();

			//g_pLandscape->Render( g_pGame->m_pCamera );
			g_pLandscape->Trace(g_pGame->m_pCamera, g_pUIManager->GetMousePos());

			//if ( g_pEditor->IsActive() )
				g_pEditor->Update();

			//pEmitter->Update();
			//pEmitter->Render( pParticleMat );
			
			/*mat4 mMVP;
			mMVP.Identity();
			mMVP.Translate( vec3( -3.f, 11.7f, -60.f ) ); // model matrix
			mMVP.Mult( mCam ); // view matrix
			mMVP.Mult( mProj ); // projection matrix*/

			pEnt2->SetDir( 0 /*45*/ );
			//pEnt2->SetPos( vec3( -3.f, 11.7f, -60.f ) );
 			//pEnt2->Render();

			//pTex->Bind();
			
			grid.Render();

			g_pEngine->Update();

			float s = g_pEngine->GetTime();

		g_pSoundMgr->Update();

		//------------------------------------------------------------------
		// Tip: frame counter
		//------------------------------------------------------------------
		float e = g_pEngine->GetTime();
		t = e - s;

		++dwNumFPS;

		if ( GetTickCount() > dwShowTime )
		{
			sprintf_s( szBuffer, "FPS: %d", dwNumFPS );
			pWin->SetTitle( szBuffer );

			dwShowTime	= GetTickCount() + 1000;
			dwNumFPS	= 0;
		}
	}

	grid.Release();

	//RELEASE( pEnt );
	RELEASE( pEnt2 );

	RELEASE( pRTM );
	DEL( pRT );
	RELEASE( pParticleMat );
	DEL( pEmitter );
	DEL( g_pLandscape );
	RELEASE( pMat );

	RELEASE( pTex );
	RELEASE( pParticleTex );

	DEL( g_pEditor );

	RELEASE( g_pEngine );
	RELEASE( g_pSoundMgr );

	RELEASE( g_pInput );
	RELEASE( g_pVfs );

	RELEASE( g_pSystem );
	FreeLibrary( hSysModule );

	return 0;
}


/**
*
*/
void CGame::ProcKeyboard()
{
	const char * pKeys = g_pInput->GetKeyboardState();

	if ( pKeys )
	{
		const float fStep	= 0.04f;

		float	fShiftX = 0,
			fShiftY = 0,
			fShiftZ = 0;

		if ( pKeys[ DIK_W ] )
			fShiftZ = -fStep;

		if ( pKeys[ DIK_S ] )
			fShiftZ = fStep;

		if ( pKeys[ DIK_A ] )
			fShiftX -= fStep;

		if ( pKeys[ DIK_D ] )
			fShiftX += fStep;

		if ( pKeys[ DIK_Q ] )
			fShiftY += fStep;

		if ( pKeys[ DIK_Z ] )
			fShiftY -= fStep;

		vec3 vOffset( fShiftX, fShiftY, fShiftZ );

		mat4 m;

		g_pGame->m_pCamera->GetQuat()->ToMatrix( m );
		vOffset.Multiply( m );

		if ( pKeys[ DIK_A ] || pKeys[ DIK_D ] )
		{
			if ( !fShiftX && !fShiftZ )
			{
				vOffset.y = 0;
				vOffset.NormalizeTo( fStep );
			}
		}

		g_pGame->m_pCamera->Move( vOffset );
	}
}

////////////////////////////////////////////////////////////////////////////////