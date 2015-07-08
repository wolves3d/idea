////////////////////////////////////////////////////////////////////////////////

#ifndef __Shape_h_included__
#define __Shape_h_included__

////////////////////////////////////////////////////////////////////////////////

/*class CShape
{
	public:

	virtual void Render( CMaterial * pMaterial ) = 0;
};*/

////////////////////////////////////////////////////////////////////////////////
/*
class /*ENGINE_API CMesh : public IMesh // CShape
{
	public:

	CMesh();
	~CMesh();

	bool SetVertexBuffer( CVertexBuffer * pVB );
	size_t AddSurface( const char * szName, CIndexBuffer * pIB );

	void Render			( IMaterial * pMaterial );
	void RenderSurface	( size_t nSurf, CMaterial * pMaterial );

	inline size_t GetSurfaceCount() const
	{
		return m_pSurfaces.GetCount();
	}

	//private :

	EPrimitive					m_ePrimType;

	TMeshHeader					m_Header;		// Mesh Header
	CStack <TSurface *>			m_pSurfaces;	// Surfaces

	CVertexBuffer *				m_pVB;
	CIndexBuffer *				m_pIB;
};*/

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Shape_h_included__

////////////////////////////////////////////////////////////////////////////////