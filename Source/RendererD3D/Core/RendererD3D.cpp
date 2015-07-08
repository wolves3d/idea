////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DXGI.h"

////////////////////////////////////////////////////////////////////////////////

#pragma comment ( lib, "d3d10.lib" )
#pragma comment ( lib, "dxgi.lib" )

////////////////////////////////////////////////////////////////////////////////

ID3D10Device *	g_pDevice		= NULL;
ID3D10Debug *	g_pDeviceDbg	= NULL;

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CRendererD3D::ms_ClassDesc = 
{
	8084,
	IT_RENDERER,
	1,
	"D3D Renderer by WW v0.1"
};


//!

//!
//!
const TIntfDesc * CRendererD3D::GetInterfaceDesc() const
{
	return &ms_ClassDesc;
}


//!

//!
//!
CRendererD3D::CRendererD3D() :
	m_pSwapChain	( NULL ),
	m_pFrameBufferRT( NULL )
{
	ZeroMemory( m_ppShaders, SHADER_COUNT * sizeof(IShader *) );
}


//!

//!
//!
EResult CRendererD3D::Init( HWND hWindow )
{
	g_pVfs = (IVfs *) g_pSystem->GetInterface( IT_VFS );
	g_pResManager = (IResManager *) g_pSystem->GetInterface( IT_RES_MGR );
	g_pXMLWrapper = (IXMLWrapper *) g_pSystem->GetInterface( IT_UTIL );

	////////////////////////////////////////////////////////////////////////////

	IDXGIFactory * pDXGIFactory( NULL );
	HRESULT hRes = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&pDXGIFactory );

	// Search for a PerfHUD adapter.  
	UINT nAdapter = 0;
	IDXGIAdapter* adapter = NULL;
	IDXGIAdapter* selectedAdapter = NULL;
	D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_HARDWARE;

	while ( pDXGIFactory->EnumAdapters( nAdapter, &adapter ) != DXGI_ERROR_NOT_FOUND )
	{
		if (adapter)
		{
			DXGI_ADAPTER_DESC adaptDesc;
			if (SUCCEEDED(adapter->GetDesc(&adaptDesc)))
			{
				const bool isPerfHUD = wcscmp(adaptDesc.Description, L"NVIDIA PerfHUD") == 0;

				// Select the first adapter in normal circumstances or the PerfHUD one if it exists.
				if ( nAdapter == 0 || isPerfHUD )
					selectedAdapter = adapter;

				if(isPerfHUD)
					driverType = D3D10_DRIVER_TYPE_REFERENCE;

				++nAdapter;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWindow;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hRes = D3D10CreateDeviceAndSwapChain(
		selectedAdapter,	//NULL,
		driverType,	//D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0, // D3D10_CREATE_DEVICE_DEBUG,
		D3D10_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&g_pDevice );

	if ( FAILED( hRes ) )
	{
		DEBUG_MSG( "CreateDevice failed" );
		return R_GENERIC_ERROR;
	}

	hRes = g_pDevice->QueryInterface( __uuidof( ID3D10Debug ), (void **)&g_pDeviceDbg );


	// Create a render target view
	ID3D10Texture2D * pBackBuffer;
	hRes = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID *)&pBackBuffer );
	hRes = g_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pFrameBufferRT );
	pBackBuffer->Release();

	g_pDevice->OMSetRenderTargets( 1, &m_pFrameBufferRT, NULL );

	////////////////////////////////////////////////////////////////////////////

	// Setup the viewport
	D3D10_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pDevice->RSSetViewports( 1, &vp );

	////////////////////////////////////////////////////////////////////////////

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

	/*dwColor	= 0xFFD4D0C8;
	tImage.nWidth	= 1;
	tImage.nHeight	= 1;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 4;
	tImage.pData	= (byte *)&dwColor;*/

	dwColor	= 0xFFD4D0C8;
	tImage.nWidth	= 512;
	tImage.nHeight	= 512;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 512 * 512 * 4;
	tImage.pData	= NEW byte [ tImage.nSize ];

	for ( uint n = 0; n < 512*512*4; ++n )
	{
		tImage.pData[ n ] = rand();
	}

	CreateTexture( pTex, "Sys_Grey" );
	pTex->Init( tImage, EAT_IMMUTABLE );
	m_pSysTextures[ EST_GREY ] = pTex;
	tImage.pData = NULL;
/*
	dwColor	= 0xFF000000;
	tImage.nWidth	= 1;
	tImage.nHeight	= 1;
	tImage.eFormat	= TImage::IMG_FMT_RGBA8;
	tImage.nSize	= 4;
	tImage.pData	= (byte *)&dwColor;

	CreateTexture( pTex, tImage, "Sys_Black" );

	m_pSysTextures[ EST_BLACK ] = pTex;
	tImage.pData = NULL;
*/
	////////////////////////////////////////////////////////////////////////////

	ReloadShaders();

	return R_GENERIC_ERROR;
}


/**
*
*/
void CRendererD3D::Release()
{
	RELEASE( g_pDevice );
	//RELEASE( m_pD3D ); 
}

////////////////////////////////////////////////////////////////////////////////