////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "SoundMgr.h"


/**
*
*/
class CVoice : public IVoice
{

public:

	/**
	*
	*/
	CVoice( bool bStereo ) :
		m_bLoop		( false ),
		m_nCurSample( 0 )
	{
		m_pBuffer = g_pSoundMgr->CreateSoundBuffer( SND_BUFFER_STEREO );

		m_Queue.Alloc( 8, false );
	}


	/**
	*
	*/
	~CVoice()
	{
		RELEASE( m_pBuffer );
	}


	/**
	*
	*/
	void QueueSample ( ISample * pSample )
	{
		DEBUG_ASSERT( NULL != pSample );

		if ( pSample )
			m_Queue.Push( pSample );
	}


	/**
	*
	*/
	void ClearQueue	()
	{
		m_Queue.Clear();
		m_nCurSample = 0;
	}


	/**
	*
	*/
	void Play( bool bRewind = false )
	{
		DEBUG_ASSERT( NULL != m_pBuffer );
		DEBUG_ASSERT( m_Queue.GetCount() > m_nCurSample );

		if ( NULL == m_pBuffer )
			return;

		if ( m_Queue.GetCount() < m_nCurSample )
			return;
		
		ISample * pSample = m_Queue[ m_nCurSample ];

		dword	dwLockedSizeOne;
		dword	dwLockedSizeTwo;
		void *	pLockedDataOne;
		void *	pLockedDataTwo;
		uint	nSize = 4 * 2 * 44100;

		HRESULT hRes = m_pBuffer->Lock(
			0,			//dwOffset
			nSize,
			&pLockedDataOne,
			&dwLockedSizeOne,
			&pLockedDataTwo,
			&dwLockedSizeTwo, 0 );

		if ( FAILED( hRes ) )
		{
			DEBUG_MSG( "lock failed" );
			return;
		}

		pSample->GetData( pLockedDataOne, 0, nSize );

		m_pBuffer->Unlock(
			pLockedDataOne,
			dwLockedSizeOne,
			pLockedDataTwo,
			dwLockedSizeTwo );

		//m_pBuffer->Play( 0, 0, ( pVoice->m_bLoop ) ? DSBPLAY_LOOPING : 0 );
		m_pBuffer->Play( 0, 0, DSBPLAY_LOOPING );

		m_dwPlayPos  = 0;
		m_dwWrtiePos = 0;
		m_dwSamplePos = nSize;
	}


	/**
	*
	*/
	bool IsPlaying()
	{
		return false;
	}


	/**
	* Ответ - в байтах
	*/
	uint GetPlayPos()
	{
		dword dwPlayPos = 0;

		if ( m_pBuffer )
		{
			HRESULT	hRes = m_pBuffer->GetCurrentPosition( &dwPlayPos, 0 );

			if ( FAILED( hRes ) )
			{
				DEBUG_MSG( "GetCurrentPosition failed" );
			}

			return dwPlayPos;
		}

		DEBUG_MSG( "error" );
		return dwPlayPos;
	}


	/**
	*
	*/
	void SetVolume( float fVol )
	{
	}


	/**
	*
	*/
	float GetVolume()
	{
		return 0.f;
	}


	/**
	*
	*/
	void SetPos( const vec3 & vPos )
	{
		m_vPos = vPos;
	}


	/**
	*
	*/
	const vec3 & GetPos	()
	{
		return m_vPos;
	}

	/**
	*
	*/
	void Update()
	{
		// Пишем блоками по 1 / 10 секунды
		dword dwBlockSize = (dword)( 4.f * 44100.f * 0.1f );

		dword dwOldPos = m_dwPlayPos;
		m_dwPlayPos = GetPlayPos();

		bool bWrap = ( dwOldPos > m_dwPlayPos );

		if ( !bWrap && ( m_dwPlayPos < m_dwWrtiePos + dwBlockSize ) )
			return;

		ISample * pSample = m_Queue[ m_nCurSample ];

		dword	dwLockedSizeOne;
		dword	dwLockedSizeTwo;
		void *	pLockedDataOne;
		void *	pLockedDataTwo;

		HRESULT hRes = m_pBuffer->Lock(
			m_dwWrtiePos,
			dwBlockSize,
			&pLockedDataOne,
			&dwLockedSizeOne,
			&pLockedDataTwo,
			&dwLockedSizeTwo, 0 );

		if ( FAILED( hRes ) )
		{
			DEBUG_MSG( "lock failed" );
			return;
		}

		dword dwRest = ( pSample->GetLength() - m_dwSamplePos );

		if ( dwBlockSize > dwRest )
		{
			dwBlockSize = dwRest;
		}

		pSample->GetData( pLockedDataOne, m_dwSamplePos, dwBlockSize );

		m_pBuffer->Unlock(
			pLockedDataOne,
			dwLockedSizeOne,
			pLockedDataTwo,
			dwLockedSizeTwo );

		m_dwWrtiePos	+= dwBlockSize;
		m_dwSamplePos	+= dwBlockSize;
		pCounter->IncValue( dwBlockSize );

		if ( m_dwWrtiePos >= ( 4 * 2 * 44100 ) )
		{
			pCounter->ResetValue();
			m_dwWrtiePos = 0;
		}

		if ( m_dwSamplePos >= pSample->GetLength() )
		{
			m_dwSamplePos = 0;
			m_nCurSample++;

			if ( m_nCurSample >= m_Queue.GetCount() )
			{
				if ( m_bLoop )
				{
					m_nCurSample = 0;
				}
				else
				{
					m_pBuffer->Stop();
				}
			}
		}
	}


private:

	IDirectSoundBuffer *	m_pBuffer;
	IDirectSound3DBuffer *	m_pBuffer3D;
	
	bool					m_bLoop;
	vec3					m_vPos;

	CStack <ISample *>		m_Queue;
	uint					m_nCurSample;

	dword					m_dwPlayPos;
	dword					m_dwWrtiePos;
	dword					m_dwSamplePos;
};


/**
*
*/
IVoice * CSoundMgr::CreateVoice( bool bStereo )
{
	IVoice * pVoice = NEW CVoice( bStereo );

	if ( pVoice )
	{
		m_Voices.Push( pVoice );
	}

	return pVoice;
}

////////////////////////////////////////////////////////////////////////////////