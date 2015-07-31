/*
================================================================================
//	Name: frame buffer
//	Desc:
================================================================================
*/
extern PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC						glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC						glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC					glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC					glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC					glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT;

// GL_ARB_draw_buffers
extern PFNGLDRAWBUFFERSARBPROC							glDrawBuffersARB;

/*
================================================================================
//	Name: ARB_vertex_buffer_object
//	Desc:
================================================================================
*/
extern	PFNGLBINDBUFFERARBPROC				glBindBufferARB;
extern	PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB;
extern	PFNGLGENBUFFERSARBPROC				glGenBuffersARB;
extern	PFNGLISBUFFERARBPROC				glIsBufferARB;
extern	PFNGLBUFFERDATAARBPROC				glBufferDataARB;
extern	PFNGLBUFFERSUBDATAARBPROC			glBufferSubDataARB;
extern	PFNGLGETBUFFERSUBDATAARBPROC		glGetBufferSubDataARB;
extern	PFNGLMAPBUFFERARBPROC				glMapBufferARB;
extern	PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB;
extern	PFNGLGETBUFFERPARAMETERIVARBPROC	glGetBufferParameterivARB;
extern	PFNGLGETBUFFERPOINTERVARBPROC		glGetBufferPointervARB;

/*
================================================================================
//	Name: GL_ARB_vertex_program
//	Desc:
================================================================================
*/
extern	PFNGLGENPROGRAMSARBPROC		glGenProgramsARB;
extern	PFNGLBINDPROGRAMARBPROC		glBindProgramARB;
extern	PFNGLPROGRAMSTRINGARBPROC	glProgramStringARB;

extern	PFNGLPROGRAMLOCALPARAMETER4FVARBPROC	glProgramLocalParameter4fvARB;
extern	PFNGLENABLEVERTEXATTRIBARRAYARBPROC		glEnableVertexAttribArrayARB;
extern	PFNGLDISABLEVERTEXATTRIBARRAYARBPROC	glDisableVertexAttribArrayARB;
extern	PFNGLVERTEXATTRIBPOINTERARBPROC			glVertexAttribPointerARB;

/*
================================================================================
//	Name: GL_ARB_vertex_shader
//	Desc:
================================================================================
*/
extern	PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocationARB;
extern	PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB;

/*
================================================================================
//	Name: GL_ARB_shader_objects
//	Name: GL_ARB_shading_language_100
//	Desc:
================================================================================
*/
extern	PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
extern	PFNGLGETHANDLEARBPROC				glGetHandleARB;
extern	PFNGLDETACHOBJECTARBPROC			glDetachObjectARB;
extern	PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
extern	PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
extern	PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
extern	PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
extern	PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
extern	PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
extern	PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
extern	PFNGLVALIDATEPROGRAMARBPROC			glValidateProgramARB;
extern	PFNGLUNIFORM1FARBPROC				glUniform1fARB;
extern	PFNGLUNIFORM2FARBPROC				glUniform2fARB;
extern	PFNGLUNIFORM3FARBPROC				glUniform3fARB;
extern	PFNGLUNIFORM4FARBPROC				glUniform4fARB;
extern	PFNGLUNIFORM1IARBPROC				glUniform1iARB;
extern	PFNGLUNIFORM2IARBPROC				glUniform2iARB;
extern	PFNGLUNIFORM3IARBPROC				glUniform3iARB;
extern	PFNGLUNIFORM4IARBPROC				glUniform4iARB;
extern	PFNGLUNIFORM1FVARBPROC				glUniform1fvARB;
extern	PFNGLUNIFORM2FVARBPROC				glUniform2fvARB;
extern	PFNGLUNIFORM3FVARBPROC				glUniform3fvARB;
extern	PFNGLUNIFORM4FVARBPROC				glUniform4fvARB;
extern	PFNGLUNIFORM1IVARBPROC				glUniform1ivARB;
extern	PFNGLUNIFORM2IVARBPROC				glUniform2ivARB;
extern	PFNGLUNIFORM3IVARBPROC				glUniform3ivARB;
extern	PFNGLUNIFORM4IVARBPROC				glUniform4ivARB;
extern	PFNGLUNIFORMMATRIX2FVARBPROC		glUniformMatrix2fvARB;
extern	PFNGLUNIFORMMATRIX3FVARBPROC		glUniformMatrix3fvARB;
extern	PFNGLUNIFORMMATRIX4FVARBPROC		glUniformMatrix4fvARB;
extern	PFNGLGETOBJECTPARAMETERFVARBPROC	glGetObjectParameterfvARB;
extern	PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;
extern	PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
extern	PFNGLGETATTACHEDOBJECTSARBPROC		glGetAttachedObjectsARB;
extern	PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
extern	PFNGLGETACTIVEUNIFORMARBPROC		glGetActiveUniformARB;
extern	PFNGLGETUNIFORMFVARBPROC			glGetUniformfvARB;
extern	PFNGLGETUNIFORMIVARBPROC			glGetUniformivARB;
extern	PFNGLGETSHADERSOURCEARBPROC			glGetShaderSourceARB;

////////////////////////////////////////////////////////////////////////////////

typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
extern	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;