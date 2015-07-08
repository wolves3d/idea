////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

/**
*
*/
class CRenderTarget : public IRenderTarget
{
	friend CRenderer_GL;
	public:

				~CRenderTarget();

	bool		Init		( uint nWidth, uint nHeight );
	PTexture	GetTexture	();
	bool		Bind		();

	private:

		GLuint nFrameBuffer;  // color render target
		GLuint nDepthBuffer; // depth render target
		GLuint stencil_rb; // depth render target

		PTexture m_pTexture;

		uint m_nListID;
};


/**
*
*/
CRenderTarget::~CRenderTarget()
{
	g_pRenderer->RemoveRenderTarget( m_nListID );

	if ( g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
	{
		glDeleteFramebuffersEXT( 1, &nFrameBuffer );
		GL_VALIDATE;

		glDeleteFramebuffersEXT( 1, &nDepthBuffer );
		GL_VALIDATE;
	}
}


/**
*
*/
bool CRenderTarget::Init( uint nWidth, uint nHeight )
{
	if ( false == g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
		return false;
	
	TImage tImage;
	{
		tImage.nWidth	= nWidth;
		tImage.nHeight	= nHeight;
		tImage.eFormat	= TImage::IMG_FMT_RGBA8,
		tImage.nSize	= nWidth * nHeight * 4,
		tImage.pData	= NULL;
	};

	g_pRenderer->CreateTexture( m_pTexture, /*tImage,*/ "frame_buffer" );

	glGenFramebuffersEXT( 1, &nFrameBuffer );
	GL_VALIDATE;

	glGenRenderbuffersEXT( 1, &nDepthBuffer );
	GL_VALIDATE;

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, nFrameBuffer );
	GL_VALIDATE;

	// Не удобно, но этот hack единственный (?) способ получить
	// индекс текстуры

	m_pTexture->Bind();

	GLint nTex;
	glGetIntegerv( GL_TEXTURE_2D_BINDING_EXT, &nTex );

	//////

	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D,
		nTex,
		0 );
	GL_VALIDATE;


	// initialize depth renderbuffer
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, nDepthBuffer );

	// init depth buffer
	glRenderbufferStorageEXT(
		GL_RENDERBUFFER_EXT,
		GL_DEPTH_COMPONENT24,
		nWidth,
		nHeight );
	GL_VALIDATE;

	// attach depth buffer to current framebuffer
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT, 
		GL_RENDERBUFFER_EXT,
		nDepthBuffer);
	GL_VALIDATE;

	// unbind frame buffer ( set to default )
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	GL_VALIDATE;

	GLenum eStatus = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

	if ( GL_FRAMEBUFFER_COMPLETE_EXT != eStatus )
	{
		// delete all data here!
		return false;
	}

	return true;
}


/**
*
*/
bool CRenderTarget::Bind()
{
	if ( false == g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
		return false;

	//pTexture->Bind(); // ( ??? )

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, nFrameBuffer );
	GL_VALIDATE;
	return true;
}


/**
*
*/
PTexture CRenderTarget::GetTexture()
{
	return m_pTexture;
}


/**
*
*/
IRenderTarget * CRenderer_GL::CreateRenderTarget()
{
	CRenderTarget * pRT = NEW CRenderTarget;

	if ( pRT )
	{
		pRT->m_nListID = m_RTList.Add( pRT );

		if ( INVALID_INDEX == pRT->m_nListID )
		{
			DEL( pRT );
			return NULL;
		}
	}

	return pRT;
}


/**
*
*/
void CRenderer_GL::RemoveRenderTarget( uint nListID )
{
	m_RTList.Remove( nListID );
}


/**
*
*/
bool CRenderer_GL::SetRenderTarget( IRenderTarget * pRT )
{
	if ( false == g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
		return false;

	if ( pRT )
	{
		pRT->Bind();
	}
	else
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		GL_VALIDATE;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////