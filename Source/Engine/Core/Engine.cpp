////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "../UI/UIManager.h"
#include "../EasyWin/EasyWin.h"
#include "../Scene/Scene.h"
#include "../Material/Material.h"
//#include "../Material/ImageManager/ImageManager.h"
#include "../World/Entity/EntityManager.h"
//#include "../ResourceManager/ResourceManager.h"

#include "../Editors/MaterialEditor/MaterialEditor.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch ( Msg )
	{
		case WM_CLOSE:
			PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc( hWnd, Msg, wParam, lParam );
}

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CEngine::ms_ClassDesc = 
{
	7070,
	IT_ENGINE,
	1,
	"Engine by WW v0.1"
};

////////////////////////////////////////////////////////////////////////////////

CEngine *	g_pEngine	= NULL;
IRenderer * g_pRenderer	= NULL;
IVfs *		g_pVfs		= NULL;
IInput *	g_pInput	= NULL;

////////////////////////////////////////////////////////////////////////////////

CEngine::CEngine() :
	m_pUIManager	( NULL ),
	m_pImageManager	( NULL ),
	m_pWin			( NULL ),
	m_pCamera		( NULL ),
	m_pMaterialEditor( NULL )
{
	ITestRes * pRes = g_pSystem->CreateTestRes();

	if ( pRes )
	{
		pRes->Init( "UT_Engine.txt" );
		CTestRunner::Run( pRes );
		pRes->Release();
	}

	QueryPerformanceFrequency( &m_nPerfCounterFreq );
	QueryPerformanceCounter( &m_nStartPerfCount );

	m_pResManager = (IResManager *)g_pSystem->CreateInterface( 4321 );
}

////////////////////////////////////////////////////////////////////////////////

CSceneObject * pSceneObj = NULL;

////////////////////////////////////////////////////////////////////////////////

mat4 mX;
mat4 mY;
mat4 mZ;

