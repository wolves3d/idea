////////////////////////////////////////////////////////////////////////////////

#include "MayaExport.h"
#include "Mesh.h"

/**
* GetVertexCount
* Возвращает кол-во вершин в mesh'е ( Обычно это кол-во UV-координат mesh'а )
*/
uint CMayaMesh::GetVertexCount()
{
	uint nVertexCount = m_MeshData.GetVertexCount();

	// Считать вершинки только если они ещё не посчитаны
	if ( !nVertexCount )
	{
		// Считываем имена UVSet'ов
		MStringArray UVSets;
		m_FnMesh.getUVSetNames( UVSets );

		// This is an annoying hack. Даже если mesh не содержит
		// UV-координат, он всё равно может вернуть UVSet.
		// По этому, дополнительно, необходимо проверить количество координат
		// в первом UVSet'е
		if ( !UVSets.length() || !m_FnMesh.numUVs( UVSets[ 0 ] ) )
		{
			// Данный mesh вообще не сожержит UV-координат,
			// сохраняем кол-во вершин.
			m_MeshData.tHeader.nVertexCount = (uint)m_FnMesh.numVertices();
		}
		else
		{
			// По идее, во всех UVSet'ах кол-во UV-координат должно совпадать.
			// По этому возвращаем число координат указанное в первом UVSet'е
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
	// Узнаём имена всех UVset'ов
	MStringArray pSetNames;
	m_FnMesh.getUVSetNames( pSetNames );

	// Узнаём количество UVset'ов
	m_nUvSetCount = pSetNames.length();

	// Если в mesh'е есть один ( или более ) UVset
	if ( m_nUvSetCount )
	{
		// Создаём массив UVset'ов
		if ( m_pUvSets = NEW TUVSet [ m_nUvSetCount ] )
		{
			// Перебираем каждый UVset
			for ( uint n = 0; n < m_nUvSetCount; ++n )
			{
				// Настраиваем ссылку ( для удобства )
				TUVSet & tUVset = m_pUvSets[ n ];

				// Копируем название UVSet'а
				tUVset.sName = pSetNames[ n ];

				// Копируем UV-координаты из UVSet'а
				m_FnMesh.getUVs( tUVset.pU, tUVset.pV, &tUVset.sName );
			}
		}
	}

	return true;
}