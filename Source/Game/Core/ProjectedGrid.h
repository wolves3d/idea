////////////////////////////////////////////////////////////////////////////////

#ifndef __ProjectedGrid_h_included__
#define __ProjectedGrid_h_included__

////////////////////////////////////////////////////////////////////////////////

class CProjectedGrid
{
	public:
		CProjectedGrid();
		~CProjectedGrid();
		void Init();
		void Release();
		void Render();

	private:

		void Update();

		uint m_gridWidth;
		uint m_gridHeight;
		uint m_gridVertexCount;

		IVertexDecl * m_vertexDecl;
		PVertexBuffer m_vertexBuffer;
		IMaterial * m_Material;
		CUniform m_mvpUniform;

		struct GridVertex
		{
			vec3 vPos;
			vec2 vTex;
		};

		ICamera * m_pProjector;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __ProjectedGrid_h_included__

////////////////////////////////////////////////////////////////////////////////