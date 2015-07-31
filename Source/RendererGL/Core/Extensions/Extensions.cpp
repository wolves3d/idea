////////////////////////////////////////////////////////////////////////////////

#include "common.h"

/*
================================================================================
//	Name: frame buffer
//	Desc:
================================================================================
*/
PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT						= NULL;
PFNGLBINDRENDERBUFFEREXTPROC					glBindRenderbufferEXT					= NULL;
PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT				= NULL;
PFNGLGENRENDERBUFFERSEXTPROC					glGenRenderbuffersEXT					= NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT				= NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT			= NULL;
PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT						= NULL;
PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT					= NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT					= NULL;
PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT					= NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT				= NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				glFramebufferTexture1DEXT				= NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				glFramebufferTexture2DEXT				= NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				glFramebufferTexture3DEXT				= NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT			= NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT= NULL;
PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT						= NULL;

PFNGLDRAWBUFFERSARBPROC							glDrawBuffersARB						= NULL;


/*
================================================================================
//	Name: ARB_vertex_buffer_object
//	Desc:
================================================================================
*/
PFNGLBINDBUFFERARBPROC				glBindBufferARB				= NULL;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB			= NULL;
PFNGLGENBUFFERSARBPROC				glGenBuffersARB				= NULL;
PFNGLISBUFFERARBPROC				glIsBufferARB				= NULL;
PFNGLBUFFERDATAARBPROC				glBufferDataARB				= NULL;
PFNGLBUFFERSUBDATAARBPROC			glBufferSubDataARB			= NULL;
PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubDataARB		= NULL;
PFNGLMAPBUFFERARBPROC				glMapBufferARB				= NULL;
PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB			= NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC	glGetBufferParameterivARB	= NULL;
PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointervARB		= NULL;

/*
================================================================================
//	Name: GL_ARB_vertex_program
//	Desc:
================================================================================
*/
PFNGLGENPROGRAMSARBPROC		glGenProgramsARB	= NULL;
PFNGLBINDPROGRAMARBPROC		glBindProgramARB	= NULL;
PFNGLPROGRAMSTRINGARBPROC	glProgramStringARB	= NULL;

PFNGLPROGRAMLOCALPARAMETER4FVARBPROC	glProgramLocalParameter4fvARB	= NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC		glEnableVertexAttribArrayARB	= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	glDisableVertexAttribArrayARB	= NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC			glVertexAttribPointerARB		= NULL;

/*
================================================================================
//	Name: GL_ARB_vertex_shader
//	Desc:
================================================================================
*/
PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocationARB		= NULL;
PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB		= NULL;

/*
================================================================================
//	Name: GL_ARB_shader_objects
//	Name: GL_ARB_shading_language_100
//	Desc:
================================================================================
*/
PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB			= NULL;
PFNGLGETHANDLEARBPROC				glGetHandleARB				= NULL;
PFNGLDETACHOBJECTARBPROC			glDetachObjectARB			= NULL;
PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB		= NULL;
PFNGLSHADERSOURCEARBPROC			glShaderSourceARB			= NULL;
PFNGLCOMPILESHADERARBPROC			glCompileShaderARB			= NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB	= NULL;
PFNGLATTACHOBJECTARBPROC			glAttachObjectARB			= NULL;
PFNGLLINKPROGRAMARBPROC				glLinkProgramARB			= NULL;
PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB		= NULL;
PFNGLVALIDATEPROGRAMARBPROC			glValidateProgramARB		= NULL;
PFNGLUNIFORM1FARBPROC				glUniform1fARB				= NULL;
PFNGLUNIFORM2FARBPROC				glUniform2fARB				= NULL;
PFNGLUNIFORM3FARBPROC				glUniform3fARB				= NULL;
PFNGLUNIFORM4FARBPROC				glUniform4fARB				= NULL;
PFNGLUNIFORM1IARBPROC				glUniform1iARB				= NULL;
PFNGLUNIFORM2IARBPROC				glUniform2iARB				= NULL;
PFNGLUNIFORM3IARBPROC				glUniform3iARB				= NULL;
PFNGLUNIFORM4IARBPROC				glUniform4iARB				= NULL;
PFNGLUNIFORM1FVARBPROC				glUniform1fvARB				= NULL;
PFNGLUNIFORM2FVARBPROC				glUniform2fvARB				= NULL;
PFNGLUNIFORM3FVARBPROC				glUniform3fvARB				= NULL;
PFNGLUNIFORM4FVARBPROC				glUniform4fvARB				= NULL;
PFNGLUNIFORM1IVARBPROC				glUniform1ivARB				= NULL;
PFNGLUNIFORM2IVARBPROC				glUniform2ivARB				= NULL;
PFNGLUNIFORM3IVARBPROC				glUniform3ivARB				= NULL;
PFNGLUNIFORM4IVARBPROC				glUniform4ivARB				= NULL;
PFNGLUNIFORMMATRIX2FVARBPROC		glUniformMatrix2fvARB		= NULL;
PFNGLUNIFORMMATRIX3FVARBPROC		glUniformMatrix3fvARB		= NULL;
PFNGLUNIFORMMATRIX4FVARBPROC		glUniformMatrix4fvARB		= NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC	glGetObjectParameterfvARB	= NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB	= NULL;
PFNGLGETINFOLOGARBPROC				glGetInfoLogARB				= NULL;
PFNGLGETATTACHEDOBJECTSARBPROC		glGetAttachedObjectsARB		= NULL;
PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB		= NULL;
PFNGLGETACTIVEUNIFORMARBPROC		glGetActiveUniformARB		= NULL;
PFNGLGETUNIFORMFVARBPROC			glGetUniformfvARB			= NULL;
PFNGLGETUNIFORMIVARBPROC			glGetUniformivARB			= NULL;
PFNGLGETSHADERSOURCEARBPROC			glGetShaderSourceARB		= NULL;

