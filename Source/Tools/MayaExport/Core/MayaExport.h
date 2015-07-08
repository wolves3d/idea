////////////////////////////////////////////////////////////////////////////////

#ifndef __MayaExport_h_included__
#define __MayaExport_h_included__

#define _CRT_SECURE_NO_DEPRECATE

////////////////////////////////////////////////////////////////////////////////
#define REQUIRE_IOSTREAM

#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MFnPlugin.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MPointArray.h>
#include <maya/MFileIO.h>
#include <maya/MFnMesh.h>
#include <maya/MFnIkJoint.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include "Maya/MFloatPointArray.h"

////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "_Shared/Base.h"
#include "_Shared/Math/3dmath.h"
#include "_Shared/Templates/SafePointer.h"
#include "_Shared/Templates/Stack.h"
#include "_Shared/Templates/Vector.h"
#include "_Shared/Templates/String.h"

#include "_Shared/API/ISystem.h"
#include "_Shared/API/IVFS.h"
#include "_Shared/API/IRenderer.h"
#include "_Shared/API/IEngine.h"

////////////////////////////////////////////////////////////////////////////////

class CMayaObject
{
	public:

	virtual					~CMayaObject	(){};

	virtual bool			FillObjectHeader( TObjectHdr * pHeader );
	virtual bool			IsValid			() const					= 0;
	virtual IShape::EType	GetType			() const					= 0;
	virtual bool			Save			( FILE * pFile )			= 0;

	protected:

	MObject m_Object;
};

////////////////////////////////////////////////////////////////////////////////

FILE * WriteModelHeader( const char * szFileName, uint nObjectCount );
MStatus ExportScene();
MStatus ExportAnim();

/**
*
*/
void ExportSkin	( FILE * pFile, MObject & obj );
void ExportJoint( FILE * pFile, MObject & obj );
void ExportMesh	( FILE * pFile, MObject & obj );

extern CVector <MObject> pSceneObjects;

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __MayaExport_h_included__

////////////////////////////////////////////////////////////////////////////////