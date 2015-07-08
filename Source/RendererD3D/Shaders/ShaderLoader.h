class CShaderCreator : public IResCreator
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
	public:

	const IResCreatorInfo * GetInfo () const;

	void	CreateResInstance	( TResource ** ppOutResource );
	size_t	InitRes				( TResource * pRes, size_t nFileSize );
	size_t	UpdateRes			( TResource * pRes, size_t nBytesWritten );
	void	FinalizeRes			( TResource * pRes );

	static IResCreatorInfo m_Info;
};