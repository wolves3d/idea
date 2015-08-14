////////////////////////////////////////////////////////////////////////////////

/*
https://www.opengl.org/wiki/Framebuffer_Object_Examples#glReadPixels
http://www.lighthouse3d.com/tutorials/opengl_framebuffer_objects/
*/

#include "Common.h"
#include "../Texture/Texture.h"

/**
*
*/
class CRenderTarget : public IRenderTarget
{
friend CRenderer_GL;
public:
	CRenderTarget();
	~CRenderTarget();

	bool		Init(uint nWidth, uint nHeight, uint nColorCount, bool bDepth);
	PTexture	GetTexture(uint nColor);
	bool		Bind		();

	bool		CopyToVertexBuffer(IVertexBuffer * pDest, uint nColor);

	private:

		bool CheckColor(uint nColor);

		GLuint m_nFrameBuffer;  // color render target
		GLuint m_nDepthBuffer; // depth render target
		GLuint m_stencil_rb; // depth render target

		#define MAX_MRT_COUNT 4
		CTexture * m_pColorTexture[MAX_MRT_COUNT];
		uint m_nColorCount;

		uint m_nListID;

		static const GLenum s_drawBuffers[];
};


const GLenum CRenderTarget::s_drawBuffers[] =
{
	GL_COLOR_ATTACHMENT0_EXT,
	GL_COLOR_ATTACHMENT1_EXT,
	GL_COLOR_ATTACHMENT2_EXT,
	GL_COLOR_ATTACHMENT3_EXT,
};


CRenderTarget::CRenderTarget()
	: m_nFrameBuffer(0)
	, m_nDepthBuffer(0)
	, m_stencil_rb(0)
	, m_nColorCount(0)
{

}


/**
*
*/
CRenderTarget::~CRenderTarget()
{
	g_pRenderer->RemoveRenderTarget( m_nListID );

	if ( g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
	{
		if (0 != m_nDepthBuffer)
		{
			glDeleteFramebuffersEXT(1, &m_nDepthBuffer);
			GL_VALIDATE;
		}

		glDeleteFramebuffersEXT(1, &m_nFrameBuffer);
		GL_VALIDATE;
	}
}


bool CRenderTarget::CheckColor(uint nColor)
{
	if (nColor < m_nColorCount)
		return true;

	DEBUG_ASSERT(!"invalid color attachment");
	return false;
}


/**
*
*/
bool CRenderTarget::Init( uint nWidth, uint nHeight, uint nColorCount, bool bDepth )
{
	if ( false == g_pRenderer->IsExtSupported( EXT_GL_FBO ) )	
		return false;

	if (nColorCount > MAX_MRT_COUNT)
	{
		DEBUG_ASSERT(!"invalid MRT count");
		return false;
	}
	
	TImage tImage;
	{
		tImage.nWidth	= nWidth;
		tImage.nHeight	= nHeight;
		tImage.eFormat	= TImage::IMG_FMT_RGB32_FLOAT,
		//tImage.nSize	= nWidth * nHeight * 4, ησι
		tImage.pData	= NULL;
	};

	// Color -------------------------------------------------------------------

	glGenFramebuffersEXT(1, &m_nFrameBuffer);
	GL_VALIDATE;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
	GL_VALIDATE;

	for (uint i = 0; i < nColorCount; ++i)
	{
		m_pColorTexture[i] = g_pRenderer->CreateTexture("frame_buffer");
		m_pColorTexture[i]->Init(tImage, EAT_IMMUTABLE);

		GLint nTex = m_pColorTexture[i]->GetHandle();

		glFramebufferTexture2DEXT(
			GL_FRAMEBUFFER_EXT,
			GL_COLOR_ATTACHMENT0_EXT + i,
			GL_TEXTURE_2D,
			nTex,
			0);
		GL_VALIDATE;
	}

	m_nColorCount = nColorCount;

	// Depth -------------------------------------------------------------------

	if (true == bDepth)
	{
		glGenRenderbuffersEXT(1, &m_nDepthBuffer);
		GL_VALIDATE;

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nDepthBuffer);
		GL_VALIDATE;

		// init depth buffer
		glRenderbufferStorageEXT(
			GL_RENDERBUFFER_EXT,
			GL_DEPTH_COMPONENT24,
			nWidth,
			nHeight);
		GL_VALIDATE;

		// attach depth buffer to current framebuffer
		glFramebufferRenderbufferEXT(
			GL_FRAMEBUFFER_EXT,
			GL_DEPTH_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT,
			m_nDepthBuffer);
		GL_VALIDATE;
	}

	// Finalize -----------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_nFrameBuffer );
	GL_VALIDATE;

	glDrawBuffersARB(m_nColorCount, s_drawBuffers);
	GL_VALIDATE;

	return true;
}


bool CRenderTarget::CopyToVertexBuffer(IVertexBuffer * pDest, uint nColor)
{
	if (false == CheckColor(nColor))
		return false;

	const TImage & textureDesc = m_pColorTexture[nColor]->GetDesc();
	if (textureDesc.nSize != pDest->GetSize())
	{
		DEBUG_ASSERT(!"incompatible buffer sizes!");
		return false;
	}

	g_pRenderer->SetRenderTarget(this);

	// set source buffer for reading
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + nColor);

	glBindBufferARB(GL_PIXEL_PACK_BUFFER_EXT, pDest->GetHandle());
	GL_VALIDATE;

	glReadPixels(
		0, 0,
		textureDesc.nWidth, textureDesc.nHeight,
		GL_RGB, GL_FLOAT,
		NULL	// destination buffer pointer
		);
	GL_VALIDATE;

	// disable buffer for reading
	glReadBuffer(GL_NONE);
	GL_VALIDATE;

	glBindBufferARB(GL_PIXEL_PACK_BUFFER_EXT, 0);
	GL_VALIDATE;
	
	g_pRenderer->SetRenderTarget(NULL);

	return true;
}


/**
*
*/
PTexture CRenderTarget::GetTexture(uint nColor)
{
	PTexture res;

	if (true == CheckColor(nColor))
	{
		res = m_pColorTexture[nColor];
	}
	
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

		glDrawBuffer(GL_BACK);
		GL_VALIDATE;
	}
	else
	{
		pRT->Bind();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////