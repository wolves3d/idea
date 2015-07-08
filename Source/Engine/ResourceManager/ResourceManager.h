/**
*
*/
class CCriticalSec
{
	public:

	inline CCriticalSec()
	{
		InitializeCriticalSection( &m_Section );
	}

	~CCriticalSec()
	{
		DeleteCriticalSection( &m_Section );
	}

	inline void Enter()
	{
		EnterCriticalSection( &m_Section );
	}

	inline void Leave()
	{
		LeaveCriticalSection( &m_Section );
	}

	private:

	CRITICAL_SECTION m_Section;
};


/**
*
*/
enum EResType
{
	RT_TEXTURE,
	RT_MESH
};


//!

//!
//!
class CResManager : public IResManager
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:

	CResManager	();
	~CResManager();

	//=== IBaseIntf ============================================================

	const TIntfDesc *	GetInterfaceDesc() const { return &ms_ClassDesc; }
	void				Release			() { PURE_DEL( this ); }

	static TIntfDesc ms_ClassDesc;

	//=== IResManager ==========================================================

	void RegCreator( IResCreator * pCreator );

	//! �� ������� ���� �� �������� ������
	bool Request( const char * szFile, TResource ** ppOutResource );

	//! ������ ������ � ������� �� �������� � ����� �������
	uint Demand( const char * szFile, TResource ** ppOutResource, EResPriority ePriority = ERP_LOW );

	TResource::EResState	GetResState( uint nResGUID ) const;
	void					RemoveRes( uint nQueueID );

	void		Update		();

	//EResult QueueTexture( const char * szFileName, PTexture & pOutTexture );
	
	PTexture	LoadTexture	( const char * szFileName );

	//=== CResManager ==========================================================

	static DWORD WINAPI LoadThread( LPVOID lpParameter );

	private:

		//! ������� ���������� ��� ������� ������� ������ �� ����� �����
		IResCreator * DetectCreator( const char * szFileName );

		//! ������� ���������� ��� ������� ������� ������ �� ������� �������
		IResCreator * DetectCreator( TMemChunk & tMemory );

		void UpdateResQueue();
		void CreateTextureFromRes( TResource * pRes );

		CVector <IResCreator *>		m_pCreators;

		bool			m_bThreadWork;
		HANDLE			m_hLoadThread;
		DWORD			m_dwLoadThread;
		CVector <TResource *>	m_ResQueue;	///< ������� ��������. �������� ������ �� ������� ������

		CVector <TResource *>		m_LoadedRes;

		CCriticalSec			m_NewResSection;
		CVector <TResource *>	m_pDemands;

		CCriticalSec			m_ReadySection;
		CVector <TResource *>	m_ResReady;

		///

		uint m_nMaxResSlots;
		CContainer <TResource *> m_ResSlots;
};

extern IResManager * g_pResManager;