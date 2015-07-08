////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ModelAbility.h"


/*
*
*/
IModel * CEngine::CreateModel()
{
	return NEW CModel;
}


/*
*
*/
CModel::CModel() :
	m_nNumObjects	( 0		),
	m_pBoneMatrices	( NULL	),
	m_pObjects		( NULL	),
	m_pShape		( NULL	),
	m_pMaterial		( NULL	)
{
	tAnimState.pAnimBind = NULL;
}


/*
*
*/
CModel::~CModel()
{
	for ( uint n = 0; n < m_pAnims.GetCount(); ++n )
	{
		DEL( m_pAnims[ n ].pAnim );
		DEL_ARRAY( m_pAnims[ n ].pIndices );
	}

	m_pAnims.Delete();

	RELEASE( m_pShape );
	RELEASE( m_pMaterial );
	DEL_ARRAY( m_pObjects );
}


/*
*
*/
void CModel::Render( const mat4 & mWorld )
{
	if ( !m_pObjects )
		return;

	//----------------------------------------------------------------------
	//	Tip:
	//----------------------------------------------------------------------
	for ( uint n = 0; n < m_nNumObjects; ++n )
	{
		CObject * pObj = m_pObjects + n;

		pObj->RebuildMatrix( 0.f );

		if ( m_pBoneMatrices )
		{
			mat4 & mMatrix = m_pBoneMatrices[ n ];

			mMatrix = pObj->m_mInvMatrix;
			mMatrix.Mult( pObj->GetMatrix() );
//			mMatrix.Transpose();
		}
	}

	//----------------------------------------------------------------------
	//	Tip:
	//----------------------------------------------------------------------
	const CAnim * pAnim = tAnimState.pAnimBind->pAnim;

	for ( uint n = 0; n < m_nNumObjects; ++n )
	{
		CObject * pObj = m_pObjects + n;

		// update animations ///////////////////////////////////////////////////
		if ( tAnimState.pAnimBind )
		{
			const short		nIndex	= tAnimState.pAnimBind->pIndices[ n ];

			if ( INVALID_INDEX != nIndex )
			{
				quat q;

				if ( pAnim->GetQuat( q, nIndex, tAnimState.fTime ) )
					pObj->SetRotAnim( q );
			}
		}

		// render //////////////////////////////////////////////////////////////
		mat4 mMatrix	= pObj->GetMatrix();
		mMatrix.Mult( mWorld );

		// skeleton debug lines ////////////////////////////////////////////////
		g_pRenderer->Axis3D( mMatrix );

		CObject * pParent = pObj->GetParent();

		if ( pParent )
		{
			mat4 mParent = pParent->GetMatrix();

			vec3 vStart( 0, 0, 0 );
			vec3 vEnd( 0, 0, 0 );
			mParent.Mult( mWorld );

			vStart.Multiply( mMatrix );
			vEnd.Multiply( mParent );

			g_pEngine->PushLine( vStart, vEnd, 0xFFFFFFFF );
		}
	}

	if ( m_pShape )
	{
		if ( m_pBoneMatrices )
		{
			if ( m_pMaterial )
			{
				IShader * pShader = m_pMaterial->GetShader();

				CUniform uBones( "pBoneMatrices", pShader );
				uBones.Connect( m_pMaterial );
				uBones.SetValue( (vec4*)m_pBoneMatrices, m_nNumObjects * 4 );

				const mat4 & mVP = g_pEngine->GetCamera()->GetViewProjMatrix();

				mat4 mMVP( mWorld );
				mMVP.Mult( mVP );

				CUniform unfmMVP( "mMVP", pShader );
				unfmMVP.Connect( m_pMaterial );
				unfmMVP.SetValue( (vec4*)mMVP.pArray, 4 );
			}
		}

		m_pShape->Render( m_pMaterial );
	}

	if ( pAnim )
	{
		tAnimState.fTime = g_pEngine->GetTime() - tAnimState.fStartTime;
		tAnimState.fTime *= 30;

		if ( tAnimState.fTime > pAnim->m_fTime )
		{
			tAnimState.fStartTime = g_pEngine->GetTime();
			tAnimState.fTime = 0;
		}
	}
}


