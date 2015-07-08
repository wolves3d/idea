////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Renderer/BaseVertexDecl.h"

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
class CVertexDecl : public CBaseVertexDecl
{
	public:
		/*
	// IVertexDecl /////////////////////////////////////////////////////////////
	uint	GetVertexCount	();
	void	SetVertexCount	( uint nCount );

	uint	GetStride	() const;
	void	SetStride	( uint nStride );
	void	AddAttr		( const char * szName, const char * szSemantics, ETypeID eType, uint nOffset );
	const TVertexAttr *	GetAttr		( uint nAttr ) const;
	uint	GetAttrCount() const;

	EPrimitive	GetPrimitiveType();
	void		SetPrimitiveType( EPrimitive ePrim );

	/**
	*
	*/
	void Assign( bool bAssign ) {}
	bool Register();
	/*
	// CVertexDecl /////////////////////////////////////////////////////////////
			CVertexDecl	();
			~CVertexDecl();

	private:

	uint		m_nStride;
	uint		m_nVertexCount;


	CVector <TVertexAttr *> m_pAttrs;
	*/
};

////////////////////////////////////////////////////////////////////////////////

/**
*
*
CVertexDecl::CVertexDecl() :
	m_nStride		( 0 ),
	m_nVertexCount	( 0 )
//	m_ePrimType		( PRIM_POINT )
{
}


/**
*
*
CVertexDecl::~CVertexDecl()
{
	for ( uint n = 0; n < m_pAttrs.GetCount(); ++n )
	{
		PURE_DEL( m_pAttrs[ n ] );
	}

	m_pAttrs.Delete();
}


/**
*
*
uint CVertexDecl::GetStride() const
{
	return m_nStride;
}


/**
*
*
void CVertexDecl::SetStride( uint nStride )
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
*
void CVertexDecl::AddAttr( const char * szName, const char * szSemantics, ETypeID eType, uint nOffset )
{
	if ( !szName )
		return;

	if ( !(*szName) )
		return;

	if ( ( strlen( szName ) + 1 ) >= MAX_NAME_LEN )
	{
		// Длинна имени аттрибута длиннее MAX_NAME_LEN
		DEBUG_ASSERT( false );
		return;
	}

	TVertexAttr * pAttr = NEW TVertexAttr;

	if ( pAttr )
	{
		strcpy( pAttr->szName, szName );
		pAttr->eType	= eType;
		pAttr->nOffset	= nOffset;

		if ( INVALID_INDEX == m_pAttrs.AddToTail( pAttr ) )
		{
			DEL( pAttr );
			return;
		}

		uint nStride = nOffset + g_pSystem->GetSizeOf( eType );

		if ( m_nStride < nStride )
		{
			m_nStride = nStride;
		}
	}
}


/**
*
*
const TVertexAttr * CVertexDecl::GetAttr( uint nAttr ) const
{
	if ( nAttr >= m_pAttrs.GetCount() )
	{
		return NULL;
	}

	return /*NULL;*m_pAttrs[ nAttr ];
}


/**
*
*
uint CVertexDecl::GetAttrCount() const
{
	return m_pAttrs.GetCount();
}


/**
*
*
uint CVertexDecl::GetVertexCount()
{
	return m_nVertexCount;
}


/**
*
*
void CVertexDecl::SetVertexCount( uint nCount )
{
	m_nVertexCount = nCount;
}




bool CVertexDecl::Register()
{
	return false;
}




/**
*
*/
IVertexDecl * CRendererD3D::CreateVertexDecl()
{
	return NEW CVertexDecl();
}

//////////////////////////////////////////////////////////////////////////