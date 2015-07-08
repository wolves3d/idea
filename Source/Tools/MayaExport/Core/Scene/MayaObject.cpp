////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include <maya/MQuaternion.h>

/**
*
*/
bool CMayaObject::FillObjectHeader( TObjectHdr * pHeader )
{
	MStatus		mStatus;
	MFnDagNode	fnDagNode( m_Object, &mStatus );

	if (	( NULL == pHeader ) ||
			( MS::kSuccess != mStatus )	)
	{
		MGlobal::displayInfo( mStatus.errorString() );
		return false;
	}

	ZeroMemory( pHeader, sizeof( TObjectHdr ) );

	//--------------------------------------------------------------------------
	// Tip: Извлекаем имя объекта
	//--------------------------------------------------------------------------
	CStr::Copy( pHeader->szName, fnDagNode.name().asChar(), TObjectHdr::nMaxNameLen );

	//--------------------------------------------------------------------------
	// Tip: Извлекаем имя родителя
	//--------------------------------------------------------------------------
	if ( fnDagNode.parentCount() )
	{
		DEBUG_ASSERT( 1 == fnDagNode.parentCount() );

		MFnDagNode fnParentDagNode( fnDagNode.parent( 0 ), &mStatus );

		if ( MS::kSuccess == mStatus )
		{
			CStr::Copy(
				pHeader->szParent,
				fnParentDagNode.name().asChar(),
				TObjectHdr::nMaxNameLen );
		}
	}

	pHeader->eType = GetType();

	//--------------------------------------------------------------------------
	// Tip: Извлекаем информацию об ориентации объекта
	//--------------------------------------------------------------------------
	MFnTransform fnTransform( m_Object, &mStatus );

	if ( mStatus )
	{
		quat & qRot		= pHeader->qRot;
		vec3 & vPos		= pHeader->vPos;
		vec3 & vScale	= pHeader->vScale;

		// copy scale
		double scale[ 3 ];
		fnTransform.getScale( scale );
		vScale.Set( (float)scale[ 0 ], (float)scale[ 1 ], (float)scale[ 2 ] );

		// copy quat
		double q[4];
		fnTransform.getRotationQuaternion( q[0], q[1], q[2], q[3], MSpace::kTransform );
		qRot.Set( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );

		// copy position
		MVector mvPos = fnTransform.getTranslation( MSpace::kTransform );
		vPos.Set( (float)mvPos.x, (float)mvPos.y, (float)mvPos.z );

		MFnIkJoint fnIkJoint( m_Object, &mStatus );
		if ( mStatus )
		{
			/*
			if ( CStr::Compare( "joint1", pHeader->szName ) )
			{
				MTransformationMatrix mWorldSpcae = fnTransform.transformation( &mStatus );
				//MTransformationMatrix mBoneSpace( mWorldSpcae.asMatrixInverse() );
				mWorldSpcae.getRotationQuaternion( q[0], q[1], q[2], q[3] );
				qRot.Set( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );
			}
			else*/
			{
				MTransformationMatrix mWorldSpcae = fnTransform.transformation( &mStatus );
				MTransformationMatrix mBoneSpace( mWorldSpcae.asMatrixInverse() );
				mBoneSpace.getRotationQuaternion( q[0], q[1], q[2], q[3] );
				qRot.Set( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );
			}
		}
	}
	else
	{
		pHeader->vScale	= vec3::vOne;
		pHeader->qRot	= quat::qOne;
		pHeader->vPos	= vec3::vNull;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////