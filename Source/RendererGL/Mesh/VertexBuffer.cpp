////////////////////////////////////////////////////////////////////////////////

#include "Common.h"


/**
*
*/
const byte * GetMemPointer( const IIndexBuffer * pBuffer );


/**
*
*/
class CVertexBuffer : public IVertexBuffer
{
	friend CRenderer_GL;

	//--------------------------------------------------------------------------
	// Tip: Public methods & fields
	//--------------------------------------------------------------------------
	public:

	CVertexBuffer	();
	~CVertexBuffer	();

	bool	Alloc			( uint nSize, dword dwFlags );
	bool	Free			();
	void	Release			();

	bool	AllocMulti(uint nCount) ;
	IVertexBuffer * GetSubBuffer(uint nID);

	void *	Lock			( size_t nOffset, size_t nSize, bool bReadBack = false );
	void	Unlock			();
	EResult UpdateData		( size_t nOffset, void * pData, size_t nSize );

	void	RenderIndexed	( IVertexDecl * pDecl, IIndexBuffer * pIB, EPrimitive ePrim, uint nFirst = 0, uint nSize = 0 );
	void	Render			( IVertexDecl * pDecl, uint nFirst, uint nCount, EPrimitive ePrim );

	uint	GetHandle() { return m_nBufferID; }
	size_t	GetSize() { return m_nSize; }

	//--------------------------------------------------------------------------
	// Tip: Private methods & fields
	//--------------------------------------------------------------------------
	private:

	void BindVertexDecl( IVertexDecl * pDecl );

	bool		m_isMultiSub;
	IVertexBuffer ** m_ppSubBuffers;

	size_t		m_nSize;
	uint		m_nIndex;			// ������ � ������ ���� VB
	bool 		m_bLocked;

	union
	{
		uint		m_nBufferID;		// OpenGL buffer
		void *		m_pMemBuffer;		// Buffer in system memory
	};
};


/**
*
*/
CVertexBuffer::CVertexBuffer()
	: m_nSize(0)
	, m_bLocked(false)
	, m_isMultiSub(false)
	, m_ppSubBuffers(NULL)
{
	m_nBufferID		= 0;
	m_pMemBuffer	= NULL;
}


/**
*
*/
CVertexBuffer::~CVertexBuffer()
{
	Free();
	g_pRenderer->RemoveVB( m_nIndex );
}


/**
*
*/
void CVertexBuffer::Release()
{
	PURE_DEL( this );
}


bool CVertexBuffer::AllocMulti(uint nCount)
{
	m_isMultiSub = true;
	m_nSize = nCount;

	m_ppSubBuffers = new PVertexBuffer [nCount];

	for (uint i = 0; i < nCount; ++i)
	{
		m_ppSubBuffers[i] = g_pRenderer->CreateVB();
	}

	return true;
}


IVertexBuffer * CVertexBuffer::GetSubBuffer(uint nID)
{
	if (true == m_isMultiSub)
	{
		if (nID < m_nSize)
		{
			return m_ppSubBuffers[nID];
		}
	}

	DEBUG_ASSERT(!"invalid op");
	return NULL;
}


/**
*
*/
bool CVertexBuffer::Alloc( uint nSize, dword dwFlags )
{
	GLenum eUsage = GL_STATIC_DRAW_ARB;

	//--------------------------------------------------------------------------
	// Tip: �������� ���������
	//--------------------------------------------------------------------------
	if ( 0 == nSize )
	{
		DEBUG_ASSERT( !"Allocating null-size buffer" );
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: ������������, ���� ������ ��� ���� ��������
	//--------------------------------------------------------------------------
	if ( 0 != m_nSize )
	{
		DEBUG_ASSERT( !"Allocating allocated buffer" );
		return Free();
	}

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		//----------------------------------------------------------------------
		// Tip: ������ VBO ������
		//----------------------------------------------------------------------
		glGenBuffersARB( 1, &m_nBufferID );
		GL_VALIDATE;

		if ( 0 == m_nBufferID )
		{
			// ������! ������ �� ��� ������!
			return false;
		}

		// ������ ����� ������ ��������
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		// �������� ������ ��� �������
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, nSize, NULL, eUsage );

		if ( GL_NO_ERROR != glGetError() )
		{
			// Tip: ������! ������ �� ���� ��������!
			return false;
		}

		// ������ ������ ����������
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	}
	else
	{
		//----------------------------------------------------------------------
		// Tip: ������ SysMem ������
		//----------------------------------------------------------------------
		m_pMemBuffer = NEW byte [ nSize ];

		if ( NULL == m_pMemBuffer )
		{
			// ������! ������ �� ��� ������!
			return false;
		}
	}

	//--------------------------------------------------------------------------
	// Tip: ��
	//--------------------------------------------------------------------------
	m_nSize = nSize;
	return true;
}


