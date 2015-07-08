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
	// Узнаём количество треугольников в текущем полигоне
	int numTriangles;
	itPoly.numTriangles( numTriangles );

	// Перебераем каждый треугольник текущего полигона
	while( numTriangles-- )
	{
		MPointArray	nonTweaked;
		MIntArray	triIndices;

		// Берём индексы вершин треугольника
		itPoly.getTriangle(
			numTriangles,		// номер треугольника
			nonTweaked,			// ???
			triIndices,			// массив индексов треугольника
			MSpace::kObject );

		// Добавляем индексы в контейнер ( вектор )
		pOutIndices.AddToTail( triIndices[ 0 ] );
		pOutIndices.AddToTail( triIndices[ 1 ] );
		pOutIndices.AddToTail( triIndices[ 2 ] );
	}
}


/*
* Сопоставляет два массива [индексы вершин полигона] и
* [индексы вершин треугольника]
* Возвращает индексы индексов полигона, которые совпадают
* с индексами треугольника... чё не понятно? =)
*/
MIntArray GetLocalIndices( MIntArray & polyIndices, MIntArray & triIndices )
{
	MIntArray   localIndex;

	//assert ( triVertices.length() == 3 );    // Should always deal with a triangle

	for ( uint i = 0; i < triIndices.length(); ++i )
	{
		for ( uint j = 0; j < polyIndices.length(); ++j )
		{
			// Если [индекс треугольника] совпадает с [индексом полигона],
			// то сохраняем номер этого индекса
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
* Выбирает индексы текстурных координат из полигона, заданного
* итератором itPoly, для набора текстурных координат pUVSet
*/
void GetPolyUvIndices( CUintVector & pOutIndices, MItMeshPolygon & itPoly, const MString * pUVSet )
{
	// Берём индексы вершин для текущего полигона ( Не треугольника! )
	MIntArray polyIndices;
	itPoly.getVertices( polyIndices );

	// Узнаём количество треугольников в текущем полигоне
	int numTriangles;
	itPoly.numTriangles( numTriangles );

	// Перебераем каждый треугольник текущего полигона
	while( numTriangles-- )
	{
		MPointArray	nonTweaked;
		MIntArray	triIndices;

		// Берём индексы вершин треугольника
		itPoly.getTriangle(
			numTriangles,		// номер треугольника
			nonTweaked,			// ???
			triIndices,			// массив индексов треугольника
			MSpace::kObject );

		// Индексы текстурных координат считаются относительно полигона
		// Находим индексы текстурных координат...
		MIntArray localIndex = GetLocalIndices( polyIndices, triIndices );

		if ( 3 == localIndex.length() )
		{
			// Для каждого индекса треугольника
			for ( int n = 0; n < 3; ++n )
			{
				int nUVIndex;

				// Берём индекс текстурной вершины
				itPoly.getUVIndex( localIndex[ n ], nUVIndex, pUVSet );

				// Добавляем индекс в контейнер ( вектор )
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
	// Присоединяем итератор полигонов к mesh'у
	MItMeshPolygon itPoly( m_Object );

	// Перебираем каждый полигон mesh'а
	while ( !itPoly.isDone() )
	{
		// Получаем номер текущего полигона
		uint nPolyID = itPoly.index();

		// Проверка правильности номера полигона
		if ( nPolyID >= pMaterialIndices.length() )
		{
			// Номер текущего полигона не верный!
			continue;
		}

		// Получаем индекс материала назначенного текущему полигону
		uint nSurfID = pMaterialIndices[ nPolyID ];

		// Проверка индекса материала
		if ( nSurfID >= m_nSurfCount )
		{
			// Текущему полигону не назначен ни какой материал.
			// Данный полигон вообще не будет экспортирован.
			// Типа, это ошибка =)
			DEBUG_ASSERT( !"no material assigned to poly" );
			itPoly.next();
			continue;
		}

		TMayaSurface & tSurface = m_pSurfaces[ nSurfID ];

		// Выбираем индексы вершин всех треугольников текущего полигона
		GetPolyIndices( tSurface.pIndices, itPoly );

		if ( tSurface.ppUvIndices )
		{
			// Выбираем индексы текстурных вершин для каждого UVset'а
			for ( uint nUV = 0; nUV < m_nUvSetCount; ++nUV )
			{
				GetPolyUvIndices(	tSurface.ppUvIndices[ nUV ],
									itPoly, &m_pUvSets[ nUV ].sName );
			}
		}

		// Переходим к следующему полигону
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

	// Пользователь может задать свой объект
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
		// Не знаю как такое может быть, но чисто теоретически,
		// такое возможно
		DEBUG_ASSERT( !"так не должно быть" );
		return NULL;
	}

	//for( uint nInstance = 0; nInstance < m_FnMesh.parentCount(); ++nInstance )
	uint nInstance = 0;
	{
		// Массив ссылок на shader'ы, котрые назначены данному mesh'у
		MObjectArray Shaders;

		// Этот массив будет хранить индексы материала для каждого
		// полигона. Размер массива будет равняться количеству полигонов,
		// порядковый номер элемента будет указывать на номер полигона в mesh'е,
		// а значение элемента - индекс материала назначенного данному полигону
		MIntArray MatIndices;

		// Получаем имена shader'ов и заполняем массив ссылок
		// полигонов на материалы
		fnMesh.getConnectedShaders( nInstance, Shaders, MatIndices );

		// Узнаём количество shader'ов( материалов )
		uint nSurfCount = Shaders.length();

		if ( nSurfCount )
		{
			if ( pSurfaces = NEW TMayaSurface [ nSurfCount ] )
			{
				// Записываем количество surface'ов
				if ( pOutSurfCount )
				{
					(*pOutSurfCount) = nSurfCount;
				}
				else
				{
					m_nSurfCount = nSurfCount;
				}

				// Если пользователь не указал свой объект, то
				// сохраняем результат в экземпляр класса
				if ( NULL == pObject )
					m_pSurfaces = pSurfaces;

				// Назначаем имена каждому surface'у
				while ( nSurfCount-- )
				{
					TMayaSurface & tSurf = m_pSurfaces[ nSurfCount ];

					// Выясняем название материала
					MString sShaderName = GetShaderName( Shaders[ nSurfCount ] );

					// Назначаем имя surface'а
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