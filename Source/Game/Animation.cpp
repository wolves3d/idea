////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Animation.h"

ICounter * pCounter;

/**
*
*/
CAnimation::CAnimation() :
	m_nCurTex( 0 )
{
}


/**
*
*/
bool CAnimation::Load( const char *szFile )
{
	pCounter = g_pSystem->AddCounter( "anim" );

	IMaterial * pMaterial = g_pEngine->CreateMaterial();
	pMaterial->SetShader( g_pRenderer->GetShader( UI_SHADER ) );

	m_pSprite = g_pUIManager->CreateSprite();
	m_pSprite->SetPos( 0, 0 );
	m_pSprite->SetSize( 128, 128 );
	m_pSprite->SetMaterial( pMaterial );

	szFile = "test/untitled";
	bool bContinue = true;
	int nCount = 1;
	CStr str;

	while ( bContinue )
	{
		PTexture pTexture;

		str = "test/untitled";
		str.AddNumber( nCount );
		str.Add( ".tga" );

		bContinue = g_pEngine->LoadTexture( pTexture, str.GetString() );

		if ( bContinue )
			m_Textures.Push( pTexture );

		if ( nCount > 99 )
			break;

		nCount++;
	}

	return true;
}


/**
*
*/
void CAnimation::Play()
{
	m_dwStart = GetTickCount();
	m_nCurTex = 0;

	IMaterial * pMaterial = m_pSprite->GetMaterial();
	pMaterial->SetTexture( DIFFUSE_MAP,	m_Textures[ m_nCurTex ] );
}


/**
*
*/
void CAnimation::Render()
{
	IMaterial * pMaterial = m_pSprite->GetMaterial();

	if ( GetTickCount() > ( m_dwStart + ( m_nCurTex * 40 ) ) )
	{
		pCounter->IncValue();
		m_nCurTex++;

		if ( m_nCurTex >= m_Textures.GetCount() )
		{
			m_dwStart = GetTickCount();
			m_nCurTex = 0;
			pCounter->ResetValue();
		}

		pMaterial->SetTexture( DIFFUSE_MAP,	m_Textures[ m_nCurTex ] );
	}

	g_pUIManager->PushSprite( m_pSprite );
}

////////////////////////////////////////////////////////////////////////////////