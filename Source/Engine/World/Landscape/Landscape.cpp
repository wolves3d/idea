////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Landscape.h"

#include "gl/glu.h"
//#include "d3dx9.h"
#pragma comment ( lib, "glu32.lib" )









/**
*
*/
class CFrustum
{
	public:

	inline bool IsPointInFrustum( const vec3 & vPos )
	{
		return IsSphereInFrustum( vPos, 0 );
	}

	inline bool IsSphereInFrustum( const vec3 & vPos, float fRadius )
	{
		// Go through all the sides of the frustum
		for ( int i = 0; i < 6; ++i )	
		{
			// If the center of the sphere is farther away from the plane than the radius
			if ( ( Dot( pPlanes[ i ].vNormal, vPos ) + pPlanes[ i ].fOffset ) <= -fRadius )
			{
				// The distance was greater than the radius so the sphere is outside of the frustum
				return false;
			}
		}

		// The sphere was inside of the frustum!
		return true;
	}

	inline void Recalc( const mat4 & mProj, const mat4 & mModel )
	{
		mat4 mClip = mModel;
		mClip.Mult( mProj );

		// Расчитываем ЛЕВУЮ плоскость пирамиды
		plLeft.vNormal.x	= mClip.m14 + mClip.m11;
		plLeft.vNormal.y	= mClip.m24 + mClip.m21;
		plLeft.vNormal.z	= mClip.m34 + mClip.m31;
		plLeft.fOffset		= mClip.m44 + mClip.m41;
		plLeft.Normalize();

		// Расчитываем ПРАВУЮ плоскость пирамиды
		plRight.vNormal.x	= mClip.m14 - mClip.m11;
		plRight.vNormal.y	= mClip.m24 - mClip.m21;
		plRight.vNormal.z	= mClip.m34 - mClip.m31;
		plRight.fOffset		= mClip.m44 - mClip.m41;
		plRight.Normalize();

		// Расчитываем ВЕРХНЮЮ плоскость пирамиды
		plTop.vNormal.x	= mClip.m14 - mClip.m12;
		plTop.vNormal.y	= mClip.m24 - mClip.m22;
		plTop.vNormal.z	= mClip.m34 - mClip.m32;
		plTop.fOffset	= mClip.m44 - mClip.m42;
		plTop.Normalize();

		// Расчитываем НИЖНЮЮ плоскость пирамиды
		plBottom.vNormal.x	= mClip.m14 + mClip.m12;
		plBottom.vNormal.y	= mClip.m24 + mClip.m22;
		plBottom.vNormal.z	= mClip.m34 + mClip.m32;
		plBottom.fOffset	= mClip.m44 + mClip.m42;
		plBottom.Normalize();

		// Расчитываем БЛИЖНЮЮ плоскость пирамиды
		plNear.vNormal.x	= mClip.m14 - mClip.m13;
		plNear.vNormal.y	= mClip.m24 - mClip.m23;
		plNear.vNormal.z	= mClip.m34 - mClip.m33;
		plNear.fOffset		= mClip.m44 - mClip.m43;
		plNear.Normalize();

		// Расчитываем ДАЛЬНЮЮ плоскость пирамиды
		plFar.vNormal.x	= mClip.m14 + mClip.m13;
		plFar.vNormal.y	= mClip.m24 + mClip.m23;
		plFar.vNormal.z	= mClip.m34 + mClip.m33;
		plFar.fOffset	= mClip.m44 + mClip.m43;
		plFar.Normalize();
	}

	union
	{
		struct
		{
			CPlane	plNear,
					plFar,
					plTop,
					plBottom,
					plLeft,
					plRight;
		};

		struct
		{
			CPlane	pPlanes[ 6 ];
		};
	};
};


/**
*
*/
CLandscape::CLandscape() :
	m_vSize		( 0, 0	),
//	m_pShader	( NULL	),
	m_pVertices	( NULL	),
	m_pIndices	( NULL	),
	m_pTiles	( NULL	),
	m_nTileCount( 0		)
{

}


/**
*
*/
CLandscape::~CLandscape()
{
	DEL_ARRAY( m_pTiles );

	//DEL( pMat );
	DEL_ARRAY( m_pVertices );
	DEL_ARRAY( m_pIndices );
}


/**
*
*/
void CLandscape::Load( const char * szFileName )
{
	if ( !szFileName )
		return;

	IFile * pFile = NULL;
	g_pVfs->OpenFile( &pFile, szFileName, VFS_READ | VFS_BINARY );

	if ( !pFile )
		return;

    byte * pHeightMap = NEW byte [ pFile->GetSize() ];

	if ( pHeightMap )
		g_pVfs->Read( pFile, pHeightMap, pFile->GetSize() );

	g_pVfs->CloseFile( &pFile );

	//m_vSize.Set( 128, 128 );

	m_nTileCount = 16;
	m_pTiles = NEW CLandTile [ m_nTileCount ];

	for ( uint y = 0; y < 4; ++y )
	{
		for ( uint x = 0; x < 4; ++x )
		{
			uint nOffset = ( 129 * y * CLandTile::SIDE_CELL_COUNT ) + ( CLandTile::SIDE_CELL_COUNT * x );

			uint nTile		= y * 4 + x;
			m_pTiles[ nTile ].Create( pHeightMap + nOffset, 129 );
		}
	}

/*
	// nrml

	for ( uint nTri = 0; nTri < m_nTriangleCount; ++nTri )
	{
		int nOffset = nTri * 3;

		TVertex & v1 = pVertices[ m_pIndices[ nOffset++ ] ];
		TVertex & v2 = pVertices[ m_pIndices[ nOffset++ ] ];
		TVertex & v3 = pVertices[ m_pIndices[ nOffset   ] ];

		vec3 vNormal = Cross( v1.vPos - v3.vPos, v1.vPos - v2.vPos );
		vNormal.Normalize();

		/*v1.vNormal = vNormal;
		v2.vNormal = vNormal;
		v3.vNormal = vNormal;*
	}*/

	DEL_ARRAY( pHeightMap );

	m_pVDecl = g_pRenderer->CreateVertexDecl();

	if ( m_pVDecl )
	{
		m_pVDecl->SetStride( sizeof( TLandVertex ) );
		m_pVDecl->AddAttr( "vPos", NULL, TYPE_VEC3, 0 );
		m_pVDecl->AddAttr( "vTex0", NULL, TYPE_VEC2, sizeof( vec3 ) );
	}
}