EResult CEngine::Init( HINSTANCE hInst )
{
	mX.Identity();
	mY.Identity();
	mZ.Identity();

	mX.RotateY( 90 );
	mY.RotateX( 90 );

	if ( m_pWin = NEW CEasyWin )
	{
		m_pWin->SetSize( 1280, 1024 );
		m_pWin->Create( hInst );
	}

	g_pVfs = (IVfs *)g_pSystem->GetInterface( IT_VFS );
	g_pInput = (IInput *)g_pSystem->GetInterface( IT_INPUT );
	g_pXMLWrapper = (IXMLWrapper *)g_pSystem->GetInterface( IT_UTIL );
	g_pSoundMgr = (ISoundMgr *)g_pSystem->GetInterface( IT_SOUND_MGR );

	if ( g_pInput )
	{
		g_pInput->Init( hInst, m_pWin->GetHandle() );
		g_pInput->RegHandler( this );
	}

	if ( g_pSoundMgr )
	{
		g_pSoundMgr->Init( m_pWin->GetHandle() );
	}

	if ( g_pRenderer = (IRenderer *)g_pSystem->GetInterface( IT_RENDERER ) )
	{
		g_pRenderer->Init( m_pWin->GetHandle() );
	}


	
	if ( m_pUIManager = NEW CUIManager )
	{
		//m_pScene = NEW CScene;

		/*CWindow * pDeskTop = (CWindow *)m_pUIManager->CreateWin( NULL );

		if ( pDeskTop )
		{
			pDeskTop->SetSize( ivec2( 400, 400) );
			pDeskTop->SetPos( ivec2( 100, 100 ) );

			CButton * pButton = NEW CButton;
			pDeskTop->AddControl( pButton, 0 );
			pButton->SetCaption( "OK" );
			pButton->SetPos( ivec2( 20, 20 ));

			pButton = NEW CButton;
			pDeskTop->AddControl( pButton, 0 );
			pButton->SetCaption( "CANCEL" );
			pButton->SetPos( ivec2( 80, 20 ));
		}*/
	}

	//----------------------------------------------------------
	// Tip: texture
	//----------------------------------------------------------
	PTexture pBrickTex;
	LoadTexture( pBrickTex, "texture.tga" );
	//g_pRenderer->CreateTexture( pBrickTex, "texture.tga" );

	m_pDefMaterial = LoadMaterial( "Checker.tga" );
	g_pRenderer->SetDefTexture( m_pDefMaterial->GetTexture( DIFFUSE_MAP ) );


	RegisterCommands();

	//----------------------------------------------------------
	// Tip: lines
	//----------------------------------------------------------
	ZeroMemory( &m_lines, sizeof( m_lines ) );

	if ( m_lines.pLinesBuffer = g_pRenderer->CreateVB() )
	{
		m_lines.nMaxLines	= 65536;
		m_lines.nNumLines	= 0;
		m_lines.nBufferSize	= m_lines.nMaxLines * 2 * sizeof( TLines::TLineVertex );
		
		m_lines.pLinesBuffer->Alloc( m_lines.nBufferSize, 0 );
		m_lines.pLines = (TLines::TLineVertex *)m_lines.pLinesBuffer->Lock( 0, m_lines.nBufferSize );

		/*
		for (uint n = 0;  n < m_lines.nMaxLines * 2; ++n )
		{
			m_lines.pLines[ n ].vPoint.Set( 0, 0, 0 );
			m_lines.pLines[ n ].vColor.Set( 1, 1, 1, 1 );
		}
		*/
	}

	m_lines.pMaterial = CreateMaterial();
	m_lines.pMaterial->SetShader( g_pRenderer->GetShader( "Line" ) );
	m_lines.pMaterial->EnableDepthTest();

	if ( m_lines.pVDecl = g_pRenderer->CreateVertexDecl() )
	{
		m_lines.pVDecl->SetStride( sizeof( TLines::TLineVertex ) );
		m_lines.pVDecl->AddAttr( "vPos", NULL, TYPE_VEC3, 0 );
		m_lines.pVDecl->AddAttr( "vColor", NULL, TYPE_VEC4, sizeof( vec3 ) );
	}

	m_pEntityManager	= NEW CEntityManager;

	//----------------------------------------------------------
	// Tip: full frame quad
	//----------------------------------------------------------
	vec2 vertices[] =
	{
		vec2(-1.0f, -1.0f),
		vec2(0.0f, 0.0f),

		vec2(-1.0f, 1.0f),
		vec2(0.0f, 1.0f),

		vec2(1.0f, 1.0f),
		vec2(1.0f, 1.0f),

		vec2(1.0f, -1.0f),
		vec2(1.0f, 0.0f)
	};

	const uint bufferSize = sizeof(vertices);
	m_fullFrameQuad.vertexBuffer = g_pRenderer->CreateVB();
	m_fullFrameQuad.vertexBuffer->Alloc(bufferSize, 0);
	m_fullFrameQuad.vertexBuffer->UpdateData(0, vertices, bufferSize);

	m_fullFrameQuad.vertexDecl = g_pRenderer->CreateVertexDecl();
	m_fullFrameQuad.vertexDecl->SetStride(2 * sizeof(vec2));
	m_fullFrameQuad.vertexDecl->AddAttr("vPos", NULL, TYPE_VEC2, 0);
	m_fullFrameQuad.vertexDecl->AddAttr("vTex0", NULL, TYPE_VEC2, sizeof(vec2));

	return R_OK;
}

/**
*
*/
CEngine::~CEngine()
{
	RELEASE( m_pDefMaterial );

	m_pBindings.Delete();
	m_pCommandHash.Cleanup();

	m_lines.pLinesBuffer->Unlock();
	RELEASE( m_lines.pLinesBuffer );

	RELEASE(m_fullFrameQuad.vertexBuffer);
	DEL(m_fullFrameQuad.vertexDecl);


	DEL( m_pWin );
	DEL( m_pImageManager );
	DEL( m_pUIManager );

	RELEASE( m_pResManager );
	RELEASE( g_pRenderer );
	g_pVfs		= NULL;
}


/**
*
*/
void CEngine::Release()
{
	PURE_DEL( this );
}

