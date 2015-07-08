////////////////////////////////////////////////////////////////////////////////

#ifndef __Material_h_included__
#define __Material_h_included__

////////////////////////////////////////////////////////////////////////////////

class CMaterial : public IMaterial
{
	public:

		enum EState
		{
			ALPHA_FUNC, // ?
			DEPTH_FUNC, // ?

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

		enum EConstant // Material Constant Slots //////////////////////////////
		{
			MC_ALPHA_REF,
			MC_COLOR_ADD,
			MC_COLOR_MOD,

			MC_TOTAL
		};

		class CConstant
		{
			friend CMaterial;

			public :

			inline CConstant () :
				m_nCount	( 0		),
				m_pValues	( NULL	)
			{
			}

			~CConstant()
			{
				DEL_ARRAY( m_pValues );
			}

			inline bool Init( size_t nCount )
			{
				if ( nCount )
				{
					if ( m_nCount ) // Если уже инициализирована...
					{
						// ...проверяем чтобы размер массива совпадал
						DEBUG_ASSERT( nCount == m_nCount );
						return ( nCount == m_nCount );
					}

					if ( m_pValues = NEW float[ nCount ] )
					{
						m_nCount = nCount;
						return true;
					}
				}
				
				return false;
			}

			inline bool SetData( float * pData, size_t nCount )
			{
				DEBUG_ASSERT( pData );
				DEBUG_ASSERT( nCount == m_nCount );

				if ( pData )
				{
					if ( nCount == m_nCount )
					{
						for ( size_t i = 0; i < m_nCount; ++i )
							m_pValues[ i ] = pData[ i ];

						return true;
					}
				}

				return false;
			}

			private:

			size_t	m_nCount;
			float *	m_pValues;
		};

		CMaterial();

		void EnableAttr( EState eState, bool bEnable = true );

		inline CConstant * GetConstant( EConstant eConst )
		{
			DEBUG_ASSERT( eConst >= 0 );
			DEBUG_ASSERT( eConst < MC_TOTAL );

			if ( eConst >= 0 )
			{
				if ( eConst < MC_TOTAL )
				{
					return m_pConstants + eConst;
				}
			}

			return NULL;
		}

		
		IShader *	GetShader();
		bool		SetShader( IShader * pShader );

		g_pRenderer->GetShader( SC_BUMP | SC_COLOR_ADD );
		

		IShader * CMaterial::PrepareShader();

	//private:

		IShader *	m_pShader;
		PTexture 	m_pTexture;

	private:

		bool		FindShader();

		bool		m_pStates	[ MS_TOTAL ];
		CConstant	m_pConstants[ MC_TOTAL ];
		PTexture	m_pTextures	[ MT_TOTAL ];
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Material_h_included__

////////////////////////////////////////////////////////////////////////////////