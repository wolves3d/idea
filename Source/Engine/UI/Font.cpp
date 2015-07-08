////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"

////////////////////////////////////////////////////////////////////////////////

class CFont : public IMyFont
{
public:

	inline size_t GetSize() const
	{
		return m_nSize;
	}

	const vec4 & GetCharUV( char cChar ) const
	{
		return m_pFontUVs[ ( uint )cChar ];
	}

	float GetCharWidth( char cChar ) const
	{
		const vec4 & vUVs = GetCharUV( cChar );
		return ( vUVs.z * 256.f );
	}

	void SetCharUV( uint nChar, const vec4 & vUV )
	{
		if ( nChar < 256 )
		{
			m_pFontUVs[ ( uint )nChar ] = vUV;
		}
	}

	inline bool IsConstantWidth() const
	{
		return m_bConstWidth;
	}

	inline const PTexture GetTexture() const
	{
		return m_pFontTexture;
	}

	//private:

	size_t		m_nSize;
	bool		m_bConstWidth;
	PTexture	m_pFontTexture;
	vec4		m_pFontUVs[ 256 ];
};


/**
*
*/
bool CUIManager::LoadFont( const char * szFntFile, const char * szTexture )
{
	//--------------------------------------------------------------------------
	// Tip: Font material setup
	//--------------------------------------------------------------------------
	DEL( m_pFontMaterial );
	DEL( m_pDefaultFont );

	if ( m_pFontMaterial = g_pEngine->CreateMaterial() )
	{
		m_pFontMaterial->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
		m_pFontMaterial->EnableAlphaTest( false );
		m_pFontMaterial->EnableDepthTest( false );

		//m_pFontMaterial->EnableAlphaBlend();
		//m_pFontMaterial->BlendFunc( BF_ONE_MINUS_SRC_COLOR, BF_ZERO );

		m_pFontMaterial->SetTexture( DIFFUSE_MAP,
			g_pRenderer->GetSysTexture( EST_GREY ) );
	}
	else
	{
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: Font uv's loading
	//--------------------------------------------------------------------------
	if ( !g_pVfs )
		return false;
	
	IFile * pFile;

	if ( R_OK == g_pVfs->OpenFile( &pFile, szFntFile, VFS_READ | VFS_BINARY ) )
	{
		CFont * pFont = NEW CFont;
		m_pDefaultFont = pFont;

		uint16 nNumChars;
		g_pVfs->Seek( pFile, 10, VFS_SEEK_SET );
		g_pVfs->Read( pFile, &nNumChars, sizeof( uint16 ) );

		g_pVfs->Seek( pFile, 20, VFS_SEEK_SET );

		for ( int i = 0; i < nNumChars; ++i )
		{
			struct
			{
				uint16 nCode;
				uint16 nPage;
				uint16 nPosX;
				uint16 nPosY;
				uint16 nWidth;
				uint16 nHeight;
				uint16 nLeadX;
				uint16 nTrailX;
			} tCharInfo;

			g_pVfs->Read( pFile, &tCharInfo, sizeof( tCharInfo ) );

			if ( tCharInfo.nCode > 126 )
				tCharInfo.nCode -= 848;

			pFont->SetCharUV( (uint)tCharInfo.nCode, vec4(
				(float)tCharInfo.nPosX / 256.f,
				(float)tCharInfo.nPosY / 256.f,
				(float)tCharInfo.nWidth / 256.f,
				(float)tCharInfo.nHeight / 256.f ) );
		}

		g_pVfs->CloseFile( &pFile );

		//--------------------------------------------------------------------------
		// Tip: Font texture loading
		//--------------------------------------------------------------------------
		PTexture pTexture;

		if ( g_pEngine->LoadTexture( pTexture, szTexture ) )
		{
			//pTexture->SetFilter( FILTER_NEAREST );
			m_pFontMaterial->SetTexture( DIFFUSE_MAP, pTexture );
			pFont->m_pFontTexture = pTexture;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////