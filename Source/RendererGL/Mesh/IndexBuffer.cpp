////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/*
#include "Shaders/BaseShader.h"
size_t SizeOf( ETypeID eType );
size_t GetElementCount( ETypeID eType );
*/


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CIndexBuffer : public IIndexBuffer
{
	friend CRenderer_GL;

	//==========================================================================
	// Tip: Public methods & fields
	//==========================================================================
public:

	CIndexBuffer();
	~CIndexBuffer();
	void	Release			();

	bool	Alloc			( uint nSize, dword dwFlags );
	void *	Lock			( uint nOffset, size_t nSize, bool bReadBack = false );
	void	Unlock			();

	bool	Bind			();

	void	SetAttrs		( uint nNumIndices, ETypeID eType )
	{
		m_nNumIndices	= nNumIndices;
		m_eType			= eType;
	}

	inline uint GetNumIndices() const
	{
		return m_nNumIndices;
	}

	inline ETypeID GetType() const
	{
		return m_eType;
	}

	const byte * GetMemPointer() const
	{
		return m_pMemBuffer;
	}

	//==========================================================================
	// Tip: Private methods & fields
	//==========================================================================
private:

	bool m_bLocked;
	uint m_nNumIndices, m_nSize;
	ETypeID	m_eType;

	uint	m_nIndex;			// Индекс в списке всех IB

	byte * m_pMemBuffer;
	uint m_nBufferID;	// OpenGL specific
};


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
CIndexBuffer::CIndexBuffer() :
	m_nSize			( 0		),
	m_nNumIndices	( 0		),
	m_eType			( TYPE_SHORT ),
	m_bLocked		( false	)
{
	m_pMemBuffer	= NULL;
	m_nBufferID		= 0;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
CIndexBuffer::~CIndexBuffer()
{
	g_pRenderer->RemoveIB( m_nIndex );

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		if ( m_nBufferID )
		{
			glDeleteBuffersARB( 1, &m_nBufferID );
			GL_VALIDATE;
		}
	}
	else
	{
		DEL_ARRAY( m_pMemBuffer );
	}
}


/**
*
*/
void CIndexBuffer::Release()
{
	PURE_DEL( this );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CIndexBuffer::Alloc( uint nSize, dword dwFlags )
{
	DEBUG_ASSERT( m_nSize == NULL );

	//--------------------------------------------------------------------------
	// Tip: Проверка аргумента
	//--------------------------------------------------------------------------
	if ( !nSize )
		return false;

	GLenum eUsage = GL_STATIC_DRAW_ARB;

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		//----------------------------------------------------------------------
		// Tip: Буффер создаётся впервые
		//----------------------------------------------------------------------
		glGenBuffersARB( 1, &m_nBufferID );
		GL_VALIDATE;

		if ( !m_nBufferID )
		{
			//------------------------------------------------------------------
			// Tip: Ошибка! Буффер не был создан!
			//------------------------------------------------------------------
			return R_GENERIC_ERROR;
		}

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nSize, NULL, eUsage );
		GL_VALIDATE;

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
	else
	{
		m_pMemBuffer = NEW byte [ nSize ];
	}

	m_nSize = nSize;
	return true;
}


/**
*
*/
void * CIndexBuffer::Lock( uint nOffset, size_t nSize, bool bReadBack  )
{
	DEBUG_ASSERT( 0 == nOffset ); // offset not supported

	//--------------------------------------------------------------------------
	// Tip: Выходим, если буффер уже залочен
	//--------------------------------------------------------------------------
	if ( m_bLocked )
	{
		DEBUG_ASSERT( !"Try to lock locked buffer" );
		return NULL;
	}

	byte * pResult = NULL;

	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		pResult = (byte *)glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
			bReadBack ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB );
		GL_VALIDATE;

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
	else
	{
		pResult = m_pMemBuffer;
	}

	m_bLocked = true;
	return pResult;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CIndexBuffer::Unlock()
{
	if ( false == m_bLocked )
	{
		DEBUG_ASSERT( !"Try to unlock non-locked buffer" );
		return;
	}
	
	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;

		glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );
		GL_VALIDATE;

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
		GL_VALIDATE;
	}
	
	m_bLocked = false;
	return;
}


/**
*
*/
bool CIndexBuffer::Bind()
{
	if ( g_pRenderer->IsExtSupported( EXT_GL_VBO ) )
	{
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferID );
		GL_VALIDATE;
	}

	return true;
}


/**
*
*/
IIndexBuffer * CRenderer_GL::CreateIB()
{
	CIndexBuffer *	pIB;

	if ( pIB = NEW CIndexBuffer() )
	{
		pIB->m_nIndex = m_IBList.Add( pIB );

		if ( INVALID_INDEX == pIB->m_nIndex  )
		{
			pIB->Release();
			pIB = NULL;
		}
	}

	return pIB;
}


/**
*
*/
void CRenderer_GL::RemoveIB( uint nListID )
{
	m_IBList.Remove( nListID );
}


/**
*
*/
const byte * GetMemPointer( const IIndexBuffer * pBuffer )
{
	return ((CIndexBuffer *)pBuffer)->GetMemPointer();
}

////////////////////////////////////////////////////////////////////////////////