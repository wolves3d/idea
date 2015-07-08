
#include "Common.h"
#include <stdlib.h> // for atoi()


/**
*
*/
IVertexDecl * ParseVertexFormat( CXMLElement * pBranch )
{
	IVertexDecl * pDecl = g_pRenderer->CreateVertexDecl();

	if ( !pDecl )
		return NULL;

	uint nOffset = 0;

	// Перебираем все дочерние элементы ----------------------------------------

	for ( size_t n = 0; n < pBranch->GetNumChilds(); ++n )
	{
		CXMLElement * pInputElem = pBranch->GetChild( n );
		CXMLAttr * pType = pInputElem->GetAttr( "Type" );
		CXMLAttr * pSem = pInputElem->GetAttr( "D3D_Semantic" );

		ETypeID eType = g_pSystem->GetType( pType->GetValue() );

		pDecl->AddAttr(
			pInputElem->GetName(),
			( NULL != pSem ) ? pSem->GetValue() : NULL,
			eType, nOffset );

		nOffset += g_pSystem->GetSizeOf( eType );
	}

	pDecl->SetStride( nOffset ); // Записывам шаг (размер) вершины в байтах 

	return pDecl;
}





/**
*
*/
uint ParseConstants( CXMLElement * pBranch, TUniform ** ppUniforms )
{
	const size_t nUniformCount = pBranch->GetNumAttrs();
	//pShader->m_nUniformCount = pBranch->GetNumAttrs();

	if ( 0 == nUniformCount )
		return 0;

	(*ppUniforms) = NEW TUniform [ nUniformCount ];

	if ( NULL == (*ppUniforms) )
		return 0;

	for ( size_t n = 0; n < nUniformCount; ++n )
	{
		const CXMLAttr * pAttr = pBranch->GetAttr( n );

		CStr sType( pAttr->GetValue() );
		const char * szArray = strstr( sType.GetString(), " [" );

		TUniform * pUnfm = (*ppUniforms) + n;

		if ( szArray )
		{
			CStr sCount = szArray + 2;			// skip " [" substring
			sCount.Cut( sCount.GetSize() - 1 );	// cutoff ending ']' symbol

			pointer nOffset = szArray - sType.GetString() + 1;
			sType.Cut( nOffset );				// cutoff [##] string

			pUnfm->nCount= atoi( sCount );
		}
		else
		{
			pUnfm->nCount = 1;
			/*
			switch ( pShader->m_pUniforms[ n ].eType )
			{
				case TYPE_VEC4:
					pShader->m_pUniforms[ n ].nCount = 1;
				break;

				case TYPE_MAT4:
					pShader->m_pUniforms[ n ].nCount = 4;
				break;

				default:
					DEBUG_ASSERT( !"unexpected type" );
					pShader->m_pUniforms[ n ].nCount = 0;
			}
			*/
		}

		pUnfm->sName	= pAttr->GetName();
		pUnfm->eType	= g_pSystem->GetType( sType.GetString() );

		//pUnfm->nLoc	= pShader->GetUniformLoc( pUnfm->sName );
	}

	return nUniformCount;
}




class CShaderCfgLoader : public IResCreator
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:

	const IResCreatorInfo * GetInfo () const
	{
		return &m_Info;
	}

	void CreateResInstance( TResource ** ppOutResource )
	{
		if ( NULL == ppOutResource )
			return;

		TResource * pRes = NEW TResource;

		if ( pRes )
		{
			PTexture * pTex = NEW PTexture;
			g_pRenderer->CreateTexture( (*pTex), "no tex name" );
			pRes->pResource = pTex;
		}

		(*ppOutResource) = pRes;
	}

	size_t InitRes( TResource * pRes, size_t nFileSize )
	{
		pRes->pBuffer = NEW byte[ nFileSize +1 ];

		if ( pRes->pBuffer )
		{
			pRes->nSize = nFileSize;
			pRes->pBuffer[ nFileSize ] = 0;
		}

		return 0;
	}

	size_t UpdateRes( TResource * pRes, size_t nBytesWritten )
	{
		return 0;
	}

	void FinalizeRes( TResource * pRes );

	static IResCreatorInfo m_Info;
};




