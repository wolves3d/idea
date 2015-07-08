	////////////////////////////////////////////////////////////////////////////////

#ifndef __BaseShader_h_included__
#define __BaseShader_h_included__

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
class CBaseShader : public IShader
{
	friend class CMaterial;
	friend class CVertexBuffer;
	friend class CRenderer_GL;

	public :

			CBaseShader			( const char * szName );
			~CBaseShader		();

	bool			Assign();

	bool			SetVertexProgram	( const char * szSource, IVertexDecl * pDecl, bool bGLSL = false );
	bool			SetFragmentProgram	( const char * szSource, bool bGLSL = false );

	
	bool			SetVertexDecl		( IVertexDecl * pDecl );
	IVertexDecl *	GetVertexDecl		()
	{
		return m_pVertexDecl;
	}

	uint GetUniformID( const char * szName ) const
	{
		for ( uint n = 0; n < m_nUniformCount; ++n )
		{
			if ( m_pUniforms[ n ].sName.Compare( szName ) )
				return n;
		}

		return INVALID_INDEX;
	}

	void SetUniforms( uint nID, const vec4 * pValues )
	{
		const TUniform & tUniform = m_pUniforms[ nID ];

		if ( m_hProgram )
		{
			switch ( tUniform.eType )
			{
				case TYPE_VEC4:
					glUniform4fvARB( tUniform.nLoc, tUniform.nCount, (float *)pValues );
					GL_VALIDATE;
				break;

				case TYPE_MAT4:
					glUniformMatrix4fvARB(
						tUniform.nLoc,
						tUniform.nCount,
						false,
						(float *)pValues );
					GL_VALIDATE;
				break;
			}
		}
		else
		{
			if ( !g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
				return;

			if ( nID < m_nUniformCount )
			{
				switch ( tUniform.eType )
				{
					case TYPE_VEC4:
					{
						//red
						glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 1, (float *)pValues );
						GL_VALIDATE;
					}
					break;

					case TYPE_MAT4:
					{
						for ( uint n = 0; n < tUniform.nCount; ++n )
						{
							uint i = 1 + ( n * 4 );
							glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i, (float *)pValues );
							GL_VALIDATE;
							glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i + 1, (float *)(pValues + 1) );
							GL_VALIDATE;
							glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i + 2, (float *)(pValues + 2) );
							GL_VALIDATE;
							glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i + 3, (float *)(pValues + 3) );
							GL_VALIDATE;

							pValues += 4;
						}
					}
					break;

					default:
						DEBUG_ASSERT( !"invalid type" );
				}
			}
		}
	}

	uint GetUniformCount() const
	{
		return m_nUniformCount;
	}

	ETypeID GetUniformType( uint nID ) const
	{
		if ( nID < m_nUniformCount )
			return m_pUniforms[ nID ].eType;

		DEBUG_ASSERT( false );
		return INVALID_TYPE;
	}

	//!  ол-во регистров занимаемых переменной (массивом)
	uint GetUniformSize( uint nID ) const
	{
		if ( nID < m_nUniformCount )
		{
			uint nRegCount = m_pUniforms[ nID ].nCount;

			switch ( m_pUniforms[ nID ].eType )
			{
				case TYPE_VEC4:
					return nRegCount;

				case TYPE_MAT4:
					return ( 4 * nRegCount );

				default:
					DEBUG_ASSERT( !"unexpected type of constant" );
			}
		}

		DEBUG_ASSERT( false );
		return 0;
	}

	//==========================================================================
	// Tip: Protected methods & fields
	//==========================================================================
	//protected:

	uint GetUniformLoc( const char * szUniform ) const;

	IVertexDecl *	m_pVertexDecl;
	CStr			m_sName;
	uint		m_hProgram;
	GLuint		m_hVertexProgram;
	GLuint		m_hFragmentProgram;

	TUniform *	m_pUniforms;
	uint		m_nUniformCount;
};


////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __BaseShader_h_included__

////////////////////////////////////////////////////////////////////////////////