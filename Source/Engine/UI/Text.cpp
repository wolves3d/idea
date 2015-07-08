////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

/**
*
*/
class CText2D : public IText2D
{
	public:

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	CText2D( IMyFont * pFont ) :
		m_bMoved( false ),
		m_pFont	( pFont )
	{
		m_vPos.Set( 0, 0 );

		m_pFont = g_pUIManager->GetFont( NULL );

		m_pMaterial = g_pEngine->CreateMaterial();
		m_pMaterial->EnableDepthTest( false );
		m_pMaterial->EnableAlphaTest();
		m_pMaterial->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
		m_pMaterial->SetTexture( DIFFUSE_MAP, pFont->GetTexture() );

		m_uColorMod.AttachToShader( "vColorMod", g_pRenderer->GetShader( UI_SHADER ) );
		m_uColorMod.Connect( m_pMaterial );
		m_uColorMod.SetValue( &vec4( 0, 0, 0, 1 ), 1 );
	}


	/**
	*
	*/
	~CText2D()
	{
		RELEASE( m_pMaterial );
		m_Sprites.Delete();
	}


	/**
	*
	*/
	void Release()
	{
		PURE_DEL( this );
	}


	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	const vec2 & GetPos() const
	{
		return m_vPos;
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	void SetPos( float x, float y )
	{
		m_vPos.Set( x, y );
		m_bMoved = true;
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	void SetColor( const vec4 & vColor )
	{
		m_uColorMod.SetValue( &vColor, 1 );
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	void SetText( const char * szText )
	{
		for ( uint n = 0; n < m_Sprites.GetCount(); n++ )
		{
			g_pUIManager->DeleteSprite( m_Sprites[ n ] );
		}

		m_Sprites.Clear();

		if ( NULL == szText )
			return;

		////////////////////////////////////////////////////////////////////////

		m_sText = szText;

		m_Sprites.Alloc( m_sText.GetSize() );

		vec2 vCaret = m_vPos;

		for ( uint n = 0; n < m_sText.GetSize(); n++ )
		{
			ISprite2D * pSprite = g_pUIManager->CreateSprite();

			if ( pSprite )
			{
				char cChar = szText[ n ];

				const vec4 & vUVs = m_pFont->GetCharUV( cChar );
				float fWidth = m_pFont->GetCharWidth( cChar );
				
				pSprite->SetMaterial( m_pMaterial );
				pSprite->SetSize( fWidth, 16.f );
				pSprite->SetUVs( vec4( vUVs.x, 1 - vUVs.y - vUVs.w, vUVs.x + vUVs.z, 1 - vUVs.y ) );
				m_Sprites.Push( pSprite );
			}
		}

		m_bMoved = true;
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	void Render()
	{
		vec2 vCaret = m_vPos;

		for ( uint n = 0; n < m_Sprites.GetCount(); n++ )
		{
			char cChar = m_sText[ n ];

			if ( 10 == cChar )
			{
				if ( m_bMoved )
				{
					vCaret.x = m_vPos.x;
					vCaret.y += 16;
					continue;
				}
			}

			ISprite2D * pSprite = m_Sprites[ n ];

			if ( m_bMoved )
			{
				float fWidth = m_pFont->GetCharWidth( m_sText[ n ] );
				pSprite->SetPos( vCaret.x, vCaret.y );
				vCaret.x += fWidth + 1.f;
			}

			g_pUIManager->PushSprite( pSprite );
		}

		m_bMoved = false;
	}

	private:

		bool		m_bMoved;
		vec2		m_vPos;
		uint		m_nLength;
		
		CStack <ISprite2D *>	m_Sprites;
		CStr					m_sText;
		const IMyFont *			m_pFont;
		IMaterial *				m_pMaterial;

		CUniform				m_uColorMod;
};


/**
*
*/
IText2D * CUIManager::CreateText()
{
	return NEW CText2D( m_pDefaultFont );
}

////////////////////////////////////////////////////////////////////////////////