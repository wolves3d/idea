////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "EntityManager.h"

#include "Abilities/ModelAbility.h"

/**
*
*/
CEntity::CEntity() :
	m_bDirtyMatrix	( true ),
	m_pModel		( NULL )
{
	m_vScale.Set( 1, 1, 1 );

	m_qOrient.Set( 0, 0, 0, 1 );
	//m_qOrient.vDir.Set( 0, 0, 0 );
	//m_qOrient.fAngle = 1;

	m_vPos.Set( 0, 0, 0 );
}


/**
*
*/
CEntity::~CEntity()
{
	DEL( m_pModel );
}


/**
*
*/
void CEntity::Release()
{
	PURE_DEL( this );
}

IModel * CEntity::GetModel()
{
	return m_pModel;
}

/**
*
*/
void CEntity::Update()
{
	if ( m_bDirtyMatrix )
		UpdateMatrix();
}


/**
*
*/
void CEntity::UpdateMatrix()
{
	quat qRot( m_qOrient );
	qRot.RotateY( m_fDir );

	mat4 mRot;
	qRot.ToMatrix( mRot );

	m_mWorld.Identity();
	m_mWorld.Scale( m_vScale );
	m_mWorld.Mult( mRot ); //m_mWorld.Rotate( m_qOrient );
	m_mWorld.Translate( m_vPos );

	m_bDirtyMatrix = false;
}


/**
*
*/
void CEntity::Render()
{
	if ( m_pModel )
	{
		if ( m_bDirtyMatrix )
			UpdateMatrix();

		m_pModel->Render( m_mWorld );
	}
}

////////////////////////////////////////////////////////////////////////////////