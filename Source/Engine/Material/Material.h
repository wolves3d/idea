////////////////////////////////////////////////////////////////////////////////

#ifndef __Material_h_included__
#define __Material_h_included__

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

class CMaterial : public IMaterial
{
	friend CUniform;

	public:

					CMaterial		();
					~CMaterial		();
	void			Release			();

	bool			Assign			();

	void			BlendFunc		( EBlendFactor eSrc, EBlendFactor eDst );

	void			EnableAlphaBlend( bool bEnable = true );
	void			EnableAlphaTest	( bool bEnable = true );
	void			EnableDepthTest	( bool bEnable = true );
	void			EnableDepthWrite( bool bEnable = true );

	PTexture		GetTexture		( ETexture eTex );
	bool			SetTexture		( ETexture eTex, PTexture pTex );

	IShader *		GetShader		();
	bool			SetShader		( IShader * pShader );

	void			AddSubMaterial	( IMaterial * pMaterial );
	IMaterial *		GetSubMaterial	( uint nSub );
	uint			GetSubCount		() const;

	private:

	#define MAX_REGISTERS 98

	void SetRegisters( uint nUniform, const vec4 * pValues, uint nCount )
	{
		if ( nUniform < m_nUniformCount )
		{
			TUniformInfo &	tInfo	= m_pUniformInfos[ nUniform ];
			uint			nLast	= tInfo.nFirst + nCount;

			DEBUG_ASSERT( nLast <= ( tInfo.nFirst + tInfo.nSize ) );

			if ( nLast < MAX_REGISTERS )
			{
				vec4 * pRegs = m_pRegisters + tInfo.nFirst;

				for ( uint n = 0; n < nCount; ++n )
					pRegs[ n ] = pValues[ n ];

				return;
			}
			else
			{
				DEBUG_ASSERT( !"register overflow" );
			}

			DEBUG_ASSERT( !"invalid uniform id" );
		}
	}

	bool				m_bAlphaBlend,
						m_bAlphaTest,
						m_bDepthTest,
						m_bDepthWrite;

	EBlendFactor		m_eBlendSrc,
						m_eBlendDst;

	PTexture *			m_pTexMaps;

	IShader *			m_pShader;

	struct TUniformInfo
	{
		uint nFirst;
		uint nSize;
	} * m_pUniformInfos;

	uint m_nUniformCount;

	vec4 				m_pRegisters[ MAX_REGISTERS ];

	CVector <IMaterial *>	m_pSubMaterials;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Material_h_included__

////////////////////////////////////////////////////////////////////////////////