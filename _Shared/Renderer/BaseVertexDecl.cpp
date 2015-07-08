#include "Common.h"
#include "BaseVertexDecl.h"




CBaseVertexDecl::CBaseVertexDecl() :
	m_nStride		( 0 )//,
//	m_bRegistred	( false )
{
}


/**
*
*/
CBaseVertexDecl::~CBaseVertexDecl()
{
	/*for ( uint n = 0; n < m_pAttrs.GetCount(); ++n )
	{
		PURE_DEL( m_pAttrs[ n ] );
	}*/

	m_pAttrs.Delete();
}


/**
*
*/
uint CBaseVertexDecl::GetStride() const
{
	return m_nStride;
}


/**
*
*/
void CBaseVertexDecl::SetStride( uint nStride )
{
	if ( nStride < m_nStride )	
	{
		// Вроде как невалидный шаг
		DEBUG_ASSERT( false );
	}

	m_nStride = nStride;
}


/**
*
*/
void CBaseVertexDecl::AddAttr( const char * szName, const char * szSemantics, ETypeID eType, uint nOffset )
{
	if ( !szName )
		return;

	if ( !(*szName) )
		return;

	if ( CStr::GetSize(szName) >= MAX_NAME_LEN )
	{
		// Длинна имени аттрибута длиннее MAX_NAME_LEN
		DEBUG_ASSERT( false );
		return;
	}

	TVertexAttr tAttr;

	strcpy( tAttr.szName, szName );
	tAttr.eType		= eType;
	tAttr.nOffset	= nOffset;

	if ( INVALID_INDEX == m_pAttrs.AddToTail( tAttr ) )
	{
		return;
	}

	uint nStride = nOffset + g_pSystem->GetSizeOf( eType );

	if ( m_nStride < nStride )
	{
		m_nStride = nStride;
	}
}


/**
*
*/
const TVertexAttr * CBaseVertexDecl::GetAttr( uint nAttr ) const
{
	if ( nAttr < m_pAttrs.GetCount() )
	{
		return &m_pAttrs[ nAttr ];
	}

	return NULL;
}


/**
*
*/
uint CBaseVertexDecl::GetAttrCount() const
{
	return m_pAttrs.GetCount();
}


/**
*
*
bool CBaseVertexDecl::Register()
{
/*	if ( m_bRegistred )
		return true;
	/*
	//--------------------------------------------------------------------------
	// Tip: Декларируем формат вершин
	//--------------------------------------------------------------------------
	for ( uint n = 0; n < GetAttrCount(); ++n )
	{
		TAttr & tAttr = m_pAttrs[ n ];

		GLint nLoc = INVALID_INDEX;//glGetAttribLocationARB( g_pRenderer->GetCurrentProgram(), tAttr.tAttr.szName );
					//GL_VALIDATE;

		//if ( INVALID_INDEX != nLoc )
		//{
//			tAttr.nLoc			= nLoc;

//		if ( INVALID_INDEX != nLoc )
		{
			tAttr.nElemCount	= GetElementCount( tAttr.tAttr.eType );

			switch ( tAttr.tAttr.eType )
			{
				case TYPE_VEC2:
				case TYPE_VEC3:
				case TYPE_VEC4:
					tAttr.eType = GL_FLOAT;
				break;

				case TYPE_DWORD:
					tAttr.eType = GL_UNSIGNED_BYTE;
				break;

				default:
				{
					DEBUG_ASSERT( !"unknown type" );
				}
			
		}
	}
	*
//	m_bRegistred = true;
	return true;
}*/

//////////////////////////////////////////////////////////////////////////