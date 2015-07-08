////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Templates/HashTable.h"

//!

//!
//!
class CVar : public IVar
{
	public :

	//!

	//!
	//!
	CVar() :
		eType( TYPE_INT )
	{
	}


	//!

	//!
	//!
	bool SetString	( const char * szString )
	{
		if ( TYPE_STRING != eType )
		{
			if ( pStringVal	= NEW CStr( szString ) )
			{
				eType = TYPE_STRING;
			}
			else
			{
				return false;
			}
		}

		return pStringVal->Set( szString );
	}


	//!

	//!
	//!
	const char * GetString	() const
	{
		if ( TYPE_STRING == eType )
			return pStringVal->GetString();

		DEBUG_MSG( "Invalid variable type!" );
		return NULL;
	}

	CStr	sName;
	ETypeID eType;

	union
	{
		int		nIntegerVal;
		float	fFloatVal;
		CStr *	pStringVal;
	};
};


//!

//!
//!
class CVarStorage : public IVarStorage
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:


	//!

	//!
	//!
	CVarStorage()
	{
		m_pVarTable.Init();
	}


	//!

	//!
	//!
	IVar * RegVar( const char * szVarName )
	{
		if ( szVarName )
		{
			CVar * pVar = NEW CVar;

			if ( pVar )
			{
				pVar->sName = szVarName;

				uint nID = m_pVars.Add( pVar );
				
				if ( INVALID_INDEX != nID )
				{
					m_pVarTable.AddItem( szVarName, nID );
					return pVar;
				}
				else
				{
					DEL( pVar );
				}
			}
		}

		return NULL;
	}


	//!

	//!
	//!
	IVar * GetVar( const char * szVarName )
	{
		uint nID;

		if ( m_pVarTable.GetItem( szVarName, nID ) )
		{
			return m_pVars[ nID ];
		}

		return NULL;
	}


	//!

	//!
	//!
	uint GetVarID( const char * szVarName ) const
	{
		uint nID = INVALID_INDEX;
		m_pVarTable.GetItem( szVarName, nID );
		return nID;
	}


	//!

	//!
	//!
	const char * GetVarName( uint nVar ) const
	{
		if ( m_pVars.IsValid( nVar ) )
		{
			return m_pVars[ nVar ]->sName.GetString();
		}

		return NULL;
	}


	//!

	//!
	//!
	ETypeID GetVarType( uint nVar ) const
	{
		if ( m_pVars.IsValid( nVar ) )
		{
			return m_pVars[ nVar ]->eType;
		}

		DEBUG_MSG( "Invalid variable index!" );
		return INVALID_TYPE;
	}


	//!

	//!
	//!
	bool SetFloat( uint nVar, float	fVal )
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return false;
		}

		CVar * pVar = m_pVars[ nVar ];

		if ( pVar->eType == TYPE_STRING )
		{
			DEL( pVar->pStringVal );
		}

		pVar->eType		= TYPE_FLOAT;
		pVar->fFloatVal	= fVal;

		return true;
	}


	//!

	//!
	//!
	bool SetInt( uint nVar, int	nVal )
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return false;
		}

		CVar * pVar = m_pVars[ nVar ];

		if ( pVar->eType == TYPE_STRING )
		{
			DEL( pVar->pStringVal );
		}

		pVar->eType			= TYPE_INT;
		pVar->nIntegerVal	= nVal;

		return true;
	}


	//!

	//!
	//!
	bool SetString( uint nVar, const char *szString )
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return false;
		}

		CVar * pVar = m_pVars[ nVar ];

		// Если раньше тип переменной был отличен от TYPE_STRING...
		if ( pVar->eType != TYPE_STRING )
		{
			// ...То создаём указатель на объект строки
			if ( pVar->pStringVal = NEW CStr( szString ) )
			{
				// меняем тип переменной на TYPE_STRING
				pVar->eType = TYPE_STRING;
				return true;
			}
			else
			{
				// Произошла ошибка при создании строки
				return false;
			}
		}
		
		// Переменная имеет тип строки, просто меняем значение
		return pVar->pStringVal->Set( szString );
	}


	//!

	//!
	//!
	float GetFloat( uint nVar ) const
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return 0.f;
		}

		CVar * pVar = m_pVars[ nVar ];

		if ( TYPE_FLOAT == pVar->eType )
		{
			return pVar->fFloatVal;
		}
		else if ( TYPE_INT == pVar->eType )
		{
			return (float)pVar->nIntegerVal;
		}
		
		DEBUG_MSG( "Invalid variable type!" );
		return 0.f;
	}


	//!

	//!
	//!
	int GetInt( uint nVar ) const
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return 0;
		}

		CVar * pVar = m_pVars[ nVar ];

		if ( TYPE_INT == pVar->eType )
		{
			return pVar->nIntegerVal;
		}
		else if ( TYPE_FLOAT == pVar->eType )
		{
			return (int)pVar->fFloatVal;
		}

		DEBUG_MSG( "Invalid variable type!" );
		return 0;
	}


	//!

	//!
	//!
	const char * GetString( uint nVar ) const
	{
		if ( !m_pVars.IsValid( nVar ) )
		{
			DEBUG_MSG( "Invalid variable index!" );
			return NULL;
		}

		CVar * pVar = m_pVars[ nVar ];

		if ( TYPE_STRING == pVar->eType )
		{
			return pVar->pStringVal->GetString();
		}

		DEBUG_MSG( "Invalid variable type!" );
		return NULL;
	}


	//==========================================================================
	// Tip: Private Methods & Fields
	//==========================================================================
	private:

	CContainer <CVar *> m_pVars;
	CHashTable <uint>	m_pVarTable;
};


//!

//!
//!
bool CSystem::CreateVarStorage()
{
	return ( NULL != ( g_pVarStorage = NEW CVarStorage ) );
}


//!

//!
//!
IVarStorage * CSystem::GetVarStorage	()
{
	return g_pVarStorage;
}

////////////////////////////////////////////////////////////////////////////////