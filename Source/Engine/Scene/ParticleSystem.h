class CParticleSystem : public IParticleSystem
{
	public:

		CParticleSystem();
		~CParticleSystem();
		void Init();
		void Update();
		void Render( IMaterial * pMaterial );

	private:

		void Rebuild();
		void EmitParticles( size_t nNumParticles );

		struct TParticle
		{
			vec3	vPos;
			vec3	vSpeed;
			vec3	vAccel;
			float	fScale;
			float	fLifeTime;
			float	fBornTime;
			dword	dwColor;
		};


		PVertexBuffer	m_pVB;
		byte *			m_pSysMemVB;
		size_t	m_nNumParticles;
		float	m_fLastTime;

		float	m_fRate;
		float	m_fTimeAccum;

		TParticle *		m_pParticles;
		CStack <uint>	m_Free;
};