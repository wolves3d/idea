////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "../Texture/Texture.h"

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

	bool		CopyToVertexBuffer(IVertexBuffer * pDest);

	private:

		GLuint nFrameBuffer;  // color render target
		GLuint nDepthBuffer; // depth render target
		GLuint stencil_rb; // depth render target

		CTexture * m_pTexture;

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

	m_pTexture = g_pRenderer->CreateTexture("frame_buffer");
	m_pTexture->Init(tImage, EAT_IMMUTABLE);

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


bool CRenderTarget::CopyToVertexBuffer(IVertexBuffer * pDest)
{
	const TImage & textureDesc = m_pTexture->GetDesc();
	if (textureDesc.nSize != pDest->GetSize())
	{
		DEBUG_ASSERT("!incompatible buffer sizes!");
		return false;
	}

	g_pRenderer->SetRenderTarget(this);

	// set source buffer for reading
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

	glBindBufferARB(GL_PIXEL_PACK_BUFFER_EXT, pDest->GetHandle());
	glReadPixels(
		0, 0,
		textureDesc.nWidth, textureDesc.nHeight,
		GL_RGBA, GL_FLOAT,
		NULL	// destination buffer pointer
		);

	// disable buffer for reading
	glReadBuffer(GL_NONE);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_EXT, 0);
	
	g_pRenderer->SetRenderTarget(NULL);

	return true;
}


/**
*
*/
PTexture CRenderTarget::GetTexture()
{
	PTexture res;
	res = m_pTexture;
	return res;
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

	if (NULL == pRT)
	{
		// NULL means default render target (frame buffer)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		GL_VALIDATE;
	}
	else
	{
		pRT->Bind();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////