////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "BaseShader.h"


/**
*
*/
CBaseShader::CBaseShader( const char * szName ) :
	m_pVertexDecl		( NULL	),
	m_nUniformCount		( 0		),
	m_pUniforms			( NULL	),
	m_hProgram			( 0		),
	m_hVertexProgram	( 0		),
	m_hFragmentProgram	( 0		)
{
	m_sName = szName;

	/*for ( uint n = 0; n < SA_MAX_ABILITIES; ++n )
	{
		TUniformID & uniform = m_pConstIndices[ n ];

		uniform.nArrayID	= INVALID_INDEX;
		uniform.nLocationGL	= INVALID_INDEX;
	}*/
}


/**
*
*/
CBaseShader::~CBaseShader()
{
	DEL( m_pVertexDecl );
	DEL( m_pUniforms );

	//glDeleteProgramsARB( 1, &m_hVertexProgram );
	//glDeleteProgramsARB( 1, &m_hFragmentProgram );
}


/**
*
*/
bool CBaseShader::Assign()
{
	//SysWrite( Va( "Assign vertex shader %d", m_hVertexProgram ) );
	//SysWrite( Va( "Assign fragment shader %d", m_hFragmentProgram ) );

	if ( m_hProgram ) // GLSL version
	{
		g_pRenderer->SetCurrentProgram( m_hProgram );

		glUseProgramObjectARB( m_hProgram );
		GL_VALIDATE;
	}
	else
	{
		if ( 0 == m_hVertexProgram )
			return false;

		if ( g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
		{
			glEnable( GL_VERTEX_PROGRAM_ARB );
			GL_VALIDATE;

			glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_hVertexProgram );
			GL_VALIDATE;

			glEnable( GL_FRAGMENT_PROGRAM_ARB );
			GL_VALIDATE;

			glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_hFragmentProgram );
			GL_VALIDATE;
		}
	}

	return true;
}


/**
*
*/
bool CBaseShader::SetVertexProgram( const char * szSource, IVertexDecl * pDecl, bool bGLSL )
{
	if  ( !szSource )
		return false;

	if ( bGLSL )
	{
		m_hProgram = glCreateProgramObjectARB();

		GLint dwSize = strlen( szSource );
		GLhandleARB hShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
		glShaderSourceARB( hShader, 1, &szSource, &dwSize );
		glCompileShaderARB( hShader );

		GLint compiled = 0;
		glGetObjectParameterivARB( hShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

		if ( !compiled )
		{
			int maxLength = 0;
			glGetObjectParameterivARB( hShader,
				GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength );

			char *infoLog = NEW char[ maxLength ];
			glGetInfoLogARB( hShader, maxLength, &maxLength, infoLog );

			MessageBox( NULL, infoLog, "Vertex program compilation error", MB_ICONEXCLAMATION );
			DEL_ARRAY( infoLog );
			return false;
		}

		glAttachObjectARB( m_hProgram, hShader );
		GL_VALIDATE;
		glDeleteObjectARB( hShader );

		glLinkProgramARB( m_hProgram );
		glGetObjectParameterivARB( m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &compiled );
		DEBUG_ASSERT( compiled );

		glValidateProgramARB( m_hProgram );
		glGetObjectParameterivARB( m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &compiled );
		DEBUG_ASSERT( compiled );
	}
	else
	{
		if ( !g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
			return false;

		glEnable( GL_VERTEX_PROGRAM_ARB );
		GL_VALIDATE;

		glGenProgramsARB( 1, &m_hVertexProgram );
		GL_VALIDATE;

		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_hVertexProgram );
		GL_VALIDATE;

		glProgramStringARB(
			GL_VERTEX_PROGRAM_ARB,
			GL_PROGRAM_FORMAT_ASCII_ARB,
			strlen( szSource ),
			szSource );
		//GL_VALIDATE;

		const GLubyte * szError = glGetString( GL_PROGRAM_ERROR_STRING_ARB );

		if ( strlen( (const char *)szError ) )
			MessageBox( 0, (const char *)szError, "vs_error", 0 );

		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );
		glDisable( GL_VERTEX_PROGRAM_ARB );
	}


//	if(error.getLength() || !shaderID)
//		return Logger::writeErrorLog(error);


	//m_hProgram = glCreateProgramObjectARB();
	//GLint dwSize = (GLint)strlen( szSource );
	//GLhandleARB hShader = glCreateShaderObjectARB( GL_VERTEX_PROGRAM_ARB );
	//glShaderSourceARB( hShader, 1, &szSource, &dwSize );
	//glCompileShaderARB( hShader );

	/*

	glGetObjectParameterivARB( hShader,
		GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

	if ( !compiled )
	{
		int maxLength = 0;
		glGetObjectParameterivARB( hShader,
			GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength );

		char *infoLog = NEW char[ maxLength ];
		glGetInfoLogARB( hShader, maxLength, &maxLength, infoLog );

		MessageBox( NULL, infoLog, "Vertex program compilation error", MB_ICONEXCLAMATION );
		DEL_ARRAY( infoLog );
		return false;
	}

	glAttachObjectARB( m_hProgram, hShader );
	glDeleteObjectARB( hShader );
*
	for ( uint n = 0; n < pDecl->GetAttrCount(); ++n )
	{
		const TVertexAttr * pAttr = pDecl->GetAttr( n );
		glBindAttribLocationARB( m_hProgram, n, pAttr->szName );
	}
/*
	glLinkProgramARB( m_hProgram );

	GLint linked = false;
	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &linked);

	if ( !linked )
	{
		__asm { int 3 }//WARNING( "not linked" );
	}

	glValidateProgramARB( m_hProgram );

	GLint validated = false;

	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);

	if (!validated)
	{
		__debugbreak();
		__asm { int 3 }//WARNING( "not validated" );
	}
*/

	return true;
}


