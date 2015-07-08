//! @defgroup landscape_group Landscape
//! @ingroup engine_group
//! �������� �������� ��������� �� ����� �����
//! \image html Landscape.jpg
//! ���������� � ���������:
//! - �������� ����� ���� ����������� (������������ �� ������)
//! - ������ ������ � �������� ������ ��������� � �����
//! ������� ���������:
//! - ������� �������� "������" ����� ����� �� 4 ��������� �� ���
//! - ����� ���������
//! - ����� ������ (� ������� ��������)


//! ��������� ������ ���������
struct TLandVertex
{
	vec3 vPos;		//!< ������� �������
	vec2 vUV;		//!< ���������� ����������
	vec3 vNormal;	//!< �������
	vec4 vInfluence;//!< ������� ������� �� 4-� ��������� ����������
};


//! ����� ����� �������� 32x32 ������
//! @ingroup landscape_group
//! @todo �������� ����������
//! @todo ���������� VB & IB
class CLandTile
{
public:

	CLandTile();
	~CLandTile();


	void Create( byte * pHeightmap, uint nStride );

	//! ��������� ��������� � ������������ ������
	void SetMaterial( uint nCell, IMaterial * pMat );

	void Trace( vec3 * pRay, mat4 * pSpace );

	void Render();

	//private:

		enum
		{
			// ���������� ������ (edge/�����)
			SIDE_CELL_COUNT = 32,

			// (auto) ���������� ������ (edge/�����)
			SIDE_VTX_COUNT = SIDE_CELL_COUNT + 1,

			// (auto) ����� ���������� ������ �� ����
			TILE_VTX_COUNT = SIDE_VTX_COUNT * SIDE_VTX_COUNT,

			// ���������� ����������� bbox'��
			TILE_DIVS = 4
		};

		CContainer <IMaterial *> m_pMaterials;

		byte			m_pCells[ TILE_VTX_COUNT ];	//!< ������� ����������
		TLandVertex *	m_pVertices;
		short *			m_pIndices;
		PVertexBuffer 	m_pVB;
		IIndexBuffer *	m_pIB;


		bbox			m_bbox;
		bbox *			m_pTraceBoxes;
};


//! ����� �������� ���������
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