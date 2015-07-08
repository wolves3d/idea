
class CShader : public IShader
{
	//--------------------------------------------------------------------------
	// Tip: Public fields & methods
	//--------------------------------------------------------------------------
	public:

	CShader() :
		m_pVS			( NULL	),
		m_pByteCodeVS	( NULL	),
		m_pPS			( NULL	),
		m_pLayout		( NULL	),
		m_pRegisters	( NULL	),
		m_nUniformCount	( 0		),
		m_pVDecl		( NULL	)
	{
	}

	~CShader()
	{
		RELEASE( m_pByteCodeVS );
	}

		

	bool Assign()
	{
		/*
		ID3D10Buffer * pCB = m_ppConstBuffers[ 0 ];

		pCB->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&cb0 );
		memcpy( &cb0->mWorldViewProj, pValues, sizeof( mat4 ) );
		pCB->Unmap();
		*/

		g_pDevice->VSSetShader( m_pVS );
		g_pDevice->PSSetShader( m_pPS );
		g_pDevice->IASetInputLayout( m_pLayout );

		return false;
	}

	IVertexDecl * GetVertexDecl()
	{
		return m_pVDecl;
	}


	uint GetUniformCount() const
	{
		return m_nUniformCount;
	}


	uint GetUniformID ( const char * szName ) const
	{
		for ( uint n = 0; n < m_nUniformCount; ++n )
		{
			if ( m_pUniforms[ n ].sName.Compare( szName ) )
				return n;
		}

		return INVALID_INDEX;
	}


	ETypeID GetUniformType(uint) const { return INVALID_TYPE; }


	uint GetUniformSize ( uint nID ) const
	{
		if ( nID < m_nUniformCount )
		{
			return m_pUniforms[ nID ].nCount;
		}

		return 0;
	}


	void SetUniforms( uint nID, const vec4 * pValues )
	{
		if ( NULL == m_pRegisters )
		{
			m_ppConstBuffers[ 0 ]->Map( D3D10_MAP_WRITE_DISCARD, 0, (void **)&m_pRegisters );

			DEBUG_ASSERT( NULL != m_pRegisters );

			if ( NULL == m_pRegisters )
			{
				return;
			}
		}

		uint nFirst = m_pUniforms[ nID ].nLoc;
		uint nCount = m_pUniforms[ nID ].nCount;

		memcpy( m_pRegisters + nFirst, pValues, nCount * sizeof( vec4 ) );

		if ( nID == (m_nUniformCount-1) )
		{
			m_ppConstBuffers[ 0 ]->Unmap();
			m_pRegisters = NULL;
			g_pDevice->VSSetConstantBuffers( 0, 1, &m_ppConstBuffers[ 0 ] );
		}
	}

	ID3D10VertexShader *	m_pVS;
	ID3D10Blob *			m_pByteCodeVS;
	ID3D10PixelShader *		m_pPS;
	ID3D10InputLayout *		m_pLayout;
	ID3D10Buffer **			m_ppConstBuffers;

	IVertexDecl *			m_pVDecl;

	struct TUniform
	{
		CStr	sName;	/// Имя константы
		int		nLoc;	/// ID константы
		ETypeID	eType;	/// Тип константы
		uint	nCount; /// Кол-во элементов ( для массива )
	} * m_pUniforms;

	uint m_nUniformCount;
	CStr		m_sName;

	vec4 * m_pRegisters;
};