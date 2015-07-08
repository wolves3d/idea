////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Scene.h"
#include "../Material/Material.h"

/*
================================================================================
// Name:
// Desc:
================================================================================
*
void CSceneObject::Render() const
{
	// build model matrix
	g_pRenderer->PushMatrix( MM_MODELVIEW );
	mat4 mWorld = *g_pRenderer->GetMatrix( MM_MODELVIEW );
	mat4 mModel;

	m_qRot.ToMatrix( mModel );
	mModel.Translate( m_vPos );
	mModel.Mult( mWorld );
	g_pRenderer->SetMatrix( MM_MODELVIEW, mModel );

	// render
	/*IShader * pShader = pMaterial->m_pShader;
	pShader->SetSampler( 0, pMaterial->m_pTexture );
	g_pRenderer->SetShader( pShader );*

	pMesh->SetMaterial( pMaterial );
	pMesh->Render();

	g_pRenderer->PopMatrix( MM_MODELVIEW );
}*/


////////////////////////////////////////////////////////////////////////////////

CObject::CObject() :
	m_pParent			( NULL	),
	m_bDirtyMatrix		( true	),
	m_fLastUpdateTime	( 0.f	)
{
	m_vScale.Set( 1, 1, 1 );
	m_qRotAnim.Set( 0, 0, 0, 1 );
	m_vPos.Set( 0, 0, 0 );
}


CObject::~CObject()
{
}


void CObject::SetName( const char * szName )
{
	m_sName = szName;
}


const char * CObject::GetName()
{
	return m_sName.GetString();
}


void CObject::SetParent( CObject * pParent )
{
	m_pParent = pParent;
}


void CObject::SetParentName( const char * szName )
{
	m_sParentName = szName;
}


/*
*
*/
CObject * CObject::GetParent()
{
	return m_pParent;
}


/*
*
*/
void CObject::SetPos( vec3 & vPos )
{
	m_vPos = vPos;
	m_bDirtyMatrix = true;
}

void CObject::SetScale( vec3 & vScale )
{
	m_vScale = vScale;
	m_bDirtyMatrix = true;
}

void CObject::SetRot( quat & qRot )
{
	m_qRot = qRot;
	m_bDirtyMatrix = true;
}

void CObject::SetRotAnim( quat & qRot )
{
	m_qRotAnim = qRot;
	m_bDirtyMatrix = true;
}

bool CObject::IsMatrixRebuildNeeded( float fAnimTime )
{
	// матрицу не нужно перестраивать если не установлен
	// флаг m_bDirtyMatrix и последнее время обновления ровняется fAnimTime

	return ( !m_bDirtyMatrix && ( m_fLastUpdateTime == fAnimTime ) );
}


void CObject::RebuildMatrix( float fAnimTime )
{
	m_mMatrix.Identity();

	m_mMatrix.Scale( m_vScale );

	quat qR = m_qRot;
	qR.Mult( m_qRotAnim );

	mat4 mr;
	qR.ToMatrix( mr );
	m_mMatrix.Mult( mr );

	m_mMatrix.Translate( m_vPos );

	if ( m_pParent )
	{
		if ( true ) //m_pParent->IsMatrixRebuildNeeded( fAnimTime ) )
			m_pParent->RebuildMatrix( fAnimTime );

		m_mMatrix.Mult( m_pParent->GetMatrix() );
	}

	m_bDirtyMatrix = false;
}


const mat4 & CObject::GetMatrix()
{
	return m_mMatrix;
}

void CObject::Render()
{
/*	RebuildMatrix( 0.f );

	if ( m_pShape )
	{
		g_pRenderer->PushMatrix( MM_MODELVIEW );
		g_pRenderer->MultMatrix( MM_MODELVIEW, GetMatrix() );
		m_pShape->Render( m_pMaterial );
		g_pRenderer->PopMatrix( MM_MODELVIEW );
	}
	else
	{
		// no object
		g_pRenderer->Axis3D( GetMatrix() );
	}

	if ( GetParent() )
	{
		vec3 vStart( 0, 0, 0 );
		vec3 vEnd( 0, 0, 0 );

		vStart.Multiply( GetMatrix() );
		vEnd.Multiply( GetParent()->GetMatrix() );

		g_pRenderer->Line3D( vStart, vEnd, 0xFFFFFFFF );
	}*/
}

/*
void CObject::AddRotKey( float fTime, const quat & qRot )
{
	m_RotKeyQueue.Push( CRotKey( (int)fTime, qRot ) );
}*/


/**
* Подсчёт размера буффера ( для сохранения )
*
uint CObject::CalcSaveSize()
{
	FinalizeKeys();

	uint nSize = 0;

	nSize += 4;									// block size

	nSize += 32;								// name
	nSize += 32;								// parent name;

	nSize += sizeof( vec3 );					// vScale
	nSize += sizeof( quat );					// qRot
	nSize += sizeof( vec3 );					// vPos

	nSize += sizeof( uint );					// m_nNumRotKeys
	nSize += m_nNumRotKeys * sizeof( CRotKey );	// array of RotKeys

	return nSize;
}


bool CObject::Save( char * pBuffer, uint nMaxSize )
{
	uint nSize = CalcSaveSize();

	if ( !pBuffer )
		return false;

	if ( nMaxSize < nSize )
		return false;

	memset( pBuffer, 0, nSize );

	memcpy( pBuffer, &nSize, sizeof( uint ) );
	pBuffer += sizeof( uint );

	sprintf( pBuffer, "%s", GetName() );
	pBuffer += 32;

	if ( GetParent() )
		sprintf( pBuffer, "%s", GetParent()->GetName() );
	pBuffer += 32;

	memcpy( pBuffer, &m_vScale, sizeof( vec3 ) );
	pBuffer += sizeof( vec3 );

	memcpy( pBuffer, &m_qRot,	sizeof( quat ) );
	pBuffer += sizeof( quat );

	memcpy( pBuffer, &m_vPos,	sizeof( vec3 ) );
	pBuffer += sizeof( vec3 );

	memcpy( pBuffer, &m_nNumRotKeys,	sizeof( uint ) );
	pBuffer += sizeof( uint );

	if ( m_nNumRotKeys )
		memcpy( pBuffer, m_pRotKeys, m_nNumRotKeys * sizeof( CRotKey ) );

	return true;
}*/