/*
*
*/
void CModel::RebuildHierarchy()
{
	//--------------------------------------------------------------------------
	// Tip: Назначаем каждому объекту родителя ( если есть )
	//--------------------------------------------------------------------------
	for ( uint n = 0; n < m_nNumObjects; ++n )
	{
		CObject & obj = m_pObjects[ n ];

		if ( obj.m_sParentName.GetSize() )
		{
			for ( uint m = 0; m < m_nNumObjects; ++m )
			{
				if ( n == m )
					continue;

				if ( obj.m_sParentName.Compare( m_pObjects[ m ].m_sName ) )
				{
					obj.SetParent( m_pObjects + m );
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Tip: Высчитываем инверсные матрицы
	//--------------------------------------------------------------------------
	for ( uint n = 0; n < m_nNumObjects; ++n )
	{
		CObject & obj = m_pObjects[ n ];

		obj.RebuildMatrix( 0.f );
		obj.m_mInvMatrix = obj.GetMatrix();
		obj.m_mInvMatrix.Inverse();

		if ( m_pBoneMatrices )
			m_pBoneMatrices[ n ] = obj.GetMatrix();
	}
}


/**
*
*/
uint CModel::GetAnimCount() const
{
	return m_pAnims.GetCount();
}


/**
*
*/
const CAnim * CModel::GetAnim( uint nAnim )
{
	if ( nAnim < m_pAnims.GetCount() )
	{
		return m_pAnims[ nAnim ].pAnim;
	}

	return NULL;
}


/**
*
*/
const CAnim * CModel::GetAnim( const char * szAnimName )
{
	for ( uint i = 0; i < GetAnimCount(); ++i )
	{
		const CAnim * pAnim = GetAnim( i );

		if ( CStr::Compare( pAnim->GetName(), szAnimName ) )
		{
			return pAnim;
		}
	}

	return NULL;
}


/**
*
*/
bool CModel::PlayAnim(	const char *	szAnimName,
						EAnimChannel	eChannel,
						bool			bLoop,
						bool			bForward,
						float			fSpeed,
						float			fBlendTime )
{
	for ( uint n = 0; n < m_pAnims.GetCount(); ++n )
	{
		if ( CStr::Compare( szAnimName, m_pAnims[ n ].pAnim->GetName() ) )
		{
			tAnimState.pAnimBind	= &m_pAnims[ n ];
			tAnimState.fStartTime	= g_pEngine->GetTime();
			tAnimState.fTime		= 0.f;
			return true;
		}
	}

	return false;
}


/**
*
*/
bool CAnim::Load( const char * szFileName )
{
	IFile * pFile;

	if ( R_OK != g_pVfs->OpenFile( &pFile, szFileName, VFS_BINARY | VFS_READ ) )
	{
		SysWrite( Va( "Failed to open anim file \"%s\"", szFileName ) );
		return false;
	}


	TAnimFileHdr tAnimHdr;
	g_pVfs->Read( pFile, &tAnimHdr, sizeof( TAnimFileHdr ) );

	m_nJointCount	= tAnimHdr.nNumObjects;
	m_fTime			= tAnimHdr.fAnimTime;

	m_pJoints = NEW TJoint[ m_nJointCount ];

	// Загрузка объектов и ключей анимации
	// Перебираем каждый объект
	for ( uint n = 0; n < m_nJointCount; ++n )
	{
		TJoint & tJoint = m_pJoints[ n ];

		g_pVfs->Read( pFile, &tJoint.tHeader, sizeof( TJointHdr ) );

		if ( tJoint.tHeader.nScaleKeyCount )
		{
			DEBUG_ASSERT( false );
		}

		if ( tJoint.tHeader.nRotKeyCount )
		{
			tJoint.pRotKeys = NEW CRotKey[ tJoint.tHeader.nRotKeyCount ];

			g_pVfs->Read(
				pFile,
				tJoint.pRotKeys,
				sizeof( CRotKey ) *  tJoint.tHeader.nRotKeyCount );
		}

		if ( tJoint.tHeader.nTransKeyCount )
		{
			DEBUG_ASSERT( false );
		}
	}

	g_pVfs->CloseFile( &pFile );

	return true;
}


bool CModel::AddAnim( const CAnim * pAnim )
{
	if ( pAnim )
	{
		TAnimBind tBind;
		tBind.pAnim = pAnim;
		tBind.pIndices = NEW short[ m_nNumObjects ];

		if ( tBind.pIndices )
		{
			// Выставляем значения по умолчанию
			// для индексов костей
			for ( uint n = 0; n < m_nNumObjects; ++n )
				tBind.pIndices[ n ] = INVALID_INDEX;

			for ( uint n = 0; n < pAnim->GetJointCount(); ++n )
			{
				for ( uint i = 0; i < m_nNumObjects; ++i )
				{
					if ( CStr::Compare(
						m_pObjects[ i ].GetName(),
						pAnim->GetJointName( n ) ) )
					{
						tBind.pIndices[ i ] = n;
						break;
					}
				}
			}

			m_pAnims.AddToTail( tBind );
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////