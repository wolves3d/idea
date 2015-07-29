////////////////////////////////////////////////////////////////////////////////

#ifndef __Renderer_GL_h_included__
#define __Renderer_GL_h_included__


/**
*
*/
enum EGLExt
{
	EXT_GL_VBO = 0,
	EXT_GL_FBO,
	EXT_GL_VRTX_PROGRAM,
	EXT_GL_MAX
};


// Forward declarartions
class CTexture;


/**
*
*/
class CRenderer_GL : public IRenderer
{
	public :

		CRenderer_GL();
		~CRenderer_GL();

		const TIntfDesc *	GetInterfaceDesc() const;
		EResult				Init			( HWND hWindow );
		void				Release			();

		void				EnableAlphaTest	( bool bEnable = true );
		void				EnableBlend		( bool bEnable = true );
		void				EnableDepthTest	( bool bEnable = true );

		void				AlphaRef		( float fRef );
		void				AlphaFunc		( EAlphaFunc eFunc );
		bool				BlendFunc		( EBlendFactor eSrc, EBlendFactor eDst );
		void				DepthMask		( bool bMask );
		

		//--------------------------------------------------------------------------
		// Tip: Buffers
		//--------------------------------------------------------------------------
		PVertexBuffer	CreateVB		();
		void			RemoveVB		( uint nListID );

		IIndexBuffer *	CreateIB		();
		void			RemoveIB		( uint nListID );

		IVertexDecl *	CreateVertexDecl();
		IVertexDecl *	GetVertexDecl	() const;
		void			SetVertexDecl	( IVertexDecl * pDecl );

		//----------------------------------------------------------------------
		// Tip: Textures
		//----------------------------------------------------------------------
		EResult		CreateTexture	( PTexture & pTexture, const char * szName );
		CTexture *	CreateTexture(const char * szName);

		PTexture	GetSysTexture	( ESysTexture eTexture );
		void		SetDefTexture	( PTexture & pTexture );

		void		ParsePrograms( IShader * pShader, CXMLElement * pStruct );

		bool CreateShader( const char * szPath, IVertexDecl * pDecl,
			TUniform * pUnfms, uint nUnfmCount );
		IShader	*	GetShader		( const char * szShader );
		IShader	*	GetShader		( EShaderType eShaderType );
		void		SetShader		( IShader * pShader );

		bool			SetSampler		( uint nSampler, PTexture pTex );

		IRenderTarget * CreateRenderTarget	();
		void			RemoveRenderTarget	( uint nListID );
		bool			SetRenderTarget		( IRenderTarget * pRT );

		/*IMaterial *	CreateMaterial	();
		void SetActiveMaterial( CMaterial * pMat )
		{
			m_pActiveMaterial = pMat;
		}*/

		//--------------------------------------------------------------------------
		//	Tip: Utils
		//--------------------------------------------------------------------------
		void		Line2D( const vec2 & vStart, const vec2 & vEnd, dword dwColor );
		void		Line3D( const vec3 & vStart, const vec3 & vEnd, dword dwColor );

		void	BBox( const bbox & box );
		void		Axis3D( const vec3 & vOrigin );
		void		Axis3D( const mat4 & mWorld );
		void		Circle3D( const mat4 & mWorld, float fRadius, dword dwColor );

		//----------------------------------------------------------------------
		// Tip: Scene
		//----------------------------------------------------------------------
		bool		ClearScene		();
		bool		Present			();

		//----------------------------------------------------------------------
		// Tip: Matrices
		//----------------------------------------------------------------------
		void		PushMatrix	( EMatrixMode eMode );
		void		PopMatrix	( EMatrixMode eMode );
		void		SetMatrix	( EMatrixMode eMode, const mat4 & matrix );
		void		MultMatrix	( EMatrixMode eMode, const mat4 & matrix );
		const mat4 *GetMatrix	( EMatrixMode eMode );

		const ivec2 & GetViewPort() const
		{
			return m_vViewPort;
		}

		void SetViewPort( const ivec2 & vViewPort )
		{
			m_vViewPort = vViewPort;
			glViewport( 0, 0, m_vViewPort.x, m_vViewPort.y );
		}

		IShader * GetActiveShader()
		{
			return m_pActiveShader;
		}

		void SetCurrentProgram( unsigned long hProgram )
		{
			m_hCurrProgram = hProgram;
		}

		unsigned long GetCurrentProgram()
		{
			return m_hCurrProgram;
		}

		bool IsExtSupported( EGLExt eExt )
		{
			if ( eExt < EXT_GL_MAX )
				return m_bExtensions[ eExt ];

			DEBUG_ASSERT( false );
			return false;
		}


		static		TIntfDesc		ms_ClassDesc;

	private :

		void InitExtensions();
		void ReloadShaders();

		HWND	m_hWindow;		// Window handle
		HDC		m_hDevCon;		// Device Context handle
		HGLRC	m_hGLRenderCon;	// GL rendering context

		//CMaterial * m_pActiveMaterial;

		CContainer <IShader *>		m_ShaderList;
		CContainer <ITexture *>		m_TextureList;
		CContainer <PVertexBuffer>	m_VBList;
		CContainer <IIndexBuffer*>	m_IBList;
		CContainer <IRenderTarget*>	m_RTList;

		PTexture m_pSysTextures[ EST_TOTAL ];

		IShader *	m_pActiveShader;
		IShader *	m_ppShaders[ SHADER_COUNT ];

		ICounter *	m_pFrameCounter;

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

		static GLenum m_AlphaFuncTable[ AF_MAX_FUNCS ];
		static GLenum m_BlendFactorTable[ BF_MAX_FACTORS ];

		struct TRenderStat
		{
			uint nVBCount;	// кол-во созданных VB
			uint nIBCount;	// кол-во созданных IB
			uint nTexCount; // кол-во текстур
		};

		unsigned long m_hCurrProgram;

		//----------------------------------------------------------------------
		// Tip: Extensions
		//----------------------------------------------------------------------
		bool m_bExtensions[ EXT_GL_MAX ];
};

////////////////////////////////////////////////////////////////////////////////

extern CRenderer_GL * g_pRenderer;

extern GLenum g_pElemTypeGL[ LAST_TYPE ];
extern GLenum g_pElemCountGL[LAST_TYPE];

////////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG

#define GL_VALIDATE \
	do { \
	const GLenum errCode = glGetError(); \
	if (GL_NO_ERROR != errCode) \
		g_pSystem->AssertMsg(__FILE__, __LINE__, Va("OpenGL error! enumID = %#04x", errCode)); } while (false);

#else

#define GL_VALIDATE

#endif // #ifdef _DEBUG

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Renderer_GL_h_included__

////////////////////////////////////////////////////////////////////////////////