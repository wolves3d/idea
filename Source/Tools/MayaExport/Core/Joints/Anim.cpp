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
	// Tip: ���������� ��� ������� � �����
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
	// Tip: ���������� ��������� ����� ��������
	//--------------------------------------------------------------------------
	TAnimFileHdr tAnimHdr;
	ZeroMemory( &tAnimHdr, sizeof( TAnimFileHdr ) );

	tAnimHdr.nSign			=
		'A' | 'N' << 8 | 'I' << 16 | 'M' << 24;			/// ������� ����� "ANIM"
	tAnimHdr.nHeaderSize	= sizeof( TAnimFileHdr );	/// ������ ���������
	tAnimHdr.nVersion		= 1;						/// ������
	tAnimHdr.nNumObjects	= pJoints.GetCount();		/// ���-�� ��������
	tAnimHdr.fAnimTime		= (float)( dEnd  - dStart );/// ����� ��������

	FILE * pFile = fopen( "d:\\maya.anim", "wb" );

	fwrite( &tAnimHdr, sizeof( TAnimFileHdr ), 1, pFile );

	// ���������� �������� � ������ ��������
	// ���������� ������ ������ � ���������
	for ( uint n = 0; n < pJoints.GetCount(); ++n )
	{
		TJointHdr tJointHdr;
		ZeroMemory( &tJointHdr, sizeof( TJointHdr ) );

		// ���������� ����� ������� � ������ �������
		MStatus		mStatus;
		MFnDagNode	fnDagNode( pJoints[ n ], &mStatus );

		// �������� ������������ �����������
		if ( MStatus::kSuccess == mStatus )
		{
			// �������� ��� �������
			CStr::Copy(
				tJointHdr.szName,
				fnDagNode.name().asChar(),
				TJointHdr::nMaxNameLen );
		}

		// ��������� ��������� ���� �������
		tJointHdr.nScaleKeyCount	= 0;
		tJointHdr.nRotKeyCount		= (int)( tAnimHdr.fAnimTime + 0.5f );
		tJointHdr.nTransKeyCount	= 0;


		// ��������� ��������� �������
		fwrite( &tJointHdr, sizeof( TJointHdr ), 1, pFile );

		// ���������� ������ ���������������
		// todo

		// ���������� ������ ��������
		// �������� �������
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
			// ��������� ����� �����
			fwrite( &i, sizeof( uint ), 1, pFile );

			// ��������� �������� ����������� ��� ����� �����
			fwrite( &qRot, sizeof( quat ), 1, pFile );
		}

		// ���������� ������ �������������
		// todo
	}

	fclose( pFile );
	pJoints.Delete();

	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////