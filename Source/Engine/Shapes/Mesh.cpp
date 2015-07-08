////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Shape.h"


/**
*
*/
class CMesh : public IMesh
{
	public :

			CMesh	();
			~CMesh	();
	void	Release	();

	bool	LoadFromStream( IFile * pFile );

	bool	SetVertexBuffer( PVertexBuffer pVB );

	bool	SetMaterial( IMaterial * pMat );
	size_t	AddSurface( const char * szName, IIndexBuffer * pIB );
	size_t	GetSurfCount() const;
	const char *	GetSurfName		( uint nSurf );

	void	RenderSurface( uint nSurf, IMaterial * pMaterial );
	void	Render( IMaterial *	pMaterial );

	void	SetVertexDecl( IVertexDecl * pDecl )
	{
		m_pVertexDecl = pDecl;
	}

	//==========================================================================
	// Tip: Private Methods & Fields
	//==========================================================================
	private:

	TMeshData			m_Data;
	EPrimitive			m_ePrimType;

	PVertexBuffer 		m_pVB;
	IVertexDecl *		m_pVertexDecl;
	
	CVector <IIndexBuffer *> m_pSurfaces;
};


/**
*
*/
CMesh::CMesh() :
	m_ePrimType		( PRIM_TRIANGLE ),
	m_pVB			( NULL )
{
	TMeshData::THeader & tHeader = m_Data.tHeader;
	
	tHeader.nVertexCount	= 0;
	//tHeader.nVertexStride	= 0;
	tHeader.nComponentCount	= 0;
	tHeader.nSurfCount		= 0;

	m_Data.pSurfaces		= NULL;
}


/**
*
*/
CMesh::~CMesh()
{
	RELEASE( m_pVB );
	DEL_ARRAY( m_Data.pSurfaces );

	for ( uint n = 0; n < m_pSurfaces.GetCount(); ++n )
	{
		RELEASE( m_pSurfaces[ n ] );
	}

	m_pSurfaces.Delete();
}


/**
*
*/
void CMesh::Release()
{
	PURE_DEL( this );
}


/**
*
*
bool CMesh::SetMaterial( IMaterial * pMat )
{
	m_pMaterial = pMat;
	return false;
}


/**
*
*/
bool CMesh::SetVertexBuffer( PVertexBuffer pVB )
{
	m_pVB = pVB;
	return false;
}


/**
*
*/
size_t CMesh::AddSurface( const char * szName, IIndexBuffer * pIB )
{
	m_pSurfaces.AddToTail( pIB );
	return -1;
}


/**
/* Возвращает кол-во поверхностей для данной сетки
*/
size_t CMesh::GetSurfCount() const
{
	return m_Data.tHeader.nSurfCount;
}


/**
*
*/
const char * CMesh::GetSurfName( uint nSurf )
{
	if ( nSurf < GetSurfCount() )
	{
		return m_Data.pSurfaces[ nSurf ].tHeader.szName;
	}

	return NULL;
}


