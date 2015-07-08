////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/UnitTest.h"

/**
*
*/
class CTestRes : public ITestRes
{

public:

	/**
	*
	*/
	CTestRes() :
		nSuccessCount	( 0 ),
		nFailCount		( 0 ),
		m_bTestOK		( true ),
		m_pOutFile		( NULL )
	{
	}

	/**
	*
	*/
	void Init( const char * szLogFile )
	{
		if ( NULL == szLogFile )
			szLogFile = "unittest.txt";

		m_pOutFile = fopen( szLogFile, "wt" );
	}

	/**
	*
	*/
	void Release()
	{
		if ( m_pOutFile )
		{
			fprintf( m_pOutFile, "\n\n%d test(s) succeded", nSuccessCount );

			if ( nFailCount )
				fprintf( m_pOutFile, "\n%d test(s) filed", nFailCount );

			fclose( m_pOutFile );
		}
		
		PURE_DEL( this );
	}
	
	/**
	*
	*/
	void Assert( const char * szTest, const char * szFile, int nLine )
	{
		DEBUG_ASSERT( NULL != m_pOutFile );

		if ( m_pOutFile )
		{
			fprintf( m_pOutFile, "\"%s\" assertion failed in file \"%s\", line %d", szTest, szFile, nLine );
			fflush( m_pOutFile );
		}

		m_bTestOK = false;
	}

	/**
	*
	*/
	void Fail( const char * szTest )
	{
		DEBUG_ASSERT( NULL != m_pOutFile );

		if ( m_pOutFile )
		{
			fprintf( m_pOutFile, "Abnormal termination \"%s\"\n", szTest );
			fflush( m_pOutFile );
		}

		m_bTestOK = false;
	}

	/**
	*
	*/
	bool Next()
	{
		if ( m_bTestOK )
		{
			nSuccessCount++;
			return true;
		}

		m_bTestOK = true;
		nFailCount++;

		return false;
	}


	/**
	*
	*/
	bool Status() const
	{
		return ( 0 == nFailCount );
	}

	/*bool Run()
	{
		uint nTotalCount	= m_pTests.GetCount();
		uint nFailedCount	= 0;

		m_pOutFile = fopen( "unittest.txt", "wt" );

		for ( uint n = 0; n < m_pTests.GetCount(); ++n )
		{
			__try
			{
				if ( false == m_pTests[ n ]->runTest( this ) )
				{
					fprintf( m_pOutFile, "\n%s failed", m_pTests[ n ]->szTestName );
					nFailedCount++;
				}
			}
			__except( EXCEPTION_EXECUTE_HANDLER  )
			{
				fprintf( m_pOutFile, "test failed!\n" );
				nFailedCount++;
			}
		}

		fprintf( m_pOutFile, "\n\n%d test(s) succeded", m_pTests.GetCount() - nFailedCount );

		if ( 0 != nFailedCount )
		{
			DEBUG_MSG( "Unit test failed" );
			fprintf( m_pOutFile, "\n%d test(s) filed", nFailedCount );
		}

		fclose( m_pOutFile );
		m_pOutFile = NULL;
		return true;
	}*/

private:

	FILE *	m_pOutFile;
	bool	m_bTestOK;
	uint	nSuccessCount;
	uint	nFailCount;
};


/**
*
*/
ITestRes * CSystem::CreateTestRes()
{
	return NEW CTestRes;
}