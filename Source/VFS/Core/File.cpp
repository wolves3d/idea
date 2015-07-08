////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CDiskFile : public CFile
{
	friend CVfs;

	public :

		inline CDiskFile() :
			CFile(),
			m_pFile( NULL )
		{
		}

		inline ~CDiskFile()
		{
			if ( m_pFile )
				Close();
		}

		/*inline */EResult Open( const char * szFileName, int nAccess )
		{
			CStr sArg;

			if ( nAccess & VFS_READ )
			{
				sArg.Set( "r" );
			}
			else if ( nAccess & VFS_WRITE )
			{
				sArg.Set( "w" );
			}

			if ( nAccess & VFS_TEXT )
			{
				sArg.Add( "t" );
			}
			else if ( nAccess & VFS_BINARY )
			{
				sArg.Add( "b" );
			}

			if ( m_pFile = fopen( szFileName, sArg.GetString() ) )
			{
				//--------------------------------------------------------------
				// Tip: If read-only mode...
				//--------------------------------------------------------------
				if (	( VFS_READ & nAccess )		&&
						( ! ( VFS_WRITE & nAccess ))	)
				{
					fseek( m_pFile, 0L, SEEK_END );
					m_nSize = ftell( m_pFile );
					rewind( m_pFile );
					m_nCurPos = 0;
				}

				return R_OK;
			}
			
			return R_FILE_ERROR;
		}

		inline void Close()
		{
			CFile::Close();

			if ( m_pFile )
				fclose( m_pFile );
		}

		inline void Flush()
		{
			if ( m_pFile )
				fflush( m_pFile );
		}

		inline size_t Read	( void * pBuffer, size_t nSize )
		{
			if ( m_pFile )
			{
				DEBUG_ASSERT( ( m_nCurPos + nSize ) <= m_nSize );

				size_t a = fread( pBuffer, nSize, 1, m_pFile );

				if ( 1 == a )
				{
					m_nCurPos += nSize;
					return nSize;
				}
				else
				{
					DEBUG_MSG( "VFS Error?!" );
				}
			}

			return 0;
		}


		inline int Seek( long nOffset, EVfsEnum eOrigin )
		{
			if ( m_pFile )
			{
				int nOrigin;

				switch ( eOrigin )
				{
					case VFS_SEEK_SET: nOrigin = SEEK_SET; break;
					case VFS_SEEK_CUR: nOrigin = SEEK_CUR; break;
					case VFS_SEEK_END: nOrigin = SEEK_END; break;

					default:
						return -1;
				}

				if ( !fseek( m_pFile, nOffset, nOrigin ) )
				{
					m_nCurPos = ftell( m_pFile );
					return 0;
				}
			}

			return -1;
		}

		inline size_t Write( const void * pBuffer, size_t nSize )
		{
			if ( m_pFile && pBuffer && nSize )
			{
				size_t nWrite = fwrite( pBuffer, 1, nSize, m_pFile );

				if ( m_nCurPos == m_nSize )
					m_nSize += nWrite;

				m_nCurPos += nWrite;
				return nWrite;
			}

			return 0;
		}

	private :

		FILE *	m_pFile;
};


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CVfs::OpenFile( IFile ** ppOutFile, const char * szFileName, int nAccess )
{
	if  (	( nAccess & VFS_TEXT	)	&&
			( nAccess & VFS_BINARY	)		)
	{
		return R_INVALID_ARG;
	}

	CDiskFile * pFile = NEW CDiskFile;

	if ( !pFile )
		return R_OUT_OF_MEMORY;

	EResult res = pFile->Open( szFileName, nAccess );

	if ( R_OK == res )
	{
		if ( INVALID_INDEX == m_FileList.Add( pFile ) )
		{
			DEL( pFile );
			return R_OUT_OF_MEMORY;
		}
		
		(*ppOutFile) = pFile;
	}
	else
	{
		DEL( pFile );
	}
	
	return res;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CVfs::CloseFile( IFile ** ppFile )
{
	if ( NULL == (*ppFile) )
		return R_INVALID_ARG;


	((CFile*)(*ppFile))->Close();
	DEL( (*ppFile) );

	return R_OK;
}
////////////////////////////////////////////////////////////////////////////////