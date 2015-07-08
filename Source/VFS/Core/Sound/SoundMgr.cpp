////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "SoundMgr.h"

#pragma comment ( lib, "dsound.lib" )

////////////////////////////////////////////////////////////////////////////////

CSoundMgr * g_pSoundMgr = NULL;
ICounter * pCounter = NULL;

////////////////////////////////////////////////////////////////////////////////

TIntfDesc CSoundMgr::ms_ClassDesc = 
{
	111,
	IT_SOUND_MGR,
	1,
	"CSoundMgr"
};


/**
*
*/
CSoundMgr::CSoundMgr() :
	m_pDSound		( NULL ),
	m_pPrimaryBuffer( NULL ),
	m_pListener		( NULL ),
	m_bCommit		( false )
{
	DEBUG_ASSERT( NULL == g_pSoundMgr );
	g_pSoundMgr = this;
}


/**
*
*/
CSoundMgr::~CSoundMgr()
{
		for ( uint n = 0; n < m_Voices.GetCount(); ++n )
	{
		DEL( m_Voices[ n ] );
	}

	m_Voices.Delete();

	RELEASE( m_pPrimaryBuffer );
	RELEASE( m_pDSound );
}


/**
*
*/
void CSoundMgr::Release()
{
	PURE_DEL( this );
}


/**
*
*/
const TIntfDesc * CSoundMgr::GetInterfaceDesc() const
{
	return & ms_ClassDesc;
}


/**
*
*/
bool CSoundMgr::Init( HWND hWindow )
{
	DEBUG_ASSERT( NULL == m_pDSound );
	DEBUG_ASSERT( NULL == m_pPrimaryBuffer );

	//--------------------------------------------------------------------------
	HRESULT hRes = DirectSoundCreate8(
		&DSDEVID_DefaultPlayback,
		&m_pDSound,
		NULL );

	if ( FAILED( hRes ) )
		goto InitFailed;

	//--------------------------------------------------------------------------
	hRes = m_pDSound->SetCooperativeLevel( hWindow, DSSCL_PRIORITY );

	if ( FAILED( hRes ) )
		goto InitFailed;

	//--------------------------------------------------------------------------
// 	hRes = m_pDSound->Initialize( NULL );
// 
// 	if ( FAILED( hRes ) )
// 		goto InitFailed;

	//--------------------------------------------------------------------------
	m_pPrimaryBuffer = CreateSoundBuffer( SND_BUFFER_PRIMARY );
	
	if ( NULL == m_pPrimaryBuffer )
		goto InitFailed;

	//--------------------------------------------------------------------------
	hRes = m_pPrimaryBuffer->QueryInterface(
		IID_IDirectSound3DListener8,
		(LPVOID *) &m_pListener );

	if ( FAILED( hRes ) )
		goto InitFailed;

	//--------------------------------------------------------------------------
	ZeroMemory( &m_ListenerAttrs, sizeof( m_ListenerAttrs ) );
	m_ListenerAttrs.dwSize = sizeof( m_ListenerAttrs );

	hRes = m_pListener->GetAllParameters( &m_ListenerAttrs );

	if ( FAILED( hRes ) )
		goto InitFailed;

	//--------------------------------------------------------------------------
	pCounter = g_pSystem->AddCounter( "snd" );
	m_pPrimaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
	m_Voices.Alloc( 16, false );
	return true;

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	InitFailed:

	RELEASE( m_pListener );
	RELEASE( m_pPrimaryBuffer );
	RELEASE( m_pDSound );
	return false;
}


/**
*
*/
void CSoundMgr::SetListener( const vec3 * pPos, const vec3 *pTgt, const vec3 *pUp )
{
	if ( pPos )
	{
		m_pListener->SetPosition( pPos->x, pPos->y, pPos->z, DS3D_DEFERRED );
		m_bCommit = true;
	}

	if ( pTgt )
	{
		vec3 vUp;

		if ( NULL == pUp )
		{
			vUp = vec3( 0, 1, 0 ); // WRONG!!!
			pUp = &vUp;
		}

		m_pListener->SetOrientation(
			pTgt->x, pTgt->y, pTgt->z,
			pUp->x, pUp->y, pUp->z, DS3D_DEFERRED );

		m_bCommit = true;
	}
}


/**
*
*/
IDirectSoundBuffer * CSoundMgr::CreateSoundBuffer( ESoundBufferType eType )
{
	WAVEFORMATEX			wf;
	DSBUFFERDESC			bufferDesc;
	IDirectSoundBuffer *	pBuffer;

	ZeroMemory( &wf, sizeof( WAVEFORMATEX ) ); 
	ZeroMemory(	&bufferDesc, sizeof( DSBUFFERDESC ) );

	wf.cbSize			= sizeof( WAVEFORMATEX );
	wf.wFormatTag		= WAVE_FORMAT_PCM; 
	wf.nChannels		= 2; 
	wf.nSamplesPerSec	= 44100; 
	wf.wBitsPerSample	= 16;
	wf.nBlockAlign		= wf.nChannels * ( wf.wBitsPerSample / 8 );
	wf.nAvgBytesPerSec	= wf.nSamplesPerSec * wf.nBlockAlign; 

	bufferDesc.dwSize			= sizeof( DSBUFFERDESC ); 
	bufferDesc.dwFlags			= DSBCAPS_CTRLVOLUME | DSBCAPS_LOCDEFER;
	bufferDesc.guid3DAlgorithm	= GUID_NULL;
	bufferDesc.lpwfxFormat		= &wf;
	//bufferDesc.dwFlags	|= DSBCAPS_GLOBALFOCUS;

	switch ( eType )
	{
		case SND_BUFFER_PRIMARY:
			bufferDesc.lpwfxFormat = NULL;
			bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
			bufferDesc.dwFlags |= DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
		break;

		case SND_BUFFER_STEREO:
			bufferDesc.dwBufferBytes = VOICE_LENGTH * wf.nSamplesPerSec * wf.nBlockAlign;
		break;

		case SND_BUFFER_3D:
			bufferDesc.dwBufferBytes = VOICE_LENGTH * wf.nSamplesPerSec * wf.nBlockAlign;
			bufferDesc.dwFlags |= DSBCAPS_CTRL3D;
		break;

		default:
			DEBUG_MSG( "invalid snd buffer type" );
			return NULL;
	}

	if ( SUCCEEDED( m_pDSound->CreateSoundBuffer( &bufferDesc, &pBuffer, 0 ) ) )
	{
		pBuffer->SetFormat( &wf );
		return pBuffer;
	}

	RELEASE( pBuffer );
	return NULL;
}


/**
*
*/
void CSoundMgr::Update()
{
	for ( uint n = 0; n < m_Voices.GetCount(); ++n )
	{
		m_Voices[ n ]->Update();
	}

	if ( m_bCommit )
	{
		if ( m_pListener )
			m_pListener->CommitDeferredSettings();

		m_bCommit = false;
	}
}

////////////////////////////////////////////////////////////////////////////////