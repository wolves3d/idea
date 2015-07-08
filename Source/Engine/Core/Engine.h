////////////////////////////////////////////////////////////////////////////////

class CEngine : public IEngine, public IInputHandler
{
	public :

		CEngine();
		~CEngine();

		const TIntfDesc *	GetInterfaceDesc() const { return &ms_ClassDesc; }
		EResult				Init			( HINSTANCE hInst );
		void				Release			();
		void				Delete			()	{};

		void				Update			();

		// IInputHandler
		bool				ProcInputEvent	( const TInputMsg & tData )
		{
			return ProcBindings( tData );
		}

		void		Sprite2D( IMaterial * pMat, const ivec2 & vPos, const ivec2 & vSize, const vec2 * pUVPos = 0, const vec2 * pUVSize = 0 );


		static TIntfDesc ms_ClassDesc;

		IMesh *				CreateMesh();
		//IMesh *				LoadMesh		( const char * szFile );
		bool				LoadTexture		( PTexture & pOutTexture, const char *szFileName );
		IMaterial *			LoadMaterial	( const char *szFileName );

		IBaseCtrl *		CreateCtrl( EControlClass eClass, IBaseCtrl * pParent = NULL );

		
		void MaterialEditor();

		IParticleSystem * CreateParticleSys();

		float GetTime() const
		{
			LARGE_INTEGER nCount;
			QueryPerformanceCounter( &nCount );

			return (float)((double)( nCount.QuadPart - m_nStartPerfCount.QuadPart ) /
				(double)m_nPerfCounterFreq.QuadPart);
		}

		IEasyWin *		GetWindow	()
		{
			return m_pWin;
		}

		IUIManager * GetUIManager();

		IImageManager *		GetImageManager	()
		{
			return m_pImageManager;
		}

		IResManager *	GetResManager()
		{
			return m_pResManager;
		}

		IEntityManager * GetEntityManager();

		void PushLine	( const vec3 & vStart, const vec3 & vEnd, dword dwColor = 0xFFFFFFFF);
		void PushBBox	( const bbox & box, dword dwColor );
		void FlushLines	( const mat4 & mWVP );

		IMaterial *		CreateMaterial();
		IModel *		CreateModel();
		ILandscape *	CreateLandscape();

		ICamera *		CreateCamera	();
		void			SetCamera		( ICamera * pCamera );
		const ICamera *	GetCamera		();

		IMaterial *	GetDefMaterial() { return m_pDefMaterial; }

	//private :

		void RegisterCommand	( const TCommand & tCmd );
		void RegisterCommands	();
		void Command			( const char * szCommandLine );
		CHashTable <TCommand> 	m_pCommandHash;
		CStack <TBind>			m_pBindings;

		bool ProcBindings	( const TInputMsg & tData );

		void AddBind( dword dwCode, const char * szString )
		{
			TBind tBind;
			tBind.dwCode = dwCode;
			tBind.sCommand = szString;
			m_pBindings.Push( tBind );
		}

		IEasyWin * m_pWin;

		IUIManager *	m_pUIManager;
		IImageManager * m_pImageManager;
		class CEntityManager * m_pEntityManager;

		class CMaterialEditor * m_pMaterialEditor;
		IResManager *		m_pResManager;
//		EResult LoadModel( const char *szModelName );

		IMaterial *		m_pDefMaterial;

		LARGE_INTEGER	m_nPerfCounterFreq,
						m_nStartPerfCount;

		ICamera * m_pCamera;

		//----------------------------------------------------------------------
		// Tip: Lines
		//----------------------------------------------------------------------

		struct TLines
		{
			struct TLineVertex
			{
				vec3	vPoint;
				vec4	vColor;
			} * pLines;

			uint			nMaxLines;
			uint			nNumLines;
			PVertexBuffer	pLinesBuffer;
			uint			nBufferSize;
			IVertexDecl *	pVDecl;
			IMaterial *		pMaterial;
		} m_lines;
};

////////////////////////////////////////////////////////////////////////////////

extern CEngine *	g_pEngine;
extern IRenderer *	g_pRenderer;
extern IVfs *		g_pVfs;
extern IInput *		g_pInput;

////////////////////////////////////////////////////////////////////////////////