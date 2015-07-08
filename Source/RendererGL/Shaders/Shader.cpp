////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Shader.h"

const char * g_pAttrNames[ IShader::TOTAL_ATTRS ];

/**
*
*/
CShader::CShader( const char *szName ) :
	m_sName		( szName	),
	m_hProgram	( NULL		),
	//m_bAlphaTest( false		),
	m_bUseVertexProgram( false ),
	m_bUseFragmentProgram( false )

{
	for ( int i = 0; i < TOTAL_ATTRS; ++i )
		g_pAttrNames[ i ] = NULL;

	g_pAttrNames[ COLOR_MOD ] = "vColorMod";

	m_tData.m_pAttrs[ ENABLE_ALPHA_TEST ].nIndex = GL_ALPHA_TEST;
}


/**
*
*/
bool CShader::SetVertexProgram( const char * szSource )
{
	if  ( !szSource )
		return false;

	m_hProgram = glCreateProgramObjectARB();

	GLint dwSize = (GLint)strlen( szSource );

	GLhandleARB hShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );

	glShaderSourceARB( hShader, 1, &szSource, &dwSize );
	glCompileShaderARB( hShader );

	// check if shader compiled
	GLint compiled = 0;

	glGetObjectParameterivARB( hShader,
		GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

	if ( !compiled )
	{
		int maxLength = 0;
		glGetObjectParameterivARB( hShader,
			GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength );

		char *infoLog = NEW char[ maxLength ];
		glGetInfoLogARB( hShader, maxLength, &maxLength, infoLog );
		MessageBox( NULL, infoLog, "vertex shader", MB_OK );
		DEL_ARRAY( infoLog );
		return false;
	}

	glAttachObjectARB( m_hProgram, hShader );
	glDeleteObjectARB( hShader );

	//glBindAttribLocationARB( m_hProgram, 1, "vPos" );
	//glBindAttribLocationARB( m_hProgram, 2, "vTex" );
	//glUniform3fARB(getUniLoc(brickProg, "BrickColor"), 1.0, 0.3, 0.2);

	glLinkProgramARB( m_hProgram );

	GLint linked = false;
	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &linked);

	if ( !linked )
		__asm { int 3 }//WARNING( "not linked" );

	glValidateProgramARB( m_hProgram );

	GLint validated = false;

	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);

	if (!validated)
		__asm { int 3 }//WARNING( "not validated" );

	m_bUseVertexProgram = true;

	for ( int i = COLOR_MOD; i < TOTAL_ATTRS; ++i )
	{
		if ( g_pAttrNames[ i ] )
		{
			m_tData.m_pAttrs[ i ].nIndex = glGetUniformLocationARB( m_hProgram, g_pAttrNames[ i ] );
		}
	}

	return true;
}


/**
*
*/
bool CShader::SetFragmentProgram( const char * szSource )
{
	if  ( !szSource )
		return false;

	GLint dwSize = (GLint)strlen( szSource );

	GLhandleARB hShader = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );

	glShaderSourceARB( hShader, 1, &szSource, &dwSize );
	glCompileShaderARB( hShader );

	// check if shader compiled
	GLint compiled = 0;

	glGetObjectParameterivARB( hShader,
		GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

	if ( !compiled )
	{
		__asm { int 3 }//WARNING( "not linked" );

		int maxLength = 0;
		glGetObjectParameterivARB( hShader,
			GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength );

		char *infoLog = NEW char[ maxLength ];
		glGetInfoLogARB( hShader, maxLength, &maxLength, infoLog );
		MessageBox( NULL, infoLog, "shader", MB_OK );
		DEL_ARRAY( infoLog );
		return false;
	}

	glAttachObjectARB( m_hProgram, hShader );
	glDeleteObjectARB( hShader );

	//glBindAttribLocationARB( m_hProgram, 1, "vPos" );
	//glBindAttribLocationARB( m_hProgram, 2, "vTex" );
	//glUniform3fARB(getUniLoc(brickProg, "BrickColor"), 1.0, 0.3, 0.2);

	glLinkProgramARB( m_hProgram );

	GLint linked = false;
	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &linked);

	if ( !linked )
	{
		DEBUG_ASSERT( "!Not lnked" );
	}

	glValidateProgramARB( m_hProgram );

	GLint validated = false;

	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);

	if (!validated)
	{
		DEBUG_ASSERT( "!Not validated" );
	}

	return true;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CRenderer_GL::CreateShader( index_t * pOutIndex )
{
	return R_GENERIC_ERROR;
	/*
	CShader * pShader = NEW CShader;

	if ( !pShader )
		return R_OUT_OF_MEMORY;

	index_t idShader = m_ShaderList.Add( pShader );

	if ( INVALID_INDEX == idShader )
	{
		DEL( pShader );
		return R_OUT_OF_MEMORY;
	}

	(*pOutIndex) = idShader;
	return R_OK;
	*/
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
IShader	* CRenderer_GL::GetShader( const char *szShader )
{
	if ( szShader )
	{
		for ( size_t i = 0; i < m_ShaderList.GetNumItems(); ++i )
		{
			CShader * pShader = (CShader *)m_ShaderList[ i ];

			if ( pShader->m_sName.Compare( szShader ) )
				return pShader;
		}
	}
	else
	{
		return m_pActiveShader;
	}


	return NULL; // set default shader
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult	CRenderer_GL::SetShader( IShader * pShader )
{
	m_pActiveShader = pShader;
	return R_OK;
}

////////////////////////////////////////////////////////////////////////////////