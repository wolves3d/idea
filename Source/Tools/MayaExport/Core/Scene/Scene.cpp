////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MDagPathArray.h>
#include<maya/MFnSkinCluster.h>

#include "../Mesh/Mesh.h"
#include "../Joints/Joints.h"
#include "../Skin/Skin.h"

/**
*
*/
FILE * WriteModelHeader( const char * szFileName, uint nObjectCount )
{
	//--------------------------------------------------------------------------
	// Tip: �������� ����� ������
	//--------------------------------------------------------------------------
	if ( !szFileName )
	{
		// �� ����� ��������
		return false;
	}

	// ������/��������� ���� ��� ������ � �������� ������
	FILE * pFile = fopen( szFileName, "wb" );

	if ( !pFile )
	{
		// �� ������� ������� ����. ������ :(
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: ���������� ��������� ����� ������
	//--------------------------------------------------------------------------
	TModelFileHdr tModelHdr = 
	{
		'M' | 'D' << 8 | 'L' << 16 | ' ' << 24, /// ������� ����� "MDL "
		sizeof ( TModelFileHdr ),				/// ������ ���������
		1,										/// ������
		nObjectCount,					 		/// ���-�� ��������
	};

	fwrite( &tModelHdr, sizeof( TModelFileHdr ), 1, pFile );

	return pFile;
}


/**
*
*/
void EnumScene( MObject & pMesh, CVector <MObject> & pSceneObjects )
{
	MGlobal::displayInfo( "Enumerating scene" );

	//--------------------------------------------------------------------------
	// Tip: ���������� ��� SkinCluster ������� � ����� ( ������ ���� ������ ���� )
	//--------------------------------------------------------------------------
	MItDependencyNodes iter( MFn::kSkinClusterFilter );

	while ( !iter.isDone() )
	{  	
		DEBUG_ASSERT( true == pMesh.isNull() );

		pMesh = iter.thisNode();
		iter.next();
	}

	//--------------------------------------------------------------------------
	// Tip: ���������� ��� ������� � �����
	//--------------------------------------------------------------------------
	for ( MItDag itDag; !itDag.isDone(); itDag.next() )
	{
		MObject & obj = itDag.item();

		switch ( obj.apiType() )
		{
			case MFn::kJoint:
			{
				pSceneObjects.AddToTail( itDag.item() );
			}
			break;

			case MFn::kMesh:
			{
				if ( pMesh.isNull() )
					pMesh = iter.thisNode();
			}
			break;
		}
	}

/*
	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	for ( uint k = 0; k < pSkinClusters.GetCount(); ++k )
	{
		MStatus status;
		MDagPathArray pBonePaths;
		MDagPathArray pGeoms;
		MFnSkinCluster fn( pSkinClusters[ k ] );
		uint nBoneCount = fn.influenceObjects( pBonePaths, &status );

		// loop through the geometries affected by this cluster
		for ( uint n = 0; n < fn.numOutputConnections(); ++n )
		{ 	
			MDagPath skinPath;
			unsigned int index = fn.indexForOutputConnection( n ); 

			// get the dag path of the ii'th geometry
			fn.getPathAtIndex( index, skinPath );

			// ������� ������������ mesh'� ( ������� ��� skin )
			// �� ������� ��������
			MObject polyshape( skinPath.node() );

			for ( uint i = 0; i < pSceneObjects.GetCount(); ++i )
			{
				if ( polyshape == pSceneObjects[ i ] )
				{
					pSceneObjects.Remove( i );
					break;
				}
			}
		}
	}*/

	// ��������� ��� skin ������� � ����� ������
	// ���������� ����� �� �����
	/*
	while ( pSkinClusters.GetCount() )
	{
		uint nTail	= pSkinClusters.GetCount() - 1;
		pSceneObjects.AddToTail( pSkinClusters[ nTail ] );
		pSkinClusters.Remove( nTail );
	}*/

	MGlobal::displayInfo( "Scene enumerated" );
}


/**
*
*/
MStatus ExportScene()
{
	MObject pMesh;
	EnumScene( pMesh, pSceneObjects );

	DEBUG_ASSERT( false == pMesh.isNull() );

	//--------------------------------------------------------------------------
	// Tip: ��������� ����, ���������� ���������
	//--------------------------------------------------------------------------
	FILE * pFile = WriteModelHeader( "D:/maya.mesh", pSceneObjects.GetCount() );

	if ( !pFile )
	{
		return MS::kFailure;
	}

	//--------------------------------------------------------------------------
	// Tip: ��������� mesh ��� skin
	//--------------------------------------------------------------------------
	switch ( pMesh.apiType() )
	{
		case MFn::kSkinClusterFilter:
			ExportSkin( pFile, pMesh );
		break;

		case MFn::kMesh:
			ExportMesh( pFile, pMesh );
		break;

		default:
			DEBUG_ASSERT( !"stop" );
	}

	//--------------------------------------------------------------------------
	// Tip: ��������� ������ ������
	//--------------------------------------------------------------------------
	for ( uint n = 0; n < pSceneObjects.GetCount(); ++n )
	{
		MObject & obj	= pSceneObjects[ n ];

		if ( MFn::kJoint == obj.apiType() )
		{
			ExportJoint( pFile, obj );
		}
		else
		{
			DEBUG_ASSERT( !"stop" );
		}
	}

	pSceneObjects.Delete();

	// ��������� ����, �� ����, �� ������ =)
	fclose( pFile );
	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////