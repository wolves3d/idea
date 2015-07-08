////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Material.h"
//#include "MultiMaterial.h"
#include "../ResourceManager/ResourceManager.h"


/**
*
*/
CMaterial::CMaterial() :
	m_bAlphaBlend	( false ),
	m_bAlphaTest	( false ),
	m_bDepthTest	( true ),
	m_bDepthWrite	( true ),
	m_pShader		( NULL ),
	m_nUniformCount	( 0		),
	m_pUniformInfos	( NULL )
{
	m_pTexMaps		= NEW PTexture [ 1 ];
}


/**
*
*/
CMaterial::~CMaterial()
{
	const uint nSubCount = m_pSubMaterials.GetCount();

	if ( nSubCount )
	{
		for ( uint n = 0; n < nSubCount; ++n )
		{
			RELEASE( m_pSubMaterials[ n ] );
		}

		m_pSubMaterials.Delete();
	}

	for ( uint n = 0; n < 1; ++n )
	{
		PTexture pTex = m_pTexMaps[ 0 ];
		
		if ( pTex )
		{
			pTex->DecRefCount();

			if ( 0 == pTex->GetRefCount() )
			{
				RELEASE( pTex );
			}
		}
	}
}


/**
*
*/
void CMaterial::Release()
{
	PURE_DEL( this );
}


/**
* ”становка текстур / стейтов / шейдеров
*/
bool CMaterial::Assign()
{
	DEBUG_ASSERT( m_pShader );

	if ( NULL == m_pShader )
		return false;

	m_pShader->Assign();

	for ( uint n = 0; n < m_nUniformCount; ++n )
	{
		m_pShader->SetUniforms( n, m_pRegisters + m_pUniformInfos[ n ].nFirst );
	}

	//--------------------------------------------------------------------------
	// Tip: Maps setup
	//--------------------------------------------------------------------------
	g_pRenderer->SetSampler( DIFFUSE_MAP, m_pTexMaps[ 0 ] );

	if ( m_pTexMaps[ 0 ] )
	{
		m_pTexMaps[ 0 ]->Bind();
	}

	//--------------------------------------------------------------------------
	// Tip: Alpha blend setup
	//--------------------------------------------------------------------------
	if ( m_bAlphaBlend )
	{
		g_pRenderer->BlendFunc( m_eBlendSrc, m_eBlendDst );
		g_pRenderer->EnableBlend();
		//glAlphaFunc( GL_GREATER, 0.1f );
	}
	else
	{
		g_pRenderer->EnableBlend( false );
	}

	//--------------------------------------------------------------------------
	// Tip: Alpha test setup
	//--------------------------------------------------------------------------
	if ( m_bAlphaTest )
	{
		g_pRenderer->EnableAlphaTest();
		//glAlphaFunc( GL_GREATER, 0.1f );
	}
	else
	{
		g_pRenderer->EnableAlphaTest( false );
	}

	//--------------------------------------------------------------------------
	// Tip: Depth test setup
	//--------------------------------------------------------------------------
	if ( m_bDepthTest )
	{
		g_pRenderer->EnableDepthTest();
	}
	else
	{
		g_pRenderer->EnableDepthTest( false );
	}

	//--------------------------------------------------------------------------
	// Tip: Depth write setup
	//--------------------------------------------------------------------------
	g_pRenderer->DepthMask( m_bDepthWrite );

	return true;
}


/**
*
*/
void CMaterial::BlendFunc( EBlendFactor eSrc, EBlendFactor eDst )
{
	m_eBlendSrc = eSrc;
	m_eBlendDst = eDst;
}


/**
*
*/
void CMaterial::EnableAlphaBlend( bool bEnable )
{
	m_bAlphaBlend = bEnable;
}


/**
*
*/
void CMaterial::EnableAlphaTest( bool bEnable )
{
	m_bAlphaTest = bEnable;
}

/**
*
*/
void CMaterial::EnableDepthTest( bool bEnable )
{
	m_bDepthTest = bEnable;
}


/**
*
*/
void CMaterial::EnableDepthWrite( bool bEnable )
{
	m_bDepthWrite = bEnable;
}


/**
*
*/
PTexture CMaterial::GetTexture( ETexture eTex )
{
	return m_pTexMaps[ 0 ];
}