////////////////////////////////////////////////////////////////////////////////

void CEngine::Update()
{
	m_pResManager->Update();

	g_pInput->ProcHandlers(); // process input

	// iterate world
	//m_pEntityManager->Update();

	const ICamera * pCam = GetCamera();
	FlushLines( pCam->GetViewProjMatrix() );

	// render ui
	m_pUIManager->Render();

	g_pRenderer->Present();
}


bool CEngine::ProcBindings( const TInputMsg & tData )
{
	if ( IMT_KEY_DOWN == tData.eType )
	{
		TBind * pBindings = m_pBindings.GetArray();

		for ( uint i = 0; i < m_pBindings.GetCount(); ++i, ++pBindings )
		{
			if ( tData.dwData == pBindings->dwCode )
			{
				Command( pBindings->sCommand.GetString() );
				return true;
			}
		}
	}

	return false;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CEngine::PushLine( const vec3 & vStart, const vec3 & vEnd, dword dwColor )
{
	DEBUG_ASSERT( m_lines.pLines );

	if ( NULL == m_lines.pLines )
		return;

	if ( m_lines.nNumLines <m_lines.nMaxLines )
	{
		uint nOffset = m_lines.nNumLines * 2;
		m_lines.pLines[ nOffset ].vPoint = vStart;
		m_lines.pLines[ nOffset ].vColor.Set(
			(float)(( dwColor & 0x00FF0000 ) >> 16	)/ 255.0f,
			(float)(( dwColor & 0x0000FF00 ) >> 8	)/ 255.0f,
			(float)(( dwColor & 0x000000FF )		)/ 255.0f,
			(float)(( dwColor & 0xFF000000 ) >> 24	)/ 255.0f );

		++nOffset;
		m_lines.pLines[ nOffset ].vPoint = vEnd;
		m_lines.pLines[ nOffset ].vColor.Set(
			(float)(( dwColor & 0x00FF0000 ) >> 16	)/ 255.0f,
			(float)(( dwColor & 0x0000FF00 ) >> 8	)/ 255.0f,
			(float)(( dwColor & 0x000000FF )		)/ 255.0f,
			(float)(( dwColor & 0xFF000000 ) >> 24	)/ 255.0f );

		++m_lines.nNumLines;
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CEngine::PushBBox( const bbox & box, dword dwColor )
{
	PushLine( box.vMin, vec3( box.vMin.x, box.vMax.y, box.vMin.z ), dwColor );
	PushLine( vec3( box.vMin.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMax.y, box.vMin.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMin.y, box.vMin.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMin.y, box.vMin.z ), box.vMin, dwColor );

	PushLine( vec3( box.vMin.x, box.vMin.y, box.vMax.z ), vec3( box.vMin.x, box.vMax.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMin.x, box.vMax.y, box.vMax.z ), vec3( box.vMax.x, box.vMax.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMax.y, box.vMax.z ), vec3( box.vMax.x, box.vMin.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMin.y, box.vMax.z ), vec3( box.vMin.x, box.vMin.y, box.vMax.z ), dwColor );

	PushLine( vec3( box.vMin.x, box.vMin.y, box.vMin.z ), vec3( box.vMin.x, box.vMin.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMin.x, box.vMax.y, box.vMin.z ), vec3( box.vMin.x, box.vMax.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMax.y, box.vMin.z ), vec3( box.vMax.x, box.vMax.y, box.vMax.z ), dwColor );
	PushLine( vec3( box.vMax.x, box.vMin.y, box.vMin.z ), vec3( box.vMax.x, box.vMin.y, box.vMax.z ), dwColor );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CEngine::FlushLines( const mat4 & mWVP )
{
	m_lines.pLinesBuffer->Unlock();
	m_lines.pLines = NULL;	

	CUniform unfmMVP( "mMVP", m_lines.pMaterial->GetShader() );
	unfmMVP.Connect( m_lines.pMaterial );
	unfmMVP.SetValue( (vec4*)mWVP.pArray, 4 );

	m_lines.pMaterial->Assign();

	g_pRenderer->EnableDepthTest(false);
	m_lines.pLinesBuffer->Render( m_lines.pVDecl, 0, m_lines.nNumLines * 2, PRIM_LINE );
	g_pRenderer->EnableDepthTest(true);

	m_lines.nNumLines = 0;
	m_lines.pLines = (TLines::TLineVertex *)m_lines.pLinesBuffer->Lock( 0, m_lines.nBufferSize );
}


////////////////////////////////////////////////////////////////////////////////

void CEngine::MaterialEditor()
{
	if ( !m_pMaterialEditor )
	{
		m_pMaterialEditor = NEW CMaterialEditor;
	}

	if ( m_pMaterialEditor )
	{
		m_pMaterialEditor->Activate( !m_pMaterialEditor->IsActive() );
	}
}

////////////////////////////////////////////////////////////////////////////////

IEntityManager * CEngine::GetEntityManager()
{
	return m_pEntityManager;
}

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CEngine::Sprite2D( IMaterial * pMat, const ivec2 & vPos, const ivec2 & vSize, const vec2 * pUVPos, const vec2 * pUVSize )
{
	DEBUG_ASSERT( false );

	//	3----2		point 0 = vPos
	//	|    |		point 2 = vPos + vSize
	//	0----1

	IMesh * m_pSpriteMesh = NULL;

	if ( m_pSpriteMesh )
	{
		mat4 m;
		m.Identity();

		/*
		g_pRenderer->PushMatrix( MM_MODELVIEW );
		m = (* g_pRenderer->GetMatrix( MM_MODELVIEW ));
		m.Scale( vec3( (float)vSize.x, (float)vSize.y, 1.f ) );
		m.Translate( vec3( (float)vPos.x, -(float)vPos.y, 0 ) );
		g_pRenderer->SetMatrix( MM_MODELVIEW, m );
		*/

		if ( pUVPos || pUVSize )
		{
			/*
			g_pRenderer->PushMatrix( MM_TEXTURE );
			m = (* g_pRenderer->GetMatrix( MM_TEXTURE ));

			if ( pUVSize )
				m.Scale( vec3( pUVSize->x, pUVSize->y, 0 ) );

			if ( pUVPos )
				m.Translate( vec3( pUVPos->x, pUVPos->y, 0 ) );

			g_pRenderer->SetMatrix( MM_TEXTURE, m );
			*/
		}

		//m_pSpriteMesh->SetMaterial( pMat );
		m_pSpriteMesh->Render( pMat );

		/*
		g_pRenderer->PopMatrix( MM_MODELVIEW );

		if ( pUVPos || pUVSize )
			g_pRenderer->PopMatrix( MM_TEXTURE );
			*/
	}
}


bool CEngine::DrawFullframeQuad(IRenderTarget * frameBuffer, IMaterial * pMaterial)
{
	PTexture frameBufferTex = frameBuffer->GetTexture();
	const TImage & textureDesc = frameBufferTex->GetDesc();

	ivec2 prevViewport = g_pRenderer->GetViewPort();
	g_pRenderer->SetRenderTarget(frameBuffer);
	g_pRenderer->SetViewPort(ivec2(textureDesc.nWidth, textureDesc.nHeight));

	//g_pRenderer->ClearScene();

	pMaterial->Assign();

	m_fullFrameQuad.vertexBuffer->Render(
		m_fullFrameQuad.vertexDecl,
		0, 4, PRIM_QUAD);

	
	/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)textureDesc.nWidth, 0.0, (GLdouble)textureDesc.nHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// render

	glBegin(GL_QUADS);

	glVertex2f(-1.0f, -1.0f);
	//glTexCoord2f(0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);

	glVertex2f(-1.0f, 1.0f);
	//glTexCoord2f(0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);

	glVertex2f(1.0f, 1.0f);
	//glTexCoord2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);

	glVertex2f(1.0f, -1.0f);
	//glTexCoord2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);

	glEnd();
	GL_VALIDATE;

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	*/
	g_pRenderer->SetRenderTarget(NULL);
	g_pRenderer->SetViewPort(prevViewport);

	return false; // undone
}