/**
*
*/
bool CVertexBuffer::Free()
{
	if (true == m_isMultiSub)
	{
		for (uint i = 0; i < m_nSize; ++i)
		{
			IVertexBuffer * subBuffer = GetSubBuffer(i);
			subBuffer->Release();
		}

		DEL_ARRAY(m_ppSubBuffers);
		m_nSize = 0;
		return true;
	}

	//--------------------------------------------------------------------------
	// Tip: �������, ���� ������ �������
	//--------------------------------------------------------------------------
	if ( m_bLocked )
	{
		DEBUG_ASSERT( !"deleting locked buffer" );
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: �������, ���� ������ �� ��������
	//--------------------------------------------------------------------------
	if ( 0 == m_nSize )
		return true;

	//--------------------------------------------------------------------------
	// Tip: ������������ ������
	//--------------------------------------------------------------------------
	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glDeleteBuffersARB( 1, &m_nBufferID );
		GL_VALIDATE;

		m_nBufferID = 0;
	}
	else
	{
		DEL_ARRAY( m_pMemBuffer );
	}

	m_nSize = 0;
	return true;
}


/**
*
*/
void * CVertexBuffer::Lock( size_t nOffset, size_t nSize, bool bReadBack  )
{
	GL_VALIDATE;
	byte * pResult = NULL;

	//--------------------------------------------------------------------------
	// Tip: �������, ���� ������ ��� �������
	//--------------------------------------------------------------------------
	if ( m_bLocked )
	{
		DEBUG_ASSERT( !"Try to lock locked buffer" );
		return NULL;
	}

	//--------------------------------------------------------------------------
	// Tip: �������, ���� nOffset � nSize �� ���������� � �������
	//--------------------------------------------------------------------------
	if ( ( nOffset + nSize ) > m_nSize )
	{
		DEBUG_ASSERT( !"Out of buffer" );
		return NULL;
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		pResult = (byte *)glMapBufferARB(
			GL_ARRAY_BUFFER_ARB,
			bReadBack ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB );
		GL_VALIDATE;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
	else
	{
		pResult = (byte *)m_pMemBuffer;
	}

	//--------------------------------------------------------------------------
	// Tip: ��
	//--------------------------------------------------------------------------
	m_bLocked = true;
	return pResult + nOffset;
}


/**
*
*/
void CVertexBuffer::Unlock()
{
	//--------------------------------------------------------------------------
	// Tip: �������, ���� ������ �� ��� �������
	//--------------------------------------------------------------------------
	if ( false == m_bLocked )
	{
		DEBUG_ASSERT( !"Try to unlock non-locked buffer" );
		return;
	}

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		glUnmapBufferARB( GL_ARRAY_BUFFER_ARB );
		GL_VALIDATE;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}

	m_bLocked = false;
}


/**
*
*/
EResult CVertexBuffer::UpdateData( size_t nOffset, void * pData, size_t nSize )
{
	if ( ( 0 == nSize ) || ( NULL == pData ) )
	{
		DEBUG_ASSERT( !"invalid arg" );
		return R_INVALID_ARG;
	}

	if ( ( nOffset + nSize ) > m_nSize )
	{
		DEBUG_ASSERT( !"Out of buffer" );
		return R_INVALID_ARG;
	}

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, nOffset, nSize, pData );
		GL_VALIDATE;

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
	else
	{
		memcpy( (byte *)m_pMemBuffer + nOffset, pData, nSize );
	}

	return R_OK;
}


/**
*
*/
void CVertexBuffer::BindVertexDecl( IVertexDecl * pDecl )
{
	if ( NULL == pDecl )
		return;

	byte * pOffset = NULL;

	for (uint n = 0; n < pDecl->GetAttrCount(); ++n)
	{
		const TVertexAttr *	pAttr = pDecl->GetAttr( n );
		
		// for debug
		int a = glGetAttribLocationARB(g_pRenderer->GetCurrentProgram(), pAttr->szName);

		{
			uint bufferID = m_nBufferID;
			uint stride = (GLsizei)pDecl->GetStride();
			uint offset = pAttr->nOffset;

			if (m_isMultiSub)
			{
				if (n < m_nSize)
				{
					bufferID = GetSubBuffer(n)->GetHandle();
					stride = 0;
					offset = 0;
				}
				else
				{
					bufferID = 0;
				}
			}

			if (0 != bufferID)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, bufferID);
				GL_VALIDATE;

				glVertexAttribPointerARB(
					n, //tAttr.nLoc,
					g_pElemCountGL[pAttr->eType],								// element count
					GL_FLOAT, //tAttr.eType,										// element type
					GL_FALSE, //( tAttr.eType == GL_FLOAT ) ? GL_FALSE : GL_TRUE,	// need normalize int numbers?
					stride,								// stride
					pOffset + offset);					// offset
				GL_VALIDATE;

				glEnableVertexAttribArrayARB(n /* tAttr.nLoc */);
				GL_VALIDATE;
			}
		}
	}

	/*else
	{
		if ( n == 0 )
		{
			glDisableClientState( GL_VERTEX_ARRAY );
			continue;
		}

		glDisableVertexAttribArrayARB( n  );
		GL_VALIDATE;
	}*/
}


