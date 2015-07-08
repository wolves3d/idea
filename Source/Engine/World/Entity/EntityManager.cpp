////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "EntityManager.h"
#include "Shapes/Shape.h"
#include "_Shared/api/IXMLWrapper.h"

#include "Abilities/ModelAbility.h"

/**
*
*/
CEntityManager::CEntityManager()
{
	DECL_CLASS( CEntity, 0 );
	DECL_FIELD( CEntity, float, "direction", m_fDir );
}


/**
*
*/
void CEntityManager::Update()
{
	index_t it = INVALID_INDEX;

	while ( m_pEntites.Iterate( it ) )
	{
		CEntity * pEnt = m_pEntites[ it ];
		pEnt->Update();
		pEnt->Render();
	}
}


/**
*
*/
void CEntityManager::Render()
{
	//g_pRenderer->PushMatrix( MM_MODELVIEW );

	//g_pRenderer->SetMatrix( MM_MODELVIEW, m_pEntity->GetMatrix() );	//CShape * pShape = m_pEntity->GetShape();
	//pShape->Render( m_pEntity->GetMaterial() );

	//g_pRenderer->PopMatrix( MM_MODELVIEW );
}


/**
*
*/
IEntity * CEntityManager::CreateEntity( const char * szName )
{
	CEntity *	pEnt	= NEW CEntity;

	IXMLWrapper * pWrapper = (IXMLWrapper *)g_pSystem->GetInterface( IT_UTIL );

	CXMLElement * pEntXML = NULL;
	pWrapper->LoadXML( szName, &pEntXML );

	if ( NULL == pEntXML )
		return NULL;

	////////////////////////////////////////////////////////////////////////////

	CXMLAttr * pModelAttr = pEntXML->GetAttr( "Model" );

	if ( pModelAttr )
	{
		CModel * pModel = pEnt->m_pModel = NEW CModel;
		pModel->Load( pModelAttr->GetValue() );
	}
	
	/*	////////////////////////////////////////////////////////////////////////

		CXMLAttr * pMatAttr = pModelXML->GetAttr( "Material" );

		if ( pMatAttr )
		{
			IMaterial * pMat = g_pEngine->LoadMaterial( pMatAttr->GetValue() );

			pMat->SetShader( g_pRenderer->GetShader( "Mesh" ) );

			pEnt->m_pModel->m_pObjects[ 0 ].SetMaterial( pMat ); // omfg!!!
		}
	}
*/
	DEL( pEntXML );
	m_pEntites.Add( pEnt );
	return pEnt;

	/*error:

		DEL( pEnt );
		DEL( pMat );
		return NULL;*/
}


/**
*
*/
IEntity * CEntityManager::LoadEntity( const char * szFileName )
{
	//IFile * pFile;
	//g_pVfs->OpenFile( &pFile, szFileName, VFS_READ )
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////