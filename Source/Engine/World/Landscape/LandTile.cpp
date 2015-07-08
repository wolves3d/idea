#include "Common.h"
#include "Landscape.h"


CLandTile::CLandTile() :
	m_pVB		( NULL ),
	m_pIB		( NULL ),
	m_pVertices	( NULL ),
	m_pIndices	( NULL ),
	m_pTraceBoxes( NULL )
{
}


CLandTile::~CLandTile()
{
	RELEASE( m_pVB );
	RELEASE( m_pIB );
	DEL_ARRAY( m_pVertices );
	DEL_ARRAY( m_pIndices );
	DEL_ARRAY( m_pTraceBoxes );
}


void CLandTile::Create( byte * pHeightmap, uint nStride )
{
	m_pVB = g_pRenderer->CreateVB();

	if ( NULL == m_pVB )
		return;

	const uint nBufferSize = TILE_VTX_COUNT * sizeof( TLandVertex );
	bool bRes = m_pVB->Alloc( nBufferSize, 0 );

	if ( !bRes )
	{
		RELEASE( m_pVB );
		return;
	}

	m_pVertices = NEW TLandVertex [ TILE_VTX_COUNT ];
	m_pTraceBoxes = NEW bbox [ TILE_DIVS * TILE_DIVS ];

	m_bbox.vMin.Set( 0, 0, 0 );
	m_bbox.vMax.Set( SIDE_CELL_COUNT, 0, SIDE_CELL_COUNT );

	//--------------------------------------------------------------------------
	// Tip: Заполняем вершины
	//--------------------------------------------------------------------------
	for ( uint y = 0; y < SIDE_VTX_COUNT; ++y )
	{
		for ( uint x = 0; x < SIDE_VTX_COUNT; ++x )
		{
			uint			nHeightIndex	= ( y * nStride ) + x;
			TLandVertex &	vertex			= m_pVertices[ ( SIDE_VTX_COUNT * y ) + x  ];

			vertex.vPos.x = (scalar)x;
			vertex.vPos.y = (float)pHeightmap[ nHeightIndex ] * 0.1f;
			vertex.vPos.z = (scalar)y;

 			if ( m_bbox.vMax.y < vertex.vPos.y )
 				m_bbox.vMax.y = vertex.vPos.y;

			vertex.vUV.x = (float)x / (float)SIDE_CELL_COUNT;
			vertex.vUV.y = (float)y / (float)SIDE_CELL_COUNT;
		}
	}

	//--------------------------------------------------------------------------
	// Tip: Заполняем BBox'ы
	//--------------------------------------------------------------------------
	const float fBoxSide = (float)SIDE_CELL_COUNT / (float)TILE_DIVS;

	for ( uint y = 0; y < TILE_DIVS; ++y )
	{
		for ( uint x = 0; x < TILE_DIVS; ++x )
		{
			bbox * pBox = m_pTraceBoxes + ( ( y * TILE_DIVS ) + x );

			pBox->vMin.Set( x * fBoxSide, 0, y * fBoxSide );
			pBox->vMax.Set( pBox->vMin.x + fBoxSide, 4, pBox->vMin.z + fBoxSide );
		}
	}



	void * pLocked = m_pVB->Lock( 0, nBufferSize );
	memcpy( pLocked, m_pVertices, nBufferSize );
	m_pVB->Unlock();

	/////////////////////////////////////////////////////////////////////////

	uint nIndexCount = SIDE_CELL_COUNT * SIDE_CELL_COUNT * 2 * 6;
	m_pIB = g_pRenderer->CreateIB();
	m_pIB->SetAttrs( nIndexCount, TYPE_SHORT );
	m_pIB->Alloc( sizeof ( short ) * nIndexCount, 0 );

	m_pIndices = NEW short [ nIndexCount ];

	//const uint nTriCount = SIDE_CELL_COUNT;

	for ( uint y = 0; y < SIDE_CELL_COUNT; ++y )
	{
		const uint nRawStart = SIDE_VTX_COUNT * y;

		for ( uint x = 0; x < SIDE_CELL_COUNT; ++x )
		{
			int nOffset = ( y * SIDE_CELL_COUNT + x ) * 6;

			// Первый треугльник
			// v1 --- v2 
			// |     /
			// |   /
			// | /
			// v3
			m_pIndices[ nOffset ]		= nRawStart + x;					// v1
			m_pIndices[ nOffset + 1 ]	= nRawStart + x + 1;				// v2
			m_pIndices[ nOffset + 2 ]	= nRawStart + x + SIDE_VTX_COUNT;	// v3

			// Воторой треугльник
			//        v1 
			//       / |
			//     /   |
			//   /     |
			// v3 --- v2
			m_pIndices[ nOffset + 3 ] = nRawStart + x + 1;					// v1
			m_pIndices[ nOffset + 4 ] = nRawStart + x + SIDE_VTX_COUNT + 1;	// v2
			m_pIndices[ nOffset + 5 ] = nRawStart + x + SIDE_VTX_COUNT;		// v3
		}
	}

	pLocked = m_pIB->Lock( 0, sizeof ( short ) * nIndexCount );
	memcpy( pLocked, m_pIndices, sizeof ( short ) * nIndexCount );
	m_pIB->Unlock();
}


