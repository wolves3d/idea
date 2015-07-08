////////////////////////////////////////////////////////////////////////////////

#ifndef __RendererD3D_h_included__
#define __RendererD3D_h_included__

////////////////////////////////////////////////////////////////////////////////

#include "d3d10.h"

////////////////////////////////////////////////////////////////////////////////

class CRendererD3D : public IRenderer
{
	public :

		CRendererD3D();

		const TIntfDesc *	GetInterfaceDesc() const;
		EResult				Init			( HWND hWindow );
		void				Release			();
		void				Delete			() { delete this; }

		void				EnableAlphaTest	( bool bEnable = true ){};
		void				EnableBlend		( bool bEnable = true ){};
		void				EnableDepthTest	( bool bEnable = true ){};

		void				AlphaRef		( float fRef ){};
		void				AlphaFunc		( EAlphaFunc eFunc ){};
		bool				BlendFunc		( EBlendFactor eSrc, EBlendFactor eDst ){ return false; }
		void				DepthMask		( bool bMask ){};
		

		bool				ReloadShaders	();

		//--------------------------------------------------------------------------
		// Tip: Buffers
		//--------------------------------------------------------------------------
		PVertexBuffer	CreateVB		();
		IIndexBuffer *	CreateIB		();

		IVertexDecl *	CreateVertexDecl();
		IVertexDecl *	GetVertexDecl	() const { return NULL; }
		void			SetVertexDecl	( IVertexDecl * pDecl ) {}

		//----------------------------------------------------------------------
		// Tip: Textures
		//----------------------------------------------------------------------
		EResult		CreateTexture	( PTexture & pTexture, const char * szName );
		PTexture	GetSysTexture	( ESysTexture eTexture ) { return m_pSysTextures[ eTexture ]; }
		void		SetDefTexture	( PTexture & pTex ){};

		void		ParsePrograms( IShader * pShader, CXMLElement * pStruct ){};
		//EResult		CreateShader	( index_t * pOutIndex );

		bool CreateShader( const char * szPath, IVertexDecl * pDecl, TUniform * pUnfms, uint nUnfmCount );

		IShader	*	GetShader		( const char * szShader );
		IShader	*	GetShader		( EShaderType eShaderType );
		void		SetShader		( IShader * pShader ){};

		bool			SetSampler		( uint nSampler, PTexture pTex ){ return false;}

		IRenderTarget * CreateRenderTarget	() { return NULL; }
		bool			SetRenderTarget		( IRenderTarget * pRT ) { return false; }

		/*IMaterial *	CreateMaterial	();
		void SetActiveMaterial( CMaterial * pMat )
		{
			m_pActiveMaterial = pMat;
		}*/

		//--------------------------------------------------------------------------
		//	Tip: Utils
		//--------------------------------------------------------------------------
		void		BBox( const bbox & box ) {};
		void		Line2D( const vec2 & vStart, const vec2 & vEnd, dword dwColor ){};
		void		Line3D( const vec3 & vStart, const vec3 & vEnd, dword dwColor ){};

		void		Axis3D( const vec3 & vOrigin ){};
		void		Axis3D( const mat4 & mWorld ){};
		void		Circle3D( const mat4 & mWorld, float fRadius, dword dwColor ){};

		//----------------------------------------------------------------------
		// Tip: Scene
		//----------------------------------------------------------------------
		bool		ClearScene		();
		bool		Present			();

		//----------------------------------------------------------------------
		// Tip: Matrices
		//----------------------------------------------------------------------
		void		PushMatrix	( EMatrixMode eMode ){};
		void		PopMatrix	( EMatrixMode eMode ){};
		void		SetMatrix	( EMatrixMode eMode, const mat4 & matrix );
		void		MultMatrix	( EMatrixMode eMode, const mat4 & matrix ){};
		const mat4 *GetMatrix	( EMatrixMode eMode ){ return NULL; }

		const ivec2 & GetViewPort() const
		{
			return m_vViewPort;
		}

		void SetViewPort( const ivec2 & vViewPort )
		{
			m_vViewPort = vViewPort;
			//glViewport( 0, 0, m_vViewPort.x, m_vViewPort.y );
		}

		IShader * GetActiveShader()
		{
			return m_pActiveShader;
		}

		static		TIntfDesc		ms_ClassDesc;

	private :

//		void InitExtensions();
//		void ReloadShaders();

		//----------------------------------------------------------------------
		// Tip: Direct3D part
		//----------------------------------------------------------------------
		IDXGISwapChain *			m_pSwapChain;
		ID3D10RenderTargetView *	m_pFrameBufferRT;


		//CMaterial * m_pActiveMaterial;

		//CContainer <IMesh *>		m_MeshList;
		CContainer <IShader *>	m_ShaderList;
		CContainer <ITexture *>		m_TextureList;
		CContainer <PVertexBuffer>	m_VBList;

		PTexture m_pSysTextures[ EST_TOTAL ];

		IShader *	m_pActiveShader;
		IShader *	m_ppShaders[ SHADER_COUNT ];

		//----------------------------------------------------------------------
		// Tip: Scene
		//----------------------------------------------------------------------
		mat4 m_mProjection;
		mat4 m_mModelView;
		mat4 m_mTexture;

		CStack <mat4> m_ProjectionStack;
		CStack <mat4> m_ModelViewStack;
		CStack <mat4> m_TextureStack;

		ivec2	m_vViewPort;

		EAlphaFunc	m_eAlphaFunc;
		float		m_fAlphaRef;

		//static GLenum m_AlphaFuncTable[ AF_MAX_FUNCS ];
		//static GLenum m_BlendFactorTable[ BF_MAX_FACTORS ];

		struct TRenderStat
		{
			uint nVBCount;	// кол-во созданных VB
			uint nIBCount;	// кол-во созданных IB
			uint nTexCount; // кол-во текстур
		};
};

////////////////////////////////////////////////////////////////////////////////

extern CRendererD3D *	g_pRenderer;
extern ID3D10Device *	g_pDevice;
extern ID3D10Debug *	g_pDeviceDbg;

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __RendererD3D_h_included__

////////////////////////////////////////////////////////////////////////////////