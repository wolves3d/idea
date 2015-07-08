////////////////////////////////////////////////////////////////////////////////

#include "_Shared\Math\3dmath.h"
#include "_Shared\API\ISoundMgr.h"

#include <mmsystem.h>
#include <dsound.h>

////////////////////////////////////////////////////////////////////////////////

#define VOICE_LENGTH 2


/**
*
*/
enum ESoundBufferType
{
	SND_BUFFER_PRIMARY,
	SND_BUFFER_STEREO,
	SND_BUFFER_3D
};


/**
*
*/
class CSoundMgr : public ISoundMgr
{

public:

	CSoundMgr();
	~CSoundMgr();

	const TIntfDesc *	GetInterfaceDesc() const;
	bool				Init			( HWND hWindow );
	void				Update			();
	void				Release			();
	void				Delete			() { delete this; }

	ISample *	CreateSample( const char * szFileName ) { return NULL; }
	ISample *	CreateSample( const void * pData, uint nDataSize );
	IVoice *	CreateVoice	( bool bStereo );

	void		SetListener( const vec3 * pPos, const vec3 *pTgt, const vec3 *pUp );

	//--------------------------------------------------------------------------
	//	Tip: 
	//--------------------------------------------------------------------------
	IDirectSoundBuffer * CreateSoundBuffer( ESoundBufferType eType );

	static TIntfDesc ms_ClassDesc;

private:

	//----------------------------------------------------------------------
	//	Tip: 
	//----------------------------------------------------------------------
	IDirectSound8 *				m_pDSound;
	IDirectSoundBuffer *		m_pPrimaryBuffer;
	IDirectSound3DListener *	m_pListener;
	DS3DLISTENER				m_ListenerAttrs;

	bool						m_bCommit;
	CStack <IVoice *>			m_Voices;
};

extern ICounter * pCounter;

////////////////////////////////////////////////////////////////////////////////

extern CSoundMgr * g_pSoundMgr;

////////////////////////////////////////////////////////////////////////////////