////////////////////////////////////////////////////////////////////////////////


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CTexture : public ITexture
{
	friend CRenderer_GL;

	public :

		CTexture();
		~CTexture();

		void Release();
		EResult SetRef(PTexture pRefTexture);
		PTexture GetRef();
		uint GetRefCount() const;
		void IncRefCount();
		void DecRefCount();

		EResult Init(const TImage & pImage, EAccessType eAccess);
		EResult	SetFilter( TextureFormat eFilter );
		EResult	Bind();
		const char * GetName() const;

		const TImage & GetDesc();

	private :

		CStr		m_sName;
		uint		m_nTextureID;
		uint		m_nRefCount;
		PTexture	m_pRefTexture;

		TImage		m_desc;
};

////////////////////////////////////////////////////////////////////////////////