void CLandTile::SetMaterial( uint nCell, IMaterial * pMat )
{
	DEBUG_ASSERT( nCell < TILE_VTX_COUNT )

		if ( nCell < TILE_VTX_COUNT )
		{
			index_t nCurMat = INVALID_INDEX;

			while ( m_pMaterials.Iterate( nCurMat ) )
			{
				if ( pMat == m_pMaterials[ nCurMat ] )
				{
					// Такой материал уже есть в тайле
					m_pCells[ nCell ] = nCurMat;
					return;
				}
			}

			// Такого материала ещё нет в тайле
			nCurMat = m_pMaterials.Add( pMat );

			if ( INVALID_INDEX != nCurMat )
			{
				// В тайле не должно быть более 256 материалов
				// ( размерность байта)
				DEBUG_ASSERT( nCurMat < 256 )

					if ( nCurMat < 256 )
					{
						m_pCells[ nCell ] = nCurMat;
					}
			}
		}
}


void CLandTile::Trace( vec3 * pRay, mat4 * pSpace )
{

	for ( uint y = 0; y < TILE_DIVS; ++y )
	{
		for ( uint x = 0; x < TILE_DIVS; ++x )
		{
			bbox bBox = *( m_pTraceBoxes + ( ( y * TILE_DIVS ) + x ) );

			if ( bBox.RayHitTest( pRay ) )
			{
				bBox.Translate( *pSpace );
				g_pEngine->PushBBox( bBox, 0xFFFFFFFF );

				const float fBoxSide = (float)SIDE_CELL_COUNT / (float)TILE_DIVS;

				for ( uint boxZ = 0; boxZ < fBoxSide; ++boxZ )
				{
					for ( uint boxX = 0; boxX < fBoxSide; ++boxX )
					{
						uint n = ( y * fBoxSide * SIDE_CELL_COUNT );	// начало тайла
						n += x * fBoxSide;
						n += boxZ * SIDE_CELL_COUNT;					// начало строки
						n += boxX;										// треугольник

						const uint		nCurTri = n * 3 * 2;
						const short *	pVertID = m_pIndices + nCurTri;
						vec3			vTriangle[ 3 ];

						vTriangle[ 0 ] = m_pVertices[ *pVertID ].vPos;
						pVertID++;

						vTriangle[ 1 ] = m_pVertices[ *pVertID ].vPos;
						pVertID++;

						vTriangle[ 2 ] = m_pVertices[ *pVertID ].vPos;


						if ( RayTriangle( pRay, vTriangle ) )
						{
							vTriangle[ 0 ].Multiply( *pSpace );
							vTriangle[ 1 ].Multiply( *pSpace );
							vTriangle[ 2 ].Multiply( *pSpace );

							vTriangle[ 0 ].y += 0.1f;
							vTriangle[ 1 ].y += 0.1f;
							vTriangle[ 2 ].y += 0.1f;

							g_pEngine->PushLine( vTriangle[ 0 ], vTriangle[ 1 ], 0xFFFFFFFF );
							g_pEngine->PushLine( vTriangle[ 1 ], vTriangle[ 2 ], 0xFFFFFFFF );
							g_pEngine->PushLine( vTriangle[ 2 ], vTriangle[ 0 ], 0xFFFFFFFF );
						}
					}
				}
			}
		}
	}
/*
	const uint nTriCount = SIDE_CELL_COUNT * SIDE_CELL_COUNT * 2;

	for ( uint n = 0; n < nTriCount; ++n )
	{
		vec3			vTriangle[ 3 ];
		const uint		nCurTri = n * 3;
		const short *	pVertID = m_pIndices + nCurTri;

		vTriangle[ 0 ] = m_pVertices[ *pVertID ].vPos;
		pVertID++;

		vTriangle[ 1 ] = m_pVertices[ *pVertID ].vPos;
		pVertID++;

		vTriangle[ 2 ] = m_pVertices[ *pVertID ].vPos;

		if ( RayTriangle( pRay, vTriangle ) )
		{
			vTriangle[ 0 ].Multiply( *pSpace );
			vTriangle[ 1 ].Multiply( *pSpace );
			vTriangle[ 2 ].Multiply( *pSpace );

			vTriangle[ 0 ].y += 0.1f;
			vTriangle[ 1 ].y += 0.1f;
			vTriangle[ 2 ].y += 0.1f;

			//g_pRenderer->SetSampler( 0, g_pRenderer->GetSysTexture( EST_WHITE ) );

			g_pEngine->PushLine( vTriangle[ 0 ], vTriangle[ 1 ], 0xFFFFFFFF );
			g_pEngine->PushLine( vTriangle[ 1 ], vTriangle[ 2 ], 0xFFFFFFFF );
			g_pEngine->PushLine( vTriangle[ 2 ], vTriangle[ 0 ], 0xFFFFFFFF );
		}
	}
	*/
}


void CLandTile::Render()
{

}