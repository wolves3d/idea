////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "SoundMgr.h"



/**
*
*/
class CSample : public ISample
{

public:

	CSample( const void * pData, uint nDataSize ) :
		m_pFile		( pData ),
		m_nDataSize	( nDataSize )
	{
		const TWaveFileHdr *	pFileHdr	= (TWaveFileHdr *)pData;
		const TWaveFormatHdr *	pFmtHeader	= NULL;
		const byte *			pFile		= (byte *)pData + sizeof( TWaveFileHdr );
		const byte *			pBorder		= pFile + nDataSize;

		while ( pFile < pBorder )
		{
			const TWaveChunkHdr	* pChunkHdr = (TWaveChunkHdr *)pFile;
			pFile += sizeof( TWaveChunkHdr );

			if ( !memcmp( "fmt ", pChunkHdr->Id, 4 ) )
			{
				pFmtHeader = (TWaveFormatHdr *)pFile;
				//*pFile += sizeof( TWaveFormatHdr );*/

				if ( pFmtHeader->Format == 0x0001 )
				{
					//m_SampleInfo.nBitsPerSample = FmtHeader.BitsPerSample;
					//m_SampleInfo.nNumChannels	= FmtHeader.Channels;
					//m_SampleInfo.nSamplesPerSec	= FmtHeader.nSamplesPerSec;
					//continue;
				}
				else
				{
					DEBUG_MSG( "Wrong file" );
				}
			}
			else if ( !memcmp( "data", pChunkHdr->Id, 4 ) )
			{
				if ( NULL != pFmtHeader )
				{
					if ( pFmtHeader->Format == 0x0001 )
					{	
						m_pData		= pFile;
						m_nDataSize	= pChunkHdr->nSize;
						//m_SampleInfo.dwSampleCount = ChunkHeader.nSize / FmtHeader.Channels / 2;
					}
					else
					{
						//D_WARNING( "Wrong file data" )
						//	return false;
					}
					/*	else if ( FmtHeader.Format == 0x0011 ) { IMA ADPCM }
					else if ( FmtHeader.Format == 0x0055 ) { MP3 WAVE }	*/
				}
			}

			pFile += pChunkHdr->nSize;
		}

		/*
		dword dwResult = CreateSample( pBuffer->eFormat, pBuffer->dwFreq,
		pBuffer->pData, pBuffer->dwSize );

		DeleteArray ( pBuffer->pData );
		*
		if ( !m_SampleInfo.dwSizeInBytes )
		{
			D_WARNING( "Wrong file data" )
				return false;
		}

		return true; */
	}


	/**
	* Ответ в байтах
	*/
	uint GetLength()
	{
		return m_nDataSize;
	}


	/**
	*
	*/
	uint GetData ( void * pDest, uint nOffset, uint nSize )
	{
		DEBUG_ASSERT( NULL != nSize );
		DEBUG_ASSERT( NULL != pDest );
		DEBUG_ASSERT( NULL != m_pData );
		DEBUG_ASSERT( m_nDataSize >= ( nOffset + nSize ) );

		if ( pDest && m_pData && nSize )
		{
			if ( m_nDataSize >= ( nOffset + nSize ) )
			{
				memcpy( pDest, m_pData + nOffset, nSize );
				return nSize;
			}
		}

		return 0;
	}

private:

	struct TWaveFileHdr
	{
		unsigned char	Id[ 4 ];
		unsigned int	nSize;
		unsigned char	Type[ 4 ];
	};

	struct TWaveChunkHdr
	{
		unsigned char	Id[ 4 ];
		unsigned int	nSize;
	};

	struct TWaveFormatHdr
	{
		unsigned short	Format;
		unsigned short	Channels;
		unsigned int	nSamplesPerSec;
		unsigned int	nBytesPerSec;
		unsigned short	BlockAlign;
		unsigned short	BitsPerSample;
	};

	const void *	m_pFile;
	const byte *	m_pData;
	uint			m_nDataSize;
};


/**
*
*/
ISample * CSoundMgr::CreateSample( const void * pData, uint nDataSize )
{
	return NEW CSample( pData, nDataSize );
}

////////////////////////////////////////////////////////////////////////////////