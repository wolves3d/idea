/** @defgroup renderer The Renderer Group
*  ������ �������������
*  @{
*/

////////////////////////////////////////////////////////////////////////////////

#ifndef __IRenderer_h_included__
#define __IRenderer_h_included__

////////////////////////////////////////////////////////////////////////////////

#ifdef RENDERER_IMPL

#define RENDERER_API __declspec( dllexport )

#else

#define RENDERER_API __declspec( dllimport )

#endif

////////////////////////////////////////////////////////////////////////////////


/*
================================================================================
//	Name: Texture
//	Desc:
================================================================================
*/
enum TextureFormat
{
	TEX_FMT_RGB,
	TEX_FMT_RGBA,
	TEX_FMT_RENDER_TARGET,
};


//!

//!
//!
enum EAccessType
{
	EAT_IMMUTABLE,	//! �������� ���� ���, ������ ������ �� CPU �� GPU
	EAT_CPU_WRITE,	//! CPU ����� ������
};


//!

//!
//!
struct ITexture
{
	virtual EResult		Init		( const TImage & tImage, EAccessType eAccess ) = 0;

	virtual void		Release		()										= 0;

	virtual const char *GetName		() const								= 0;
//	virtual EResult		SetData		( const TImage & tImage )				= 0;
	virtual EResult		Bind		()										= 0;
	virtual int			GetHandle() = 0;

	virtual const TImage & GetDesc() =0;

	virtual EResult		SetRef		( CSafePointer < ITexture > pRefTexture ) = 0;
	virtual CSafePointer < ITexture >	GetRef		()						= 0;
	//virtual EResult		SetSubData	( const TImage * pImage ) = 0;
	//virtual EResult		GetSubData	( const TImage * pImage ) = 0;

	virtual uint		GetRefCount	()	const								= 0;
	virtual void		IncRefCount	()										= 0;
	virtual void		DecRefCount	()										= 0;
};

typedef CSafePointer< ITexture > PTexture;


/*
================================================================================
//	Name: Mesh
//	Desc:
================================================================================
*/
enum EBlock
{
	CHUNK_MESH_HEADER,
	CHUNK_VERTEX_DESC,
	CHUNK_VERTEX_BUFFER,
	CHUNK_VERTICES,
	CHUNK_BINORMALS,
	CHUNK_TEX_VERTICES,
	CHUNK_SURFACE, 
};

//------------------------------------------------------------------------------

struct TVertexComponent
{
	inline const char * GetName() const { return szName; }
	inline ETypeID		GetType() const { return eType; } 

	enum { MAX_COMPONENT_STR = 16 };

	char	szName[ MAX_COMPONENT_STR ];
	ETypeID	eType;
	UINT32	nSize;
};


struct TSurface
{
	struct THeader
	{
		enum { MAX_SURF_STR = 64 };

		uint	nHeaderSize;			/// ������ ���������
		uint	nIndexCount;			/// ���������� �������� ( �� ������������� )
		uint	nIndexSize;				/// ������ ������� ( 2 ��� 4 ����� )
		char	szName[ MAX_SURF_STR ];	/// ��� surface'�
	};

	inline TSurface()
	{
	}

	inline size_t	GetNumIndices() const { return tHeader.nIndexCount; }
	inline dword	GetIndex( size_t nIndex ) const { return ((short *)pIndices)[ nIndex ]; }

	inline void		SetName( const char * szName )
	{
		if ( !szName )
			szName = "no name";

		strcpy( tHeader.szName, szName );
	}

	THeader		tHeader;
	void *		pIndices;
};

//------------------------------------------------------------------------------


/**
*
*/
struct TVertexAttr
{
	#define MAX_NAME_LEN 32

	char	szName[ MAX_NAME_LEN ];
	ETypeID eType;
	uint	nOffset;
};


/**
*
*/
struct IVertexDecl
{
	virtual			~IVertexDecl() {};

	virtual uint			GetStride		() const						= 0;
	virtual void			SetStride		( uint nStride )				= 0;

	virtual void			Assign			( bool bAssign = true  )		= 0;
//	virtual bool			Register		()								= 0;

	virtual void			AddAttr(
		const char * szName,
		const char * szSemantics,
		ETypeID eType,
		uint nOffset ) = 0;

	virtual const TVertexAttr *	GetAttr		( uint nAttr ) const = 0;

	virtual uint			GetAttrCount	() const						= 0;
	//virtual const char *	GetAttrName		() const						= 0;
};




struct TUniform
{
	CStr	sName;		/// ��� ���������
	int		nLoc;		/// ID ��������� � GLSL ���������
	ETypeID	eType;		/// ��� ���������
	uint	nCount;		/// ���-�� ���������
};




/**
*
*/
enum EShaderType
{
	MESH_SHADER	= 0,
	SKIN_SHADER,
	WATER_SHADER,
	UI_SHADER,
	LINE_SHADER,
	LAND_SHADER,

	SHADER_COUNT
};