/**
*
*/
bool CBaseShader::SetFragmentProgram( const char * szSource, bool bGLSL )
{
	if ( NULL == szSource )
		return false;

	if ( bGLSL )
	{
		GLhandleARB hShader = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
		GLint dwSize = strlen( szSource );

		glShaderSourceARB( hShader, 1, &szSource, &dwSize );
		glCompileShaderARB( hShader );

		GLint compiled = 0;
		glGetObjectParameterivARB( hShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

		if (false == compiled)
		{
			int maxLength = 0;
			glGetObjectParameterivARB(hShader,
				GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);

			char *infoLog = NEW char[maxLength];
			glGetInfoLogARB(hShader, maxLength, &maxLength, infoLog);

			MessageBox(NULL, infoLog, "Fragment program compilation error", MB_ICONEXCLAMATION);
			DEL_ARRAY(infoLog);
			return false;
		}

		glAttachObjectARB( m_hProgram, hShader );
		glDeleteObjectARB( hShader );

		glLinkProgramARB( m_hProgram );
		glGetObjectParameterivARB( m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &compiled );
		DEBUG_ASSERT( compiled );

		glValidateProgramARB( m_hProgram );
		glGetObjectParameterivARB( m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &compiled );
		DEBUG_ASSERT( compiled );
	}
	else
	{
		if ( !g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
			return false;

		glEnable( GL_FRAGMENT_PROGRAM_ARB );
		glGenProgramsARB( 1, &m_hFragmentProgram );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_hFragmentProgram );
		glProgramStringARB(
			GL_FRAGMENT_PROGRAM_ARB,
			GL_PROGRAM_FORMAT_ASCII_ARB,
			strlen( szSource ),
			szSource );

		const GLubyte * szError = glGetString( GL_PROGRAM_ERROR_STRING_ARB );

		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, 0 );
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
	}

	return true;
/*
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
		int maxLength = 0;
		glGetObjectParameterivARB( hShader,
			GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength );

		char *infoLog = NEW char[ maxLength ];
		glGetInfoLogARB( hShader, maxLength, &maxLength, infoLog );
		MessageBox( NULL, infoLog, "Fragment program compilation error", MB_ICONEXCLAMATION );
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
		DEBUG_ASSERT( !"Fragment program linking error" );
	}

	glValidateProgramARB( m_hProgram );

	GLint validated = false;

	glGetObjectParameterivARB(m_hProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);

	if (!validated)
	{
		DEBUG_ASSERT( !"Not validated" );
	}

	return true;*/
}


/**
*
*/
uint CBaseShader::GetUniformLoc( const char * szUniform ) const
{
	if ( NULL == m_hProgram )
		return INVALID_INDEX;

	if ( szUniform )
	{
		GLint nLoc = glGetUniformLocationARB( m_hProgram, szUniform );
		GL_VALIDATE;

		return (uint)nLoc;
	}

	DEBUG_ASSERT( !"uniform not found" );
	return INVALID_INDEX;
}


/**

*
int CBaseShader::GetConstantIndex( EShaderAbility eUniform ) const
{
	if ( ( eUniform >= 0 ) && ( eUniform < SA_MAX_ABILITIES ) )
	{
		return m_pConstIndices[ eUniform ].nArrayID;
	}

	DEBUG_ASSERT( !"invalid uniform index" );
	return INVALID_INDEX;
}*/


/**
*
*/
bool CBaseShader::SetVertexDecl( IVertexDecl * pDecl )
{
	if ( NULL == pDecl )
		return false;

	m_pVertexDecl = pDecl;

	return true;
}


/**
*
*/
IShader	* CRenderer_GL::GetShader( EShaderType eShaderType )
{
	return m_ppShaders[ eShaderType ];
}


/**
*
*/
IShader	* CRenderer_GL::GetShader( const char *szShader )
{
	if ( szShader )
	{
		for ( size_t i = 0; i < m_ShaderList.GetNumItems(); ++i )
		{
			CBaseShader * pShader = (CBaseShader *)m_ShaderList[ i ];

			if ( pShader->m_sName.Compare( szShader ) )
				return pShader;
		}
	}

	return NULL; // set default shader
}


/**
*
*/
void CRenderer_GL::SetShader( IShader * pShader )
{
	m_pActiveShader = pShader;
}


////////////////////////////////////////////////////////////////////////////////