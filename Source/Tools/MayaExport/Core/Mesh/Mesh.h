////////////////////////////////////////////////////////////////////////////////

/**
*
*/
struct TMayaSurface
{
	MString			sName;			/// Имя назначенного материала
	CUintVector		pIndices;		/// Индексы вершин
	CUintVector *	ppUvIndices;	/// Индексы текстурных координат
};


/**
*
*/
struct TUVSet
{
	MString		sName;		/// Имя UVset'а
	MFloatArray pU;			/// Массив текстурных координат
	MFloatArray pV;			/// Массив текстурных координат
};


/**
*
*/
class CMayaMesh : public CMayaObject
{
	//--------------------------------------------------------------------------
	// Tip: Public fields & methods
	//--------------------------------------------------------------------------
	public:

	CMayaMesh	( MObject object );
	~CMayaMesh	();

	uint			GetVertexCount	();
	bool			Save			( FILE * pFile );
	TMayaSurface *	ExtractSurfaces	( uint * pOutSurfCount = NULL, MObject * pObject = NULL );
	bool			ExtractVertices	();
	bool			ExtractUVSets	();

	//--------------------------------------------------------------------------
	// Tip: Inline methods
	//--------------------------------------------------------------------------
	const TUVSet * GetUVSet( uint nUVSet )
	{
		if ( nUVSet < m_nUvSetCount )
			return m_pUvSets + nUVSet;

		return NULL;
	}

	const MFloatPointArray * GetVertices()
	{
		if ( !m_pVertices.length() )
			ExtractVertices();

		return & m_pVertices;
	}

	const TMayaSurface * GetSurface( uint nSurf )
	{
		if ( nSurf < GetSurfCount() )
			return & m_pSurfaces[ nSurf ];

		return NULL;
	}

	uint GetSurfCount()
	{
		if ( !m_nSurfCount )
			ExtractSurfaces();

		return m_nSurfCount;
	}

	bool CMayaMesh::IsValid() const
	{
		return m_bValid;
	}

	IShape::EType CMayaMesh::GetType() const
	{
		return IShape::ST_MESH;
	}

	//--------------------------------------------------------------------------
	// Tip: Private fields & methods
	//--------------------------------------------------------------------------
	private:

	void			PolysToSurfaces	( MIntArray pMaterialIndices );
	bool			FillMeshData	();

	//--------------------------------------------------------------------------
	// Tip: Fields
	//--------------------------------------------------------------------------
	bool				m_bValid;

	MFloatPointArray	m_pVertices;
	//MFloatPointArray pNormals;
	//MFloatPointArray pWeigths;

	uint				m_nUvSetCount;
	TUVSet *			m_pUvSets;

	uint				m_nSurfCount;
	TMayaSurface *		m_pSurfaces;

	TMeshData			m_MeshData;
	MFnMesh				m_FnMesh;
};

////////////////////////////////////////////////////////////////////////////////