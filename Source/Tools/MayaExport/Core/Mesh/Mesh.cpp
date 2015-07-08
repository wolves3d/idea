////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Mesh.h"

/**
*
*/
CMayaMesh::CMayaMesh( MObject object ) :
	m_nUvSetCount	( 0		),
	m_pUvSets		( NULL	),
	m_nSurfCount	( 0		),
	m_pSurfaces		( NULL	),
	m_bValid		( false	)
{
	ZeroMemory( &m_MeshData, sizeof ( TMeshData ) );

	if ( m_FnMesh.setObject( object ) )
	{
		m_Object = object;
		m_bValid = true;
	}
}


/**
*
*/
CMayaMesh::~CMayaMesh()
{
	while( m_nSurfCount-- )
	{
		TMayaSurface & tSurf = m_pSurfaces[ m_nSurfCount ];

		for ( uint n = 0; n < m_nUvSetCount; ++n )
			tSurf.ppUvIndices[ n ].Delete();

		tSurf.pIndices.Delete();
	}

	DEL_ARRAY( m_pUvSets );
	DEL_ARRAY( m_pSurfaces );
}


/**
*
*/



/**
*
*/
struct TVertex
{
	vec3 vPos;
	vec2 vUV;
};


/**
*
*/
bool CMayaMesh::FillMeshData()
{
	//--------------------------------------------------------------------------
	//	Tip: Prepare arrays
	//--------------------------------------------------------------------------
	/*MMatrix mModelMatrix = m_FnMesh.transformationMatrix();

	mat4 m;
	for ( int mi = 0; mi < 4; ++mi )
	{
		for ( int mj = 0; mj < 4; ++mj )
		{
			m.pArray[ mi * 4 + mj ] = (float)mModelMatrix.matrix[ mi ][ mj ];
		}
	}*/

	////////////////////////////////////////////////////////////////////////////

	ZeroMemory( &m_MeshData, sizeof( TMeshData ) );

	m_MeshData.tHeader.nHeaderSize	= sizeof( TMeshData::THeader );
	m_MeshData.tHeader.nVertexCount	= GetVertexCount();
	m_MeshData.tHeader.nSurfCount	= m_nSurfCount;

	if (	( 0 == m_nSurfCount ) ||
			( 0 == m_MeshData.tHeader.nVertexCount )	)
	{
		return false;
	}

	m_MeshData.tHeader.nComponentCount = 2;

	m_MeshData.pSurfaces = NEW TSurface [ m_nSurfCount ];
	TVertex * pVB = NEW TVertex[ m_MeshData.tHeader.nVertexCount ];
	m_MeshData.pComponents = NEW TVertexComponent[ m_MeshData.tHeader.nComponentCount ];

	if (	( NULL == m_MeshData.pSurfaces ) ||
			( NULL == pVB ) ||
			( NULL == m_MeshData.pComponents ) )
	{
		DEL_ARRAY( m_MeshData.pSurfaces );
		DEL_ARRAY( pVB );
		DEL_ARRAY( m_MeshData.pComponents );

		return false;
	}

	m_MeshData.pVertices = pVB;
	m_MeshData.tHeader.nVertexSize = sizeof( TVertex );

	m_MeshData.pComponents[ 0 ].eType = TYPE_VEC3;
	m_MeshData.pComponents[ 0 ].nSize = sizeof( vec3 );
	strcpy( m_MeshData.pComponents[ 0 ].szName, "vPos" );

	m_MeshData.pComponents[ 1 ].eType = TYPE_VEC2;
	m_MeshData.pComponents[ 1 ].nSize = sizeof( vec2 );
	strcpy( m_MeshData.pComponents[ 1 ].szName, "vTex0" );

	//--------------------------------------------------------------------------
	//	Tip: Prepare arrays
	//--------------------------------------------------------------------------
	for ( uint nID = 0; nID < m_nSurfCount; ++nID )
	{
		TMayaSurface & tSurf = m_pSurfaces[ nID ];

		TUVSet & tUVset = m_pUvSets[ 0 ]; // may not exist
		CUintVector & tUVindices = tSurf.ppUvIndices[ 0 ];  // may not exist

		for ( dword n = 0; n < tSurf.pIndices.GetCount(); ++n )
		{
			int t = tUVindices[ n ];
			int p = tSurf.pIndices[ n ];

			pVB[ t ].vPos.Set(	m_pVertices[ p ].x,
								m_pVertices[ p ].y,
								m_pVertices[ p ].z );

			//pVB[ t ].vPos.Multiply( m );

			//pBiNormals[ t ].Set(BiNormals[ p ].x,
			//BiNormals[ p ].y,
			//BiNormals[ p ].z );

			pVB[ t ].vUV.Set( tUVset.pU[ t ], tUVset.pV[ t ] );
		}

		if ( m_MeshData.pSurfaces )
		{
			TSurface & tSurface = m_MeshData.pSurfaces[ nID ];

			tSurface.tHeader.nIndexSize		= sizeof( uint );
			tSurface.tHeader.nIndexCount	= tSurf.pIndices.GetCount();
			tSurface.pIndices				= tUVindices.GetArray();

			strcpy( tSurface.tHeader.szName, tSurf.sName.asChar() );
		}
	}

	return true;
}