void CShaderCfgLoader::FinalizeRes( TResource * pRes )
{
	pRes->eState = TResource::FAILED;

	//--------------------------------------------------------------------------
	// Tip: Check XMLWrapper
	//--------------------------------------------------------------------------
	IXMLWrapper * pWrapper = (IXMLWrapper *)g_pSystem->GetInterface( IT_UTIL );

	if ( !pWrapper )
		return;

	CXMLElement * pRoot = NULL;
	pWrapper->LoadXMLFromMem( (char *)pRes->pBuffer, pRes->nSize, &pRoot );
	DEL_ARRAY( pRes->pBuffer );
	//pRes->pResource = pRoot;

	//--------------------------------------------------------------------------
	// Tip: Check root element name
	//--------------------------------------------------------------------------
	CStr sName( pRoot->GetName() );

	if ( !sName.Compare( "Shaders" ) )
	{
		DEL( pRoot );
		return;
	}

	//--------------------------------------------------------------------------
	// Tip: Parsing shaders
	//--------------------------------------------------------------------------
	for ( size_t i = 0; i < pRoot->GetNumChilds(); ++i )
	{
		//----------------------------------------------------------------------
		// Tip: New shader
		//----------------------------------------------------------------------
		CXMLElement * pShaderBranch = pRoot->GetChild( i );
		CStr sPath( "Shaders/" );
		sPath.Add( pShaderBranch->GetName() );

//		const CStr		sShader( pShaderBranch->GetName() );
//		CBaseShader	*	pShader	= NEW CBaseShader( sShader.GetString() );
//		CXMLAttr *		pProgPath = pShaderBranch->GetAttr( "ProgramPath" );
//		CStr			sProgPath( pProgPath->GetValue() );

		SysWrite( Va( "\tshader %s", pShaderBranch->GetName() ) );

		IVertexDecl *	pDecl = NULL;
		TUniform	*	pUniforms = NULL;
		uint			nUnfmCount = 0;

		//----------------------------------------------------------------------
		// Tip: Parse shader data
		//----------------------------------------------------------------------
		for ( size_t j = 0; j < pShaderBranch->GetNumChilds(); ++j )
		{
			CXMLElement *	pStruct = pShaderBranch->GetChild( j );
			const CStr		sStruct( pStruct->GetName() );

			if ( sStruct.Compare( "VertexFormat" ) )
			{
				pDecl = ParseVertexFormat( pStruct );
			}
			else if ( sStruct.Compare( "Constants" ) )
			{
/*
				pShader->m_nUniformCount	= 1;
				pShader->m_pUniforms		= NEW CBaseShader::TUniform [ pShader->m_nUniformCount ];

				pShader->m_pUniforms[ 0 ].sName	= "vColorMod";
				pShader->m_pUniforms[ 0 ].eType	= TYPE_VEC4;
				pShader->m_pUniforms[ 0 ].nLoc	= pShader->GetUniformLoc( pShader->m_pUniforms[ 0 ].sName );
				pShader->m_pUniforms[ 0 ].nCount	= 1;

				m_ppShaders[ UI_SHADER ] = pShader;
*/
				nUnfmCount = ParseConstants( pStruct, &pUniforms );
				//pShader->ParseMaps( pStruct );
			}
		}

		g_pRenderer->CreateShader( sPath.GetString(), pDecl, pUniforms, nUnfmCount );
	}

	DEL( pRoot );

	pRes->eState = TResource::LOADED;
}




IResCreatorInfo CShaderCfgLoader::m_Info =
{
	"My shader config loader :)",
	".xml"
};





void RegisterShaderCfgLoader()
{
	g_pResManager->RegCreator( NEW CShaderCfgLoader );
}