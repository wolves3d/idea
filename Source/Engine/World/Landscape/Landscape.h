//! @defgroup landscape_group Landscape
//! @ingroup engine_group
//! Описание плоского ландшафта по карте высот
//! \image html Landscape.jpg
//! Требования к ландшафту:
//! - ландшафт может быть бесконечным (подгружаться по тайлам)
//! - буферы вершин и индексов должны храниться в тайле
//! Порядок отрисовки:
//! - Сначала рисуются "чистые" куски тайла по 4 материала за раз
//! - Потом смешанные
//! - Потом декали (в порядке приорита)


//! Структура вершин ландшафта
struct TLandVertex
{
	vec3 vPos;		//!< Позиция вершины
	vec2 vUV;		//!< Текстурная координата
	vec3 vNormal;	//!< Нормаль
	vec4 vInfluence;//!< Влияние каждого из 4-х возможных материалов
};


//! Кусок земли размером 32x32 клетки
//! @ingroup landscape_group
//! @todo добавить деструктор
//! @todo выпиливать VB & IB
class CLandTile
{
public:

	CLandTile();
	~CLandTile();


	void Create( byte * pHeightmap, uint nStride );

	//! Установка материала в определенной клетке
	void SetMaterial( uint nCell, IMaterial * pMat );

	void Trace( vec3 * pRay, mat4 * pSpace );

	void Render();

	//private:

		enum
		{
			// количество клеток (edge/ребро)
			SIDE_CELL_COUNT = 32,

			// (auto) количество вершин (edge/ребро)
			SIDE_VTX_COUNT = SIDE_CELL_COUNT + 1,

			// (auto) общее количество вершин на тайл
			TILE_VTX_COUNT = SIDE_VTX_COUNT * SIDE_VTX_COUNT,

			// количество разделяющих bbox'ов
			TILE_DIVS = 4
		};

		CContainer <IMaterial *> m_pMaterials;

		byte			m_pCells[ TILE_VTX_COUNT ];	//!< Индексы материалов
		TLandVertex *	m_pVertices;
		short *			m_pIndices;
		PVertexBuffer 	m_pVB;
		IIndexBuffer *	m_pIB;


		bbox			m_bbox;
		bbox *			m_pTraceBoxes;
};


//! Класс плоского ландшафта
//! @ingroup landscape_group
class CLandscape : public ILandscape
{
	public:

				CLandscape	();
				~CLandscape	();
		void	Load		( const char * szFileName );
		void	Render		( ICamera * pCam );
		void	DrawStat	();

		void	SetMaterial	( IMaterial * p )
		{
			pMat = p;
			/*
			if ( pMat )
			{
				m_pShader = g_pRenderer->GetShader( LAND_SHADER );
				pMat->SetShader( m_pShader );
			}
			*/
		}

		void Trace( ICamera * pCam, const ivec2 vMouse );

		IMaterial * pMat;

	private:

		ivec2	m_vSize;
		//uint	m_nTriangleCount;
		short * m_pIndices;
		vec3 *	m_pVertices;

//		IShader * m_pShader;

		IVertexDecl * m_pVDecl;

		CLandTile * m_pTiles;
		uint		m_nTileCount;
};