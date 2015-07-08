////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Shaders/BaseShader.h"


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
class CMesh : public IMesh
{
	friend CRenderer_GL;

	public :

		/**
		*
		*/
		inline CMesh() :
			m_nLockOffset	( 0 ),
			m_nLockSize		( 0 ),
			m_nBufferID		( 0 ),
			m_pVB			( NULL ),
			m_pIB			( NULL )
		{
			m_Header.nVertexCount	= 0;
			m_Header.nVertexStride	= 0;
			m_Header.nNumComponents	= 0;
			m_Header.nNumSurfaces	= 0;
		}


		/**
		*
		*/
		~CMesh()
		{
			//m_pComponents.Delete();
			m_pSurfaces.Delete();
			m_pMaterial = NULL;
		}


		/**
		*
		*/
		bool SetMaterial( CMaterial * pMat )
		{
			m_pMaterial = pMat;
			return false;
		}

		/**
		*
		*/
		//bool SetVertexBuffer( const void * pVB, size_t nVertexCount, size_t nStride )
		bool SetVertexBuffer( CVertexBuffer * pVB )
		{
			m_pVB = pVB;
/*
			if ( pVB && nVertexCount && nStride )
			{
				const size_t nSize = nVertexCount * nStride;

				if ( m_pVB = NEW byte [ nSize ] )
				{
					memcpy( m_pVB, pVB, nSize );
					m_Header.nVertexCount = nVertexCount;
					m_Header.nVertexStride= nStride;

					if ( !m_nBufferID )
						glGenBuffersARB( 1, &m_nBufferID );

					if ( m_nBufferID )
					{
						const size_t nSize = m_Header.nVertexCount * m_Header.nVertexStride;
						glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nBufferID );
						glBufferDataARB( GL_ARRAY_BUFFER_ARB, nSize, m_pVB, GL_STATIC_DRAW_ARB );
						glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
					}

					return true;
				}
			}
*/
			return false;
		}



		/**
		*
		*/
		size_t AddSurface( const char * szName, CIndexBuffer * pIB )
		{
			m_pIB = pIB;
			/*
			if ( !szName || !pIndices || !nNumIndices )
			{
				return INVALID_INDEX;
			}

			TSurface * pSurf = NEW TSurface;

			if ( pSurf )
			{
				pSurf->hdr.nNumIndices = nNumIndices;
				strcpy( pSurf->hdr.szName, szName );

				pSurf->bHidden		= false;
				pSurf->nBufferID	= 0;
				pSurf->pIndices		= pIndices;

				glGenBuffersARB( 1, &pSurf->nBufferID );
				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pSurf->nBufferID );

				glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
					nNumIndices * sizeof( short ), pIndices, GL_STATIC_DRAW_ARB );

				glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

				m_pSurfaces.Push( pSurf );
			}
*/
			return -1;
		}


		/**
		/* Возвращает кол-во поверхностей для данной сетки
		*/
		size_t GetSurfaceCount() const
		{
			return m_pSurfaces.GetCount();
		}


		/**
		*
		*/
		void RenderSurface( size_t nSurf )
		{
			m_pMaterial->Assign();
			m_pVB->Render( m_pIB );

			/*if ( m_pIB )
			{
				short * pIndices = (short *)m_pIB->Lock( 0, , true );

				for ( size_t nTri = 0; nTri < ( m_pIB->GetNumIndices() / 3 ); ++nTri )
				{
					size_t nOffset = nTri * 3;

					struct TVertex
					{
						vec3 vPos;
						vec2 vTex;
						vec3 vNormal;
					};

					const TVertex & v1 = ((TVertex*)m_pVB)[ pIndices[ nOffset++ ] ];
					const TVertex & v2 = ((TVertex*)m_pVB)[ pIndices[ nOffset++ ] ];
					const TVertex & v3 = ((TVertex*)m_pVB)[ pIndices[ nOffset ] ];

					g_pRenderer->PushLine3D( v1.vPos, v1.vPos + v1.vNormal, 0xFFFFFFFF );
					g_pRenderer->PushLine3D( v2.vPos, v2.vPos + v2.vNormal, 0xFFFFFFFF );
					g_pRenderer->PushLine3D( v3.vPos, v3.vPos + v3.vNormal, 0xFFFFFFFF );
				}
			}*/
		}


		/**
		*
		*/
		void Render()
		{
			const size_t nSurfCount = GetSurfaceCount();

			if ( nSurfCount )
			{
				//--------------------------------------------------------------
				// Tip: Вывод трекгольников с использованием IndexBuffer'а
				//--------------------------------------------------------------
				DEBUG_ASSERT( PRIM_TRIANGLE == m_ePrimType );

				for ( uint i = 0; i < nSurfCount; ++i )
				{
					RenderSurface( i );
				}
			}
			else
			{
				RenderSurface( INVALID_INDEX ); // triangle strip
			}
		}

	private :

		vec4 *		m_pTangents;

		size_t						m_nLockOffset;
		size_t						m_nLockSize;

		EPrimitive					m_ePrimType;


		TMeshHeader					m_Header;		// Mesh Header
		CStack <TSurface *>			m_pSurfaces;	// Surfaces

		GLuint						m_nBufferID;	// OpenGL specific
		CMaterial *					m_pMaterial;

		CVertexBuffer *				m_pVB;
		CIndexBuffer *				m_pIB;
};

////////////////////////////////////////////////////////////////////////////////