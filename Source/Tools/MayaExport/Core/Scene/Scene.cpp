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
	// Tip: Открытие файла модели
	//--------------------------------------------------------------------------
	if ( !szFileName )
	{
		// Не верны аргумент
		return false;
	}

	// Создаём/открываем файл для записи в бинарном режиме
	FILE * pFile = fopen( szFileName, "wb" );

	if ( !pFile )
	{
		// Не удалось создать файл. Ошибка :(
		return false;
	}

	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка файла модели
	//--------------------------------------------------------------------------
	TModelFileHdr tModelHdr = 
	{
		'M' | 'D' << 8 | 'L' << 16 | ' ' << 24, /// Подпись файла "MDL "
		sizeof ( TModelFileHdr ),				/// Размер заголовка
		1,										/// Версия
		nObjectCount,					 		/// Кол-во объектов
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
	// Tip: Запоминаем все SkinCluster объекты в сцене ( должен быть только один )
	//--------------------------------------------------------------------------
	MItDependencyNodes iter( MFn::kSkinClusterFilter );

	while ( !iter.isDone() )
	{  	
		DEBUG_ASSERT( true == pMesh.isNull() );

		pMesh = iter.thisNode();
		iter.next();
	}

	//--------------------------------------------------------------------------
	// Tip: Перебираем все объекты в сцене
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

			// Удаляем оригинальные mesh'ы ( входные для skin )
			// из очереди экспорта
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

	// Переносим все skin объекты в общий массив
	// разделение более не нужно
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
	// Tip: Открываем файл, записываем заголовок
	//--------------------------------------------------------------------------
	FILE * pFile = WriteModelHeader( "D:/maya.mesh", pSceneObjects.GetCount() );

	if ( !pFile )
	{
		return MS::kFailure;
	}

	//--------------------------------------------------------------------------
	// Tip: Экспортим mesh или skin
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
	// Tip: Экспортим каждый объект
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

	// Закрываем файл, по идее, всё хорошо =)
	fclose( pFile );
	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////