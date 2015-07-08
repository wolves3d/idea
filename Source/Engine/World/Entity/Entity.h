////////////////////////////////////////////////////////////////////////////////

#ifndef __Entity_h_included__
#define __Entity_h_included__

////////////////////////////////////////////////////////////////////////////////

struct IUserData
{
	//void Save()
	//void Parse()
};

class CAbility
{
};

class CEntity : public IEntity
{
	friend CEntityManager;

	public:

			CEntity	();
			~CEntity();
	void	Release	();

	void	Update	();
	void	Render	();

	//--------------------------------------------------------------------------
	// Tip: Load and Save
	//--------------------------------------------------------------------------
	void LoadData() {};		// create abilities e.t.c.
	void UnloadData() {};	// delete abilities e.t.c.

	//--------------------------------------------------------------------------
	// Tip: Position / Orientation / Direction / Scale
	//--------------------------------------------------------------------------
	const float	GetDir () const
	{
		return m_fDir;
	}

	inline void	SetDir( const float fDir )
	{
		m_fDir			= fDir;
		m_bDirtyMatrix	= true;
	}

	inline const quat &	GetOrient() const
	{
		return m_qOrient;
	}

	inline void	SetOrient( const quat & qOrient )
	{
		m_qOrient		= qOrient;
		m_bDirtyMatrix	= true;
	}

	inline const vec3 &	GetPos() const
	{
		return m_vPos;
	}

	inline void	SetPos( const vec3 & vPos )
	{
		m_vPos			= vPos;
		m_bDirtyMatrix	= true;
	}

	inline const vec3 & GetScale() const
	{
		return m_vScale;
	}

	inline void SetScale( const vec3 & vScale )
	{
		m_vScale		= vScale;
		m_bDirtyMatrix	= true;
	}

	inline const mat4 & GetMatrix() const
	{
		DEBUG_ASSERT( false == m_bDirtyMatrix );

		return m_mWorld;
	}

	//--------------------------------------------------------------------------
	// Tip: Material
	//--------------------------------------------------------------------------
	/*IMaterial *	GetMaterial()
	{
		return m_pMaterial;
	}

	void SetMaterial(IMaterial * pMaterial )
	{
		m_pMaterial = pMaterial;
	}*/

	//--------------------------------------------------------------------------
	// Tip: Shape
	//--------------------------------------------------------------------------
/*	inline CShape *	GetShape()
	{
		return m_pShape;
	}

	void SetShape( CShape * pShape )
	{
		m_pShape = pShape;
	}*/

	IModel * GetModel();

	private:

	void UpdateMatrix();

	class CModel *	m_pModel;
	//IMaterial *	m_pMaterial;

	float		m_fDir;
	vec3		m_vScale;
	quat		m_qOrient;
	vec3		m_vPos;
	mat4		m_mWorld;
	bool		m_bDirtyMatrix;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Entity_h_included__

////////////////////////////////////////////////////////////////////////////////