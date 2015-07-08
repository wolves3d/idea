////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Shader.h"
#include "ShaderLoader.h"

////////////////////////////////////////////////////////////////////////////////

IResCreatorInfo CShaderCreator::m_Info =
{
	"My first D3D10 shader loader :)",
	"sh"
};

////////////////////////////////////////////////////////////////////////////////

EResult CompileShader(
	const char *	szFileName,
	const char *	szSource,
	const char *	szEntryPoint,
	const char *	szProfile,
	ID3D10Blob **	ppShaderByteCode )
{
	if ( ( NULL == szFileName ) || ( NULL == szSource ) || ( NULL == ppShaderByteCode ) )
		return R_INVALID_ARG;

	size_t nSize = strlen( szSource );

	if ( 0 == nSize )
		return R_INVALID_ARG;

	ID3D10Blob * pErrorMsg = NULL;

	HRESULT hRes = D3D10CompileShader(
		szSource,			// file data
		nSize,				// file data size
		NULL,				// file name (set to null if prev args not null)
		NULL,				// macro array (defines)
		NULL,				// ? include
		szEntryPoint,
		szProfile,
		0,					// shader compile flags
		ppShaderByteCode,	
		&pErrorMsg );

	if ( FAILED( hRes ) )
	{
		if ( NULL != pErrorMsg )
		{
			const char * szErr = Va(
				"\tShader compilation failed, file \"%s\"\n\t\t%s",
				szFileName, (const char *)pErrorMsg->GetBufferPointer() );

			SysWrite( szErr );
			DEBUG_MSG( szErr );
		}

		return R_GENERIC_ERROR;
	}

	return R_OK;
}


const IResCreatorInfo * CShaderCreator::GetInfo () const
{
	return &m_Info;
}




void CShaderCreator::CreateResInstance( TResource ** ppOutResource )
{
	if ( NULL == ppOutResource )
		return;

	TResource * pRes = NEW TResource;

	if ( pRes )
	{
		CShader * pShader = NEW CShader;
		pRes->pResource = pShader;
	}

	(*ppOutResource) = pRes;
}




size_t CShaderCreator::InitRes( TResource * pRes, size_t nFileSize )
{
	pRes->nReadySize = 0;
	pRes->pBuffer = NEW byte[ nFileSize ];

	if ( pRes->pBuffer )
	{
		pRes->nSize = nFileSize;
	}

	return 0;
}




size_t CShaderCreator::UpdateRes( TResource * pRes, size_t nBytesWritten )
{
	/*
	CXMLElement * pShdFile;
	g_pXMLWrapper->LoadXML( (const char*)pRes->pBuffer, &pShdFile );
	*/

	return 0;
}




void CShaderCreator::FinalizeRes( TResource * pRes )
{
	// Компилируем шейдер --------------------------------------------------

	ID3D10Blob *	pByteCode = NULL;
	CShader *		pShader = (CShader *)pRes->pResource;
	uint			nConstBufferSize;

	const char * szEntry = NULL;
	const char * szProfile = NULL;
	bool bIsVertexShader = true;

	if ( CStr::FinishWith( pRes->sFileName.GetString(), ".vsh" ) )
	{
		szEntry = "Ripple";
		szProfile = "vs_4_0";
	}
	else if ( CStr::FinishWith( pRes->sFileName.GetString(), ".psh" ) )
	{
		szEntry = "main";
		szProfile = "ps_4_0";
		bIsVertexShader = false;
	}

	EResult rStatus = CompileShader(
		pRes->sFileName.GetString(),
		(const char *)pRes->pBuffer,
		szEntry,
		szProfile,
		&pByteCode );

	// После компиляции исходный код уже не нужен
	DEL_ARRAY( pRes->pBuffer );

	if ( R_OK != rStatus )
	{
		// Компиляция провалилась
		pRes->eState = TResource::FAILED;
		return;
	}

	ID3D10ShaderReflection * pRef = NULL;
	HRESULT hRes;

	if ( bIsVertexShader )
	{
		pShader->m_pByteCodeVS = pByteCode;
		// Обрабатываем переменные ---------------------------------------------

		hRes = D3D10ReflectShader(
			pByteCode->GetBufferPointer(),
			pByteCode->GetBufferSize(),
			&pRef );

		if ( ( S_OK == hRes ) && ( NULL != pRef ) )
		{
			D3D10_SHADER_DESC desc;
			pRef->GetDesc( &desc );

			DEBUG_ASSERT( 1 == desc.ConstantBuffers )

				// Перебираем каждый буффер констант
				for ( uint i = 0; i < desc.ConstantBuffers; ++i )
				{
					ID3D10ShaderReflectionConstantBuffer * pCBuf =
						pRef->GetConstantBufferByIndex( i );

					D3D10_SHADER_BUFFER_DESC bufDesc;
					pCBuf->GetDesc( &bufDesc );

					const char * szCBName	= bufDesc.Name;
					nConstBufferSize		= bufDesc.Size;

					pShader->m_nUniformCount = bufDesc.Variables;
					pShader->m_pUniforms = NEW CShader::TUniform [ pShader->m_nUniformCount ];

					// Перебираем каждую переменную в буффере
					for ( uint nVar = 0; nVar < bufDesc.Variables; ++nVar )
					{
						ID3D10ShaderReflectionVariable * pVar =
							pCBuf->GetVariableByIndex( nVar );

						D3D10_SHADER_VARIABLE_DESC varDesc;
						pVar->GetDesc( &varDesc );

						CShader::TUniform & tUniform = pShader->m_pUniforms[ nVar ];

						tUniform.sName	= varDesc.Name;
						tUniform.nCount = varDesc.Size / sizeof( vec4 );
						tUniform.nLoc	= varDesc.StartOffset / sizeof( vec4 );
					}
				}
		}

		hRes = g_pDevice->CreateVertexShader(
			pByteCode->GetBufferPointer(),
			pByteCode->GetBufferSize(),
			&pShader->m_pVS );
	}

	//--------------------------------------------------------------------------

	if ( !bIsVertexShader )
	{
		hRes = g_pDevice->CreatePixelShader(
			pByteCode->GetBufferPointer(),
			pByteCode->GetBufferSize(),
			&pShader->m_pPS );

		RELEASE( pByteCode );
	}

	///

	if ( FAILED( hRes ) )
	{
		return;
	}

	pRes->eState = TResource::LOADED;
}