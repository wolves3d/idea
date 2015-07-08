////////////////////////////////////////////////////////////////////////////////

#ifndef __Vfs_h_included__
#define __Vfs_h_included__

////////////////////////////////////////////////////////////////////////////////

class CFile : public IFile
{
	public :

		inline CFile() :
			m_nSize		( 0 ),
			m_nCurPos	( 0 )
		{
		}

		virtual ~CFile() {};

		virtual EResult Open	( const char * szFileName, int nAccess )= 0;

		virtual void	Close	()
		{
			m_nSize		= 0;
			m_nCurPos	= 0;
		}

		virtual int		Seek	( long nOffset, EVfsEnum eOrigin )			= 0;
		virtual size_t	Read	( void * pBuffer, size_t nSize )			= 0;
		virtual size_t	Write	( const void * pBuffer, size_t nSize )			= 0;

		//----------------------------------------------------------------------
		// Tip: Inline methods
		//----------------------------------------------------------------------
		inline size_t GetSize	() const
		{
			return m_nSize;
		}

		inline size_t Tell		() const
		{
			return m_nCurPos;
		}

		inline size_t Remain	() const
		{
			return ( m_nSize - m_nCurPos );
		}

		//----------------------------------------------------------------------
		// Tip: Common members
		//----------------------------------------------------------------------
		size_t	m_nSize;
		size_t	m_nCurPos;
};

////////////////////////////////////////////////////////////////////////////////

class CVfs : public IVfs
{
	public :

		//----------------------------------------------------------------------
		// Tip: IBaseInterface
		//----------------------------------------------------------------------
		const TIntfDesc *	GetInterfaceDesc() const;
		EResult				Init			();
		void				Release			();
		void				Delete			();

		EResult OpenFile	( IFile ** ppOutFile, const char * szFileName, int nAccess );
		EResult	CloseFile	( IFile ** ppFile );

		int		Seek		( IFile * pFile, long nOffset, EVfsEnum eOrigin );
		size_t	Read		( IFile * pFile, void * pBuffer, size_t nSize );
		size_t	Write		( IFile * pFile, const void * pBuffer, size_t nSize );

		EResult OpenSearch	( IFileSearch ** ppOutSearch );
		bool	SearchNext	( IFileSearch * pSearch );
		EResult CloseSearch	( IFileSearch ** ppSearch );

		// Utils ///////////////////////////////////////////////////////////////////

		bool	IsFileExist	( const char * szFileName ) const;
		char *	LoadToMem	( const char * szFileName, int nAccess );

		static	TIntfDesc	ms_ClassDesc;

	private :

		CContainer <IFile *> m_FileList;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #define __Vfs_h_included__

////////////////////////////////////////////////////////////////////////////////