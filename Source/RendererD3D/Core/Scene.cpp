////////////////////////////////////////////////////////////////////////////////

#include "Common.h"


/**
*
*/
bool CRendererD3D::ClearScene()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 1.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	g_pDevice->ClearRenderTargetView( m_pFrameBufferRT, ClearColor );

	/*
	m_pDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xFF0000FF,
		1.0f,
		0 );

	m_pDevice->BeginScene();
*/
	return false;
}


/**
*
*/
bool CRendererD3D::Present()
{
	m_pSwapChain->Present( 0, 0 );
	
	return false;
}


//!

//!
//!
void CRendererD3D::SetMatrix( EMatrixMode eMode, const mat4 & matrix )
{
	/*
	mat4 m;
	//D3DXMatrixOrthoLH( (D3DXMATRIX *)&m, 800, 600, -1, 1 );
	D3DXMatrixOrthoOffCenterLH(
		(D3DXMATRIX *)&m,
		0,
		800,
		600,
		0,
		-1,
		1 );

	mat4 mmm = matrix;
	mmm.Mult( m );

	struct
	{
		mat4	mWorldViewProj;
		vec2	vOffset;
		vec2	vShit;
	} *cb0;

	//tMatrix.Connect()

	ID3D10Buffer * pCB;
	D3D10_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( D3D10_BUFFER_DESC ) );
	bd.Usage			= D3D10_USAGE_DYNAMIC;
	bd.ByteWidth		= sizeof( *cb0 );
	bd.BindFlags		= D3D10_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE ; // set 0 to setup data at alloc only
	bd.MiscFlags		= 0;
	g_pDevice->CreateBuffer( &bd, NULL, &pCB );

	pCB->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&cb0 );
	cb0->mWorldViewProj = mmm;
	pCB->Unmap();

	g_pDevice->VSSetConstantBuffers( 0, 1, &pCB );
*/
}

////////////////////////////////////////////////////////////////////////////////