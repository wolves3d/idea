////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ParticleSystem.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////

size_t nMaxParticles = 5000;

struct TParticleVertex
{
	vec3	vPos;
	vec2	vTex;
	dword	dwColor;
};



/**
*
*/
CParticleSystem::CParticleSystem() :
	m_nNumParticles	( 0		),
	m_pVB			( NULL	),
	m_pSysMemVB		( NULL	),
	m_fLastTime		( 0		)
{
	m_fRate = 1000;
	m_fTimeAccum = 0;
}


/**
*
*/
CParticleSystem::~CParticleSystem()
{
	RELEASE( m_pVB );
	DEL_ARRAY( m_pSysMemVB );
	DEL_ARRAY( m_pParticles );
}


/**
*
*/
void CParticleSystem::Init()
{
	uint nSize = sizeof( TParticleVertex ) * nMaxParticles * 6;

	m_pSysMemVB = NEW byte [ nSize ];
	m_pParticles = NEW TParticle [ nMaxParticles ];

	if ( m_pVB = g_pRenderer->CreateVB() )
	{
		m_pVB->Alloc( nSize, 0 );
	}

	m_fLastTime = g_pEngine->GetTime();
} 


/**
*
*/
void CParticleSystem::Update()
{
	const float fCurTime		= g_pEngine->GetTime();
	float		fEmitterTime	= fCurTime - m_fLastTime;

	m_fLastTime = fCurTime;

	//--------------------------------------------------------------------------
	// Tip: Emit new partcles
	//--------------------------------------------------------------------------
	int n = (int)( ( ( fEmitterTime + m_fTimeAccum ) * m_fRate ) + 0.5f );

	if ( n )
	{
		EmitParticles( n );
		m_fTimeAccum = 0;
	}
	else
	{
		m_fTimeAccum += fEmitterTime;
	}

	Rebuild();
}


/**
*
*/
void CParticleSystem::Render( IMaterial * pMaterial )
{
	DEBUG_ASSERT( pMaterial );
/*
	if ( m_Used.GetNumItems() && pMaterial )
	{	
		pMaterial->Assign();
		m_pVB->Render( 0, m_Used.GetNumItems() * 6, PRIM_TRIANGLE );
	}
	*/
}


/**
*
*/
void CParticleSystem::Rebuild()
{		
	const float fTime	= g_pEngine->GetTime();

	TParticleVertex * pData = (TParticleVertex *)m_pSysMemVB;

	size_t i = INVALID_INDEX;
	size_t l = 0;

/*	while ( m_Used.Iterate( i ) )
	{
		TParticle & tParticle = m_pParticles[ m_Used[ i ] ];

		float fAge = fTime - tParticle.fBornTime;

		if ( fAge > tParticle.fLifeTime )
		{
			m_Used.Remove( i );
			continue;
		}

		dword dwColor = tParticle.dwColor;

		tParticle.vPos = ( tParticle.vSpeed * fAge ) +
			( ( tParticle.vAccel * fAge * fAge ) / 2 );

		size_t nVertex = l * 6;

		pData[ nVertex ].vPos.Set( 0, 0, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 0, 1 );
		pData[ nVertex ].dwColor = dwColor;
		++nVertex;

		pData[ nVertex ].vPos.Set( tParticle.fScale, 0, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 1, 1 );
		pData[ nVertex ].dwColor = dwColor;
		++nVertex;

		pData[ nVertex ].vPos.Set( tParticle.fScale, tParticle.fScale, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 1, 0 );
		pData[ nVertex ].dwColor = dwColor;
		++nVertex;

		pData[ nVertex ].vPos.Set( 0, 0, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 0, 1 );
		pData[ nVertex ].dwColor = dwColor;
		++nVertex;

		pData[ nVertex ].vPos.Set( tParticle.fScale, tParticle.fScale, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 1, 0 );
		pData[ nVertex ].dwColor = dwColor;
		++nVertex;

		pData[ nVertex ].vPos.Set( 0, tParticle.fScale, 0 );
		pData[ nVertex ].vPos.Add( tParticle.vPos );
		pData[ nVertex ].vTex.Set( 0, 0 );
		pData[ nVertex ].dwColor = dwColor;
		++l;
	}

	m_pVB->UpdateData( 0, m_pSysMemVB, m_Used.GetNumItems() * 6 * sizeof( TSpriteVertex ) );

	*/
}


/**
*
*/
void CParticleSystem::EmitParticles( size_t nNumParticles )
{
	return;

	for ( size_t i = 0; i < nNumParticles; ++i )
	{

//		if ( m_Used.GetNumItems() >= nMaxParticles )
//			break;
		
		TParticle * pParticle = NEW TParticle;

		if ( pParticle )
		{
			float x = (float)rand() / 32768 * M_PI * 2;
			float y = (float)rand() / 32768 * M_PI;
			float s = (float)rand() / 32768 * 3;

			pParticle->vPos.Set( 0, 0, s );
			pParticle->vSpeed.Set( sin( x ), cos( y ), 0 );
			pParticle->vSpeed.NormalizeTo( s );
			pParticle->vAccel.Set( 0, -9, 0 );
			pParticle->fScale = 3;
			pParticle->dwColor = 0xFF000000 | rand(); // ABGR

			pParticle->fLifeTime = 2;
			pParticle->fBornTime = m_fLastTime;

//			m_Used.Add( pParticle );
		}
	}
}


/**
*
*/
IParticleSystem * CEngine::CreateParticleSys()
{
	return NEW CParticleSystem;
}