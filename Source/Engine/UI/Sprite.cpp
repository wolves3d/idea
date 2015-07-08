////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UIManager.h"


/**
*
*/
class CSprite2D : public ISprite2D
{
	public:

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	CSprite2D( uint nOffset, uint nID ) :
		m_bVisible	( true	),
		m_pMaterial	( NULL	),
		m_nOffset	( nOffset ),
		m_nID		( nID )
	{
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	~CSprite2D()
	{
		SetMaterial( NULL );
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline const vec2 & GetPos() const
	{
		TSpriteVertex * pVertices = g_pUIManager->GetSpritesData() + m_nOffset;

		return pVertices->vPos;
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline void SetPos( float x, float y )
	{
		TSpriteVertex * pVertices = g_pUIManager->GetSpritesData();

		if ( pVertices )
		{
			pVertices += m_nOffset;

			vec2 vOffset(
				x - pVertices->vPos.x,
				y - pVertices->vPos.y );
			
			pVertices->vPos.Add( vOffset );
			pVertices++;

			pVertices->vPos.Add( vOffset );
			pVertices++;

			pVertices->vPos.Add( vOffset );
			pVertices++;

			pVertices->vPos.Add( vOffset );
		}
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline const vec2 GetSize() const
	{
		TSpriteVertex * pVertices = g_pUIManager->GetSpritesData() + m_nOffset;

		return vec2(
			pVertices[ 2 ].vPos.x - pVertices[ 0 ].vPos.x,
			pVertices[ 2 ].vPos.y - pVertices[ 0 ].vPos.y );
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline void SetSize( float fWidth, float fHeight )
	{
		TSpriteVertex * pVertices = g_pUIManager->GetSpritesData();

		if ( pVertices )
		{
			pVertices += m_nOffset;

			vec2 & vPos = pVertices->vPos;
			pVertices++;

			pVertices->vPos.Set( vPos.x + fWidth, vPos.y );
			pVertices++;

			pVertices->vPos.Set( vPos.x + fWidth, vPos.y + fHeight );
			pVertices++;

			pVertices->vPos.Set( vPos.x, vPos.y + fHeight );
		}
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline bool IsVisible() const
	{
		return m_bVisible;
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline void SetMaterial( IMaterial * pMaterial )
	{
		m_pMaterial = pMaterial;
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline IMaterial * GetMaterial()
	{
		return m_pMaterial;
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	void SetUVs( const vec4 & vUV )
	{
		TSpriteVertex * pVertices = g_pUIManager->GetSpritesData();

		if ( pVertices )
		{
			pVertices += m_nOffset;

			pVertices->vUV.Set( vUV.x, vUV.w );
			pVertices++;

			pVertices->vUV.Set( vUV.z, vUV.w );
			pVertices++;

			pVertices->vUV.Set( vUV.z, vUV.y );
			pVertices++;

			pVertices->vUV.Set( vUV.x, vUV.y );
		}
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline const vec4 GetUVs()
	{
		return vec4( 0, 0, 0, 0 );
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	inline uint	GetFirstVertex() const
	{
		return m_nOffset;
	}

	inline uint	GetID() const
	{
		return m_nID;
	}

	private:

	uint		m_nOffset;
	uint		m_nID;
	bool		m_bVisible;
	IMaterial * m_pMaterial;
};


/**
*
*/
void CUIManager::InitSprites()
{
	const uint nMaxIndices = MAX_SPRITES * 6;
	m_nMaxVertices = MAX_SPRITES * 4;

	m_pSpriteVrts		= NEW TSpriteVertex [ m_nMaxVertices ];
	m_pSpriteIndices	= NEW short [ nMaxIndices ];

	if ( m_pSpritesVB = g_pRenderer->CreateVB() )
	{
		m_nMaxVertices = MAX_SPRITES * 4;
		m_pSpritesVB->Alloc( m_nMaxVertices * ( sizeof( vec2 ) + sizeof( vec2 ) ), 0 );

		for ( uint n = 0; n < m_nMaxVertices; ++n )
		{
			m_pSpriteVrts[ n ].vPos.Set( 0, 0 );
		}
	}

	if ( m_pSpriteIB = g_pRenderer->CreateIB() )
	{
		m_pSpriteIB->Alloc( nMaxIndices * sizeof( short ), 0 );
		m_pSpriteIB->SetAttrs( nMaxIndices, TYPE_SHORT );
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	m_FreeSpriteIDs.Alloc( MAX_SPRITES, false );

	for ( uint n = 0, nOffset = 0; n < MAX_SPRITES; n++ )
	{
		CSprite2D * pSprite = NEW CSprite2D( nOffset, n );

		if ( pSprite )
		{
			m_pSprites[ n ] = pSprite;

			pSprite->SetUVs( vec4( 0, 0, 1, 1 ) );
			nOffset += 4;

			m_FreeSpriteIDs.Push( n );
		}
	}
}


/**
*
*/
ISprite2D * CUIManager::CreateSprite()
{
	uint nFreeSptriteID;
	
	if ( m_FreeSpriteIDs.Pop( nFreeSptriteID ) )
	{
		return m_pSprites[ nFreeSptriteID ];
	}

	DEBUG_MSG( "no free sprites available!" );
	return NULL;
}


/**
*
*/
void CUIManager::DeleteSprite( ISprite2D * pSprite )
{
	if ( pSprite->GetID() <= MAX_SPRITES )
	{
		m_FreeSpriteIDs.Push( pSprite->GetID() );
		return;
	}

	DEBUG_MSG( DBG_STR_INVALID_ID )
}


/**
*
*/
void CUIManager::PushSprite( ISprite2D * pSprite, bool bDrawBeforeUI )
{
	//--------------------------------------------------------------------------
	//	Tip: Проверка аргументов
	//--------------------------------------------------------------------------
	if ( NULL == pSprite )
	{
		DEBUG_MSG( "bad sprite pointer!" );
		return;
	}

	if ( !pSprite->IsVisible() )
		return;

	IMaterial * pMat = pSprite->GetMaterial();

	if ( NULL == pMat )
	{
		DEBUG_MSG( "bad sprite material!" );
		return;
	}

	//--------------------------------------------------------------------------
	//	Tip:
	//--------------------------------------------------------------------------
	TSpriteDIP *	pPrevDIP	= NULL;
	TSpriteDIP *	pCurDIP		= NULL;
	uint			nItemCount	= m_pSpriteDIPs.GetCount();

	if ( 0 == nItemCount )
	{
		TSpriteDIP tDIP;

		tDIP.pMaterial		= pMat;
		tDIP.nIndexOffset	= 0;
		tDIP.nIndexCount	= 0;

		m_pSpriteDIPs.AddToTail( tDIP );
		m_pSpriteDIPcount->IncValue();
		nItemCount++;
	}

	// Настраиваем указатель на инфо о последнем DIP
	pPrevDIP = &m_pSpriteDIPs[ nItemCount - 1 ];

	// Если, материал предыдущего DIP'а не совпадает с текущим спрайтом, то
	// необходимо создать новый DIP
	if ( pMat != pPrevDIP->pMaterial )
	{
		TSpriteDIP tDIP;

		tDIP.pMaterial		= pMat;
		tDIP.nIndexOffset	= pPrevDIP->nIndexOffset + pPrevDIP->nIndexCount;
		tDIP.nIndexCount	= 0;

		m_pSpriteDIPs.AddToTail( tDIP );
		m_pSpriteDIPcount->IncValue();
		pCurDIP = &m_pSpriteDIPs[ nItemCount ];
	}
	else
	{
		pCurDIP = pPrevDIP;
	}

	//--------------------------------------------------------------------------
	//	Tip: Добавляем спрайт в IndexBuffer
	//--------------------------------------------------------------------------
	short * pIndices	= m_pSpriteIndices + pCurDIP->nIndexOffset + pCurDIP->nIndexCount;
	int		nVertex		= pSprite->GetFirstVertex();

	pIndices[ 0 ]	= nVertex;
	pIndices[ 1 ]	= nVertex + 1;
	pIndices[ 2 ]	= nVertex + 2;
	pIndices[ 3 ]	= nVertex;
	pIndices[ 4 ]	= nVertex + 2;
	pIndices[ 5 ]	= nVertex + 3;

	pCurDIP->nIndexCount += 6;

	////////////////////////////////////////////////////////////////////////////

	m_pSpriteCount->IncValue();
}


int g = 0;

/**
* Render all sprites
*/
void CUIManager::RenderSprites()
{
	//--------------------------------------------------------------------------
	// Tip: Копирование VertexBuffer'а из системной памяти в видео
	//--------------------------------------------------------------------------
	if ( m_pSpritesVB )
	{
		const uint	nSizeVB	= MAX_SPRITES * 4 * sizeof( TSpriteVertex );
		void *		pVrts	= m_pSpritesVB->Lock( 0, nSizeVB );
		
		if ( pVrts )
		{
			memcpy( pVrts, m_pSpriteVrts, nSizeVB );
			m_pSpritesVB->Unlock();
		}
	}

	//--------------------------------------------------------------------------
	// Tip: Копирование IndexBuffer'а из системной памяти в видео
	//--------------------------------------------------------------------------
	if ( m_pSpriteIB )
	{
		const uint	nSizeIB	= MAX_SPRITES * 6 * sizeof ( short );
		void *		pIndсs	= m_pSpriteIB->Lock( 0, nSizeIB );

		if ( pIndсs )
		{
			memcpy( pIndсs, m_pSpriteIndices, nSizeIB );
			m_pSpriteIB->Unlock();
		}
	}

	//--------------------------------------------------------------------------
	// Tip:
	//--------------------------------------------------------------------------
	mat4 mWVP( m_mWorld );
	mWVP.Mult( m_mProj );

	// Прикрепляем myBuffer к шейдеру
	CUniform myBuffer;
	myBuffer.AttachToShader( "mMVP", g_pRenderer->GetShader( UI_SHADER ) );

	//--------------------------------------------------------------------------
	// Tip: Отрисовываем каждый DIP
	//--------------------------------------------------------------------------
	if ( m_pSpritesVB && m_pSpriteIB )
	{
		for ( uint n = 0; n < m_pSpriteDIPs.GetCount(); ++n )
		{
			TSpriteDIP & tDIP = m_pSpriteDIPs[ n ];

			IShader * pMatShd = tDIP.pMaterial->GetShader();

//			if ( pMatShd )
			{
				// Соединяем myBuffer с текущим материалом
				myBuffer.Connect( tDIP.pMaterial );
				myBuffer.SetValue( (vec4 *)mWVP.pArray, 4 );

				tDIP.pMaterial->Assign();

				if ( pMatShd )
				{
					m_pSpritesVB->RenderIndexed(
						pMatShd->GetVertexDecl(),
						m_pSpriteIB,
						PRIM_TRIANGLE,
						tDIP.nIndexOffset,
						tDIP.nIndexCount );
				}
			}
		}
	}

	// Очищаем очередь DIP'ов
	m_pSpriteDIPs.Clear();
}

////////////////////////////////////////////////////////////////////////////////