/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/

struct TSpriteVertex
{
	vec2 vPos;
	vec2 vUV;
};


class CUIManager : public IUIManager, public IInputHandler
{
	friend CBaseControl;
	friend CWindow;

	public :

	#define MAX_SPRITES 1024

	// IInputHandler
	bool ProcInputEvent( const TInputMsg & tData );

	CUIManager();
	~CUIManager();

	void	Render		();

	CBaseControl * GetDesktop()
	{
		return m_pDesktop;
	}

	void SetDesktop( CBaseControl * pDesktop )
	{
		m_pDesktop = pDesktop;
	}

	void InitSprites();

	bool LoadFont( const char * szFntFile, const char * szTexture  );

	//void ProcessInput();



	IBaseCtrl *	CreateCtrl	( EControlClass eClass, IBaseCtrl * pParent = NULL );
	void		RemoveCtrl	( uint nID );


	// IBaseCtrl *	CreateWin	( IBaseCtrl * pParent = NULL );
	// void		DestroyWin	( IBaseCtrl * pWin );

	void		RebuildVertexBuffer();

	const IMyFont *	GetFont( const char * szName )
	{
		return m_pDefaultFont;
	}

	inline const ivec2 & GetMousePos() const
	{
		return m_vMousePos;
	}

	inline void ShowCursor( bool bShow = true )
	{
		m_bShowCursor = bShow;
	}

	inline const CBaseControl * GetActiveControl() const
	{
		return m_pActiveControl;
	}

	inline void SetActiveControl( CBaseControl * pActiveControl )
	{
		m_pActiveControl = pActiveControl;
	}

	inline const vec2 & GetCaretPos() const
	{
		return m_vCaretPos;
	}

	inline IMaterial * GetWindowMaterial()
	{
		return m_pWindowMaterial;
	}

	inline bool PushDesktop()
	{
		m_DesktopStack.Push( m_pDesktop );
		return true;
	}

	inline bool PopDesktop()
	{
		if ( !m_DesktopStack.GetCount() )
			return false;

		m_DesktopStack.Pop( m_pDesktop );
		return true;
	}

	TSpriteVertex * GetSpritesData()
	{
		return m_pSpriteVrts;
	}


	ISprite2D *	CreateSprite();
	void		DeleteSprite( ISprite2D * pSprite );
	void		PushSprite	( ISprite2D * pSprite, bool bDrawBeforeUI = false );
	IText2D *	CreateText	();

	private :

	void InitControl( CBaseControl * pCtrl );
	void RenderSprites();

	void RenderDebugInfo();

	CContainer <CBaseControl *> m_WindowList;

	mat4 m_mProj;
	mat4 m_mWorld;

	IMaterial * m_pWindowMaterial;
	IMaterial * m_pFontMaterial;

	ISprite2D * m_pCursorSprite;
	IMaterial * m_pCursorMaterial;

	ivec2		m_vMousePos;
	bool		m_bShowCursor;

	IMyFont *		m_pDefaultFont;

	IInput *	m_pInput;
	vec2		m_vCaretPos;

	CBaseControl * m_pActiveControl;
	CBaseControl *	m_pDesktop;

	CStack <CBaseControl *>	m_DesktopStack;

	//////////////

	struct TSpriteDIP
	{
		IMaterial * pMaterial;
		uint		nIndexOffset;		// Сдвиг на первый индекс материала
		uint		nIndexCount;		// Кол-во индексов
	};

	PVertexBuffer			m_pSpritesVB;
	TSpriteVertex *			m_pSpriteVrts;
	uint					m_nVertexCount;
	uint					m_nMaxVertices;
	//CStack <ISprite2D *>	m_SpriteQueue;
	CStack <uint>			m_FreeSpriteIDs;
	ISprite2D *				m_pSprites[ MAX_SPRITES ];

	IIndexBuffer *			m_pSpriteIB;
	short *					m_pSpriteIndices;
	CVector <TSpriteDIP>	m_pSpriteDIPs;

	IVertexDecl	*			m_pTextVertexDecl;
	PVertexBuffer			m_pTextVertices;

	//////////////

	ICounter *	m_pSpriteCount;
	ICounter *	m_pSpriteDIPcount;
	IText2D *	m_pDebugText;
};

////////////////////////////////////////////////////////////////////////////////

extern CUIManager * g_pUIManager;

////////////////////////////////////////////////////////////////////////////////