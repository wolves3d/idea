////////////////////////////////////////////////////////////////////////////////

/*
struct TPosKey
{
	uint nFrame;
	vec3 vPos;
};

struct TAnimData
{
	char		szObjName[ 32 ];
	TPosKey	*	pPosKeys;
};

/*
*
*/
class CAnim
{
	public:

		/**
		*
		*/
		CAnim() :
			m_pJoints		( NULL ),
			m_nJointCount	( 0 ),
			m_fTime			( 0.f )
		{
		}


		/**
		*
		*/
		~CAnim()
		{
			for ( uint n = 0; n < m_nJointCount; ++n )
			{
				DEL_ARRAY( m_pJoints[ n ].pScaleKeys );
				DEL_ARRAY( m_pJoints[ n ].pRotKeys );
				DEL_ARRAY( m_pJoints[ n ].pTransKeys );
			}

			DEL_ARRAY( m_pJoints );
		}

		void SetName( const char * szName ) { m_sName.Set( szName ); } 
		const char * GetName() const { return m_sName.GetString(); }

		uint GetJointCount	() const
		{
			return m_nJointCount;
		}

		uint GetFrameCount	() const	{ return m_nFrameCount; }

		const char * GetJointName( uint nJoint ) const
		{
			if ( nJoint < GetJointCount() )
			{
				return m_pJoints[ nJoint ].tHeader.szName;
			}

			return NULL;
		}

		bool GetQuat( quat & qRotOut, uint nJoint, float fTime ) const
		{
			if ( nJoint < GetJointCount() )
			{
				if ( ( fTime + 1 ) < m_fTime )
				{
					float t = fTime - (int)fTime;
					quat q1 = m_pJoints[ nJoint ].pRotKeys[ (uint)fTime ].value;
					quat q2 = m_pJoints[ nJoint ].pRotKeys[ (uint)fTime + 1 ].value;

					Slerp( &q1, &q1, &q2, t );
					qRotOut = q1;
					qRotOut = m_pJoints[ nJoint ].pRotKeys[ (uint)fTime ].value;
					return true;
				}
			}

			return false;
		}

		bool Load( const char * szFileName );

		float		m_fTime;

		CStr		m_sName;

		uint		m_nFrameCount;
		uint		m_nJointCount;
		//TAnimData *	m_pObjectData;

		/**
		*
		*/
		struct TJoint
		{
			TJoint() :
				pScaleKeys	( NULL ),
				pRotKeys	( NULL ),
				pTransKeys	( NULL )
			{
			}

			TJointHdr	tHeader;

			CScaleKey *	pScaleKeys;
			CRotKey *	pRotKeys;
			CPosKey *	pTransKeys;
		} * m_pJoints;
};

struct TAnimBind
{
	const CAnim *	pAnim;
	short *			pIndices;
};

enum EAnimChannel
{
	ANIM_CHANNEL_DEFAULT,
	ANIM_CHANNEL_TORSE,
	ANIM_CHANNEL_LEGS
};

struct TAnimState
{
	TAnimBind *		pAnimBind;	// Анимация
	EAnimChannel	eChannel;
	float			fSpeed;		// Коэфициент скорости проигрывания
	bool			bForward;	// Флаг направления проигрывания
	bool			bLoop;		// Флаг цикленности
	float			fBlend;		// Коэфициент блендинга
	uint			nFrame;		// Текущий кадр
	float			fTime;
	float			fStartTime;
};

struct TAnimChannel
{
	TAnimState * pPriAnim;
	TAnimState * pSecAnim;
};


/**
*
*/
class CModel : public IModel
{
	public:

	CModel();
	~CModel();

	void Render( const mat4 & mWorld );

	const bbox & GetBBox()
	{
		static bbox bbtest;
		bbtest.vMin.Set( -2, -2, -2 );
		bbtest.vMax.Set( 2, 2, 2 );
		return bbtest;
	}

	void RebuildHierarchy();
	void Load( const char * szFile ); // old

	// Animation ///////////////////////////////////////////////////////////////

	bool			AddAnim		( const CAnim * pAnim );
	void			RemoveAnim	( const char * szAnimName );

	uint			GetAnimCount() const;
	const CAnim *	GetAnim		( uint nAnim );
	const CAnim *	GetAnim		( const char * szAnimName );
	
	bool			PlayAnim	(	const char *	szAnimName,
									EAnimChannel	eChannel	= ANIM_CHANNEL_DEFAULT,
									bool			bLoop		= false,
									bool			bForward	= true,
									float			fSpeed		= 1.f,
									float			fBlendTime	= 0.f );

	TAnimState *	GetAnimState( EAnimChannel eChannel = ANIM_CHANNEL_DEFAULT );

	//==========================================================================
	// Tip: Private Methods & Fields
	//==========================================================================
	private:

	void LoadObjects	( const char * szFile );
	bool LoadMaterial	( CXMLElement * pMaterial );

	uint		m_nNumObjects;
	CObject *	m_pObjects;
	mat4 *		m_pBoneMatrices;

	IShape *	m_pShape;
	IMaterial * m_pMaterial;

	CVector <TAnimBind> m_pAnims;
	TAnimState			tAnimState;
};

////////////////////////////////////////////////////////////////////////////////