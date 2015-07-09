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
		void Project();
		float HeightFunc(float x, float z);

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

		mat4 m_rangeMatrix;
		mat4 m_invViewProj;
		mat4 m_projector;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __ProjectedGrid_h_included__

////////////////////////////////////////////////////////////////////////////////