/**
*
*/
void CVertexBuffer::Render( IVertexDecl * pDecl, uint nFirst, uint nCount, EPrimitive ePrim )
{
//	DEBUG_ASSERT( !"not working function" );
	//if ( !PrepareRender() )
//		return;

	//--------------------------------------------------------------------------
	// Tip: Bind VBO object
	//--------------------------------------------------------------------------
	/*
	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;
	}
	*/

	// assign vertex declaration
	BindVertexDecl( pDecl );

	GLenum prim_mode = GL_POINTS;

	switch ( ePrim )
	{
		case PRIM_LINE:		prim_mode = GL_LINES;		break;
		case PRIM_TRIANGLE:	prim_mode = GL_TRIANGLES;	break;
		case PRIM_QUAD:		prim_mode = GL_QUADS;		break;
	}
	
	glDrawArrays( prim_mode, nFirst, (GLsizei)nCount );
	GL_VALIDATE;

	glUseProgramObjectARB( 0 );
	GL_VALIDATE;

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CVertexBuffer::RenderIndexed( IVertexDecl * pDecl, IIndexBuffer * pIB, EPrimitive ePrim, uint nFirst, uint nSize )
{
	if ( ( 0 == m_nSize ) || ( true == m_bLocked ) )
	{
		DEBUG_ASSERT( !"error" );
		return;
	}

	if ( NULL == pDecl )
		return;

	if ( !pIB )
	{
		DEBUG_ASSERT( !"invalid index buffer" );
		return;
	}

	if ( ( nFirst + nSize ) >= pIB->GetNumIndices() )
	{
		DEBUG_ASSERT( !"out of vb" );
		return;
	}


	//--------------------------------------------------------------------------
	// Tip: Bind VBO object
	//--------------------------------------------------------------------------
	BindVertexDecl( pDecl );

	// assgin index buffer
	pIB->Bind();

	GLenum prim_mode = GL_POINTS;

	switch ( ePrim )
	{
		case PRIM_LINE:		prim_mode = GL_LINES;		break;
		case PRIM_TRIANGLE:	prim_mode = GL_TRIANGLES;	break;
		case PRIM_QUAD:		prim_mode = GL_QUADS;		break;
	}

	GLenum index_type = GL_INVALID_ENUM;
	uint	nElemSize = 0;

	switch ( pIB->GetType() )
	{
		case TYPE_SHORT:
			index_type = GL_UNSIGNED_SHORT;
			nElemSize = sizeof( short );
		break;

		case TYPE_INT:
			index_type = GL_UNSIGNED_INT;
			nElemSize = sizeof( uint );
		break;

		default:
			DEBUG_ASSERT( !"unknown buffer index type" );
			return;
	}

	if ( GL_INVALID_ENUM != index_type )
	{
		GLsizei			nCount	= ( 0 == nSize ) ? pIB->GetNumIndices() : nSize;
		const byte *	pOffset	= g_pRenderer->IsExtSupported( EXT_GL_VBO ) ? NULL : GetMemPointer( pIB );

		glDrawElements( prim_mode, nCount, index_type, pOffset + ( nFirst * nElemSize ) );
		GL_VALIDATE;
	}

	pDecl->Assign( false );

	glUseProgramObjectARB( 0 );
	GL_VALIDATE;

	if ( g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
	{
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );
		GL_VALIDATE;
		glDisable( GL_VERTEX_PROGRAM_ARB );
		GL_VALIDATE;

		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, 0 );
		GL_VALIDATE;
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
		GL_VALIDATE;
	}

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
}


/**
*
*/
PVertexBuffer CRenderer_GL::CreateVB()
{
	CVertexBuffer * pVB;
	PVertexBuffer	pResult;

	if ( pVB = NEW CVertexBuffer() )
	{
		pVB->m_nIndex = m_VBList.Add( pResult );

		if ( INVALID_INDEX == pVB->m_nIndex  )
		{
			pVB->Release();
		}
		else
		{
			pResult = pVB;
		}
	}

	return pResult;
}


/**
*
*/
void CRenderer_GL::RemoveVB( uint nListID )
{
	m_VBList.Remove( nListID );
}

////////////////////////////////////////////////////////////////////////////////