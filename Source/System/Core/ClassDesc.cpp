////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/**
*
*/
class CClassDesc : public IClassDesc
{
	public:

		CClassDesc( const char * szName, void * pFunc )
		{
			m_sName = szName;
			m_pFunc = pFunc;
		}

		void DeclField( const char * szType, const char * szName, uint nOffset )
		{
			TField * pField = NEW TField;

			if ( pField )
			{
				pField->eType	= g_pSystem->GetType( szType );
				pField->sName	= szName;
				pField->nOffset	= nOffset;

				m_Fields.Push( pField );
			}
		}

		const char * GetName()
		{
			return m_sName.GetString();
		}

	private:

		struct TField
		{
			ETypeID eType;
			CStr	sName;
			uint	nOffset;
		};

		CStr				m_sName;
		void *				m_pFunc;
		CStack <TField *>	m_Fields;
};


/**
*
*/
IClassDesc * CSystem::CreateClassDesc( const char * szName, void * pFunc )
{
	IClassDesc * pDesc = NEW CClassDesc( szName, pFunc );

	if ( pDesc )
	{
		m_Classes.Push( pDesc );
	}

	return pDesc;
}


/**
*
*/
void * CSystem::CreateClassInst( const char * szName )
{
	for ( uint i = 0; i < m_Classes.GetCount(); ++i )
	{
		IClassDesc * pDesc = m_Classes[ i ];

		if ( CStr::Compare( szName, pDesc->GetName() ) )
		{
			return pDesc;
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////