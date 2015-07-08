////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "_Shared/Renderer/BaseVertexDecl.h"

////////////////////////////////////////////////////////////////////////////////

/**
*
*/
class CVertexDecl : public CBaseVertexDecl
{
	public:

	void	Assign( bool bAssign = true );
};

////////////////////////////////////////////////////////////////////////////////


/**
*
*
size_t GetElementCount( ETypeID eType )
{
	switch ( eType )
	{
		case TYPE_VEC2: return 2;
		case TYPE_VEC3: return 3;
		case TYPE_VEC4: return 4;
		case TYPE_DWORD: return 4;
	}

	return 1;
}*/






/**
*
*/
void CVertexDecl::Assign( bool bAssign )
{
//	if ( !m_bRegistred )
//		Register();

	for ( uint n = 0; n < m_pAttrs.GetCount(); ++n )
	{
		const TVertexAttr &tAttr = m_pAttrs[ n ];

		const GLint nElemCount = g_pElemCountGL[ tAttr.eType ];
		const GLenum eElemType = g_pElemTypeGL[ tAttr.eType ];

		//if ( INVALID_INDEX != tAttr.nLoc )
		{
			if ( bAssign )
			{
				if ( n == 0 )
				{
					glEnableClientState( GL_VERTEX_ARRAY );

					glVertexPointer(
						nElemCount,
						eElemType,
						(GLsizei)GetStride(),
						(void *)tAttr.nOffset );

					continue;
				}
				if ( g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
				{
					glVertexAttribPointerARB(
						n, //tAttr.nLoc,
						nElemCount,											// element count
						eElemType,										// element type
						( eElemType == GL_FLOAT ) ? GL_FALSE : GL_TRUE,	// need normalize int numbers?
						(GLsizei)GetStride(),								// stride
						(void *)tAttr.nOffset );						// offset
					GL_VALIDATE;

					glEnableVertexAttribArrayARB( n );
					GL_VALIDATE;
				}
				else
				{
					if ( n == 1 )
					{
						glEnableClientState( GL_TEXTURE_COORD_ARRAY );
						GL_VALIDATE;
					}
				}
			}
			else
			{
				/*
				if ( n == 0 )
				{
					glDisableClientState( GL_VERTEX_ARRAY );
					GL_VALIDATE;
					continue;
				}
				*/

				if ( g_pRenderer->IsExtSupported( EXT_GL_VRTX_PROGRAM ) )
				{
					glDisableVertexAttribArrayARB( n /* tAttr.nLoc*/ );
					GL_VALIDATE;
				}
				/*
				else
				{
					if ( n == 1 )
					{
						glDisableClientState( GL_TEXTURE_COORD_ARRAY );
						GL_VALIDATE;
					}
				}
				*/
			}
		}
	}
}


/**
*
*/
IVertexDecl * CRenderer_GL::CreateVertexDecl()
{
	return NEW CVertexDecl();
}

//////////////////////////////////////////////////////////////////////////