/**
*
*/
void CMesh::RenderSurface( uint nSurf, IMaterial * pMaterial )
{
	if ( m_pSurfaces.GetCount() <= nSurf )
	{
		DEBUG_MSG( "Invalid surface index!" );
		return;
	}

	/*if ( pMaterial )
	{
		const char * szName = m_Data.pSurfaces[ nSurf ].tHeader.szName;
*/
		IMaterial * pSubMaterial = pMaterial->GetSubMaterial( nSurf );

		if ( pSubMaterial )
		{
			pMaterial->SetTexture(
				DIFFUSE_MAP,
				pSubMaterial->GetTexture( DIFFUSE_MAP ) );
		}
		/*
	}

	IShader * pShader = pMaterial->GetShader();
	pShader->Assign();

//	IShader * pShader = pMaterial->GetShader();
//	pShader->SetVertexDecl( m_pVertexDecl );
*/
	pMaterial->Assign();

	if ( m_pVB )
	{
		if ( INVALID_INDEX != nSurf )
		{
			m_pVB->RenderIndexed( m_pVertexDecl, m_pSurfaces[ nSurf ], PRIM_TRIANGLE );
		}
	}

	IIndexBuffer * m_pIB =  m_pSurfaces[ nSurf ];

	/*if ( m_pIB )
	{
	short * pIndices = (short *)m_pIB->Lock( 0, m_pIB->GetNumIndices(), true );

	for ( size_t nTri = 0; nTri < ( m_pIB->GetNumIndices() / 3 ); ++nTri )
	{
	size_t nOffset = nTri * 3;

	struct TVertex
	{
	vec3 vPos;
	vec2 vTex;
	vec3 vNormal;
	};

	const TVertex & v1 = ((TVertex*)m_pVB)[ pIndices[ nOffset++ ] ];
	const TVertex & v2 = ((TVertex*)m_pVB)[ pIndices[ nOffset++ ] ];
	const TVertex & v3 = ((TVertex*)m_pVB)[ pIndices[ nOffset ] ];

	g_pRenderer->PushLine3D( v1.vPos, v1.vPos + v1.vNormal, 0xFFFFFFFF );
	g_pRenderer->PushLine3D( v2.vPos, v2.vPos + v2.vNormal, 0xFFFFFFFF );
	g_pRenderer->PushLine3D( v3.vPos, v3.vPos + v3.vNormal, 0xFFFFFFFF );
	}
	}*/
}


/**
*
*/
void CMesh::Render( IMaterial * pMaterial )
{
	if ( !pMaterial )
		return;

	//IShader * pShader = pMaterial->GetShader();
	//pShader->SetVertexDecl( m_pVertexDecl );
	//pMaterial->Assign();

	const size_t nSurfCount = GetSurfCount();

	if ( nSurfCount )
	{
		//--------------------------------------------------------------
		// Tip: Вывод трекгольников с использованием IndexBuffer'а
		//--------------------------------------------------------------
		DEBUG_ASSERT( PRIM_TRIANGLE == m_ePrimType );

		for ( uint i = 0; i < nSurfCount; ++i )
		{
			RenderSurface( i, pMaterial );
		}
	}
	else
	{
		RenderSurface( INVALID_INDEX, pMaterial ); // triangle strip
	}
}


/**
*
*/
IMesh * CEngine::CreateMesh()
{
	return NEW CMesh;
}


