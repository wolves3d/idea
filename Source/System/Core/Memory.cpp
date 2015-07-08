////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "malloc.h"
#include "StackWalker/StackWalker.h"



class CPointerList
{
	public:

		CPointerList() :
			m_pRoot( NULL ),
			m_pTail( NULL )
		{
		}

		inline void AddPointer( void * pPtr, size_t nSize, const char * szFile, int nLine )
		{
			if ( !m_pRoot )
			{
				m_pRoot = (node_t *)malloc( sizeof( node_t ) );
				m_pTail = m_pRoot;
				goto fill_tail;
			}

			if ( m_pTail )
			{
				if ( m_pTail->pNext = (node_t *)malloc( sizeof( node_t ) ) )
				{
					m_pTail = m_pTail->pNext;
					goto fill_tail;
				}
			}

			return;

			fill_tail:
				m_pTail->pNext= NULL;
				m_pTail->pPtr = pPtr;
				m_pTail->nSize= nSize;
				m_pTail->szFile= NULL;
				m_pTail->nLine= nLine;

				if ( szFile )
				{
					if ( m_pTail->szFile = (char *)malloc( strlen( szFile ) + 1 ) )
					{
						strcpy( m_pTail->szFile, szFile );
					}
				}
		}

		inline void RemovePointer( void * pPtr )
		{
			node_t * pPrev = NULL;
			node_t * pCur = m_pRoot;

			while ( pCur )
			{
				if ( pPtr == pCur->pPtr )
				{
					if ( pPrev )
					{
						pPrev->pNext = pCur->pNext;

						if ( pCur == m_pTail )
							m_pTail = pPrev;
					}
					else
					{
						m_pRoot = pCur->pNext;

						if ( pCur == m_pTail )
							m_pTail = m_pRoot;
					}

					free( pCur->szFile );
					free( pCur );
					return;
				}

				pPrev = pCur;
				pCur = pCur->pNext;
			}

			DEBUG_ASSERT( !"Bad pointer!" )
		}

		inline void DumpLeaks()
		{
			FILE * f = fopen( "leaks.txt", "wt" );

			if ( NULL == f )
				return;

			node_t *	pTemp;
			size_t		nTotalSize = 0;

			while ( m_pRoot )
			{
				pTemp = m_pRoot;
				m_pRoot = m_pRoot->pNext;

				//fprintf( f, "%p ( %d bytes ) allocated in %s, line %d\n",
				//	pTemp->pPtr, pTemp->nSize, pTemp->szFile, pTemp->nLine );

				fprintf( f, "%p ( %d bytes ) allocated at\n%s, line %d\n",
					pTemp->pPtr, pTemp->nSize, pTemp->szFile, pTemp->nLine );

				nTotalSize += pTemp->nSize;

				free( pTemp->pPtr );
				free( pTemp->szFile );
				free( pTemp );
			}

			fprintf( f, "\n\nTotal leak: %d bytes", nTotalSize );
			fclose( f );
		}

	private:

		struct node_t
		{
			void * pPtr;
			size_t nSize;
			char * szFile;
			int nLine;

			node_t * pNext;
		} * m_pRoot, * m_pTail;
};

CPointerList tList;

extern StackWalker * pSW;

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void *CSystem::Alloc( size_t nSize, const char * szFileName, int nLine )
{
	DEBUG_ASSERT( nSize );

	void * pResult = malloc( nSize );
	
	if ( pResult )
	{
		if ( m_pMemCounter )
			m_pMemCounter->IncValue( nSize );

		if ( pSW )
		{
			/////////////// call stack ///////////////

			#define MaxStr 1024
			char	pCallStack[ MaxStr ];
			memset( pCallStack, 0, MaxStr );

			pSW->ShowCallstack( GetCurrentThread(), NULL, NULL, NULL, pCallStack, MaxStr );
			tList.AddPointer( pResult, nSize, pCallStack, nLine );

			/////////////// end call stack ///////////////
		}
		else
		{
			tList.AddPointer( pResult, nSize, szFileName, nLine );
		}
	}

	return pResult;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CSystem::Delete( void * pData )
{
	DEBUG_ASSERT( pData );

	if ( pData )
	{
		tList.RemovePointer( pData );

		free( pData );
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CSystem::DumpLeaks()
{
	tList.DumpLeaks();
}

////////////////////////////////////////////////////////////////////////////////