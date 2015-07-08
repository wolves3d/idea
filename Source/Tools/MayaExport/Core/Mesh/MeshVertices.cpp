////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Mesh.h"

/**
* GetVertexCount
* ���������� ���-�� ������ � mesh'� ( ������ ��� ���-�� UV-��������� mesh'� )
*/
uint CMayaMesh::GetVertexCount()
{
	uint nVertexCount = m_MeshData.GetVertexCount();

	// ������� �������� ������ ���� ��� ��� �� ���������
	if ( !nVertexCount )
	{
		// ��������� ����� UVSet'��
		MStringArray UVSets;
		m_FnMesh.getUVSetNames( UVSets );

		// This is an annoying hack. ���� ���� mesh �� ��������
		// UV-���������, �� �� ����� ����� ������� UVSet.
		// �� �����, �������������, ���������� ��������� ���������� ���������
		// � ������ UVSet'�
		if ( !UVSets.length() || !m_FnMesh.numUVs( UVSets[ 0 ] ) )
		{
			// ������ mesh ������ �� �������� UV-���������,
			// ��������� ���-�� ������.
			m_MeshData.tHeader.nVertexCount = (uint)m_FnMesh.numVertices();
		}
		else
		{
			// �� ����, �� ���� UVSet'�� ���-�� UV-��������� ������ ���������.
			// �� ����� ���������� ����� ��������� ��������� � ������ UVSet'�
			nVertexCount = (uint)m_FnMesh.numUVs( UVSets[ 0 ] );
			m_MeshData.tHeader.nVertexCount = nVertexCount;
		}
	}

	return nVertexCount; //m_MeshData.GetVertexCount();
}


/**
*
*/
bool CMayaMesh::ExtractVertices()
{
	// Fetch Vertices
	return m_FnMesh.getPoints( m_pVertices, MSpace::kObject );

	// Fetch Binormals
	//fnMesh.getNormals( BiNormals, MSpace::kWorld );
}


/**
*
*/
bool CMayaMesh::ExtractUVSets()
{
	// ����� ����� ���� UVset'��
	MStringArray pSetNames;
	m_FnMesh.getUVSetNames( pSetNames );

	// ����� ���������� UVset'��
	m_nUvSetCount = pSetNames.length();

	// ���� � mesh'� ���� ���� ( ��� ����� ) UVset
	if ( m_nUvSetCount )
	{
		// ������ ������ UVset'��
		if ( m_pUvSets = NEW TUVSet [ m_nUvSetCount ] )
		{
			// ���������� ������ UVset
			for ( uint n = 0; n < m_nUvSetCount; ++n )
			{
				// ����������� ������ ( ��� �������� )
				TUVSet & tUVset = m_pUvSets[ n ];

				// �������� �������� UVSet'�
				tUVset.sName = pSetNames[ n ];

				// �������� UV-���������� �� UVSet'�
				m_FnMesh.getUVs( tUVset.pU, tUVset.pV, &tUVset.sName );
			}
		}
	}

	return true;
}