/**
*
*/
void ExportMesh( FILE * pFile, MObject & obj )
{
	MObject object = obj;
	CMayaMesh mesh( object );

	mesh.Save( pFile );
}


/**
*
*/
bool CMayaMesh::Save( FILE * pFile )
{
	if ( NULL == pFile )
	{
		return false;
	}

	m_bValid &= ExtractVertices();
	m_bValid &= ExtractUVSets();
	/*m_bValid &=*/ ExtractSurfaces();
	m_bValid &= FillMeshData();

	// Настраиваем ссылки ( для удобства чтения кода )
	const uint & nComponentCount	= m_MeshData.tHeader.nComponentCount;
	const uint & nVertexCount		= m_MeshData.tHeader.nVertexCount;
	const uint & nVertexSize		= m_MeshData.tHeader.nVertexSize;
	const uint & nSurfaceCount		= m_MeshData.tHeader.nSurfCount;

	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка объекта
	//--------------------------------------------------------------------------
	TObjectHdr tHeader;
	FillObjectHeader( &tHeader );

	//tHeader.szParent[ 0 ] = 0;
	tHeader.nShapeBlockSize = 0;

	if ( m_bValid )
	{
		tHeader.nShapeBlockSize =
			sizeof( TMeshData::THeader ) +
			nComponentCount * sizeof( TVertexComponent ) +
			nVertexCount * nVertexSize +
			nSurfaceCount * sizeof( TSurface::THeader );
	}

	// Пишем заголовок object'а
	fwrite( &tHeader, sizeof( TObjectHdr ), 1, pFile );

	if ( !m_bValid )
	{
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: Сохранение mesh'а
	//--------------------------------------------------------------------------

	// Пишем заголовок mesh'а
	fwrite( &m_MeshData.tHeader, sizeof( TMeshData::THeader ), 1, pFile );

	// Пишем компоненты вершины
	fwrite( m_MeshData.pComponents,
		nComponentCount * sizeof( TVertexComponent ), 1, pFile );

	// Пишем массив вершин
	fwrite( m_MeshData.pVertices, nVertexCount * nVertexSize, 1, pFile );

	// Записываем каждый материал ( surface )
	for ( uint n = 0; n < nSurfaceCount; ++n )
	{
		TSurface & tSurf = m_MeshData.pSurfaces[ n ];

		// Пишем заголовок surface'а
		fwrite( &tSurf.tHeader, sizeof( TSurface::THeader ), 1, pFile );

		// Пишем индексы вершин
		fwrite( tSurf.pIndices,
			tSurf.tHeader.nIndexCount * tSurf.tHeader.nIndexSize,
			1, pFile );
	}

	return true;
}


/**
*
*
bool WriteMesh( FILE * pFile, const TMeshData & tMesh )
{
	if ( !pFile )
		return false;

	// Настраиваем ссылки ( для удобства чтения кода )
	const uint & nComponentCount	= tMesh.tHeader.nComponentCount;
	const uint & nVertexCount		= tMesh.tHeader.nVertexCount;
	const uint & nVertexSize		= tMesh.tHeader.nVertexSize;
	const uint & nSurfaceCount		= tMesh.tHeader.nSurfCount;

	// Пишем заголовок mesh'а
	fwrite( &tMesh.tHeader, sizeof( TMeshData::THeader ), 1, pFile );

	// Пишем компоненты вершины
	fwrite( tMesh.pComponents,
		nComponentCount * sizeof( TVertexComponent ), 1, pFile );

	// Пишем массив вершин
	fwrite( tMesh.pVertices, nVertexCount * nVertexSize, 1, pFile );

	// Записываем каждый материал ( surface )
	for ( uint n = 0; n < nSurfaceCount; ++n )
	{
		TSurface & tSurf = tMesh.pSurfaces[ n ];

		// Пишем заголовок surface'а
		fwrite( &tSurf.tHeader, sizeof( TSurface::THeader ), 1, pFile );

		// Пишем индексы вершин
		fwrite( tSurf.pIndices,
			tSurf.tHeader.nIndexCount * tSurf.tHeader.nIndexSize,
			1, pFile );
	}

	return true;
}*/

////////////////////////////////////////////////////////////////////////////////