/**
*
*/
bool CMaterial::SetTexture( ETexture eTex, PTexture pTex )
{
	if ( m_pTexMaps[ 0 ] )
	{
		m_pTexMaps[ 0 ]->DecRefCount();
	}

	if ( !pTex )
	{
		pTex = g_pRenderer->GetSysTexture( EST_DEFAULT );
	}

	if ( !pTex )
	{
		// ≈сли выполнение попало сюда, то значит не существует
		// даже системной текстуры по умолчанию ( default )
		return false;
	}

	pTex->IncRefCount();

	m_pTexMaps[ 0 ] = pTex;
	return false;
}


/**
*
*/
IShader * CMaterial::GetShader()
{
	return m_pShader;
}


/**
*
*/
/*bool CMaterial::SetUniform( uint nUniform, const vec4 & vValue )
{
	if ( m_pUniforms )
	{
		if ( ( nUniform >= 0 ) && ( nUniform < m_nUniformCount ) )
		{
			m_pUniforms[ nUniform ] = vValue;
			return true;
		}
	}

	DEBUG_ASSERT( false && "Invalid uniform ID" );
	return false;
}*/


/**
*
*/
bool CMaterial::SetShader( IShader * pShader )
{
	// ћатериал не умеет переключать шейдеры?
	DEBUG_ASSERT( NULL == m_pShader );

	DEL_ARRAY( m_pUniformInfos );

	if ( pShader )
	{
		m_nUniformCount = pShader->GetUniformCount();

		if ( 2 == m_nUniformCount )
		{
			int a=  0;
		}

		if ( m_nUniformCount )
		{
			uint nOffset	= 0;
			m_pUniformInfos = NEW TUniformInfo [ m_nUniformCount ];

			for ( uint n = 0; n < m_nUniformCount; ++n )
			{
				TUniformInfo & tInfo = m_pUniformInfos[ n ];

				uint nRegCount	= pShader->GetUniformSize( n );
				tInfo.nFirst	= nOffset;
				tInfo.nSize		= nRegCount;

				nOffset += nRegCount;
			}
		}
	}
	
	m_pShader = pShader;
	return false;
}


/**
*
*/
void CMaterial::AddSubMaterial( IMaterial * pMaterial )
{
	if ( !pMaterial )
		pMaterial = g_pEngine->GetDefMaterial();

	m_pSubMaterials.AddToTail( pMaterial );
}


/**
*
*/
IMaterial *	CMaterial::GetSubMaterial( uint nSub )
{
	if ( nSub < GetSubCount() )
	{
		return m_pSubMaterials[ nSub ];
	}

	return NULL;
}


/**
*
*/
uint CMaterial::GetSubCount() const
{
	return m_pSubMaterials.GetCount();
}


/**
*
*/
IMaterial * CEngine::CreateMaterial()
{
	CMaterial * pMat = NEW CMaterial;

	return pMat;
}


/**
*
*/
IMaterial * CEngine::LoadMaterial( const char * szFileName )
{
	IMaterial * pMaterial = CreateMaterial();

	if ( pMaterial )
	{
		pMaterial->SetShader( g_pRenderer->GetShader( MESH_SHADER ) );

		PTexture tex;

		if ( LoadTexture( tex, szFileName ) )
		{
			pMaterial->SetTexture( DIFFUSE_MAP, tex );
		}
	}

	return pMaterial;

	/*
	IMaterial * pMaterial = NULL;

	IXMLWrapper * pWrapper = (IXMLWrapper *)g_pSystem->GetInterface( IT_UTIL );

	CXMLElement * pMatXML = NULL;
	pWrapper->LoadXML( szFileName, &pMatXML );

	if ( pMatXML )
	{
		pMaterial = CreateMaterial();

		CXMLAttr * pDiff = pMatXML->GetAttr( "DiffuseMap" );

		if ( pDiff )
		{
			PTexture tex;

			if ( LoadTexture( tex, pDiff->GetValue() ) )
			{
				pMaterial->SetTexture( DIFFUSE_MAP, tex );
			}
		}
	}

	DEL( pMatXML );
	return pMaterial;
	*/
}


/**
*
*/
bool CEngine::LoadTexture( PTexture & pOutTexture, const char * szFileName )
{
	if ( !szFileName )
		return false;

	//if ( R_OK == g_pRenderer->CreateTexture( pOutTexture, szFileName ) )
	{
		TResource * pTestRes = NULL;
		m_pResManager->Demand( szFileName, &pTestRes );
		
		pOutTexture = *( (PTexture *)pTestRes->pResource );

		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////