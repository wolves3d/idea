////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Scene.h"
#include "_Shared/api/IXMLWrapper.h"


/**
*
*
CSceneObject * CScene::LoadSceneObj( const char * szFileName )
{
	CSceneObject * pObject = NULL;
	CXMLElement * pXMLMaterial;
	IXMLWrapper * pWrapper = (IXMLWrapper *)g_pSystem->GetInterface( IT_UTIL );

	if ( R_OK == pWrapper->LoadXML( szFileName, &pXMLMaterial ) )
	{
		if ( pObject = NEW CSceneObject )
		{
			CXMLAttr * pAttr = pXMLMaterial->GetAttr( "Material" );

			if ( pAttr )
			{
				pObject->pMaterial = g_pEngine->CreateMaterial();
				//pObject->pMaterial = (CMaterial *)g_pEngine->LoadMaterial( pAttr->GetValue() );
			}

			if ( pAttr = pXMLMaterial->GetAttr( "Model" ) )
			{
				//g_pEngine->m_pImageManager->LoadModel( pAttr->GetValue(), &pObject->pMesh );
			}
		}

		DEL( pXMLMaterial );
	}

	return pObject;
}


/*
================================================================================
// Name:
// Desc:
================================================================================
*
CScene::~CScene()
{
//	m_RenderQueue.Delete();
}


/*
================================================================================
// Name:
// Desc:
================================================================================
*
bool CScene::QueueObject( CSceneObject *pObject )
{
	if ( pObject )
	{
		m_RenderQueue.Add( pObject );
		return true;
	}

	return false;
}


/*
================================================================================
// Name:
// Desc:
================================================================================
*
void CScene::Render()
{
	for ( index_t nIndex = INVALID_INDEX; m_RenderQueue.Iterate( nIndex ); )
	{
		const CSceneObject * pObject = m_RenderQueue[ nIndex ];

		//pObject->Render();
	}

	m_RenderQueue.Clear();
}
*/
////////////////////////////////////////////////////////////////////////////////