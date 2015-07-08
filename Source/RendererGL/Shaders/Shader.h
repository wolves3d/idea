////////////////////////////////////////////////////////////////////////////////

#ifndef __Shader_h_included__
#define __Shader_h_included__
/*
enum EState
{
	MS_ALPHA_TEST,

	MS_COLOR_WRITE,

	MS_DEPTH_TEST,
	MS_DEPTH_WRITE,

	MS_COLOR_ADD,
	MS_COLOR_MOD,

	MS_BUMP_MAP,
	MS_SPECULAR_MAP,

	MS_TOTAL
};


/**
*
*
class CShader : public IShader
{
	friend CRenderer_GL;

	public :

		// IShader

		void SetAttr( EShaderAttr eAttr, ETypeID eType, const void * pData )
		{
			switch ( eType )
			{
				case TYPE_BOOL:
					m_tData.m_pAttrs[ eAttr ].vVec4.x = (float)(bool)pData;
				break;

				case TYPE_VEC4:
					m_tData.m_pAttrs[ eAttr ].vVec4 = *(vec4 *)pData;
				break;
			}
		}

		// shaders

		inline bool IsVertexProgramUsed() const
		{
			return m_bUseVertexProgram;
		}

		inline void UseVertexPorgram( bool bUse = true )
		{
			m_bUseVertexProgram = bUse;
		}

		void SetSampler( int nSampler, const PTexture & pTexture )
		{
			m_tData.m_pSamplers[ nSampler ] = pTexture;
		}

		const TData & GetData() const
		{
			return m_tData;
		}

		void SetData( const TData & tData )
		{
			m_tData = tData;
		}

		void Assign() const;

	//private :

		bool m_bUseVertexProgram;
		bool m_bUseFragmentProgram;

		CShader( const char *szName );
		inline bool IsValid				() const { return true; }
		void		ParseVertexFormat	( const class CXMLElement * pStruct );
		void		ParsePrograms		( const class CXMLElement * pStruct );
		void		ParseMaps			( const class CXMLElement * pStruct );
		void		ParseCaps			( const class CXMLElement * pStruct );

		bool		SetVertexProgram	( const char * szSource );
		bool		SetFragmentProgram	( const char * szSource );

		CStr		m_sName;
		GLhandleARB	m_hProgram;

		TData		m_tData;

		struct TCap
		{
			CStr			sCapName;
			CStack <CStr>	pDirectives;
		};

		size_t m_nCapCount;
		TCap * m_pCaps;
};
*/
////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Shader_h_included__

////////////////////////////////////////////////////////////////////////////////