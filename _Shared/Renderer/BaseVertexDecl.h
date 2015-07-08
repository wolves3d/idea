
class CBaseVertexDecl : public IVertexDecl
{
	public:

	//void	Assign( bool bAssign = true );
	CBaseVertexDecl	();
	~CBaseVertexDecl();

	uint	GetVertexCount	();
	void	SetVertexCount	( uint nCount );

	uint	GetStride	() const;
	void	SetStride	( uint nStride );
	void	AddAttr		( const char * szName, const char * szSemantics, ETypeID eType, uint nOffset );
	const TVertexAttr *	GetAttr		( uint nAttr ) const;
	uint	GetAttrCount() const;

	EPrimitive	GetPrimitiveType();
	void		SetPrimitiveType( EPrimitive ePrim );

	protected:

	uint		m_nStride;

	CVector <TVertexAttr> m_pAttrs;

	// bool m_bRegistred;
};