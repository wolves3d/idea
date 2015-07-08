////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

#include "_Shared/Math/3dmath.h"
#include "Core/Input.h"
#include "tinyXML/XMLWrapper.h"
#include "Core/Sound/SoundMgr.h"

////////////////////////////////////////////////////////////////////////////////

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CVfsLibInfo : public ILibrary
{
	public :

		//======================================================================
		//
		//======================================================================
		inline CVfsLibInfo() :
			m_nNumInterfaces( 4 )
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
					case 0: return &CVfs::ms_ClassDesc;
					case 1: return &CInput::ms_ClassDesc;
					case 2: return &CXMLWrapper::ms_ClassDesc;
					case 3: return &CSoundMgr::ms_ClassDesc;
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
				case 0: return NEW CVfs;
				case 1: return NEW CInput;
				case 2: return NEW CXMLWrapper;
				case 3: return NEW CSoundMgr;
			}

			return NULL;
		}

	private :

		//======================================================================
		//
		//======================================================================
		size_t m_nNumInterfaces;

} * g_pLibrary;


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
extern "C"
{
	//==========================================================================
	//
	//==========================================================================
	__declspec (dllexport) ILibrary * InitLibrary( ISystem * pSystem )
	{
		if ( !g_pLibrary )
		{
			g_pSystem	= pSystem;
			g_pLibrary	= NEW CVfsLibInfo;
		}

		return g_pLibrary;
	}

	//==========================================================================
	//
	//==========================================================================
	__declspec (dllexport) void UninitLibrary()
	{
		if ( g_pLibrary )
		{
			DEL( g_pLibrary	);
			g_pSystem	= NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////