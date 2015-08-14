////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "BaseShader.h"
#include "_Shared/API/IResManager.h"


/**
*
*/
bool CRenderer_GL::CreateShader( const char * szPath, IVertexDecl * pDecl,
	TUniform * pUnfms, uint nUnfmCount )
{
	CBaseShader * pShader = NEW CBaseShader( szPath + CStr::GetSize( "Shaders" ) );
	// LoadShader( CBaseShader * pShader, const char * szPath )

	if ( NULL == szPath )
		return false;

	bool	bUseGLSL	= true;
	bool	bSuccess	= false;
	char *	pVP			= NULL;
	char *	pFP			= NULL;

	bUseGLSL &= g_pVfs->IsFileExist( Va( "%s/VertexProgram.glsl", szPath ) );
	bUseGLSL &= g_pVfs->IsFileExist( Va( "%s/FragmentProgram.glsl", szPath ) );

	if ( bUseGLSL )
	{
		pVP = g_pVfs->LoadToMem( Va( "%s/VertexProgram.glsl", szPath ), VFS_READ | VFS_BINARY );
		pFP = g_pVfs->LoadToMem( Va( "%s/FragmentProgram.glsl", szPath ), VFS_READ | VFS_BINARY );
	}
	else
	{
		pVP = g_pVfs->LoadToMem( Va( "%s/VertexProgram.arb", szPath ), VFS_READ | VFS_BINARY );
		pFP = g_pVfs->LoadToMem( Va( "%s/FragmentProgram.arb", szPath ), VFS_READ | VFS_BINARY );
	}

	if ( ( NULL != pVP ) && ( NULL != pFP ) )
	{
		bSuccess = pShader->SetVertexProgram( pVP, NULL, bUseGLSL );
		bSuccess &= pShader->SetFragmentProgram( pFP, bUseGLSL );
	}

	DEL_ARRAY( pVP );
	DEL_ARRAY( pFP );

	pShader->m_pUniforms			= pUnfms;
	pShader->m_nUniformCount		= nUnfmCount;
	pShader->m_pVertexDecl		= pDecl;

	pShader->Assign(); // Needed by ATI/AMD cards for uniform location

	for ( uint n = 0; n < nUnfmCount; ++n )
	{
		pUnfms[ n ].nLoc = pShader->GetUniformLoc( pUnfms[ n ].sName );
	}

	if ( pShader->IsValid() )
	{
		SysWrite(Va("\t%s\n", szPath));
		m_ShaderList.Add( pShader );
		return bSuccess;
	}

	DEL( pShader );
	return false;
}


/**
*
*/
void CRenderer_GL::ReloadShaders()
{
	IResManager * pResManager = static_cast<IResManager *>( g_pSystem->GetInterface( IT_RES_MGR ) );

	SysWrite( "Reloading shaders..." );

	TResource * pRes = NULL;
	pResManager->Request( "shaders.xml",  &pRes );

	//////////////////////////////////////////////////////////////////////////

	m_ppShaders[ UI_SHADER ] = g_pRenderer->GetShader( "UI" );
	m_ppShaders[ MESH_SHADER ] = NULL;//g_pRenderer->GetShader( "Mesh" );
	m_ppShaders[ LAND_SHADER ] = g_pRenderer->GetShader( "Land" );
	//m_ppShaders[ LINE_SHADER ] = g_pRenderer->GetShader( "Line" );

	SysWrite( "Shaders reloaded" );
}

////////////////////////////////////////////////////////////////////////////////