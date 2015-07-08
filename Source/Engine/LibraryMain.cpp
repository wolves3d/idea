////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ResourceManager/ResourceManager.h"

////////////////////////////////////////////////////////////////////////////////

ISystem * g_pSystem = NULL;

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CEngineLibInfo : public ILibrary
{
	public :

		CEngineLibInfo() :
		   m_nNumInterfaces( 2 )
		{
		}

		//======================================================================
		//
		//======================================================================
		size_t GetNumInterfaces() const
		{
			return m_nNumInterfaces;
		}

		//======================================================================
		//
		//======================================================================
		const TIntfDesc * GetInterfaceDesc( size_t nInterfaceID ) const
		{
			if ( nInterfaceID < m_nNumInterfaces )
			{
				switch ( nInterfaceID )
				{
					case 0: return &CEngine::ms_ClassDesc;
					case 1: return &CResManager::ms_ClassDesc;
				}
			}

			return NULL;
		}

		//======================================================================
		//
		//======================================================================
		IBaseIntf * CreateInterface( int nInterfaceID )
		{
			switch ( nInterfaceID )
			{
				case 0:

					if ( !g_pEngine )
						g_pEngine = NEW CEngine;

				return g_pEngine;

				case 1: return NEW CResManager;
			}

			return NULL;
		}

	private :

		//======================================================================
		//
		//======================================================================
		size_t m_nNumInterfaces;

} * g_pLibrary;

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	/*class ENGINE_API CVova : public CBaseControl
	{
		public:

	};*/

	//==========================================================================
	//
	//==========================================================================
	ENGINE_API ILibrary * InitLibrary( ISystem * pSystem )
	{
		if ( !g_pLibrary )
		{
			g_pSystem	= pSystem;
			g_pLibrary	= NEW CEngineLibInfo;
		}

		return g_pLibrary;
	}

	//==========================================================================
	//
	//==========================================================================
	ENGINE_API void UninitLibrary()
	{
		if ( g_pLibrary )
		{
			DEL( g_pLibrary	);
			g_pSystem	= NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////