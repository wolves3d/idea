////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "../ResourceManager.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
struct TGA_Header
{
	unsigned char idLength;
	unsigned char colorMapType;

	// The image type.
	//#define TGA_TYPE_MAPPED      1
	#define TGA_TYPE_COLOR       2
	//#define TGA_TYPE_GRAY        3
	//#define TGA_TYPE_MAPPED_RLE  9
	#define TGA_TYPE_COLOR_RLE   10
	//#define TGA_TYPE_GRAY_RLE    11
	unsigned char imageType;

	// Color Map Specification.
	// We need to separately specify high and low bytes to avoid endianness
	// and alignment problems.
	unsigned char colorMapIndexLo, colorMapIndexHi;
	unsigned char colorMapLengthLo, colorMapLengthHi;
	unsigned char colorMapSize;

	// Image Specification.
	unsigned char xOriginLo, xOriginHi;
	unsigned char yOriginLo, yOriginHi;

	unsigned char widthLo, widthHi;
	unsigned char heightLo, heightHi;

	unsigned char bpp;

	/* Image descriptor.
	3-0: attribute bpp
	4:   left-to-right ordering
	5:   top-to-bottom ordering
	7-6: zero */
	#define TGA_DESC_ABITS      0x0f
	#define TGA_DESC_HORIZONTAL 0x10
	#define TGA_DESC_VERTICAL   0x20
	unsigned char descriptor;
};


/**
*
*/
void CreateTextureFromTarga( byte * pData, size_t nSize, PTexture & pTex )
{
	//byte *	pData = tRec.pBuffer;
	//size_t	nSize = tRec.nSize;

	TGA_Header * pHeader = ( TGA_Header * )pData;

	pData += sizeof( TGA_Header ) + pHeader->idLength;
	nSize -= sizeof( TGA_Header );

	if ( nSize < pHeader->idLength )
	{
		//WARNING( "Invalid TARGA file!" );
		//DeleteThis( pImage );
		//return R_INVALID_ARG;
	}

	nSize -= pHeader->idLength;

	int nWidth	= ( pHeader->widthHi << 8 ) | pHeader->widthLo;
	int nHeight	= ( pHeader->heightHi << 8 ) | pHeader->heightLo;

	TImage::EImageFmt eFormat;

	switch ( pHeader->bpp )
	{
		case 24 : eFormat = TImage::IMG_FMT_RGB8;  break;
		case 32 : eFormat = TImage::IMG_FMT_RGBA8; break;
		//default :
			//WARNING( Format( "TARGA - unknown BPP ( %d )", pHeader->bpp ) );
			//return R_INVALID_ARG;
	}

	pHeader->bpp  /= 8; // bits per pixel -> bytes per pixel
	size_t nImgSize = nWidth * nHeight * pHeader->bpp;
	byte * pImgData = pData;//NEW byte [ nImgSize ];

	switch ( pHeader->imageType )
	{
		case TGA_TYPE_COLOR :
		{
			if ( nImgSize > nSize )
			{
				//WARNING( "Invalid TARGA file!" );
				//DEL_ARRAY( pOutImage->pData );
				//return R_INVALID_ARG;
			}
			else
			{
				TImage tImage;

				tImage.nWidth	= nWidth;
				tImage.nHeight	= nHeight;
				tImage.eFormat	= eFormat;
				tImage.nSize	= nImgSize;
				tImage.pData	= pImgData;

				pTex->Init( tImage, EAT_IMMUTABLE );
				pTex->SetRef( NULL );
				//memcpy( pOutImage->pData, pData, pOutImage->nSize );
			}
		}
		break;

		/*case TGA_TYPE_COLOR_RLE :
		{
			dword dwOffset = 0;

			while ( dwOffset < pOutImage->nSize )
			{
				byte header = (*pData);
				pData += 1;

				if ( header & 128 ) // is block compressed ( 1st bit )
				{
					// block compressed

					byte pixel[ 4 ];
					header = ( header & 127 ) + 1;
					memcpy( pixel, pData, pHeader->bpp );
					pData += pHeader->bpp;

					if ( ( dwOffset + ( pHeader->bpp * header ) ) <=
						pOutImage->nSize )
					{
						while( header )
						{
							memcpy( pOutImage->pData + dwOffset, pixel,
								pHeader->bpp );

							dwOffset += pHeader->bpp;
							--header;
						}
					}
					else
					{
						//WARNING( "Invalid TARGA file data!" );
						DEL_ARRAY( pOutImage->pData );
						return R_INVALID_ARG;
					}
				}
				else
				{
					// block uncompressed
					++header;

					if ( ( dwOffset + ( pHeader->bpp * header ) ) <=
						pOutImage->nSize )
					{
						memcpy( pOutImage->pData + dwOffset, pData,
							pHeader->bpp * header );

						dwOffset  += pHeader->bpp * header;
						pData += pHeader->bpp * header;
					}
					else
					{
						//WARNING( "Invalid TARGA file data!" );
						DEL_ARRAY( pOutImage->pData );
						return R_INVALID_ARG;
					}
				}
			}
			break;
		}*/
	}

	//--------------------------------------------------------------------------
	// Tip: BGRA -> RGBA
	//--------------------------------------------------------------------------
	/*for ( size_t i = 0; i < pOutImage->nSize; i += pHeader->bpp )
	{
		byte t						= pOutImage->pData[ i ];
		pOutImage->pData[ i ]		= pOutImage->pData[ i + 2 ];
		pOutImage->pData[ i + 2 ]	= t;
	}*/
/*
	for ( size_t y = 0; y < pOutImage->nHeight / 2; ++y )
	{
		byte * pSrc = pOutImage->pData + ( y * pOutImage->nHeight ) * pHeader->bpp;
		byte * pDst = pOutImage->pData + ( ( pOutImage->nHeight - y - 1 ) * pOutImage->nHeight ) * pHeader->bpp;

		for ( size_t x = 0; x < pOutImage->nWidth; ++x )
		{
			byte c[ 4 ];
			byte * a = pSrc + ( x * pHeader->bpp );
			byte * b = pDst + ( x * pHeader->bpp );

			memcpy( c, a, pHeader->bpp );
			memcpy( a, b, pHeader->bpp );
			memcpy( b, c, pHeader->bpp );
		}
	}
*/
//	return R_OK;
}


