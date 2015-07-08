////////////////////////////////////////////////////////////////////////////////

#include "common.h"

/**
*
*/
bool CRenderer_GL::ClearScene()
{
	glClearColor( 0, 0, 0, 0 );
	GL_VALIDATE;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	GL_VALIDATE;

	return true;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
bool CRenderer_GL::Present()
{
	m_pFrameCounter->IncValue();

	//glFinish();
	SwapBuffers( m_hDevCon );
	GL_VALIDATE;

	return true;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
const mat4 * CRenderer_GL::GetMatrix( EMatrixMode eMode )
{
	switch ( eMode )
	{
		case MM_PROJECTION:
			return & m_mProjection;

		case MM_MODELVIEW:
			return & m_mModelView;

		case MM_TEXTURE:
			return & m_mTexture;

		default:
			DEBUG_ASSERT( !"Invalid enum" );
		return NULL;
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::SetMatrix( EMatrixMode eMode, const mat4 & mMatrix )
{
	switch ( eMode )
	{
		case MM_PROJECTION:
			m_mProjection = mMatrix;
			glMatrixMode( GL_PROJECTION );
			goto set_matrix;
		return;

		case MM_MODELVIEW:
			m_mModelView = mMatrix;
			glMatrixMode( GL_MODELVIEW );
			goto set_matrix;
		break;

		case MM_TEXTURE:
			m_mTexture = mMatrix;
			glMatrixMode( GL_TEXTURE );
			goto set_matrix;
		return;

		default:
			DEBUG_ASSERT( !"Invalid enum" );
		return;
	}

	set_matrix:
		glLoadMatrixf( mMatrix.pArray );
		GL_VALIDATE;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::MultMatrix( EMatrixMode eMode, const mat4 & mMatrix )
{
	switch ( eMode )
	{
		case MM_PROJECTION:
			m_mProjection = mMatrix;
			glMatrixMode( GL_PROJECTION );
			goto set_matrix;
			return;

		case MM_MODELVIEW:
			m_mModelView = mMatrix;
			glMatrixMode( GL_MODELVIEW );
			goto set_matrix;
			break;

		case MM_TEXTURE:
			m_mTexture = mMatrix;
			glMatrixMode( GL_TEXTURE );
			goto set_matrix;
			return;

		default:
			DEBUG_ASSERT( !"Invalid enum" );
			return;
	}

	set_matrix:
	glMultMatrixf( mMatrix.pArray );
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::PushMatrix( EMatrixMode eMode )
{
	switch ( eMode )
	{
		case MM_PROJECTION:
			m_ProjectionStack.Push( m_mProjection );
		break;

		case MM_MODELVIEW:
			m_ModelViewStack.Push( m_mModelView );
		break;

		case MM_TEXTURE:
			m_TextureStack.Push( m_mTexture );
		break;

		default:
			DEBUG_ASSERT( !"Invalid enum" );
	}
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
void CRenderer_GL::PopMatrix( EMatrixMode eMode )
{
	mat4 m;

	switch ( eMode )
	{
		case MM_PROJECTION:
			if ( m_ProjectionStack.Pop( m ) )
				SetMatrix( MM_PROJECTION, m );
		break;

		case MM_MODELVIEW:
			if ( m_ModelViewStack.Pop( m ) )
				SetMatrix( MM_MODELVIEW, m );
		break;

		case MM_TEXTURE:
			if ( m_TextureStack.Pop( m ) )
				SetMatrix( MM_TEXTURE, m );
		break;

		default:
			DEBUG_ASSERT( !"Invalid enum" );
	}
}


////////////////////////////////////////////////////////////////////////////////