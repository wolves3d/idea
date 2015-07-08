////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "../Shaders/BaseShader.h"

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CRenderer_GL::ms_ClassDesc = 
{
	8084,
	IT_RENDERER,
	1,
	"GL Renderer by WW v0.1"
};

////////////////////////////////////////////////////////////////////////////////

// Таблицы :)
GLenum g_pElemTypeGL[ LAST_TYPE ];
GLenum g_pElemCountGL[ LAST_TYPE ];

struct TFillTable
{
	TFillTable()
	{
		g_pElemTypeGL[ TYPE_VEC2 ] = GL_FLOAT;
		g_pElemTypeGL[ TYPE_VEC3 ] = GL_FLOAT;
		g_pElemTypeGL[ TYPE_VEC4 ] = GL_FLOAT;
		g_pElemTypeGL[ TYPE_DWORD ] = GL_UNSIGNED_BYTE;

		g_pElemCountGL[ TYPE_VEC2 ] = 2;
		g_pElemCountGL[ TYPE_VEC3 ] = 3;
		g_pElemCountGL[ TYPE_VEC4 ] = 4;
		g_pElemCountGL[ TYPE_DWORD ] = 4;
	}
} g_TableInstance;

////////////////////////////////////////////////////////////////////////////////

GLenum CRenderer_GL::m_BlendFactorTable[ BF_MAX_FACTORS ];
GLenum CRenderer_GL::m_AlphaFuncTable[ AF_MAX_FUNCS ];

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
CRenderer_GL::CRenderer_GL() :
	m_pActiveShader	( NULL )
{
	m_AlphaFuncTable[ AF_LEQUAL ]					= GL_LEQUAL;
	m_AlphaFuncTable[ AF_EQUAL ]					= GL_EQUAL;
	m_AlphaFuncTable[ AF_GEQUAL ]					= GL_GEQUAL;

	m_BlendFactorTable[ BF_ZERO ]					= GL_ZERO;
	m_BlendFactorTable[ BF_ONE ]					= GL_ONE;

	m_BlendFactorTable[ BF_SRC_ALPHA ]				= GL_SRC_ALPHA;
	m_BlendFactorTable[ BF_SRC_COLOR ]				= GL_SRC_COLOR;

	m_BlendFactorTable[ BF_DST_ALPHA ]				= GL_DST_ALPHA;
	m_BlendFactorTable[ BF_DST_COLOR ]				= GL_DST_COLOR;

	m_BlendFactorTable[ BF_ONE_MINUS_SRC_ALPHA ]	= GL_ONE_MINUS_SRC_ALPHA;
	m_BlendFactorTable[ BF_ONE_MINUS_SRC_COLOR ]	= GL_ONE_MINUS_SRC_COLOR;

	m_BlendFactorTable[ BF_ONE_MINUS_DST_ALPHA ]	= GL_ONE_MINUS_DST_ALPHA;
	m_BlendFactorTable[ BF_ONE_MINUS_DST_COLOR ]	= GL_ONE_MINUS_DST_COLOR;

	m_BlendFactorTable[ BF_SRC_ALPHA_SATURATE ]		= GL_SRC_ALPHA_SATURATE;

	for ( uint n = 0; n < EXT_GL_MAX; ++n )
	{
		m_bExtensions[ n ] = false;
	}

	for (uint n = 0; n < SHADER_COUNT; ++n)
	{
		m_ppShaders[n] = NULL;
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
const TIntfDesc * CRenderer_GL::GetInterfaceDesc() const
{
	return &ms_ClassDesc;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CRenderer_GL::Init( HWND hWindow )
{
	m_hWindow	= hWindow;
	g_pRenderer = this;
	g_pVfs		= ( IVfs* )g_pSystem->GetInterface( IT_VFS );

	//WriteLog( "Renderer Init...\n" );	

	m_hDevCon = GetDC( hWindow );

	PIXELFORMATDESCRIPTOR   PixFmtDesc;
	int                   iPixelFormat;

	memset( &PixFmtDesc, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

	PixFmtDesc.nSize        = sizeof( PIXELFORMATDESCRIPTOR );
	PixFmtDesc.nVersion     = 1;
	PixFmtDesc.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	PixFmtDesc.iPixelType   = PFD_TYPE_RGBA;
	PixFmtDesc.cColorBits   = 32;
	PixFmtDesc.cDepthBits   = 24;
	PixFmtDesc.iLayerType   = PFD_MAIN_PLANE;
	PixFmtDesc.cAccumBits   = 0;
	PixFmtDesc.cStencilBits = 8;
	PixFmtDesc.cAlphaBits   = 0;

	//Write( "Initing OpenGL..." );

	if ( iPixelFormat = ChoosePixelFormat( m_hDevCon, &PixFmtDesc ) )
	{
		if ( SetPixelFormat( m_hDevCon, iPixelFormat, &PixFmtDesc ) )
		{
			if ( m_hGLRenderCon = wglCreateContext( m_hDevCon ) )
			{
				if ( wglMakeCurrent( m_hDevCon, m_hGLRenderCon ) )
				{
					/*WriteOK();
					Write( "\tVendor: %s\n", glGetString( GL_VENDOR ) );
					Write( "\tRenderer: %s\n", glGetString( GL_RENDERER ) );
					Write( "\tVersion: %s\n", glGetString( GL_VERSION ) );*/
				}
				else
				{
					/*WARNING( "wglMakeCurrent failed!" );
					return WriteFAIL();*/
				}
			}
			else
			{
				/*WARNING( "wglCreateContext failed!" );
				return WriteFAIL();*/
			}
		}
		else
		{
			/*WARNING( "SetPixelFormat failed!" );
			return WriteFAIL();*/
		}
	}
	else
	{
		/*WARNING( "ChoosePixelFormat failed!" );
		return WriteFAIL();*/
	}

	/*if ( !InitExtensions() )
	{
		WARNING( "Required extensions not found!" );
		return false;
	}

	Write( "--------------------------------------------------\n" );
*/
	//m_dwMaxBonesPerDraw = 20;
	//glGetIntegerv( GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &a );
	//glGetIntegerv( GL_MAX_VARYING_FLOATS_ARB, &b );
	//glGetIntegerv( GL_MAX_VERTEX_ATTRIBS_ARB, &c );

	//RegisterCommands();// script funcs

	InitExtensions();

	wglSwapIntervalEXT( 0 ); // v-sync

	int w = 800, h = 600 - 25; // clien area @@@
	//pWin->GetSize( &w, &h );

	double   fAspect = ( double )w / ( double )h;

	double	zNear	= 1.8,
			zFar	= 20.0;

	glEnable( GL_DEPTH_TEST );

	//SetViewPort( ivec2( w, h ) );

	glClearColor  ( 0.0f, 0.0f, 0.0f, 1.0f );
	glMatrixMode  ( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 34.516, fAspect, zNear, zFar );
	glMatrixMode  ( GL_MODELVIEW );

	//glEnable( GL_CULL_FACE );
	//glCullFace( GL_BACK );

	glEnable( GL_TEXTURE_2D );
	
	glAlphaFunc( GL_GREATER, 0.01f );

	ReloadShaders();

	//if ( m_pCursor )
		//ShowCursor( false );

	//m_pAudioMgr->Init();
	//m_pAudioMgr->Play( m_pAudioMgr->OpenStream( "test.ogg" ), true );

	m_mModelView.Identity();
	m_mTexture.Identity();

	PTexture	pTex;

	TImage	tImage;
	dword	dwColor	= 0xFFFFFFFF;
	tImage.nWidth	= 1;
	tImage.nHeight	= 1;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 4;
	tImage.pData	= (byte *)&dwColor;
	CreateTexture( pTex, "Sys_White" );
	pTex->Init( tImage, EAT_IMMUTABLE );
	m_pSysTextures[ EST_WHITE ] = pTex;
	tImage.pData = NULL;

	dwColor	= 0xFFD4D0C8;
	tImage.nWidth	= 1;
	tImage.nHeight	= 1;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 4;
	tImage.pData	= (byte *)&dwColor;

	CreateTexture( pTex, "Sys_Grey" );
	pTex->Init( tImage, EAT_IMMUTABLE );
	m_pSysTextures[ EST_GREY ] = pTex;
	tImage.pData = NULL;

	dwColor	= 0xFF000000;
	tImage.nWidth	= 1;
	tImage.nHeight	= 1;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 4;
	tImage.pData	= (byte *)&dwColor;

	CreateTexture( pTex, "Sys_Black" );
	pTex->Init( tImage, EAT_IMMUTABLE );

	m_pSysTextures[ EST_BLACK ] = pTex;
	tImage.pData = NULL;


	m_pFrameCounter = g_pSystem->AddCounter( "Frame" );

	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
CRenderer_GL::~CRenderer_GL()
{
	//--------------------------------------------------------------------------
	// Tip: Del render targets
	//--------------------------------------------------------------------------
	DEBUG_ASSERT( 0 == m_RTList.GetNumItems() );
	
	for ( index_t i = INVALID_INDEX; m_RTList.Iterate( i ); )
	{
		IRenderTarget * pRT = m_RTList[ i ];
		DEL( pRT );
	}

	m_RTList.Delete();

	//--------------------------------------------------------------------------
	// Tip: Del shaders
	//--------------------------------------------------------------------------
	for ( index_t i = INVALID_INDEX; m_ShaderList.Iterate( i ); )
	{
		IShader * pShader = m_ShaderList[ i ];
		DEL( pShader );
	}

	m_ShaderList.Delete();

	//--------------------------------------------------------------------------
	// Tip: Удаление всех текстур
	//--------------------------------------------------------------------------
	for ( index_t i = INVALID_INDEX; m_TextureList.Iterate( i ); )
		m_TextureList[ i ] = NULL;
	
	m_TextureList.Delete();

	//--------------------------------------------------------------------------
	// Tip: Удаление всех VB
	//--------------------------------------------------------------------------
	DEBUG_ASSERT( 0 == m_VBList.GetNumItems() );

	for ( index_t i = INVALID_INDEX; m_VBList.Iterate( i ); )
		RELEASE( m_VBList[ i ] );
	
	m_VBList.Delete();

	//--------------------------------------------------------------------------
	// Tip: Удаление всех IB
	//--------------------------------------------------------------------------
	DEBUG_ASSERT( 0 == m_IBList.GetNumItems() );

	for ( index_t i = INVALID_INDEX; m_IBList.Iterate( i ); )
		RELEASE( m_IBList[ i ] );
	
	m_IBList.Delete();

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	m_ProjectionStack.Delete();
	m_ModelViewStack.Delete();
	m_TextureStack.Delete();

	//--------------------------------------------------------------------------
	// Tip: Del GL
	//--------------------------------------------------------------------------
	wglMakeCurrent( 0, 0 );
	ReleaseDC( m_hWindow, m_hDevCon );
	wglDeleteContext( m_hGLRenderCon );

	//DEL( g_pRenderer );
}


void CRenderer_GL::Release()
{
	g_pSystem->RemoveCounter( m_pFrameCounter );

	PURE_DEL( this );
}

////////////////////////////////////////////////////////////////////////////////

/*void CRenderer_GL::SetAttr( ERenderAttr eAttr, int nVal )
{
	switch ( eAttr )
	{
		case RA_ALPHA_TEST:
		{
			if ( nVal )
				glEnable( GL_ALPHA_TEST );
			else
				glDisable( GL_ALPHA_TEST );
		}
		break;

		case RA_DEPTH_TEST:
		{
			( nVal ) ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );
		}
		break;
	}
}*/