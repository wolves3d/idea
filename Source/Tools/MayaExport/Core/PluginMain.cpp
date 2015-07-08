////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"

////////////////////////////////////////////////////////////////////////////////

#pragma comment( lib, "D:/Soft/Maya/Lib/Foundation.lib" )
#pragma comment( lib, "D:/Soft/Maya/Lib/OpenMaya.lib" )
#pragma comment( lib, "D:/Soft/Maya/Lib/OpenMayaAnim.lib" )

////////////////////////////////////////////////////////////////////////////////

#define PLUGIN_VERSION	"0.1 alpha"
#define PLUGIN_VENDOR	"William Kovner"
#define PLUGIN_CMD		"wwExportMesh"
#define PLUGIN_CMD_ANIM	"wwExportAnim"

////////////////////////////////////////////////////////////////////////////////

ISystem *	g_pSystem = NULL;
HMODULE		hSysModule;
CVector <MObject> pSceneObjects;

/**
*
*/
class CExportCommand : public MPxCommand
{
	public:

	CExportCommand()
	{
	};

	static void* creator()
	{
		return NEW CExportCommand;
	}

	virtual MStatus	doIt( const MArgList& )
	{
		return ExportScene();
	}
};


/**
*
*/
class CExportAnimCommand : public MPxCommand
{
public:

	CExportAnimCommand()
	{
	};

	static void* creator()
	{
		return NEW CExportAnimCommand;
	}

	virtual MStatus	doIt( const MArgList& )
	{
		return ExportAnim();
	}
};


/*==============================================================================
//
//
==============================================================================*/
__declspec( dllexport ) MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION, "Any", &status );

	status = plugin.registerCommand( PLUGIN_CMD, CExportCommand::creator );
	status = plugin.registerCommand( PLUGIN_CMD_ANIM, CExportAnimCommand::creator );

	/*if ( !status )
	{
		status.perror( "registerCommand" );
		return status;
	}*/

	////////////////////////////////////////////////////////////////////////////

	hSysModule = LoadLibrary( "D:/Idea/Bin/Modules/System.dll" );

	if ( hSysModule )
	{
		PROC_CREATESYSTEM SystemCreate =
			(PROC_CREATESYSTEM)GetProcAddress( hSysModule, "CreateSystem" );

		if ( SystemCreate )
		{
			if ( g_pSystem = SystemCreate() )
			{
				g_pSystem->Init( "D:/Idea/Bin/Modules" );
				return status;
			}
		}

		FreeLibrary( hSysModule );
	}

	////////////////////////////////////////////////////////////////////////////

	return MStatus( MStatus::kFailure );
}


/*==============================================================================
//
//
==============================================================================*/
__declspec( dllexport ) MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	if ( g_pSystem )
	{
		g_pSystem->Release();
		g_pSystem = NULL;
		FreeLibrary( hSysModule );
	}

	status = plugin.deregisterCommand( PLUGIN_CMD );

	if ( !status )
		status.perror( "deregisterCommand" );

	status = plugin.deregisterCommand( PLUGIN_CMD_ANIM );

	if ( !status )
		status.perror( "deregisterCommand" );

	return status;
}

////////////////////////////////////////////////////////////////////////////////