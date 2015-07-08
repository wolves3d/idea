////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Skin.h"
#include "../Mesh/Mesh.h"

#include<maya/MFnSkinCluster.h>
#include<maya/MDagPathArray.h>
#include<maya/MItGeometry.h>
#include<maya/MFloatArray.h>
#include<maya/MFloatMatrix.h>

/**
*
*/
struct TSkinVertex
{
	vec3	vPos;
	vec2	vUV;
	vec4	vSkin;
	//uint	pBones	[ MAX_BONES_PER_VERTEX ];
	//float	pWeights[ MAX_BONES_PER_VERTEX ];
};


/**
*
*/
void ExportSkin( FILE * pFile, MObject & obj )
{
	bool bError = false;
	MStatus status;
	MFnSkinCluster fn( obj, &status );

	if ( !status )
	{
		DEBUG_ASSERT( !"stop" );
		return;
	}

	// Считываем информацию о костях
	MDagPathArray pBonePaths;	/// Массив DAG-путей к костям
	fn.influenceObjects( pBonePaths, &status );

	int * pBones = NEW int [ pBonePaths.length() ];

	for ( uint n = 0; n < pBonePaths.length(); n++ )
	{
		pBones[ n ] = INVALID_INDEX;

		for ( uint i = 0; i < pSceneObjects.GetCount(); ++i )
		{
			MFnDagNode dagWorldObj( pSceneObjects[ i ] );
			MFnDagNode dagSkinBone( pBonePaths[ n ].node() );

			if ( CStr::Compare(
				dagWorldObj.name().asChar(),
				dagSkinBone.name().asChar() ) )
			{
				pBones[ n ] = i;
			}
		}

		DEBUG_ASSERT( INVALID_INDEX != pBones[ n ] );
	}

	DEBUG_ASSERT( status );

	// loop through the geometries affected by this cluster
	unsigned int nGeoms = fn.numOutputConnections();

	DEBUG_ASSERT( 1 == nGeoms );

	// Получаем путь к n-ой ( в даннос случае нулевой ) геометрии ( mesh )
	MDagPath skinPath;
	fn.getPathAtIndex( fn.indexForOutputConnection( 0 ), skinPath );

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	// Получаем указатель на итератор mesh'а
	MItGeometry		gIter	( skinPath );
	CMayaMesh		mesh	( skinPath.node() );

	mesh.ExtractUVSets();
	mesh.ExtractSurfaces();
	uint nSurfCount = mesh.GetSurfCount();

	const MFloatPointArray *	pVertices	= mesh.GetVertices();
	TSkinVertex *				pVB			= NEW TSkinVertex [ mesh.GetVertexCount() ];
	TSkinWeight *				pSkinWeights= NEW TSkinWeight [ gIter.count() ];

	//--------------------------------------------------------------------------
	// Tip: Пробегаем по каждой вершине ( для чтения весов )
	//--------------------------------------------------------------------------
	for ( uint nVertex = 0; !gIter.isDone(); gIter.next(), ++nVertex )
	{
		MObject tVertex = gIter.currentItem();

		for ( uint nW = 0; nW < MAX_BONES_PER_VERTEX; ++nW )
		{
			pSkinWeights[ nVertex ].pWeights[ nW ]	= 0;
			pSkinWeights[ nVertex ].pBones[ nW ]	= 0; 
		}

		//gIter.index();

		// Получаем веса для текущей вершины
		// 
		MFloatArray pWeights;		/// Массив веса каждой кости
		uint		nWeightCount;

		fn.getWeights( skinPath, tVertex, pWeights, nWeightCount );

		uint nu = pWeights.length();

		if 	( 0 != nWeightCount )
		{
			unsigned int numWeights=0;

			// Output the weight data for this vertex
			//
			for( unsigned int jj = 0; jj != nWeightCount; ++jj )
			{
				// ignore weights of little effect 
				if ( 0.f != pWeights[ jj ] )
				{ 	 	
					//DEBUG_ASSERT( numWeights <= MAX_BONES_PER_VERTEX );

					if ( numWeights >= MAX_BONES_PER_VERTEX )
					{
						bError = true;
						break;
					}

					pSkinWeights[ nVertex ].pBones[ numWeights ] = jj;//pBones[ jj ];
					pSkinWeights[ nVertex ].pWeights[ numWeights ] = pWeights[ jj ];

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
	// Tip:
	//--------------------------------------------------------------------------
	TVertexComponent pComponents[] =
	{
		{ "vPos",		TYPE_VEC3,	sizeof( vec3 )	},
		{ "vTex0",		TYPE_VEC2,	sizeof( vec2 )	},
		{ "vSkin",		TYPE_VEC4,	sizeof( vec4 )	},
		//{ "nBone0",		TYPE_INT,	sizeof( int )	},
		//{ "nBone1",		TYPE_INT,	sizeof( int )	},
		//{ "fWeight0",	TYPE_FLOAT,	sizeof( float )	},
		//{ "fWeight1",	TYPE_FLOAT,	sizeof( float )	}
	};

	TMeshData tMeshData;

	tMeshData.tHeader.nComponentCount	= 3;
	tMeshData.tHeader.nVertexSize		= sizeof( TSkinVertex );
	tMeshData.pComponents				= pComponents;
	tMeshData.pVertices					= pVB;
	tMeshData.pSurfaces					= NEW TSurface [ nSurfCount ];

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	float mf[4][4];
	skinPath.inclusiveMatrix().get( mf );
	MFloatMatrix m ( mf );

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	for ( uint nSurf = 0; nSurf < nSurfCount; ++nSurf )
	{
		const TMayaSurface *	pSurf		= mesh.GetSurface( nSurf );
		const TUVSet *			pUVSet		= mesh.GetUVSet( 0 );
		CUintVector &			tUVindices	= pSurf->ppUvIndices[ 0 ];

		for ( uint nIndex = 0; nIndex < pSurf->pIndices.GetCount(); ++nIndex )
		{
			uint nUVCoordID	= tUVindices[ nIndex ];
			uint nVertexID	= pSurf->pIndices[ nIndex ];

			MFloatPoint v = (*pVertices)[ nVertexID ];
			v *= m;

			TSkinVertex & tCurVertex = pVB[ nUVCoordID ];

			tCurVertex.vPos.Set(	v.x,
										v.y,
										v.z );

			tCurVertex.vSkin.x = (scalar)pSkinWeights[ nVertexID ].pBones[ 0 ];
			tCurVertex.vSkin.y = (scalar)pSkinWeights[ nVertexID ].pWeights[ 0 ];
			tCurVertex.vSkin.z = (scalar)pSkinWeights[ nVertexID ].pBones[ 1 ];
			tCurVertex.vSkin.w = (scalar)pSkinWeights[ nVertexID ].pWeights[ 1 ];

			/*for ( uint j = 0; j < MAX_BONES_PER_VERTEX; ++j )
			{
				pVB[ nUVCoordID ].pBones[ j ] =
					pSkinWeights[ nVertexID ].pBones[ j ];

				pVB[ nUVCoordID ].pWeights[ j ] =
					pSkinWeights[ nVertexID ].pWeights[ j ];
			}*/

			tCurVertex.vUV.Set(
				pUVSet->pU[ nUVCoordID ],
				pUVSet->pV[ nUVCoordID ] );
		}

		////////////////////////////////////////////////////////////////////////
		TSurface & tSurface = tMeshData.pSurfaces[ nSurf ];

		tSurface.tHeader.nIndexSize		= sizeof( uint );
		tSurface.tHeader.nIndexCount	= pSurf->pIndices.GetCount();
		tSurface.pIndices				= tUVindices.GetArray();

		strcpy( tSurface.tHeader.szName, pSurf->sName.asChar() );
	}

	DEL_ARRAY( pBones );
	DEL_ARRAY( pSkinWeights );


	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка объекта
	//--------------------------------------------------------------------------
	TObjectHdr tHeader;
	ZeroMemory( &tHeader, sizeof( TObjectHdr ) );

	tHeader.eType	= IShape::ST_SKIN;
	tHeader.vScale	= vec3::vOne;
	tHeader.qRot	= quat::qOne;
	tHeader.vPos	= vec3::vNull;
	CStr::Copy( tHeader.szName, fn.name().asChar(), TObjectHdr::nMaxNameLen );

	tHeader.nShapeBlockSize =	sizeof( TMeshData::THeader );
	tHeader.nShapeBlockSize +=	tMeshData.tHeader.nComponentCount * sizeof( TVertexComponent );
	tHeader.nShapeBlockSize +=	mesh.GetVertexCount() * sizeof( TSkinVertex );
	tHeader.nShapeBlockSize +=	nSurfCount * sizeof( TSurface::THeader );

	// Пишем заголовок объекта
	fwrite( &tHeader, sizeof( TObjectHdr ), 1, pFile );

	//--------------------------------------------------------------------------
	// Tip: Сохранение mesh'а
	//--------------------------------------------------------------------------
	TMeshData::THeader & tMeshHdr = tMeshData.tHeader;
	tMeshHdr.nHeaderSize	= sizeof( TMeshData::THeader );
	tMeshHdr.nVertexCount	= mesh.GetVertexCount();
	tMeshHdr.nSurfCount		= mesh.GetSurfCount();

	// Пишем заголовок mesh'а
	fwrite( &tMeshData.tHeader, sizeof( TMeshData::THeader ), 1, pFile );

	// Пишем компоненты вершины
	fwrite( tMeshData.pComponents,
		tMeshData.tHeader.nComponentCount * sizeof( TVertexComponent ), 1, pFile );

	// Пишем массив вершин
	fwrite( pVB, mesh.GetVertexCount() * sizeof( TSkinVertex ), 1, pFile );

	// Записываем каждый материал ( surface )
	for ( uint n = 0; n < nSurfCount; ++n )
	{
		TSurface & tSurf = tMeshData.pSurfaces[ n ];

		// Пишем заголовок surface'а
		fwrite( &tSurf.tHeader, sizeof( TSurface::THeader ), 1, pFile );

		// Пишем индексы вершин
		fwrite( tSurf.pIndices,
			tSurf.tHeader.nIndexCount * tSurf.tHeader.nIndexSize,
			1, pFile );
	}


	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	DEL_ARRAY( pVB );

	if ( bError )
	{
		DEBUG_ASSERT( !"there were more than 2 bones per vertex" );
	}
}

////////////////////////////////////////////////////////////////////////////////