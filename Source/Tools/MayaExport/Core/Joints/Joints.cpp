////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include "Joints.h"


/**
*
*/
CMayaJoint::CMayaJoint( MObject object )
{
	m_Object = object;
}


/**
*
*/
CMayaJoint::~CMayaJoint()
{
}


/**
*
*/
bool CMayaJoint::IsValid () const
{
	return true;
}


/**
*
*/
IShape::EType CMayaJoint::GetType () const
{
	return IShape::ST_BONE;
}


/**
*
*/
bool CMayaJoint::Save( FILE * pFile )
{
	//--------------------------------------------------------------------------
	// Tip: Сохранение заголовка объекта
	//--------------------------------------------------------------------------
	TObjectHdr tHeader;
	FillObjectHeader( &tHeader );

	// Пишем заголовок object'а
	fwrite( &tHeader, sizeof( TObjectHdr ), 1, pFile );

	return false;
}


/**
*
*/
void ExportJoint(FILE * pFile, MObject & obj )
{
	MObject object = obj;
	CMayaJoint joint( object );

	joint.Save( pFile );
}

////////////////////////////////////////////////////////////////////////////////
/*
struct TMayaObjectHdr : public TObjectHdr
{
	TMayaObjectHdr( MDagPath _dagPath )
	{
		eType = IShape::ST_BONE;
		//dagPath = _dagPath;

		MStatus		sResult;
		MFnDagNode	fnNode( _dagPath, &sResult );

		if ( MS::kSuccess == sResult )
		{
			CStr::Copy( szName, fnNode.name().asChar(), TObjectHdr::nMaxNameLen );
		}

		MFnTransform fnTransform( _dagPath, &sResult );

		if ( MS::kSuccess == sResult )
		{
			// copy position
			MVector mvPos = fnTransform.getTranslation( MSpace::kWorld );
			vPos.Set( (float)mvPos.x, (float)mvPos.y, (float)mvPos.z );

			// copy quat
			fnTransform.getRotationQuaternion( (double&)qRot.x, (double&)qRot.y, (double&)qRot.z, (double&)qRot.w );
		}
	}

	//MDagPath dagPath;
};


class CSkeleton
{
	public:

	enum
	{
		BonesEnum,	// блок данных, в котором перечислены кости
		Keys,	
		Version	= 0// версия скелета
	};

	~CSkeleton()
	{
		for ( uint n = 0; n < m_BoneList.GetCount(); ++n )
		{
			CObject * pBone = m_BoneList[ n ];
			DEL( pBone );
		}

		m_BoneList.Delete();
	}

	CObject * AddBone( CObject * pNewBone )
	{
		if ( pNewBone )
		{
			m_BoneList.Push( pNewBone );
		}

		return pNewBone;
	}

	CObject * GetBone( uint nID  )
	{
		return m_BoneList[ nID ];
	}

	uint GetBoneCount() const
	{
		return m_BoneList.GetCount();
	}

	private:

	char * pName;
	CStack <CObject *> m_BoneList;
};


/**
/* Recursive function!
*
void TraverseTree( const MDagPath & dagPath, CSkeleton * pSkeleton, TMayaObjectHdr * pParent )
{
	if ( !pSkeleton )
		return;

	TMayaObjectHdr * pJoint	= NEW TMayaObjectHdr( dagPath );

	if ( pJoint )
	{
		// debug
		MGlobal::displayInfo( pJoint->szName );

		if ( pParent )
		{
			CStr::Copy(
				pJoint->szParent,
				pParent->szName,
				TObjectHdr::nMaxNameLen );
		}

		// pSkeleton->AddBone( pJoint );

		for( uint i = 0; i < dagPath.childCount(); ++i )
		{
			MObject		objChild( dagPath.child( i ) );
			MFnDagNode	fnChild	( objChild );
			MDagPath	dagChildPath;

			// получаем путь к нему (в представлении Maya)
			fnChild.getPath( dagChildPath );

			TraverseTree( dagChildPath, pSkeleton, pJoint );
		}
	}
}


/**
/*
*
void BuildSkeleton( MDagPath dagRoot )
{
	CSkeleton * pSkeleton = NEW CSkeleton;
	TraverseTree( dagRoot, pSkeleton, NULL );

	MAnimControl animCtrl;

	double dStart	= animCtrl.animationStartTime().as( MTime::kFilm );
	double dEnd		= animCtrl.animationEndTime().as( MTime::kFilm );
/*
	for ( double dTime = dStart; dTime < dEnd; dTime += 1.0 )
	{
		animCtrl.setCurrentTime( MTime( dTime, MTime::kFilm ) );

		//fprintf( f, "Time: %f\n\n", dTime );

		for ( uint n = 0; n < pSkeleton->GetBoneCount(); ++n )
		{
			CMyJoint * pBone = (CMyJoint *)pSkeleton->GetBone( n );

			const char * szParent	= "no parent";
			const char * szName		= pBone->GetName();

			if ( pBone->GetParent() )
				szParent = pBone->GetParent()->GetName();

			//fprintf( f, "bone '%s' child of '%s'\n", szName, szParent );

			MFnTransform fnTrans( pBone->dagPath );

			double x, y, z, w;
			fnTrans.getRotationQuaternion( x, y, z, w );
			pBone->AddRotKey( (float)dTime, quat( (float)x, (float)y, (float)z, (float)w ) );

			if ( dTime == dStart )
			{
				// относительное перемещение
				//relativeTranslate = parentRotate.inverse() * (currentTranslate - parentTranslate);

				MVector vPos = fnTrans.getTranslation( MSpace::kWorld );

				if ( pBone->GetParent() )
				{
					CMyJoint * pParent = (CMyJoint *)pBone->GetParent();
					MFnTransform fnTrans( pParent->dagPath );
					MVector vParentPos = fnTrans.getTranslation( MSpace::kWorld );
					vPos = vPos - vParentPos;
				}

				fnTrans.getRotationQuaternion( x, y, z, w, MSpace::kObject );

				pBone->SetPos( vec3( (float)vPos.x, (float)vPos.y, (float)vPos.z ) );
				pBone->SetRot( quat( (float)x, (float)y, (float)z, (float)w ) );
			}
			//fprintf( "quat %f %f %f %f\n\n", x, y, z, w );
		}

		//fprintf( f, "----------------------------------------------------\n" );
	}

	FILE *f = fopen( "D:/maya.anim", "wb" );
	char * pBuffer = NULL;
	uint nSize = 0;

	uint nBoneCount = pSkeleton->GetBoneCount();

	fwrite( "MDL ", 4, 1, f );						// signature
	fwrite( &nBoneCount, sizeof( uint ), 1, f );	// nNumObjects;

	for ( uint n = 0; n < nBoneCount; ++n )
	{
		CObject * pBone = pSkeleton->GetBone( n );
		pBone->FinalizeKeys();
		uint nNeededSize = 0;//pBone->CalcSaveSize();

		if ( nSize < nNeededSize )
		{
			DEL( pBuffer );
			nSize = nNeededSize;
			pBuffer = NEW char [ nSize ];
		}

		if ( pBuffer )
		{
			//pBone->Save( pBuffer, nNeededSize );
			fwrite( pBuffer, nNeededSize, 1, f );
		}
	}

	DEL( pBuffer );
	fclose( f );

	DEL( pSkeleton );
	*
}


/*
================================================================================
//	Name:
//	Desc: поиск корня скелета
================================================================================
*
MObject searchRootJoint()
{
	// создать итератор
	MItDag itDag;

	// перебирать все объекты, пока не закончатся
	for ( ; !itDag.isDone(); itDag.next() )
	{
		// для начала выбираем все объекты, которые являются
		// началом для каждого из возможных поддеревьев Maya
		if ( itDag.depth() != 1 )
			continue;

		// получаем объект (в представлении Maya)
		MObject object = itDag.item();

		if ( object.hasFn( MFn::kJoint ) )
		{
			MFnDagNode temp( object );
			const char * pName = temp.name().asChar();
			MGlobal::displayInfo( "found!" );
			return object;
		}
		*else
		{
		// определяем узел DAG-дерева

		MFnDagNode fnNode( object );
		MDagPath dagPath;
		// получаем путь к нему (в представлении Maya)

		fnNode.getPath( dagPath );

		//for ( object.

		// пытаемся искать (это моя функция, которая проверяет всех
		// потомков данного объекта на наличие функций для joint'ов)
		//object = traversDagForJoints(dagPath);
		if(!object.isNull()) return object;
		}*
	}
	// не найдено ни одного объекта с нужным набором функций
	return MObject::kNullObj;
}


void ExportJoints()
{
	MObject moRoot = searchRootJoint();

	if ( MObject::kNullObj != moRoot )
	{
		MGlobal::displayInfo( "root found" );
		MDagPath dagPath;
		MFnDagNode fnNode( moRoot );
		fnNode.getPath( dagPath ); // получаем путь к нему (в представлении Maya)

		BuildSkeleton( dagPath );
	}
}*/

////////////////////////////////////////////////////////////////////////////////