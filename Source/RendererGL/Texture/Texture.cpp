////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Texture.h"

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/

		CTexture::CTexture() :
			m_nTextureID( 0 ),
			m_nRefCount	( 1 )
		{
			//m_pTexMaps = NEW PTexture [ 1 ];
		}

		CTexture::~CTexture()
		{
			glDeleteTextures( 1, &m_nTextureID );
		}

		void CTexture::Release()
		{
			DecRefCount();

			if ( 0 == GetRefCount() )
			{
				PURE_DEL( this );
			}
		}

		EResult CTexture::SetRef(PTexture pRefTexture)
		{
			if ( pRefTexture )
			{
				if ( pRefTexture->GetRef() )
					return R_INVALID_ARG;
			}

			m_pRefTexture = pRefTexture;
			return R_OK;
		}

		PTexture CTexture::GetRef()
		{
			return m_pRefTexture;
		}

		EResult CTexture::Init(const TImage & pImage, EAccessType eAccess)
		{
			if ( !m_nTextureID )
				return R_INVALID_OP;

			m_desc = pImage;

			//glEnable( GL_TEXTURE_2D );

			glBindTexture( GL_TEXTURE_2D, m_nTextureID );
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

			GLint nIntFormat;	// Internal GL format
			GLint nFormat;		// The format of the pixel data

			switch ( pImage.eFormat )
			{
			case TImage::IMG_FMT_RGB8 :
				nIntFormat	= GL_RGB8;
				nFormat		= GL_BGR_EXT;
				m_desc.nSize = (m_desc.nWidth * m_desc.nHeight * 3);
				break;

			case TImage::IMG_FMT_RGBA8 :
				nIntFormat = GL_RGBA8; // GL_RGBA32F_ARB
				nFormat		= GL_BGRA_EXT;
				m_desc.nSize = (m_desc.nWidth * m_desc.nHeight * 4);
				break;

			default:

				glBindTexture( GL_TEXTURE_2D, 0 );
				return R_INVALID_ARG;
			}


			glTexImage2D(
				GL_TEXTURE_2D,			// target
				0,						// level
				nIntFormat,
				(int)pImage.nWidth,		// width
				(int)pImage.nHeight,	// height
				0,						// border
				nFormat,
				GL_UNSIGNED_BYTE,		// color type
				pImage.pData );			// data
			GL_VALIDATE;

			glTexParameterf	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameterf	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glBindTexture	( GL_TEXTURE_2D, 0 );

			return R_OK;
		}

		EResult	CTexture::Bind()
		{
			if ( m_pRefTexture )
				return m_pRefTexture->Bind();

			if ( m_nTextureID )
			{
				glBindTexture( GL_TEXTURE_2D, m_nTextureID );
				GL_VALIDATE;
			}

			return R_OK;
		}

		 const char * CTexture::GetName() const
		{
			return m_sName.GetString();
		}

		 uint CTexture::GetRefCount() const
		{
			return m_nRefCount;
		}

		 void CTexture::IncRefCount()
		{
			m_nRefCount++;
		}

		 void CTexture::DecRefCount()
		{
			DEBUG_ASSERT( 0 != m_nRefCount )

			if ( m_nRefCount > 0 )
			{
				m_nRefCount--;
			}
		}

		 const TImage & CTexture::GetDesc()
		 {
			 return m_desc;
		 }


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CTexture::SetFilter( TextureFormat eFilter )
{
	/*
	GLint iMinFilter;

	switch ( eFilter )
	{
		case FILTER_NEAREST:
			iMinFilter = GL_NEAREST;
		break;

		case FILTER_LINEAR:
			iMinFilter = GL_LINEAR;
		break;

		default :
			return R_INVALID_ARG;
	}

	glBindTexture	( GL_TEXTURE_2D, m_nTextureID );
	glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iMinFilter );
	glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iMinFilter );
	*/
	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/



/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/

CTexture * CRenderer_GL::CreateTexture(const char * szName)
{
	CTexture * pTexture = NEW CTexture;

	if (NULL != pTexture)
	{
		glGenTextures(1, &pTexture->m_nTextureID);

		if (pTexture->m_nTextureID)
		{
			pTexture->m_sName = szName;
			return pTexture;
		}

		DEL(pTexture);
	}

	return NULL;
}


EResult	CRenderer_GL::CreateTexture( PTexture & pOutTexture, const char * szName )
{
	if ( !szName )
		return R_INVALID_ARG;

	pOutTexture = CreateTexture(szName);
	return R_OK;
}


/**
*
*/
PTexture CRenderer_GL::GetSysTexture( ESysTexture eTexture )
{
	return m_pSysTextures[ eTexture ];
}


/**
*
*/
void CRenderer_GL::SetDefTexture( PTexture & pTex )
{
	m_pSysTextures[ EST_DEFAULT ] = pTex;
}


/**
*
*/
bool CRenderer_GL::SetSampler( uint nSampler, PTexture pTex )
{
	if ( !pTex )
		pTex = GetSysTexture( EST_DEFAULT );

	pTex->Bind();
	return false;
}

////////////////////////////////////////////////////////////////////////////////