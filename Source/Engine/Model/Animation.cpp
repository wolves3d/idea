////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/*
*
*/
class CAnimation
{
public:

	uint			GetNumObjects	();
	const char *	GetObjectName	( uint nObject );

	void			RebuildMatrices	( uint nFrame );
	const mat4 *	GetMatrices		();

private:

	CStr m_sName;
	uint m_nNumObjects;

	// CStr m_pObjNameTable	[ nNumObjects ];
	// TKey m_pKeys			[ nNumObjects ];
	// mat4 m_pMatrices		[ nNumObjects ];
};


/*
*
*
CAnimation * CEngine::LoadAnimation( const char * szFileName )
{
	CAnimation * pAnim = NEW CAnimation;
}
*/