////////////////////////////////////////////////////////////////////////////////

#ifndef __Scene_h_included__
#define __Scene_h_included__

////////////////////////////////////////////////////////////////////////////////

//#include "Camera.h"

////////////////////////////////////////////////////////////////////////////////

/*class CSceneObject
{
	public :

		inline CSceneObject()
		{
			float f = 0;

			m_vPos.Set( 0, 0, 0 );
			m_qRot.vDir.Set( 1, 0, 0 );
			m_qRot.fAngle = cosf( 0.5f * f * TO_RADIAN );
			m_qRot.vDir.Mult( sinf( 0.5f * f * TO_RADIAN ) );
		}

		inline CSceneObject( PMesh mesh ) :
				pMesh		( mesh		)
		{
		}

		~CSceneObject()
		{
			DEL( pMaterial );
			pMesh = NULL;
		}

		void Render() const;

		PMesh		pMesh;
		IMaterial * pMaterial;

		vec3 m_vPos;
		quat m_qRot;
};

////////////////////////////////////////////////////////////////////////////////
*
class CScene : public IScene
{
	public :

		CScene()
		{
			m_mProj.Projection( 34.516f, 4.f / 3.f, 1, 200 );
		}

		~CScene();
		//bool QueueObject( CSceneObject *pObject );
		//void Render();

		//----------------------------------------------------------------------
		// Tip: Inline methods
		//----------------------------------------------------------------------
		inline ICamera * GetCamera()
		{
			return &m_Camera;
		}

		inline const mat4 & GetViewMatrix() const
		{
			return m_mView;
		}

		inline const mat4 & GetProjMatrix() const
		{
			return m_mProj;
		}

		inline const mat4 & GetViewProjMatrix() const
		{
			return m_mViewProj;
		}

		inline void Update()
		{
			ICamera * pCam = GetCamera();
			pCam->BuildMatrix( m_mView );

			m_mViewProj.Identity();
			m_mViewProj.Mult( m_mView );
			m_mViewProj.Mult( m_mProj );
		}

	private :

		CCamera m_Camera;
		//CContainer <CSceneObject *> m_RenderQueue;

		mat4 m_mView;
		mat4 m_mProj;
		mat4 m_mViewProj;
};
*/
////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Scene_h_included__

////////////////////////////////////////////////////////////////////////////////