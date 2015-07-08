////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "BaseLocation.h"

//!

//!
//!
class CLocRoom : public IBaseLoc
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:


	//!

	//!
	//!
	bool Init()
	{
		TResource * pMyTexRes = NULL;
		//g_pResManager->Demand( "back.tga", (void **)&pMyTex );
		g_pResManager->Request( "back.tga", &pMyTexRes );
		m_pBackTex = *( (PTexture *)pMyTexRes->pResource );
		

		m_pBackMat = g_pEngine->CreateMaterial();
		m_pBackMat->SetShader( g_pRenderer->GetShader( UI_SHADER ) );
		m_pBackMat->SetTexture( DIFFUSE_MAP, m_pBackTex );
		m_pBackMat->EnableDepthTest( false );
		m_pBackMat->EnableDepthWrite( false );

		m_pBackSprite = g_pUIManager->CreateSprite();
		m_pBackSprite->SetPos( 0, 0 );
		m_pBackSprite->SetSize( 800, 600 );
		m_pBackSprite->SetMaterial( m_pBackMat );

		IShader * pUIShader = g_pRenderer->GetShader( UI_SHADER );
		CUniform uColorMod( "vColorMod", pUIShader );
		uColorMod.Connect( m_pBackMat );
		uColorMod.SetValue( &vec4( 1, 1, 1, 1 ), 1 );

		/*
		IFile *	pFile = NULL;
		void *	pData = NULL;
		uint	nSize = 0;
		g_pVfs->OpenFile( &pFile, "sample.wav", VFS_READ | VFS_BINARY );

		if ( pFile )
		{
		nSize = pFile->GetSize();
		pData = NEW byte [ nSize ];
		g_pVfs->Read( pFile, pData, nSize );
		g_pVfs->CloseFile( &pFile );
		}

		IVoice * pVoice		= g_pSoundMgr->CreateVoice( true );
		ISample * pSample	= g_pSoundMgr->CreateSample( pData, nSize );

		pVoice->QueueSample( pSample );
		pVoice->Play( true );
		*/

		return false;
	}


	//!

	//!
	//!
	void Update()
	{
		g_pUIManager->PushSprite( m_pBackSprite );
		g_pUIManager->Render();
	}


	//!

	//!
	//!
	void Release()
	{
		if ( m_pBackSprite )
		{
			g_pUIManager->DeleteSprite( m_pBackSprite );
			m_pBackSprite = NULL;
		}

		RELEASE( m_pBackTex );
		RELEASE( m_pBackMat );
	}


	//==========================================================================
	// Tip: Private Methods & Fields
	//==========================================================================
	private:

		ISprite2D * m_pBackSprite;	/// Спрайт для задника

		IMaterial * m_pBackMat;
		PTexture	m_pBackTex;		/// Текстура задника
};


//!

//!
//!
IBaseLoc * CreateRoom()
{
	return NEW CLocRoom;
}

////////////////////////////////////////////////////////////////////////////////