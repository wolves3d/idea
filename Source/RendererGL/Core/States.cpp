////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
void CRenderer_GL::AlphaRef( float fRef )
{
	m_fAlphaRef = fRef;

	glAlphaFunc( m_AlphaFuncTable[ m_eAlphaFunc ], m_fAlphaRef );
}


/**
*
*/
void CRenderer_GL::AlphaFunc( EAlphaFunc eFunc )
{
	m_eAlphaFunc = eFunc;

	glAlphaFunc( m_AlphaFuncTable[ m_eAlphaFunc ], m_fAlphaRef );
}


/**
*
*/
bool CRenderer_GL::BlendFunc( EBlendFactor eSrc, EBlendFactor eDst )
{
	DEBUG_ASSERT( ( eSrc >= 0 ) && ( eSrc < BF_MAX_FACTORS ) );
	DEBUG_ASSERT( ( eDst >= 0 ) && ( eDst < BF_MAX_FACTORS ) );

	glBlendFunc( m_BlendFactorTable[ eSrc ], m_BlendFactorTable[ eDst ] );

	return false;
}


/**
*
*/
void CRenderer_GL::EnableAlphaTest( bool bEnable )
{
	if ( bEnable )
	{
		glEnable( GL_ALPHA_TEST );
	}
	else
	{
		glDisable( GL_ALPHA_TEST );
	}
}


/**
*
*/
void CRenderer_GL::EnableBlend( bool bEnable )
{
	if ( bEnable )
	{
		glEnable( GL_BLEND );
	}
	else
	{
		glDisable( GL_BLEND );
	}
}


/**
*
*/
void CRenderer_GL::EnableDepthTest( bool bEnable )
{
	if ( bEnable )
	{
		glEnable( GL_DEPTH_TEST );
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}
}


/**
*
*/
void CRenderer_GL::DepthMask( bool bMask )
{
	glDepthMask( bMask );
}