/**
*
*
struct IConstTable
{
	virtual EShaderType GetShaderType() const = 0;
};


struct TConst
{
	int		nConstID;
	ETypeID	eType;
};


struct TMeshConstants : IConstTable
{
	EShaderType GetShaderType() const { return MESH_SHADER; }

	TConst tLightPos;
};*/

//////////////////////////////////////////////////////////////////////////

enum EShaderAbility
{
	SA_INVALID_ABILITY = -1,
	SA_COLOR_MOD = 0,
	SA_MAX_ABILITIES
};


//! IShader - ����������� ��������� �������
//! ������������� ������ � ���������� �������
struct IShader
{
	virtual bool	IsValid			() const	{ return true; }
	virtual bool	Assign			()										= 0;

//	virtual bool			SetVertexDecl( IVertexDecl * pDecl )	= 0;
	virtual IVertexDecl *	GetVertexDecl()							= 0;

	//! ���������� ���������� ����������
	virtual uint	GetUniformCount	() const										= 0;

	//! ���������� ������ ���������� �� ������� �����
	virtual uint	GetUniformID	( const char * szName ) const					= 0;

	//! ���������� ��� ���������� �� ������� �������
	virtual ETypeID	GetUniformType	( uint nID ) const								= 0;

	//! ���������� ������ ����������(�������) �� ������� �������
	virtual uint	GetUniformSize	( uint nID ) const								= 0;

	//! ��������� ��������(�) ����������
	virtual void	SetUniforms		( uint nID, const vec4 * pValues )				= 0;
};


enum EUseMode
{
	// access mode
	WRITE_ONLY		=	1 < 0,
	READ_WRITE		=	1 < 1,
	STREAM_WRITE	=	1 < 2,

	// pool
	SYSTEM_MEM		=	1 < 3
};

/*
================================================================================
//	Name: Renderer API
//	Desc:
================================================================================
*/
enum ESysTexture
{
	EST_DEFAULT,
	EST_WHITE,
	EST_GREY,
	EST_BLACK,
	EST_TOTAL
};


/**
*
*/
enum ETexture
{
	DIFFUSE_MAP,
	BUMP_MAP,
	SPECULAR_MAP
};


/**
*
*/
enum EPrimitive
{
	PRIM_POINT,
	PRIM_LINE,
	PRIM_TRIANGLE,
	PRIM_QUAD
};


/**
*
*/
struct IIndexBuffer
{
	virtual	void	Release			() = 0;

	virtual bool	Alloc			( uint nSize, dword dwFlags ) = 0;
	virtual void *	Lock			( uint nOffset, size_t nSize, bool bReadBack = false ) = 0;
	virtual void	Unlock			() = 0;

	virtual bool	Bind			() = 0;

	virtual void	SetAttrs		( uint nNumIndices, ETypeID eType ) = 0;
	virtual ETypeID GetType			() const = 0;

	virtual uint	GetNumIndices	() const = 0;
};


/**
*
*/
struct IVertexBuffer
{
	virtual	void	Release			() = 0;

	virtual bool	Alloc			( uint nSize, dword dwFlags ) = 0;
	virtual bool	Free			() = 0;

	virtual bool	AllocMulti		( uint nCount ) = 0;
	virtual IVertexBuffer * GetSubBuffer(uint nID) = 0;

	virtual void *	Lock			(size_t nOffset, size_t nSize, bool bReadBack = false ) = 0;
	virtual void	Unlock			() = 0;
	virtual EResult UpdateData		( size_t nOffset, void * pData, size_t nSize ) = 0;

	virtual uint	GetHandle() = 0;
	virtual size_t	GetSize() = 0;

	virtual void	RenderIndexed	( IVertexDecl * pDecl, IIndexBuffer * pIB, EPrimitive ePrim, uint nFirst = 0, uint nSize = 0 ) = 0;
	virtual void	Render			( IVertexDecl * pDecl, uint nFirst, uint nCount, EPrimitive ePrim ) = 0;
};

//typedef CSafePointer< IVertexBuffer > PVertexBuffer;
typedef IVertexBuffer * PVertexBuffer;

enum EAlphaFunc
{
	AF_LESS	= 0,
	AF_LEQUAL,
	AF_EQUAL,
	AF_GEQUAL,
	AF_GREATER,

	AF_MAX_FUNCS
};

enum EBlendFactor
{
	BF_ZERO = 0,
	BF_ONE,

	BF_SRC_ALPHA,
	BF_SRC_COLOR,

	BF_DST_ALPHA,
	BF_DST_COLOR,

	BF_ONE_MINUS_SRC_ALPHA,
	BF_ONE_MINUS_SRC_COLOR,

	BF_ONE_MINUS_DST_ALPHA,
	BF_ONE_MINUS_DST_COLOR,

	BF_SRC_ALPHA_SATURATE,

	BF_MAX_FACTORS
};


/**
*
*/
struct IRenderTarget
{
	virtual				~IRenderTarget() {}

	virtual bool		Init(uint nHeight, uint nWidth, uint nColorCuount, bool bDepth ) = 0;
	virtual PTexture	GetTexture(uint nColor)								= 0;
	virtual bool		Bind		()								= 0;