//!

//!
//!
class CTexCreator : public IResCreator
{
	//==========================================================================
	// Tip: Public Methods & Fields
	//==========================================================================
public:

	const IResCreatorInfo * GetInfo () const
	{
		return &m_Info;
	}

	void CreateResInstance( TResource ** ppOutResource )
	{
		if ( NULL == ppOutResource )
			return;

		TResource * pRes = NEW TResource;

		if ( pRes )
		{
			PTexture * pTex = NEW PTexture;
			g_pRenderer->CreateTexture( (*pTex), "no tex name" );
			pRes->pResource = pTex;
		}

		(*ppOutResource) = pRes;
	}

	size_t InitRes( TResource * pRes, size_t nFileSize )
	{
		pRes->pBuffer = NEW byte[ nFileSize ];

		if ( pRes->pBuffer )
		{
			pRes->nSize = nFileSize;
		}

		return 0;
	}

	size_t UpdateRes( TResource * pRes, size_t nBytesWritten )
	{
		return 0;
	}

	void FinalizeRes( TResource * pRes )
	{
		PTexture pTex = *((PTexture *)pRes->pResource);
		CreateTextureFromTarga( pRes->pBuffer, pRes->nSize, pTex );

		DEL_ARRAY( pRes->pBuffer );

		pRes->eState = TResource::LOADED;
	}

	static IResCreatorInfo m_Info;
};


IResCreatorInfo CTexCreator::m_Info =
{
	"My TARGA loader :)",
	".tga"
};


//!

//!
//!
void RegisterTargaLoader()
{
	g_pResManager->RegCreator( NEW CTexCreator );
}

////////////////////////////////////////////////////////////////////////////////