ivec2 g_vMouse;

void CLandscape::Render( ICamera * pCam )
{
	//CFrustum frust;
	//frust.Recalc( *pProj, *pMdl );

	const mat4 & mVP = g_pEngine->GetCamera()->GetViewProjMatrix();

	CUniform unfmMVP( "mMVP", pMat->GetShader() );
	unfmMVP.Connect( pMat );

	bbox box;
	vec3 pRay[ 2 ];

	ivec2 vMouse;
	vMouse.x = g_vMouse.x;
	vMouse.y = g_vMouse.y;
	pCam->Unproject( pRay, vMouse );

// 	vec3 vLine1( pRay[0].x-1, pRay[0].y,  pRay[0].z );
// 	vec3 vLine2( pRay[1].x-1, pRay[1].y,  pRay[1].z );
// 	g_pEngine->PushLine( vLine1, vLine2, 0xFFFFFFFF );

	for ( int y = 0; y < 4; ++y )
	{
		for ( int x = 0; x < 4; ++x )
		{
			float fScale = 0.25f;
			uint nTile = ( y * 4 + x );
			CLandTile * pTile = m_pTiles + nTile;
	
			mat4 mTileWorld( mat4::mIdentity );
			mTileWorld.Scale( fScale );
			mTileWorld.Translate( vec3( (x-2) * CLandTile::SIDE_CELL_COUNT * fScale, 0, y * CLandTile::SIDE_CELL_COUNT * fScale ) );

			// debug
			box = pTile->m_bbox;
			box.Translate( mTileWorld );

			if ( box.RayHitTest( pRay ) )
			{
				g_pEngine->PushBBox( box, 0xFFFF0000 );
				
				mat4 mTileInv( mTileWorld );
				mTileInv.Inverse();

				vec3 vMyRay[] = { pRay[ 0 ], pRay[ 1 ] };
				vMyRay[0].Multiply( mTileInv );
				vMyRay[1].Multiply( mTileInv );
				pTile->Trace( vMyRay, &mTileWorld );
				
			}

			
			mTileWorld.Mult( mVP );
			unfmMVP.SetValue( (vec4*)mTileWorld.pArray, 4 );

			pMat->Assign();
			//pTile->m_pVB->SetVertexDecl( m_pVDecl );
			pTile->m_pVB->RenderIndexed(m_pVDecl, pTile->m_pIB, PRIM_TRIANGLE);

			//pTile->m_pVB->RenderIndexed(m_pVDecl, pTile->m_pIB, PRIM_POINT);
		}
	}
}


void CLandscape::Trace( ICamera * pCam, const ivec2 vMouse )
{
	g_vMouse = vMouse;
/*
	vec3 vMyRay[ 2 ];
	pCam->Unproject( vMyRay, vScreen );

	vMyRay[ 0 ].x -= 1;
	vMyRay[ 1 ].x -= 1;

	g_pEngine->PushLine( vMyRay[ 0 ], vMyRay[ 1 ], 0xFFFFFFFF );


	/*

	for ( uint n = 0; n < m_nTriangleCount; ++n )
	{
		vec3 vTriangle[] =
		{
			m_pVertices[ m_pIndices[ n * 3 ] ],
			m_pVertices[ m_pIndices[ n * 3 + 1 ] ],
			m_pVertices[ m_pIndices[ n * 3 + 2 ] ]
		};

		if ( RayTriangle( vRay, vTriangle ) )
		{
			vTriangle[ 0 ].y += 0.1f;
			vTriangle[ 1 ].y += 0.1f;
			vTriangle[ 2 ].y += 0.1f;

			g_pRenderer->SetSampler( 0, g_pRenderer->GetSysTexture( EST_WHITE ) );

			g_pRenderer->Line3D( vTriangle[ 0 ], vTriangle[ 1 ], 0xFFFFFFFF );
			g_pRenderer->Line3D( vTriangle[ 1 ], vTriangle[ 2 ], 0xFFFFFFFF );
			g_pRenderer->Line3D( vTriangle[ 2 ], vTriangle[ 0 ], 0xFFFFFFFF );
		}
	}
	*/
}


ILandscape * CEngine::CreateLandscape()
{
	return NEW CLandscape;
}

////////////////////////////////////////////////////////////////////////////////