	// for GPU tricks
	virtual	bool		CopyToVertexBuffer(IVertexBuffer * pDest, uint nColor)	= 0;
};


/**
*
*/
enum EMatrixMode
{
	MM_PROJECTION,
	MM_MODELVIEW,
	MM_TEXTURE
};


//! ����������� ��������� (API) �������������

//! 
//! \ingroup renderer
struct IRenderer : public IBaseIntf
{
	virtual EResult		Init			( HWND hWindow )			= 0;
	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	virtual void		EnableAlphaTest	( bool bEnable = true )				= 0;
	virtual void		EnableBlend		( bool bEnable = true )				= 0;
	virtual void		EnableDepthTest	( bool bEnable = true )				= 0;

	virtual void		AlphaRef		( float fRef )						= 0;
	virtual void		AlphaFunc		( EAlphaFunc eFunc )				= 0;
	virtual bool		BlendFunc		( EBlendFactor eSrc, EBlendFactor eDst ) = 0;
	virtual void		DepthMask		( bool bMask )						= 0;

	//--------------------------------------------------------------------------
	//	Tip: Textures
	//--------------------------------------------------------------------------
	//! �������� ��������

	//! ��� ��� ����� ���-�� ��������
	//! ��-��
	//! \param[out] pTexture �������� �� ������ ��������.
	//! \param[in] tImage �������� �� ��������� ����������� TImage
	//! \param[in] szName ���������� ��� �������� (����� ��������� � ���-�������.
	virtual EResult		CreateTexture( PTexture & pTexture, const char * szName )= 0;

	virtual void		SetDefTexture	( PTexture & pTexture )				= 0;
	virtual PTexture	GetSysTexture	( ESysTexture eTexture )			= 0;

	virtual bool		SetSampler		( uint nSampler, PTexture pTex ) = 0;

	//--------------------------------------------------------------------------
	//	Tip: Shaders
	//--------------------------------------------------------------------------
	virtual bool CreateShader ( const char * szPath, IVertexDecl * pDecl,
		TUniform * pUnfms, uint nUnfmCount ) = 0;
	virtual void			SetShader		( IShader * pShader ) = 0;
	virtual IShader	*		GetShader		( EShaderType eShaderType )		= 0;
	virtual IShader	*		GetShader		( const char *	szShaderName )	= 0;
	virtual IShader *		GetActiveShader	()								= 0;
	//virtual IMaterial *		CreateMaterial	() = 0;

	//--------------------------------------------------------------------------
	// Tip: Buffers
	//--------------------------------------------------------------------------
	virtual PVertexBuffer 	CreateVB() = 0;
	virtual IIndexBuffer *	CreateIB() = 0;

	virtual IVertexDecl *	CreateVertexDecl()						= 0;



	//--------------------------------------------------------------------------
	//	Tip: RenderTargets
	//--------------------------------------------------------------------------
	virtual IRenderTarget * CreateRenderTarget	()							= 0;
	virtual bool			SetRenderTarget		( IRenderTarget * pRT )		= 0;
	/*
	virtual bool		PushRenderTarget()									= 0;
	virtual bool		PopRenderTarget	()									= 0;
	virtual bool		SetRenderTarget	( TextureID idRenderTarget )		= 0;
*/

	//--------------------------------------------------------------------------
	//	Tip: Utils
	//--------------------------------------------------------------------------
	virtual void		Line2D( const vec2 & vStart, const vec2 & vEnd, dword dwColor ) = 0;
	virtual void		Line3D( const vec3 & vStart, const vec3 & vEnd, dword dwColor ) = 0;
	virtual	void		Circle3D( const mat4 & mWorld, float fRadius, dword dwColor ) = 0;
	virtual void		BBox( const bbox & box ) = 0;
	virtual void		Axis3D( const vec3 & vOrigin ) = 0;
	virtual void		Axis3D( const mat4 & mWorld ) = 0;

	//--------------------------------------------------------------------------
	//	Tip: Scene
	//--------------------------------------------------------------------------
	virtual bool		ClearScene		()									= 0;
	virtual bool		Present			()									= 0;

	//--------------------------------------------------------------------------
	// Tip: Matrices
	//--------------------------------------------------------------------------
	/*
	virtual void		PushMatrix	( EMatrixMode eMode )						= 0;
	virtual void		PopMatrix	( EMatrixMode eMode )						= 0;
	virtual void		SetMatrix	( EMatrixMode eMode, const mat4 & matrix )	= 0;
	virtual void		MultMatrix	( EMatrixMode eMode, const mat4 & matrix )	= 0;
	virtual const mat4 *GetMatrix	( EMatrixMode eMode )						= 0;
*/

	//--------------------------------------------------------------------------
	// Tip: Viewport
	//--------------------------------------------------------------------------
	virtual	const ivec2 & GetViewPort	() const							= 0;
	virtual void		SetViewPort		( const ivec2 & vViewPort )			= 0;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __IRenderer_h_included__

////////////////////////////////////////////////////////////////////////////////

/** @} */