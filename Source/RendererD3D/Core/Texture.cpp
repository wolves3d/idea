////////////////////////////////////////////////////////////////////////////////

#include "Common.h"


//!

//!
//!
class CTexture : public ITexture
{
	public :

	inline CTexture() :
		m_pTex		( NULL ),
		m_pResView	( NULL )
	{
	}

	~CTexture()
	{
	}


	//!

	//!
	//!
	EResult Init( const TImage & tImage, EAccessType eAccess )
	{
		D3D10_TEXTURE2D_DESC tex_desc;
		ZeroMemory( &tex_desc, sizeof( D3D10_TEXTURE2D_DESC ) );

		switch ( eAccess )
		{
			case EAT_IMMUTABLE:
			{
				tex_desc.Usage = D3D10_USAGE_IMMUTABLE;

				if ( NULL == tImage.pData )
				{
					return R_INVALID_ARG;
				}
			}
			break;

			case EAT_CPU_WRITE:
			{
				tex_desc.Usage = D3D10_USAGE_DYNAMIC;
				tex_desc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE ;
			}
			break;

			default:
				tex_desc.Usage = D3D10_USAGE_DEFAULT;
		}

		tex_desc.Width		= tImage.nWidth;
		tex_desc.Height		= tImage.nHeight;
		tex_desc.MipLevels	= 1; // Use 1 for a multisampled texture; or 0 to generate a full set of subtextures. 
		tex_desc.ArraySize	= 1;
		tex_desc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags	= D3D10_BIND_SHADER_RESOURCE;
		tex_desc.SampleDesc.Count	= 1;
		tex_desc.SampleDesc.Quality	= 0;

		D3D10_SUBRESOURCE_DATA res_data;
		ZeroMemory( &res_data, sizeof( D3D10_SUBRESOURCE_DATA ) );

		res_data.SysMemPitch		= tex_desc.Width * 4; // 4 for byte RGBA
		res_data.SysMemSlicePitch	= tImage.nSize;
		res_data.pSysMem			= tImage.pData;

		HRESULT hRes = g_pDevice->CreateTexture2D(
			&tex_desc,
			&res_data,
			&m_pTex );

		if ( FAILED( hRes ) )
		{
			return R_GENERIC_ERROR;
		}

		D3D10_SHADER_RESOURCE_VIEW_DESC res_desc;
		ZeroMemory( &res_desc, sizeof( D3D10_SHADER_RESOURCE_VIEW_DESC ) );

		res_desc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		res_desc.ViewDimension	= D3D10_SRV_DIMENSION_TEXTURE2D;

		res_desc.Texture2D.MipLevels		= 1;
		res_desc.Texture2D.MostDetailedMip	= 0;

		hRes = g_pDevice->CreateShaderResourceView(
			m_pTex,
			&res_desc,
			&m_pResView );

		return R_OK;
	}


	//!

	//!
	//!
	void Release()
	{
		DecRefCount();

		if ( 0 == GetRefCount() )
		{
			PURE_DEL( this );
		}
	}

	EResult SetRef( PTexture pRefTexture )
	{
		if ( pRefTexture )
		{
			if ( pRefTexture->GetRef() )
				return R_INVALID_ARG;
		}

		m_pRefTexture = pRefTexture;
		return R_OK;
	}

	PTexture GetRef()
	{
		return m_pRefTexture;
	}

	EResult SetData( const TImage & pImage );
	EResult	SetFilter( TextureFormat eFilter );

	EResult	Bind()
	{
		if ( m_pRefTexture )
			return m_pRefTexture->Bind();

		if ( m_pTex )
		{
			g_pDevice->PSSetShaderResources(
				0, // ÃÄÅ ÂÇßÒÜ?! ÒÓÒ -> D3D10ReflectShader
				1,
				&m_pResView );

			return R_OK;
		}

		return R_INVALID_OP;
	}

	inline const char * GetName() const
	{
		return m_sName.GetString();
	}

	uint GetRefCount() const
	{
		return m_nRefCount;
	}

	void IncRefCount()
	{
		m_nRefCount++;
	}

	void DecRefCount()
	{
		DEBUG_ASSERT( 0 != m_nRefCount )

			if ( m_nRefCount > 0 )
			{
				m_nRefCount--;
			}
	}

//private :

	ID3D10Texture2D	*			m_pTex;
	ID3D10ShaderResourceView *	m_pResView;

	CStr		m_sName;
	uint		m_nRefCount;
	PTexture	m_pRefTexture;
};


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CTexture::SetFilter( TextureFormat eFilter )
{
	/*
	GLint iMinFilter;

	switch ( eFilter )
	{
	case FILTER_NEAREST:
		iMinFilter = GL_NEAREST;
		break;

	case FILTER_LINEAR:
		iMinFilter = GL_LINEAR;
		break;

	default :
		return R_INVALID_ARG;
	}
*/
	//glBindTexture	( GL_TEXTURE_2D, m_nTextureID );
	//glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iMinFilter );
	//glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iMinFilter );
	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult CTexture::SetData( const TImage & pImage )
{
	if ( TImage::IMG_FMT_RGBA8 != pImage.eFormat )
		return R_INVALID_ARG;

	D3D10_MAPPED_TEXTURE2D map_tex;
	HRESULT hRes = m_pTex->Map( 0, D3D10_MAP_WRITE_DISCARD, 0, &map_tex );

	if ( SUCCEEDED ( hRes ) )
	{
		memcpy( map_tex.pData, pImage.pData, pImage.nSize );
		m_pTex->Unmap( 0 );

		D3D10_SHADER_RESOURCE_VIEW_DESC res_desc;
		ZeroMemory( &res_desc, sizeof( D3D10_SHADER_RESOURCE_VIEW_DESC ) );

		res_desc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		res_desc.ViewDimension	= D3D10_SRV_DIMENSION_TEXTURE2D;

		res_desc.Texture2D.MipLevels		= 1;
		res_desc.Texture2D.MostDetailedMip	= 0;

		HRESULT hRes = g_pDevice->CreateShaderResourceView(
			m_pTex,
			&res_desc,
			&m_pResView );

		hRes = 0;
	}

	return R_OK;
}


/*
================================================================================
//	Name:
//	Desc:
================================================================================
*/
EResult	CRendererD3D::CreateTexture( PTexture & pOutTexture, const char * szName )
{
	if ( !szName )
		return R_INVALID_ARG;

	CTexture * pLocalTexture = NEW CTexture;

	if ( !pLocalTexture )
		return R_OUT_OF_MEMORY;
/*
	
*/
	pLocalTexture->m_sName = szName;
	pOutTexture = pLocalTexture;
	return R_OK;
}

////////////////////////////////////////////////////////////////////////////////