////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MQuaternion.h>


/**
*
*/
MStatus ExportAnim()
{
	//--------------------------------------------------------------------------
	// Tip: Перебираем все объекты в сцене
	//--------------------------------------------------------------------------
	CVector <MDagPath> pJoints;

	for ( MItDag itDag; !itDag.isDone(); itDag.next() )
	{
		if ( itDag.item().hasFn( MFn::kJoint ) || itDag.item().hasFn( MFn::kTransform ) )
		{
			MDagPath dagPath;

			if ( MS::kSuccess == itDag.getPath( dagPath ) )
			{
				pJoints.AddToTail( dagPath );
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////

	MAnimControl	animCtrl;
	double			dStart	= animCtrl.animationStartTime().as( MTime::kFilm );
	double			dEnd	= animCtrl.animationEndTime().as( MTime::kFilm );

	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка файла анимации
	//--------------------------------------------------------------------------
	TAnimFileHdr tAnimHdr;
	ZeroMemory( &tAnimHdr, sizeof( TAnimFileHdr ) );

	tAnimHdr.nSign			=
		'A' | 'N' << 8 | 'I' << 16 | 'M' << 24;			/// Подпись файла "ANIM"
	tAnimHdr.nHeaderSize	= sizeof( TAnimFileHdr );	/// Размер заголовка
	tAnimHdr.nVersion		= 1;						/// Версия
	tAnimHdr.nNumObjects	= pJoints.GetCount();		/// Кол-во объектов
	tAnimHdr.fAnimTime		= (float)( dEnd  - dStart );/// Время анимации

	FILE * pFile = fopen( "d:\\maya.anim", "wb" );

	fwrite( &tAnimHdr, sizeof( TAnimFileHdr ), 1, pFile );

	// Сохранение объектов и ключей анимации
	// Перебираем каждый объект и сохраняем
	for ( uint n = 0; n < pJoints.GetCount(); ++n )
	{
		TJointHdr tJointHdr;
		ZeroMemory( &tJointHdr, sizeof( TJointHdr ) );

		// Подключаем набор функций к нашему объекту
		MStatus		mStatus;
		MFnDagNode	fnDagNode( pJoints[ n ], &mStatus );

		// Проверям правильность подключения
		if ( MStatus::kSuccess == mStatus )
		{
			// Копируем имя объекта
			CStr::Copy(
				tJointHdr.szName,
				fnDagNode.name().asChar(),
				TJointHdr::nMaxNameLen );
		}

		// Заполняем остальные поля объекта
		tJointHdr.nScaleKeyCount	= 0;
		tJointHdr.nRotKeyCount		= (int)( tAnimHdr.fAnimTime + 0.5f );
		tJointHdr.nTransKeyCount	= 0;


		// Сохраняем заголовок объекта
		fwrite( &tJointHdr, sizeof( TJointHdr ), 1, pFile );

		// Сохранение ключей масштабирования
		// todo

		// Сохранение ключей вращения
		// текущего объекта
		for ( uint i = 0; i < tJointHdr.nRotKeyCount; ++i )
		{
			MFnTransform	fn( pJoints[ n ], &mStatus );
			MQuaternion		Rotation;

			animCtrl.setCurrentTime( MTime( (double)i, MTime::kFilm ) );
			fn.getRotation( Rotation, MSpace::kTransform );
			
			quat qRot(
				(float)Rotation.x,
				(float)Rotation.y,
				(float)Rotation.z,
				-(float)Rotation.w );
/*
			quat qO( 0, 0, 0, 1 );

			MFnIkJoint fnJ(fn.object(), &mStatus );
			if ( mStatus )
			{
				fnJ.getOrientation(Rotation);

				qO.Set(
				(float)Rotation.x,
				(float)Rotation.y,
				(float)Rotation.z,
				(float)Rotation.w );
			}

			qO.Mult( qRot );
			qRot = qO;
/*

			MTransformationMatrix mWorldSpcae = fn.transformation( &mStatus );
			MTransformationMatrix mBoneSpace( mWorldSpcae.asMatrixInverse() );
			double q[4];
			mBoneSpace.getRotationQuaternion( q[0], q[1], q[2], q[3] );
			quat qRot( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );
*/
			// Сохраняем номер кадра
			fwrite( &i, sizeof( uint ), 1, pFile );

			// Сохраняем значение кватерниона для этого кадра
			fwrite( &qRot, sizeof( quat ), 1, pFile );
		}

		// Сохранение ключей трансформации
		// todo
	}

	fclose( pFile );
	pJoints.Delete();

	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////