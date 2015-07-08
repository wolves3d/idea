////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Mesh.h"

/**
*
*/
MString GetShaderName( MObject shadingEngine )
{
	// attach a function set to the shading engine
	MFnDependencyNode fn( shadingEngine );

	// get access to the surfaceShader attribute. This will be connected to
	// lambert , phong nodes etc.
	MPlug sshader = fn.findPlug("surfaceShader");

	// will hold the connections to the surfaceShader attribute
	MPlugArray materials;

	// get the material connected to the surface shader
	sshader.connectedTo(materials,true,false);

	// if we found a material
	if(materials.length())
	{
		MFnDependencyNode fnMat(materials[0].node());
		return fnMat.name();
	}
	return "none";
}


/**
*
*/
void GetPolyIndices( CUintVector & pOutIndices, MItMeshPolygon & itPoly )
{
	// ����� ���������� ������������� � ������� ��������
	int numTriangles;
	itPoly.numTriangles( numTriangles );

	// ���������� ������ ����������� �������� ��������
	while( numTriangles-- )
	{
		MPointArray	nonTweaked;
		MIntArray	triIndices;

		// ���� ������� ������ ������������
		itPoly.getTriangle(
			numTriangles,		// ����� ������������
			nonTweaked,			// ???
			triIndices,			// ������ �������� ������������
			MSpace::kObject );

		// ��������� ������� � ��������� ( ������ )
		pOutIndices.AddToTail( triIndices[ 0 ] );
		pOutIndices.AddToTail( triIndices[ 1 ] );
		pOutIndices.AddToTail( triIndices[ 2 ] );
	}
}


/*
* ������������ ��� ������� [������� ������ ��������] �
* [������� ������ ������������]
* ���������� ������� �������� ��������, ������� ���������
* � ��������� ������������... �� �� �������? =)
*/
MIntArray GetLocalIndices( MIntArray & polyIndices, MIntArray & triIndices )
{
	MIntArray   localIndex;

	//assert ( triVertices.length() == 3 );    // Should always deal with a triangle

	for ( uint i = 0; i < triIndices.length(); ++i )
	{
		for ( uint j = 0; j < polyIndices.length(); ++j )
		{
			// ���� [������ ������������] ��������� � [�������� ��������],
			// �� ��������� ����� ����� �������
			if ( triIndices[ i ] == polyIndices[ j ] )
			{
				localIndex.append( j );
				break;
			}
		}

		// if nothing was added, add default "no match"
		// localIndex.append( -1 );
	}

	return localIndex;
}


/**
* GetPolyUvIndices
*
* �������� ������� ���������� ��������� �� ��������, ���������
* ���������� itPoly, ��� ������ ���������� ��������� pUVSet
*/
void GetPolyUvIndices( CUintVector & pOutIndices, MItMeshPolygon & itPoly, const MString * pUVSet )
{
	// ���� ������� ������ ��� �������� �������� ( �� ������������! )
	MIntArray polyIndices;
	itPoly.getVertices( polyIndices );

	// ����� ���������� ������������� � ������� ��������
	int numTriangles;
	itPoly.numTriangles( numTriangles );

	// ���������� ������ ����������� �������� ��������
	while( numTriangles-- )
	{
		MPointArray	nonTweaked;
		MIntArray	triIndices;

		// ���� ������� ������ ������������
		itPoly.getTriangle(
			numTriangles,		// ����� ������������
			nonTweaked,			// ???
			triIndices,			// ������ �������� ������������
			MSpace::kObject );

		// ������� ���������� ��������� ��������� ������������ ��������
		// ������� ������� ���������� ���������...
		MIntArray localIndex = GetLocalIndices( polyIndices, triIndices );

		if ( 3 == localIndex.length() )
		{
			// ��� ������� ������� ������������
			for ( int n = 0; n < 3; ++n )
			{
				int nUVIndex;

				// ���� ������ ���������� �������
				itPoly.getUVIndex( localIndex[ n ], nUVIndex, pUVSet );

				// ��������� ������ � ��������� ( ������ )
				pOutIndices.AddToTail( nUVIndex );
			}
		}
	}
}