/**
*
*/
bool CMesh::LoadFromStream( IFile * pFile )
{
	uint nBlockSize;

	//--------------------------------------------------------------------------
	// Tip: Check file
	//--------------------------------------------------------------------------
	if ( NULL == pFile )
		return false;

	//--------------------------------------------------------------------------
	// Tip: Читаем заголовок mesh'а
	//--------------------------------------------------------------------------
	TMeshData::THeader & tHeader = m_Data.tHeader;
	nBlockSize = sizeof( TMeshData::THeader );

	if ( !g_pVfs->CheckRead( pFile, &tHeader, nBlockSize ) )
		goto error_label;

	//--------------------------------------------------------------------------
	// Tip: Читаем информацию о компонентах вершины
	//--------------------------------------------------------------------------
	m_Data.pComponents = NEW TVertexComponent [ tHeader.nComponentCount ]; // need to be cached
	nBlockSize = tHeader.nComponentCount * sizeof ( TVertexComponent );

	if ( !g_pVfs->CheckRead( pFile, m_Data.pComponents, nBlockSize ) )
		goto error_label;

	m_pVertexDecl = g_pRenderer->CreateVertexDecl();

	if ( NULL == m_pVertexDecl )
		goto error_label;

	for ( uint n = 0, nOffset = 0; n < tHeader.nComponentCount; ++n )
	{
		TVertexComponent & tComp = m_Data.pComponents[ n ];
		m_pVertexDecl->AddAttr( tComp.GetName(), NULL, tComp.GetType(), nOffset );
		nOffset += g_pSystem->GetSizeOf( tComp.GetType() );
	}

	m_pVertexDecl->SetStride( tHeader.nVertexSize );

	//DEBUG_ASSERT( m_pVertexDecl->GetStride() == tHeader.nVertexSize );

	//--------------------------------------------------------------------------
	// Tip: Читаем массив вершин ( vertex buffer )
	//--------------------------------------------------------------------------
	nBlockSize = tHeader.nVertexCount * tHeader.nVertexSize;
	m_Data.pVertices = NEW byte [ nBlockSize ];

	if ( NULL == m_Data.pVertices )
		goto error_label;

	if ( !g_pVfs->CheckRead( pFile, m_Data.pVertices, nBlockSize ) )
		goto error_label;

	//--------------------------------------------------------------------------
	// Tip: surfaces
	//--------------------------------------------------------------------------
	m_Data.pSurfaces = NEW TSurface [ tHeader.nSurfCount ];

	if ( NULL == m_Data.pSurfaces )
		goto error_label;

	for ( uint n = 0; n < tHeader.nSurfCount; ++n )
	{
		TSurface & tSurf = m_Data.pSurfaces[ n ];
		nBlockSize = sizeof( TSurface::THeader );

		if ( !g_pVfs->CheckRead( pFile, &tSurf.tHeader, nBlockSize ) )
			goto error_label;

		nBlockSize = tSurf.tHeader.nIndexCount * tSurf.tHeader.nIndexSize;
		tSurf.pIndices = NEW byte [ nBlockSize ];

		if ( NULL == tSurf.pIndices )
			goto error_label;

		if ( !g_pVfs->CheckRead( pFile, tSurf.pIndices, nBlockSize ) )
			goto error_label;
	}

	//--------------------------------------------------------------------------
	// Tip: Renderer
	//--------------------------------------------------------------------------
	struct TVertex
	{
		vec3 vPos;
		vec2 vTex;
	};

	if ( m_pVB = g_pRenderer->CreateVB() )
	{
		if ( m_pVB->Alloc( tHeader.nVertexCount * tHeader.nVertexSize, 0 ) )
		{
			TVertex * p = (TVertex *)m_pVB->Lock( 0, tHeader.nVertexCount * tHeader.nVertexSize );
			if ( p )
			{
				memcpy( p, m_Data.pVertices, tHeader.nVertexCount * tHeader.nVertexSize );
			}
			m_pVB->Unlock();
		}
	}

	DEL_ARRAY( m_Data.pVertices );

	//--------------------------------------------------------------------------
	// Tip: surfaces ( index buffers )
	//--------------------------------------------------------------------------
	for ( uint n = 0; n < tHeader.nSurfCount; ++n )
	{
		TSurface::THeader & tSurfHeader = m_Data.pSurfaces[ n ].tHeader;

		IIndexBuffer * pIB = g_pRenderer->CreateIB();

		if ( pIB )
		{
			if ( pIB->Alloc( tSurfHeader.nIndexSize * tSurfHeader.nIndexCount, 0 ) )
			{
				void *p = pIB->Lock( 0, tSurfHeader.nIndexSize * tSurfHeader.nIndexCount );

				if ( p )
				{
					memcpy( p, m_Data.pSurfaces[ n ].pIndices, tSurfHeader.nIndexSize * tSurfHeader.nIndexCount );

					pIB->SetAttrs( tSurfHeader.nIndexCount, TYPE_INT );
					pIB->Unlock();
					AddSurface( tSurfHeader.szName, pIB );
				}
				else
				{
					RELEASE( pIB );
				}
			}
			else
			{
				RELEASE( pIB );
			}
		}

		DEL_ARRAY( m_Data.pSurfaces[ n ].pIndices );
	}

	//--------------------------------------------------------------------------
	// Tip: Загрузка прошла удачно, выходим
	//--------------------------------------------------------------------------
	return true;

	//--------------------------------------------------------------------------
	// Tip: Поведение кода в случае ошибки при разборе потока
	//--------------------------------------------------------------------------
	error_label:

	memset( &m_Data.tHeader, 0, sizeof( TMeshData::THeader ) );
	DEL_ARRAY( m_Data.pComponents );
	DEL_ARRAY( m_Data.pVertices );
	DEL_ARRAY( m_Data.pSurfaces );
	DEL( m_pVertexDecl );
	m_pVB = NULL;
	return false;
}

////////////////////////////////////////////////////////////////////////////////