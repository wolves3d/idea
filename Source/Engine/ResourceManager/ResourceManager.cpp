////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ResourceManager.h"


//!

//!
//!
void RegisterTargaLoader();
void RegisterShaderCfgLoader();


//!

//!
//!
TIntfDesc CResManager::ms_ClassDesc = 
{
	4321,
	IT_RES_MGR,
	1,
	"Resource manager by WW v0.8"
};


//!

//!
//!
DWORD CResManager::LoadThread( LPVOID lpParameter )
{
	CResManager * pMgr = (CResManager *)lpParameter;

	while( pMgr->m_bThreadWork )
	{
		pMgr->UpdateResQueue();
		Sleep( 10 );
	}

	return 0;
}


/**
*
*/
CResManager::CResManager() :
	m_nMaxResSlots( 10000 )
{
	g_pResManager = this;

	m_ResSlots.Resize( m_nMaxResSlots );
	m_pCreators.Resize( 5, false );

	RegisterTargaLoader();
	RegisterShaderCfgLoader();

	m_bThreadWork = true;
	m_hLoadThread = CreateThread( NULL, 0, LoadThread, this, 0, &m_dwLoadThread );
}


/**
*
*/
CResManager::~CResManager()
{
	g_pResManager = NULL;

	m_bThreadWork = false;
	WaitForMultipleObjects( 1, &m_hLoadThread, TRUE, INFINITE );
	CloseHandle( m_hLoadThread );

	index_t nID = INVALID_INDEX;

	for ( uint n = 0; n < m_ResReady.GetCount(); ++n )
	{
		TResource * pRes = m_ResReady[ n ];
		DEL_ARRAY( pRes->pBuffer );
		DEL( pRes );
	}

	for ( uint n = 0; n < m_ResQueue.GetCount(); ++n )
	{
		TResource * pRes = m_ResQueue[ n ];
		g_pVfs->CloseFile( (IFile **)&pRes->pFile );
		DEL_ARRAY( pRes->pBuffer );
		DEL( pRes );
	}

	/*for ( index_t nID = INVALID_INDEX; m_ResSlots.Iterate( nID ); )
	{
		DEL( m_ResSlots[ nID ] );
	}*/

	m_ResReady.Delete();
	m_ResQueue.Delete();
	m_pCreators.Delete(); // mem leak?
	m_ResSlots.Delete();
}


//!

//!
//!
void CResManager::RegCreator( IResCreator * pCreator )
{
	if ( NULL == pCreator )
		return;

	m_pCreators.AddToTail( pCreator );
}


//!

//!
//!
bool CResManager::Request( const char * szFile, TResource ** ppOutResource )
{
	// FixMe: TEST FOR PRIMARY THREAD!!!

	uint nResGUID = Demand( szFile, ppOutResource, ERP_HIGH );

	if ( INVALID_INDEX != nResGUID )
	{
		while ( true )
		{
			TResource::EResState eState = GetResState( nResGUID );

			if ( TResource::FAILED == eState )
				return false;

			Update(); // FixMe: MUST CALL FROM PRIMARY THREAD!!!
			Sleep( 0 );

			if ( ( TResource::READY_TO_USE == eState ) || ( TResource::LOADED == eState ) )
				return true;
		}
	}

	return false;
}


//!

//!
//!
TResource::EResState CResManager::GetResState( uint nResGUID ) const
{
	if ( m_ResSlots.IsValid( nResGUID ) )
	{
		return m_ResSlots[ nResGUID ]->eState;
	}
	
	return TResource::INVALID;
}




void CResManager::RemoveRes( uint nQueueID )
{
	
}





//!

//!
//!
uint CResManager::Demand( const char * szFile, TResource ** ppOutResource, EResPriority ePriority )
{	
	// detect resource type ( select right IResCreator )
	//IResCreator * pTexCreator = m_pCreators[ 0 ]; // hack!
	IResCreator * pTexCreator = DetectCreator( szFile );

	if ( NULL == pTexCreator )
	{
		DEBUG_MSG( "unknown res type!" );
		return INVALID_INDEX;
	}

	// create new resource instance
	TResource * pRes = (*ppOutResource);
	uint nSlot = INVALID_INDEX;

	// Создаем экземпляр ресурса только если он не задан явно -----------------

	if ( NULL == pRes )
		pTexCreator->CreateResInstance( &pRes );

	if ( pRes && pRes->pResource )
	{
		(*ppOutResource) = pRes;
		pRes->pCreator = pTexCreator;
		pRes->sFileName.Set( szFile );

		// Добавляем новый ресурс в очередь
		m_NewResSection.Enter();
		m_pDemands.AddToTail( pRes );
		m_NewResSection.Leave();

		nSlot = m_ResSlots.Add( pRes );
		pRes->eState = TResource::QUEUED;
	}

	return nSlot;
}


