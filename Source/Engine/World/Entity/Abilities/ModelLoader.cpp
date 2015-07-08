////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ModelAbility.h"


/**
*
*/
void CModel::LoadObjects( const char * szFile )
{
	const int nFileVer = 1;

	IFile * pFile;

	if ( R_OK != g_pVfs->OpenFile( &pFile, szFile,  VFS_BINARY | VFS_READ ) )
	{
		SysWrite( Va( "Failed to open model file \"%s\"", szFile ) );
		return;
	}


	TModelFileHdr tFileHdr;

	//--------------------------------------------------------------------------
	// Tip: Читаем заголовок MODEL файла
	//--------------------------------------------------------------------------
	if ( !g_pVfs->CheckRead( pFile, &tFileHdr, sizeof( TModelFileHdr ) ) )
	{
		// invalid file
		goto error_label;
	}

	if (	/*( 0 != strcmp( "MDL ", (char *)&tFileHdr.nSign ) ) ||*/
			( 0 == tFileHdr.nNumObjects ) ||
			( nFileVer != tFileHdr.nVersion ) ||
			( sizeof( TModelFileHdr ) != tFileHdr.nHeaderSize ) )
	{
		// invalid file or no objects
		goto error_label;
	}
	
	m_nNumObjects	= tFileHdr.nNumObjects;
	m_pObjects		= NEW CObject [ m_nNumObjects ];

	if ( NULL == m_pObjects )
		goto error_label;

	//--------------------------------------------------------------------------
	// Tip: Читаем mesh
	//--------------------------------------------------------------------------
	{
		TObjectHdr tMeshHdr;
		g_pVfs->Read( pFile, &tMeshHdr, sizeof( TObjectHdr ) );
	}

	m_pShape = g_pEngine->CreateMesh();
	m_pShape->LoadFromStream( pFile );

	m_pBoneMatrices	= NEW mat4[ m_nNumObjects ];

	//--------------------------------------------------------------------------
	// Tip: Читаем объекты
	//--------------------------------------------------------------------------
	for ( uint nObj = 0; nObj < m_nNumObjects; ++nObj )
	{
		TObjectHdr tObjHdr;
		CObject * pObject = m_pObjects + nObj;

		// Читаем заголовок объекта
		if ( sizeof ( TObjectHdr ) != g_pVfs->Read( pFile, &tObjHdr, sizeof( TObjectHdr ) ) )
		{
			goto error_label;
		}

		pObject->SetParentName( tObjHdr.szParent );
		pObject->SetName( tObjHdr.szName );
		pObject->SetScale( tObjHdr.vScale );
		pObject->SetPos( tObjHdr.vPos );
		pObject->SetRot( tObjHdr.qRot );

		////////////////////////////////////////////////////////////////////////

		/*switch ( tObjHdr.eType )
		{
			default:
			{
				g_pVfs->Seek( pFile, tObjHdr.nShapeBlockSize, VFS_SEEK_CUR );
				continue;
			}
		}*/
	}

	RebuildHierarchy();
	g_pVfs->CloseFile( &pFile );
	return;

	//--------------------------------------------------------------------------
	// Tip: Поведение кода в случае ошибки при разборе файла
	//--------------------------------------------------------------------------
	error_label:

	m_nNumObjects = 0;
	g_pVfs->CloseFile( &pFile );
}


/*
*
*/
bool CModel::LoadMaterial( CXMLElement * pMaterial )
{
	CXMLAttr *		pNameAttr	= pMaterial->GetAttr( "Name" );
	const char *	szMatName	= ( NULL != pNameAttr ) ? pNameAttr->GetValue() : NULL;

	CXMLAttr *		pShader		= pMaterial->GetAttr( "Shader" );
	const char *	szShader	= ( NULL != pShader ) ? pShader->GetValue() : NULL;

	if ( !szMatName )
	{
		DEBUG_ASSERT( !"No material name!" );
		return false;
	}

	// Пробигаем по всем объектам модели, пытаемся найти
	// нужный материал по имени ( szName )
	//for ( uint n = 0; n < m_nNumObjects; ++n )
	{
		//CObject &	pObj	= m_pObjects[ n ];
		IMesh *		pMesh	= (IMesh *)m_pShape;

		IMaterial * pNewMaterial = g_pEngine->CreateMaterial();

		if ( !pNewMaterial )
		{
			// Ошибка выделения памяти
			return false;
		}

		m_pMaterial = pNewMaterial;
		pNewMaterial->SetShader( g_pRenderer->GetShader( szShader ) );

		for ( uint n = 0; n < pMesh->GetSurfCount(); ++n )
		{
			IMaterial *	pSubMat = NULL;

			if ( CStr::Compare( szMatName, pMesh->GetSurfName( n ) ) )
			{
				pSubMat = g_pEngine->CreateMaterial();

				if ( pSubMat )
				{
					//pSubMat->SetShader( g_pRenderer->GetShader( szShader ) );
					pSubMat->SetShader( g_pRenderer->GetShader( MESH_SHADER ) );
				}
			}

			pNewMaterial->AddSubMaterial( pSubMat );
		}

		DEBUG_ASSERT( pNewMaterial->GetSubCount() == pMesh->GetSurfCount() );
	}

	return true;
}



/*
*
*/
void CModel::Load( const char * szFile )
{
	CXMLElement * pModelXML = NULL;
	g_pXMLWrapper->LoadXML( szFile, &pModelXML );
	
	if ( !pModelXML )
		return;

	CXMLElement * pMeshXML = pModelXML->GetChild( "Mesh" );

	if ( pMeshXML )
	{
		CXMLAttr * pMeshAttr = pMeshXML->GetAttr( "File" );

		if ( pMeshAttr )
		{
			LoadObjects( pMeshAttr->GetValue() );
		}

		//----------------------------------------------------------------------
		// Tip: Читаем материалы
		//----------------------------------------------------------------------
		for ( uint n = 0; n < pMeshXML->GetNumChilds(); ++n )
		{
			LoadMaterial( pMeshXML->GetChild( n ) );
		}
	}

	//--------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------
	CXMLElement * pAnimXML = pModelXML->GetChild( "Animations" );

	if ( pAnimXML )
	{
		for ( uint n = 0; n < pAnimXML->GetNumChilds(); ++n )
		{
			CXMLElement * pCurAnimXML = pAnimXML->GetChild( n );

			CXMLAttr * pFileAttr = pCurAnimXML->GetAttr( "File" );
			CXMLAttr * pNameAttr = pCurAnimXML->GetAttr( "Name" );

			CAnim * pAnim = NEW CAnim;

			if ( pAnim->Load( pFileAttr->GetValue() ) )
			{
				pAnim->SetName( pNameAttr->GetValue() );
				AddAnim( pAnim );
				PlayAnim( "test_anim" );
				continue;
			}

			DEL( pAnim );
		}
	}

	DEL( pModelXML );
}

////////////////////////////////////////////////////////////////////////////////