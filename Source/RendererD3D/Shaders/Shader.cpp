////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Shader.h"
#include "ShaderLoader.h"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//!

//! Перезагружает все шейдеры
//!
bool CRendererD3D::ReloadShaders()
{
	g_pResManager->RegCreator( NEW CShaderCreator );

	TResource * pShaderConfig = NULL;
	g_pResManager->Request( "Shaders.xml", &pShaderConfig );

	
	m_ppShaders[ UI_SHADER ] = GetShader( "UI" );

	return true;

}


//!

//!
//!
IShader	* CRendererD3D::GetShader( const char * szShader )
{
	if ( szShader )
	{
		for ( size_t i = 0; i < m_ShaderList.GetNumItems(); ++i )
		{
			CShader * pShader = static_cast<CShader *>( m_ShaderList[ i ] );

			if ( pShader->m_sName.Compare( szShader ) )
				return pShader;
		}
	}

	return NULL; // set default shader
}


//!

//!
//!
IShader	* CRendererD3D::GetShader( EShaderType eShaderType )
{
	return m_ppShaders[ eShaderType ];
}






bool CRendererD3D::CreateShader( const char * szPath, IVertexDecl * pDecl, TUniform * pUnfms, uint nUnfmCount )
{
		TResource * pShaderRes  = NULL;

	g_pResManager->Request( Va( "%s/hlsl.vsh", szPath ), &pShaderRes );
	g_pResManager->Request( Va( "%s/hlsl.psh", szPath ), &pShaderRes );

	uint			nConstBufferSize = 96;

	CShader * pShader = static_cast<CShader *>( pShaderRes->pResource );



	// Tip: Create our vertex input layout ---------------------------------

	const D3D10_INPUT_ELEMENT_DESC layout_ui[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D10_INPUT_ELEMENT_DESC layout_line[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	/*
	D3D10_INPUT_ELEMENT_DESC * pLayout = NEW D3D10_INPUT_ELEMENT_DESC [ pDecl->GetAttrCount() ];

	for ( uint n = 0; n < pDecl->GetAttrCount(); ++n )
	{
		TVertexAttr * pAttr = pDecl->GetAttr();
		pLayout[ n ].SemanticName = pAttr->szName
	}*/

	const D3D10_INPUT_ELEMENT_DESC * pLay = NULL;
	uint nCount = 0;

	if ( CStr::FinishWith( szPath, "UI" ) )
	{
		pLay = layout_ui;
		nCount = 2;
	}
	else if ( CStr::FinishWith( szPath, "Line" ) )
	{
		pLay = layout_line;
		nCount = 2;
	}

	if ( pLay )
	{
		HRESULT hRes = g_pDevice->CreateInputLayout(
			pLay,
			nCount,
			pShader->m_pByteCodeVS->GetBufferPointer(),
			pShader->m_pByteCodeVS->GetBufferSize(),
			&pShader->m_pLayout );

		if ( FAILED( hRes ) )
		{
			return false; // fixme
		}
	}

	g_pDevice->IASetInputLayout	( pShader->m_pLayout );

	pShader->m_pVDecl = pDecl;

	//----------------------------------------------------------------


	g_pDevice->VSSetShader		( pShader->m_pVS );
	g_pDevice->GSSetShader		( NULL );
	g_pDevice->PSSetShader		( pShader->m_pPS );

	pShader->m_ppConstBuffers = NEW ID3D10Buffer * [ 1 ];

	D3D10_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( D3D10_BUFFER_DESC ) );
	bd.Usage			= D3D10_USAGE_DYNAMIC;
	bd.ByteWidth		= nConstBufferSize;
	bd.BindFlags		= D3D10_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE ; // set 0 to setup data at alloc only
	bd.MiscFlags		= 0;
	g_pDevice->CreateBuffer( &bd, NULL, pShader->m_ppConstBuffers );

	g_pDevice->VSSetConstantBuffers( 0, 1, pShader->m_ppConstBuffers );

	//--------------------------------

	pShader->m_sName.Set( szPath + CStr::GetSize( "Shaders" ) );
	m_ShaderList.Add( pShader );

	return true;
}

////////////////////////////////////////////////////////////////////////////////