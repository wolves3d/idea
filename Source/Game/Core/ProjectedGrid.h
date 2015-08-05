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

		PTexture GetFrameTexture(uint nColor);

	private:

		struct GridVertex
		{
			vec3 vPos;
			/*
			vec2 vTex;

			vec3 vTangent;
			vec3 vBitangent;
			vec3 vNormal;
			*/
		};

		void Update();
		void Project();
		float HeightFunc(float x, float z);

		void UpdateNormals(GridVertex * vertexArray);
		uint m_gridWidth;
		uint m_gridHeight;
		uint m_gridVertexCount;

		IRenderTarget * m_frameBuffer;
		IMaterial * m_frameMaterial;

		IVertexDecl * m_vertexDecl;
		IVertexBuffer * m_VAO;
		IIndexBuffer * m_indexBuffer;
		IMaterial * m_Material;

		CUniform m_mvpUniform;
		CUniform m_uniformModelViewMatrix;
		CUniform m_lightPos;
		CUniform m_eyePos;
		CUniform m_uniformIVP;
		CUniform m_shift;

		CUniform m_waveSetup;
		CUniform m_waveDirs;
		CUniform m_waveParams;

		ICamera * m_pCamera;
		ICamera * m_pProjector2;
		vec3 m_lookPoint;

		mat4 m_rangeMatrix;
		mat4 m_invViewProj;
		mat4 m_projector;

		vec3 m_topLeftHit, m_topRightHit, m_bottomLeftHit, m_bottomRightHit;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __ProjectedGrid_h_included__

////////////////////////////////////////////////////////////////////////////////