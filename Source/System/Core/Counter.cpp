////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/**
*
*/
class CCounter : public ICounter
{
	public :

	CCounter( const char * szName )
	{
		m_sName = szName;

		ResetTime();
		ResetValue();
	}

	const char * GetName() const
	{
		return m_sName.GetString();
	}

	//--------------------------------------------------------------------------
	//	Tip: Time functionality
	//--------------------------------------------------------------------------
	float GetTime() const
	{
		return m_fTime;
	}

	void Activate()
	{
	}

	void Deactivate()
	{
	}

	void ResetTime()
	{
		m_fTime	= 0.f;
	}

	//--------------------------------------------------------------------------
	//	Tip: Counter ( number value ) functionality
	//--------------------------------------------------------------------------
	uint GetValue() const
	{
		return m_nValue;
	}

	void IncValue( size_t nInc = 1 )
	{
		m_nValue += nInc;
	}

	void ResetValue()
	{
		m_nValue = 0;
	}

	private:

	CStr	m_sName;
	float	m_fTime;
	size_t	m_nValue;
};


/**
*
*/
ICounter * CSystem::AddCounter( const char * szName )
{
	if ( !szName )
	{
		DEBUG_MSG( DBG_STR_NULL_ARG )
		return NULL;
	}
	
	CCounter * pCounter = NEW CCounter( szName );
	
	if ( pCounter )
	{
		m_Counters.Push( pCounter );
	}
	else
	{
		DEBUG_MSG( DBG_STR_OUT_OF_MEM )
	}

	return pCounter;
}


/**
*
*/
ICounter * CSystem::GetCounter( uint nCounter )
{
	if ( nCounter < m_Counters.GetCount() )
		return m_Counters[ nCounter ];

	return NULL;
}


/**
*
*/
uint CSystem::GetCounterCount()
{
	return m_Counters.GetCount();
}


/**
*
*/
void CSystem::RemoveCounter( ICounter * pCounter )
{

}


/**
*
*/
void CSystem::StartCount( ICounter * pCounter )
{

}


/**
*
*/
void CSystem::StopCount( ICounter * pCounter )
{

}

////////////////////////////////////////////////////////////////////////////////