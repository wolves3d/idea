////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/**
*
*/
class CVertexBuffer : public IVertexBuffer
{
	public:

	/**
	*
	*/	
	CVertexBuffer() :
		m_pVB	( NULL ),
		m_pVDecl( NULL )
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
		DEBUG_ASSERT( NULL == m_pVB );
		Free();

		D3D10_BUFFER_DESC bd;

		bd.Usage			= D3D10_USAGE_DYNAMIC;
		bd.ByteWidth		= nSize;
		bd.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE ; // set 0 to setup data at alloc only
		bd.MiscFlags		= 0;

		HRESULT hRes = g_pDevice->CreateBuffer( &bd, NULL, &m_pVB );
		return SUCCEEDED( hRes );
	}


	/**
	*
	*/
	bool Free()
	{
		RELEASE( m_pVB );
		return true;
	}


	/**
	*
	*/
	void * Lock( size_t nOffset, size_t nSize, bool bReadBack )
	{
		if ( m_pVB )
		{
			void * pData;

			HRESULT hRes = m_pVB->Map( D3D10_MAP_WRITE_DISCARD, 0, &pData );

			if ( SUCCEEDED ( hRes ) )
			{
				return pData;
			}
			else
			{
				DEBUG_MSG( "Map failed" );
				int a =0;
			}
		}

		return NULL;
	}


	/**
	*
	*/
	void Unlock()
	{
		if ( m_pVB )
		{
			m_pVB->Unmap();
		}
	}

	/**
	*
	*/
	EResult UpdateData( size_t nOffset, void * pData, size_t nSize )
	{
		if ( NULL == pData )
		{
			return R_INVALID_ARG;
		}

		void * pLock = Lock( nOffset, nSize, false );

		if ( pLock )
		{
			memcpy( pLock, pData, nSize );
			Unlock();
			return R_OK;
		}

		return R_GENERIC_ERROR;
	}


	/**
	*
	*/
	void SetVertexDecl( IVertexDecl * pDecl )
	{
		m_pVDecl = pDecl;
	}


	/**
	*
	*/
	void Render( IVertexDecl * pDecl, uint nFirst, uint nCount, EPrimitive ePrim )
	{
		if ( NULL == m_pVB )
			return;

		if ( NULL == pDecl )
			return;

		D3D10_PRIMITIVE_TOPOLOGY top = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		
		if ( PRIM_LINE == ePrim )
			top = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			
		uint stride[] = { pDecl->GetStride() };
		uint offsets[] = { 0 };
		g_pDevice->IASetVertexBuffers( 0, 1, &m_pVB, stride, offsets );
		g_pDevice->IASetPrimitiveTopology( top );
		g_pDevice->Draw( nCount, nFirst );
	}


	/**
	*
	*/
	void RenderIndexed( IVertexDecl * pDecl, IIndexBuffer * pIB, EPrimitive ePrim, uint nFirst, uint nSize )
	{
		if ( ( NULL == m_pVB ) || ( NULL == pIB ) || ( NULL == pDecl ) )
			return;

		pIB->Bind();

		uint strides[] = { pDecl->GetStride() };
		uint offsets[] = { 0 };
		g_pDevice->IASetVertexBuffers( 0, 1, &m_pVB, strides, offsets );
		g_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		g_pDevice->DrawIndexed( nSize/*pIB->GetNumIndices()*/, nFirst, 0 );
		//g_pDevice->DrawIndexed( 0, 0/*nFirst*/, 0 );
	}

	private:

		ID3D10Buffer *	m_pVB;
		IVertexDecl *	m_pVDecl;
};


/**
*
*/
PVertexBuffer CRendererD3D::CreateVB()
{
	PVertexBuffer pResult;

	
	if ( pResult = NEW CVertexBuffer() )
	{
		//pResult->m_nIndex = m_VBList.Add( pResult );
	}

	return pResult;
}

////////////////////////////////////////////////////////////////////////////////