/**
*
*/
void CMayaMesh::PolysToSurfaces( MIntArray pMaterialIndices )
{	
	// ������������ �������� ��������� � mesh'�
	MItMeshPolygon itPoly( m_Object );

	// ���������� ������ ������� mesh'�
	while ( !itPoly.isDone() )
	{
		// �������� ����� �������� ��������
		uint nPolyID = itPoly.index();

		// �������� ������������ ������ ��������
		if ( nPolyID >= pMaterialIndices.length() )
		{
			// ����� �������� �������� �� ������!
			continue;
		}

		// �������� ������ ��������� ������������ �������� ��������
		uint nSurfID = pMaterialIndices[ nPolyID ];

		// �������� ������� ���������
		if ( nSurfID >= m_nSurfCount )
		{
			// �������� �������� �� �������� �� ����� ��������.
			// ������ ������� ������ �� ����� �������������.
			// ����, ��� ������ =)
			DEBUG_ASSERT( !"no material assigned to poly" );
			itPoly.next();
			continue;
		}

		TMayaSurface & tSurface = m_pSurfaces[ nSurfID ];

		// �������� ������� ������ ���� ������������� �������� ��������
		GetPolyIndices( tSurface.pIndices, itPoly );

		if ( tSurface.ppUvIndices )
		{
			// �������� ������� ���������� ������ ��� ������� UVset'�
			for ( uint nUV = 0; nUV < m_nUvSetCount; ++nUV )
			{
				GetPolyUvIndices(	tSurface.ppUvIndices[ nUV ],
									itPoly, &m_pUvSets[ nUV ].sName );
			}
		}

		// ��������� � ���������� ��������
		itPoly.next();
	}
}


/**
*
*/
TMayaSurface * CMayaMesh::ExtractSurfaces( uint * pOutSurfCount, MObject * pObject )
{
	MFnMesh			fnMesh( m_FnMesh );
	TMayaSurface *	pSurfaces		= NULL;

	if ( pOutSurfCount )
		(*pOutSurfCount) = 0;

	// ������������ ����� ������ ���� ������
	if ( pObject )
	{
		if ( !fnMesh.setObject( *pObject ) )
		{
			DEBUG_ASSERT( !"wrong object, has no MFnMesh functionality" );
			return NULL;
		}
	}

	if ( !fnMesh.parentCount() )
	{
		// �� ���� ��� ����� ����� ����, �� ����� ������������,
		// ����� ��������
		DEBUG_ASSERT( !"��� �� ������ ����" );
		return NULL;
	}

	//for( uint nInstance = 0; nInstance < m_FnMesh.parentCount(); ++nInstance )
	uint nInstance = 0;
	{
		// ������ ������ �� shader'�, ������ ��������� ������� mesh'�
		MObjectArray Shaders;

		// ���� ������ ����� ������� ������� ��������� ��� �������
		// ��������. ������ ������� ����� ��������� ���������� ���������,
		// ���������� ����� �������� ����� ��������� �� ����� �������� � mesh'�,
		// � �������� �������� - ������ ��������� ������������ ������� ��������
		MIntArray MatIndices;

		// �������� ����� shader'�� � ��������� ������ ������
		// ��������� �� ���������
		fnMesh.getConnectedShaders( nInstance, Shaders, MatIndices );

		// ����� ���������� shader'��( ���������� )
		uint nSurfCount = Shaders.length();

		if ( nSurfCount )
		{
			if ( pSurfaces = NEW TMayaSurface [ nSurfCount ] )
			{
				// ���������� ���������� surface'��
				if ( pOutSurfCount )
				{
					(*pOutSurfCount) = nSurfCount;
				}
				else
				{
					m_nSurfCount = nSurfCount;
				}

				// ���� ������������ �� ������ ���� ������, ��
				// ��������� ��������� � ��������� ������
				if ( NULL == pObject )
					m_pSurfaces = pSurfaces;

				// ��������� ����� ������� surface'�
				while ( nSurfCount-- )
				{
					TMayaSurface & tSurf = m_pSurfaces[ nSurfCount ];

					// �������� �������� ���������
					MString sShaderName = GetShaderName( Shaders[ nSurfCount ] );

					// ��������� ��� surface'�
					tSurf.sName = sShaderName;

					tSurf.ppUvIndices = NEW CUintVector [ m_nUvSetCount ];
				}

				PolysToSurfaces( MatIndices );
			}
		}
	}

	return pSurfaces;
}

////////////////////////////////////////////////////////////////////////////////