//! @detailed Метод по расширению файла подбирает подходящий загрузчик
//! @param szFileName Указатель на имя файла
//! @return Указатель на загрузчик или NULL если подходящий не найден
IResCreator * CResManager::DetectCreator( const char * szFileName )
{
	for ( uint n = 0; n < m_pCreators.GetCount(); ++n )
	{
		IResCreator * pCreator	= m_pCreators.GetItem( n );
		const IResCreatorInfo * pInfo	= pCreator->GetInfo();

		if ( pInfo && pInfo->szExtension )
		{
			if ( CStr::FinishWith( szFileName, pInfo->szExtension ) )
				return pCreator;
		}
	}

	return NULL;
}


//!
//!
IResCreator * CResManager::DetectCreator( TMemChunk & tMemory )
{
	return NULL;
}


/**
*
*/
void CResManager::UpdateResQueue()
{
	// Добавляем новые запросы в поток загрузки --------------------------------

	m_NewResSection.Enter();

		for ( uint n = 0; n < m_pDemands.GetCount(); ++n )
		{
			m_ResQueue.AddToTail( m_pDemands[ n ] );
		}

		m_pDemands.Clear();

	m_NewResSection.Leave();


	// Выходим если нет заданий ------------------------------------------------

	if ( 0 == m_ResQueue.GetCount() )
		return;

	// Update ------------------------------------------------------------------

	size_t nLimit = 4096 * 50;

	while ( ( nLimit > 0 ) && m_ResQueue.GetCount() )
	// За один вызов разрешается считать не более nLimit байт
	{
		TResource * pRes = m_ResQueue[ 0 ];

		size_t nBlockSize = 4096;

		if ( NULL == pRes->pFile )
		// Если файл ещё не открыт, то
		{
			// Открываем файл
			g_pVfs->OpenFile( (IFile **)&pRes->pFile, pRes->sFileName, VFS_READ | VFS_BINARY );

			if ( NULL == pRes->pFile )
			// Если не удаётся открыть файл, то
			{
				pRes->eState = TResource::FAILED;

				// Удаляем ресурс из очереди
				m_ResQueue.Remove( 0 );

				// Продолжаем цикл обработки ресурсов со следующего элемента
				continue;
			}
		
			// После того как был открыт файл инициализируем ресурс
			pRes->pCreator->InitRes( pRes, ((IFile *)pRes->pFile)->GetSize()  );

			if ( NULL == pRes->pBuffer )
			// Если провалилась инициализация
			{
//				pRes->eState = TResource::FAILED;

				// Удаляем ресурс из очереди
				m_ResQueue.Remove( 0 );

				// Продолжаем цикл обработки ресурсов со следующего элемента
				continue;
			}
		}

		// Читаем файл по частям -----------------------------------------------

		if ( nBlockSize > pRes->nSize )
		{
			nBlockSize = (uint)pRes->nSize;
		}

		if ( pRes->nReadySize )
		{
			if ( ( nBlockSize + pRes->nReadySize ) > pRes->nSize )
				nBlockSize = (int)(pRes->nSize - pRes->nReadySize);
		}

		size_t nRead = g_pVfs->Read(
			(IFile *)pRes->pFile,
			pRes->pBuffer + pRes->nReadySize,
			nBlockSize );

		pRes->nReadySize += nBlockSize;
		nLimit -= nBlockSize;

		if ( pRes->nSize == pRes->nReadySize )
		{
			/*
			size_t nMoreBytes = pRes->pCreator->UpdateRes(
				pRes,
				pRes->nReadySize );
				

			if ( 0 == nMoreBytes ) */
			// Больше нет надобности читать файл
			{
				g_pVfs->CloseFile( (IFile **)&pRes->pFile );

				m_LoadedRes.AddToTail( pRes );

				// Удаляем ресурс из очереди
				m_ResQueue.Remove( 0 );
			}
		}
	}

	// Убираем отработанные запросы из очереди загрузки ------------------------

	m_ReadySection.Enter();

		for ( uint n = 0; n < m_LoadedRes.GetCount(); ++n )
		{
			m_ResReady.AddToTail( m_LoadedRes[ n ] );
		}

	m_ReadySection.Leave();

	m_LoadedRes.Clear();
}


//!

//!
//!
void CResManager::Update()
{
	// В начале нужно удалить ресурс из очереди, т.к. мы можем рекурсивно
	// вернуться сюда при выполнении FinalizeRes
	// В результате, ресурс может быть финализирован несколько раз (deadlock)

	CVector <TResource *> resLocalCopy;
	uint nRes;

	m_ReadySection.Enter();

		nRes = m_ResReady.GetCount();

		while ( nRes-- )
		{
			resLocalCopy.AddToTail( m_ResReady.GetItem( nRes ) );
		}

		m_ResReady.Clear();

	m_ReadySection.Leave();

	//-----------

	nRes = resLocalCopy.GetCount();

	for ( uint n = 0; n < nRes; ++n )
	{
		TResource * pRes = resLocalCopy[ n ];		
		pRes->pCreator->FinalizeRes( pRes );
		//DEL( pRes );
	}

	resLocalCopy.Delete();
}

////////////////////////////////////////////////////////////////////////////////