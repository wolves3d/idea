////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "EasyWin/EasyWin.h"


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CSystem::DestroyWin( void )
{
	if ( m_pWin )
	{
		m_pWin->Destroy();
		DEL( m_pWin );
	}
}

////////////////////////////////////////////////////////////////////////////////