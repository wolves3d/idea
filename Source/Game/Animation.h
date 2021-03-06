////////////////////////////////////////////////////////////////////////////////


class CAnimation
{

public:

	CAnimation();
	bool Load( const char *szFile );
	void Render();
	void Play();

private:

	dword	m_dwStart;
	uint	m_nCurTex;
	CStack <PTexture>	m_Textures;
	ISprite2D *			m_pSprite;
};

////////////////////////////////////////////////////////////////////////////////