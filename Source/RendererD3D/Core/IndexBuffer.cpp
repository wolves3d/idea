////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/**
*
*/
class CIndexBuffer : public IIndexBuffer
{
	public:

	/**
	*
	*/	
	CIndexBuffer() :
		m_pIB( NULL )
	{
	}


	/**
	*
	*/	
	void	Release()
	{
	  Free();
	  PURE_DEL( this );
	}


	/**
	*
	*/
	bool Alloc( uint nSize, dword dwFlags )
	{
	  DEBUG_ASSERT( NULL == m_pIB );
	  Free();

	  D3D10_BUFFER_DESC bd;

	  bd.Usage			= D3D10_USAGE_DYNAMIC;
	  bd.ByteWidth		= nSize;
	  bd.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	  bd.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	  bd.MiscFlags		= 0;

	  HRESULT hRes = g_pDevice->CreateBuffer( &bd, NULL, &m_pIB );
	  return SUCCEEDED( hRes );
	}


	/**
	*
	*/
	bool Free()
	{
	  RELEASE( m_pIB );
	  return true;
	}


	/**
	*
	*/
	void * Lock( uint nOffset, size_t nSize, bool bReadBack )
	{
		if ( m_pIB )
		{
			void * pData;

			HRESULT hRes = m_pIB->Map( D3D10_MAP_WRITE_DISCARD, 0, &pData );

			if ( SUCCEEDED ( hRes ) )
			{
				return pData;
			}
		}

		return NULL;
	}


	/**
	*
	*/
	void Unlock()
	{
	  if ( m_pIB )
	  {
		  m_pIB->Unmap();
	  }
	}


	/**
	*
	*/
	void SetAttrs( uint nNumIndices, ETypeID eType )
	{
	  m_nNumIndices	= nNumIndices;
	  m_eType		= eType;
	}


	//!

	//!
	//!
	ETypeID GetType() const
	{
	  return m_eType;
	}


	//!

	//!
	//!
	uint GetNumIndices() const
	{
		return m_nNumIndices;
	}


	//!

	//!
	//!
	bool Bind()
	{
		if ( !m_pIB )
			return false;

		DXGI_FORMAT eFmt;

		switch ( m_eType )
		{
			case TYPE_SHORT:
				eFmt = DXGI_FORMAT_R16_UINT;
			break;

			case TYPE_INT:
				eFmt = DXGI_FORMAT_R32_UINT;
			break;

			default:
				DEBUG_MSG( "invalid indices format!" );
				return false;
		}

		g_pDevice->IASetIndexBuffer( m_pIB, eFmt, 0 );
		return true;
	}

private:

	ID3D10Buffer *	m_pIB;
	ETypeID			m_eType;
	uint			m_nNumIndices;
};


/**
*
*/
IIndexBuffer * CRendererD3D::CreateIB()
{
	IIndexBuffer * pResult;

	if ( pResult = NEW CIndexBuffer() )
	{
		//pResult->m_nIndex = m_VBList.Add( pResult );
	}

	return pResult;
}

////////////////////////////////////////////////////////////////////////////////