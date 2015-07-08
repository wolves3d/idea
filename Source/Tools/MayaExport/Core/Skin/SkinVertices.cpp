////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Skin.h"
#include "../Mesh/Mesh.h"

#include<maya/MFnSkinCluster.h>
#include<maya/MDagPathArray.h>
#include<maya/MItGeometry.h>
#include<maya/MFloatArray.h>

/*
bool CMayaSkin::SaveVertices( FILE * pFile )
{

	// Fetch Vertices
	m_FnMesh.getPoints( m_pVertices, MSpace::kObject );



	mesh.GetSurfCount();

	// Пробегаем по каждой вершине
	for ( uint nVertex = 0; !gIter.isDone(); gIter.next(), ++nVertex )
	{
		MObject tVertex = gIter.component();

		// Получаем веса для текущей вершины
		// 
		MFloatArray pWeights;		/// Массив веса каждой кости
		uint		nWeightCount;

		fn.getWeights( skinPath, tVertex, pWeights, nWeightCount );

		uint nu = pWeights.length();

		//DEBUG_ASSERT( nWeightCount <= MAX_BONES_PER_VERTEX );

		if 	( 0 != nWeightCount )//	&& 
			//					( gIter.index() == ii ) && 
			//!gIter.isDone() )
		{
			unsigned int numWeights=0;
			float outWts[ MAX_BONES_PER_VERTEX ]={1.0f,0};
			unsigned int outInfs[ MAX_BONES_PER_VERTEX ]={-1}; 

			// Output the weight data for this vertex
			//
			for( unsigned int jj = 0; jj != nWeightCount; ++jj )
			{
				// ignore weights of little effect 
				if ( 0.f != pWeights[ jj ] )
				{ 	 	
					DEBUG_ASSERT( numWeights <= MAX_BONES_PER_VERTEX );

					if ( numWeights >= MAX_BONES_PER_VERTEX )
						break;

					/*if ( 0 != numWeights )
					{
					outWts[ 0 ] -= pWeights[ jj ];
					outWts[ numWeights ] = pWeights[ jj ];
					}*

					pSkinWeights[ nVertex ].pBones[ numWeights ] = jj;
					pSkinWeights[ nVertex ].pWeights[ numWeights ] = pWeights[ jj ];

					//outInfs[ numWeights ] = jj;
					++numWeights;
				}
			}

			float fSum = 0;

			for ( uint nW = 0; nW < MAX_BONES_PER_VERTEX; ++nW )
				fSum += pSkinWeights[ nVertex ].pWeights[ nW ];

			DEBUG_ASSERT( 1.f == fSum );
		}
	}

	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка объекта
	//--------------------------------------------------------------------------
	TObjectHdr tHeader;
	ZeroMemory( &tHeader, sizeof( TObjectHdr ) );

	tHeader.eType = IShape::ST_SKIN;
	tHeader.nShapeBlockSize = mesh.GetVertexCount() * sizeof( TSkinWeight );
	CStr::Copy( tHeader.szName, fn.name().asChar(), TObjectHdr::nMaxNameLen );

	// Пишем заголовок объекта
	fwrite( &tHeader, sizeof( TObjectHdr ), 1, pFile );
}
*/
////////////////////////////////////////////////////////////////////////////////