////////////////////////////////////////////////////////////////////////////////

PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = NULL;

/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::InitExtensions()
{
	//----------------------------------------------------------------------
	// Tip: Check strings
	//----------------------------------------------------------------------
	const char * szExtensions = (const char *)glGetString( GL_EXTENSIONS );

	if ( !szExtensions )
	{
		return;
	}

	m_bExtensions[ EXT_GL_VBO ]			= ( NULL != strstr( szExtensions, "GL_ARB_vertex_buffer_object" ) );
	m_bExtensions[ EXT_GL_VRTX_PROGRAM ]= ( NULL != strstr( szExtensions, "GL_ARB_vertex_program" ) );
	m_bExtensions[ EXT_GL_FBO ]			= ( NULL != strstr( szExtensions, "GL_EXT_framebuffer_object" ) );

	//----------------------------------------------------------------------
	// Tip: Extensions
	//----------------------------------------------------------------------
	glIsRenderbufferEXT						=	(PFNGLISRENDERBUFFEREXTPROC)					wglGetProcAddress( "glIsRenderbufferEXT" );
	glBindRenderbufferEXT					=	(PFNGLBINDRENDERBUFFEREXTPROC)					wglGetProcAddress( "glBindRenderbufferEXT" );
	glDeleteRenderbuffersEXT				=	(PFNGLDELETERENDERBUFFERSEXTPROC)				wglGetProcAddress( "glDeleteRenderbuffersEXT" );
	glGenRenderbuffersEXT					=	(PFNGLGENRENDERBUFFERSEXTPROC)					wglGetProcAddress( "glGenRenderbuffersEXT" );
	glRenderbufferStorageEXT				=	(PFNGLRENDERBUFFERSTORAGEEXTPROC)				wglGetProcAddress( "glRenderbufferStorageEXT" );
	glGetRenderbufferParameterivEXT			=	(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)		wglGetProcAddress( "glGetRenderbufferParameterivEXT" );
	glIsFramebufferEXT						=	(PFNGLISFRAMEBUFFEREXTPROC)						wglGetProcAddress( "glIsFramebufferEXT" );
	glBindFramebufferEXT					=	(PFNGLBINDFRAMEBUFFEREXTPROC)					wglGetProcAddress( "glBindFramebufferEXT" );
	glDeleteFramebuffersEXT					=	(PFNGLDELETEFRAMEBUFFERSEXTPROC)				wglGetProcAddress( "glDeleteFramebuffersEXT" );
	glGenFramebuffersEXT					=	(PFNGLGENFRAMEBUFFERSEXTPROC)					wglGetProcAddress( "glGenFramebuffersEXT" );
	glCheckFramebufferStatusEXT				=	(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)			wglGetProcAddress( "glCheckFramebufferStatusEXT" );
	glFramebufferTexture1DEXT				=	(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)				wglGetProcAddress( "glFramebufferTexture1DEXT" );
	glFramebufferTexture2DEXT				=	(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)				wglGetProcAddress( "glFramebufferTexture2DEXT" );
	glFramebufferTexture3DEXT				=	(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)				wglGetProcAddress( "glFramebufferTexture3DEXT" );
	glFramebufferRenderbufferEXT			=	(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)			wglGetProcAddress( "glFramebufferRenderbufferEXT" );
	glGetFramebufferAttachmentParameterivEXT=	(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress( "glGetFramebufferAttachmentParameterivEXT" );
	glGenerateMipmapEXT						=	(PFNGLGENERATEMIPMAPEXTPROC)					wglGetProcAddress( "glGenerateMipmapEXT" );

	glDrawBuffersARB = (PFNGLDRAWBUFFERSARBPROC)wglGetProcAddress("glDrawBuffersARB");

	//--------------------------------------------------------------------------
	// Tip: ARB_vertex_buffer_object
	//--------------------------------------------------------------------------
	if ( m_bExtensions[ EXT_GL_VBO ] )
	{
		glBindBufferARB				= (PFNGLBINDBUFFERARBPROC)			wglGetProcAddress( "glBindBufferARB" );
		glDeleteBuffersARB			= (PFNGLDELETEBUFFERSARBPROC)		wglGetProcAddress( "glDeleteBuffersARB" );
		glGenBuffersARB				= (PFNGLGENBUFFERSARBPROC)			wglGetProcAddress( "glGenBuffersARB" );
		glIsBufferARB				= (PFNGLISBUFFERARBPROC)			wglGetProcAddress( "glIsBufferARB" );
		glBufferDataARB				= (PFNGLBUFFERDATAARBPROC)			wglGetProcAddress( "glBufferDataARB" );
		glBufferSubDataARB			= (PFNGLBUFFERSUBDATAPROC)			wglGetProcAddress( "glBufferSubDataARB" );
		glGetBufferSubDataARB		= (PFNGLGETBUFFERSUBDATAARBPROC)	wglGetProcAddress( "glGetBufferSubDataARB" );
		glMapBufferARB				= (PFNGLMAPBUFFERARBPROC)			wglGetProcAddress( "glMapBufferARB" );
		glUnmapBufferARB			= (PFNGLUNMAPBUFFERARBPROC)			wglGetProcAddress( "glUnmapBufferARB" );
		glGetBufferParameterivARB	= (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress( "glGetBufferParameterivARB" );
		glGetBufferPointervARB		= (PFNGLGETBUFFERPOINTERVARBPROC)	wglGetProcAddress( "glGetBufferPointervARB" );
	}

	//--------------------------------------------------------------------------
	// Tip: GL_ARB_vertex_program
	//--------------------------------------------------------------------------
	if ( m_bExtensions[ EXT_GL_VRTX_PROGRAM ] )
	{
		glGenProgramsARB	= (PFNGLGENPROGRAMSARBPROC)wglGetProcAddress( "glGenProgramsARB" );
		glBindProgramARB	= (PFNGLBINDPROGRAMARBPROC)wglGetProcAddress( "glBindProgramARB" );
		glProgramStringARB	= (PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress( "glProgramStringARB" );

		glProgramLocalParameter4fvARB=(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)wglGetProcAddress( "glProgramLocalParameter4fvARB" );
		glEnableVertexAttribArrayARB= (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress( "glEnableVertexAttribArrayARB" );
		glDisableVertexAttribArrayARB=(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress( "glDisableVertexAttribArrayARB" );
		glVertexAttribPointerARB	= (PFNGLVERTEXATTRIBPOINTERARBPROC)	wglGetProcAddress( "glVertexAttribPointerARB" );

		//----------------------------------------------------------------------
		// Tip: GL_ARB_vertex_shader
		//----------------------------------------------------------------------
		glBindAttribLocationARB		= (PFNGLBINDATTRIBLOCATIONARBPROC)	wglGetProcAddress( "glBindAttribLocationARB" );
		glGetAttribLocationARB		= (PFNGLGETATTRIBLOCATIONARBPROC)	wglGetProcAddress( "glGetAttribLocationARB" );
	}

	//--------------------------------------------------------------------------
	// Tip: GL_ARB_shader_objects
	// Tip: GL_ARB_shading_language_100
	//--------------------------------------------------------------------------
	glDeleteObjectARB			= (PFNGLDELETEOBJECTARBPROC)		wglGetProcAddress( "glDeleteObjectARB" );
	glGetHandleARB				= (PFNGLGETHANDLEARBPROC)			wglGetProcAddress( "glGetHandleARB" );
	glDetachObjectARB			= (PFNGLDETACHOBJECTARBPROC)		wglGetProcAddress( "glDetachObjectARB" );
	glCreateShaderObjectARB		= (PFNGLCREATESHADEROBJECTARBPROC)	wglGetProcAddress( "glCreateShaderObjectARB" );
	glShaderSourceARB			= (PFNGLSHADERSOURCEARBPROC)		wglGetProcAddress( "glShaderSourceARB" );
	glCompileShaderARB			= (PFNGLCOMPILESHADERARBPROC)		wglGetProcAddress( "glCompileShaderARB" );
	glCreateProgramObjectARB	= (PFNGLCREATEPROGRAMOBJECTARBPROC)	wglGetProcAddress( "glCreateProgramObjectARB" );
	glAttachObjectARB			= (PFNGLATTACHOBJECTARBPROC)		wglGetProcAddress( "glAttachObjectARB" );
	glLinkProgramARB			= (PFNGLLINKPROGRAMARBPROC)			wglGetProcAddress( "glLinkProgramARB" );
	glUseProgramObjectARB		= (PFNGLUSEPROGRAMOBJECTARBPROC)	wglGetProcAddress( "glUseProgramObjectARB" );
	glValidateProgramARB		= (PFNGLVALIDATEPROGRAMARBPROC)		wglGetProcAddress( "glValidateProgramARB" );
	glUniform1fARB				= (PFNGLUNIFORM1FARBPROC)			wglGetProcAddress( "glUniform1fARB" );
	glUniform2fARB				= (PFNGLUNIFORM2FARBPROC)			wglGetProcAddress( "glUniform2fARB" );
	glUniform3fARB				= (PFNGLUNIFORM3FARBPROC)			wglGetProcAddress( "glUniform3fARB" );
	glUniform4fARB				= (PFNGLUNIFORM4FARBPROC)			wglGetProcAddress( "glUniform4fARB" );
	glUniform1iARB				= (PFNGLUNIFORM1IARBPROC)			wglGetProcAddress( "glUniform1iARB" );
	glUniform2iARB				= (PFNGLUNIFORM2IARBPROC)			wglGetProcAddress( "glUniform2iARB" );
	glUniform3iARB				= (PFNGLUNIFORM3IARBPROC)			wglGetProcAddress( "glUniform3iARB" );
	glUniform4iARB				= (PFNGLUNIFORM4IARBPROC)			wglGetProcAddress( "glUniform4iARB" );
	glUniform1fvARB				= (PFNGLUNIFORM1FVARBPROC)			wglGetProcAddress( "glUniform1fvARB" );
	glUniform2fvARB				= (PFNGLUNIFORM2FVARBPROC)			wglGetProcAddress( "glUniform2fvARB" );
	glUniform3fvARB				= (PFNGLUNIFORM3FVARBPROC)			wglGetProcAddress( "glUniform3fvARB" );
	glUniform4fvARB				= (PFNGLUNIFORM4FVARBPROC)			wglGetProcAddress( "glUniform4fvARB" );
	glUniform1ivARB				= (PFNGLUNIFORM1IVARBPROC)			wglGetProcAddress( "glUniform1ivARB" );
	glUniform2ivARB				= (PFNGLUNIFORM2IVARBPROC)			wglGetProcAddress( "glUniform2ivARB" );
	glUniform3ivARB				= (PFNGLUNIFORM3IVARBPROC)			wglGetProcAddress( "glUniform3ivARB" );
	glUniform4ivARB				= (PFNGLUNIFORM4IVARBPROC)			wglGetProcAddress( "glUniform4ivARB" );
	glUniformMatrix2fvARB		= (PFNGLUNIFORMMATRIX2FVARBPROC)	wglGetProcAddress( "glUniformMatrix2fvARB" );
	glUniformMatrix3fvARB		= (PFNGLUNIFORMMATRIX3FVARBPROC)	wglGetProcAddress( "glUniformMatrix3fvARB" );
	glUniformMatrix4fvARB		= (PFNGLUNIFORMMATRIX4FVARBPROC)	wglGetProcAddress( "glUniformMatrix4fvARB" );
	glGetObjectParameterfvARB	= (PFNGLGETOBJECTPARAMETERFVARBPROC)wglGetProcAddress( "glGetObjectParameterfvARB" );
	glGetObjectParameterivARB	= (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress( "glGetObjectParameterivARB" );
	glGetInfoLogARB				= (PFNGLGETINFOLOGARBPROC)			wglGetProcAddress( "glGetInfoLogARB" );
	glGetAttachedObjectsARB		= (PFNGLGETATTACHEDOBJECTSARBPROC)	wglGetProcAddress( "glGetAttachedObjectsARB" );
	glGetUniformLocationARB		= (PFNGLGETUNIFORMLOCATIONARBPROC)	wglGetProcAddress( "glGetUniformLocationARB" );
	glGetActiveUniformARB		= (PFNGLGETACTIVEUNIFORMARBPROC)	wglGetProcAddress( "glGetActiveUniformARB" );
	glGetUniformfvARB			= (PFNGLGETUNIFORMFVARBPROC)		wglGetProcAddress( "glGetUniformfvARB" );
	glGetUniformivARB			= (PFNGLGETUNIFORMIVARBPROC)		wglGetProcAddress( "glGetUniformivARB" );
	glGetShaderSourceARB		= (PFNGLGETSHADERSOURCEARBPROC)		wglGetProcAddress( "glGetShaderSourceARB" );

	//////////////////////////////////////////////////////////////////////////

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
}
////////////////////////////////////////////////////////////////////////////////